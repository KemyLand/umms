#ifndef CORE_HPP
#define CORE_HPP


/* Definitions of core concepts, such as pipelines, sources, endpoints, etc...
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


#include <vector>
#include <memory>
#include <utility>
#include <type_traits>


namespace umms
{
	template<typename AtomType>
	class source
	{
		public:
			virtual ~source() = 0;


			virtual bool receive
			(
				AtomType &what
			) = 0;
	};


	template<typename AtomType>
	inline source<AtomType>::~source() {}


	template<typename AtomType>
	class endpoint
	{
		public:
			virtual ~endpoint() = 0;


			virtual void send
			(
				AtomType &&what
			) = 0;
	};


	template<typename AtomType>
	inline endpoint<AtomType>::~endpoint() {}


	template<typename AtomType>
	class transformer
	{
		public:
			virtual ~transformer() = 0;


			virtual void process
			(
				AtomType &what
			) = 0;
	};


	template<typename AtomType>
	inline transformer<AtomType>::~transformer() {}


	template<typename AtomType>
	class pipeline
	{
		public:
			using transformer_handle = std::unique_ptr<transformer<AtomType>>;


		private:
			source<AtomType>&               pipe_source;
			endpoint<AtomType>&             pipe_endpoint;
			std::vector<transformer_handle> transformers;


		public:
			template<typename... OtherTypes>
			inline pipeline
			(
				source<AtomType>& pipe_source,
				OtherTypes&&...   others
			)
			: pipe_source   ( pipe_source                                               ),
			  pipe_endpoint ( extract_endpoint( std::forward<OtherTypes>( others )... ) )
			{
				this->handle_transformer_list( std::forward<OtherTypes>( others )... );
			}


		private:
			inline endpoint<AtomType>& extract_endpoint
			(
				endpoint<AtomType>& pipe_endpoint
			)
			{
				return pipe_endpoint;
			}


			template<typename FirstType, typename SecondType, typename... OtherTypes>
			inline endpoint<AtomType>& extract_endpoint
			(
				FirstType&&     unused_first,
				SecondType&&    second,
				OtherTypes&&... others
			)
			{
				return this->extract_endpoint
				(
					std::forward<SecondType>( second ),
					std::forward<OtherTypes>( others )...
				);
			}


			/* I have to do this. Otherwise, fucking G++ won't compile it.
			 * Now you see why I hate C++?
			 */
			template<typename EndpointType>
			inline std::enable_if_t<std::is_base_of<endpoint<AtomType>, std::remove_reference_t<EndpointType>>::value>
			handle_transformer_list
			(
				transformer_handle&& transformer,
				EndpointType&&       unused_endpoint
			)
			{
				this->append_transformer( std::move( transformer ) );
			}


			template<typename... OtherTypes>
			inline void handle_transformer_list
			(
				transformer_handle&& transformer,
				OtherTypes&&...      others
			)
			{
				this->append_transformer( std::move( transformer ) );
				this->handle_transformer_list( std::forward<OtherTypes>( others )... );
			}


		public:
			inline void prepend_transformer
			(
				transformer_handle&& transformer
			)
			{
				this->transformers.push_front( std::move( transformer ) );
			}


			inline void append_transformer
			(
				transformer_handle&& transformer
			)
			{
				this->transformers.push_back( std::move( transformer ) );
			}


			inline const auto& get_transformers() const
			{
				return this->transformers;
			}


			bool flowstep()
			{
				AtomType atom;
				if( !this->pipe_source.receive( atom ) )
				{
					return false;
				}

				for( auto &transformer : this->transformers )
				{
					transformer->process( atom );
				}

				this->pipe_endpoint.send( std::move( atom ) );
				return true;
			}
	};


	using raw_atom     = std::vector<unsigned char>;
	using raw_pipeline = pipeline<raw_atom>;
}


#endif
