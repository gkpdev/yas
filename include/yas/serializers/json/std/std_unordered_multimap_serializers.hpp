
// Copyright (c) 2010-2015 niXman (i dot nixman dog gmail dot com). All
// rights reserved.
//
// This file is part of YAS(https://github.com/niXman/yas) project.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef _yas__text__std_unordered_multimap_serializer_hpp
#define _yas__text__std_unordered_multimap_serializer_hpp

#include <yas/detail/type_traits/type_traits.hpp>
#include <yas/detail/type_traits/selector.hpp>
#include <yas/detail/io/serialization_exception.hpp>

#include <unordered_map>

namespace yas {
namespace detail {

/***************************************************************************/

template<typename K, typename V>
struct serializer<
	type_prop::not_a_pod,
	ser_method::use_internal_serializer,
	archive_type::text,
	std::unordered_multimap<K, V>
> {
	template<typename Archive>
	static Archive& save(Archive& ar, const std::unordered_multimap<K, V>& map) {
		ar & (std::uint32_t)map.size();
		for ( const auto &it: map ) {
			ar & it.first
				& it.second;
		}
		return ar;
	}

	template<typename Archive>
	static Archive& load(Archive& ar, std::unordered_multimap<K, V>& map) {
		std::uint32_t size = 0;
		ar & size;
		for ( ; size; --size ) {
			K key = K();
			V val = V();
			ar & key
				& val;
			map.insert(std::make_pair(std::move(key), std::move(val)));
		}
		return ar;
	}
};

/***************************************************************************/

} // namespace detail
} // namespace yas

#endif // _yas__text__std_unordered_multimap_serializer_hpp
