#pragma once
#include "../context/context.hpp"
namespace nem {
	struct lua_user_cmd_t {
		int		m_choked_cmds{}, m_cmd_number{}, m_tick_count {};
		float	m_pitch{}, m_yaw{}, m_fwd_move{}, m_side_move{}, m_move_yaw{};
		bool	m_allow_send_packet{}, m_in_attack{}, m_in_jump{}, m_in_duck{},
				m_in_forward{}, m_in_back{}, m_in_use{}, m_in_cancel{}, m_in_left{},
				m_in_right{}, m_in_move_left{}, m_in_move_right{}, m_in_attack2{}, m_in_run{},
				m_in_reload{}, m_in_alt1{}, m_in_alt2{}, m_in_score{}, m_in_speed{}, m_in_walk{}, m_in_zoom{},
				m_in_weapon1{}, m_in_weapon2{}, m_in_bullrush{}, m_in_grenade1{}, m_in_grenade2{},
				m_in_attack3{}, m_weaponselect{}, m_weaponsubtype{};
	};

	class c_lua {
	private:
		struct callback_t {
			__forceinline callback_t( ) = default;

			__forceinline callback_t(
				const sol::protected_function& fn,
				const std::size_t owner_hash
			) : m_fn{ fn }, m_owner_hash{ owner_hash } {}

			sol::protected_function m_fn{};
			std::size_t				m_owner_hash{};
		};

		using events_t = std::unordered_map< std::string, std::deque< callback_t > >;

		sol::state					m_state{};
		events_t					m_events{};
		std::vector< std::string >	m_scripts{};
	public:
		void init( );

		__forceinline events_t& events( );

		__forceinline void load( const std::string& name );

		__forceinline void unload( const std::string& name );

		__forceinline void unload_all ( );

		__forceinline bool is_loaded( const std::string& name );
	};

	inline const auto g_lua = std::make_unique< c_lua >( );
}

#include "impl/lua.inl"