#include "../../../nem.hpp"

namespace nem::hacks {
	void c_eng_pred::prepare( ) {

		if ( m_last_frame_stage == valve::e_frame_stage::net_update_end ) {
			valve::g_prediction->update(
				valve::g_client_state->m_delta_tick,
				valve::g_client_state->m_delta_tick > 0,
				valve::g_client_state->m_last_cmd_ack,
				valve::g_client_state->m_last_cmd_out + valve::g_client_state->m_choked_cmds
			);
		}

		if ( m_net_velocity_modifier < 1.f )
			valve___g_prediction->m_prev_ack_had_errors = true;

		if ( m_is_out_of_epsilon ) {
			valve___g_prediction->m_cmds_predicted = 0;
			valve___g_prediction->m_prev_ack_had_errors = true;
		}

		if ( valve___g_client_state->m_delta_tick > 0 )
			m_is_out_of_epsilon = false;

		m_backup.m_cur_time = valve::g_global_vars->m_cur_time;
		m_backup.m_frame_time = valve::g_global_vars->m_frame_time;

		valve::g_global_vars->m_cur_time = valve::to_time( valve::g_local_player->tick_base( ) );
		valve::g_global_vars->m_frame_time = valve::g_global_vars->m_interval_per_tick;
	}

	void c_eng_pred::process ( valve::user_cmd_t* const user_cmd, bool init_data ) {
		m_local_data.at ( user_cmd->m_number % 150 ).init ( *user_cmd );

		predict ( user_cmd, init_data );
	}

	void c_eng_pred::predict( cc_def ( valve::user_cmd_t* ) user_cmd, bool init_data ) {

		if ( init_data )
		if ( user_cmd.get ( )->m_number == -1 )
			return;

		valve::g_prediction->update(
			valve::g_client_state->m_delta_tick,
			valve::g_client_state->m_delta_tick > 0,
			valve::g_client_state->m_last_cmd_ack,
			valve::g_client_state->m_last_cmd_out + valve::g_client_state->m_choked_cmds
		);

		g_context->addresses ( ).m_pred_player = valve::g_local_player.operator nem::valve::c_player * ( );
		*g_context->addresses ( ).m_pred_seed.get ( ) = user_cmd.get ( )->m_random_seed;

		valve::g_local_player->set_local_view_angles( user_cmd.get ( )->m_view_angles );

		valve::g_local_player->cur_user_cmd( ) = user_cmd.get ( );
		valve::g_local_player->last_user_cmd( ) = *user_cmd.get ( );

		const auto backup_in_prediction = valve::g_prediction->m_in_prediction;
		const auto backup_first_time_predicted = valve::g_prediction->m_first_time_predicted;

		valve___g_prediction->m_in_prediction = true;
		valve___g_prediction->m_first_time_predicted = false;

		valve___g_move_helper->set_host( valve::g_local_player );

		valve___g_movement->start_track_prediction_errors( valve::g_local_player );

		const auto backup_velocity_modifier = valve::g_local_player->velocity_modifier( );
		const auto tick_base_backup = valve::g_local_player->tick_base( );

		valve___g_prediction->setup_move( valve::g_local_player, user_cmd.get ( ), valve::g_move_helper, &m_move_data );

		m_move_data.m_move = user_cmd.get( )->m_move;
		m_move_data.m_buttons = user_cmd.get( )->m_buttons;
		m_move_data.m_view_angles = user_cmd.get( )->m_view_angles;
		m_move_data.m_angles = user_cmd.get( )->m_view_angles;
		m_move_data.m_impulse_command = user_cmd.get( )->m_impulse;

		valve___g_movement->process_movement( valve::g_local_player, &m_move_data );

		valve::g_local_player->tick_base( ) = tick_base_backup;

		valve___g_prediction->finish_move( valve::g_local_player, user_cmd.get ( ), &m_move_data );

		valve::g_local_player->set_abs_origin(
			valve::g_local_player->origin( ) );

		valve___g_movement->finish_track_prediction_errors( valve::g_local_player );

		valve___g_move_helper->set_host( nullptr );

		if ( const auto weapon = valve::g_local_player->weapon( ) ) {	
			weapon->update_inaccuracy( );

			m_inaccuracy = weapon->inaccuracy( );
			m_spread = weapon->spread( );

			const auto item_index = weapon->item_index( );
			const auto v11 =
				item_index == valve::e_item_index::awp || item_index == valve::e_item_index::g3sg1
				|| item_index == valve::e_item_index::scar20 || item_index == valve::e_item_index::ssg08;
			const auto wpn_data = weapon->wpn_data( );

			if ( valve::g_local_player->flags( ) & valve::e_ent_flags::ducking )
				m_min_inaccuracy = v11 ? wpn_data->m_inaccuracy_crouch_alt : wpn_data->m_inaccuracy_crouch;
			else
				m_min_inaccuracy = v11 ? wpn_data->m_inaccuracy_stand_alt : wpn_data->m_inaccuracy_stand;
		}
		else
			m_inaccuracy = m_spread = crypt_float ( 0.f );

		m_local_data.at( user_cmd.get( )->m_number % 150 ).save( user_cmd.get( )->m_number );

		valve::g_local_player->velocity_modifier( ) = backup_velocity_modifier; 

		valve::g_prediction->m_in_prediction = backup_in_prediction;
		valve::g_prediction->m_first_time_predicted = backup_first_time_predicted;

		update_shoot_pos( *user_cmd.get ( ) );
	}

	void c_eng_pred::restore( ) {
		g_context->addresses( ).m_pred_player = nullptr;
		*g_context->addresses( ).m_pred_seed.get( ) = -1;

		valve::g_global_vars->m_cur_time = m_backup.m_cur_time;
		valve::g_global_vars->m_frame_time = m_backup.m_frame_time;
	}

	void c_eng_pred::update_shoot_pos( const valve::user_cmd_t& user_cmd ) const {
		const auto anim_state = valve::g_local_player->anim_state( );
		if ( !anim_state )
			return;

		valve::g_local_player->set_abs_angles( { 0.f, anim_state->m_foot_yaw, 0.f } );

		float old_body_pitch = valve::g_local_player->pose_params( ).at( 12u );
		valve::g_local_player->pose_params( ).at( 12u ) = ( user_cmd.m_view_angles.x + 90.0f ) / 180.0f;

		auto& bones = g_anim_sync->local_data( ).m_shoot_pos_bones;
		g_anim_sync->setup_bones( valve::g_local_player, bones, valve::g_global_vars->m_cur_time, crypt_int ( 15 ) );
		valve::g_local_player->pose_params( ).at( 12u ) = old_body_pitch;

		g_context->shoot_pos( ) = valve___g_local_player->get_shoot_pos ( bones, false );
	}

	void c_eng_pred::on_packet_update ( std::uintptr_t cl_state ) {

	}
}