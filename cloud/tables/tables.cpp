#include "tables.h"

c_tables_light* g_tables_light = new c_tables_light;

s_table_light* c_tables_light::create_table(int size)
{
	s_table_light* table = new s_table_light;

	table->m_base = malloc( size );
	table->m_size = size;

	return table;
}

void s_table_light::add_value(DWORD hash, int size, void* value)
{
	s_table_internal_light* end = (s_table_internal_light*)((DWORD)m_base + m_size);

	end->m_hash = hash;
	end->m_size = size;
	memcpy(end->m_data, value, size);

	m_size += 8 + size;
}

s_value_light* s_table_light::get_value(DWORD hash)
{
	s_value_light* value = (s_value_light*)malloc(sizeof(s_value_light));
	s_table_internal_light* item = (s_table_internal_light*)m_base;

	while (item->m_hash)
	{
		if (item->m_hash == hash)
		{
			value->m_value = item->m_data;
			value->m_size = item->m_size;

			return value;
		}

		item = (s_table_internal_light*)((DWORD)item + 8 + item->m_size);
	}

	value->m_value = NULL;
	value->m_size = NULL;

	return value;
}

void s_table_light::clear()
{
	memset(m_base, 0x00, m_size);
	m_size = NULL;
}

