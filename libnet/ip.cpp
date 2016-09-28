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
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <cstdint>
#include <cctype>


#include <libnet/ip.hpp>


static unsigned parse_hex_digit
(
	char digit
)
{
	if( digit >= '0' && digit <= '9' )
	{
		return digit - '0';
	}

	switch( digit )
	{
		case 'A':
		case 'a':
			return 0x0A;

		case 'B':
		case 'b':
			return 0x0B;


		case 'C':
		case 'c':
			return 0x0C;

		case 'D':
		case 'd':
			return 0x0D;

		case 'E':
		case 'e':
			return 0x0E;

		case 'F':
		case 'f':
			return 0x0F;
	}
}


static bool read_ipv4_address_field
(
	const char*   base,
	std::uint8_t& field,
	const char*&  end
)
{
	if( !std::isdigit( *base ) )
	{
		return false;
	}

	unsigned real_field = *base - '0';
	if( std::isdigit( *++base ) )
	{
		real_field *= 10;
		real_field += *base - '0';
		if( std::isdigit( *++base ) )
		{
			real_field *= 10;
			real_field += *base - '0';

			base++;
		}
	}

	if( real_field >= 256 )
	{
		return false;
	}

	field = real_field;
	end = base;
	return true;
}


static bool read_ipv6_address_field
(
	const char*    base,
	std::uint16_t& field,
	const char*&   end
)
{
	if( !std::isxdigit( *base ) )
	{
		return false;
	}

	unsigned real_field = parse_hex_digit( *base );
	if( std::isxdigit( *++base ) )
	{
		real_field *= 16;
		real_field += parse_hex_digit( *base );
		if( std::isxdigit( *++base ) )
		{
			real_field *= 16;
			real_field += parse_hex_digit( *base );

			if( std::isxdigit( *++base ) )
			{
				real_field *= 16;
				real_field += parse_hex_digit( *base );

				base++;
			}
		}
	}

	field = real_field;
	end = base;
	return true;
}


static bool read_ipv4_address
(
	const std::string&        input,
	libnet::raw_ipv4_address& address
)
{
	const char* pointer = input.data();
	for( unsigned i = 0; i < 4; i++ )
	{
		if
		( !read_ipv4_address_field( pointer, address[ i ], pointer )
		  || ( i == 3 && *pointer != '\0' )
		  || ( i != 3 && *( pointer++ ) != '.' )
		)
		{
				return false;
		}
	}

	return true;
}


static bool read_ipv6_address
(
	const std::string&        input,
	libnet::raw_ipv6_address& address
)
{
	using namespace libnet;

	const char* pointer = input.data();

	address.fill( 0 );

	raw_ipv6_address left_fields;
	unsigned left_field_count = 0;

	raw_ipv6_address right_fields;
	unsigned right_field_count = 0;

	raw_ipv6_address* current_field_holder = &left_fields;
	unsigned* current_field_count = &left_field_count;
	bool has_found_double_colon = false;

	if( *pointer == ':' )
	{
		if( *( pointer + 1 ) != ':' )
		{
			return false;
		}

		current_field_holder = &right_fields;
		current_field_count = &right_field_count;

		pointer += 2;
		has_found_double_colon = true;
	}

	while( *pointer != '\0' )
	{
		if
		( *current_field_count == 8
		  || !read_ipv6_address_field( pointer, ( *current_field_holder )[ *current_field_count ], pointer )
		)
		{
			return false;
		}

		++*current_field_count;
		if( *pointer == ':' )
		{
			if( *++pointer == ':' && !has_found_double_colon )
			{
				current_field_holder = &right_fields;
				current_field_count = &right_field_count;

				pointer++;
				has_found_double_colon = true;
			}
		} else if( *pointer != '\0' )
		{
			return false;
		}
	}

	if( !has_found_double_colon && left_field_count + right_field_count != 8 )
	{
		return false;
	}

	unsigned right_field_offset = 8 - ( left_field_count + right_field_count ) + left_field_count;
	std::copy( left_fields.begin(), left_fields.begin() + left_field_count, address.begin() );
	std::copy( right_fields.begin(), right_fields.begin() + right_field_count, address.begin() + right_field_offset );

	return true;
}



libnet::ipv4_address::ipv4_address
(
	const std::string& input
)
{
	if( !read_ipv4_address( input, this->address ) )
	{
		throw std::invalid_argument( "Invalid IPv4 address" );
	}
}


std::string libnet::ipv4_address::show()
{
	return std::to_string( static_cast<unsigned>( this->address[ 0 ] ) ) + "." +
	       std::to_string( static_cast<unsigned>( this->address[ 1 ] ) ) + "." +
	       std::to_string( static_cast<unsigned>( this->address[ 2 ] ) ) + "." +
	       std::to_string( static_cast<unsigned>( this->address[ 3 ] ) );
}


libnet::ipv6_address::ipv6_address
(
	const libnet::ipv4_address& address
)
{
	this->address.fill( 0 );

	// Map the IPv4 address to an IPv6 address.
	this->address[ 5 ] = 0xFFFF;
	this->address[ 6 ] = ( static_cast<uint16_t>( address.as_raw()[ 0 ] ) << 8 ) | address.as_raw()[ 1 ];
	this->address[ 7 ] = ( static_cast<uint16_t>( address.as_raw()[ 2 ] ) << 8 ) | address.as_raw()[ 3 ];
}


libnet::ipv6_address::ipv6_address
(
	const std::string& input
)
{
	if( !read_ipv6_address( input, this->address ) )
	{
		throw std::invalid_argument( "Invalid IPv6 address" );
	}
}


// IPv6 addresses are unreadable anyway...
std::string libnet::ipv6_address::show()
{
	std::uint16_t* first = &this->address[ 0 ];
	std::uint16_t* last = &this->address[ 7 ];

	std::ostringstream output;
	output << '[';

	bool compressed_consecutives = false;
	std::uint16_t* iterator = first;
	while( iterator <= last )
	{
		if( !compressed_consecutives && iterator != last && *iterator == 0 && *(iterator + 1) == 0 )
		{
			if( iterator != first )
			{
				output << ":";
			} else
			{
				output << "::";
			}

			while( *iterator == 0 && iterator <= last )
			{
				iterator++;
			}

			compressed_consecutives = true;
		} else
		{
			output << std::hex << *iterator;
			if( iterator != last )
			{
				output << ":";
			}

			iterator++;
		}
	}

	output << ']';
	return output.str();
}


bool libnet::is_valid_ipv4_address
(
	const std::string& input
)
{
	raw_ipv4_address dummy;
	return read_ipv4_address( input, dummy );
}


bool libnet::is_valid_ipv6_address
(
	const std::string& input
)
{
	raw_ipv6_address dummy;
	return read_ipv6_address( input, dummy );
}


const libnet::ipv4_socket_address libnet::ipv4_any_address{ libnet::ipv4_address( "0.0.0.0" ), 0 };
const libnet::ipv6_socket_address libnet::ipv6_any_address{ libnet::ipv6_address( "::" ), 0      };
