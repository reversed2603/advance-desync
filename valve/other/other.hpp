#pragma once

namespace nem::valve {
	enum struct e_ent_handle : unsigned long { invalid = std::numeric_limits< unsigned long >::max( ) };

	struct move_msg_t {
		char	pad0[ 12u ]{};
		int		m_backup_cmds{};
		int		m_new_cmds{};
	};

	struct vertex_t {
		vec2_t m_pos{}, m_texture_pos{};
	};

	struct view_setup_t {
		int			m_x{};
		int			m_x_unscaled{};
		int			m_y{};
		int			m_y_unscaled{};
		int			m_width{};
		int			m_old_width{};
		int			m_height{};
		int			m_old_height{};
		char		pad0[ 144u ]{};
		float		m_fov{};
		float		m_view_model_fov{};
		vec3_t		m_origin{};
		qangle_t	m_angles{};
		float       m_znear{};
		float       m_zfar{};
		float       m_znear_viewmodel{};
		float       m_zfar_viewmodel{};
		float       m_aspect_ratio{};
		float       m_near_blur_depth{};
		float       m_near_focus_depth{};
		float       m_far_focus_depth{};
		float       m_far_blur_depth{};
		float       m_near_blur_radius{};
		float       m_far_blur_radius{};
		int         m_quality{};
		int         m_motionblur_mode{};
		float       m_shutter_time{};
		vec3_t      m_shutter_open_pos{};
		qangle_t    m_shutter_open_angles{};
		vec3_t      m_shutter_close_pos{};
		qangle_t	m_shutter_close_angles{};
		float       m_offcenter_top{};
		float       m_offcenter_bottom{};
		float       m_offcenter_left{};
		float       m_offcenter_right{};
		int         m_edge_blur{};
	};

	struct item_schema_t {
		template < typename _key_t, typename _value_t >
		struct node_t {
			int			m_prev_id{};
			int			m_next_id{};
			char		pad0[ 8u ]{};
			_key_t		m_key{};
			_value_t	m_value{};
		};

		template < typename _key_t, typename _value_t >
		struct head_t {
			node_t< _key_t, _value_t >* m_mem{};
			int							m_alloc_count{};
			int							m_grow_size{};
			int							m_start_element{};
			int							m_next_available{};
			char						pad0[ 4u ]{};
			int							m_last_element{};
		};

		struct str_t {
			char* m_buffer{};
			int		m_capacity{};
			int		m_grow_size{};
			int		m_length{};
		};

		struct paint_kit_t {
			int		m_id{};
			str_t	m_name{};
			str_t	m_description{};
			str_t	m_item_name{};
		};

		char						pad0[ 652u ]{};
		head_t< int, paint_kit_t* > m_paint_kits{};
	};

	struct mat_sys_cfg_t {
		struct view_mode_t {
			int m_width{};
			int m_height{};
			int m_format{};
			int m_refresh_tate{};
		};

		view_mode_t m_vide_mode{};
		float		m_monitor_gamma{};
		float		m_gamma_tv_range_min{};
		float		m_gamma_tv_range_max{};
		float		m_gamma_tv_exponent{};
		bool		m_gamma_tv_enabled{};
		bool        m_triple_buffered{};
		int			m_aa_samples{};
		int			m_force_anisotropic_lvl{};
		int			m_skip_mip_lvls{};
		int			m_dx_support_lvl{};
		int			m_flags{};
		bool		m_edit_mode{};
		char		m_proxies_test_mode{};
		bool		m_compressed_textures{};
		bool		m_filter_lightmaps{};
		bool		m_filter_textures{};
		bool		m_reverse_depth{};
		bool		m_buffer_primitives{};
		bool		m_draw_flat{};
		bool		m_measure_fill_rate{};
		bool		m_visualize_fill_rate{};
		bool		m_no_transparency{};
		bool		m_software_lighting{};
		bool		m_allow_cheats{};
		char		m_show_mip_lvls{};
		bool		m_show_low_res_image{};
		bool		m_show_normal_map{};
		bool		m_mip_map_textures{};
		char		m_fullbright{};
		bool		m_fast_no_bump{};
		bool		m_suppress_rendering{};
		bool		m_draw_gray{};
		bool		m_show_specular{};
		bool		m_show_diffuse{};
		int			m_windowed_size_limit_width{};
		int			m_windowed_size_limit_height{};
		int			m_aa_quality{};
		bool		m_shadow_depth_texture{};
		bool		m_motion_blur{};
		bool		m_support_flashlight{};
		bool		m_paint_enabled{};
		char pad0[ 0xC ]{};
	};

	struct game_event_t {
		__forceinline const char* name( );

		__forceinline float get_float( const char* name );

		__forceinline int get_int( const char* name );

		__forceinline const char* get_str ( const char* name, const char* def_val = "" );
	};

	struct base_event_listener_t {
		virtual void unk( ) {};

		virtual void fire_game_event( game_event_t* const event ) = 0;

		virtual int debug_event_id( ) { return 42; }
	};

	struct material_var_t {
		__forceinline void set_float( const float value );

		__forceinline void set_int( const int value );

		__forceinline void set_str( const char* value );

		__forceinline void set_vec( const float v0, const float v1, const float v2 );

		__forceinline void set_comp_vec ( const float val, const int comp );
	};

	struct material_t {
		__forceinline const char* name( );

		__forceinline const char* texture_group( );

		__forceinline void set_alpha( const float value );

		__forceinline void set_clr( const float r, const float g, const float b );

		__forceinline void set_flag( const int flag, const bool value );

		__forceinline material_var_t* find_var( const char* name, bool* const found, const bool complain = true );
	};

	enum struct e_move_type : std::uint8_t {
		none,
		isometric,
		walk,
		step,
		fly,
		fly_gravity,
		vphysics,
		push,
		noclip,
		ladder,
		observer,
		custom
	};

	enum e_buttons : std::uint32_t {
		in_attack = 1u << 0u,
		in_jump = 1u << 1u,
		in_duck = 1u << 2u,
		in_forward = 1u << 3u,
		in_back = 1u << 4u,
		in_use = 1u << 5u,
		in_cancel = 1u << 6u,
		in_left = 1u << 7u,
		in_right = 1u << 8u,
		in_move_left = 1u << 9u,
		in_move_right = 1u << 10u,
		in_attack2 = 1u << 11u,
		in_run = 1u << 12u,
		in_reload = 1u << 13u,
		in_alt1 = 1u << 14u,
		in_alt2 = 1u << 15u,
		in_score = 1u << 16u,
		in_speed = 1u << 17u,
		in_walk = 1u << 18u,
		in_zoom = 1u << 19u,
		in_weapon1 = 1u << 20u,
		in_weapon2 = 1u << 21u,
		in_bullrush = 1u << 22u,
		in_grenade1 = 1u << 23u,
		in_grenade2 = 1u << 24u,
		in_attack3 = 1u << 25u
	};
	DEFINE_ENUM_FLAG_OPERATORS( e_buttons )

	struct weapon_data_t {
		char		pad0[ 4u ]{};
		const char*	m_con_name{};
		char		pad1[ 12u ]{};
		int			m_max_clip1{};
		int			m_max_clip2{};
		int			m_default_clip1{};
		int			m_default_clip2{};
		int			m_primary_reserve_ammo_max{};
		int			m_secondary_reserve_ammo_max{};
		const char*	m_world_model{};
		const char*	m_view_model{};
		const char*	m_dropped_model{};
		char		pad2[ 80u ]{};
		const char*	m_hud_name{};
		const char*	m_name{};
		char		pad3[ 2u ]{};
		bool		m_is_melee{};
		char		pad4[ 9u ]{};
		float		m_weight{};
		char		pad5[ 40u ]{};
		int			m_type{};
		int			m_unk_type{};
		int			m_price{};
		int			m_kill_award{};
		char		pad7[ 4u ]{};
		float		m_cycle_time{};
		float		m_cycle_time_alt{};
		char		pad8[ 8u ]{};
		bool		m_full_auto{};
		char		pad9[ 3u ]{};
		int			m_dmg{};
		float		m_headshot_multiplier{};
		float		m_armor_ratio{};
		int			m_bullets{};
		float		m_penetration{};
		char		pad10[ 8u ]{};
		float		m_range{};
		float		m_range_modifier{};
		float		m_throw_velocity{};
		char		pad11[ 12u ]{};
		bool		m_has_silencer{};
		char		pad12[ 11u ]{};
		const char*	m_bullet_type{};
		float		m_max_speed{};
		float		m_max_speed_alt{};
		char		pad13[ 4u ]{};
		float		m_spread{};
		float		m_spread_alt{};
		float		m_inaccuracy_crouch{};
		float		m_inaccuracy_crouch_alt{};
		float		m_inaccuracy_stand{};
		float		m_inaccuracy_stand_alt{};
		float		m_inaccuracy_jump_initial{};
		float		m_inaccuracy_jump_apex{};
		float		m_inaccuracy_jump{};
		float		m_inaccuracy_jump_alt{};
		float		m_inaccuracy_land{};
		float		m_inaccuracy_land_alt{};
		float		m_inaccuracy_ladder{};
		float		m_inaccuracy_ladder_alt{};
		float		m_inaccuracy_fire{};
		float		m_inaccuracy_fire_alt{};
		float		m_inaccuracy_move{};
		float		m_inaccuracy_move_alt{};
		float		m_inaccuracy_reload{};
		int			m_recoil_seed{};
		float		m_recoil_angle{};
		float		m_recoil_angle_alt{};
		float		m_recoil_angle_variance{};
		float		m_recoil_angle_variance_alt{};
		float		m_recoil_magnitude{};
		float		m_recoil_magnitude_alt{};
		float		m_recoil_magnitude_variance{};
		float		m_recoil_magnitude_variance_alt{};
		int			m_spread_seed{};
	};

	enum struct e_item_index : std::uint16_t {
		none,
		deagle,
		elite,
		fiveseven,
		glock,
		ak47 = 7u,
		aug,
		awp,
		famas,
		g3sg1,
		galil = 13u,
		m249,
		m4a1 = 16u,
		mac10,
		p90 = 19u,
		zone_repulsor,
		mp5sd = 23u,
		ump45,
		xm1014,
		bizon,
		mag7,
		negev,
		sawedoff,
		tec9,
		taser,
		hkp2000,
		mp7,
		mp9,
		nova,
		p250,
		shield,
		scar20,
		sg553,
		ssg08,
		knife_gg,
		knife,
		flashbang,
		he_grenade,
		smoke_grenade,
		molotov,
		decoy,
		inc_grenade,
		c4,
		healthshot = 57u,
		knife_t = 59u,
		m4a1_silencer,
		usp_silencer,
		cz75a = 63u,
		revolver,
		ta_grenade = 68u,
		fists,
		breachcharge,
		tablet = 72u,
		melee = 74u,
		axe,
		hammer,
		spanner = 78u,
		knife_ghost = 80u,
		firebomb,
		diversion,
		frag_grenade,
		snowball,
		bumpmine,
		knife_bayonet = 500u,
		knife_css = 503u,
		knife_flip = 505u,
		knife_gut,
		knife_karambit,
		knife_m9_bayonet,
		knife_tactical,
		knife_falchion = 512u,
		knife_survival_bowie = 514u,
		knife_butterfly,
		knife_push,
		knife_cord,
		knife_canis,
		knife_ursus,
		knife_gypsy_jackknife,
		knife_outdoor,
		knife_stiletto,
		knife_widowmaker,
		knife_skeleton = 525u
	};

	enum e_mask : std::uint32_t {
		solid = 0x200400bu,
		shot_hull = 0x600400bu,
		shot_player = 0x4600400bu,
		contents_hitbox = 0x40000000u,
		surf_hitbox = 0x8000u,
		surf_nodraw = 0x0080u,
		contents_grate = 0x8u,
		contents_solid = 1u,
		contents_current_90 = 0x80000u,
		contents_monster = 0x2000000u,
		contents_opaque = 0x80u,
		contents_ignore_nodraw_opaque = 0x2000u,
		all = 0xffffffffu
	};
	DEFINE_ENUM_FLAG_OPERATORS( e_mask )

	struct net_channel_info_t {
		__forceinline float latency( const int flow );

		__forceinline float avg_latency( const int flow );
	};

	enum e_solid_type : int {
		solid_none = 0,
		solid_bsp = 1,
		solid_bbox = 2,
		solid_obb = 3,
		solid_obb_yaw = 4,
		solid_custom = 5,
		solid_vphysics = 6,
		solid_last
	};

	struct ray_t {
		__forceinline constexpr ray_t( ) = default;

		__forceinline ray_t( const vec3_t& start, const vec3_t& end )
			: m_start{ start },
			  m_delta{ end - start }, m_is_ray{ true },
			  m_is_swept{ m_delta.length_sqr( ) != 0.f } {}

		__forceinline ray_t(
			const vec3_t& start, const vec3_t& end,
			const vec3_t& mins, const vec3_t& maxs
		) : m_start{ start + ( mins + maxs ) * 0.5f },
			m_delta{ end - start },
			m_start_offset{ ( mins + maxs ) * -0.5f },
			m_extents{ ( maxs - mins ) * 0.5f },
			m_is_ray{ m_extents.length_sqr( ) < 1e-6 },
			m_is_swept{ m_delta.length_sqr( ) != 0.f } { }

		vec3_t				m_start{};
		char				pad0[ 4u ]{};
		vec3_t				m_delta{};
		char				pad1[ 4u ]{};
		vec3_t				m_start_offset{};
		char				pad2[ 4u ]{};
		vec3_t				m_extents{};
		char				pad3[ 4u ]{};
		mat3x4_t*			m_world_axis_transform{};
		bool				m_is_ray{};
		bool				m_is_swept{};
	};

	class c_collideable {
	public:
		vec3_t& obb_mins ( );
		vec3_t& obb_maxs ( );
		e_solid_type solid_type ( );
	};

	class c_entity;

	struct beam_info_t {
		int			m_type{};
		c_entity*	m_start_entity{};
		int			m_start_attachment{};
		c_entity*	m_end_entity{};
		int			m_end_attachment{};
		vec3_t		m_start{};
		vec3_t		m_end{};
		int			m_model_index{ -1 };
		const char* m_model_name{};
		int			m_halo_index{ -1 };
		const char* m_halo_name{};
		float		m_halo_scale{};
		float		m_life{};
		float		m_width{};
		float		m_end_width{};
		float		m_fade_length{};
		float		m_amplitude{};
		float		m_brightness{};
		float		m_speed{};
		int			m_start_frame{};
		float		m_frame_rate{};
		float		m_red{};
		float		m_green{};
		float		m_blue{};
		bool		m_renderable{ true };
		int			m_segments{ -1 };
		int			m_flags{};
		vec3_t		m_center{};
		float		m_start_radius{};
		float		m_end_radius{};
	};

	struct trace_t {
		__forceinline void clear( );

		__forceinline bool hit( ) const;

		vec3_t			m_start_pos{};
		vec3_t			m_end_pos{};

		struct {
			vec3_t	m_normal{};
			float	m_dist{};
			char	m_type{};
			char	m_signbits{};
			char	pad0[ 2u ]{};
		}				m_plane{};

		float			m_fraction{};

		int				m_contents{};
		std::uint16_t	m_disp_flags{};

		bool			m_all_solid{};
		bool			m_start_solid{};
		float			m_fraction_left_solid{};

		struct {
			const char*		m_name{};
			short			m_surface_props{};
			std::uint16_t	m_flags{};
		}				m_surface{};

		int				m_hitgroup{};
		short			m_physics_bone{};
		std::uint16_t	m_world_surface_index{};
		c_entity*		m_hit_entity{};
		int				m_hitbox{};
	};

	struct trace_filter_t {
		virtual bool should_hit_entity ( c_entity* const entity, const e_mask mask ) const = 0;

		virtual int type ( ) const = 0;
	};

	using should_hit_fn_t = bool ( __cdecl* )( c_entity* const, const valve::e_mask );

	struct trace_filter_simple_t {
		__forceinline trace_filter_simple_t ( )
			: m_vtable { g_context->addresses ( ).m_trace_filter_simple_vtable.get ( ) }
		{
		}

		__forceinline trace_filter_simple_t (
			c_entity* const ignore_entity, const int collision_group
		) : m_vtable { g_context->addresses ( ).m_trace_filter_simple_vtable.get ( ) },
			m_ignore_entity { ignore_entity }, m_collision_group { collision_group } {}

		std::uintptr_t	m_vtable {};
		c_entity* m_ignore_entity {};
		int				m_collision_group {};
		should_hit_fn_t	m_should_hit_fn {};
	};

	struct trace_filter_skip_two_entities_t {
		__forceinline trace_filter_skip_two_entities_t ( )
			: m_vtable { g_context->addresses ( ).m_trace_filter_skip_two_entities_vtable.get ( ) }
		{
		}

		__forceinline trace_filter_skip_two_entities_t (
			c_entity* const ignore_entity0, c_entity* const ignore_entity1, const int collision_group = 0
		) : m_vtable { g_context->addresses ( ).m_trace_filter_skip_two_entities_vtable.get ( ) },
			m_ignore_entity0 { ignore_entity0 }, m_collision_group { collision_group }, m_ignore_entity1 { ignore_entity1 } {}

		std::uintptr_t	m_vtable {};
		c_entity* m_ignore_entity0 {};
		int				m_collision_group {};
		should_hit_fn_t	m_should_hit_fn {};
		c_entity* m_ignore_entity1 {};
	};

	struct trace_filter_world_only_t : public trace_filter_t {
		virtual bool should_hit_entity ( c_entity* const entity, const e_mask mask ) const { return false; }

		virtual int type ( ) const { return 1; }
	};

	struct surface_data_t {
		struct {
			float    m_friction{};
			float    m_elasticity{};
			float    m_density{};
			float    m_thickness{};
			float    m_dampening{};
		}		m_physics{};

		struct {
			float    m_reflectivity{};
			float    m_hardness_factor{};
			float    m_roughness_factor{};
			float    m_rough_threshold{};
			float    m_hard_threshold{};
			float    m_hard_velocity_threshold{};
			float    m_high_pitch_occlusion{};
			float    m_mid_pitch_occlusion{};
			float    m_low_pitch_occlusion{};
		}		m_audio{};

		struct {
			std::uint16_t m_walk_step_left{};
			std::uint16_t m_walk_step_right{};
			std::uint16_t m_run_step_left{};
			std::uint16_t m_run_step_right{};
			std::uint16_t m_impact_soft{};
			std::uint16_t m_impact_hard{};
			std::uint16_t m_scrape_smooth{};
			std::uint16_t m_scrape_rough{};
			std::uint16_t m_bullet_impact{};
			std::uint16_t m_rolling{};
			std::uint16_t m_break_sound{};
			std::uint16_t m_strain_sound{};
		}		m_sounds{};

		struct {
			float			m_max_speed_factor{};
			float			m_jump_factor{};
			float			m_penetration_modifier{};
			float			m_damage_modifier{};
			std::uint16_t	m_material{};
			std::uint8_t	m_climbable{};
		}		m_game{};

		char	pad0[ 48u ]{};
	};

	struct move_data_t {
		bool			m_first_run{};
		bool			m_game_code_moved_player{};
		bool			m_no_air_control{};
		e_ent_handle	m_player_handle{};
		int				m_impulse_command{};
		qangle_t		m_view_angles{};
		qangle_t		m_abs_view_angles{};
		e_buttons		m_buttons{};
		e_buttons		m_old_buttons{};
		vec3_t			m_move{};
		float			m_max_speed{};
		float			m_client_max_speed{};
		vec3_t			m_velocity{};
		vec3_t			m_trailing_velocity{};
		float			m_trailing_velocity_time{};
		qangle_t		m_angles{};
		qangle_t		m_old_angles{};
		float			m_out_step_height{};
		vec3_t			m_out_wish_velocity{};
		vec3_t			m_out_jump_velocity{};
		vec3_t			m_constraint_center{};
		float			m_constraint_radius{};
		float			m_constraint_width{};
		float			m_constraint_speed_factor{};
		bool			m_constraint_past_radius{};
		vec3_t			m_abs_origin{};
	};

	struct net_channel_t {
		__forceinline void send_datagram( const std::uintptr_t data );

		char pad0[ 20u ]{};
		bool m_processing_messages{};
		bool m_should_delete{};
		char pad1[ 2u ]{};
		int	 m_out_seq_number{};
		int	 m_in_seq_number{};
		int  m_out_seq_number_ack{};
		int  m_out_reliable_state{};
		int  m_in_reliable_state{};
		int  m_choked_packets{};
		char pad2[ 1044u ]{};
	};

	struct user_cmd_t {
		__forceinline std::uint32_t calc_checksum( );

		char		pad0[ 4u ];
		int			m_number{};
		int			m_tick{};
		qangle_t	m_view_angles{};
		vec3_t		m_aim_direction{};
		vec3_t		m_move{};
		e_buttons	m_buttons{};
		char		m_impulse{};
		int			m_weapon_select{};
		int			m_weapon_sub_type{};
		int			m_random_seed{};
		short		m_mouse_dx{};
		short		m_mouse_dy{};
		bool		m_predicted{};
		char		pad1[ 24u ]{};
	};

	struct cmd_context_t {
		bool		m_needs_processing{};
		user_cmd_t	m_user_cmd{};
		int			m_cmd_number{};
	};

	struct verified_user_cmd_t {
		user_cmd_t		m_cmd{};
		std::uint32_t	m_crc{};
	};

	enum struct e_frame_stage : int {
		undefined = -1,
		start,
		net_update_start,
		net_update_post_data_update_start,
		net_update_post_data_update_end,
		net_update_end,
		render_start,
		render_end
	};

	using bones_t = std::array< mat3x4_t, 128u >;

	struct ik_context_t {};

	class c_player;

	struct bone_accessor_t {
		c_player*	m_animating{};
		mat3x4_t*	m_bones{};
		int			m_readable_bones{};
		int			m_writable_bones{};
	};

	struct studio_hitbox_t {
		int         m_bone{};
		int         m_group{};
		vec3_t      m_min{};
		vec3_t      m_max{};
		int         m_hitbox_name_index{};
		qangle_t	m_rotation{};
		float       m_radius{};
		char		pad0[ 16u ]{};
	};

	struct studio_bone_t {
		int			m_name_index{};
		int			m_parent{};
		int			m_bone_controller[ 6u ]{};
		vec3_t		m_pos{};
		float		m_quat[ 4u ]{};
		vec3_t		m_rot{};
		vec3_t		m_pos_scale{};
		vec3_t		m_rot_scale{};
		mat3x4_t	m_pose_to_bone{};
		float		m_alignment[ 4u ]{};
		int			m_flags{};
		int			m_proc_type{};
		int			m_proc_index{};
		int			m_physics_bone{};
		int			m_surface_prop_index{};
		int			m_contents{};
		int			m_surface_prop_lookup{};
		char		pad0[ 28u ]{};
	};

	struct studio_hitbox_set_t {
		__forceinline studio_hitbox_t* hitbox( const int index );

		int    m_name_index{};
		int    m_hitboxes_count{};
		int    m_hitbox_index{};
	};

	struct studio_hdr_t {
		__forceinline studio_hitbox_set_t* hitbox_set( const int index );

		inline studio_hitbox_t* p_hitbox ( int i, int set );

		__forceinline studio_bone_t* bone( const int index );

		int						m_id{};
		int						m_version{};
		long					m_checksum{};
		char					m_name[ 64u ]{};
		int						m_length{};
		vec3_t					m_eye_pos{};
		vec3_t					m_illum_pos{};
		vec3_t					m_hull_min{};
		vec3_t					m_hull_max{};
		vec3_t					m_obb_min{};
		vec3_t					m_obb_max{};
		std::uint32_t			m_flags{};
		int						m_bones_count{};
		int						m_bone_index{};
		int						m_bone_controllers_count{};
		int						m_bone_controller_index{};
		int						m_hitbox_sets_count{};
		int						m_hitbox_set_index{};
	};

	struct draw_model_state_t {
		studio_hdr_t*	m_studio_hdr{};
		std::uintptr_t	m_studio_hw_data{};
		std::uintptr_t	m_renderable{};
		mat3x4_t*		m_model_to_world{};
		std::uintptr_t	m_decal_handle{};
		int				m_draw_flags{};
		int				m_lod{};
	};

	struct draw_model_info_t {
		studio_hdr_t*	m_studio_hdr{};
		std::uintptr_t	m_studio_hw_data{};
		uint16_t		m_decals{};
		int				m_skin{};
		int				m_body{};
		int				m_hitbox_set{};
		std::uintptr_t	m_renderable{};
		int				m_lod{};
		std::uintptr_t	m_clr_meshes{};
		bool			m_static_lighting{};
		char			m_lighting_state[ 440u ]{};
	};

	template < typename _value_t >
	struct utl_mem_t {
		_value_t*	m_ptr{};
		int			m_alloc_count{};
		int			m_grow_size{};
	};

	template < typename _value_t >
	struct utl_vec_t {
		__forceinline _value_t& at ( const int i );

		void purge ( )
		{
			m_mem.m_alloc_count = 0;
			m_mem.m_ptr = nullptr;
			m_elements = 0;
		}

		__forceinline const _value_t& at ( const int i ) const;
		__forceinline ~utl_vec_t ( ) { purge ( ); };

		utl_mem_t< _value_t >	m_mem {};
		int						m_size {};
		_value_t* m_elements {};

		inline void destruct ( _value_t* memory )
		{
			memory->~_value_t ( );
		}

		__forceinline void remove_all ( ) { 
			for ( int i = m_size; --i >= 0; ) {
				destruct ( &at ( i ) );
			}

			m_size = 0;
		}

	};
	struct cvar_t {
		using callback_t = void ( __cdecl* )( );

		__forceinline void set_int ( const int value );

		__forceinline void set_float ( const float value );

		__forceinline void set_str ( const char* value );

		__forceinline float get_float ( );

		__forceinline const char* get_str ( );

		__forceinline int get_int ( );

		__forceinline bool get_bool ( );

		char			pad0 [ 4u ] {};
		cvar_t* m_next {};
		int				m_registered {};
		const char* m_name {};
		const char* m_help_str {};
		std::uint32_t	m_flags {};
		callback_t		m_callback {};
		cvar_t* m_parent {};
		char* m_def_value {};
		const char* m_str {};
		int				m_str_len {};
		float			m_float_value {};
		int				m_int_value {};
		int				m_has_min {};
		float			m_min_value {};
		int				m_has_max {};
		float			m_max_value {};
		valve::utl_vec_t < callback_t > m_callbacks {};
	};
	struct model_t {
		char			pad0[ 4u ]{};
		char			m_name[ 260 ]{};
		int				m_load_flags{};
		int				m_server_count{};
		int				m_type{};
		int				m_flags{};
		vec3_t			m_mins{};
		vec3_t			m_maxs{};
		float			m_radius{};
		std::uintptr_t	m_key_values{};

		union {
			std::uintptr_t	m_brush{};
			std::uint16_t	m_studio;
			std::uintptr_t	m_sprite;
		};
	};

	struct model_render_info_t {
		vec3_t			m_origin{};
		qangle_t		m_angles{};
		char			pad[ 4u ]{};
		std::uintptr_t	m_renderable{};
		model_t*		m_model{};
		mat3x4_t*		m_model_to_world{};
		mat3x4_t*		m_lighting_offset{};
		vec3_t*			m_lighting_origin{};
		int				m_flags{};
		int				m_index{};
		int				m_skin{};
		int				m_body{};
		int				m_hitbox_set{};
		std::uint16_t	m_instance{};
	};

	struct model_data_t {
		studio_hdr_t*				m_studio_hdr{};
		char						pad0[ 44u ]{};
		utl_vec_t< std::uint32_t >	m_bone_flags{};
	};

	struct var_mapping_t {
		struct entry_t {
			std::uint16_t	m_type{};
			std::uint16_t	m_needs_to_interpolate{};
			void* m_data{};
			void* m_watcher{};
		};

		utl_vec_t< entry_t >	m_entries{};
		int						m_interpolated_entries{};
		float					m_last_interpolation_time{};
	};

	struct bf_write_t {
		__forceinline void write_user_cmd( user_cmd_t* const to, user_cmd_t* const from );
	};

	struct player_info_t {
		std::uint8_t	pad[ 8u ]{};

		union {
			struct {
				int	m_xuid_low;
				int	m_xuid_high;
			};

			std::int64_t m_xuid{};
		};

		char			m_name[ 128u ]{};
		int				m_uid{};
		char			m_guid[ 33u ]{};
		std::uint32_t	m_friends_id{};
		char			m_friends_name[ 128u ]{};
		bool			m_fake_player{};
		bool			m_is_hltv{};
		std::uint32_t	m_custom_files[ 4u ]{};
		unsigned char	m_files_downloaded{};
	};

	enum e_ent_flags : std::uint32_t {
		on_ground = 1u << 0u,
		ducking = 1u << 1u,
		anim_ducking = 1u << 2u,
		water_jump = 1u << 3u,
		on_train = 1u << 4u,
		in_rain = 1u << 5u,
		frozen = 1u << 6u,
		at_controls = 1u << 7u,
		client = 1u << 8u,
		fake_client = 1u << 9u,
		in_water = 1u << 10u,
		fly = 1u << 11u,
		swim = 1u << 12u,
		conveyor = 1u << 13u,
		npc = 1u << 14u,
		godmode = 1u << 15u,
		no_target = 1u << 16u,
		aim_target = 1u << 17u,
		partial_ground = 1u << 18u,
		static_prop = 1u << 19u,
		graphed = 1u << 20u,
		grenade = 1u << 21u,
		step_movement = 1u << 22u,
		dont_touch = 1u << 23u,
		base_velocity = 1u << 24u,
		world_brush = 1u << 25u,
		object = 1u << 26u,
		kill_me = 1u << 27u,
		on_fire = 1u << 28u,
		dissolving = 1u << 29u,
		ragdoll = 1u << 30u,
		unblockable_by_player = 1u << 31u
	};
	DEFINE_ENUM_FLAG_OPERATORS( e_ent_flags )

	struct anim_layer_t {
		float			m_anim_time{};
		float			m_fade_out_time{};
		std::uintptr_t	m_studio_hdr{};
		int				m_dispatched_src{};
		int				m_dispatched_dst{};
		int				m_order{};
		int				m_sequence{};
		float			m_prev_cycle{};
		float			m_weight{};
		float			m_weight_delta_rate{};
		float			m_playback_rate{};
		float			m_cycle{};
		c_player*		m_owner{};
		int				m_invalidate_physics_bits{};
	};

	using anim_layers_t = std::array< anim_layer_t, 13u >;

	using pose_params_t = std::array< float, 24u >;

	class c_weapon;

	struct anim_state_t {
		__forceinline void reset( );

		__forceinline void update(
			const float pitch, const float yaw, const float roll
		);

		__forceinline int select_sequence_from_acitivty_modifier ( int activity )
		{
			bool is_player_ducked = m_duck_amount > 0.55f;
			bool is_player_running = m_speed_as_portion_of_walk_speed > 0.25f;

			int32_t cur_sequence = -1;
			switch ( activity )
			{
				case 985:
				{
					cur_sequence = 15 + static_cast < int32_t >( is_player_running );
					if ( is_player_ducked )
						cur_sequence = 17 + static_cast < int32_t >( is_player_running );
				}
				break;

				case 981:
				{
					cur_sequence = 8;
					if ( m_prev_weapon != m_weapon )
						cur_sequence = 9;
				}
				break;

				case 980:
				{
					cur_sequence = 6;
				}
				break;

				case 986:
				{
					cur_sequence = 14;
				}
				break;


				case 988:
				{
					cur_sequence = 20;
					if ( is_player_running )
						cur_sequence = 22;

					if ( is_player_ducked )
					{
						cur_sequence = 21;
						if ( is_player_running )
							cur_sequence = 19;
					}
				}
				break;

				case 989:
				{
					cur_sequence = 23;
					if ( is_player_ducked )
						cur_sequence = 24;
				}
				break;

				case 987:
				{
					cur_sequence = 13;
				}
				break;
				default: break;
			}

			return cur_sequence;
		}


		void set_layer_seq ( valve::anim_layer_t* layer, int act );

		char					pad0[ 4 ]{};
		bool					m_first_update{};
		char					pad1[ 3 ]{};
		int						m_tick_count{};
		char					pad2[ 84 ]{};
		c_player*				m_player{};
		c_weapon*				m_weapon{};
		c_weapon*				m_prev_weapon{};
		float					m_prev_update_time{};
		int						m_prev_update_frame{};
		float					m_update_delta{};
		float					m_eye_yaw{};
		float					m_eye_pitch{};
		float					m_foot_yaw{};
		float					m_prev_foot_yaw{};
		float					m_move_yaw{};
		float					m_move_yaw_ideal{};
		float					m_move_yaw_cur_to_ideal{};
		float					m_lower_body_realign_timer{};
		float					m_feet_cycle{};
		float					m_feet_weight{};
		float                   m_move_weight_smoothed{};
		float					m_duck_amount{};
		float					m_duck_additional{};
		char					pad5[ 4u ]{};
		vec3_t					m_origin{};
		vec3_t					m_prev_origin{};
		vec3_t					m_velocity{};
		vec3_t					m_velocity_normalized{};
		vec3_t					m_velocity_normalized_non_zero{};
		float					m_speed_2d{};
		float					m_up_velocity{};
		float					m_speed_as_portion_of_run_speed{};
		float					m_speed_as_portion_of_walk_speed{};
		float					m_speed_as_portion_of_crouch_speed{};
		float					m_time_since_started_moving{};
		float					m_time_since_stopped_moving{};
		bool					m_on_ground{};
		bool					m_landing{};
		char					pad6[ 4u ]{};
		float					m_time_since_in_air{};
		float					m_left_ground_height{};
		float					m_land_anim_multiplier{};
		float					m_walk_to_run_transition{};
		bool                    m_landed_on_ground_this_frame {};
		bool                    m_left_ground_this_frame {};
		float					m_in_air_smooth_value{};
		bool					m_on_ladder{};
		float                   m_ladder_weight {};
		float                   m_ladder_speed {};
		bool                    m_walk_to_run_trans_state {};
		bool                    m_defuse_started {};
		bool                    m_plant_anim_start {};
		bool                    m_twtch_anim_started {};
		bool                    m_adjust_started {};
		char                    m_act_mods_server [ 20 ] {};
		float                   m_next_twitch_time {};
		float                   m_last_injury_time {};
		float					m_velocity_test_time{};
		vec3_t					m_prev_velocity{};
		vec3_t					m_dst_acceleration{};
		vec3_t					m_acceleration{};
		float					m_acceleration_weight{};
		float                   m_aim_matrix_trans {};
		float                   m_aim_matrix_delay {};
		bool                    m_flashed {};
		float					m_strafe_weight{};
		char					pad10[ 4u ]{};
		float					m_strafe_cycle{};
		int						m_strafe_sequence{};
		char					pad11[ 388u ]{};
		float					m_camera_shooth_height{};
		bool					m_smooth_height_valid{};
		float                   m_wtf {};
		float                   m_tf {};
		float					m_min_body_yaw{};
		float					m_max_body_yaw{};
		float					m_min_body_pitch{};
		float					m_max_body_pitch{};
		int						m_anim_set_version{};
	};

	enum struct e_life_state : std::uint8_t {
		alive,
		dying,
		dead,
		respawnable,
		discard_body
	};
	
	enum struct e_class_id : std::uint32_t {
		planted_c4 = 129u,
		precipitation = 138u,
		molotov_projectile = 114u,
		decoy_projectile = 48u,
		base_cs_grenade_projectile = 9u,
		inferno = 100u,
		smoke_grenade_projectile = 157u,
		ragdoll = 42u,
		knife = 107u,
		cascade_light = 35u,
		tonemap_controller = 69u
	};

	struct ret_counted_t {
	private:
		volatile long m_ref_count;
	public:
		virtual void destructor( char bDelete ) = 0;
		virtual bool on_final_release( ) = 0;

		void unref( ) {
			if ( InterlockedDecrement( &m_ref_count ) == 0 && on_final_release( ) )
				destructor( 1 );
		}
	};

	enum struct e_send_prop_type : std::uint32_t {
		_int, 
		_float, 
		vec3d, 
		vec2d, 
		str, 
		_array, 
		data_table, 
		int64
	};

	struct recv_prop_t;

	struct recv_table_t {
		recv_prop_t*	m_props{};
		int				m_props_count{};
		void*			m_decoder{};
		char*			m_net_table_name{};
		bool			m_initialized{};
		bool			m_in_main_list{};
	};

	struct recv_prop_t {
		char*				m_var_name{};
		e_send_prop_type	m_send_type{};
		int					m_flags{};
		int					m_str_buffer_size{};
		bool				m_in_array{};
		void*				m_extra_data{};
		recv_prop_t*		m_array_prop{};
		void*				m_array_len_proxy{};
		std::uintptr_t		m_proxy_fn{};
		void*				m_data_table_proxy_fn{};
		recv_table_t*		m_data_table{};
		std::uint32_t		m_offset{};
		int					m_element_stride{};
		int					m_elements{};
		const char*			m_parent_array_prop_name{};
	};

	struct recv_proxy_data_t {
		recv_prop_t*	m_prop{};

		struct {
			union {
				float			m_float;
				long			m_int;
				const char*		m_str;
				void*			m_data;
				vec3_t			m_vec3;
				std::int64_t	m_int64{};
			};

			int					m_type;
		}				m_value{};

		int				m_element{};
		int				m_object_id{};
	};

	enum struct e_field_type : std::uint32_t {
		_void,
		_float,
		str,
		vec3,
		vec4,
		_int,
		_bool,
		_short,
		_char,
		clr32,
		embedded,
		custom,
		classptr,
		ehandle,
		edict,
		pos_vec,
		time,
		tick,
		mdl_name,
		sound_name,
		input,
		fn,
		mat,
		vmat_worldspace,
		mat3x4_worldspace,
		interval,
		mdl_index,
		material_index,
		vec2
	};

	struct data_map_t;

	struct type_description_t {
		e_field_type			m_type{};
		const char*				m_name{};
		int						m_offset{};
		std::uint16_t			m_size{};
		std::uint16_t			m_flags{};
		const char*				m_external_name{};
		std::uintptr_t*			m_save_restore_ops{};
		std::uintptr_t			m_input_fn{};
		data_map_t*				m_data_map{};
		std::uint32_t			m_field_size_in_bytes{};
		type_description_t*		m_override_field{};
		std::uint32_t			m_override_count{};
		float					m_tolerance{};
		int						m_flat_offset[ 2u ]{};
		std::uint16_t			m_flat_group{};
	};

	struct data_map_t {
		type_description_t* m_description{};
		int                 m_size{};
		char const*			m_name{};
		data_map_t*			m_base_map{};
		bool                m_chains_validated;
		bool                m_packed_offsets_computed;
		int                 m_packed_size;
	};

	struct client_class_t {
		std::uintptr_t		m_create_fn{};
		std::uintptr_t		m_create_event_fn{};
		const char*			m_network_name{};
		recv_table_t*		m_recv_table{};
		client_class_t*		m_next{};
		e_class_id			m_class_id{};
	};

}
#pragma region MASKS

#define   DISPSURF_FLAG_SURFACE           (1<<0)
#define   DISPSURF_FLAG_WALKABLE          (1<<1)
#define   DISPSURF_FLAG_BUILDABLE         (1<<2)
#define   DISPSURF_FLAG_SURFPROP1         (1<<3)
#define   DISPSURF_FLAG_SURFPROP2         (1<<4)

#define   CONTENTS_EMPTY                0

#define   CONTENTS_SOLID                0x1       
#define   CONTENTS_WINDOW               0x2
#define   CONTENTS_AUX                  0x4
#define   CONTENTS_GRATE                0x8
#define   CONTENTS_SLIME                0x10
#define   CONTENTS_WATER                0x20
#define   CONTENTS_BLOCKLOS             0x40 
#define   CONTENTS_OPAQUE               0x80 
#define   LAST_VISIBLE_CONTENTS         CONTENTS_OPAQUE

#define   ALL_VISIBLE_CONTENTS            (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define   CONTENTS_TESTFOGVOLUME        0x100
#define   CONTENTS_UNUSED               0x200     
#define   CONTENTS_BLOCKLIGHT           0x400
#define   CONTENTS_TEAM1                0x800 
#define   CONTENTS_TEAM2                0x1000 
#define   CONTENTS_IGNORE_NODRAW_OPAQUE 0x2000
#define   CONTENTS_MOVEABLE             0x4000
#define   CONTENTS_AREAPORTAL           0x8000
#define   CONTENTS_PLAYERCLIP           0x10000
#define   CONTENTS_MONSTERCLIP          0x20000
#define   CONTENTS_CURRENT_0            0x40000
#define   CONTENTS_CURRENT_90           0x80000
#define   CONTENTS_CURRENT_180          0x100000
#define   CONTENTS_CURRENT_270          0x200000
#define   CONTENTS_CURRENT_UP           0x400000
#define   CONTENTS_CURRENT_DOWN         0x800000

#define   CONTENTS_ORIGIN               0x1000000 

#define   CONTENTS_MONSTER              0x2000000 
#define   CONTENTS_DEBRIS               0x4000000
#define   CONTENTS_DETAIL               0x8000000 
#define   CONTENTS_TRANSLUCENT          0x10000000
#define   CONTENTS_LADDER               0x20000000
#define   CONTENTS_HITBOX               0x40000000

#define   SURF_LIGHT                    0x0001 
#define   SURF_SKY2D                    0x0002 
#define   SURF_SKY                      0x0004 
#define   SURF_WARP                     0x0008 
#define   SURF_TRANS                    0x0010
#define   SURF_NOPORTAL                 0x0020 
#define   SURF_TRIGGER                  0x0040 
#define   SURF_NODRAW                   0x0080 

#define   SURF_HINT                     0x0100 

#define   SURF_SKIP                     0x0200   
#define   SURF_NOLIGHT                  0x0400   
#define   SURF_BUMPLIGHT                0x0800   
#define   SURF_NOSHADOWS                0x1000   
#define   SURF_NODECALS                 0x2000   
#define   SURF_NOPAINT                  SURF_NODECALS
#define   SURF_NOCHOP                   0x4000   
#define   SURF_HITBOX                   0x8000   

// -----------------------------------------------------
// spatial content masks - used for spatial queries (traceline,etc.)
// -----------------------------------------------------
#define   MASK_ALL                      (0xFFFFFFFF)
#define   MASK_SOLID                    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCSOLID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCFLUID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_WATER                    (CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
#define   MASK_OPAQUE                   (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
#define   MASK_OPAQUE_AND_NPCS          (MASK_OPAQUE|CONTENTS_MONSTER)
#define   MASK_BLOCKLOS                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
#define   MASK_BLOCKLOS_AND_NPCS        (MASK_BLOCKLOS|CONTENTS_MONSTER)
#define   MASK_VISIBLE                  (MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_VISIBLE_AND_NPCS         (MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_SHOT                     (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define   MASK_SHOT_BRUSHONLY           (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
#define   MASK_SHOT_HULL                (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
#define   MASK_SHOT_PORTAL              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_SOLID_BRUSHONLY          (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID_BRUSHONLY    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
#define   MASK_NPCSOLID_BRUSHONLY       (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC           (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC_FLUID     (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
#define   MASK_SPLITAREAPORTAL          (CONTENTS_WATER|CONTENTS_SLIME)
#define   MASK_CURRENT                  (CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)
#define   MASK_DEADSOLID                (CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)
#pragma endregion
#include "impl/other.inl"