#ifndef UMMS_TESTING_ENDPOINT_HPP
#define UMMS_TESTING_ENDPOINT_HPP


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


#include <ostream>


#include <umms/core.hpp>
#include <umms/atoms.hpp>


namespace umms
{
	class testing_endpoint : public endpoint<raw_atom>
	{
		private:
			std::ostream &output;


		public:
			inline testing_endpoint
			(
				std::ostream &output
			)
			: output( output )
			{}


			void send
			(
				raw_atom &&atom
			);
	};
}


#endif
