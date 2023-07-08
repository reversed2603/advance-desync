#pragma once

namespace util {
	__forceinline std::string to_utf8( const std::wstring_view str );

	__forceinline std::wstring to_utf16( const std::string_view str );

	__forceinline constexpr std::uint32_t hash( const char* str );
}

#include "impl/util.inl"