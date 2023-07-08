#pragma once

namespace nem::hacks {
	struct shot_t {
		__forceinline constexpr shot_t( ) = default;

		__forceinline shot_t(
			const vec3_t& src, const aim_last_target_t* const target, const int next_shift_amount, const std::ptrdiff_t cmd_num, const float sent_time, const float latency
		) : m_src{ src }, m_target{ target ? *target : aim_last_target_t{} }, m_shot_time{ valve___g_global_vars->m_real_time },
			m_target_index { target && target->m_entry->m_player ? target->m_entry->m_player->index ( ) : -1 }, m_next_shift_amount { next_shift_amount }, 
			m_sent_time { cmd_num != -1 ? sent_time : 0.f }, m_cmd_number { cmd_num != -1 ? cmd_num : -1 }, m_latency { cmd_num != -1 ? latency : 0.f } {}

		vec3_t							m_src{};

		aim_last_target_t				m_target{};

        float							m_shot_time{},
                                        m_sent_time{}, m_latency{};
		bool							m_processed{};
		int								m_dmg{},
										m_target_index{},
										m_next_shift_amount{},
										m_cmd_number{ -1 }, m_process_tick{};

		struct {
			vec3_t	m_impact_pos{};
			int		m_fire_tick{}, m_hurt_tick{}, m_hitgroup{}, m_dmg{};
		}								m_server_info{};
	};

	class c_shots : valve::base_event_listener_t {
	private:
		std::deque< shot_t > m_elements{};
	public:
		void on_net_update( );

		void fire_game_event( valve::game_event_t* const event );

		void register_self ( );

		__forceinline std::deque< shot_t >& elements( );

		__forceinline shot_t* last_unprocessed( );

		__forceinline void add(
			const vec3_t& src, const aim_last_target_t* const target, const int next_shift_amount, const std::ptrdiff_t cmd_num, const float sent_time, const float latency
		);
	};

	inline const auto g_shots = std::make_unique< c_shots >( );

	struct log_data_t {
		float m_creation_time = 0.f;
		float m_text_alpha = 0.0f;
		float m_back_alpha = 50.0f;
		float m_spacing = 0.0f;

		std::string m_string = "";
		std::string m_icon = "";

		col_t m_color;

		bool m_printed = false;
	};

	class c_logs {
	public:
		void draw_data ( );
		void push_log ( std::string log, std::string icon, col_t color );

	private:
		std::deque < log_data_t > m_logs;
	};

	inline std::unique_ptr < c_logs > g_logs = std::make_unique < c_logs > ( );
}

#include "impl/shots.inl"