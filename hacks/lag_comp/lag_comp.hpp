#pragma once
enum e_advance_act : int {
	act_none = 0,
	act_jump,
	act_fall,
	act_land,
	act_adjust_stopped_moving,
	act_balance_adjust
};

enum e_anim_state_poses {
	ACT_DIE_STAND = 953,
	ACT_DIE_STAND_HEADSHOT,
	ACT_DIE_CROUCH,
	ACT_DIE_CROUCH_HEADSHOT,
	ACT_NULL,
	ACT_DEFUSE,
	ACT_DEFUSE_WITH_KIT,
	ACT_FLASHBANG_REACTION,
	ACT_FIRE_PRIMARY,
	ACT_FIRE_PRIMARY_OPT_1,
	ACT_FIRE_PRIMARY_OPT_2,
	ACT_FIRE_SECONDARY,
	ACT_FIRE_SECONDARY_OPT_1,
	ACT_FIRE_SECONDARY_OPT_2,
	ACT_RELOAD,
	ACT_RELOAD_START,
	ACT_RELOAD_LOOP,
	ACT_RELOAD_END,
	ACT_OPERATE,
	ACT_DEPLOY,
	ACT_CATCH,
	ACT_SILENCER_DETACH,
	ACT_SILENCER_ATTACH,
	ACT_TWITCH,
	ACT_TWITCH_BUYZONE,
	ACT_PLANT_BOMB,
	ACT_IDLE_ADJUST_TURN,
	ACT_IDLE_ADJUST_STOPPEDMOVING,
	ACT_ALIVE_LOOP,
	ACT_FLINCH,
	ACT_FLINCH_HEAD,
	ACT_FLINCH_MOLOTOV,
	ACT_JUMP,
	ACT_FALL,
	ACT_CLIMB_LADDER,
	ACT_LAND_LIGHT,
	ACT_LAND_HEAVY,
	ACT_EXIT_LADDER_TOP,
	ACT_EXIT_LADDER_BOTTOM
};

enum e_r_side 
{
	l_side = 1, // -58
	r_side, // +58
	c_side // ?
};

enum e_r_mode : int
{
	r_anim_logic,
	r_at_target_logic,
	r_prev_state_logic,
	r_auto_wall_logic
};

namespace nem::hacks {
	struct lag_backup_t {
		__forceinline constexpr lag_backup_t ( ) = default;

		__forceinline lag_backup_t ( valve::c_player* const player );

		__forceinline void restore ( valve::c_player* const player ) const;

		float			m_foot_yaw {}, m_sim_time {}, m_obb_change_time {}, m_obb_change_height {};
		vec3_t			m_origin {}, m_abs_origin {},
			m_obb_min {}, m_obb_max {};

		valve::e_ent_flags m_flags {};

		valve::bones_t	m_bones {};
		int				m_bones_count {},
			m_readable_bones {}, m_writable_bones {};

		unsigned long	m_mdl_bone_counter {};

	};

	struct lag_anim_data_t;
	struct player_entry_t;
	struct lag_record_t {
		struct anim_side_t {
			valve::bones_t			m_bones{};
			std::size_t				m_bones_count{};

			float					m_foot_yaw{}, m_prev_foot_yaw{},
				m_move_yaw_ideal{}, m_move_yaw_cur_to_ideal{},
				m_move_yaw{}, m_move_weight_smoothed{}, m_air_smooth_value{}, m_time_to_realign_body{};

			valve::anim_layers_t	m_anim_layers{};
			valve::pose_params_t    m_pose_params{};
		};

		__forceinline constexpr lag_record_t( ) = default;

		__forceinline lag_record_t(
			valve::c_player* const player
		) : m_dormant{ player->dormant( ) },
			m_flags{ player->flags( ) },
			m_sim_ticks{ valve::to_ticks( player->sim_time( ) - player->old_sim_time( ) ) },
			m_total_cmds{ m_sim_ticks }, m_receive_tick{ valve___g_client_state->m_server_tick },
			m_sim_time{ player->sim_time( ) }, m_old_sim_time{ player->old_sim_time( ) },
			m_lby{ player->lby( ) }, m_duck_amount{ player->duck_amount( ) },
			m_weapon{ player->weapon( ) }, m_third_person_recoil{ player->third_person_recoil( ) },
			m_move_type{ player->move_type( ) },
			m_walking{ player->walking( ) }, m_view_offset{ player->view_offset( ) },
			m_last_shot_time{ m_weapon ? m_weapon->last_shot_time( ) : std::numeric_limits< float >::max( ) },
			m_eye_angles{ player->eye_angles( ) },
			m_time_diff{ player->sim_time( ) - player->old_sim_time( ) },
			m_velocity{ player->velocity( ) }, m_origin{ player->origin( ) },
			m_obb_min{ player->obb_min( ) }, m_obb_max{ player->obb_max( ) },
			m_sim_time_delay{ valve___g_client_state->m_server_tick - valve::to_ticks( player->sim_time( ) ) },
			m_lc_exploit{ m_sim_time_delay >= 64 }, // what the fuck
			m_anim_layers{ player->anim_layers( ) } {}

		__forceinline void restore( valve::c_player* const player, const int anim_index, const bool only_anim = false ) const;

		__forceinline void correct( cc_def( lag_anim_data_t* ) previous, const lag_anim_data_t* const pre_previous, player_entry_t& entry );

		__forceinline bool valid( ) const;

		bool							m_dormant{}, m_trying_to_resolve{},
			m_broke_lc{}, m_accelerating{}, m_too_much_diff{}, m_lc_exploit{}, m_extrapolated{}, m_shot{}, m_failed_sim{}, m_invalid_sim{};
		float                           m_null_play_back_delta{}, m_first_play_back_delta{}, m_second_play_back_delta{};
		float                           m_null_weight_delta{}, m_first_weight_delta{}, m_second_weight_delta{};
		valve::e_ent_flags				m_flags{}, m_interp_flags{};
		int								m_sim_ticks{}, m_total_cmds{}, m_anim_side{}, m_priority{}, m_receive_tick{};
		float							m_sim_time{}, m_old_sim_time{}, m_lby{}, m_duck_amount{}, m_on_ground_time{}, m_third_person_recoil{}, m_anim_time{}, m_air_dur{};

		bool                            m_reforce_safe_points{}, m_force_safe_points{}, m_solve_via_anim{};
		int                            m_sim_time_delay{}, m_in_velocity_sync_process{};
		valve::e_move_type              m_move_type{ };

		bool                            m_walking{ }, m_anim_updated{}, m_max_foot_yaw_sync{}, m_is_jittering{}, m_should_process_record{ true };
		float                           m_move_yaw{};
		e_r_mode                        m_r_mode{};

		std::optional < bool >          m_on_ground{};
		float                           m_act_time{};


		vec3_t                          m_eye_ang_delta{}, m_velocity_delta{}, m_view_offset_delta{};

		float                           m_duck_amt_delta{}, m_lby_delta{};

		valve::c_weapon* m_weapon{};
		float							m_last_shot_time{}, m_max_delta{};

		qangle_t						m_eye_angles{};
		vec3_t							m_velocity{}, m_origin{}, m_obb_min{}, m_obb_max{};
		bool                            m_entry_after_dormant{}, m_invalid{};
		float                           m_rot_multipl{};

		float                           m_time_diff{ };
		vec3_t                          m_view_offset{};

		valve::anim_layers_t			m_anim_layers{};
		bool                            m_dir_changed{};
		float                           m_last_vel_imp{};

		std::array< anim_side_t, 3u >	m_anim_sides{};
		bool                            m_safe_head{};
	};

	struct lag_anim_data_t {
		__forceinline constexpr lag_anim_data_t( ) = default;

		__forceinline lag_anim_data_t( const lag_record_t* const lag_record );

		__forceinline lag_anim_data_t(const lag_anim_data_t* const lag_record);

		struct anim_side_t {
			float	m_foot_yaw{}, m_prev_foot_yaw{},
					m_move_yaw_ideal{}, m_move_yaw_cur_to_ideal{},
					m_move_yaw{}, m_move_weight_smoothed {}, m_air_smooth_value {}, m_time_to_realign_body {};
		};

		valve::c_weapon*				m_weapon{};

		qangle_t						m_eye_angles{};
		vec3_t							m_origin{}, m_velocity{}, m_view_offset {};

		valve::e_ent_flags				m_flags{};
		float							m_lby{}, m_duck_amount{}, m_third_person_recoil{}, m_lby_delta {};

		bool                            m_dormant {};

		valve::anim_layers_t			m_anim_layers{};
		float                           m_sim_time {};

		std::array< anim_side_t, 3u >	m_anim_sides{};
	};

	struct player_entry_t {

		__forceinline void reset ( );

		valve::c_player* m_player {};

		float											m_spawn_time {},
			m_receive_time {},
			m_alive_loop_cycle{}, m_alive_loop_rate{}, m_cur_sim{}, m_old_sim{}, m_last_sim{ }, m_broke_sim{ };

		int                                             m_records_count {};

		valve::bones_t									m_bones {};
		std::array < vec3_t, 128 >                      m_bone_origins {};

		bool                                            m_is_obb_changed {};

		bool											m_unk {}, m_try_anim_resolver { true };
		std::array < int, 4 > 					     	m_prev_anim_side {};

		int                                             m_anim_misses {};
		int                                             m_stand_misses {};
		int                                             m_auto_wall_misses {};
		int                                             m_at_target_misses {};
		vec3_t                                          m_render_origin {};
		bool                                            m_can_disable_interp {};                                         

		std::ptrdiff_t                                  m_freestand_side {}, m_prev_freestand_side {};

		std::optional< lag_anim_data_t >				m_prev_anim_data{};
		std::optional< lag_anim_data_t >				m_pre_prev_anim_data{};

		std::deque< std::shared_ptr< lag_record_t > >	m_lag_records{};
		vec3_t                                          m_mins{}, m_maxs{};
		bool                                            m_fix_sim{};
	};

	class c_lag_comp {
	private:
		std::array< player_entry_t, 64u > m_entries{};
	public:
		void on_net_update( );

		__forceinline float calc_time_delta( const float sim_time ) const;

		__forceinline player_entry_t& entry( const std::size_t i );
	};

	inline const auto g_lag_comp = std::make_unique< c_lag_comp >( );
}

#include "impl/lag_comp.inl"