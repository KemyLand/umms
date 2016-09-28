#ifndef UMMS_MULTIPLIER_TRANSFORMER_HPP
#define UMMS_MULTIPLIER_TRANSFORMER_HPP


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
#include <umms/atoms.hpp>


namespace umms
{
	class multiplier_transformer : public transformer<int_atom>
	{
		private:
			bool     flip_flop;
			int_atom temporary;


		public:
			inline multiplier_transformer()
			: flip_flop( false )
			{}


			bool process
			(
				int_atom &atom
			);
	};
}


#endif
