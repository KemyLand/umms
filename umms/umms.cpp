/* UMMS, the Universal Media Multiplexing Server.
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


#include <iostream>
#include <memory>
#include <utility>


#include <libnet/ip.hpp>
#include <libnet/udp.hpp>


#include <umms/core.hpp>
#include <umms/atoms.hpp>
#include <umms/udp_source.hpp>
#include <umms/testing_endpoint.hpp>
#include <umms/reversing_transformer.hpp>
#include <umms/multiplier_transformer.hpp>
#include <umms/int_via_raw_gateway.hpp>
#include <umms/ipv6_via_raw_gateway.hpp>


int main()
{
	using namespace umms;

	libnet::udp_socket server_socket = libnet::ipv6_any_address;
	std::cerr << "Listening on " << server_socket.get_address().show() << "...\n";

	testing_endpoint endpoint { std::cout };
	gateway<int_atom, raw_atom> output_gateway { endpoint };
	int_pipeline multiplier { std::unique_ptr<transformer<int>>( new multiplier_transformer ), std::unique_ptr<transformer<int>>( new multiplier_transformer ), output_gateway };
	gateway<raw_atom, int_atom> multiplier_gateway { multiplier };
	raw_pipeline reverser { std::unique_ptr<transformer<raw_atom>>( new reversing_transformer ), multiplier_gateway };
	gateway<ipv6_packet_atom, raw_atom> udp_gateway { reverser };
	udp_source source { server_socket };

	ipv6_packet_atom atom = libnet::ipv6_any_address;
	while( source.receive( atom ) )
	{
		udp_gateway.send( std::move( atom ) );
	}

	return 0;
}
