/* A raw source coming from a file. Each line is sent as an atom.
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
#include <umms/raw_file_source.hpp>


bool umms::raw_file_source::receive
(
	umms::raw_atom &atom
)
{
	atom.clear();

	int ret;
	while( ( ret = this->input.get() ) != EOF && ret != '\n' )
	{
		atom.push_back( ret );;
	}

	return this->input.good();
}
