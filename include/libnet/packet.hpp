#ifndef LIBNET_PACKET_HPP
#define LIBNET_PACKET_HPP


/* A packet is hereby defined as a pair of a network address and a payload.
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


#include <vector>
#include <utility>


namespace libnet
{
	template<typename AddressType>
	const AddressType& placeholder_address();


	template<typename AddressType>
	class packet
	{
		private:
			AddressType                peer_address;
			std::vector<unsigned char> payload;


		public:
			inline packet
			(
				const AddressType&           peer_address = placeholder_address<AddressType>(),
				std::vector<unsigned char>&& payload      = std::vector<unsigned char>()
			)
			: peer_address ( peer_address         ),
			  payload      ( std::move( payload ) )
			{}


			inline const AddressType& get_peer_address() const
			{
				return this->peer_address;
			}


			inline auto& get_payload()
			{
				return this->payload;
			}


			inline const auto& get_payload() const
			{
				return this->payload;
			}
	};
}


#endif
