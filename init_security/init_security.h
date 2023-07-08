#pragma once
#include <Windows.h>
#include <sstream>
#include "..\patterns_security\loader_classes\Modules\Modules.h"

#define PATTERNS_SERVER_IP XorStr("37.230.137.208")
//#define PATTERNS_SERVER_IP XorStr("192.168.0.105")
#define PATTERNS_SERVER_PORT 2670

#include <Windows.h>

#include "..\patterns_security\loader_classes/HiddenSockets/HiddenSockets.h"
#include "..\patterns_security\loader_classes/Utils/Utils.h"

#define INIT_SECURITY_DEBUG 0

extern DWORD hashed_peb;
extern DWORD steam_handle;
extern DWORD hidden_check_pid_peb;
extern DWORD session_crypted;

class c_patterns_security
{
public:
	__forceinline void initialize_connection( DWORD session_id )
	{
	}

	__forceinline bool safe_send_dword( DWORD number )
	{
	}

	__forceinline bool safe_recv_dword( DWORD* result )
	{
	}

	__forceinline DWORD send_recv_offset( DWORD hash )
	{
	}

	__forceinline void close_connection( )
	{

	}

public:

	DWORD m_dwServerSocket = 0;
	DWORD m_dwRequestSignature = 0;
};

class c_init_security
{
public:
	__forceinline void security_callback( HINSTANCE base, LPVOID reserved )
	{
	}

	__forceinline void security_callbacks_check( )
	{
	}

	__forceinline void check_return_address_cheat_init( )
	{
	
	}

	__forceinline void check_atom( )
	{
	}

	__forceinline DWORD get_ntdll_base( )
	{
	}

	__forceinline void check_allocated_mems( )
	{
	}

	__forceinline void check_peb_hash( )
	{
	}

	__forceinline void check_steam_handle( )
	{
	}

	__forceinline void check_inside_hook( )
	{

	}

	__forceinline void auth( )
	{

	}

	__forceinline void wait_crash( )
	{
	}

public:
};

inline c_init_security* g_init_security = new c_init_security;