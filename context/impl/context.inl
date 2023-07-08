#pragma once

#include "../context.hpp"

namespace nem {
	__forceinline c_context::addresses_t& c_context::addresses( ) {
		return m_addresses;
	}

	__forceinline c_context::cvars_t& c_context::cvars( ) {
		return m_cvars;
	}

	__forceinline c_context::net_info_t& c_context::net_info( ) {
		return m_net_info;
	}

	__forceinline c_context::s_auto_peek_data& c_context::get_auto_peek_info()
	{
		return m_apeek_data;
	}

	__forceinline bool& c_context::is_bone_available ( ) {
		return m_bone_available;
	}

	__forceinline vec3_t& c_context::shoot_pos( ) {
		return m_shoot_pos;
	}

	__forceinline int& c_context::last_sent_cmd_number ( ) {
		return m_last_sent_cmd_number;
	}

	__forceinline bool& c_context::defensive_allowed ( ) {
		return m_defensive_allowed;
	}

	__forceinline bool& c_context::left_create_move ( ) {
		return m_left_create_move;
	}

	__forceinline bool& c_context::send_packet ( ) {
		return m_send_packet;
	}

	__forceinline int& c_context::last_cmd_number( ) {
		return m_last_cmd_number;
	}

	__forceinline bool& c_context::allow_anim_update( ) {
		return m_allow_anim_update;
	}

	__forceinline c_context::s_kill_feed_data& c_context::get_kill_feed_data()
	{
		m_kill_feed_data;
	}

	__forceinline float& c_context::c4_explode_time ( ) {
		return m_c4_explode_time;
	}

	__forceinline float& c_context::c4_dmg ( ) {
		return m_c4_dmg;
	}

	__forceinline float& c_context::c4_radius ( ) {
		return m_c4_radius;
	}

	__forceinline float& c_context::c4_radius_scaled ( ) {
		return m_c4_radius_scaled;
	}

	__forceinline vec3_t& c_context::c4_explode_origin ( ) {
		return m_c4_explode_origin;
	}

	__forceinline std::deque < c_context::out_cmds_data_t >& c_context::get_out_cmds ( ) {
		return m_out_cmds_data;
	}

	__forceinline std::deque < c_context::networked_data_t>& c_context::net_correction ( ) {
		return m_net_correction;
	}

	__forceinline bool& c_context::allow_setup_bones( ) {
		return m_allow_setup_bones;
	}

	__forceinline int& c_context::should_buy ( ) {
		return m_should_buy;
	}

	__forceinline bool& c_context::force_bone_mask( ) {
		return m_force_bone_mask;
	}

	__forceinline std::vector< int >& c_context::sented_cmds( ) {
		return m_sented_cmds;
	}

	__forceinline std::mutex& c_context::get_mutex ( ) {
		return m_mtx;
	}

	__forceinline e_context_flags& c_context::flags( ) {
		return m_flags;
	}


	__forceinline valve::c_weapon*& c_context::weapon( ) {
		return m_weapon;
	}

	__forceinline valve::weapon_data_t*& c_context::wpn_data( ) {
		return m_wpn_data;
	}

	__forceinline bool& c_context::freeze_time( ) {
		return m_freeze_time;
	}

	__forceinline bool& c_context::is_c4_planted ( ) {
		return m_c4_planted;
	}

	__forceinline bool& c_context::should_reset_k_feed()
	{
		return m_should_reset;
	}

	__forceinline bool& c_context::broke_lc( ) {
		return m_broke_lc;
	}

	__forceinline vec3_t& c_context::last_sent_origin( ) {
		return m_last_sent_origin;
	}

	__forceinline bool& c_context::is_out_of_epsilon ( ) {
		return m_is_out_of_epsilon;
	}

	__forceinline vec3_t& c_context::aim_bot_shoot_pos ( ) {
		return m_aim_bot_shoot_pos;
	}
}