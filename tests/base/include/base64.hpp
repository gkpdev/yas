
// Copyright (c) 2010-2017 niXman (i dot nixman dog gmail dot com). All
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

#ifndef __yas__tests__base__include__base64_hpp
#define __yas__tests__base__include__base64_hpp

/***************************************************************************/

template<typename archive_traits>
bool base64_test(std::ostream &log, const char *archive_type, const char *test_name) {
    (void)archive_type;
    {
        static const char a[] = "some string";
        static const char d[] = "c29tZSBzdHJpbmc=";
        typename archive_traits::oarchive_type::stream_type os;
        std::size_t r = ::yas::detail::modp_b64_encode(os, a, sizeof(a)-1);
        if ( r != sizeof(d)-1 ) {
            YAS_TEST_REPORT(log, archive_type, test_name);
            return false;
        }
        auto ibuf = os.get_intrusive_buffer();
        if ( 0 != std::memcmp(ibuf.data, d, ibuf.size) ) {
            YAS_TEST_REPORT(log, archive_type, test_name);
            return false;
        }

        typename archive_traits::iarchive_type::stream_type is(os.get_intrusive_buffer());
        std::string buf;
        buf.resize(os.get_intrusive_buffer().size);
        auto size = ::yas::detail::modp_b64_decode(&buf[0], is, os.get_intrusive_buffer().size);
        if ( size != sizeof(a)-1 ) {
            YAS_TEST_REPORT(log, archive_type, test_name);
            return false;
        }
        if ( 0 != std::memcmp(&buf[0], &a[0], size) ) {
            YAS_TEST_REPORT(log, archive_type, test_name);
            return false;
        }
    }
    {
        static const char a[] = "some strin";
        static const char d[] = "c29tZSBzdHJpbg==";
        typename archive_traits::oarchive_type::stream_type os;
        std::size_t r = ::yas::detail::modp_b64_encode(os, a, sizeof(a)-1);
        if ( r != sizeof(d)-1 ) {
            YAS_TEST_REPORT(log, archive_type, test_name);
            return false;
        }
        auto ibuf = os.get_intrusive_buffer();
        if ( 0 != std::memcmp(ibuf.data, d, ibuf.size) ) {
            YAS_TEST_REPORT(log, archive_type, test_name);
            return false;
        }

        typename archive_traits::iarchive_type::stream_type is(os.get_intrusive_buffer());
        std::string buf;
        buf.resize(os.get_intrusive_buffer().size);
        auto size = ::yas::detail::modp_b64_decode(&buf[0], is, os.get_intrusive_buffer().size);
        if ( size != sizeof(a)-1 ) {
            YAS_TEST_REPORT(log, archive_type, test_name);
            return false;
        }
        if ( 0 != std::memcmp(&buf[0], &a[0], size) ) {
            YAS_TEST_REPORT(log, archive_type, test_name);
            return false;
        }
    }
    {
        static const char a[] = "some stri";
        static const char d[] = "c29tZSBzdHJp";
        typename archive_traits::oarchive_type::stream_type os;
        std::size_t r = ::yas::detail::modp_b64_encode(os, a, sizeof(a)-1);
        if ( r != sizeof(d)-1 ) {
            YAS_TEST_REPORT(log, archive_type, test_name);
            return false;
        }
        auto ibuf = os.get_intrusive_buffer();
        if ( 0 != std::memcmp(ibuf.data, d, ibuf.size) ) {
            YAS_TEST_REPORT(log, archive_type, test_name);
            return false;
        }
    }

    return true;
}

/***************************************************************************/

#endif // __yas__tests__base__include__base64_hpp