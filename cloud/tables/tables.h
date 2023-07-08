#pragma once
#include <Windows.h>

struct s_table_internal_light
{
	DWORD m_hash;
	DWORD m_size;
	unsigned char m_data[];
};

struct s_value_light
{
	void* m_value;
	int m_size;

	template<typename T>
	T get()
	{
		return (T)m_value;
	}
};

struct s_table_light
{
	void* m_base;
	int m_size;

	void add_value(DWORD hash, int size, void* value);
	s_value_light* get_value(DWORD hash);
	void clear();
};

class c_tables_light
{
public:
	s_table_light* create_table(int size);
};

extern c_tables_light* g_tables_light;