#include "../../../nem.hpp"

namespace nem::hacks {

	void c_knife_bot::handle_knife_bot ( valve::user_cmd_t& cmd )
	{
		m_best_index = -1;

		if ( !valve::g_local_player->weapon ( ) )
			return;

		if ( !valve::g_local_player->weapon ( )->is_knife ( ) )
			return;

		if ( !select_target ( ) )
			return;

		auto best_angle = get_hitbox_pos ( 5, m_best_player );
		auto entity_angle = math::calc_ang ( g_context->shoot_pos ( ), best_angle );
		auto health = m_best_player->health ( );

		auto stab = false;

		if ( m_best_player->has_heavy_armor ( ) )
		{
			if ( health <= 55 && health > get_min_dmg ( ) )
				stab = true;
		}
		else
		{
			if ( health <= 65 && health > get_min_dmg ( ) )
				stab = true;
		}

		if ( health >= 90 )
			stab = true;

		stab = stab && m_best_distance < 60;

		cmd.m_view_angles = entity_angle;

		if ( stab )
			cmd.m_buttons |= valve::e_buttons::in_attack2;
		else
			cmd.m_buttons |= valve::e_buttons::in_attack;

		cmd.m_tick = valve::to_ticks ( m_best_player->sim_time ( ) + g_context->net_info ( ).m_lerp );
	}
	__forceinline float dist_to ( vec3_t& from, const vec3_t& value ) { return ( from - value ).length ( ); }

	bool c_knife_bot::select_target ( )
	{
		float good_distance = 75.0f;

		for ( int i = 1; i <= valve___g_global_vars->m_max_clients; i++ )
		{
			valve::c_player* player = ( valve::c_player* )valve___g_entity_list->find_entity ( i );

			if ( !player || player == valve::g_local_player || player->dormant ( ) || !player->alive ( ) || player->team ( ) == valve::g_local_player->team ( ) )
				continue;

			vec3_t local_position = valve::g_local_player->origin ( );
			local_position.z += 50.0f;

			vec3_t entity_position = player->abs_origin ( );
			entity_position.z += 50.0f;

			float current_distance = dist_to ( local_position, entity_position );

			if ( current_distance < good_distance )
			{
				good_distance = current_distance;
				m_best_index = i;
				m_best_player = player;
			}
		}

		m_best_distance = good_distance;

		return m_best_index != -1;
	}
}