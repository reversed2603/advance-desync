#include "requests.h"

#include "..\..\..\nem\dependencies\xorstr.hpp"

#include "..\cloud.h"

c_requests_light* g_requests_light = new c_requests_light;

typedef INT(__stdcall* dllmain)(HMODULE, DWORD, LPVOID);

void c_requests_light::init()
{
	m_ws2_32_base = LoadLibraryA( xorstr_( "ws2_32.dll" ) );

	c_recv = (decltype(c_recv))GetProcAddress((HMODULE)m_ws2_32_base, xorstr_("recv"));
	c_send = (decltype(c_recv)) GetProcAddress( ( HMODULE ) m_ws2_32_base, xorstr_("send"));
	c_inet_pton = (decltype(c_inet_pton)) GetProcAddress( ( HMODULE ) m_ws2_32_base, xorstr_("inet_pton"));
	c_WSAStartup = (decltype(c_WSAStartup)) GetProcAddress( ( HMODULE ) m_ws2_32_base, xorstr_("WSAStartup"));
	c_socket = (decltype(c_socket)) GetProcAddress( ( HMODULE ) m_ws2_32_base, xorstr_("socket"));
	c_connect = (decltype(c_connect)) GetProcAddress( ( HMODULE ) m_ws2_32_base, xorstr_("connect"));
	c_htons = (decltype(c_htons)) GetProcAddress( ( HMODULE ) m_ws2_32_base, xorstr_("htons"));
	c_closesocket = (decltype(c_closesocket)) GetProcAddress( ( HMODULE ) m_ws2_32_base, xorstr_("closesocket"));

	m_inited = true;
}

bool c_requests_light::connect()
{
	s_table_light* recv_table = g_tables_light->create_table(0x1000);
	s_table_light* send_table = g_tables_light->create_table(0x1000);

	int request_type;
	struct sockaddr_in server_address;
	WSADATA wsa_data;
	int result;

	m_client_signature += *(DWORD*)0x7FFE0008;

	auto ip = SERVER_IP;
	int port = SERVER_PORT;

	server_address.sin_family = AF_INET;
	server_address.sin_port = c_htons(port);

	c_inet_pton(AF_INET, ip, &server_address.sin_addr);

	result = c_WSAStartup(MAKEWORD(2, 2), &wsa_data);

	if (result != 0)
		return false;

	m_server_socket = c_socket(AF_INET, SOCK_STREAM, NULL);

	if (m_server_socket == INVALID_SOCKET)
		return false;
	
	result = c_connect(m_server_socket, (const sockaddr*)&server_address, sizeof(server_address));

	if (result == SOCKET_ERROR || m_server_socket == INVALID_SOCKET)
		return false;
	
	send_table->clear();
	recv_table->clear();

	send_table->add_value(m_client_signature, 4, &m_client_signature); // junk
	send_table->add_value(FNV1A("client_id"), 4, &m_client_signature);

	g_cipher_light->crypt(send_table->m_base, send_table->m_size, CLIENT_KEY, 0);
	send_data(send_table->m_base, send_table->m_size);

	send_table->clear();

	recv_table->m_size = recv_data(recv_table->m_base, 0x10000);
	
	g_cipher_light->decrypt(recv_table->m_base, recv_table->m_size, CLIENT_KEY, m_client_signature);
	memcpy(m_server_key, recv_table->get_value(FNV1A("server_key"))->get<void*>(), 16);

	recv_table->clear();

	m_connected = true;

	return true;
}

bool c_requests_light::send_data(void* buffer, int size)
{
	int size_to_send = size;
	char* last_pos = (char*)buffer;
	int result;

	while (size_to_send > 0)
	{
		result = c_send(m_server_socket, last_pos, size_to_send, 0);

		if ( result <= 0 )
		{
			g_cloud->reinitialize( );
			return false;
		}

		size_to_send -= result;
		last_pos += result;
	}

	result = c_send(m_server_socket, (char*)xorstr_("SUCK"), 4, 0);

	if ( result <= 0 )
	{
		g_cloud->reinitialize( );
	}

	return result > 0;
}

int c_requests_light::recv_data(void* buffer, int max_size)
{
	int recv_len = 1;
	int result = 0;

	void* base = buffer;
	SIZE_T size = max_size;

	while (recv_len > 0)
	{
		recv_len = c_recv(m_server_socket, (char*)base + result, max_size - result, NULL);

		result += recv_len;

		if (recv_len >= 4 && *((DWORD*)((char*)base + result - 4)) == 0x4B435553)
		{
			if ( result - 4 <= 0 )
			{
				g_cloud->reinitialize( );
			}

			return result - 4;
		}
	}

	if ( result <= 0 )
	{
		g_cloud->reinitialize( );
	}

	return result;
}