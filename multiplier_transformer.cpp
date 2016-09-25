/* For each two input atoms X and Y, outputs X times Y as a single atom.
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


#include <umms/core.hpp>
#include <umms/multiplier_transformer.hpp>


bool umms::multiplier_transformer::process
(
	int &atom
)
{
	this->flip_flop = !this->flip_flop;
	if( !this->flip_flop )
	{
		atom *= this->temporary;
		return true;
	} else
	{
		this->temporary = atom;
		return false;
	}
}
