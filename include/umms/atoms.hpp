#ifndef UMMS_ATOMS_HPP
#define UMMS_ATOMS_HPP


/* Global atom kind exports.
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


#include <libnet/ip.hpp>


#include <umms/core.hpp>


namespace umms
{
	using raw_atom         = std::vector<unsigned char>;
	using int_atom         = int;
	using ipv4_packet_atom = libnet::ipv4_packet;
	using ipv6_packet_atom = libnet::ipv6_packet;


	using raw_pipeline         = pipeline<raw_atom>;
	using int_pipeline         = pipeline<int_atom>;
	using ipv4_packet_pipeline = pipeline<ipv4_packet_atom>;
	using ipv6_packet_pipeline = pipeline<ipv6_packet_atom>;
}


#endif
