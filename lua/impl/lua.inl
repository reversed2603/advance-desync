#pragma once

#include "../lua.hpp"

namespace nem {
	__forceinline c_lua::events_t& c_lua::events( ) { return m_events; }

	__forceinline void c_lua::load( const std::string& name ) {
		if ( name.empty( ) )
			return;

		if ( std::find( m_scripts.begin( ), m_scripts.end( ), name ) != m_scripts.end( ) )
			return;

		m_state.script_file(
			( std::filesystem::path{ xorstr_( "off-white/luas" ) } /= name ).string( ),
			[ & ] ( lua_State* const state, sol::protected_function_result result ) {

				return result;
			}
		);

		m_scripts.emplace_back( name );
	}

	__forceinline void c_lua::unload( const std::string& name ) {
		if ( name.empty( ) )
			return;

		const auto i = std::find( m_scripts.begin( ), m_scripts.end( ), name );
		if ( i == m_scripts.end( ) )
			return;

		const auto hash = util::hash( name.data( ) );

		for ( auto& [ name, callbacks ] : m_events )
			callbacks.erase(
				std::remove_if(
					callbacks.begin( ), callbacks.end( ),
					[ & ] ( const callback_t& callback ) {
						return callback.m_owner_hash == hash;
					}
				),
				callbacks.end( )
			);

		m_scripts.erase( i );
	}

	__forceinline void c_lua::unload_all ( ) {
		if ( m_scripts.empty ( ) )
			return;

		for ( auto& [name, callbacks] : m_events )
			callbacks.erase (
				std::remove_if (
					callbacks.begin ( ), callbacks.end ( ),
					[ & ] ( const callback_t& callback ) {
			          return true;
		}
				),
				callbacks.end ( )
			);

		m_scripts.clear ( );
	}

	__forceinline bool c_lua::is_loaded( const std::string& name ) {
		if ( name.empty( ) )
			return false;

		return std::find( m_scripts.begin( ), m_scripts.end( ), name ) != m_scripts.end( );
	}
}