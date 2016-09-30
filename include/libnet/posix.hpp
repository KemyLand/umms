#ifndef LIBNET_POSIX_HPP
#define LIBNET_POSIX_HPP


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


#include <sys/socket.h>
#include <netinet/in.h>


#include <libnet/ip.hpp>


namespace libnet
{
	ipv6_socket_address normalize_address
	(
		const struct sockaddr_in6& in6_addr
	);


	struct sockaddr_in6 denormalize_address
	(
		const ipv6_socket_address& address
	);
}

#endif
