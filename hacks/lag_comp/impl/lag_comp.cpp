#include "../../../nem.hpp"

namespace nem::hacks {

	void c_lag_comp::on_net_update( ) {
		if ( !valve___g_local_player 
			|| !valve___g_engine->in_game( ) )
			return;

        const auto tick_rate = valve::to_ticks( 1.f );

		for ( auto i = 1; i <= valve___g_global_vars->m_max_clients; ++i ) {
			auto& entry = m_entries.at( i - 1 );

			const auto player = static_cast< valve::c_player* >(
				valve___g_entity_list->find_entity( i )
			);

			if ( !player )
			{
				entry.reset ( );

				continue;
			}

			if ( !valve___g_local_player )
			{
				entry.reset ( );

				continue;
			}

			if ( player == valve___g_local_player ) {
				entry.reset( );

				continue;
			}

			if ( player->team ( ) == valve___g_local_player->team ( ) )	{
				entry.reset ( );

				continue;
			}

			if ( entry.m_player != player )
				entry.reset( );

			entry.m_player = player;

			if ( !player
				|| !player->alive( ) ) {
				entry.reset( );

                if ( player )
                    entry.m_player = player;

				continue;
			}

			if ( player->dormant( ) ) {
				entry.m_prev_anim_data = std::nullopt;
				entry.m_pre_prev_anim_data = std::nullopt;
				entry.m_try_anim_resolver = true;

				if ( entry.m_lag_records.empty( ) ) {
					entry.m_lag_records.emplace_back(
						std::make_shared< lag_record_t >( player ) 
					);

					continue;
				}

				if ( !entry.m_lag_records.back( )->m_dormant ) {
					entry.m_lag_records.clear( );

					entry.m_lag_records.emplace_back(
						std::make_shared< lag_record_t >( player )
					);

					continue;
				}

				continue;
			}

			const auto anim_state = player->anim_state( );
			if ( !anim_state ) {
				entry.reset( );

				continue;
			}

			bool invalid_processing{};

			if ( player->sim_time( ) == crypt_float ( 0.f ) )
				continue;

			if ( player->sim_time( ) == player->old_sim_time( ) ) {
				continue;
			}

			/* fake update -> restore previous simulation data */
			if ( player->anim_layers( ).at( 11u ).m_cycle == entry.m_alive_loop_cycle
				&& player->anim_layers( ).at( 11u ).m_playback_rate == entry.m_alive_loop_rate ) {
				player->sim_time( ) = entry.m_cur_sim;
				player->old_sim_time( ) = entry.m_old_sim;
				continue;
			}

			if ( player->sim_time( ) < entry.m_last_sim
				&& ( entry.m_last_sim != 0.f ) ) {
				entry.m_broke_sim = player->sim_time( );
			}

			if ( entry.m_broke_sim >= player->sim_time( ) )
				invalid_processing = true;

			++entry.m_records_count;

			entry.m_old_sim = entry.m_cur_sim;
			entry.m_cur_sim = player->sim_time( );

			if ( !invalid_processing )
				entry.m_last_sim = player->sim_time( );

			entry.m_alive_loop_cycle = player->anim_layers ( ).at ( 11 ).m_cycle;
			entry.m_alive_loop_rate = player->anim_layers ( ).at ( 11 ).m_playback_rate;

            while ( entry.m_lag_records.size( ) > tick_rate )
                entry.m_lag_records.pop_front( );

			entry.m_receive_time = valve___g_global_vars->m_real_time;

			if ( entry.m_spawn_time != player->spawn_time( ) ) {
				anim_state->reset( );

				entry.m_try_anim_resolver = true;
				entry.m_prev_anim_side.fill ( 0 );
				entry.m_anim_misses = 0;
				entry.m_at_target_misses = crypt_int ( 0 );
				entry.m_auto_wall_misses = crypt_int ( 0 );
				entry.m_stand_misses = crypt_int ( 0 );
				entry.m_prev_anim_data = std::nullopt;
				entry.m_pre_prev_anim_data = std::nullopt;
				entry.m_last_sim = 0.f;

				entry.m_lag_records.clear( );
			}

			entry.m_spawn_time = player->spawn_time( );

			lag_anim_data_t* previous{};
			if ( entry.m_prev_anim_data.has_value( ) )
				previous = &entry.m_prev_anim_data.value( );

			const auto current = entry.m_lag_records.emplace_back(
				std::make_shared< lag_record_t >( player )
			).get( );

			entry.m_mins = player->obb_min( );
			entry.m_maxs = player->obb_max( );
			entry.m_render_origin = current->m_origin;
			current->m_entry_after_dormant = entry.m_records_count <= 1;
			current->m_invalid = invalid_processing;

			lag_anim_data_t* pre_previous{};	

			if (entry.m_pre_prev_anim_data.has_value())
				pre_previous = &entry.m_pre_prev_anim_data.value();

			current->correct( previous, pre_previous, entry );

			g_anim_sync->on_net_update( entry, current, previous );
			entry.m_prev_anim_data.emplace( current );

			if ( previous )
			entry.m_pre_prev_anim_data.emplace( previous );
		}
	}
}