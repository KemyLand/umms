#ifndef LIBNET_UDP_HPP
#define LIBNET_UDP_HPP


/* UDP library for UMMS' use.
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
#include <vector>
#include <string>
#include <cstddef>


#include <libnet/ip.hpp>


namespace libnet
{
	class udp_socket
	{
		public:
			class internal
			{
				public:
					virtual ~internal() = 0;
			};


		private:
			std::unique_ptr<internal> socket_internal;


		public:
			udp_socket
			(
				const ipv4_socket_address& address
			);


			udp_socket
			(
				const ipv6_socket_address& address,
				bool                       allow_ipv4 = true
			);


			bool is_ready();


			bool receive
			(
				ipv6_packet& packet,
				bool         non_blocking    = false
			);


			void send
			(
				const ipv6_packet& what
			);


			ipv6_socket_address get_address() const;
	};


	inline udp_socket::internal::~internal() {}


	// 65535 bytes maximum - 20 bytes of IP header - 8 bytes of UDP header.
	const size_t maximum_udp_data_length = 65507;
}


#endif
