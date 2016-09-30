/* BSD sockets (*not* strict POSIX!) implementation details.
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


#include <cstdint>
#include <cstring>


#include <sys/socket.h>
#include <netinet/ip.h>


#include <libnet/ip.hpp>
#include <libnet/posix.hpp>


libnet::ipv6_socket_address libnet::normalize_address
(
	const struct sockaddr_in6& in6_addr
)
{
	raw_ipv6_address raw_address;
	for( unsigned i = 0; i < 8; i++ )
	{
		raw_address[ i ] = ( static_cast<std::uint16_t>( in6_addr.sin6_addr.s6_addr[ i * 2 ] ) << 8 )
		                   | in6_addr.sin6_addr.s6_addr[ i * 2 + 1 ];
	}

	return ipv6_socket_address( raw_address, ntohs( in6_addr.sin6_port ) );
}


struct sockaddr_in6 libnet::denormalize_address
(
	const libnet::ipv6_socket_address& address
)
{
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

