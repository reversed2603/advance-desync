#pragma once
#define max_edict_bits 11

#define max_edicts  ( 1<<max_edict_bits )

#include "../visuals.hpp"

namespace nem::hacks {
	__forceinline float& c_visuals::last_hurt_time( ) { return m_last_hurt_time; }

	__forceinline std::vector< vec3_t >& c_visuals::player_sounds( ) { return m_player_sounds; }

	__forceinline std::vector< c_visuals::bullet_impact_t >& c_visuals::bullet_impacts( ) { return m_bullet_impacts; }

	__forceinline void c_weather::reset_weather ( ) const {
		if ( !m_weather_created )
			return;

		const auto highest_ent_index = valve___g_entity_list->highest_index ( );

		for ( std::size_t i {}; i <= highest_ent_index; ++i ) {
			const auto ent = static_cast < valve::c_entity* > ( valve___g_entity_list->find_entity ( i ) );

			if ( !ent )
				continue;

			const auto cl_class = ent->client_class ( );

			if ( !cl_class )
				continue;

			if ( cl_class->m_class_id == valve::e_class_id::precipitation ) {

				using fn_t = void ( __thiscall* )( const std::uintptr_t, const int );

				const auto networkable = ent->networkable ( );
				if ( networkable )
					( *reinterpret_cast< fn_t** >( networkable ) ) [ 1u ] ( networkable, 0 );
			}
		}
	}
}