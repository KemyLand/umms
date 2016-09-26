#ifndef UMMS_RAW_FILE_SOURCE_HPP
#define UMMS_RAW_FILE_SOURCE_HPP


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


#include <istream>


#include <umms/core.hpp>


namespace umms
{
	class raw_file_source : public source<raw_atom>
	{
		private:
			std::istream &input;


		public:
			inline raw_file_source
			(
				std::istream &input
			)
			: input( input )
			{}


			bool receive
			(
				raw_atom &atom
			);
	};
}


#endif
