#pragma once
#include <winsock.h>
#include "..\loader_includes.h"

#include "..\Rebuild\Rebuild.h"
#include "..\Syscalls\Syscalls.h"

class C_HiddenSockets
{
public:
	int ( FAR PASCAL* c_send )( DWORD  s,
		const char* buf,
		int        len,
		int        flags ) = NULL;

	int ( FAR PASCAL* c_recv )( DWORD  s,
		const char* buf,
		int        len,
		int        flags ) = NULL;

	int ( FAR PASCAL* c_inet_pton )( INT   Family,
		PCSTR pszAddrString,
		PVOID pAddrBuf ) = NULL;

	int ( FAR PASCAL* c_WSAStartup )( WORD      wVersionRequired,
		LPWSADATA lpWSAData ) = NULL;

	DWORD( FAR PASCAL* c_socket )( int af,
		int type,
		int protocol ) = NULL;

	int ( FAR PASCAL* c_connect )( DWORD         s,
		const sockaddr* name,
		int            namelen ) = NULL;

	u_short( FAR PASCAL* c_htons )( u_short hostshort ) = NULL;

public:
	void Initialize( );
	void ParseFunctions( );

public:
	const void* m_pWS2_32Base = NULL;
};

inline C_HiddenSockets* g_HiddenSockets = new C_HiddenSockets;