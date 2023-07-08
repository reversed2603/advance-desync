/*
* created by slazyy on 15.10.2020
*/

#pragma once

#include "../cfg.hpp"

namespace sdk {
	/* g_cfg should be constructed before this */
	template < typename _value_t >
	__forceinline cfg_var_t< _value_t >::cfg_var_t( const std::size_t hash, const _value_t value )
		: detail::base_cfg_var_t{ hash }, m_value{ value } { g_cfg->add_var( this ); }

	template < typename _value_t >
	__forceinline _value_t* cfg_var_t< _value_t >::operator &( ) { return &m_value; }

	template < typename _value_t >
	__forceinline cfg_var_t< _value_t >::operator _value_t( ) const { return m_value; }

	template < typename _value_t >
	__forceinline _value_t* cfg_var_t< _value_t >::operator ->( ) { return &m_value; }

	template < typename _value_t >
	__forceinline const _value_t* cfg_var_t< _value_t >::operator ->( ) const { return &m_value; }

	template < typename _value_t >
	__forceinline _value_t& cfg_var_t< _value_t >::value( ) { return m_value; }

	template < typename _value_t >
	__forceinline const _value_t& cfg_var_t< _value_t >::value( ) const { return m_value; }

	template < typename _value_t >
	void cfg_var_t< _value_t >::save( nlohmann::json& object ) const {
		auto& arr = object[ std::to_string( m_hash ) ];

		if constexpr ( std::is_same_v< _value_t, std::string > ) {
			for ( const auto& chr : m_value )
				arr.emplace_back( chr );

			return;
		}

		/* it'll save the value as well as alignment, but i don't care */
		for ( std::size_t i{}; i < sizeof( _value_t ); ++i )
			arr.emplace_back( reinterpret_cast< const std::uint8_t* >( &m_value )[ i ] );
	}

	template < typename _value_t >
	void cfg_var_t< _value_t >::load( const nlohmann::json& object ) {
		const auto arr = object.find( std::to_string( m_hash ) );
		if ( arr == object.end( ) )
			return;


		const auto& bytes = arr.value( );
		if ( !bytes.is_array( )
			|| bytes.size( ) != sizeof( _value_t ) )
			return;

		if constexpr ( std::is_same_v< _value_t, std::string > ) {
			m_value.clear( );
			m_value.reserve( bytes.size( ) );

			for ( const auto& chr : bytes.items( ) )
				m_value.push_back( chr.value( ).template get< char >( ) );

			return;
		}

		for ( std::size_t i{}; i < sizeof( _value_t ); ++i )
			reinterpret_cast< std::uint8_t* >( &m_value )[ i ] = bytes.at( i ).template get< std::uint8_t >( );

	}

	__forceinline void c_cfg::add_var( detail::base_cfg_var_t* const var ) { m_vars.emplace_back( var ); }

	__forceinline detail::base_cfg_var_t* c_cfg::find_var( const std::size_t hash ) const {
		for ( const auto& var : m_vars ) {
			if ( var->hash( ) != hash )
				continue;

			return var;
		}

		return nullptr;
	}
}