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


#include <string>


#include <umms/core.hpp>
#include <umms/int_via_raw_gateway.hpp>


void umms::gateway<umms::raw_atom, int>::send
(
	umms::raw_atom &&atom
)
{
	std::string input;
	for( auto byte : atom )
	{
		input.push_back( static_cast<char>( byte ) );
	}

	this->gateway_endpoint.send( std::stoi( input ) );
}


void umms::gateway<int, umms::raw_atom>::send
(
	int &&atom
)
{
	raw_atom output;
	if( atom < 0 )
	{
		output.push_back( '-' );
		atom = -atom;
	}

	int divisor = 1;
	while( atom / divisor >= 10 )
	{
		divisor *= 10;
	}

	do
	{
		int digit = atom / divisor;
		atom -= digit * divisor;
		divisor /= 10;

		output.push_back( '0' + digit );
		while( divisor >= 1 && atom / divisor == 0 )
		{
			output.push_back( '0' );
			divisor /= 10;
		}
	} while( atom > 0 );

	this->gateway_endpoint.send( std::move( output ) );
}
