#pragma once

namespace nem::valve {
	class c_entity {
	public:
		__forceinline std::uintptr_t renderable( );

		__forceinline std::uintptr_t networkable( );

		__forceinline int index( );

		__forceinline e_ent_handle handle( );

		__forceinline bool dormant( );

		__forceinline client_class_t* client_class( );

		__forceinline bool setup_bones(
			mat3x4_t* const bones, const int max_bones,
			const int mask, const float time
		);

		__forceinline valve::type_description_t* get_data_map_entry ( valve::data_map_t* map, const char* name )
		{
			while ( map )
			{
				for ( int i = 0; i < map->m_size; i++ )
				{
					if ( map->m_description[ i ].m_name == nullptr )
						continue;

					if ( strcmp ( name, map->m_description[ i ].m_name ) == 0 )
						return &map->m_description[ i ];
				}
				map = map->m_base_map;
			}

			return 0;
		}

		__forceinline bool& get_use_custom_bloom_scale ( );

		__forceinline bool& get_user_custom_auto_exposure_min ( );

		__forceinline bool& get_user_custom_auto_exposure_max ( );

		__forceinline float& get_custom_bloom_scale ( );

		__forceinline float& get_auto_custom_exposure_min ( );

		__forceinline float& get_auto_custom_exposure_max ( );

		__forceinline float& sim_time( );

		__forceinline float& old_sim_time( );

		__forceinline vec3_t& origin( );

		__forceinline qangle_t& abs_angles( );

		__forceinline vec3_t& abs_origin( );

		__forceinline vec3_t& abs_velocity( );

		__forceinline int& precip_type ( );

		__forceinline void set_abs_angles( 
			const qangle_t& abs_angles 
		);

		__forceinline void set_abs_origin(
			const vec3_t& abs_origin
		);

		__forceinline e_ent_flags& flags( );

		__forceinline std::uint32_t& eflags( );

		__forceinline model_t* model( );

		__forceinline model_data_t*& mdl_data( );

		__forceinline std::uint32_t& effects( );

		__forceinline std::uint32_t& occlusion_flags( );

		__forceinline int& occlusion_frame( );

		__forceinline ik_context_t*& ik_context( );

		__forceinline int& last_setup_bones_frame( );

		__forceinline void*& inverse_kinematics ( );

		__forceinline float& last_setup_bones_time( );

		__forceinline unsigned long& mdl_bone_counter( );

		__forceinline bone_accessor_t& bone_accessor( );

		__forceinline studio_hdr_t* studio_hdr_ ( );

		__forceinline utl_vec_t< mat3x4_t >& bone_cache( );

		__forceinline std::uint8_t& ent_client_flags( );

		__forceinline bool breakable( );

		__forceinline int& team( );

		__forceinline bool friendly( );

		__forceinline int& sim_tick( );

		__forceinline vec3_t& obb_min( );

		__forceinline vec3_t& obb_max( );

		__forceinline float& obb_change_time ( );

		__forceinline float& obb_change_height ( );

		__forceinline void set_collision_bounds(
			const vec3_t& obb_min, const vec3_t& obb_max, float change_time
		);

		__forceinline const vec3_t& world_space_center( );

		__forceinline bool is_player( );

		__forceinline valve::data_map_t* get_pred_desc_map ( );

		__forceinline mat3x4_t& get_rgfl ( );

		__forceinline bool is_weapon( );

		__forceinline var_mapping_t& var_mapping( );

		__forceinline float& cycle( );

		__forceinline float& anim_time( );

		__forceinline int& anim_parity ( );

		__forceinline e_ent_handle& move_parent( );

		__forceinline e_ent_handle& rag_doll ( );

		__forceinline bool& spotted( );

		__forceinline e_ent_handle& thrower( );

		__forceinline int& explode_effect_tick_begin( );

		__forceinline float& grenade_spawn_time( );

		__forceinline bool& use_fast_path( );

		__forceinline int& bomb_site( );

		__forceinline bool& is_bomb_ticking( );

		__forceinline float& c4_blow( );

		__forceinline vec3_t& base_velocity ( );

		__forceinline std::uint16_t model_instance( );

		__forceinline int& model_index( );

		__forceinline e_ent_handle& world_model_handle( );

		__forceinline int& sequence( );

		__forceinline e_ent_handle& view_model_weapon_handle( );

		__forceinline e_ent_handle& view_model_owner( );

		__forceinline void set_model_index( const int index );
	};


	struct inferno_t : public c_entity {
		//OFFSET ( get_spawn_time ( ), float, 0x20 );

		__forceinline float get_spwn_time ( )
		{
			return *reinterpret_cast< float* >(
				reinterpret_cast< std::uintptr_t >( this ) + 0x20u
				);
		}


		static float get_expiry_time ( )
		{
			return 7.03125f;
		}
	};

	class c_weapon : public c_entity {
	public:
		__forceinline float max_speed( );

		__forceinline e_item_index& item_index( );

		__forceinline float& postpone_fire_ready_time( );

		__forceinline float& accuracy_penalty( );

		__forceinline float& last_shot_time( );

		__forceinline float& recoil_index( );

		__forceinline int& ammo( );

		__forceinline float& next_primary_attack( );

		__forceinline float& next_secondary_attack( );

		__forceinline float& throw_time( );

		__forceinline float& throw_strength( );

		__forceinline bool& pin_pulled( );

		__forceinline float spread( );

		__forceinline float inaccuracy( );

		__forceinline void update_inaccuracy( );

		__forceinline weapon_data_t* wpn_data( );

		__forceinline bool& in_reload( );

		__forceinline int& zoom_lvl( );

		__forceinline int& item_id_high( );

		__forceinline int& account_id( );

		__forceinline int& fallback_paint_kit( );

		__forceinline int& item_quality( );

		__forceinline int& fallback_stat_trak( );

		__forceinline float& fallback_wear( );

		__forceinline int& owner_xuid_low( );

		__forceinline int& owner_xuid_high( );

		__forceinline bool& custom_material_initialized( );

		__forceinline utl_vec_t< ret_counted_t* >& custom_materials( );

		__forceinline utl_vec_t< ret_counted_t* >& custom_materials2( );

		__forceinline utl_vec_t< ret_counted_t* >& visuals_data_processors( );


		__forceinline std::string get_weapon_name ( )
		{
			auto get_clean_name = [ ] ( const char* name ) -> const char* {
				if ( name[ 0 ] == 'C' )
					name++;

				auto start = strstr ( name, xorstr_ ( "Weapon" ) );
				if ( start != nullptr )
					name = start + 6;

				return name;
			};

			if ( !this )
				return "";

			e_item_index weapon_index = item_index ( );


			std::string str_result = "";
			switch ( weapon_index )
			{
				case e_item_index::revolver: str_result = xorstr_ ( "Revolver R8" ); break;
				case e_item_index::scar20: str_result = xorstr_ ( "SCAR20" ); break;
				case e_item_index::deagle: str_result = xorstr_ ( "Deagle" ); break;
				case e_item_index::elite: str_result = xorstr_ ( "Dual elite" ); break;
				case e_item_index::fiveseven: str_result = xorstr_ ( "Fiveseven" ); break;
				case e_item_index::frag_grenade: str_result = xorstr_ ( "Frag grenade" ); break;
				case e_item_index::smoke_grenade: str_result = xorstr_ ( "Smoke grenade" ); break;
				case e_item_index::decoy: str_result = xorstr_ ( "Decoy grenade" ); break;
				case e_item_index::flashbang: str_result = xorstr_ ( "Flashbang" ); break;
				case e_item_index::hkp2000: str_result = xorstr_ ( "P2000" ); break;
				case e_item_index::inc_grenade: str_result = xorstr_ ( "Fire grenade" ); break;
				case e_item_index::molotov: str_result = xorstr_ ( "Molotov" ); break;
				case e_item_index::he_grenade: str_result = xorstr_ ( "Frag grenade" ); break;
				default: str_result = get_clean_name ( client_class ( )->m_network_name );
			}

			if ( str_result == xorstr_ ( "HKP2000" ) )
				return xorstr_ ( "P2000" );

			return str_result;
		}
		__forceinline bool is_knife ( )
		{
			if ( !this )
				return false;

			auto idx = item_index ( );

			return idx == e_item_index::knife || idx == e_item_index::knife_bayonet || idx == e_item_index::knife_butterfly || idx == e_item_index::knife_falchion
				|| idx == e_item_index::knife_flip || idx == e_item_index::knife_gut || idx == e_item_index::knife_karambit || idx == e_item_index::knife_m9_bayonet
				|| idx == e_item_index::knife_t
				|| idx == e_item_index::knife_stiletto
				|| idx == e_item_index::knife_ursus || idx == e_item_index::knife_skeleton;
		}

		__forceinline std::string get_weapon_icon ( )
		{
			if ( !this )
				return " ";

			if ( this->is_knife ( ) )
			{
				return xorstr_ ( "[" );
			}

			std::string str_result = "";
			switch ( this->item_index ( ) )
			{
				case e_item_index::scar20: str_result = xorstr_ ( "Y" ); break;
				case e_item_index::g3sg1: str_result = xorstr_ ( "X" ); break;
				case e_item_index::awp: str_result = xorstr_ ( "Z" ); break;
				case e_item_index::ssg08: str_result = xorstr_ ( "a" ); break;
				case e_item_index::deagle: str_result = xorstr_ ( "A" ); break;
				case e_item_index::revolver: str_result = xorstr_ ( "J" ); break;
				case e_item_index::hkp2000: str_result = xorstr_ ( "E" ); break; break;
				case e_item_index::glock: str_result = xorstr_ ( "D" ); break;
				case e_item_index::usp_silencer: str_result = xorstr_ ( "G" ); break;
				case e_item_index::elite: str_result = xorstr_ ( "B" ); break;
				case e_item_index::c4: str_result = xorstr_ ( "o" ); break;
				case e_item_index::p250: str_result = xorstr_ ( "F" ); break;
				case e_item_index::aug: str_result = xorstr_ ( "U" ); break;
				case e_item_index::fiveseven: str_result = xorstr_ ( "C" ); break;
				case e_item_index::ak47: str_result = xorstr_ ( "W" ); break;
				case e_item_index::galil: str_result = xorstr_ ( "Q" ); break;
				case e_item_index::cz75a: str_result = xorstr_ ( "I" ); break;
				case e_item_index::famas: str_result = xorstr_ ( "R" ); break;
				case e_item_index::tec9: str_result = xorstr_ ( "H" ); break;
				case e_item_index::bizon: str_result = xorstr_ ( "M" ); break;
				case e_item_index::m249: str_result = xorstr_ ( "g" ); break;
				case e_item_index::negev: str_result = xorstr_ ( "f" ); break;
				case e_item_index::nova: str_result = xorstr_ ( "e" ); break;
				case e_item_index::mag7: str_result = xorstr_ ( "d" ); break;
				case e_item_index::taser: str_result = xorstr_ ( "h" ); break;
				case e_item_index::he_grenade: str_result = xorstr_ ( "j" ); break;
				case e_item_index::smoke_grenade: str_result = xorstr_ ( "k" ); break;
				case e_item_index::frag_grenade: str_result = xorstr_ ( "n" ); break;
				case e_item_index::molotov: str_result = xorstr_ ( "l" ); break;
				case e_item_index::sawedoff: str_result = xorstr_ ( "c" ); break;
				case e_item_index::decoy: str_result = xorstr_ ( "m" ); break;
				case e_item_index::flashbang: str_result = xorstr_ ( "i" ); break;
				case e_item_index::m4a1: str_result = xorstr_ ( "S" ); break;
				case e_item_index::m4a1_silencer: str_result = xorstr_ ( "T" ); break;
				case e_item_index::firebomb: str_result = xorstr_ ( "k" ); break;
				case e_item_index::mac10: str_result = xorstr_ ( "K" ); break;
				case e_item_index::ump45: str_result = xorstr_ ( "L" ); break;
				case e_item_index::mp7: str_result = xorstr_ ( "N" ); break;
				case e_item_index::p90: str_result = xorstr_ ( "P" ); break;
				case e_item_index::mp9: str_result = xorstr_ ( "N" ); break;
				case e_item_index::sg553: str_result = xorstr_ ( "V" ); break;
				case e_item_index::xm1014: str_result = xorstr_ ( "e" ); break;
				case e_item_index::ta_grenade: str_result = xorstr_ ( "i" ); break;
			}

			return str_result;
		}
	};

	class c_cascade_light : c_entity {
	public:

		__forceinline vec3_t& get_shadow_direction ( );
			
	};

	class c_player : public c_entity {
	public:
		__forceinline void set_local_view_angles( const qangle_t& angle );

		__forceinline int& tick_base( );

		__forceinline int& physics_collision_state ( );

		__forceinline e_life_state& life_state( );

		__forceinline int& health( );

		__forceinline bool alive( );

		__forceinline void attachment_helper ( );

		__forceinline float& spawn_time( );

		__forceinline pose_params_t& pose_params( );

		__forceinline anim_layers_t& anim_layers( );

		__forceinline anim_state_t*& anim_state( );

		__forceinline bool& client_side_anim( );

		__forceinline void update_client_side_anim( );

		__forceinline qangle_t& eye_angles( );

		__forceinline void update_collision_bounds ( );

		__forceinline vec3_t& velocity( );

		__forceinline float max_delta ( );

		__forceinline float& fall_velocity ( );

		__forceinline float& duck_amount( );

		__forceinline e_ent_handle& wpn_handle( );

		__forceinline e_ent_handle& ground_entity( );

		__forceinline c_weapon* weapon( );

		__forceinline int& final_predicted_tick ( );

		__forceinline int& hitbox_set_index( );

		__forceinline int& skin( );

		__forceinline int& body( );

		__forceinline float& lby( );

		__forceinline int lookup_seq_act( const int sequence );

		__forceinline vec3_t& view_offset( );

		__forceinline qangle_t& aim_punch( );

		__forceinline vec3_t& aim_punch_vel( );

		__forceinline float& next_attack( );

		__forceinline bool& ducking( );

		__forceinline bool& ducked( );

		__forceinline bool& walking( );

		__forceinline bool& strafing( );

		__forceinline e_move_type& move_type( );

		__forceinline float& velocity_modifier( );

		__forceinline user_cmd_t*& cur_user_cmd( );

		__forceinline user_cmd_t& last_user_cmd( );

		__forceinline cmd_context_t& cmd_context( );

		__forceinline int& unknown_offset ( );

		__forceinline float get_layer_seq_cycle_rate ( valve::anim_layer_t* layer, int seq );

		__forceinline bool& scoped( );

		__forceinline qangle_t& view_punch( );

		__forceinline float& duck_speed( );

		__forceinline int& move_state( );

		__forceinline float& third_person_recoil( );

		__forceinline int lookup_bone( const char* name );

		__forceinline int& armor_value( );

		__forceinline bool& has_helmet( );

		__forceinline bool& has_heavy_armor( );

		__forceinline float& max_speed( );

		__forceinline float& surface_friction( );

		__forceinline bool& immune( );

		__forceinline e_ent_handle view_model_handle( );

		__forceinline c_entity* view_model( );

		__forceinline int& money( );

		__forceinline float fov( );

		__forceinline bool has_c4( );

		__forceinline int& observer_mode( );

		__forceinline e_ent_handle& observer_target( );

		__forceinline bool& use_new_anim_state( );

		__forceinline e_ent_handle* weapons( );

		__forceinline float& flash_duration( );

		__forceinline int& shots_fired( );

		__forceinline void eye_pos ( vec3_t* vec );

		__forceinline void modify_eye_pos ( vec3_t& eye_pos, valve::bones_t& bones )
		{
			if ( !this )
				return;

			if ( !this->anim_state ( ) )
				return;

			if ( !this->anim_state ( )->m_landing && this->anim_state ( )->m_duck_amount <= 0.f )
				return;

			auto head_bone = lookup_bone ( xorstr_ ( "head_0" ) );

			if ( head_bone != -1 ) {

				auto head_position = vec3_t ( bones [ 8 ][ 0 ][ 3 ],
					bones [ 8 ][ 1 ][ 3 ],
					bones [ 8 ][ 2 ][ 3 ] );

				auto head_height = head_position.z + 1.7f;

				if ( eye_pos.z > head_height ) {
					auto tmp = 0.0f;
					tmp = ( fabsf ( eye_pos.z - head_height ) - 4.0f ) * 0.16666667f;
					if ( tmp >= 0.0f )
						tmp = fminf ( tmp, 1.0f );
					eye_pos.z = ( ( head_height - eye_pos.z )
						* ( ( ( tmp * tmp ) * 3.0f ) - ( ( ( tmp * tmp ) * 2.0f ) * tmp ) ) )
						+ eye_pos.z;
				}
			}
		}

		__forceinline vec3_t get_shoot_pos ( valve::bones_t& bones, bool fake_ducking );

		__forceinline vec3_t who_tf_asked ( int hitbox_id, valve::bones_t bones )
		{
			if ( !this )
				return vec3_t ( );

			auto hdr = mdl_data ( )->m_studio_hdr;

			if ( !hdr )
				return vec3_t ( );

			auto hitbox_set = hdr->hitbox_set ( hitbox_set_index ( ) );

			if ( !hitbox_set )
				return vec3_t ( );

			auto hitbox = hitbox_set->hitbox ( hitbox_id );

			if ( !hitbox )
				return vec3_t ( );

			vec3_t min, max;

			math::vector_transform ( hitbox->m_min, bones[ hitbox->m_bone ], min );
			math::vector_transform ( hitbox->m_max, bones[ hitbox->m_bone ], max );

			return ( min + max ) * 0.5f;
		}

	};

	class i_client_unknown;

	class i_client_renderable {
	public:
		virtual i_client_unknown* get_client_unknown ( ) = 0;
	};
	class c_base_handle;
	class i_handle_entity {
	public:
		virtual							~i_handle_entity ( ) = default;
		virtual void					set_ref_handle ( const c_base_handle& handle ) = 0;
		virtual const c_base_handle& get_handle ( ) const = 0;
	};

	class i_client_unknown : public i_handle_entity {
	public:
		virtual void* get_collideable ( ) = 0;
		virtual void* get_client_networkable ( ) = 0;
		virtual i_client_renderable* get_client_renderable ( ) = 0;
		virtual void* get_client_entity ( ) = 0;
		virtual c_entity* get_base_entity ( ) = 0;
		virtual void* get_client_thinkable ( ) = 0;
		virtual void* get_client_alpha_property ( ) = 0;
	};

	class i_client_networkable {
	public:
		virtual i_client_unknown* get_client_unk ( ) = 0;
		virtual void release ( ) = 0;
		virtual void* client_class ( ) = 0;
		virtual void who ( int who ) = 0;
		virtual void on_pre_data_changed ( int type ) = 0;
		virtual void on_data_changed ( int type ) = 0;
		virtual void pre_data_update ( int type ) = 0;
		virtual void post_data_update ( int type ) = 0;
	};

	class c_local_player {
	private:
		c_player** m_local{};
	public:
		__forceinline operator bool( ) const;

		__forceinline operator c_player*( ) const;

		__forceinline c_player* operator ->( ) const;
	} inline g_local_player{};
}

#include "impl/entities.inl"