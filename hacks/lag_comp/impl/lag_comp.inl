#pragma once

#include "../lag_comp.hpp"

namespace nem::hacks {
	__forceinline lag_backup_t::lag_backup_t( valve::c_player* const player ) {
		if ( const auto anim_state = player->anim_state( ) )
			m_foot_yaw = anim_state->m_foot_yaw;

		m_origin = player->origin( );
		m_abs_origin = player->abs_origin( );

		m_obb_min = player->obb_min( );
		m_obb_max = player->obb_max( );

		const auto& bone_accessor = player->bone_accessor( );

		m_readable_bones = bone_accessor.m_readable_bones;
		m_writable_bones = bone_accessor.m_writable_bones;

		const auto& bone_cache = player->bone_cache( );

		std::memcpy(
			m_bones.data( ),
			bone_cache.m_mem.m_ptr,
			bone_cache.m_size * sizeof( mat3x4_t )
		);

		m_bones_count = bone_cache.m_size;

		m_mdl_bone_counter = player->mdl_bone_counter( );
	}

	__forceinline void lag_backup_t::restore( valve::c_player* const player ) const {
		player->origin( ) = m_origin;

		player->set_abs_origin( m_abs_origin );

		player->set_collision_bounds( m_obb_min, m_obb_max, 0.f );

		player->set_abs_angles( { 0.f, m_foot_yaw, 0.f } );

		auto& bone_accessor = player->bone_accessor( );

		bone_accessor.m_readable_bones = m_readable_bones;
		bone_accessor.m_writable_bones = m_writable_bones;

		std::memcpy(
			player->bone_cache( ).m_mem.m_ptr,
			m_bones.data( ), m_bones_count * sizeof( mat3x4_t )
		);

		player->mdl_bone_counter( ) = m_mdl_bone_counter;
	}

	__forceinline void lag_record_t::restore( valve::c_player* const player, const int anim_index, const bool only_anim ) const {
		if ( !only_anim ) {
			player->origin( ) = m_origin;

			player->set_abs_origin( m_origin );

			player->set_collision_bounds( m_obb_min, m_obb_max, 0.f );
		}

		const auto& anim_side = m_anim_sides.at( anim_index );

		player->set_abs_angles( { 0.f, anim_side.m_foot_yaw, 0.f } );

		std::memcpy(
			player->bone_cache( ).m_mem.m_ptr,
			anim_side.m_bones.data( ), anim_side.m_bones_count * sizeof( mat3x4_t )
		);

		player->mdl_bone_counter( ) = **reinterpret_cast< unsigned long** >(
			g_context->addresses( ).m_invalidate_bone_cache.get( ) + 0xau
			);
	}

	__forceinline void lag_record_t::correct ( cc_def ( lag_anim_data_t* ) previous, const lag_anim_data_t* const pre_previous, player_entry_t& entry ) {

		if ( previous.get ( ) ) {
			if ( ( m_origin - previous.get( )->m_origin ).length_sqr ( ) > crypt_float ( 4096.f ) )
				m_broke_lc = true;
		}

		if ( std::abs ( m_third_person_recoil + m_eye_angles.x - crypt_float ( 180.f ) ) < crypt_float ( 0.1f ) )
			m_eye_angles.x = crypt_float ( 89.f );

		const auto& cur_alive_loop_layer = m_anim_layers.at ( 11u );

		if ( !previous.get( )
			|| previous.get( )->m_dormant ) {
			if ( !previous.get( ) ) {
				if ( ( m_flags & valve::e_ent_flags::on_ground ) ) {
					auto max_speed = m_weapon ? std::max( 0.1f, m_weapon->max_speed( ) ) : 260.f;

					if ( m_anim_layers.at( 6u ).m_weight > 0.f && m_anim_layers.at( 6u ).m_playback_rate > 0.f
						&& m_velocity.length_2d( ) > 0.f ) {
						if ( ( m_flags & valve::e_ent_flags::ducking ) )
							max_speed *= 0.34f;
						else if ( entry.m_player->walking( ) )
							max_speed *= 0.52f;

						const auto move_multiplier = m_anim_layers.at( 6u ).m_weight * max_speed;
						const auto speed_multiplier = move_multiplier / m_velocity.length_2d ( );

						m_velocity.x *= speed_multiplier;
						m_velocity.y *= speed_multiplier;
					}
				}
			}

			return;
		}

		if ( m_weapon == previous.get( )->m_weapon
			&& m_anim_layers.at( 11u ).m_playback_rate > 0.f && previous.get( )->m_anim_layers.at( 11u ).m_playback_rate > 0.f ) {

			const auto cur_11th_cycle = cur_alive_loop_layer.m_cycle;
			auto prev_11th_cycle = previous.get( )->m_anim_layers.at( 11u ).m_cycle;

			const auto cycles_delta = cur_11th_cycle - prev_11th_cycle;

			if ( cycles_delta != crypt_float( 0.f ) ) {
				const auto sim_ticks_delta = valve::to_ticks( m_sim_time - m_old_sim_time );

				if ( sim_ticks_delta != crypt_int( 1 ) ) {
					auto prev_11th_rate = previous.get( )->m_anim_layers.at( 11u ).m_playback_rate;
					std::ptrdiff_t resimulated_sim_ticks{};

					if ( cycles_delta >= crypt_float( 0.f ) ) {
						resimulated_sim_ticks = valve::to_ticks( cycles_delta / prev_11th_rate );
					}
					else {
						std::ptrdiff_t ticks_iterated{};
						float cur_simulated_cycle{ 0.f };
						while ( true ) {
							++resimulated_sim_ticks;

							if ( ( prev_11th_rate * valve___g_global_vars->m_interval_per_tick ) + prev_11th_cycle >= 1.f )
								prev_11th_rate = m_anim_layers.at( 11u ).m_playback_rate;

							cur_simulated_cycle = ( prev_11th_rate * valve___g_global_vars->m_interval_per_tick ) + prev_11th_cycle;
							prev_11th_cycle = cur_simulated_cycle;
							if ( cur_simulated_cycle > crypt_float( 0.f ) )
								break;

							if ( ++ticks_iterated >= crypt_int( 16 ) ) {
								goto leave_cycle;
							}
						}

						const auto first_val = prev_11th_cycle - cur_simulated_cycle;

						auto recalc_everything = cur_11th_cycle - first_val;
						recalc_everything /= m_anim_layers.at( 11u ).m_playback_rate;
						recalc_everything /= valve___g_global_vars->m_interval_per_tick;
						recalc_everything += 0.5f;

						resimulated_sim_ticks += recalc_everything;
					}

				leave_cycle:
					if ( resimulated_sim_ticks < sim_ticks_delta ) {
						if ( valve___g_client_state->m_server_tick - valve::to_ticks( m_sim_time ) == resimulated_sim_ticks ) {
							entry.m_player->sim_time( ) = entry.m_cur_sim = m_sim_time = ( resimulated_sim_ticks * valve___g_global_vars->m_interval_per_tick ) + m_old_sim_time;
							entry.m_fix_sim = false;
						}
					}
				}
			}
		}

		if ( previous.get( ) && !previous.get( )->m_dormant && !m_entry_after_dormant ) {
			const auto& prev_alive_loop_layer = previous.get( )->m_anim_layers.at ( 11u );

			auto sim_ticks = valve::to_ticks ( m_sim_time - previous.get( )->m_sim_time );

			if ( ( sim_ticks - 1 ) > crypt_int ( 31 ) || previous.get( )->m_sim_time == crypt_float ( 0.f ) ) {
				sim_ticks = 1;
			}

			auto cur_cycle = m_anim_layers.at ( 11 ).m_cycle;
			auto prev_rate = previous.get( )->m_anim_layers.at ( 11 ).m_playback_rate;

			if ( prev_rate > crypt_float ( 0.f ) &&
				m_anim_layers.at ( 11 ).m_playback_rate > crypt_float ( 0.f ) &&
				m_weapon == previous.get( )->m_weapon  ) {
				auto prev_cycle = previous.get( )->m_anim_layers.at ( 11 ).m_cycle;
				sim_ticks = 0;

				if ( prev_cycle > cur_cycle )
					cur_cycle += crypt_float ( 1.f );

				while ( cur_cycle > prev_cycle ) {
					const auto last_cmds = sim_ticks;

					const auto next_rate = valve___g_global_vars->m_interval_per_tick * prev_rate;
					prev_cycle += valve___g_global_vars->m_interval_per_tick * prev_rate;

					if ( prev_cycle >= crypt_float ( 1.f ) )
						prev_rate = m_anim_layers.at ( 11 ).m_playback_rate;

					++sim_ticks;

					if ( prev_cycle > cur_cycle && ( prev_cycle - cur_cycle ) > ( next_rate * crypt_float ( 0.5f ) ) )
						sim_ticks = last_cmds;
				}
			}

			m_total_cmds = std::clamp ( sim_ticks, 0, crypt_int ( 20 ) );

			m_time_diff = m_total_cmds * valve___g_global_vars->m_interval_per_tick;

			m_anim_time = m_sim_time - valve::to_time ( m_total_cmds );

			const auto total_cmds_time = valve::to_time( m_total_cmds );
			if ( total_cmds_time > crypt_float( 0.f )
				&& total_cmds_time < crypt_float( 1.f ) )
				m_velocity = ( m_origin - previous.get ( )->m_origin )  * ( 1.f / total_cmds_time );
		}

		if ( m_weapon
			&& m_last_shot_time > ( m_sim_time - valve::to_time ( m_total_cmds ) )
			&& m_sim_time >= m_last_shot_time )
			if ( const auto wpn_data = m_weapon->wpn_data ( ); wpn_data && ( static_cast< std::size_t >( wpn_data->m_unk_type - crypt_int ( 2 ) ) <= crypt_int ( 5 ) || m_weapon->item_index ( ) == valve::e_item_index::taser ) )
				m_shot = true;

		if ( m_total_cmds >= 2 ) {
			if ( ( m_flags & valve::e_ent_flags::on_ground )
				&& ( !previous.get( ) || ( previous.get( )->m_flags & valve::e_ent_flags::on_ground ) ) ) {
				if ( m_anim_layers.at( 6u ).m_playback_rate == crypt_float( 0.f ) )
					m_velocity = {};
				else {
					if ( m_weapon == previous.get( )->m_weapon ) {
						if ( cur_alive_loop_layer.m_weight > 0.f && cur_alive_loop_layer.m_weight < 1.f ) {
							const auto speed_2d = m_velocity.length_2d ( );
							const auto max_speed = m_weapon ? std::max( 0.1f, m_weapon->max_speed( ) ) : 260.f;
							if ( speed_2d ) {
								const auto reversed_val = ( crypt_float( 1.f ) - cur_alive_loop_layer.m_weight ) * crypt_float( 0.35f );

								if ( reversed_val > crypt_float( 0.f ) && reversed_val < crypt_float( 1.f ) ) {
									const auto speed_as_portion_of_run_top_speed = ( ( reversed_val + crypt_float( 0.55f ) ) * max_speed ) / speed_2d;
									if ( speed_as_portion_of_run_top_speed ) {
										m_in_velocity_sync_process += 3;

										m_velocity.x *= speed_as_portion_of_run_top_speed;
										m_velocity.y *= speed_as_portion_of_run_top_speed;
									}
								}
							}
						}
					}
				}
			}

			/* pasta from onepasta but its pasted from skeet :| */
			if ( ( m_flags & valve::e_ent_flags::on_ground )
				&& ( previous.get( )->m_flags & valve::e_ent_flags::on_ground ) ) {
				if ( m_anim_layers.at( 6u ).m_playback_rate == 0.f )
					m_velocity = {};
				else {
					if ( m_anim_layers.at( 6u ).m_weight >= 0.1f ) {
						if ( ( cur_alive_loop_layer.m_weight <= crypt_float( 0.f ) || cur_alive_loop_layer.m_weight >= crypt_float( 1.f ) )
							&& m_velocity.length_2d ( ) > 0.f ) {
							const bool valid_6th_layer = ( m_anim_layers.at( 6u ).m_weight < crypt_float( 1.f ) )
								&& ( m_anim_layers.at( 6u ).m_weight >= previous.get( )->m_anim_layers.at( 6u ).m_weight );
							const auto max_spd = m_weapon ? std::max( 0.1f, m_weapon->max_speed( ) ) : 260.f;

							if ( valid_6th_layer ) {
								const auto max_duck_speed = max_spd * crypt_float( 0.34f );
								const auto speed_multiplier = std::max( 0.f, ( max_spd * crypt_float( 0.52f ) ) - ( max_spd * crypt_float( 0.34f ) ) );
								const auto duck_modifier = crypt_float( 1.f ) - m_duck_amount;

								m_in_velocity_sync_process += 5;

								const auto speed_via_6th_layer = ( ( ( duck_modifier * speed_multiplier ) + max_duck_speed ) * m_anim_layers.at( 6u ).m_weight ) / m_velocity.length_2d ( );

								m_velocity.x *= speed_via_6th_layer;
								m_velocity.y *= speed_via_6th_layer;
							}
						}
					}
					else {
						if ( m_anim_layers.at( 6u ).m_weight ) {
							auto weight = m_anim_layers.at( 6 ).m_weight;
							const auto length_2d = m_velocity.length_2d ( );

							if ( m_flags & valve::e_ent_flags::ducking )
								weight *= crypt_float( 0.34f );
							else {
								if ( entry.m_player->walking( ) ) {
									weight *= crypt_float( 0.52f );
								}
							}
							if ( length_2d ) {
								m_in_velocity_sync_process += 7;
								m_velocity.x = ( m_velocity.x / length_2d ) * weight;
								m_velocity.y = ( m_velocity.y / length_2d ) * weight;
							}
						}
					}
				}
			}
		}
	}

	__forceinline bool lag_record_t::valid( ) const {
		const auto& net_info = g_context->net_info ( );
		if ( m_failed_sim )
			return false;

        if ( g_lag_comp->calc_time_delta( m_sim_time ) >= 0.2f )
            return false;

		return true;
	}

	__forceinline lag_anim_data_t::lag_anim_data_t( const lag_record_t* const lag_record ) {
		m_weapon = lag_record->m_weapon;

		m_eye_angles = lag_record->m_eye_angles;
		m_origin = lag_record->m_origin;
		m_velocity = lag_record->m_velocity;
		m_view_offset = lag_record->m_view_offset;

		m_flags = lag_record->m_flags;
		m_lby = lag_record->m_lby;
		m_third_person_recoil = lag_record->m_third_person_recoil;
		m_duck_amount = lag_record->m_duck_amount;

		m_anim_layers = lag_record->m_anim_layers;

		m_lby_delta = lag_record->m_lby_delta;

		m_sim_time = lag_record->m_sim_time;

		m_dormant = lag_record->m_dormant;

		for ( std::size_t i{}; i < m_anim_sides.size( ); ++i ) {
			auto& to = m_anim_sides.at( i );
			const auto& from = lag_record->m_anim_sides.at( i );

			to.m_foot_yaw = from.m_foot_yaw;
			to.m_prev_foot_yaw = from.m_prev_foot_yaw;
			to.m_move_yaw_ideal = from.m_move_yaw_ideal;
			to.m_move_yaw_cur_to_ideal = from.m_move_yaw_cur_to_ideal;
			to.m_move_yaw = from.m_move_yaw;
			to.m_move_weight_smoothed = from.m_move_weight_smoothed;
			to.m_air_smooth_value = from.m_air_smooth_value;
			to.m_time_to_realign_body = from.m_time_to_realign_body;
		}
	}

	__forceinline lag_anim_data_t::lag_anim_data_t(const lag_anim_data_t* const lag_anim_data)
	{
		m_weapon = lag_anim_data->m_weapon;

		m_eye_angles = lag_anim_data->m_eye_angles;
		m_origin = lag_anim_data->m_origin;
		m_velocity = lag_anim_data->m_velocity;

		m_flags = lag_anim_data->m_flags;
		m_lby = lag_anim_data->m_lby;
		m_duck_amount = lag_anim_data->m_duck_amount;
		m_lby_delta = lag_anim_data->m_lby_delta;

		m_anim_layers = lag_anim_data->m_anim_layers;

		m_dormant = lag_anim_data->m_dormant;

		for (std::size_t i{}; i < m_anim_sides.size(); ++i) {
			auto& to = m_anim_sides.at(i);
			const auto& from = lag_anim_data->m_anim_sides.at(i);

			to.m_foot_yaw = from.m_foot_yaw;
			to.m_prev_foot_yaw = from.m_prev_foot_yaw;
			to.m_move_yaw_ideal = from.m_move_yaw_ideal;
			to.m_move_yaw_cur_to_ideal = from.m_move_yaw_cur_to_ideal;
			to.m_move_yaw = from.m_move_yaw;
			to.m_move_weight_smoothed = from.m_move_weight_smoothed;
			to.m_air_smooth_value = from.m_air_smooth_value;
			to.m_time_to_realign_body = from.m_time_to_realign_body;
		}
	}

	__forceinline void player_entry_t::reset ( ) {
		m_player = nullptr;

		m_alive_loop_cycle = -1.f;
		m_alive_loop_rate = -1.f;
		m_render_origin = {};
		m_cur_sim = 0.f;
		m_old_sim = 0.f;
		m_last_sim = 0.f;
		m_prev_anim_side.fill ( 0 );
		m_anim_misses = 0;
		m_at_target_misses = 0;
		m_auto_wall_misses = 0;
		m_stand_misses = 0;
		m_prev_anim_data = std::nullopt;
		m_pre_prev_anim_data = std::nullopt; 
		m_try_anim_resolver = true;
		m_records_count = 0;

		m_lag_records.clear( );
	}

	__forceinline float c_lag_comp::calc_time_delta( const float sim_time ) const {
		const auto& net_info = g_context->net_info( );

		const auto correct = std::clamp(
			net_info.m_lerp + net_info.m_latency.m_in + net_info.m_latency.m_out,
			0.f, g_context->cvars( ).m_sv_maxunlag->get_float( )
		);

		auto tick_base = valve___g_local_player->tick_base( );
		if ( g_exploits->next_shift_amount( ) > 0 )
			tick_base -= g_exploits->next_shift_amount( );


		return std::abs( correct - ( valve::to_time( tick_base ) - sim_time ) );
	}

	__forceinline player_entry_t& c_lag_comp::entry( const std::size_t i ) { return m_entries.at( i ); }
}