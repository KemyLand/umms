#ifndef LIBNET_TCP_HPP
#define LIBNET_TCP_HPP


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
#include <utility>
#include <vector>
#include <cstddef>


#include <libnet/ip.hpp>


namespace libnet
{
	class tcp_active_socket
	{
		public:
			class internal
			{
				public:
					virtual ~internal() = 0;


					virtual void close() = 0;


					virtual bool receive
					(
						std::vector<unsigned char>& buffer,
						std::size_t                 bytes
					) = 0;


					virtual void send
					(
						const void* what,
						std::size_t bytes
					) = 0;


					virtual ipv6_socket_address get_local_address  () const = 0;
					virtual ipv6_socket_address get_remote_address () const = 0;
			};


		private:
			std::unique_ptr<internal> socket_internal;


			friend class tcp_passive_socket;


			friend tcp_active_socket tcp_connect
			(
				const ipv4_socket_address& peer
			);


			friend tcp_active_socket tcp_connect
			(
				const ipv6_socket_address& peer
			);


			inline tcp_active_socket
			(
				std::unique_ptr<internal>&& socket_internal
			)
			: socket_internal( std::move( socket_internal ) )
			{}


		public:
			tcp_active_socket
			(
				const tcp_active_socket&
			) = delete;


			tcp_active_socket
			(
				tcp_active_socket&&
			) = default;


			inline ~tcp_active_socket()
			{
				this->close();
			}


			inline void close()
			{
				this->socket_internal->close();
			}


			inline bool receive
			(
				std::vector<unsigned char>& buffer,
				std::size_t                 bytes
			)
			{
				this->socket_internal->receive( buffer, bytes );
			}


			inline void send
			(
				const void* what,
				std::size_t bytes
			)
			{
				this->socket_internal->send( what, bytes );
			}


			inline ipv6_socket_address get_local_address() const
			{
				return this->socket_internal->get_local_address();
			}


			inline ipv6_socket_address get_remote_address() const
			{
				return this->socket_internal->get_remote_address();
			}
	};


	inline tcp_active_socket::internal::~internal() {}


	class tcp_passive_socket
	{
		public:
			class internal
			{
				public:
					virtual ~internal() = 0;


					virtual void                                         close             () = 0;
					virtual std::unique_ptr<tcp_active_socket::internal> accept            () = 0;
					virtual ipv6_socket_address                          get_local_address () const = 0;
			};


		private:
			std::unique_ptr<internal> socket_internal;


		public:
			tcp_passive_socket
			(
				const tcp_passive_socket&
			) = delete;


			tcp_passive_socket
			(
				tcp_passive_socket&&
			) = default;


			tcp_passive_socket
			(
				const ipv4_socket_address& address
			);


			tcp_passive_socket
			(
				const ipv6_socket_address& address,
				bool                       allow_ipv4 = true
			);


			inline ~tcp_passive_socket()
			{
				this->close();
			}


			inline void close()
			{
				this->socket_internal->close();
			}


			inline tcp_active_socket accept()
			{
				return tcp_active_socket( this->socket_internal->accept() );
			}


			inline ipv6_socket_address get_local_address() const
			{
				return this->socket_internal->get_local_address();
			}
	};


	tcp_active_socket tcp_connect
	(
		const ipv4_socket_address& peer
	);


	tcp_active_socket tcp_connect
	(
		const ipv6_socket_address& peer
	);
}


#endif
