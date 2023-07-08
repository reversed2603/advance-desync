#include "worker.h"

#include "..\cloud.h"

void __stdcall c_worker::main_thread( )
{
	while ( true )
	{
		const char* p_username = g_cloud->get_username( );

		if ( FNV1A_RT( p_username ) != g_cloud->m_username_hash )
			continue;

		if ( !g_worker->m_update )
		{
			Sleep( 100 );
			continue;
		}

		g_cloud->check_connection( );

		g_worker->check_create( );
		g_worker->check_delete( );
		g_worker->check_save( );
		g_worker->check_load( );
		g_worker->check_share( );
		g_worker->check_import( );
		g_worker->check_refresh( );

		g_worker->m_update = false;
	}
}

void c_worker::check_save( )
{
	if ( m_is_saving )
	{
		m_is_saving = false;
	}
}

void c_worker::push_cfg_save( char* buff )
{
	if ( buff )
		strcpy( m_save_buff, buff );

	m_is_save_null = !buff;
	m_is_saving = true;
	m_update = true;
}

void c_worker::check_load( )
{
	if ( m_is_loading )
	{
		m_is_loading = false;
	}
}

void c_worker::push_load( )
{
	m_is_loading = true;
}

void c_worker::check_refresh( )
{
	if ( m_is_refreshing )
	{
		m_is_refreshing = false;
	}
}

void c_worker::push_refresh( )
{
	m_is_refreshing = true;
}

void c_worker::check_create( )
{
	if ( m_is_creating )
	{
		m_is_creating = false;
	}
}

void c_worker::push_create( char* buff )
{
	strcpy( m_create_buffer, buff );
	m_is_creating = true;
}

void c_worker::check_delete( )
{
	if ( m_deleting )
	{
		m_deleting = false;
	}
}

void c_worker::push_delete( )
{
	m_deleting = true;
}

void c_worker::check_share( )
{
	if ( m_sharing )
	{
		m_sharing = false;
	}
}

void c_worker::push_share( )
{
	m_sharing = true;
}

void c_worker::check_import( )
{
	if ( m_importing )
	{
		m_importing = false;
	}
}

void c_worker::push_import( )
{
	m_importing = true;
}