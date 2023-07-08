#pragma once

namespace nem {
	struct code_section_t {
		__forceinline constexpr code_section_t( ) = default;

		__forceinline constexpr code_section_t(
			const std::size_t size, const std::uintptr_t base
		) : m_size{ size }, m_base{ base } {}

		__forceinline code_section_t( const HMODULE module ) {
			const auto dos_header = reinterpret_cast< PIMAGE_DOS_HEADER >( module );
			if ( dos_header->e_magic != IMAGE_DOS_SIGNATURE )
				return;

			const auto nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS >(
				reinterpret_cast< std::uintptr_t >( module ) + dos_header->e_lfanew
			);
			if ( nt_headers->Signature != IMAGE_NT_SIGNATURE )
				return;

			m_size = nt_headers->OptionalHeader.SizeOfCode;
			m_base = reinterpret_cast< std::uintptr_t >( module ) + nt_headers->OptionalHeader.BaseOfCode;
		}

		std::size_t		m_size{};
		std::uintptr_t	m_base{};
	};

	namespace valve {
		struct cvar_t;
		
		struct user_cmd_t;

		class c_player;

		class c_weapon;

		struct weapon_data_t;
	}

	enum e_context_flags : std::uint8_t {
		can_choke = 1u << 0u,
		choke = 1u << 1u,
		can_shoot = 1u << 2u,
		recharge = 1u << 3u,
		aim_fire = 1u << 4u,
		has_target = 1u << 5u
	};
	DEFINE_ENUM_FLAG_OPERATORS( e_context_flags )

	template<typename T>
	class crypt_ptr {
	public:

		BYTE* m_rot_value = nullptr;
		DWORD* m_crypted = nullptr;
		DWORD* m_xor_value = nullptr;

		__forceinline crypt_ptr( )
		{
			m_rot_value = nullptr;
			m_crypted = nullptr;
			m_xor_value = nullptr;
		}

		__forceinline crypt_ptr( T ptr ) 
		{
			if ( !m_rot_value )
				m_rot_value = new BYTE;

			*m_rot_value = ( ( int ) m_rot_value ) % 16;

			if (!m_xor_value )
				m_xor_value = new DWORD;

			*m_xor_value = _rotl( ( int ) m_xor_value, 13 ) ^ 0x8BF150FF;

			if (!m_crypted )
				m_crypted = new DWORD;

			*m_crypted = _rotr( ( DWORD ) ptr ^ *m_xor_value, *m_rot_value );
		}

		__forceinline  T get( ) const 
		{
			return ( T ) ( _rotl( *m_crypted, *m_rot_value ) ^ *m_xor_value );
		}

		__forceinline  T operator-> ( ) 
		{
			return get( );
		}
	};


	template<typename T>
	class crypt_ptr_safe {
	public:

		BYTE m_rot_value;
		DWORD m_crypted;
		DWORD m_xor_value;

		__forceinline crypt_ptr_safe()
		{
			m_rot_value = 0;
			m_crypted = 0;
			m_xor_value = 0;
		}

		__forceinline crypt_ptr_safe( T ptr )
		{
			m_rot_value = ( ( int ) &m_rot_value ) % 16;
			m_xor_value = _rotl( ( int ) &m_xor_value, 13 ) ^ 0x8BF150FF;
			m_crypted = _rotr( ( DWORD ) ptr ^ m_xor_value, m_rot_value );
		}

		__forceinline  T get() const
		{
			return ( T ) ( _rotl( m_crypted, m_rot_value ) ^ m_xor_value );
		}

		__forceinline  T operator-> ()
		{
			return get();
		}
	};

	class c_context {
	private:
		struct addresses_t {
			using line_goes_thru_smoke_t = bool( __cdecl* )( vec3_t, vec3_t );

			using random_float_t = float( __cdecl* )( const float, const float );

			using random_int_t = int( __cdecl* )( const int, const int );

			using random_seed_t = void( __cdecl* )( const int );

			using sky_box_set_fn = void(__fastcall*) (const char*);

			using angle_matrix_t = void( __fastcall* )( const qangle_t&, mat3x4_t& );

			using cl_read_packets_t = void ( __cdecl* ) ( bool final_tick );

			//using who_asked_fn = void(__thiscall*)(DWORD);

			crypt_ptr < random_float_t >		m_random_float{};
			crypt_ptr < random_int_t >		m_random_int{};
			crypt_ptr < random_seed_t >		m_random_seed{};

			crypt_ptr < cl_read_packets_t >   m_cl_read_packets {};

			crypt_ptr < std::uintptr_t >		m_reset_anim_state{}, m_update_anim_state{},
				m_set_abs_angles{}, m_set_abs_origin{},
				m_lookup_seq_act {}, m_breakable {}, m_upd_collision_bounds { }, m_user_cmd_calc_checksum {},
				m_set_collision_bounds{}, m_invalidate_bone_cache{}, m_lookup_bone{}, m_bone_counter {},
				m_trace_filter_simple_vtable{}, m_trace_filter_skip_two_entities_vtable{}, m_compute_hitbox_surround_box {},
				m_write_user_cmd{}, m_weapon_system{}, m_calc_shotgun_spread{},
				m_key_values_init{}, m_key_values_load_from_buffer{}, m_cam_sv_cheats_ret{},
				m_add_notify{}, m_ret_to_scope_arc{}, m_ret_to_scope_lens{}, m_set_clan_tag{},
				m_start_drawing{}, m_finish_drawing{}, m_add_clred_text{}, m_calc_text_size{},
				m_set_font_glyph_set{}, m_has_c4{}, m_ret_insert_into_tree{}, m_smoke_count{}, m_attachment_helper {},
				m_disable_post_processing{}, m_crosshair_ret{}, m_ret_to_scope_clear{}, m_ret_to_scope_blurry{},
				m_ret_to_eye_pos_and_vectors{}, m_ret_to_maintain_seq_transitions{}, m_ret_to_extrapolation{}, m_item_system{},
				m_hud{}, m_find_hud_element{}, m_clear_hud_wpn_icon{}, m_ret_to_accumulate_layers{}, m_ret_to_setup_velocity{},
				m_ayo_wtf { }, m_clear_hud_wpn {}, m_allow_to_extrapolate {}, m_view_matrix {}, m_clamp_bones_in_box {}, m_invalidate_physics_recursive {};

			//who_asked_fn        m_ayo_who_asked{ };

			crypt_ptr < sky_box_set_fn >      m_sky_box{ };


;			crypt_ptr < int* >				m_pred_seed{};
			crypt_ptr < valve::c_player* >	m_pred_player{};

			crypt_ptr < angle_matrix_t >		m_angle_matrix{};
			
			crypt_ptr < line_goes_thru_smoke_t > m_line_goes_thru_smoke{};
		}						m_addresses{};

		struct cvars_t {
			valve::cvar_t*	m_r_jiggle_bones{}, *m_cl_interp{},
							*m_cl_interp_ratio{}, *m_cl_updaterate{},
							*m_ff_dmg_reduction_bullets, *m_ff_dmg_bullet_pen{},
							*m_sv_auto_bhop{}, *m_sv_clockcorrection_msecs{},
							*m_sv_maxvelocity{}, *m_sv_friction{}, *m_sv_stopspeed {},
							*m_sv_accelerate{}, *m_sv_accelerate_use_weapon_speed{},
							*m_weapon_accuracy_nospread{}, *m_sv_maxunlag{},
							*m_sv_enable_bhop{}, *m_sv_jump_impulse{}, *m_sv_gravity{},
							*m_weapon_recoil_scale{}, *m_weapon_accuracy_shotgun_spread_patterns{},
							*m_molotov_throw_detonate_time{}, *m_weapon_molotov_maxdetonateslope{},
							*m_cl_fullupdate{};
		}						m_cvars{};

		struct net_info_t {
			float	m_lerp{};

			struct {
				float m_in{}, m_out{};
			}		m_latency{};
		}						m_net_info{};

		struct s_auto_peek_data
		{
			vec3_t m_start_pos;
			bool m_is_firing;
			bool m_old_shot;

		}m_apeek_data{ };

		bool m_send_packet {};

		bool m_defensive_allowed { true };
		
		struct s_kill_feed_data
		{
			bool m_should_reset_killfeed;
		} m_kill_feed_data;

		vec3_t					m_shoot_pos{}, m_last_sent_origin{}, m_aim_bot_shoot_pos { }, m_c4_explode_origin {};
		int						m_last_sent_cmd_number{}, m_last_cmd_number{};
		bool					m_freeze_time{}, m_allow_anim_update{},
								m_allow_setup_bones{}, m_force_bone_mask{}, m_bone_available {},
			                    m_should_reset{ }, m_broke_lc{}, m_is_out_of_epsilon { }, m_left_create_move {}, m_c4_planted {};
		float                   m_c4_explode_time {}, m_c4_dmg {}, m_c4_radius {}, m_c4_radius_scaled {};

		std::vector< int >		m_sented_cmds{};

		std::mutex              m_mtx;

		int                     m_should_buy {};

		e_context_flags			m_flags{};

		valve::c_weapon*		m_weapon{};
		valve::weapon_data_t*	m_wpn_data{};

	public:
		struct networked_data_t {
			int m_cmd_num {};
		};

		struct out_cmds_data_t
		{
			int m_command_nr;
			int m_prev_command_nr;

			bool m_is_outgoing;
			bool m_is_used;
		} m_out_cmds;

		std::deque < out_cmds_data_t > m_out_cmds_data {};
		std::deque < networked_data_t > m_net_correction {};

		void init( );

		bool will_shoot( valve::c_weapon* const weapon, const valve::user_cmd_t& user_cmd ) const;

		bool can_shoot(
			valve::c_weapon* const weapon, const valve::user_cmd_t& user_cmd,
			bool r8_ret, const int shift_amount, const bool normal
		) const;

		void add_packet( const int cmd_number );

		__forceinline addresses_t& addresses( );

		__forceinline vec3_t& c4_explode_origin ( );

		__forceinline float& c4_radius_scaled ( );

		__forceinline float& c4_radius ( );

		__forceinline float& c4_dmg ( );

		__forceinline cvars_t& cvars( );

		__forceinline net_info_t& net_info( );

		__forceinline bool& defensive_allowed ( );

		__forceinline s_auto_peek_data& get_auto_peek_info();

		__forceinline vec3_t& shoot_pos( );

		__forceinline bool& left_create_move ( );

		__forceinline int& last_sent_cmd_number( );

		__forceinline int& last_cmd_number( );

		__forceinline bool& allow_anim_update( );

		__forceinline bool& is_bone_available ( );

		__forceinline std::mutex& get_mutex ( );

		__forceinline s_kill_feed_data& get_kill_feed_data();

		__forceinline std::deque<out_cmds_data_t>& get_out_cmds ( );

		__forceinline float& c4_explode_time ( );

		__forceinline std::deque < networked_data_t >& net_correction ( );

		__forceinline int& should_buy ( );

		__forceinline bool& send_packet ( );

		__forceinline bool& allow_setup_bones( );

		__forceinline bool& force_bone_mask( );

		__forceinline std::vector< int >& sented_cmds( );

		__forceinline e_context_flags& flags( );

		__forceinline valve::c_weapon*& weapon( );

		__forceinline valve::weapon_data_t*& wpn_data( );

		__forceinline bool& freeze_time( );

		__forceinline bool& is_c4_planted ( );

		__forceinline bool& should_reset_k_feed();

		__forceinline bool& broke_lc( );

		__forceinline vec3_t& last_sent_origin( );

		__forceinline bool& is_out_of_epsilon ( );

		__forceinline vec3_t& aim_bot_shoot_pos ( );
	};

	inline const auto g_context = std::make_unique< c_context >( );
}
struct s_keybind {
	s_keybind ( )
	{
		m_code = 0;
		m_mode = 0;
		m_is_binding = false;
	}

	s_keybind ( char code, int mode )
	{
		m_code = code;
		m_mode = mode;
		m_is_binding = false;
	}

	char m_code;
	int m_mode;
	bool m_is_binding;
};

class c_key_binds {
public:
	//bool get_keybind_state ( const s_keybind* bind );
	//void KeybindNelfo ( const char* label, s_keybind* keybind );

public:
	char m_last_code = NULL;
};

inline std::unique_ptr < c_key_binds > g_key_binds = std::make_unique < c_key_binds > ( );

class c_dlights
{
public:
	void alloc_dlight ( col_t col, float exponent, float radius, float decay, int key, vec3_t direction, vec3_t origin, float die );
};

inline std::unique_ptr < c_dlights > g_dlights = std::make_unique < c_dlights > ( );

#include "impl/context.inl"