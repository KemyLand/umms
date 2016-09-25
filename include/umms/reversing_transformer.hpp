#ifndef REVERSING_TRANSFORMER_HPP
#define REVERSING_TRANSFORMER_HPP


/* A raw, simple transformer that just bitwise-reverses the contents of each atom.
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


#include <algorithm>


#include <umms/core.hpp>


namespace umms
{
	class reversing_transformer : public transformer<raw_atom>
	{
		public:
			inline void process
			(
				raw_atom &atom
			)
			{
				std::reverse( atom.begin(), atom.end() );
			}
	};
}


#endif
