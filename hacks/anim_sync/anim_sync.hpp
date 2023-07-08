#pragma once

namespace nem::hacks {
	class c_anim_sync {
	private:
		void update(
			player_entry_t& entry, cc_def ( lag_record_t* ) current, cc_def ( lag_anim_data_t* ) previous, const int side
		) const;

		void interp_anim_data ( player_entry_t& entry, cc_def ( lag_record_t* ) current, cc_def ( lag_anim_data_t* ) previous ) const;

		void find_server_foot_yaw(
			player_entry_t& entry,
			lag_record_t* const current, const lag_anim_data_t* const previous
		) const;

		void select_anim_side (
			player_entry_t& entry,
			lag_record_t* const current, const lag_anim_data_t* const previous
		) const;

		void interp_air_data (
			player_entry_t& entry,
			cc_def ( lag_record_t* ) current, cc_def ( lag_anim_data_t* ) previous
		) const;

		void process_air_interp (
			player_entry_t& entry,
			lag_record_t* const current, const lag_anim_data_t* const previous, const int anim_tick
		) const;

		struct local_data_t {
			valve::bones_t		m_shoot_pos_bones {};

			qangle_t            m_choke_shot_ang {};
			bool                m_did_shot_in_choke_cycle {};
			bool				m_shot_valid_wpn {}, m_can_anim_event {};
			int					m_shot_cmd_number {};
			float               m_lower_body_yaw {};
			qangle_t            m_shot_angle {};
			valve::anim_state_t		m_anim_state {};
			float               m_lower_body_relign_timer {};
			bool                m_did_shot_at_this_cycle {};

			valve::anim_layers_t	m_fake_anim_layers {};
			valve::pose_params_t	m_fake_pose_parameters {};
			valve::bones_t	        m_fake_bones {};
			valve::anim_state_t     m_fake_anim_state {};
			std::array < vec3_t, 128 > m_fake_bone_origins {};
			std::array < vec3_t, 128 > m_bone_origins {};
			float                   m_spawn_time {};
			valve::e_move_type      m_move_type [ 2 ] {};
			valve::e_ent_flags      m_flags [ 2 ] {};
			qangle_t                m_real_angle {};
			qangle_t                m_fake_angle {};

			valve::anim_layers_t	m_anim_layers {};
			valve::pose_params_t	m_pose_parameters {};
			valve::bones_t	        m_bones {};
			float m_last_shot_time{};
			float m_max_body_yaw{}, m_min_body_yaw{ };

			struct {
				valve::bones_t	m_bones{};
			}					m_real{};

			struct {
				valve::bones_t			m_bones{};

				float					m_spawn_time{}, m_abs_yaw{};

				valve::anim_state_t		m_anim_state{};

				valve::anim_layers_t	m_anim_layers{};
				valve::pose_params_t	m_pose_params{};
			}					m_fake{};
			std::array< valve::anim_layer_t, 13 > get_anim_layers ( )
			{
				std::array< valve::anim_layer_t, 13 > out_put;

				std::memcpy ( out_put.data ( ), valve::g_local_player->anim_layers ( ).data ( ), sizeof ( valve::anim_layer_t ) * 13 );
				std::memcpy ( &out_put.at ( 4 ), &m_anim_layers.at ( 4 ), sizeof ( valve::anim_layer_t ) );
				std::memcpy ( &out_put.at ( 5 ), &m_anim_layers.at ( 5 ), sizeof ( valve::anim_layer_t ) );
				std::memcpy ( &out_put.at ( 6 ), &m_anim_layers.at ( 6 ), sizeof ( valve::anim_layer_t ) );
				std::memcpy ( &out_put.at ( 7 ), &m_anim_layers.at ( 7 ), sizeof ( valve::anim_layer_t ) );
				std::memcpy ( &out_put.at ( 11 ), &m_anim_layers.at ( 11 ), sizeof ( valve::anim_layer_t ) );
				std::memcpy ( &out_put.at ( 12 ), &m_anim_layers.at ( 12 ), sizeof ( valve::anim_layer_t ) );

				return out_put;
			}

			std::array< valve::anim_layer_t, 13 > get_fake_anim_layers ( )
			{
				std::array< valve::anim_layer_t, 13 > out_put;

				std::memcpy ( out_put.data ( ), valve::g_local_player->anim_layers ( ).data ( ), sizeof ( valve::anim_layer_t ) * 13 );
				std::memcpy ( &out_put.at ( 4 ), &m_fake_anim_layers.at ( 4 ), sizeof ( valve::anim_layer_t ) );
				std::memcpy ( &out_put.at ( 5 ), &m_fake_anim_layers.at ( 5 ), sizeof ( valve::anim_layer_t ) );
				std::memcpy ( &out_put.at ( 6 ), &m_fake_anim_layers.at ( 6 ), sizeof ( valve::anim_layer_t ) );
				std::memcpy ( &out_put.at ( 7 ), &m_fake_anim_layers.at ( 7 ), sizeof ( valve::anim_layer_t ) );
				std::memcpy ( &out_put.at ( 11 ), &m_fake_anim_layers.at ( 11 ), sizeof ( valve::anim_layer_t ) );
				std::memcpy ( &out_put.at ( 12 ), &m_fake_anim_layers.at ( 12 ), sizeof ( valve::anim_layer_t ) );

				return out_put;
			}

			__forceinline void reset ( ) {
				m_bones = {};
				m_fake_bones = {};
				m_shot_angle = {};
				m_real_angle = {};
				m_fake_angle = {};
				m_anim_layers = {};
				m_fake_anim_layers = {};
				m_pose_parameters = {};
				m_fake_pose_parameters = {};
				m_bone_origins = {};
				m_fake_bone_origins = {};
				m_did_shot_at_this_cycle = false;
				m_did_shot_in_choke_cycle = false;
				m_lower_body_relign_timer = 0.f;
				m_lower_body_yaw = 0.f;
				m_spawn_time = 0.f;
				m_move_type [ 0 ] = m_move_type [ 1 ] = {};
				m_flags [ 0 ] = m_flags [ 1 ] = {};
			}

		} m_local_data{};
	public:
		bool setup_bones(
			valve::c_player* const player,
			valve::bones_t& bones, const float time, const int flags
		) const;

		void do_animation_event ( const std::ptrdiff_t const type, const c_eng_pred::local_data_t& local_data );

		void setup_enemy_bones ( valve::c_player* const player,
			valve::bones_t& bones, const float time ) const;

		void setup_local_bones( );

		void on_net_update(
			player_entry_t& entry,
			cc_def ( lag_record_t* ) current, cc_def ( lag_anim_data_t* ) previous
		) const;

		__forceinline local_data_t& local_data( );

		void update_local_real( valve::user_cmd_t& user_cmd, bool send_packet );
		void network_prediction_process( valve::user_cmd_t& user_cmd );
		void update_local( const qangle_t& view_angles, const bool no_view_model, bool send_packet );
	};

	inline const auto g_anim_sync = std::make_unique< c_anim_sync >( );

}

#include "impl/anim_sync.inl"