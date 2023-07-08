#pragma once

#define JM_XORSTR_DISABLE_AVX_INTRINSICS

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define _HAS_EXCEPTIONS 0

#define IMGUI_DEFINE_MATH_OPERATORS

#define IM_VEC2_CLASS_EXTRA \
        __forceinline float dot( const ImVec2& rhs ) const { return rhs.x * x + rhs.y * y; } \
        __forceinline float length_sqr( ) const { return dot( *this ); } \
        __forceinline float length( ) const { return std::sqrt( length_sqr( ) ); } \
        __forceinline float normalize( ) { const auto len = length( ); if ( len ) { x /= len; y /= len; } return len; } \
        __forceinline ImVec2 normalized( ) { auto ret = *this; ret.normalize( ); return ret; }


#define IM_VEC3_CLASS_EXTRA \
        __forceinline ImVec3 cross( const ImVec3& rhs ) const { return { ( y * rhs.z ) - ( z * rhs.y ), ( z * rhs.x ) - ( x * rhs.z ), ( x * rhs.y ) - ( y * rhs.x ) }; } \
	    __forceinline float dot( const float* const rhs ) const { return ( x * rhs[ 0u ] + y * rhs[ 1u ] + z * rhs[ 2u ] ); } \
        __forceinline float dot( const ImVec3& rhs ) const { return rhs.x * x + rhs.y * y + rhs.z * z; } \
        __forceinline float length_sqr( ) const { return dot( *this ); } \
        __forceinline float length_2d_sqr( ) const { return x * x + y * y; } \
        __forceinline float length( ) const { return std::sqrt( length_sqr( ) ); } \
        __forceinline float length_2d( ) const { return std::sqrt( length_2d_sqr( ) ); } \
        __forceinline float normalize( ) { const auto len = length( ); if ( len ) { x /= len; y /= len; z /= len; } return len; } \
        __forceinline ImVec3 normalized( ) { auto ret = *this; ret.normalize( ); return ret; }

#if __clang__ && __cplusplus >= 201907l /* don't ask */
#define __cpp_lib_format /* fck thhisshit the dependencies ofthis lib arefuckinghuge */
#endif

#include <windows.h>
#include <intrin.h>

#include <algorithm>
#include <thread>
#include <array>
#include <deque>
#include <unordered_map>
#include <format>
#include <mutex>
#include <filesystem>
#include <type_traits>
#include <fstream>
#include <semaphore>
#include <future>

#include <d3d9.h>
#include <d3dx9.h>

#include "dependencies/xorstr.hpp"

#include <mmsystem.h>


#include <playsoundapi.h>
#pragma comment(lib, "Winmm.lib")
#define ALWAYS_INLINE __forceinline
#define ENCSTR( str ) xorstr_( str )
#define SDK_CFG_ID_OBJECT "shit_paste"

#define JSON_NOEXCEPTION

#include "dependencies/json.hpp"

#include "dependencies/thread_pool/thread_pool.hpp"

#include "dependencies/minhook/minhook.h"

#include "dependencies/imgui/imgui.h"
#include "../../desync-recode-v2-main/csgo/imgui_internal.h"
#include "dependencies/imgui/imgui_impl_dx9.h"
#include "dependencies/imgui/imgui_impl_win32.h"
#include "menu/NL/PostProcessing/PostProcessing.h"
#include "dependencies/imgui/imgui_freetype.h"

#include "math/math.hpp"
#define SOL_NO_EXCEPTIONS 1
#include "dependencies/cfg/cfg.hpp"
class C_Config
{
public:
    std::string GetSave( );
    void Load( std::string& cData );

    // Ragebot
};

inline C_Config* g_Config = new C_Config ( );

#include "dependencies/logo.hpp"
#include "dependencies/font.hpp"
#include "dependencies/avatar.hpp"

#include "dependencies/sol/sol.hpp"
#include "dependencies/qo0_icons.hpp"

#include "dependencies/custom.hpp"

#include "util/util.hpp"

#include "context/context.hpp"
#include "valve/valve.hpp"

#include "lua/lua.hpp"

#include "menu/NL/Neverlose.h"

#include "hacks/hacks.hpp"

#include "menu/menu.hpp"

#include "hooks/hooks.hpp"

#include "patterns_security/patterns_security.h"

#include "init_security/init_security.h"