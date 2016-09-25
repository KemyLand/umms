/* UMMS, the Universal Media Multiplexing Server.
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


#include <iostream>
#include <memory>
#include <utility>


#include <umms/core.hpp>
#include <umms/raw_file_source.hpp>
#include <umms/testing_endpoint.hpp>
#include <umms/reversing_transformer.hpp>


int main()
{
	using namespace umms;

	raw_file_source source { std::cin  };
	testing_endpoint endpoint { std::cout };
	raw_pipeline pipeline { std::unique_ptr<transformer<raw_atom>>( new reversing_transformer ), endpoint };

	raw_atom atom;
	while( source.receive( atom ) )
	{
		pipeline.send( std::move( atom ) );
	}

	return 0;
}
