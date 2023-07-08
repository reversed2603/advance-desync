#pragma once

#include "../anti_aim.hpp"
#include "../../aim_bot/aim_bot.hpp"
namespace nem::hacks {
	__forceinline bool c_anti_aim::enabled( const valve::user_cmd_t* const user_cmd ) const {
		const auto cur_move_type = valve___g_local_player->move_type( );

		valve::e_move_type cmd_move_type{}, cmd_pred_move_type{};
		if ( user_cmd ) {

			cmd_move_type = valve::g_local_player->move_type( );
			cmd_pred_move_type = valve::g_local_player->move_type( );
		}

		return hacks::g_aim_bot->cfg ( ).m_bAntiAimEnabled
			&& cur_move_type != valve::e_move_type::noclip
			&& cur_move_type != valve::e_move_type::ladder
			&& ( !user_cmd ||
				( cmd_move_type != valve::e_move_type::ladder
					&& cmd_pred_move_type != valve::e_move_type::ladder ) ) // user_cmd->m_view_angles.y != 58.f
			&& !( valve___g_local_player->flags( ) & valve::e_ent_flags::frozen )
			&& !( g_context->flags( ) & e_context_flags::aim_fire )
			&& !g_context->freeze_time( );
	}
}