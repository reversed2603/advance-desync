#pragma once
#include <Windows.h>

class c_worker
{
public:
	static void __stdcall main_thread( );
	
	void check_save( );
	void push_cfg_save( char* buff );
	char m_save_buff[32];
	bool  m_is_save_null = false;
	bool  m_is_saving = false;

	void check_load( );
	void push_load( );
	bool m_is_loading = false;

	void check_refresh( );
	void push_refresh( );
	bool m_is_refreshing = false;

	void check_create( );
	void push_create( char* buff );
	char m_create_buffer[32];
	bool m_is_creating = false;

	void check_delete( );
	void push_delete( );
	bool m_deleting = false;

	void check_share( );
	void push_share( );
	bool m_sharing = false;

	void check_import( );
	void push_import( );
	bool m_importing = false;

public:
	bool m_update = false;
};

inline c_worker* g_worker = new c_worker;