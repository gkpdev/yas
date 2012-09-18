
// Copyright (c) 2010-2012 niXman (i dot nixman dog gmail dot com)
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

#ifndef _yas__information_hpp__included_
#define _yas__information_hpp__included_

#include <stddef.h>
#include <string.h>

#include <yas/detail/config/config.hpp>

#include <yas/detail/tools/exceptions.hpp>
#include <yas/detail/tools/static_assert.hpp>
#include <yas/detail/type_traits/type_traits.hpp>
#include <yas/detail/type_traits/properties.hpp>
#include <yas/detail/preprocessor/preprocessor.hpp>
#include <yas/detail/version.hpp>

/***************************************************************************/

namespace yas {
namespace detail {

#pragma pack(push, 1)
union archive_header {
	archive_header()
		:as_char(0)
	{}
	archive_header(char c)
		:as_char(c)
	{}
	archive_header(unsigned char v, unsigned char t, unsigned char b)
	{ bits.version=v;bits.type=t;bits.bits=b; }

	struct {
		unsigned char version:4; // 0 ... 15
		unsigned char type:3; // binary, text, json
		unsigned char bits:1; // 0 - 32 bit, 1 - 64 bit
	} bits;
	char as_char;
};
#pragma pack(pop)

YAS_STATIC_ASSERT(sizeof(archive_header)==sizeof(char), ALIGNMENT_ERROR);

/***************************************************************************/

namespace {
	static const char yas_id[3] = {'y', 'a', 's'};
	static const char hex_alpha[] = "0123456789ABCDEF";
} // ns

static const char const_foo_char = 0;
static const char const_space_char = ' ';
static char foo_char = 0;

/***************************************************************************/

template<archive_type::type>
struct header_reader_writer;

template<>
struct header_reader_writer<archive_type::binary> {
	enum { header_size = 4 };

	template<typename Archive>
	static void read(Archive* ar, yas::header_flag op, archive_header& header) {
		if ( op == yas::no_header ) return;

		char buf[header_size];
		std::streamsize rd = ar->read(buf, header_size);
		if ( rd != header_size ) throw empty_archive_exception();
		if ( memcmp(buf, yas_id, sizeof(yas_id)) )
			throw bad_archive_information_exception();
		header = *reinterpret_cast<archive_header*>(buf+sizeof(yas_id));
	}

	template<typename Archive>
	static void write(Archive* ar, yas::header_flag op, archive_type::type at) {
		if ( op == yas::no_header ) return;

		static const archive_header header(
			(unsigned char)archive_version,
			(unsigned char)at,
			(unsigned char)YAS_PLATFORM_BITS_IS_64()
		);
		static const char buf[header_size] = {
			yas_id[0], yas_id[1], yas_id[2], header.as_char
		};

		std::streamsize wr = ar->write(&buf[0], header_size);
		if ( wr != header_size ) { throw std::runtime_error("write error"); }
	}
};

/***************************************************************************/

template<>
struct header_reader_writer<archive_type::text> {
	enum { header_size = 5 };

	template<typename Archive>
	static void read(Archive* ar, yas::header_flag op, archive_header& header) {
		if ( op == yas::no_header ) return;
		char buf[header_size+1];

		if ( ar->read(buf, header_size+1) != header_size+1 )
			throw empty_archive_exception();
		if ( memcmp(buf, yas_id, sizeof(yas_id)) )
			throw bad_archive_information_exception();
		const char* p = std::lower_bound(hex_alpha, hex_alpha + 16, buf[3]);
		if (*p != buf[3]) throw std::invalid_argument("not a hex digit");
		const char* q = std::lower_bound(hex_alpha, hex_alpha + 16, buf[4]);
		if (*q != buf[4]) throw std::invalid_argument("not a hex digit");

		header.as_char = (((p - hex_alpha) << 4) | (q - hex_alpha));
	}

	template<typename Archive>
	static void write(Archive* ar, yas::header_flag op, archive_type::type at) {
		if ( op == yas::no_header ) return;
		static const archive_header header(
			(unsigned char)archive_version,
			(unsigned char)at,
			(unsigned char)YAS_PLATFORM_BITS_IS_64()
		);

		static const char buf[header_size+1] = {
			 yas_id[0], yas_id[1], yas_id[2]
			,hex_alpha[(((yas::uint8_t)header.as_char) >> 4) & 0xff]
			,hex_alpha[((yas::uint8_t)header.as_char) & 15]
			,const_space_char
		};

		if ( ar->write(buf, header_size+1) != header_size+1 )
			throw std::runtime_error("write error");
	}
};

/***************************************************************************/


template<>
struct header_reader_writer<archive_type::json> {
	enum { header_size = 17 };

	template<typename Archive>
	static void read(Archive* ar, yas::header_flag op, archive_header& header) {
		(void)ar;
		(void)op;
		(void)header;
	}

	template<typename Archive>
	static void write(Archive* ar, yas::header_flag op, archive_type::type at) {
		(void)ar;
		(void)op;
		(void)at;
	}
};

/***************************************************************************/

#define YAS_WRITE_ARCHIVE_INFORMATION_SPECIALIZATION_IMPL(unused, idx, seq) \
	template<> \
	struct archive_information<YAS_PP_SEQ_ELEM(idx, seq), direction::in> { \
		archive_information() \
			:header() \
		{} \
		\
		static yas::uint32_t	header_size() {return header_reader_writer<YAS_PP_SEQ_ELEM(idx, seq)>::header_size;} \
		archive_type::type type() const {return YAS_PP_SEQ_ELEM(idx, seq);} \
		int bits() const { \
			if ( !header.bits.version ) throw no_header_exception(); \
			return (header.bits.bits ? 64 : 32); \
		} \
		int version() const { \
			if ( !header.bits.version ) throw no_header_exception(); \
			return header.bits.version; \
		} \
		\
		static const yas::uint32_t			_header_size	= header_reader_writer<YAS_PP_SEQ_ELEM(idx, seq)>::header_size; \
		static const int						_version			= archive_version; \
		static const archive_type::type	_type				= YAS_PP_SEQ_ELEM(idx, seq); \
		static const direction::type		_direction		= direction::in; \
		static const bool						_is_readable	= true; \
		static const bool						_is_writable	= false; \
		\
	protected: \
		template<typename Archive> \
		void init_header(Archive* ar, yas::header_flag op) { \
			header_reader_writer<YAS_PP_SEQ_ELEM(idx, seq)>::read(ar, op, header); \
		}\
		\
	private: \
		archive_header header; \
	}; \
	\
	template<> \
	struct archive_information<YAS_PP_SEQ_ELEM(idx, seq), direction::out> { \
		archive_information() \
		{} \
		\
		static yas::uint32_t header_size() {return header_reader_writer<YAS_PP_SEQ_ELEM(idx, seq)>::header_size;} \
		archive_type::type type() const {return YAS_PP_SEQ_ELEM(idx, seq);} \
		int bits() const {return YAS_PLATFORM_BITS();} \
		int version() const {return archive_version;} \
		\
		static const yas::uint32_t			_header_size	= header_reader_writer<YAS_PP_SEQ_ELEM(idx, seq)>::header_size; \
		static const int						_version			= archive_version; \
		static const archive_type::type	_type				= YAS_PP_SEQ_ELEM(idx, seq); \
		static const direction::type		_direction		= direction::in; \
		static const bool						_is_readable	= false; \
		static const bool						_is_writable	= true; \
		\
	protected: \
		template<typename Archive> \
		void init_header(Archive* ar, yas::header_flag op) { \
			header_reader_writer<YAS_PP_SEQ_ELEM(idx, seq)>::write(ar, op, YAS_PP_SEQ_ELEM(idx, seq)); \
		} \
	};

#define YAS_WRITE_ARCHIVE_INFORMATION_SPECIALIZATIONS(seq) \
	template<archive_type::type, direction::type> \
	struct archive_information; \
	YAS_PP_REPEAT( \
		YAS_PP_SEQ_SIZE(seq), \
		YAS_WRITE_ARCHIVE_INFORMATION_SPECIALIZATION_IMPL, \
		seq \
	)

/***************************************************************************/

YAS_WRITE_ARCHIVE_INFORMATION_SPECIALIZATIONS(
	(archive_type::binary)
	(archive_type::text)
	(archive_type::json)
);

/***************************************************************************/

} // namespace detail
} // namespace yas

#endif // _yas__information_hpp__included_
