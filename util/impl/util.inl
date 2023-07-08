#pragma once

#include "../util.hpp"

namespace util {
	__forceinline std::string to_utf8( const std::wstring_view str ) {
		if ( str.empty( ) )
			return {};

		const auto len = WideCharToMultiByte( CP_UTF8, 0, str.data( ), str.size( ), 0, 0, 0, 0 );

		std::string ret{};

		ret.resize( len );

		WideCharToMultiByte( CP_UTF8, 0, str.data( ), str.size( ), ret.data( ), len, 0, 0 );

		return ret;
	}

	__forceinline std::wstring to_utf16( const std::string_view str ) {
		if ( str.empty( ) )
			return {};

		const auto len = MultiByteToWideChar( CP_UTF8, 0, str.data( ), str.size( ), nullptr, 0 );

		std::wstring ret{};

		ret.resize( len );

		MultiByteToWideChar( CP_UTF8, 0, str.data( ), str.size( ), ret.data( ), len );

		return ret;
	}

	__forceinline constexpr std::uint32_t hash( const char* str ) {
		auto hash = 0x811c9dc5u;

		char v4{};
		do {
			v4 = *str++;
			hash = 0x1000193u * ( hash ^ v4 );
		} while ( v4 );

		return hash;
	}
}
