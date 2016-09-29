#ifndef UMMS_UDP_SOURCE_HPP
#define UMMS_UDP_SOURCE_HPP


/* A raw source coming from an UDP socket, one atom per datagram.
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


#include <istream>


#include <libnet/udp.hpp>


#include <umms/core.hpp>
#include <umms/atoms.hpp>


namespace umms
{
	class udp_source : public source<ipv6_packet_atom>
	{
		private:
			libnet::udp_socket& socket;
			bool                non_blocking;


		public:
			inline udp_source
			(
				libnet::udp_socket& socket,
				bool                non_blocking = false
			)
			: socket       ( socket       ),
			  non_blocking ( non_blocking )
			{}


			bool receive
			(
				ipv6_packet_atom &atom
			);


			bool is_ready();


			inline bool is_non_blocking() const
			{
				return this->non_blocking;
			}


			inline void set_non_blocking
			(
				bool non_blocking
			)
			{
				this->non_blocking = non_blocking;
			}
	};
}


#endif
