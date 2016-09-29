/* OS-agnostic User Datagram Protocol (UDP) sockets.
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
#include <utility>
#include <cstddef>
#include <cstring>
#include <cerrno>


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>


#include <libnet/ip.hpp>
#include <libnet/udp.hpp>


struct internal_implementation : public libnet::udp_socket::internal
{
	int                        socket_file;
	struct sockaddr_in6        cache_source_address;
	std::vector<unsigned char> cache_datagram;
	bool                       is_cache_ready;


	internal_implementation()
	: is_cache_ready( false )
	{
		if( ( this->socket_file = socket( AF_INET6, SOCK_DGRAM, 0 ) ) < 0 )
		{
			throw std::system_error( errno, std::system_category() );
		}
	}


	~internal_implementation()
	{
		close( this->socket_file );
	}
};


static void initialize_socket
(
	struct internal_implementation& internal,
	const struct sockaddr_in6&      in6_addr,
	bool                            allow_ipv4
)
{
	int ipv6_only = allow_ipv4 ? 0 : 1;
	int fcntl_flags;

	if
	( setsockopt( internal.socket_file, IPPROTO_IPV6, IPV6_V6ONLY, &ipv6_only, sizeof( int ) ) < 0
	  || bind( internal.socket_file, reinterpret_cast<const struct sockaddr*>( &in6_addr ),
	           sizeof( struct sockaddr_in6 )
	         ) < 0
	)
	{
		throw std::system_error( errno, std::system_category() );
	}
}


static libnet::ipv6_socket_address normalize_address
(
	const struct sockaddr_in6& in6_addr
)
{
	using namespace libnet;

	raw_ipv6_address raw_address;
	for( unsigned i = 0; i < 8; i++ )
	{
		raw_address[ i ] = ( static_cast<std::uint16_t>( in6_addr.sin6_addr.s6_addr[ i * 2 ] ) << 8 )
		                   | in6_addr.sin6_addr.s6_addr[ i * 2 + 1 ];
	}

	return ipv6_socket_address( raw_address, ntohs( in6_addr.sin6_port ) );
}


static struct sockaddr_in6 denormalize_address
(
	const libnet::ipv6_socket_address& address
)
{
	using namespace libnet;

	struct sockaddr_in6 in6_addr;
	std::memset( &in6_addr, 0, sizeof( struct sockaddr_in ) );

	in6_addr.sin6_family = AF_INET6;
	in6_addr.sin6_port = htons( address.get_port() );
	for( unsigned i = 0; i < 16; i++ )
	{
		std::uint16_t field = address.get_address().as_raw()[ i / 2 ];
		in6_addr.sin6_addr.s6_addr[ i ] = i % 2 == 0 ? field >> 8 : field & 0xFF;
	}

	return in6_addr;
}


static bool try_receive
(
	int                         socket,
	std::vector<unsigned char>& datagram,
	bool                        non_blocking,
	struct sockaddr_in6&        source_address
)
{
	using namespace libnet;

	datagram.resize( maximum_udp_data_length );

	ssize_t ret;
	socklen_t source_address_length = sizeof( struct sockaddr_in6 );
	if
	( ( ret = recvfrom( socket, datagram.data(), maximum_udp_data_length, non_blocking ? MSG_DONTWAIT : 0,
	                    reinterpret_cast<struct sockaddr*>( &source_address ), &source_address_length
	                  )
	  ) < 0
	)
	{
		switch( errno )
		{
#if EAGAIN != EWOULDBLOCK
			case EAGAIN:
#endif
			case EWOULDBLOCK:
				return false;

			default:
				throw std::system_error( errno, std::system_category() );
		}
	} else if( ret > maximum_udp_data_length )
	{
		// I'm not going to support IPv6 jumbograms anytime soon.
		return false;
	}

	datagram.resize( ret );
	return true;
}


libnet::udp_socket::udp_socket
(
	const libnet::ipv4_socket_address& address
)
: socket_internal( new internal_implementation )
{
	struct internal_implementation& internal = static_cast<struct internal_implementation&>( *this->socket_internal );
	struct sockaddr_in6 in6_addr = denormalize_address( ipv6_socket_address( address.get_address(), address.get_port() ) );

	initialize_socket( internal, in6_addr, true );
}


libnet::udp_socket::udp_socket
(
	const libnet::ipv6_socket_address& address,
	bool                               allow_ipv4
)
: socket_internal( new internal_implementation )
{
	struct internal_implementation& internal = static_cast<struct internal_implementation&>( *this->socket_internal );
	struct sockaddr_in6 in6_addr = denormalize_address( address );

	initialize_socket( internal, in6_addr, allow_ipv4 );
}


bool libnet::udp_socket::is_ready()
{
	struct internal_implementation& internal = static_cast<struct internal_implementation&>( *this->socket_internal );
	if( internal.is_cache_ready )
	{
		return true;
	}

	if( try_receive( internal.socket_file, internal.cache_datagram, true, internal.cache_source_address ) )
	{
		internal.is_cache_ready = true;
		return true;
	} else
	{
		return false;
	}
}


bool libnet::udp_socket::receive
(
	libnet::ipv6_packet& packet,
	bool                 non_blocking
)
{
	struct internal_implementation& internal = static_cast<struct internal_implementation&>( *this->socket_internal );
	if( internal.is_cache_ready )
	{
		packet = ipv6_packet( normalize_address( internal.cache_source_address ), std::move( internal.cache_datagram ) );
		internal.is_cache_ready = false;
	} else
	{
		struct sockaddr_in6 in6_addr;
		std::vector<unsigned char> payload;

		size_t source_address_length = sizeof( struct sockaddr_in6 );
		ssize_t ret;
		if( !try_receive( internal.socket_file, payload, non_blocking, in6_addr ) )
		{
			return false;
		}

		packet = ipv6_packet( normalize_address( in6_addr ), std::move( payload ) );
	}

	return true;
}


void libnet::udp_socket::send
(
	const libnet::ipv6_packet& packet
)
{
	struct internal_implementation& internal = static_cast<struct internal_implementation&>( *this->socket_internal );
	struct sockaddr_in6 in6_addr = denormalize_address( packet.get_peer_address() );
	if
	( sendto( internal.socket_file, packet.get_payload().data(), packet.get_payload().size(), 0,
	          reinterpret_cast<const struct sockaddr*>( &in6_addr ), sizeof( struct sockaddr_in6 )
	        ) < 0
	)
	{
		throw std::system_error( errno, std::system_category() );
	}
}


libnet::ipv6_socket_address libnet::udp_socket::get_address() const
{
	const struct internal_implementation& internal = static_cast<struct internal_implementation&>( *this->socket_internal );

	socklen_t address_length = sizeof( struct sockaddr_in6 );
	struct sockaddr_in6 in6_addr;
	if
	( getsockname( internal.socket_file, reinterpret_cast<struct sockaddr*>( &in6_addr ),
	               &address_length
	             ) < 0
	)
	{
		throw std::system_error( errno, std::system_category() );
	}

	return normalize_address( in6_addr );
}
