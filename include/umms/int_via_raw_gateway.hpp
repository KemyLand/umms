#ifndef INT_VIA_RAW_GATEWAY_HPP
#define INT_VIA_RAW_GATEWAY_HPP


/* Int to raw and raw to int gateways.
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


#include <umms/core.hpp>


namespace umms
{
	template<>
	class gateway<raw_atom, int> : public endpoint<raw_atom>
	{
		private:
			endpoint<int>& gateway_endpoint;


		public:
			inline gateway
			(
				endpoint<int>& gateway_endpoint
			)
			: gateway_endpoint( gateway_endpoint )
			{}


			void send
			(
				raw_atom &&atom
			);
	};


	template<>
	class gateway<int, raw_atom> : public endpoint<int>
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


			void send
			(
				int &&atom
			);
	};
}


#endif
