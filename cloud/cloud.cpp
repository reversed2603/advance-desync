#include "cloud.h"

#include "requests/requests.h"
#include "worker/worker.h"

void c_cloud::init_update( )
{
	if ( !m_tables_inited )
	{
		m_send_table = g_tables_light->create_table( BUFFER_SIZE );
		m_recv_table = g_tables_light->create_table( BUFFER_SIZE );

		m_cfg_send_table = g_tables_light->create_table( BUFFER_SIZE );
		m_cfg_recv_table = g_tables_light->create_table( BUFFER_SIZE );

		m_tables_inited = true;
	}

	if ( !m_inited )
	{
		auto handle = CreateThread( 0, 0, ( LPTHREAD_START_ROUTINE ) g_worker->main_thread, 0, 0, 0 );
		SetThreadPriority( handle, THREAD_PRIORITY_TIME_CRITICAL );

		m_inited = true;
	}

	g_worker->m_update = true;
}

void c_cloud::reinitialize( )
{
	m_connected = false;
	m_inited = false;

	delete g_worker;
	g_worker = new c_worker;

	ExitThread( 0 );
}

void c_cloud::check_connection( )
{
	if ( m_connected )
		return;

	g_requests_light->init( );

	bool connected = false;

	do
	{
		connected = g_requests_light->connect( );

		if ( !connected )
			Sleep( 1000 );

	} while ( !connected );

	m_connected = true;
	g_worker->push_refresh( );
}

#include "..\allah.h"

const char* c_cloud::get_username( )
{

	m_username_hash = FNV1A( "nelfo" );
	return "nelfo";

}
