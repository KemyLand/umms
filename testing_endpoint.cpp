/* A testing, file-based endpoint.
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


#include <iomanip>


#include <umms/core.hpp>
#include <umms/testing_endpoint.hpp>


void umms::testing_endpoint::send
(
	umms::raw_atom &&what
)
{
	this->output << "[ " << std::hex;
	for( auto byte : what )
	{
		this->output << std::setfill( '0' ) << std::setw( 2 ) << (int)byte << ' ';
	}

	this->output << std::dec << "]\n";
}
