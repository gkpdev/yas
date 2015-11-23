
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

#ifndef _yas_test__pod_hpp__included_
#define _yas_test__pod_hpp__included_

/***************************************************************************/

template<typename archive_traits>
bool pod_test(const char* archive_type, const char* io_type) {
	typename archive_traits::oarchive oa;
	archive_traits::ocreate(oa, archive_type, io_type);

	std::int8_t c = '1', cc;
	std::uint8_t uc = '2', uc2;
	std::int16_t s = 3, ss;
	std::uint16_t us = 4, us2;
	std::int32_t i = 5, ii;
	std::uint32_t l = 6, ll;
	std::int64_t i64 = 7, ii64;
	std::uint64_t ui64 = 8, uui64;
	float f = 3.14f, ff;
	double d = 3.14, dd;

	enum {
		binary_expected_size =
			 sizeof(c)
			+sizeof(uc)
			+sizeof(s)
			+sizeof(us)
			+sizeof(i)
			+sizeof(l)
			+sizeof(i64)
			+sizeof(ui64)
			+sizeof(f)
			+sizeof(d)
		,text_expected_size = 28
		,json_expected_size = 28
	};

	oa & c
		& uc
		& s
		& us
		& i
		& l
		& i64
		& ui64
		& f
		& d;

	switch ( archive_traits::oarchive_type::type() ) {
		case yas::archive_type::binary:
			if ( oa.size() != archive_traits::oarchive_type::header_size()+binary_expected_size ) {
				std::cout << "POD serialization error! [1]" << std::endl;
				return false;
			}
		break;
		case yas::archive_type::text:
			if ( oa.size() != archive_traits::oarchive_type::header_size()+text_expected_size ) {
				std::cout << "POD serialization error! [2]" << std::endl;
				return false;
			}
		break;
		case yas::archive_type::json:
		if ( oa.size() != archive_traits::oarchive_type::header_size()+json_expected_size ) {
			std::cout << "POD serialization error! [2]" << std::endl;
			return false;
		}
		break;
		default:
			std::cout << "POD serialization bad archive type!" << std::endl;
			return false;
	}

	typename archive_traits::iarchive ia;
	archive_traits::icreate(ia, oa, archive_type, io_type);
	ia & cc
		& uc2
		& ss
		& us2
		& ii
		& ll
		& ii64
		& uui64
		& ff
		& dd;

	if ( c != cc || uc != uc2 || s != ss || us != us2 || i != ii || l != ll || i64 != ii64 || ui64 != uui64 || f != ff || d != dd ) {
		std::cout << "POD deserialization error! [4]" << std::endl;
		return false;
	}

	return true;
}

/***************************************************************************/

#endif // _yas_test__pod_hpp__included_
