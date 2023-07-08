#pragma once
#include "props.h"

#include "tables/tables.h"
#include "worker/worker.h"

#define BUFFER_SIZE 0x100000

enum e_request_type
{
	TYPE_GET_USER_INFO, // done
	TYPE_GET_USERS_ONLINE, // done
	TYPE_CHAT_GET_ALL_MESSAGES, // done
	TYPE_CHAT_SEND_MESSAGE, // done

	TYPE_CONFIG_CREATE, // done
	TYPE_CONFIG_DELETE, // done
	TYPE_CONFIG_REFRESH, // done
	TYPE_CONFIG_SAVE, // done
	TYPE_CONFIG_LOAD, // done
	TYPE_CONFIG_SHARE, // done
	TYPE_CONFIG_IMPORT, // done

	TYPE_MARKET_GET_PRODUCTS, // done
	TYPE_MARKET_BUY_PRODUCT, // done

	TYPE_MARKET_GET_PURCHASED_PRODUCTS, // done
	TYPE_MARKET_DOWNLOAD_PURCHASED_PRODUCT, // done
	TYPE_MARKET_RATE_PURCHASED_PRODUCT, // done

	TYPE_MARKET_GET_MY_PRODUCTS, // done
	TYPE_MARKET_PUSH_PRODUCT, // done
	TYPE_MARKET_DEL_PRODUCT, // done
};

class c_cloud
{
public:
	void init_update( );
	void reinitialize( );
	void check_connection( );
	const char* get_username( );

public:

	bool m_tables_inited = false;
	bool m_inited = false;
	bool m_connected = false;

	char* m_username = ( char* ) 0xDEADC0DE;
	DWORD m_username_hash = 0xDEADBEEF;

	s_table_light* m_send_table = 0;
	s_table_light* m_recv_table = 0;

	s_table_light* m_cfg_send_table = 0;
	s_table_light* m_cfg_recv_table = 0;
};

inline c_cloud* g_cloud = new c_cloud;