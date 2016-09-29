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


#include <cstddef>
#include <string>


#include <umms/core.hpp>
#include <umms/atoms.hpp>
#include <umms/udp_source.hpp>


bool umms::udp_source::receive
(
	umms::ipv6_packet_atom& atom
)
{
	return this->socket.receive( atom, this->non_blocking );
}


bool umms::udp_source::is_ready()
{
	return this->socket.is_ready();
}
