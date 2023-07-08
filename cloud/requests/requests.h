#pragma once
#include <Windows.h>
#include <stdint.h>

#include "..\cipher\cipher.h"
#include "..\tables\tables.h"
#include "..\props.h"


class c_requests_light
{
public:
	void init();
	bool connect();

	bool send_data(void* buffer, int size);
	int recv_data(void* buffer, int max_size);

public:

	int (FAR PASCAL* c_send)(DWORD  s,
		const char* buf,
		int        len,
		int        flags);

	int (FAR PASCAL* c_recv)(DWORD  s,
		const char* buf,
		int        len,
		int        flags);

	int (FAR PASCAL* c_inet_pton)(INT   Family,
		PCSTR pszAddrString,
		PVOID pAddrBuf);

	int (FAR PASCAL* c_WSAStartup)(WORD      wVersionRequired,
		LPWSADATA lpWSAData);

	DWORD(FAR PASCAL* c_socket)(int af,
		int type,
		int protocol);

	int (FAR PASCAL* c_connect)(DWORD         s,
		const sockaddr* name,
		int            namelen);

	u_short(FAR PASCAL* c_htons)(u_short hostshort);

	int(FAR PASCAL* c_closesocket)(DWORD socket);

public:
	void* m_ws2_32_base = NULL;

	DWORD m_server_socket = NULL;
	bool m_connected = false;
	bool m_inited = false;
	bool m_authorized = false;

	const char* m_server_ip;
	DWORD m_port;

public:
	DWORD m_client_signature = NULL;
	uint8_t m_server_key[16];

};

extern c_requests_light* g_requests_light;