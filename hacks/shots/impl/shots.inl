#pragma once

#include "../shots.hpp"

namespace nem::hacks {
	__forceinline std::deque< shot_t >& c_shots::elements( ) { return m_elements; }

	__forceinline shot_t* c_shots::last_unprocessed( ) {
		if ( m_elements.empty( ) )
			return nullptr;

		const auto shot = std::find_if(
			m_elements.begin( ), m_elements.end( ),
			[ ] ( const shot_t& shot ) {
				return !shot.m_processed
					&& shot.m_server_info.m_fire_tick
					&& shot.m_server_info.m_fire_tick == valve___g_client_state->m_server_tick;
			}
		);

		return shot == m_elements.end( ) ? nullptr : &*shot;
	}

	__forceinline void c_shots::add(
		const vec3_t& src, const aim_last_target_t* const target, const int next_shift_amount, const std::ptrdiff_t cmd_num, const float sent_time, const float latency
	) {
		m_elements.emplace_back( src, target, next_shift_amount, cmd_num, sent_time, latency );

		if ( m_elements.size( ) < 128 )
			return;

		m_elements.pop_front( );
	}
}