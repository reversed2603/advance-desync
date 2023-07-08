#pragma once

namespace nem::hacks {
	class c_eng_pred {
	private:
		struct {
			float m_cur_time{}, m_frame_time{};
		}								m_backup{};
		struct {
			int m_view_model_index, m_animation_parity;
			float m_view_cycle, m_anim_time;
			int m_view_sequence;

		} m_view_model {};

		struct net_vars_t {
			__forceinline void restore( const int cmd_number ) const;

			__forceinline void store( const int cmd_number );

			int			m_cmd_number{};

			float		m_view_offset_z{}, m_velocity_modifier {}, m_fall_velocity {}, m_third_person_recoil {};

			qangle_t	m_aim_punch{}, m_aim_punch_vel{}, m_view_punch {};
			vec3_t      m_velocity {}, m_origin {};
		};
	public:
		struct local_data_t {
			__forceinline void init( const valve::user_cmd_t& user_cmd );

			__forceinline void save( const int cmd_number );

			__forceinline void store_anim( );

			__forceinline void restore_anim( const bool layers ) const;

			float					m_spawn_time{}, m_abs_yaw{};

			int						m_tick_base{},
									m_adjusted_tick_base{}, m_shift_amount{};
			bool					m_override_tick_base{}, m_restore_tick_base{};

			vec3_t					m_move{};
			valve::user_cmd_t		m_user_cmd {};
			qangle_t                m_choke_angle {};
			valve::anim_state_t		m_anim_state{};

			valve::anim_layers_t	m_anim_layers{};
			valve::pose_params_t	m_pose_params{};

			struct {
				qangle_t			m_eye_angles{},
									m_aim_punch{}, m_view_punch{};

				vec3_t				m_origin{}, m_abs_origin{},
									m_obb_min{}, m_obb_max{},
									m_velocity{}, m_view_offset{},
									m_aim_punch_vel{}, m_abs_velocity{}, m_base_velocity {};

				valve::e_ent_handle     m_ground_entity {};

				bool				m_strafing{}, m_scoped{}, m_walking{};

				int					m_move_state{}, m_tick_base{};

				float				m_duck_amount{},m_duck_speed{},
									m_third_person_recoil{}, m_lby{}, m_fall_velocity {}, m_surface_friction {}, m_velocity_modifier {};

				valve::e_ent_flags	m_flags{};
				valve::e_move_type	m_move_type{};
			}						m_net_vars{}, m_pred_net_vars{};
		};

		valve::e_ent_flags                  m_flags {};

	private:
		valve::user_cmd_t					m_old_user_cmd{}, m_last_user_cmd{};

		bool								m_r8_can_shoot{};
		float								m_spread{}, m_inaccuracy{},
											m_min_inaccuracy{},
											m_postpone_fire_ready_time{},
											m_recoil_index{}, m_accuracy_penalty{},
											m_velocity_modifier{}, m_net_velocity_modifier{ 1.f };

		bool                                m_is_out_of_epsilon {};

		std::array< net_vars_t, 150u >		m_net_vars{};
		std::array< local_data_t, 150u >	m_local_data{};

		valve::move_data_t					m_move_data{};
		valve::e_frame_stage				m_last_frame_stage{};

		vec3_t                              m_velocity {}, m_origin {}, m_aim_punch_vel {}, m_view_punch {}, m_aim_punch {}, m_view_offset {}, m_base_velocity {};
		int                                 m_tick_base {};
		valve::e_move_type                  m_move_type {};
		float                               m_duck_amt {}, m_duck_speed {}, m_fall_velocity {}, m_velocity_modifier_data {};
		float                               m_processed_cmd {};

	public:
		void prepare( );

		void predict( cc_def ( valve::user_cmd_t* ) user_cmd, bool init_data );

		void process ( valve::user_cmd_t* const user_cmd, bool init_data );

		void restore( );

		void on_packet_update ( std::uintptr_t cl_state );

		__forceinline void recompute_velocity_modifier ( const std::ptrdiff_t cmd_num, const bool predicted ) const;

		void update_shoot_pos( const valve::user_cmd_t& user_cmd ) const;

		__forceinline valve::e_frame_stage& last_frame_stage( );

		__forceinline std::array< net_vars_t, 150u >& net_vars( );

		__forceinline std::array< local_data_t, 150u >& local_data( );

		__forceinline void reset_on_spawn( );

		__forceinline valve::user_cmd_t& old_user_cmd( );

		__forceinline valve::user_cmd_t& last_user_cmd( );

		__forceinline float& spread( );

		__forceinline bool& is_out_of_epsilon ( );

		__forceinline float& inaccuracy ( );

		__forceinline void velocity_modifier_to_data_map ( );

		__forceinline void third_person_recoil_to_data_map ( );

		__forceinline float& min_inaccuracy( );

		__forceinline void store_data_map ( );
		
		__forceinline void restore_data_map ( );

		__forceinline float& recoil_index( );

		__forceinline void save_view_model ( );

		__forceinline void adjust_view_model ( );

		__forceinline float& accuracy_penalty( );

		__forceinline float& velocity_modifier( );

		__forceinline float& net_velocity_modifier( );

		__forceinline bool& r8_can_shoot( );

		__forceinline float& postpone_fire_ready_time( );
	};

	inline const auto g_eng_pred = std::make_unique< c_eng_pred >( );
}

#include "impl/eng_pred.inl"