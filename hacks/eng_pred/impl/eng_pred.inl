#pragma once

#include "../eng_pred.hpp"

namespace nem::hacks {
	__forceinline void c_eng_pred::net_vars_t::restore( const int cmd_number ) const {
		if ( cmd_number != m_cmd_number )
			return;

		if ( !valve::g_local_player->alive ( ) )
			return;

		int counter {};

		if ( std::abs ( valve::g_local_player->view_offset ( ).z - m_view_offset_z ) <= crypt_float ( 0.03125f ) )
			valve::g_local_player->view_offset ( ).z = m_view_offset_z;

		const auto aim_punch_delta = valve::g_local_player->aim_punch( ) - m_aim_punch;
		if ( std::abs( aim_punch_delta.x ) <= crypt_float ( 0.03125f )
			&& std::abs( aim_punch_delta.y ) <= crypt_float ( 0.03125f )
			&& std::abs( aim_punch_delta.z ) <= crypt_float ( 0.03125f ) )
			valve::g_local_player->aim_punch( ) = m_aim_punch;
		else
			counter++;

		const auto aim_punch_vel_delta = valve::g_local_player->aim_punch_vel( ) - m_aim_punch_vel;
		if ( std::abs( aim_punch_vel_delta.x ) <= crypt_float ( 0.03125f )
			&& std::abs( aim_punch_vel_delta.y ) <= crypt_float ( 0.03125f )
			&& std::abs( aim_punch_vel_delta.z ) <= crypt_float ( 0.03125f ) )
			valve::g_local_player->aim_punch_vel( ) = m_aim_punch_vel;
		else
			counter++;

		const auto view_punch_delta = valve::g_local_player->view_punch ( ) - m_view_punch;
		if ( std::abs ( view_punch_delta.x ) <= crypt_float ( 0.03125f )
			&& std::abs ( view_punch_delta.y ) <= crypt_float ( 0.03125f )
			&& std::abs ( view_punch_delta.z ) <= crypt_float ( 0.03125f ) )
			valve::g_local_player->view_punch ( ) = m_view_punch;
		else
			counter++;

		const auto velocity_modifier_delta = valve::g_local_player->velocity_modifier ( ) - m_velocity_modifier;
		if ( std::abs ( velocity_modifier_delta ) <= 0.00625f )
			valve::g_local_player->velocity_modifier ( ) = m_velocity_modifier;
		else
			counter++;

		const auto fall_velocity_delta = valve::g_local_player->fall_velocity ( ) - m_fall_velocity;
		if ( std::abs ( fall_velocity_delta ) <= 0.5f )
			valve::g_local_player->fall_velocity ( ) = m_fall_velocity;
		else
			counter++;

		const auto velocity_delta = valve::g_local_player->velocity ( ) - m_velocity;
		if ( std::abs ( velocity_delta.x ) <= 0.5f
			&& std::abs ( velocity_delta.y ) <= 0.5f
			&& std::abs ( velocity_delta.z ) <= 0.5f )
			valve::g_local_player->velocity ( ) = m_velocity;
		else
			counter++;

		const auto origin_delta = valve::g_local_player->origin ( ) - m_origin;
		if ( std::abs ( origin_delta.x ) <= 0.1f
			&& std::abs ( origin_delta.y ) <= 0.1f
			&& std::abs ( origin_delta.z ) <= 0.1f )
			valve::g_local_player->origin ( ) = m_origin;	
		else
			counter++;

		const auto third_person_recoil_delta = valve::g_local_player->third_person_recoil ( ) - m_third_person_recoil;

		if ( std::abs ( third_person_recoil_delta ) <= 0.03125f ) {
			valve::g_local_player->third_person_recoil ( ) = m_third_person_recoil;	
		}

		hacks::g_eng_pred->is_out_of_epsilon ( ) = counter > 0 ? true : false;
	}

	__forceinline void c_eng_pred::net_vars_t::store( const int cmd_number ) {
		valve::g_local_player->physics_collision_state() = 0;

		if ( !valve::g_local_player->alive ( ) )
			return;

		m_cmd_number = cmd_number;

		m_view_offset_z = std::clamp( valve::g_local_player->view_offset( ).z, 0.f, 64.f );
		m_aim_punch = valve::g_local_player->aim_punch( );
		m_aim_punch_vel = valve::g_local_player->aim_punch_vel( );
		m_view_punch = valve::g_local_player->view_punch ( );
		m_velocity = valve::g_local_player->velocity ( );
		m_origin = valve::g_local_player->origin ( );
		m_velocity_modifier = valve::g_local_player->velocity_modifier ( );
		m_fall_velocity = valve::g_local_player->fall_velocity ( );
		m_third_person_recoil = valve::g_local_player->third_person_recoil ( );
	}

	__forceinline void c_eng_pred::local_data_t::init( const valve::user_cmd_t& user_cmd ) {
		std::memset( this, 0, sizeof( local_data_t ) );

		g_eng_pred->velocity_modifier( ) = valve::g_local_player->velocity_modifier( );

		if ( const auto weapon = valve::g_local_player->weapon( ) ) {
			g_eng_pred->recoil_index( ) = weapon->recoil_index( );
			g_eng_pred->accuracy_penalty( ) = weapon->accuracy_penalty( );
		}

		m_spawn_time = valve::g_local_player->spawn_time( );
		m_tick_base = m_adjusted_tick_base = valve::g_local_player->tick_base( );

		m_user_cmd = user_cmd;
		
		store_anim( );

		m_net_vars.m_eye_angles = valve::g_local_player->eye_angles( );
		m_net_vars.m_aim_punch = valve::g_local_player->aim_punch( );
		m_net_vars.m_view_punch = valve::g_local_player->view_punch( );
		m_net_vars.m_ground_entity = valve::g_local_player->ground_entity ( );

		m_net_vars.m_origin = valve::g_local_player->origin( );
		m_net_vars.m_abs_origin = valve::g_local_player->abs_origin( );
		m_net_vars.m_base_velocity = valve::g_local_player->base_velocity ( );
		m_net_vars.m_obb_min = valve::g_local_player->obb_min( );
		m_net_vars.m_obb_max = valve::g_local_player->obb_max( );
		m_net_vars.m_velocity = valve::g_local_player->velocity( );
		m_net_vars.m_view_offset = valve::g_local_player->view_offset( );
		m_net_vars.m_aim_punch_vel = valve::g_local_player->aim_punch_vel( );
		m_net_vars.m_abs_velocity = valve::g_local_player->abs_velocity( );
		m_net_vars.m_velocity_modifier = valve::g_local_player->velocity_modifier ( );

		m_net_vars.m_strafing = valve::g_local_player->strafing( );
		m_net_vars.m_scoped = valve::g_local_player->scoped( );
		m_net_vars.m_walking = valve::g_local_player->walking( );
		m_net_vars.m_surface_friction = valve::g_local_player->surface_friction ( );
		m_net_vars.m_fall_velocity = valve::g_local_player->fall_velocity ( );
		m_net_vars.m_base_velocity = valve::g_local_player->base_velocity ( );
		m_net_vars.m_fall_velocity = valve::g_local_player->fall_velocity ( );
		m_net_vars.m_move_state = valve::g_local_player->move_state( );
		m_net_vars.m_tick_base = valve::g_local_player->tick_base( );

		m_net_vars.m_duck_amount = valve::g_local_player->duck_amount( );
		m_net_vars.m_duck_speed = valve::g_local_player->duck_speed( );
		m_net_vars.m_third_person_recoil = valve::g_local_player->third_person_recoil( );
		m_net_vars.m_lby = valve::g_local_player->lby( );

		m_net_vars.m_flags = valve::g_local_player->flags( );
		m_net_vars.m_move_type = valve::g_local_player->move_type( );

		m_pred_net_vars = m_net_vars;
	}

	__forceinline void c_eng_pred::store_data_map ( ) {
		m_velocity = valve::g_local_player->velocity ( );
		m_origin = valve::g_local_player->origin ( );
		m_aim_punch = valve::g_local_player->aim_punch ( );
		m_aim_punch_vel = valve::g_local_player->aim_punch_vel ( );
		m_view_punch = valve::g_local_player->view_punch ( );
		m_view_offset = valve::g_local_player->view_offset ( );
		m_tick_base = valve::g_local_player->tick_base ( );
		m_flags = valve::g_local_player->flags ( );
		m_move_type = valve::g_local_player->move_type ( );
		m_duck_amt = valve::g_local_player->duck_amount ( );
		m_duck_speed = valve::g_local_player->duck_speed ( );
		m_base_velocity = valve::g_local_player->base_velocity ( );
		m_velocity_modifier_data = valve::g_local_player->velocity_modifier ( );
		m_fall_velocity = valve::g_local_player->fall_velocity ( );
	}

	__forceinline void c_eng_pred::restore_data_map ( ) {
		valve::g_local_player->velocity ( ) = m_velocity;
		valve::g_local_player->origin ( ) = m_origin;
		valve::g_local_player->aim_punch ( ) = m_aim_punch;
		valve::g_local_player->aim_punch_vel ( ) = m_aim_punch_vel;
		valve::g_local_player->view_punch ( ) = m_view_punch;
		valve::g_local_player->view_offset ( ) = m_view_offset;
		valve::g_local_player->tick_base ( ) = m_tick_base;
		valve::g_local_player->flags ( ) = m_flags;
		valve::g_local_player->move_type ( ) = m_move_type;
		valve::g_local_player->duck_amount ( ) = m_duck_amt;
		valve::g_local_player->duck_speed ( ) = m_duck_speed;
		valve::g_local_player->base_velocity ( ) = m_base_velocity;
		valve::g_local_player->velocity_modifier ( ) = m_velocity_modifier_data;
		valve::g_local_player->fall_velocity ( ) = m_fall_velocity;
	}

	__forceinline void c_eng_pred::velocity_modifier_to_data_map ( )
	{
		static const auto& net_var = valve::g_net_vars->entry ( xorstr_ ( "CCSPlayer->m_flVelocityModifier" ) );

		valve::data_map_t* data_map = valve::g_local_player->get_pred_desc_map ( );
		valve::type_description_t* type_desc = valve::g_local_player->get_data_map_entry ( data_map, xorstr_ ( "m_chief_keef" ) );

		if ( std::get< std::uint32_t > ( net_var ) > 0 )
		{
			if ( type_desc )
			{
				const auto recovery_rate = 1.f / crypt_float ( 2.5f );
				const auto tolerance = recovery_rate * valve::g_global_vars->m_interval_per_tick;

				if ( type_desc->m_tolerance != tolerance )
				{
					int offset = std::get< std::uint32_t > ( net_var );

					type_desc->m_type = valve::e_field_type::_float;
					type_desc->m_tolerance = tolerance;
					type_desc->m_offset = offset;
					type_desc->m_field_size_in_bytes = sizeof ( float );
					type_desc->m_flat_offset [ 0 ] = offset;

					data_map->m_packed_offsets_computed = false;
					data_map->m_packed_size = 0;
				}
			}
		}
	}

	__forceinline void c_eng_pred::third_person_recoil_to_data_map ( )
	{
		static const auto& net_var = valve::g_net_vars->entry ( xorstr_ ( "CCSPlayer->m_flThirdpersonRecoil" ) );

		valve::data_map_t* data_map = valve::g_local_player->get_pred_desc_map ( );
		valve::type_description_t* type_desc = valve::g_local_player->get_data_map_entry ( data_map, xorstr_ ( "m_bitch_love_sosa" ) );

		if ( std::get< std::uint32_t > ( net_var ) > 0 )
		{
			if ( type_desc )
			{
				const auto tolerance = 0.5f;

				if ( type_desc->m_tolerance != tolerance )
				{
					int offset = std::get< std::uint32_t > ( net_var );

					type_desc->m_type = valve::e_field_type::_float;
					type_desc->m_tolerance = tolerance;
					type_desc->m_offset = offset;
					type_desc->m_field_size_in_bytes = sizeof ( float );
					type_desc->m_flat_offset [ 0 ] = offset;

					data_map->m_packed_offsets_computed = false;
					data_map->m_packed_size = 0;
				}
			}
		}
	}

	__forceinline void c_eng_pred::local_data_t::save( const int cmd_number ) {
		if ( cmd_number != m_user_cmd.m_number )
			return;

		store_anim( );

		m_pred_net_vars.m_ground_entity = valve::g_local_player->ground_entity ( );
		m_pred_net_vars.m_eye_angles = valve::g_local_player->eye_angles( );
		m_pred_net_vars.m_aim_punch = valve::g_local_player->aim_punch( );
		m_pred_net_vars.m_view_punch = valve::g_local_player->view_punch( );

		m_pred_net_vars.m_origin = valve::g_local_player->origin( );
		m_pred_net_vars.m_abs_origin = valve::g_local_player->abs_origin( );
		m_pred_net_vars.m_obb_min = valve::g_local_player->obb_min( );
		m_pred_net_vars.m_obb_max = valve::g_local_player->obb_max( );
		m_pred_net_vars.m_velocity = valve::g_local_player->velocity ( );
		m_pred_net_vars.m_view_offset = valve::g_local_player->view_offset( );
		m_pred_net_vars.m_aim_punch_vel = valve::g_local_player->aim_punch_vel( );
		m_pred_net_vars.m_abs_velocity = valve::g_local_player->abs_velocity( );

		m_pred_net_vars.m_surface_friction = valve::g_local_player->surface_friction ( );
		m_pred_net_vars.m_velocity_modifier = valve::g_local_player->velocity_modifier ( );
		m_pred_net_vars.m_strafing = valve::g_local_player->strafing( );
		m_pred_net_vars.m_scoped = valve::g_local_player->scoped( );
		m_pred_net_vars.m_walking = valve::g_local_player->walking( );
		m_pred_net_vars.m_base_velocity = valve::g_local_player->base_velocity ( );
		m_pred_net_vars.m_fall_velocity = valve::g_local_player->fall_velocity ( );

		m_pred_net_vars.m_move_state = valve::g_local_player->move_state( );
		m_pred_net_vars.m_tick_base = valve::g_local_player->tick_base( );

		m_pred_net_vars.m_duck_amount = valve::g_local_player->duck_amount( );
		m_pred_net_vars.m_duck_speed = valve::g_local_player->duck_speed( );
		m_pred_net_vars.m_third_person_recoil = valve::g_local_player->third_person_recoil( );
		m_pred_net_vars.m_lby = valve::g_local_player->lby( );

		m_pred_net_vars.m_flags = valve::g_local_player->flags( );
		m_pred_net_vars.m_move_type = valve::g_local_player->move_type( );
	}

	__forceinline bool& c_eng_pred::is_out_of_epsilon ( ) {
		return m_is_out_of_epsilon;
	}

	__forceinline void c_eng_pred::local_data_t::store_anim( ) {
		if ( const auto anim_state = valve::g_local_player->anim_state( ) ) {
			m_anim_state = *anim_state;
			m_abs_yaw = anim_state->m_foot_yaw;
		}

		m_anim_layers = valve::g_local_player->anim_layers( );
		m_pose_params = valve::g_local_player->pose_params( );
	}

	__forceinline void c_eng_pred::local_data_t::restore_anim( const bool layers ) const {
		if ( const auto anim_state = valve::g_local_player->anim_state( ) )
			*anim_state = m_anim_state;

		if ( layers )
			valve::g_local_player->anim_layers( ) = m_anim_layers;

		valve::g_local_player->pose_params( ) = m_pose_params;

		valve::g_local_player->set_abs_angles( { 0.f, m_abs_yaw, 0.f } );
	}

	__forceinline valve::e_frame_stage& c_eng_pred::last_frame_stage( ) {
		return m_last_frame_stage;
	}

	__forceinline float& c_eng_pred::inaccuracy ( ) {
		return m_inaccuracy;
	}

	__forceinline std::array< c_eng_pred::net_vars_t, 150u >& c_eng_pred::net_vars( ) {
		return m_net_vars;
	}

	__forceinline std::array< c_eng_pred::local_data_t, 150u >& c_eng_pred::local_data( ) {
		return m_local_data;
	}

	__forceinline valve::user_cmd_t& c_eng_pred::old_user_cmd( ) {
		return m_old_user_cmd;
	}

	__forceinline valve::user_cmd_t& c_eng_pred::last_user_cmd( ) {
		return m_last_user_cmd;
	}

	__forceinline void c_eng_pred::reset_on_spawn( ) {
		m_net_velocity_modifier = 1.f;

		std::memset( m_local_data.data( ), 0, sizeof( local_data_t ) * m_local_data.size( ) );
	}

	__forceinline float& c_eng_pred::spread( ) {
		return m_spread;
	}

	__forceinline float& c_eng_pred::min_inaccuracy( ) {
		return m_min_inaccuracy;
	}

	__forceinline float& c_eng_pred::recoil_index( ) {
		return m_recoil_index;
	}

	__forceinline float& c_eng_pred::accuracy_penalty( ) {
		return m_accuracy_penalty;
	}

	__forceinline float& c_eng_pred::velocity_modifier( ) {
		return m_velocity_modifier;
	}

	__forceinline float& c_eng_pred::net_velocity_modifier( ) {
		return m_net_velocity_modifier;
	}

	__forceinline bool& c_eng_pred::r8_can_shoot( ) {
		return m_r8_can_shoot;
	}

	__forceinline float& c_eng_pred::postpone_fire_ready_time( ) {
		return m_postpone_fire_ready_time;
	}

	__forceinline void c_eng_pred::save_view_model ( ) {
		if ( !valve::g_local_player->alive ( ) )
			return;

		auto view_model = valve::g_local_player->view_model ( );

		if ( !view_model )
			return;

		m_view_model.m_animation_parity = view_model->anim_parity ( );
		m_view_model.m_view_sequence = view_model->sequence ( );
		m_view_model.m_view_cycle = view_model->cycle ( );
		m_view_model.m_anim_time = view_model->anim_time ( );
	}

	__forceinline void c_eng_pred::adjust_view_model ( ) {
		if ( !valve::g_local_player->alive ( ) )
			return;

		auto view_model = valve::g_local_player->view_model ( );

		if ( !view_model )
			return;

		if ( m_view_model.m_view_sequence != view_model->sequence ( ) || m_view_model.m_animation_parity != view_model->anim_parity ( ) )
			return;

		view_model->cycle ( ) = m_view_model.m_view_cycle;
		view_model->anim_time ( ) = m_view_model.m_anim_time;
	}

	__forceinline void c_eng_pred::recompute_velocity_modifier (
		const std::ptrdiff_t cmd_num, const bool predicted
	) const {
		if ( !( valve::g_local_player->flags( ) & valve::e_ent_flags::on_ground ) ) {
			valve::g_local_player->velocity_modifier( ) = m_net_velocity_modifier;
			return;
		}

		const auto diff = cmd_num - valve___g_client_state->m_cmd_ack + ( predicted ? crypt_int ( 0 ) : crypt_int ( 1 ) );

		if ( diff < crypt_int ( 0 ) || m_net_velocity_modifier == crypt_float ( 1.f ) )
			return;

		if ( diff ) {
			auto calc_vel_mod = ( ( valve___g_global_vars->m_interval_per_tick * crypt_float ( 0.4f ) ) * static_cast < float > ( diff ) ) + m_net_velocity_modifier;

			calc_vel_mod = std::clamp ( calc_vel_mod, 0.f, 1.f );

			valve::g_local_player->velocity_modifier ( ) = calc_vel_mod;
		}
		else
			valve::g_local_player->velocity_modifier ( ) = m_net_velocity_modifier;
	}
}