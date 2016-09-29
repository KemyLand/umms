#ifndef UMMS_IPV6_VIA_RAW_GATEWAY_HPP
#define UMMS_IPV6_VIA_RAW_GATEWAY_HPP


/* IPv6 to raw gateway. The peer address is lost in the process.
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


#include <utility>


#include <umms/core.hpp>
#include <umms/atoms.hpp>


namespace umms
{
	template<>
	class gateway<ipv6_packet_atom, raw_atom> : public endpoint<ipv6_packet_atom>
	{
		private:
			endpoint<raw_atom>& gateway_endpoint;


		public:
			inline gateway
			(
				endpoint<raw_atom>& gateway_endpoint
			)
			: gateway_endpoint( gateway_endpoint )
			{}


			inline void send
			(
				ipv6_packet_atom &&atom
			)
			{
				this->gateway_endpoint.send( std::move( atom.get_payload() ) );
			}
	};
}


#endif
