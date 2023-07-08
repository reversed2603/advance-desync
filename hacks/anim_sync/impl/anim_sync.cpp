#include "../../../nem.hpp"

namespace nem::hacks {

	void c_anim_sync::interp_air_data ( player_entry_t& entry, cc_def ( lag_record_t* ) current,
		cc_def ( lag_anim_data_t* ) previous ) const
	{
		if ( current.get ( )->m_flags & valve::e_ent_flags::on_ground ) { // store data for land sync
			if ( current.get ( )->m_anim_layers.at ( 5 ).m_weight > crypt_float ( 0.f )
				&& previous.get ( )->m_anim_layers.at ( 5 ).m_weight <= crypt_float ( 0.f ) &&
				 !( previous.get ( )->m_flags & valve::e_ent_flags::on_ground ) ) {
				const auto& land_seq = current.get ( )->m_anim_layers.at ( 5 ).m_sequence;

				if ( land_seq >= 2 ) { 
					const auto& land_act = entry.m_player->lookup_seq_act ( land_seq );

					if ( land_act == crypt_int ( ACT_LAND_LIGHT ) ||
						 land_act == crypt_int ( ACT_LAND_HEAVY ) ) {
						
						const auto& cur_cycle = current.get ( )->m_anim_layers.at ( 5 ).m_cycle;
						const auto& cur_rate = current.get ( )->m_anim_layers.at ( 5 ).m_playback_rate;

						if ( cur_cycle != crypt_float ( 0.f ) &&
							 cur_rate != crypt_float ( 0.f ) ) {
							
							const auto land_time = cur_cycle / cur_rate;
							
							if ( land_time != crypt_float ( 0.f ) ) {
								current.get ( )->m_on_ground = true;
								current.get ( )->m_act_time = current.get ( )->m_sim_time - land_time;
							}
						}
					}
				}

				if ( entry.m_player->lookup_seq_act( current.get( )->m_anim_layers.at( 4u ).m_sequence ) == crypt_int ( 985 ) ) {
					const auto update_time = ( valve::to_ticks( ( ( current.get( )->m_old_sim_time + valve___g_global_vars->m_interval_per_tick ) ) ) * valve___g_global_vars->m_interval_per_tick ) - ( ( ( (
						current.get( )->m_anim_layers.at( 4u ).m_cycle / current.get( )->m_anim_layers.at( 4u ).m_playback_rate ) / valve___g_global_vars->m_interval_per_tick
						) + 0.5f
						) * valve___g_global_vars->m_interval_per_tick );

					if ( update_time > entry.m_player->anim_state( )->m_prev_update_time ) {
						entry.m_player->anim_state( )->m_on_ground = false;
						entry.m_player->pose_params( ).at( 6u ) = crypt_float( 0.f );
						entry.m_player->anim_state( )->m_time_since_in_air = crypt_float( 0.f );
						entry.m_player->anim_state( )->m_prev_update_time = update_time;
					}
				}

				current.get ( )->m_velocity.z = crypt_float ( 0.f );
			}
		}
		else { 
			const auto jump_seq = current.get( )->m_anim_layers.at( 4u ).m_sequence;

			if ( !( previous.get ( )->m_flags & valve::e_ent_flags::on_ground ) ) {
				const auto& jump_seq = current.get ( )->m_anim_layers.at ( 4 ).m_sequence;

				if ( jump_seq >= 2 ) { 
					const auto jump_act = entry.m_player->lookup_seq_act ( jump_seq );

					if ( jump_act == crypt_int ( ACT_JUMP ) ) {
						const auto& cur_cycle = current.get ( )->m_anim_layers.at ( 4 ).m_cycle;
						const auto& cur_rate = current.get ( )->m_anim_layers.at ( 4 ).m_playback_rate;

						if ( cur_cycle != crypt_float ( 0.f ) &&
							cur_rate != crypt_float ( 0.f ) ) {

							const auto jump_time = cur_cycle / cur_rate;

							if ( jump_time != crypt_float ( 0.f ) ) {
								current.get ( )->m_on_ground = false;
								current.get ( )->m_act_time = current.get ( )->m_sim_time - jump_time;
							}
						}
					}
				}
			}

			if ( entry.m_player->lookup_seq_act( jump_seq ) == crypt_int ( 985 ) ) {
				if ( current.get( )->m_anim_layers.at( 4u ).m_weight > 0.f
					&& current.get( )->m_anim_layers.at( 4u ).m_playback_rate > 0.f ) {
					const auto jump_time = ( ( ( current.get( )->m_anim_layers.at( 4u ).m_cycle / current.get( )->m_anim_layers.at( 4u ).m_playback_rate )
						/ valve___g_global_vars->m_interval_per_tick ) + crypt_float ( 0.5f ) ) * valve___g_global_vars->m_interval_per_tick;

					static auto sv_gravity = valve::g_cvar->find_var( xorstr_( "sv_gravity" ) );
					static auto sv_jump_impulse = valve::g_cvar->find_var( xorstr_( "sv_jump_impulse" ) );

					current.get( )->m_velocity.z = sv_jump_impulse->get_float( ) - sv_gravity->get_float( ) * jump_time;
				}
			}
		}

		if ( !( current.get ( )->m_flags & valve::e_ent_flags::on_ground ) ) {
			if ( current.get ( )->m_anim_layers.at ( 4 ).m_weight != crypt_float ( 0.f )
				&& current.get ( )->m_anim_layers.at ( 4 ).m_playback_rate != crypt_float ( 0.f ) ) {
				const auto& cur_seq = current.get ( )->m_anim_layers.at ( 4 ).m_sequence;

				if ( entry.m_player->lookup_seq_act ( cur_seq ) == crypt_int ( 985 ) ) {
					const auto& cur_cycle = current.get ( )->m_anim_layers.at ( 4 ).m_cycle;
					const auto& previous_cycle = previous.get ( )->m_anim_layers.at ( 4 ).m_cycle;

					const auto previous_seq = previous.get ( )->m_anim_layers.at ( 4 ).m_sequence;

					if ( ( cur_cycle != previous_cycle || previous_seq != cur_seq )
						&& previous_cycle > cur_cycle ) {
						entry.m_player->pose_params ( ).at ( 6 ) = crypt_float ( 0.f );
						entry.m_player->anim_state ( )->m_time_since_in_air = cur_cycle / current.get ( )->m_anim_layers.at ( 4 ).m_playback_rate;
					}
				}
			}
		}
	}

	void c_anim_sync::process_air_interp ( player_entry_t& entry, lag_record_t* const current,
		const lag_anim_data_t* const previous, const int anim_tick ) const {

		if ( current->m_on_ground.has_value ( ) ) { 
			if ( !current->m_on_ground.value ( ) ) { 
				const auto jump_tick = valve::to_ticks ( current->m_act_time ) + crypt_int ( 1 );

				if ( jump_tick == anim_tick - 1 )
					current->m_interp_flags |= valve::e_ent_flags::on_ground; // not jumped yet
				else if ( jump_tick == anim_tick ) {		
					entry.m_player->anim_layers ( ).at ( 4 ).m_cycle = 0.f;
					entry.m_player->anim_layers ( ).at ( 4 ).m_weight = 0.f;
					entry.m_player->anim_layers ( ).at ( 4 ).m_playback_rate = current->m_anim_layers.at ( 4 ).m_playback_rate;
					entry.m_player->anim_layers ( ).at ( 4 ).m_sequence = current->m_anim_layers.at ( 4 ).m_sequence;
					current->m_interp_flags &= ~valve::e_ent_flags::on_ground; // alright, player jumped - remove on_ground flag
				}
			}
			else {
				const auto land_tick = valve::to_ticks ( current->m_act_time ) + crypt_int ( 1 );

				if ( land_tick == anim_tick - 1 )
					current->m_interp_flags &= ~valve::e_ent_flags::on_ground; // not landed yet
				else if ( land_tick == anim_tick ) { 

					entry.m_player->anim_layers ( ).at ( 5 ).m_cycle = 0.f;
					entry.m_player->anim_layers ( ).at ( 5 ).m_weight = 0.f;
					entry.m_player->anim_layers ( ).at ( 5 ).m_playback_rate = current->m_anim_layers.at ( 5 ).m_playback_rate;
					entry.m_player->anim_layers ( ).at ( 5 ).m_sequence = current->m_anim_layers.at ( 5 ).m_sequence;

					current->m_interp_flags |= valve::e_ent_flags::on_ground; // alright, player landed - add on_ground flag
				}
			}
		}
	}

	void c_anim_sync::interp_anim_data ( player_entry_t& entry, cc_def ( lag_record_t* ) current, cc_def ( lag_anim_data_t* ) previous ) const {
		entry.m_player->set_abs_origin ( current.get ( )->m_origin );

		current.get ( )->m_rot_multipl = crypt_float ( 1.f );

		if ( current.get ( )->m_total_cmds >= crypt_int ( 2 )
			&& previous.get ( )
			&& !current.get ( )->m_entry_after_dormant ) {

			if ( current.get ( )->m_shot ) {
				entry.m_player->lby ( ) = previous.get ( )->m_lby;
			}

			current.get ( )->m_duck_amt_delta = current.get ( )->m_duck_amount - previous.get ( )->m_duck_amount;
			current.get ( )->m_lby_delta = std::abs ( math::angle_diff ( current.get ( )->m_eye_angles.y, current.get ( )->m_lby ) );
			current.get ( )->m_eye_ang_delta = current.get ( )->m_eye_angles - previous.get ( )->m_eye_angles;
			current.get ( )->m_velocity_delta = current.get ( )->m_velocity - previous.get ( )->m_velocity;
			current.get( )->m_view_offset_delta = current.get( )->m_view_offset - previous.get( )->m_view_offset;

			interp_air_data ( entry, current, previous );
		}
	}

	void c_anim_sync::update(
		player_entry_t& entry, cc_def ( lag_record_t* ) current, cc_def ( lag_anim_data_t* ) previous, const int side
	) const {
		const auto anim_state = entry.m_player->anim_state( );
		const auto backup_move_weight = anim_state->m_feet_weight;
		const auto backup_primary_cycle = anim_state->m_feet_cycle;
		if ( previous.get ( )
			&& !current.get ( )->m_entry_after_dormant ) {
			entry.m_player->anim_layers( ) = previous.get ( )->m_anim_layers;
			auto& prev_anim_side = previous.get ( )->m_anim_sides.at( side );

			anim_state->m_foot_yaw = prev_anim_side.m_foot_yaw;
			anim_state->m_prev_foot_yaw = prev_anim_side.m_prev_foot_yaw;
			anim_state->m_move_yaw = prev_anim_side.m_move_yaw;
			anim_state->m_move_yaw_cur_to_ideal = prev_anim_side.m_move_yaw_cur_to_ideal;
			anim_state->m_move_yaw_ideal = prev_anim_side.m_move_yaw_ideal;
			anim_state->m_move_weight_smoothed = prev_anim_side.m_move_weight_smoothed;

			anim_state->m_feet_cycle = previous.get ( )->m_anim_layers.at( 6u ).m_cycle;
			anim_state->m_feet_weight = previous.get ( )->m_anim_layers.at ( 6u ).m_weight;

			const auto& layer7 = previous.get ( )->m_anim_layers.at( 7u );

			anim_state->m_strafe_weight = layer7.m_weight;
			anim_state->m_strafe_sequence = layer7.m_sequence;
			anim_state->m_strafe_cycle = layer7.m_cycle;	
			anim_state->m_acceleration_weight = previous.get ( )->m_anim_layers.at ( 12u ).m_weight;
		}
		else {
			if ( current.get ( )->m_flags & valve::e_ent_flags::on_ground )
			{
				anim_state->m_on_ground = true;
				anim_state->m_landing = false;
			}

			anim_state->m_time_since_in_air = 0.f;
			anim_state->m_feet_cycle = current.get ( )->m_anim_layers.at ( 6u ).m_cycle;
			anim_state->m_feet_weight = current.get ( )->m_anim_layers.at ( 6u ).m_weight;
			anim_state->m_strafe_weight = current.get ( )->m_anim_layers.at ( 7u ).m_weight;
			anim_state->m_strafe_sequence = current.get ( )->m_anim_layers.at ( 7u ).m_sequence;
			anim_state->m_strafe_cycle = current.get ( )->m_anim_layers.at ( 7u ).m_cycle;
			anim_state->m_acceleration_weight = current.get ( )->m_anim_layers.at ( 12u ).m_weight;

			entry.m_player->pose_params ( ).at ( 6 ) = 0.f;
			entry.m_player->anim_layers ( ) = current.get ( )->m_anim_layers;

			auto walk_run_transition = current.get ( )->m_velocity.length_2d ( ) <= crypt_float ( 135.f ) ? 0.f : 1.f;

			anim_state->m_walk_to_run_transition = walk_run_transition;
			anim_state->m_prev_update_time = current.get ( )->m_sim_time - valve___g_global_vars->m_interval_per_tick;
		}

		struct backup_t {
			__forceinline constexpr backup_t( ) = default;

			__forceinline backup_t ( valve::c_player* const player, lag_record_t* record )
				: m_cur_time { valve___g_global_vars->m_cur_time },
				m_frame_time { valve___g_global_vars->m_frame_time },
				m_real_time { valve___g_global_vars->m_real_time }, m_abs_frame_time { valve___g_global_vars->m_absolute_frame_time },
				m_frame_count { valve___g_global_vars->m_frame_count }, m_tick_count { valve___g_global_vars->m_tick_count }, m_interp_amt { valve___g_global_vars->m_interpolation_amount },
				  m_duck_amount{ player->duck_amount( ) }, m_eye_yaw{ player->eye_angles( ) },
				  m_eflags{ player->eflags( ) }, m_flags{ player->flags( ) },
				m_velocity { player->velocity ( ) }, m_lby { player->lby ( ) }, m_abs_velocity { player->abs_velocity ( ) } {}

			__forceinline void restore( valve::c_player* const player, lag_record_t* record ) const {
				valve___g_global_vars->m_cur_time = m_cur_time;				
				valve___g_global_vars->m_frame_time = m_frame_time;
				valve___g_global_vars->m_real_time = m_real_time;
				valve___g_global_vars->m_absolute_frame_time = m_abs_frame_time;
				valve___g_global_vars->m_frame_count = m_frame_count;
				valve___g_global_vars->m_tick_count = m_tick_count;
				valve___g_global_vars->m_interpolation_amount = m_interp_amt;

				player->duck_amount( ) = m_duck_amount;
				player->eye_angles( ) = m_eye_yaw;

				player->eflags( ) = m_eflags;
				player->flags( ) = m_flags;
				player->lby ( ) = m_lby;
				player->velocity( ) = m_velocity;
				player->abs_velocity( ) = m_abs_velocity;
			}

			float				m_cur_time {}, m_frame_time {}, m_real_time { }, m_abs_frame_time{ }, 
				m_duck_amount {}, m_interp_amt { };

			vec3_t              m_eye_yaw {};

			int                 m_frame_count { }, m_tick_count { };

			std::size_t			m_eflags{};
			valve::e_ent_flags	m_flags{};
			float               m_lby {};
			vec3_t				m_velocity{}, m_abs_velocity{};
		} backup{ entry.m_player, current.get ( ) };


        if ( current.get ( )->m_total_cmds >= crypt_int ( 2 ) 
			&& previous.get( ) 
			&& !current.get( )->m_entry_after_dormant ) {
			current.get( )->m_is_jittering = std::abs( current.get( )->m_eye_angles.y - previous.get( )->m_eye_angles.y ) > 15.f;

			entry.m_player->anim_state( )->m_prev_update_time = current.get( )->m_anim_time;

			for ( std::size_t i = 1; i <= current.get ( )->m_total_cmds; ++i ) {

				auto anim_time = current.get ( )->m_anim_time + valve::to_time ( i );

				const auto server_time = i < current.get ( )->m_total_cmds ? anim_time : current.get ( )->m_sim_time;

				valve___g_global_vars->m_cur_time = server_time;
				valve___g_global_vars->m_real_time = server_time;
				valve___g_global_vars->m_frame_time = valve___g_global_vars->m_interval_per_tick;
				valve___g_global_vars->m_absolute_frame_time = valve___g_global_vars->m_interval_per_tick;
				valve___g_global_vars->m_frame_count = valve::to_ticks ( server_time );
				valve___g_global_vars->m_tick_count = valve::to_ticks ( server_time );
				valve___g_global_vars->m_interpolation_amount = 0.f;

				const auto lerp_tick = i / static_cast < float > ( current.get ( )->m_total_cmds );

				if ( i < current.get ( )->m_total_cmds ) {

					auto prev_lby = previous.get ( )->m_lby;
					auto cur_lby = current.get ( )->m_lby;

					if ( prev_lby != cur_lby ) {
						auto v40 = current.get ( )->m_total_cmds - i;
						bool use_new_lby = true;

						if ( current.get ( )->m_lby_delta < crypt_float ( 1.f ) )
							use_new_lby = v40 == crypt_int ( 0 );
						else
							use_new_lby = v40 < crypt_int ( 2 );

						if ( !use_new_lby )
							cur_lby = prev_lby;

						entry.m_player->lby ( ) = cur_lby;
					}

					current.get ( )->m_interp_flags = entry.m_player->flags ( );
					process_air_interp ( entry, current.get ( ), previous.get ( ), valve::to_ticks ( anim_time ) );

					entry.m_player->flags ( ) = current.get ( )->m_interp_flags;
					entry.m_player->duck_amount ( ) = previous.get ( )->m_duck_amount + current.get ( )->m_duck_amt_delta * lerp_tick;

					if ( entry.m_player->duck_amount( ) <= 0.75f )
						entry.m_player->flags( ) &= ~valve::e_ent_flags::ducking;
					else
						entry.m_player->flags( ) |= valve::e_ent_flags::ducking;

					entry.m_player->update_collision_bounds( );

					entry.m_player->velocity ( ) = entry.m_player->abs_velocity ( ) = previous.get ( )->m_velocity + current.get ( )->m_velocity_delta * lerp_tick;
					entry.m_player->eye_angles( ) = current.get( )->m_eye_angles;
					entry.m_player->eye_angles( ).z = 0.f;

					if ( side
						&& !current.get ( )->m_shot ) {
						auto rot_handle = crypt_float ( 60.f ) * current.get ( )->m_rot_multipl;

						if ( side == crypt_int ( 2 ) )
							anim_state->m_foot_yaw = math::normalize_yaw ( current.get ( )->m_eye_angles.y + rot_handle );
						else
							anim_state->m_foot_yaw = math::normalize_yaw ( current.get ( )->m_eye_angles.y - rot_handle );
					}
				}
				else {

					entry.m_player->flags ( ) = current.get ( )->m_flags;
					entry.m_player->duck_amount ( ) = current.get ( )->m_duck_amount;
					entry.m_player->velocity ( ) = entry.m_player->abs_velocity ( ) = current.get ( )->m_velocity;
					entry.m_player->eye_angles ( ) = current.get ( )->m_eye_angles;
					entry.m_player->eye_angles ( ).z = 0.f;
				}

				entry.m_player->eflags ( ) &= ~0x1000u;

				anim_state->m_prev_update_frame = valve::g_global_vars->m_frame_count - 1;

				const auto is_hltv = valve___g_client_state->m_is_hltv;
					
				entry.m_player->client_side_anim ( ) = g_context->allow_anim_update ( ) = true;
				valve___g_client_state->m_is_hltv = true;
				entry.m_player->update_client_side_anim ( );
				entry.m_player->client_side_anim ( ) = g_context->allow_anim_update ( ) = false;
				valve___g_client_state->m_is_hltv = is_hltv;
			}
		}
		else { 

			if ( entry.m_player->anim_state( )->m_prev_update_time == current.get ( )->m_sim_time )
				entry.m_player->anim_state( )->m_prev_update_time = current.get( )->m_sim_time + valve___g_global_vars->m_interval_per_tick;

			valve___g_global_vars->m_cur_time = current.get ( )->m_sim_time;
			valve___g_global_vars->m_real_time = current.get ( )->m_sim_time;
			valve___g_global_vars->m_frame_time = valve___g_global_vars->m_interval_per_tick;
			valve___g_global_vars->m_absolute_frame_time = valve___g_global_vars->m_interval_per_tick;
			valve___g_global_vars->m_frame_count = valve::to_ticks ( current.get ( )->m_sim_time );
			valve___g_global_vars->m_tick_count = valve::to_ticks ( current.get ( )->m_sim_time );
			valve___g_global_vars->m_interpolation_amount = 0.f;

			entry.m_player->eye_angles ( ) = current.get ( )->m_eye_angles;
			entry.m_player->eye_angles ( ).z = 0.f;
			entry.m_player->flags ( ) = current.get ( )->m_flags;
			entry.m_player->duck_amount ( ) = current.get ( )->m_duck_amount;
			entry.m_player->velocity ( ) = entry.m_player->abs_velocity ( ) = current.get ( )->m_velocity;

			if ( side ) {
				auto rot_handle = crypt_float ( 60.f ) * current.get ( )->m_rot_multipl;

				if ( side == 2 )
					anim_state->m_foot_yaw = math::normalize_yaw ( current.get ( )->m_eye_angles.y + rot_handle );
				else
					anim_state->m_foot_yaw = math::normalize_yaw ( current.get ( )->m_eye_angles.y - rot_handle );
			}

			entry.m_player->eflags ( ) &= ~0x1000u;

			anim_state->m_prev_update_frame = valve::g_global_vars->m_frame_count - 1;

			const auto is_hltv = valve___g_client_state->m_is_hltv;

			entry.m_player->client_side_anim ( ) = g_context->allow_anim_update ( ) = true;
			valve___g_client_state->m_is_hltv = true;
			entry.m_player->update_client_side_anim ( );
			entry.m_player->client_side_anim ( ) = g_context->allow_anim_update ( ) = false;
			valve___g_client_state->m_is_hltv = is_hltv;
		}

		backup.restore ( entry.m_player, current.get ( ) );
		entry.m_player->anim_state ( )->m_feet_cycle = backup_primary_cycle;
		entry.m_player->anim_state ( )->m_feet_weight = backup_move_weight;

		reinterpret_cast < void ( __thiscall* )( void*, int ) > ( g_context->addresses ( ).m_invalidate_physics_recursive.get ( ) )( entry.m_player, 0x8 );

		auto& cur_anim_side = current.get ( )->m_anim_sides.at( side );

		{
			cur_anim_side.m_foot_yaw = anim_state->m_foot_yaw;
			cur_anim_side.m_prev_foot_yaw = anim_state->m_prev_foot_yaw;
			cur_anim_side.m_move_yaw_ideal = anim_state->m_move_yaw_ideal;
			cur_anim_side.m_move_yaw_cur_to_ideal = anim_state->m_move_yaw_cur_to_ideal;
			cur_anim_side.m_move_yaw = anim_state->m_move_yaw;
			cur_anim_side.m_move_weight_smoothed = anim_state->m_move_weight_smoothed;
			cur_anim_side.m_air_smooth_value = anim_state->m_in_air_smooth_value;
			cur_anim_side.m_time_to_realign_body = anim_state->m_lower_body_realign_timer;

			cur_anim_side.m_anim_layers = entry.m_player->anim_layers( );
			cur_anim_side.m_pose_params = entry.m_player->pose_params ( );
		}

		entry.m_player->anim_layers( ) = current.get ( )->m_anim_layers;
		entry.m_player->set_abs_angles( { 0.f, anim_state->m_foot_yaw, 0.f } );

		setup_bones ( entry.m_player, cur_anim_side.m_bones, current.get ( )->m_sim_time, 25 );

		const auto mdl_data = entry.m_player->mdl_data( );
		if ( !mdl_data
			|| !mdl_data->m_studio_hdr )
			return;

		cur_anim_side.m_bones_count = mdl_data->m_studio_hdr->m_bones_count;		
	}

	void c_anim_sync::select_anim_side ( player_entry_t& entry,
		lag_record_t* const current, const lag_anim_data_t* const previous
	) const {
		current->m_anim_side = 0;
		current->m_solve_via_anim = false;

		if ( current->m_velocity.length_2d ( ) <= 0.1f )
			return;

		if ( !( current->m_flags & valve::e_ent_flags::on_ground )
			|| !( previous->m_flags & valve::e_ent_flags::on_ground ) )
			return;

		const auto not_accelerating = static_cast < int > ( current->m_anim_layers.at ( 6u ).m_weight * crypt_float ( 1000.f ) ) == static_cast < int > ( previous->m_anim_layers.at ( 6u ).m_weight * crypt_float ( 1000.f ) )
			&& !static_cast < int > ( current->m_anim_layers.at ( 12u ).m_weight * crypt_float ( 1000.f ) );

		if ( not_accelerating ) {
			const auto& move_layer = current->m_anim_layers.at ( 6u );

			if ( (current->m_velocity.length_2d ( ) <= 30.f
				&& current->m_velocity.length_2d ( ) > 15.f )
				|| ( current->m_velocity.length_2d ( ) < 120.f
				&& previous->m_velocity.length_2d ( ) >= 120.f
				&& previous->m_anim_layers.at ( 12u ).m_weight < current->m_anim_layers.at ( 12u ).m_weight ) )
				current->m_force_safe_points = true;
				
			const auto delta_0 = std::abs ( move_layer.m_playback_rate - current->m_anim_sides.at ( 0u ).m_anim_layers.at ( 6u ).m_playback_rate );
			const auto delta_1 = std::abs ( move_layer.m_playback_rate - current->m_anim_sides.at ( 1u ).m_anim_layers.at ( 6u ).m_playback_rate );
			const auto delta_2 = std::abs ( move_layer.m_playback_rate - current->m_anim_sides.at ( 2u ).m_anim_layers.at ( 6u ).m_playback_rate );

			/* solve method when something not legit is happening */
			if ( !static_cast < int > ( delta_0 * crypt_float ( 1000.f ) )
				&& delta_1 > 0.2f
				&& delta_2 == delta_1 ) {
				current->m_reforce_safe_points = true;
				current->m_anim_side = crypt_int ( 0 ); 
				current->m_solve_via_anim = true;
				current->m_r_mode = e_r_mode::r_anim_logic;
				return;
			}

			/* solve method when delta_1 is an actual fake */
			if ( ( delta_0 > delta_1
				|| delta_2 > delta_1 )
				&& static_cast < int > ( delta_2 * crypt_float ( 1000.f ) ) ) {
				if ( delta_2 <= delta_0
					|| !static_cast < int > ( delta_1 * crypt_float ( 1000.f ) ) ) {
					if ( static_cast < int > ( delta_1 * crypt_float ( 1000.f ) ) )
						current->m_reforce_safe_points = true;

					current->m_anim_side = crypt_int ( 1 );
					current->m_solve_via_anim = true;
					current->m_r_mode = e_r_mode::r_anim_logic;
				}
			}
			else {
				/* solve method when delta_2 is an actual fake */
				if ( delta_2 < delta_1
					|| !static_cast < int > ( delta_2 * crypt_float ( 1000.f ) ) ) {
					if ( static_cast < int > ( delta_2 * crypt_float ( 1000.f ) ) )
						current->m_reforce_safe_points = true;

					current->m_anim_side = crypt_int ( 2 );
					current->m_solve_via_anim = true;
					current->m_r_mode = e_r_mode::r_anim_logic;
				}
			}
		}
	}

	void c_anim_sync::find_server_foot_yaw(
		player_entry_t& entry,
		lag_record_t* const current, const lag_anim_data_t* const previous
	) const {
		const auto& right_side = current->m_anim_sides.at ( r_side );

		auto delta = math::angle_diff ( right_side.m_foot_yaw, current->m_eye_angles.y );
		bool negative_delta = delta < 0.0f;

		if ( negative_delta ) {
			std::swap ( current->m_anim_sides.at ( r_side ).m_anim_layers, current->m_anim_sides.at ( l_side ).m_anim_layers );
			std::swap ( current->m_anim_sides.at ( r_side ).m_pose_params, current->m_anim_sides.at ( l_side ).m_pose_params );
			std::swap ( current->m_anim_sides.at ( r_side ).m_foot_yaw, current->m_anim_sides.at ( l_side ).m_foot_yaw );
			std::swap ( current->m_anim_sides.at ( r_side ).m_bones, current->m_anim_sides.at ( l_side ).m_bones );
		}

		auto speed_2d = current->m_velocity.length_2d ( );

		if ( entry.m_auto_wall_misses >= 2
			|| entry.m_anim_misses >= 2
			|| entry.m_stand_misses >= 2
			|| !previous ) {

			if ( !previous )
				current->m_reforce_safe_points = true;
			else
				current->m_force_safe_points = true;

			return;
		}

		if ( current->m_sim_ticks <= 0
			|| current->m_total_cmds <= 1
			|| !( current->m_flags & valve::e_ent_flags::on_ground )
			|| !( previous->m_flags & valve::e_ent_flags::on_ground ) 
			|| current->m_entry_after_dormant )
		{
			current->m_reforce_safe_points = true;
			return;
		}

		if ( current->m_eye_angles.y != previous->m_eye_angles.y && current->m_lby_delta != previous->m_lby_delta ) {
			current->m_reforce_safe_points = true;
		}

		if ( std::abs ( math::angle_diff ( previous->m_eye_angles.y, current->m_eye_angles.y ) ) > crypt_float ( 35.f ) )
			current->m_force_safe_points = true;

		select_anim_side ( entry, current, previous );

		if ( !current->m_solve_via_anim ) {				
			if ( speed_2d <= crypt_float ( 0.1f ) ) {
				current->m_reforce_safe_points = true;
				current->m_anim_side = math::angle_diff ( current->m_eye_angles.y, entry.m_player->lby ( ) ) <= crypt_float ( 0.f ) ? l_side : r_side;
			}
		}
	}

	void c_anim_sync::setup_local_bones( ) {
		const auto anim_state = valve___g_local_player->anim_state( );
		if ( !anim_state )
			return;

		const auto& local_data = g_eng_pred->local_data( ).at( g_context->last_sent_cmd_number( ) % 150 );
		if ( local_data.m_spawn_time != valve___g_local_player->spawn_time( ) )
			return;

		struct anim_backup_t {
			__forceinline anim_backup_t( )
				: m_anim_state{ *valve___g_local_player->anim_state( ) },
				m_abs_yaw{ m_anim_state.m_foot_yaw },
				m_anim_layers{ valve___g_local_player->anim_layers( ) },
				m_pose_params{ valve___g_local_player->pose_params( ) } {}

			__forceinline void restore( ) const {
				valve___g_local_player->set_abs_angles( { 0.f, m_abs_yaw, 0.f } );

				*valve___g_local_player->anim_state( ) = m_anim_state;

				valve___g_local_player->anim_layers( ) = m_anim_layers;
				valve___g_local_player->pose_params( ) = m_pose_params;
			}

			valve::anim_state_t		m_anim_state{};

			float					m_abs_yaw{};

			valve::anim_layers_t	m_anim_layers{};
			valve::pose_params_t	m_pose_params{};
		} anim_backup{};

		local_data.restore_anim( true );

		if ( m_local_data.m_fake.m_spawn_time == 0.f
			|| m_local_data.m_fake.m_spawn_time != valve___g_local_player->spawn_time( ) ) {
			m_local_data.m_fake.m_anim_state = *valve___g_local_player->anim_state( );

			m_local_data.m_fake.m_spawn_time = valve___g_local_player->spawn_time( );
		}

		if ( !valve___g_client_state->m_choked_cmds
			&& valve___g_global_vars->m_cur_time != m_local_data.m_fake.m_anim_state.m_prev_update_time ) {
			*valve___g_local_player->anim_state( ) = m_local_data.m_fake.m_anim_state;

			anim_state->m_foot_yaw = local_data.m_user_cmd.m_view_angles.y;

			anim_state->update(
				local_data.m_user_cmd.m_view_angles.x,
				local_data.m_user_cmd.m_view_angles.y, 0.f
			);

			m_local_data.m_fake.m_abs_yaw = anim_state->m_foot_yaw;
			m_local_data.m_fake.m_anim_state = *valve___g_local_player->anim_state( );
			m_local_data.m_fake.m_anim_layers = valve___g_local_player->anim_layers( );
			m_local_data.m_fake.m_pose_params = valve___g_local_player->pose_params( );
		}

		local_data.restore_anim( true );

		valve___g_local_player->anim_layers( ) = m_local_data.m_fake.m_anim_layers;
		valve___g_local_player->pose_params( ) = m_local_data.m_fake.m_pose_params;

		valve___g_local_player->set_abs_angles( { 0.f, m_local_data.m_fake.m_abs_yaw, 0.f } );

		setup_bones( valve___g_local_player, m_local_data.m_fake.m_bones, valve___g_global_vars->m_cur_time, 11 );

		local_data.restore_anim( true );

		setup_bones( valve___g_local_player, m_local_data.m_real.m_bones, valve___g_global_vars->m_cur_time, 27 );

		anim_backup.restore( );
	}

	void c_anim_sync::setup_enemy_bones ( valve::c_player* const player,
		valve::bones_t& bones, const float time ) const {
	}

	bool c_anim_sync::setup_bones(
		valve::c_player* const player,
		valve::bones_t& bones, const float time, const int flags
	) const {
		struct backup_t {
			__forceinline constexpr backup_t( ) = default;

			__forceinline backup_t( valve::c_player* const player )
				: m_cur_time{ valve___g_global_vars->m_cur_time },
				  m_frame_time{ valve___g_global_vars->m_frame_time },
				  m_frame_count{ valve___g_global_vars->m_frame_count },
				  m_real_time { valve___g_global_vars->m_real_time },
				  m_tick_count { valve___g_global_vars->m_tick_count },
				  m_abs_frame_time { valve___g_global_vars->m_absolute_frame_time },
				  m_interp_amt { valve___g_global_vars->m_interpolation_amount },
				  m_occlusion_frame{ player->occlusion_frame( ) },
				  m_ent_client_flags{ player->ent_client_flags( ) },
				  m_ik_context{ player->ik_context( ) }, m_effects{ player->effects( ) },
				m_occlusion_flags { player->occlusion_flags ( ) } {}

			float					m_cur_time{}, m_frame_time{}, m_real_time{}, m_abs_frame_time{}, m_interp_amt{ };
			int						m_frame_count{}, m_occlusion_frame{}, m_tick_count{};
			std::uint8_t			m_ent_client_flags{};
			valve::ik_context_t*	m_ik_context{};

			std::size_t				m_effects{}, m_occlusion_flags{};
		} backup{ player };

		g_context->force_bone_mask ( ) = flags & 4;
		valve___g_global_vars->m_cur_time = time;
		valve___g_global_vars->m_real_time = time;
		valve___g_global_vars->m_frame_time = valve___g_global_vars->m_interval_per_tick;
		valve___g_global_vars->m_absolute_frame_time = valve___g_global_vars->m_interval_per_tick;
		valve___g_global_vars->m_frame_count = valve::to_ticks ( time );
		valve___g_global_vars->m_tick_count = valve::to_ticks ( time );
		valve___g_global_vars->m_interpolation_amount = 0.f;

		if ( flags & 8 ) {
			player->effects( ) |= 8u;
			player->occlusion_flags( ) &= ~0xau;
			player->occlusion_frame( ) = 0;
		}

		if ( flags & 4 ) {
			player->ik_context( ) = nullptr;
			player->ent_client_flags( ) |= 2u;
		}	

		if ( flags & 2 )
			player->last_setup_bones_frame( ) = 0;

		if ( flags & 1 ) {
			player->mdl_bone_counter( ) = 0ul;
			player->last_setup_bones_time( ) = std::numeric_limits< float >::lowest( );

			auto& bone_accessor = player->bone_accessor( );

			bone_accessor.m_writable_bones = bone_accessor.m_readable_bones = 0;
		}

		auto& jiggle_bones = g_context->cvars( ).m_r_jiggle_bones;

		const auto backup_jiggle_bones = jiggle_bones->get_bool( );

		jiggle_bones->set_int( false );

		g_context->allow_setup_bones( ) = true;
		g_context->is_bone_available ( ) = true;
		const auto ret = player->setup_bones( bones.data( ), 128, ( ( ( flags >> 4 ) & 1 ) << 9 ) + 0xffd00, time );
		g_context->is_bone_available ( ) = false;
		g_context->allow_setup_bones( ) = false;

		if ( flags & 4 ) {
			player->ik_context ( ) = backup.m_ik_context;
		}

		jiggle_bones->set_int( backup_jiggle_bones );

		if ( flags & 8 ) {
			player->effects( ) = backup.m_effects;
			player->occlusion_flags( ) = backup.m_occlusion_flags;
			player->occlusion_frame( ) = backup.m_occlusion_frame;
		}

		if ( flags & 4 ) {
			player->ent_client_flags( ) = backup.m_ent_client_flags;
		}

		valve___g_global_vars->m_cur_time = backup.m_cur_time;
		valve___g_global_vars->m_real_time = backup.m_real_time;
		valve___g_global_vars->m_absolute_frame_time = backup.m_abs_frame_time;
		valve___g_global_vars->m_frame_time = backup.m_frame_time;
		valve___g_global_vars->m_tick_count = backup.m_tick_count;
		valve___g_global_vars->m_frame_count = backup.m_frame_count;
		valve___g_global_vars->m_interpolation_amount = backup.m_interp_amt;

		if ( !( flags & 4 ) )
			return ret;

		const auto mdl_data = player->mdl_data( );
		if ( !mdl_data
			|| !mdl_data->m_studio_hdr )
			return ret;

		const auto hitbox_set = mdl_data->m_studio_hdr->hitbox_set( player->hitbox_set_index( ) );
		if ( !hitbox_set )
			return ret;

		for ( int i{}; i < hitbox_set->m_hitboxes_count; ++i ) {
			const auto hitbox = hitbox_set->hitbox( i );
			if ( !hitbox
				|| hitbox->m_radius >= 0.f )
				continue;

			mat3x4_t rot_mat{};
			g_context->addresses( ).m_angle_matrix.get ( )( hitbox->m_rotation, rot_mat );

			math::concat_transforms(
				bones[ hitbox->m_bone ], rot_mat, bones.at( hitbox->m_bone )
			);
		}

		return ret;
	}

	void c_anim_sync::on_net_update(
		player_entry_t& entry,
		cc_def ( lag_record_t* ) current, cc_def ( lag_anim_data_t* ) previous
	) const {
		struct anim_backup_t {
			__forceinline constexpr anim_backup_t( ) = default;

			__forceinline anim_backup_t( valve::c_player* const player, lag_record_t* const current )
				: m_anim_state{ *player->anim_state( ) }, m_abs_yaw{ m_anim_state.m_foot_yaw },
				  m_anim_layers{ current->m_anim_layers }, m_pose_params{ player->pose_params( ) } {}

			__forceinline void restore( valve::c_player* const player, bool restore_state ) const {

				if ( restore_state ) {
					*player->anim_state ( ) = m_anim_state;
					player->pose_params ( ) = m_pose_params;
				}
				player->set_abs_angles( { 0.f, m_abs_yaw, 0.f } );

				player->anim_layers( ) = m_anim_layers;
			}

			valve::anim_state_t		m_anim_state{};

			float					m_abs_yaw{};

			valve::anim_layers_t	m_anim_layers{};
			valve::pose_params_t	m_pose_params{};
		} anim_backup{ entry.m_player, current.get ( ) };

		interp_anim_data ( entry, current, previous );

		update ( entry, current, previous, 0 );

		anim_backup.restore ( entry.m_player, true );

		if ( !entry.m_player->friendly( ) )
		{
			const auto player_info = valve___g_engine->player_info( entry.m_player->index( ) );
			if ( player_info.has_value( ) ) {
				update( entry, current, previous, 2 );

				anim_backup.restore( entry.m_player, true );

				update( entry, current, previous, 1 );

				anim_backup.restore( entry.m_player, false );

				current.get ( )->m_trying_to_resolve = true;
			}
		}

		if ( current.get ( )->m_trying_to_resolve )
			find_server_foot_yaw( entry, current.get ( ), previous.get ( ) );

		entry.m_player->anim_layers( ) = current.get ( )->m_anim_layers;

		const auto& anim_side = current.get ( )->m_anim_sides.at ( current.get ( )->m_anim_side );

		entry.m_player->pose_params ( ) = anim_side.m_pose_params;

		for ( int i = 0; i < 128; ++i ) {
			entry.m_bone_origins.at ( i ) = entry.m_player->abs_origin ( ) - anim_side.m_bones.at ( i ).get_origin ( );
		}
		
		std::memcpy ( entry.m_bones.data ( ), anim_side.m_bones.data ( ), anim_side.m_bones_count * sizeof ( mat3x4_t )	);
	}

	void c_anim_sync::update_local( const qangle_t& view_angles, const bool no_view_model, bool send_packet ) {
		const auto anim_state = valve___g_local_player->anim_state( );
		if ( !anim_state )
			return;

		valve::anim_state_t backup_state;

		const auto backup_cur_time = valve::g_global_vars->m_cur_time;
		const auto backup_frame_time = valve::g_global_vars->m_frame_time;

		valve::g_global_vars->m_cur_time = valve::to_time( valve::g_local_player->tick_base( ) );
		valve::g_global_vars->m_frame_time = valve::g_global_vars->m_interval_per_tick;

		valve::g_local_player->set_local_view_angles( view_angles );

		anim_state->m_prev_update_frame = valve::g_global_vars->m_frame_count - 1;

		//do_anim_event ( 0 );

		const auto backup_abs_velocity = valve::g_local_player->abs_velocity( );

		valve::g_local_player->abs_velocity( ) = valve::g_local_player->velocity( );

		const auto backup_eflags = valve::g_local_player->eflags( );

		const auto backup_layers = valve::g_local_player->anim_layers( );

		valve::g_local_player->eflags( ) &= ~0x1000u;

		const auto backup_client_side_anim = valve::g_local_player->client_side_anim( );

		valve::g_local_player->client_side_anim( ) = g_context->allow_anim_update( ) = true;

		if ( no_view_model )
			anim_state->update( view_angles.x, view_angles.y, view_angles.z );
		else
			valve::g_local_player->update_client_side_anim( );

		valve::g_local_player->client_side_anim( ) = backup_client_side_anim;

		g_context->allow_anim_update( ) = false;

		valve::g_local_player->eflags( ) = backup_eflags;

		valve::g_local_player->abs_velocity( ) = backup_abs_velocity;

		valve::g_global_vars->m_cur_time = backup_cur_time;
		valve::g_global_vars->m_frame_time = backup_frame_time;

		const auto walk_speed = std::clamp( anim_state->m_speed_as_portion_of_walk_speed, 0.f, 1.f );
		const auto run_speed = ( ( 0.8f - ( anim_state->m_walk_to_run_transition * 0.3f ) ) - 1.f ) * walk_speed;

		auto body_yaw_modifier = run_speed + 1.f;

		if ( anim_state->m_duck_amount > 0.f ) {
			const auto crouch_walk_speed = std::clamp( anim_state->m_speed_as_portion_of_crouch_speed, 0.f, 1.f );

			body_yaw_modifier += ( anim_state->m_duck_amount * crouch_walk_speed ) * ( 0.5f - body_yaw_modifier );
		}

		m_local_data.m_max_body_yaw = anim_state->m_max_body_yaw * body_yaw_modifier;
		m_local_data.m_min_body_yaw = anim_state->m_min_body_yaw * body_yaw_modifier;
	}

	void c_anim_sync::network_prediction_process( valve::user_cmd_t& user_cmd )
	{
		const auto anim_state = valve___g_local_player->anim_state( );
		if ( !anim_state )
			return;
		auto i = 1;
		auto choked_cmds = valve___g_client_state->m_choked_cmds;

		const auto total_cmds = choked_cmds + 1;
		for ( ; i <= total_cmds; ++i, --choked_cmds ) {
			const auto j = ( valve___g_client_state->m_last_cmd_out + i ) % 150;

			auto& cur_user_cmd = valve___g_input->m_cmds[ j ];
			auto& cur_local_data = g_eng_pred->local_data( ).at( j );
			if ( cur_local_data.m_net_vars.m_move_type != valve::e_move_type::ladder
				&& cur_local_data.m_pred_net_vars.m_move_type != valve::e_move_type::ladder ) {

				const auto old_view_angles = cur_user_cmd.m_view_angles;

				if ( cur_user_cmd.m_view_angles.x != old_view_angles.x
					|| cur_user_cmd.m_view_angles.y != old_view_angles.y
					|| cur_user_cmd.m_view_angles.z != old_view_angles.z )
					g_movement->rotate(
						cur_user_cmd, old_view_angles,
						cur_local_data.m_pred_net_vars.m_flags,
						cur_local_data.m_pred_net_vars.m_move_type
					);
			}
			g_movement->normalize( cur_user_cmd );

			valve::g_local_player->ground_entity( ) = cur_local_data.m_pred_net_vars.m_ground_entity;
			valve::g_local_player->origin( ) = cur_local_data.m_pred_net_vars.m_origin;
			valve::g_local_player->move_state( ) = cur_local_data.m_pred_net_vars.m_move_state;
			valve::g_local_player->strafing( ) = cur_local_data.m_pred_net_vars.m_strafing;
			valve::g_local_player->move_type( ) = cur_local_data.m_pred_net_vars.m_move_type;
			valve::g_local_player->scoped( ) = cur_local_data.m_pred_net_vars.m_scoped;
			valve::g_local_player->walking( ) = cur_local_data.m_pred_net_vars.m_walking;
			valve::g_local_player->lby( ) = cur_local_data.m_pred_net_vars.m_lby;
			valve::g_local_player->view_offset( ) = cur_local_data.m_pred_net_vars.m_view_offset;
			valve::g_local_player->aim_punch( ) = cur_local_data.m_pred_net_vars.m_aim_punch;
			valve::g_local_player->aim_punch_vel( ) = cur_local_data.m_pred_net_vars.m_aim_punch_vel;
			valve::g_local_player->view_punch( ) = cur_local_data.m_pred_net_vars.m_view_punch;
			valve::g_local_player->base_velocity( ) = cur_local_data.m_pred_net_vars.m_base_velocity;
			valve::g_local_player->fall_velocity( ) = cur_local_data.m_pred_net_vars.m_fall_velocity;
			valve::g_local_player->set_collision_bounds(
				cur_local_data.m_pred_net_vars.m_obb_min,
				cur_local_data.m_pred_net_vars.m_obb_max, 0.f
			);

			valve::g_local_player->surface_friction( ) = cur_local_data.m_pred_net_vars.m_surface_friction;

			valve::g_local_player->velocity( ) = cur_local_data.m_pred_net_vars.m_velocity;
			valve::g_local_player->third_person_recoil( ) = cur_local_data.m_pred_net_vars.m_third_person_recoil;
			valve::g_local_player->duck_amount( ) = cur_local_data.m_pred_net_vars.m_duck_amount;
			valve::g_local_player->flags( ) = cur_local_data.m_pred_net_vars.m_flags;

			valve::g_local_player->tick_base( ) = cur_local_data.m_pred_net_vars.m_tick_base;


			const auto last_user_cmd = cur_user_cmd.m_number == user_cmd.m_number;
			cur_local_data.m_user_cmd = cur_user_cmd;
			if ( last_user_cmd )
				continue;

			valve___g_input->m_verified_cmds[ j ] = { cur_user_cmd, cur_user_cmd.calc_checksum( ) };
		}
	}

	void c_anim_sync::update_local_real( valve::user_cmd_t& user_cmd, bool send_packet ) {
		const auto anim_state = valve___g_local_player->anim_state( );
		if ( !anim_state )
			return;

		if ( !g_anti_aim->enabled( &user_cmd ) ) {
			g_movement->normalize( user_cmd );

			const auto& prev_local_data = g_eng_pred->local_data( ).at( ( user_cmd.m_number - 1 ) % crypt_int( 150 ) );
			if ( prev_local_data.m_spawn_time == valve___g_local_player->spawn_time( ) )
				prev_local_data.restore_anim( false );

			update_local( user_cmd.m_view_angles, false, send_packet );

			g_eng_pred->local_data( ).at( user_cmd.m_number % crypt_int( 150 ) ).store_anim( );

			return;
		}

		const auto side = g_anti_aim->select_side( );

		auto yaw = user_cmd.m_view_angles.y;
		g_anti_aim->select_yaw( yaw, side, user_cmd );

		const auto& first_local_data = g_eng_pred->local_data( ).at( valve___g_client_state->m_last_cmd_out % crypt_int( 150 ) );
		if ( first_local_data.m_spawn_time == valve___g_local_player->spawn_time( ) )
			first_local_data.restore_anim( false );

		qangle_t shot_cmd_view_angles{};

		auto i = 1;
		auto choked_cmds = valve___g_client_state->m_choked_cmds;

		const auto total_cmds = choked_cmds + 1;
		if ( total_cmds < 1 ) {

			return;
		}

		for ( ; i <= total_cmds; ++i, --choked_cmds ) {
			const auto& cur_local_data = g_eng_pred->local_data( ).at( ( valve___g_client_state->m_last_cmd_out + i ) % crypt_int( 150 ) );
			if ( cur_local_data.m_spawn_time == valve___g_local_player->spawn_time( ) )
				break;
		}

		const auto shot_in_this_cycle =
			m_local_data.m_shot_cmd_number > valve___g_client_state->m_last_cmd_out
			&& m_local_data.m_shot_cmd_number <= ( valve___g_client_state->m_last_cmd_out + total_cmds );

		for ( ; i <= total_cmds; ++i, --choked_cmds ) {
			const auto j = ( valve___g_client_state->m_last_cmd_out + i ) % crypt_int ( 150 );

			auto& cur_user_cmd = valve___g_input->m_cmds[ j ];
			auto& cur_local_data = g_eng_pred->local_data( ).at( j );

			if ( cur_local_data.m_net_vars.m_move_type != valve::e_move_type::ladder
				&& cur_local_data.m_pred_net_vars.m_move_type != valve::e_move_type::ladder ) {
				const auto old_view_angles = cur_user_cmd.m_view_angles;

				g_anti_aim->process( cur_user_cmd, yaw, side, choked_cmds );

				if ( cur_user_cmd.m_view_angles.x != old_view_angles.x
					|| cur_user_cmd.m_view_angles.y != old_view_angles.y
					|| cur_user_cmd.m_view_angles.z != old_view_angles.z )
					g_movement->rotate(
						cur_user_cmd, old_view_angles,
						cur_local_data.m_pred_net_vars.m_flags,
						cur_local_data.m_pred_net_vars.m_move_type
					);
			}

			g_movement->normalize( cur_user_cmd );

			valve::g_local_player->ground_entity( ) = cur_local_data.m_pred_net_vars.m_ground_entity;
			valve::g_local_player->origin( ) = cur_local_data.m_pred_net_vars.m_origin;
			valve::g_local_player->move_state( ) = cur_local_data.m_pred_net_vars.m_move_state;
			valve::g_local_player->strafing( ) = cur_local_data.m_pred_net_vars.m_strafing;
			valve::g_local_player->move_type( ) = cur_local_data.m_pred_net_vars.m_move_type;
			valve::g_local_player->scoped( ) = cur_local_data.m_pred_net_vars.m_scoped;
			valve::g_local_player->walking( ) = cur_local_data.m_pred_net_vars.m_walking;
			valve::g_local_player->lby( ) = cur_local_data.m_pred_net_vars.m_lby;
			valve::g_local_player->view_offset( ) = cur_local_data.m_pred_net_vars.m_view_offset;
			valve::g_local_player->aim_punch( ) = cur_local_data.m_pred_net_vars.m_aim_punch;
			valve::g_local_player->aim_punch_vel( ) = cur_local_data.m_pred_net_vars.m_aim_punch_vel;
			valve::g_local_player->view_punch( ) = cur_local_data.m_pred_net_vars.m_view_punch;
			valve::g_local_player->base_velocity( ) = cur_local_data.m_pred_net_vars.m_base_velocity;
			valve::g_local_player->fall_velocity( ) = cur_local_data.m_pred_net_vars.m_fall_velocity;
			valve::g_local_player->set_collision_bounds(
				cur_local_data.m_pred_net_vars.m_obb_min,
				cur_local_data.m_pred_net_vars.m_obb_max, 0.f
			);

			valve::g_local_player->velocity( ) = cur_local_data.m_pred_net_vars.m_velocity;
			valve::g_local_player->surface_friction( ) = cur_local_data.m_pred_net_vars.m_surface_friction;
			valve::g_local_player->third_person_recoil( ) = cur_local_data.m_pred_net_vars.m_third_person_recoil;
			valve::g_local_player->duck_amount( ) = cur_local_data.m_pred_net_vars.m_duck_amount;
			valve::g_local_player->flags( ) = cur_local_data.m_pred_net_vars.m_flags;

			valve::g_local_player->tick_base( ) = cur_local_data.m_pred_net_vars.m_tick_base;


			auto cur_view_angles = cur_user_cmd.m_view_angles;

			if ( shot_in_this_cycle ) {
				if ( cur_user_cmd.m_number == m_local_data.m_shot_cmd_number )
					shot_cmd_view_angles = cur_view_angles;

				if ( cur_user_cmd.m_number > m_local_data.m_shot_cmd_number )
					cur_view_angles = shot_cmd_view_angles;
			}

			const auto last_user_cmd = cur_user_cmd.m_number == user_cmd.m_number;

			update_local( cur_view_angles, !last_user_cmd, send_packet );

			cur_local_data.m_user_cmd = cur_user_cmd;

			if ( shot_in_this_cycle ) {
				if ( cur_user_cmd.m_number == m_local_data.m_shot_cmd_number )
					cur_local_data.m_choke_angle = cur_view_angles;

				if ( cur_user_cmd.m_number > m_local_data.m_shot_cmd_number )
					cur_local_data.m_user_cmd.m_view_angles = shot_cmd_view_angles;
			}

			cur_local_data.store_anim( );

			if ( last_user_cmd )
				continue;

			valve___g_input->m_verified_cmds[ j ] = { cur_user_cmd, cur_user_cmd.calc_checksum( ) };
		}

		m_local_data.m_did_shot_in_choke_cycle = false;
		m_local_data.m_choke_shot_ang = qangle_t( 0, 0, 0 );
	}

	void c_anim_sync::do_animation_event ( const std::ptrdiff_t const type, const c_eng_pred::local_data_t& local_data ) {
		if ( local_data.m_pred_net_vars.m_flags & valve::e_ent_flags::frozen )
		{
			m_local_data.m_move_type [ type ] = valve::e_move_type::none;
			m_local_data.m_flags [ type ] = valve::e_ent_flags::on_ground;
		}

		valve::anim_layer_t* land_or_climb_layer = &valve::g_local_player->anim_layers ( ).at ( 5 );
		if ( !land_or_climb_layer )
			return;

		valve::anim_layer_t* jump_or_fall_layer = &valve::g_local_player->anim_layers ( ).at ( 4 );
		if ( !jump_or_fall_layer )
			return;

		if ( m_local_data.m_move_type [ type ] != valve::e_move_type::ladder && local_data.m_pred_net_vars.m_move_type == valve::e_move_type::ladder )
			valve::g_local_player->anim_state ( )->set_layer_seq ( land_or_climb_layer, crypt_int ( ACT_CLIMB_LADDER ) );
		else if ( m_local_data.m_move_type [ type ] == valve::e_move_type::ladder && local_data.m_pred_net_vars.m_move_type != valve::e_move_type::ladder )
			valve::g_local_player->anim_state ( )->set_layer_seq ( jump_or_fall_layer, crypt_int ( ACT_FALL ) );
		else
		{
			if ( local_data.m_pred_net_vars.m_flags & valve::e_ent_flags::on_ground )
			{
				if ( !( m_local_data.m_flags [ type ] & valve::e_ent_flags::on_ground ) )
					valve::g_local_player->anim_state ( )->set_layer_seq ( land_or_climb_layer, valve::g_local_player->anim_state ( )->m_time_since_in_air > crypt_float ( 1.0f ) && type == 0 ? crypt_int ( ACT_LAND_HEAVY ) : crypt_int ( ACT_LAND_LIGHT ) );
			}
			else if ( m_local_data.m_flags [ type ] & valve::e_ent_flags::on_ground )
			{
				if ( local_data.m_pred_net_vars.m_velocity.z > 0.0f )
					valve::g_local_player->anim_state ( )->set_layer_seq ( jump_or_fall_layer, crypt_int ( ACT_JUMP ) );
				else
					valve::g_local_player->anim_state ( )->set_layer_seq ( jump_or_fall_layer, crypt_int ( ACT_FALL ) );
			}
		}

		m_local_data.m_move_type [ type ] = local_data.m_pred_net_vars.m_move_type;
		m_local_data.m_flags [ type ] = local_data.m_pred_net_vars.m_flags;
	}
}