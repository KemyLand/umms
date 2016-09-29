#ifndef LIBNET_IP_HPP
#define LIBNET_IP_HPP


/* Common IPv4/v6 stuff.
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


#include <string>
#include <array>
#include <cstdint>


#include <libnet/packet.hpp>


namespace libnet
{
	using raw_ipv4_address = std::array<std::uint8_t, 4>;
	using raw_ipv6_address = std::array<std::uint16_t, 8>;


	template<typename AddressType>
	class ip_socket_address
	{
		private:
			AddressType address;
			unsigned    port;


		public:
			inline ip_socket_address
			(
				const AddressType& address,
				unsigned           port
			)
			: address ( address ),
			  port    ( port    )
			{}


			std::string show()
			{
				return this->address.show() + ":" + std::to_string( this->port );
			}


			const AddressType& get_address() const
			{
				return this->address;
			}


			auto get_port() const
			{
				return this->port;
			}
	};


	class ipv4_address
	{
		private:
			raw_ipv4_address address;


		public:
			inline ipv4_address
			(
				const raw_ipv4_address& address
			)
			{
				this->address = address;
			}


			ipv4_address
			(
				const std::string& address
			);


			std::string show();


			inline const raw_ipv4_address as_raw() const
			{
				return this->address;
			}
	};


	class ipv6_address
	{
		private:
			raw_ipv6_address address;


		public:
			inline ipv6_address
			(
				const raw_ipv6_address& address
			)
			{
				this->address = address;
			}


			ipv6_address
			(
				const ipv4_address& address
			);


			ipv6_address
			(
				const std::string& address
			);


			std::string show();


			inline const raw_ipv6_address as_raw() const
			{
				return this->address;
			}
	};


	using ipv4_socket_address = ip_socket_address<ipv4_address>;
	using ipv6_socket_address = ip_socket_address<ipv6_address>;
	using ipv4_packet         = packet<ipv4_socket_address>;
	using ipv6_packet         = packet<ipv6_socket_address>;


	bool is_valid_ipv4_address
	(
		const std::string& address
	);


	bool is_valid_ipv6_address
	(
		const std::string& address
	);


	extern const ipv4_socket_address ipv4_any_address;
	extern const ipv6_socket_address ipv6_any_address;


	template<>
	inline const ipv4_socket_address& placeholder_address<ipv4_socket_address>()
	{
		return ipv4_any_address;
	}


	template<>
	inline const ipv6_socket_address& placeholder_address<ipv6_socket_address>()
	{
		return ipv6_any_address;
	}
}


#endif
