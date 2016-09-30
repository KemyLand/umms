/* TCP support.
 *
 * Copyright (C) 2016: Alejandro Soto
 *
 * This file is part of UMMS.
 *
 * UMMS is free software; you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software  Foundation;
 * either version 3 of the License, or (at your opinion), any later version.
 *
 * UMMS is distributed in the hope that it will be useful, but WITHOUT  ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with UMMS;
 * if not, see <http://www.gnu.org/licenses/>.
 */


#include <memory>
#include <system_error>
#include <vector>
#include <cstddef>
#include <cerrno>


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>


#include <libnet/ip.hpp>
#include <libnet/tcp.hpp>
#include <libnet/posix.hpp>


namespace
{
	class internal_implementation
	: public libnet::tcp_active_socket::internal, public libnet::tcp_passive_socket::internal
	{
		private:
			int                 socket_file;
			bool                closed;
			struct sockaddr_in6 remote_address;


		public:
			internal_implementation
			(
				bool allow_ipv4
			)
			: closed( false )
			{
				int ipv6_only = allow_ipv4 ? 0 : 1;
				if
				( ( this->socket_file = socket( AF_INET6, SOCK_STREAM, 0 ) ) < 0
				  || setsockopt( this->socket_file, IPPROTO_IPV6, IPV6_V6ONLY, &ipv6_only, sizeof( int ) )
				)
				{
					throw std::system_error( errno, std::system_category() );
				}
			}


			inline internal_implementation
			(
				int                  socket_file,
				struct sockaddr_in6& remote_address
			)
			: socket_file    ( socket_file    ),
			  closed         ( false          ),
			  remote_address ( remote_address )
			{}


		private:
			void check_not_closed()
			{
				if( this->closed )
				{
					throw std::logic_error( "Invalid I/O operation on closed TCP socket" );
				}
			}


		public:
			void close()
			{
				if( !this->closed )
				{
					::close( this->socket_file );
					this->closed = true;
				}
			}


			libnet::ipv6_socket_address get_local_address() const
			{
				socklen_t address_length = sizeof( struct sockaddr_in6 );
				struct sockaddr_in6 in6_addr;
				if
				( getsockname( this->socket_file, reinterpret_cast<struct sockaddr*>( &in6_addr ),
				               &address_length
				             ) < 0
				)
				{
					throw std::system_error( errno, std::system_category() );
				}

				return libnet::normalize_address( in6_addr );
			}


			libnet::ipv6_socket_address get_remote_address() const
			{
				return libnet::normalize_address( this->remote_address );
			}


			void bind_listen
			(
				const libnet::ipv6_socket_address& address
			)
			{
				struct sockaddr_in6 in6_addr = libnet::denormalize_address( address );
				if
				( ::bind( this->socket_file, reinterpret_cast<struct sockaddr*>( &in6_addr ),
				          sizeof( struct sockaddr_in6 )
				        ) < 0
				  || ::listen( this->socket_file, 128 ) < 0
				)
				{
					throw std::system_error( errno, std::system_category() );
				}
			}


			void connect
			(
				const libnet::ipv6_socket_address& peer
			)
			{
				struct sockaddr_in6 in6_addr = libnet::denormalize_address( peer );
				if
				( ::connect( this->socket_file, reinterpret_cast<struct sockaddr*>( &in6_addr ),
				             sizeof( struct sockaddr_in6 )
				           ) < 0
				)
				{
					throw std::system_error( errno, std::system_category() );
				}

				this->remote_address = libnet::denormalize_address( peer );
			}


			std::unique_ptr<libnet::tcp_active_socket::internal> accept()
			{
				this->check_not_closed();

				socklen_t address_length = sizeof( struct sockaddr_in6 );

				int active_socket_file;
				struct sockaddr_in6 in6_addr;
				if
				( ( active_socket_file = ::accept( this->socket_file,
				                                   reinterpret_cast<struct sockaddr*>( &in6_addr ),
				                                   &address_length
				                                 )
				  ) < 0
				)
				{
					throw std::system_error( errno, std::system_category() );
				}

				return std::unique_ptr<libnet::tcp_active_socket::internal>
				(
					new internal_implementation( active_socket_file, in6_addr )
				);
			}


			bool receive
			(
				std::vector<unsigned char>& buffer,
				std::size_t                 bytes
			)
			{
				this->check_not_closed();

				buffer.resize( bytes );

				ssize_t read;
				if( ( read = ::read( this->socket_file, buffer.data(), bytes ) ) < 0 )
				{
					throw std::system_error( errno, std::system_category() );
				} else if( read == 0 )
				{
					return false;
				}

				buffer.resize( read );
				return true;
			}


			void send
			(
				const void* what,
				std::size_t bytes
			)
			{
				this->check_not_closed();

				const unsigned char* base = static_cast<const unsigned char*>( what );
				while( bytes > 0 )
				{
					ssize_t wrote;
					if( ( wrote = ::write( this->socket_file, base, bytes ) ) < 0 )
					{
						throw std::system_error( errno, std::system_category() );
					}

					bytes -= wrote;
					base += wrote;
				}
			}
	};
}


libnet::tcp_passive_socket::tcp_passive_socket
(
	const libnet::ipv4_socket_address& address
)
: socket_internal( new internal_implementation( true ) )
{
	internal_implementation* internal = static_cast<internal_implementation*>( &*this->socket_internal );
	internal->bind_listen( ipv6_socket_address( address.get_address(), address.get_port() ) );
}


libnet::tcp_passive_socket::tcp_passive_socket
(
	const libnet::ipv6_socket_address& address,
	bool                               allow_ipv4
)
: socket_internal( new internal_implementation( allow_ipv4 ) )
{
	internal_implementation* internal = static_cast<internal_implementation*>( &*this->socket_internal );
	internal->bind_listen( address );
}


libnet::tcp_active_socket libnet::tcp_connect
(
	const libnet::ipv4_socket_address& address
)
{
	internal_implementation* internal = new internal_implementation( true );
	internal->connect( ipv6_socket_address( address.get_address(), address.get_port() ) );

	return tcp_active_socket( std::unique_ptr<tcp_active_socket::internal>( internal ) );
}


libnet::tcp_active_socket libnet::tcp_connect
(
	const libnet::ipv6_socket_address& address
)
{
	internal_implementation* internal = new internal_implementation( true );
	internal->connect( address );

	return tcp_active_socket( std::unique_ptr<tcp_active_socket::internal>( internal ) );
}
