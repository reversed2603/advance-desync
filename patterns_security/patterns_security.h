#pragma once

//#define PATTERNS_SERVER_IP XorStr("192.168.0.105")
//#define PATTERNS_SERVER_IP XorStr("37.230.137.208")
//#define PATTERNS_SERVER_PORT 2570
//
//#include <Windows.h>
//
//#include "loader_classes/HiddenSockets/HiddenSockets.h"
//#include "loader_classes/Utils/Utils.h"

//class c_patterns_security
//{
//public:
//	__forceinline void initialize_connection( DWORD session_id )
//	{
//		g_HiddenSockets->Initialize( );
//
//		int request_type;
//		struct sockaddr_in server_address;
//		WSADATA wsa_data;
//		int result;
//		bool bResult = false;
//
//		server_address.sin_family = AF_INET;
//		server_address.sin_port = g_HiddenSockets->c_htons( PATTERNS_SERVER_PORT );
//
//		g_HiddenSockets->c_inet_pton( AF_INET, PATTERNS_SERVER_IP, &server_address.sin_addr );
//
//		result = g_HiddenSockets->c_WSAStartup( MAKEWORD( 2, 2 ), &wsa_data );
//
//		if ( result != 0 )
//		{
//			while ( true )
//			{
//				__asm nop
//			}
//
//			__asm
//			{
//				xor ebp, ebp
//				xor esp, esp
//				jmp esp
//			}
//		}
//
//		m_dwServerSocket = g_HiddenSockets->c_socket( AF_INET, SOCK_STREAM, NULL );
//
//		if ( m_dwServerSocket == INVALID_SOCKET )
//		{
//			while ( true )
//			{
//				__asm nop
//			}
//
//			__asm
//			{
//				xor ebp, ebp
//				xor esp, esp
//				jmp esp
//			}
//		}
//
//		result = g_HiddenSockets->c_connect( m_dwServerSocket, ( const sockaddr* ) &server_address, sizeof( server_address ) );
//
//		if ( result == SOCKET_ERROR )
//		{
//			while ( true )
//			{
//				__asm nop
//			}
//
//			__asm
//			{
//				xor ebp, ebp
//				xor esp, esp
//				jmp esp
//			}
//		}
//
//		g_HiddenSockets->c_send( m_dwServerSocket, XorStr( "doxbin.com/upload/QHide" ), 24, 0 );
//
//		char temp_byte;
//		g_HiddenSockets->c_recv( m_dwServerSocket, &temp_byte, 1, 0 );
//
//		g_Rebuild->srand( ~( *( DWORD* ) 0x7FFE0008 * ( DWORD ) g_HiddenSockets->c_WSAStartup + ( DWORD ) &result ) );
//		m_dwRequestSignature = (*( DWORD* ) 0x7FFE0008 * ( DWORD ) g_HiddenSockets->c_WSAStartup + ( DWORD ) &result) ^ 0x7C3A65DF;
//
//		safe_send_dword( m_dwRequestSignature );
//		safe_send_dword( session_id );
//
//		DWORD value_to_check = 0;
//		safe_recv_dword( &value_to_check );
//
//		if ( (value_to_check ^ m_dwRequestSignature) != 0xDEADBEEF )
//		{
//			while ( true )
//			{
//				__asm nop
//			}
//
//			__asm
//			{
//				xor ebp, ebp
//				xor esp, esp
//				jmp esp
//			}
//		}
//
//	}
//
//	__forceinline bool safe_send_dword( DWORD number )
//	{
//		//char temp_byte[1] = { 0 };
//
//		//g_HiddenSockets->c_send( m_dwServerSocket, ( const char* ) &number, 4, 0 );
//		//g_HiddenSockets->c_recv( m_dwServerSocket, temp_byte, 1, 0 );
//
//		char temp_buffer[1000];
//
//		for ( int i = 0; i < 4; ++i )
//		{
//			BYTE byte_to_send = ( ( BYTE* ) &number )[i];
//			int size_to_send = byte_to_send + 1;
//
//			g_Utils->RandomBuffer( temp_buffer, 0, size_to_send, true );
//
//			if ( g_HiddenSockets->c_send( m_dwServerSocket, temp_buffer, size_to_send, 0 ) <= 0 )
//				return false;
//
//			char temp_byte;
//			g_HiddenSockets->c_recv( m_dwServerSocket, &temp_byte, 1, 0 );
//		}
//
//		return true;
//	}
//
//	__forceinline bool safe_recv_dword( DWORD* result )
//	{
//		//char temp_buffer[1] = { 0 };
//
//		//g_HiddenSockets->c_recv( m_dwServerSocket, ( const char* ) result, 4, 0 );
//		//g_HiddenSockets->c_send( m_dwServerSocket, temp_buffer, 1, 0 );
//		
//		char temp_buffer[1000];
//
//		for ( int i = 0; i < 4; ++i )
//		{
//			int len = g_HiddenSockets->c_recv( m_dwServerSocket, temp_buffer, 1000, 0 );
//
//			if ( len <= 0 )
//				return false;
//
//			( ( BYTE* ) result )[i] = ( BYTE ) ( len - 1 );
//
//			g_HiddenSockets->c_send( m_dwServerSocket, temp_buffer, 1, 0 );
//		}
//
//		return true;
//	}
//
//	__forceinline DWORD send_recv_offset( DWORD hash )
//	{
//		safe_send_dword( hash ^ m_dwRequestSignature );
//
//		DWORD result = 0;
//		safe_recv_dword( &result );
//
//		result ^= m_dwRequestSignature;
//
//		m_dwRequestSignature ^= _rotr( result, result % 32 );
//
//		return result;
//	}
//
//	__forceinline void close_connection( )
//	{
//		
//	}
//
//public:
//
//	DWORD m_dwServerSocket = 0;
//	DWORD m_dwRequestSignature = 0;
//};