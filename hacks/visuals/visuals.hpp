#pragma once

namespace nem::hacks {
	class c_visuals {
	private:
		struct bbox_t {
			vec2_t m_min {}, m_size {};
		};

		struct grenade_simulation_t {
			__forceinline grenade_simulation_t ( ) = default;

			__forceinline grenade_simulation_t (
				valve::c_player* const owner, const valve::e_item_index index,
				const vec3_t& origin, const vec3_t& velocity, const float throw_time, const int offset
			) : m_owner { owner }, m_index { index } { predict ( origin, velocity, throw_time, offset ); }

			void predict ( const vec3_t& origin, const vec3_t& velocity, const float throw_time, const int offset );

			bool physics_simulate ( );

			void physics_trace_entity (
				const vec3_t& src, const vec3_t& dst,
				const valve::e_mask mask, valve::trace_t& trace
			);

			void physics_push_entity ( const vec3_t& push, valve::trace_t& trace );

			void perform_fly_collision_resolution ( valve::trace_t& trace );

			void think ( );

			void detonate ( const bool bounced );

			void update_path ( const bool bounced );

			__forceinline void push_broken_ent ( valve::c_entity* ent )
			{
				m_broken_ents.emplace_back ( ent );
			}

			__forceinline void clear_broken_ents ( )
			{
				m_broken_ents.clear ( );
			}

			__forceinline bool is_ent_broken ( valve::c_entity* ent )
			{
				return find ( m_broken_ents.begin ( ), m_broken_ents.end ( ), ent ) != m_broken_ents.end ( );
			}

			bool										m_detonated {};
			valve::c_player* m_owner {};
			vec3_t										m_origin {}, m_velocity {};
			valve::c_entity* m_last_hit_entity {}, * m_last_breakable {};
			float										m_detonate_time {}, m_expire_time {}, m_source_time {};
			valve::e_item_index							m_index {};
			int											m_tick {}, m_next_think_tick {},
				m_last_update_tick {}, m_bounces_count {}, m_collision_group {};
			std::vector< std::pair< vec3_t, bool > >	m_path {};
			std::vector < valve::c_entity* >            m_broken_ents {};
		};

		using throwed_grenades_t = std::unordered_map< valve::e_ent_handle, grenade_simulation_t >;

		bool calc_bbox ( valve::c_player* const player, bbox_t& bbox ) const;

		std::optional< valve::bones_t > try_to_lerp_bones ( const int index ) const;

		valve::material_t* create_material (
			const std::string_view name,
			const std::string_view shader, const std::string_view data
		) const;

		void override_material (
			const int type, const bool ignore_z,
			const float r, const float g, const float b, const float a
		) const;

		const char* char_code_for_item_index ( const valve::e_item_index index ) const;

		void add_line (
			const vec2_t& from, const vec2_t& to, const std::uint32_t clr
		) const;

		void add_rect (
			const vec2_t& pos, const vec2_t& size, const std::uint32_t clr
		) const;

		void add_rect_filled (
			const vec2_t& pos, const vec2_t& size, const std::uint32_t clr
		) const;

		void add_text (
			vec2_t pos, const unsigned long font,
			const std::string_view str, const std::uint32_t clr, const int flags
		) const;

		void add_text (
			vec2_t pos, const unsigned long font,
			const std::wstring_view str, const std::uint32_t clr, const int flags
		) const;

		void add_vertices (
			const valve::vertex_t* const vertices,
			const std::size_t vertices_count, const std::uint32_t clr
		) const;

		void add_circle (
			const vec2_t& pos, const float radius,
			const int segments_count, const std::uint32_t clr
		) const;

		void add_3d_circle ( const vec3_t& origin, float radius, const std::uint32_t clr );

		void add_filled_circle (
			const vec2_t& pos, const float radius,
			const int segments_count, const std::uint32_t clr
		) const;

		void add_beam_ring ( const vec3_t& pos, const float* const clr ) const;


		void add_grenade_warning_beam ( const vec3_t& from, const vec3_t& to, const float* const clr ) const;

		void oof_arrow ( const vec3_t& pos ) const;

		void handle_player ( valve::c_player* const player ) const;

		void third_person ( );

		struct bullet_impact_t {
			__forceinline bullet_impact_t ( ) = default;

			__forceinline bullet_impact_t (
				const float time, const vec3_t& from, const vec3_t& pos
			) : m_time { time }, m_from { from }, m_pos { pos } {}

			float	m_time {};
			bool	m_final { true };

			vec3_t	m_from {}, m_pos {};
		};

		struct shot_mdl_t {
			int                         m_player_index {};
			unsigned int                m_hash {};
			float						m_time {};
			valve::bones_t				m_bones {};
			mat3x4_t					m_world_matrix {};


			valve::model_render_info_t	m_info {};
			valve::draw_model_state_t	m_state {};
		};

		int								m_backup_smoke_count {};

		std::vector< vec3_t >			m_player_sounds {};

		float							m_last_hurt_time {};
		std::optional< pen_data_t >		m_pen_data {};


		std::vector< shot_mdl_t >		m_shot_mdls {};
		bool							m_rendering_shot_mdl {};
	public:
		void init ( );
		std::vector< bullet_impact_t >	m_bullet_impacts {};
		void on_paint ( );
		throwed_grenades_t				m_throwed_grenades {};
		void on_render_start ( );
		void draw_auto_peek ( );
		void change_sky_box ( );

		void draw_watermark ( );

		void on_render_end ( );
		bool add_grenade_simulation ( const grenade_simulation_t& sim, const bool warning ) const;
		void on_create_move ( const valve::user_cmd_t& cmd );
		void add_beam ( const vec3_t& from, const vec3_t& to, const float* const clr ) const;
		void on_override_view ( valve::view_setup_t* const setup );

		void preserve_killfeed ( );

		void bloom_modulation ( valve::c_entity* entity );

		void handle_weapon ( valve::c_weapon* const weapon ) const;

		void draw_key_binds ( bool imgui_suck );

		static bool is_entity_broken ( valve::c_entity* entity );

		void change_shadows ( valve::c_entity* entity );

		void change_agent ( valve::e_frame_stage );

		struct bullet_trace_t {
			bool m_local_trace {};
			vec3_t m_start_pos {}, m_end_pos {};
			float m_spawn_time {};
			float m_alpha_modifier {};
		};

		std::deque < bullet_trace_t > m_bullet_tracers {};

		grenade_simulation_t			m_grenade_trajectory {};

		void draw_spectators ( bool im_gui_suck );
		void handle_projectile ( valve::c_entity* const entity, const valve::e_class_id class_id );
		void draw_c4_status ( bool im_gui_suck );
		std::mutex m_mutex;

		struct binds_data_t {
		public:
			std::string m_name = "";
			std::string m_state = "";
			float m_alpha_modifier = 0.f;
		};

		std::map < std::string, binds_data_t > m_binds;

		struct motion_blur_history_t {
			motion_blur_history_t ( ) noexcept {
				m_last_time_upd = 0.f;
				m_prev_pitch = 0.f;
				m_prev_yaw = 0.f;
				m_prev_pos = {};
				m_no_rot_motion_blur_until = 0.f;
			}

			float m_last_time_upd {};
			float m_prev_pitch {};
			float m_prev_yaw {};
			vec3_t m_prev_pos {};
			float m_no_rot_motion_blur_until {};
		};

		void motion_blur ( valve::view_setup_t* view_setup ) noexcept;

		void on_post_screen_effects ( );

		bool on_draw_model (
			valve::studio_render_t* const ecx, const std::uintptr_t edx,
			const std::uintptr_t a0, const valve::draw_model_info_t& info,
			mat3x4_t* const bones, float* const a3, float* const a4, const vec3_t& origin, int flags
		) const;

		void on_calc_view (
			valve::c_player* const ecx, const std::uintptr_t edx,
			vec3_t& origin, qangle_t& angles, float& z_near, float& z_far, float& fov
		);

		struct {
			unsigned long	m_verdana12 {}, m_verdana12_bold {},
				m_verdana26 {}, m_verdana26n {}, m_tahoma12 {},
				m_small_fonts8 {}, m_verdana_12 {}, m_verdana_11 {}, m_icons_for_warning {}, m_icons14 {};

			ImFont* m_tahoma14;
			ImFont* m_verdana;
			ImFont* m_icon_font;
			ImFont* m_warning_icon_font;
			ImFont* m_log_font;
			ImFont* m_mojesh_ubrat_ee_ya_razreshayu;
			ImFont* m_log_icons;
			ImFont* m_museo_sans_name;
			ImFont* m_segoe_ui_esp;
			ImFont* m_visitor_tt2_brk;
			ImFont* m_smallest_pixel;
			ImFont* m_skeet_font_esp, * m_04b{};
			ImFont* m_xxxtentacion_icons;
			ImFont* m_menu_icons_fontik;
			std::string m_last_font {};

		}m_fonts {};

		void add_shot_mdl ( valve::c_player* const player, const std::shared_ptr< lag_record_t >& lag_record );

		__forceinline float& last_hurt_time ( );

		__forceinline std::vector< vec3_t >& player_sounds ( );

		__forceinline std::vector< bullet_impact_t >& bullet_impacts ( );
	};

	inline const auto g_visuals = std::make_unique< c_visuals > ( );

#define MAX_SHAPES_COUNT 1000

	enum e_shape_type {
		SH_LINE,
		SH_2D_CROSS,
	};

	struct s_shape {
		bool                   m_active = false;
		bool                   m_blur = false;
		float                  m_start_time = 0.0f;
		float                  m_duration = 0.0f;
		e_shape_type           m_shape_type = SH_LINE;
		col_t                  m_color = col_t::palette_t::white ( );
		std::vector < vec3_t > m_coordinates = { };
		std::vector < int >  m_data = { };

		s_shape ( bool blur, float duration, e_shape_type type, col_t color )
		{
			m_active = true;
			m_blur = blur;
			m_start_time = valve___g_global_vars->m_real_time;
			m_duration = duration;
			m_shape_type = type;
			m_color = color;
			m_coordinates = { };
			m_data = { };

			m_coordinates.clear ( );
			m_data.clear ( );
		}

		s_shape ( )
		{
			m_active = false;
			m_blur = false;
			m_start_time = 0.0f;
			m_duration = 0.0f;
			m_shape_type = SH_LINE;
			m_color = col_t::palette_t::white ( );
			m_coordinates = { };
			m_data = { };

			m_coordinates.clear ( );
			m_data.clear ( );
		}
	};

	class c_cached_renderer {
	public:
		void push_line ( vec3_t& start, vec3_t& end, bool blur, float duration, col_t color );
		void push_cross ( vec3_t pos, float size, float duration, bool dynamic, col_t color );

	public:
		void render ( );
		void update_shapes ( );
		int get_free_shape ( );

	public:
		s_shape m_cached_shapes[ MAX_SHAPES_COUNT ] {};

	};
	inline const auto g_cached_renderer = std::make_unique < c_cached_renderer > ( );

	struct dormant_data_t {
		float m_alpha;
		bool m_was_unseen;
		vec3_t m_origin;
		float m_receive_time;
	};
	struct b_box {
		RECT m_bbox;
		int m_width;
		int m_height;

		std::string m_weapon_name;
		std::string m_weapon_icon;
		valve::c_weapon* m_weapon;
	};
	class c_player_esp {
	public:
		void draw_data ( );
		std::array<dormant_data_t, 65> m_dormant_data {};
		std::array < float, 65 > m_is_not_alive_alpha {};
		std::array < vec3_t, 65 > m_alive_origin{};
		std::array < vec3_t, 65 > m_alive_mins{};
		std::array < vec3_t, 65 > m_alive_maxs{};
	protected:
		struct flags_data_t {
			std::string m_name {};
			float m_alpha {};
			col_t m_clr {};
		};

		RECT get_bbox ( valve::c_entity* ent );
		void draw_box ( valve::c_player* ent, RECT& rect );
		void draw_name ( valve::c_player* ent, RECT& rect );
		void draw_health ( valve::c_player* player, RECT& rect );
		void draw_flags ( valve::c_player* player, RECT& rect );
		void draw_weapon ( valve::c_player* player, RECT& rect );
		void draw_ammo ( valve::c_player* player, RECT& rect );
		void draw_skeletons ( valve::c_player* player, RECT& rect );
		void oof_indicate ( valve::c_player* player );
	};
	inline std::unique_ptr < c_player_esp > g_player_esp = std::make_unique < c_player_esp > ( );

	class c_dormant_esp {
	public:
		void start ( );

		bool adjust_sound ( valve::c_player* player );
		void setup_adjust ( valve::c_player* player, valve::snd_info_t& sound );
		bool valid_sound ( valve::snd_info_t& sound );

		struct SoundPlayer {
			void reset ( bool store_data = false, const vec3_t& origin = {}, int flags = 0 )
			{
				if ( store_data )
				{
					m_iReceiveTime = valve___g_global_vars->m_real_time;
					m_vecOrigin = origin;
					m_nFlags = flags;
				}
				else
				{
					m_iReceiveTime = 0.0f;
					m_vecOrigin = {};
					m_nFlags = 0;
				}
			}

			void Override ( valve::snd_info_t& sound )
			{
				m_iReceiveTime = valve___g_global_vars->m_real_time;
				m_vecOrigin = *sound.m_pOrigin;
			}

			float m_iReceiveTime = 0.0f;
			vec3_t m_vecOrigin = {};
			int m_nFlags = {};
		} m_cSoundPlayers[ 65 ];

		valve::utl_vec_t <valve::snd_info_t> m_utlvecSoundBuffer;
		valve::utl_vec_t <valve::snd_info_t> m_utlCurSoundList;
	};

	inline std::unique_ptr < c_dormant_esp > g_dormant_esp = std::make_unique < c_dormant_esp > ( );

	class c_hit_marker {
	public:
		struct hit_marker_data_t {
			__forceinline hit_marker_data_t ( const float& time, const int& index, const int& dmg, const int& hit_group, const vec3_t& pos, const int& add_to_x )
			{
				m_time = time;
				m_index = index;
				m_dmg = dmg;
				m_hit_group = hit_group;
				m_moved = 0.f;
				m_alpha = 255;
				m_pos = pos;
				m_add_to_x = add_to_x;
			}

			float m_time;
			int m_index;
			int m_dmg;
			int m_hit_group;
			float m_moved;
			float m_alpha;
			col_t m_col;
			vec3_t m_pos;
			int m_add_to_x{};
		};

		void add_shot ( const hit_marker_data_t& data );

		void handle_draw ( );

		void handle_dmg_render ( hit_marker_data_t& data, const vec3_t& screen_pos );

	protected:
		std::deque < hit_marker_data_t > m_hits;
	};

	inline std::unique_ptr < c_hit_marker > g_hit_marker = std::make_unique < c_hit_marker > ( );

	class c_weather {
	public:
		void update_weather ( );
		__forceinline void reset_weather ( ) const;

		bool m_weather_created {};
	};

	inline const std::unique_ptr < c_weather > g_weather = std::make_unique < c_weather > ( );
}

enum e_font_flags {
	FONT_NONE,
	FONT_CENTERED_X = 1 << 0,
	FONT_CENTERED_Y = 1 << 1,
	FONT_CENTERED = FONT_CENTERED_X | FONT_CENTERED_Y,
	FONT_DROP_SHADOW = 1 << 2,
	FONT_OUTLINE = 1 << 4
};

struct s_priority_element {
	char m_name[ 16 ];
	int m_value = NULL;

	s_priority_element ( const char* name, int value )
	{
		strcpy ( m_name, name );
		m_value = value;
	}
};

namespace render {
	void init ( );

	void add_to_draw_list ( );

	void begin ( );

	vec2_t get_text_size ( std::string_view txt, ImFont* font );

	void text ( std::string_view txt, vec2_t pos, const col_t& clr, ImFont* font, bit_flag_t<uint8_t> flags = FONT_NONE );

	void line ( const vec2_t& from, const vec2_t& to, const col_t& clr );
	void draw_line ( float x1, float y1, float x2, float y2, col_t clr, float thickness = 1.f );

	void rect ( const vec2_t& pos, const vec2_t& size, const col_t& clr, float rounding = 0.f, bool multiplied = false );

	void rect_filled ( const vec2_t& pos, const vec2_t& size, const col_t& clr, float rounding = 0.f, bool multiplied = false );

	void rect_filled_multi_clr ( const vec2_t& pos, const vec2_t& size, const col_t& clr_upr_left, const col_t& clr_upr_right, const col_t& clr_bot_left, const col_t& clr_bot_right );

	void polygon ( const std::vector<vec2_t>& points, const col_t& clr );

	void polygon_filled ( const std::vector<vec2_t>& points, const col_t& clr );

	void d_light_circle_3d ( vec3_t position, int point_count, float radius, col_t color );

	void triangle ( float x1, float y1, float x2, float y2, float x3, float y3, col_t clr, float thickness );

	void triangle_filled ( float x1, float y1, float x2, float y2, float x3, float y3, col_t clr );

	void render_filled_3d_circle ( const vec3_t& origin, float radius, col_t color );

	void render_convex_poly_filled ( ImVec2* vec, int num_points, col_t col );

	void render_3d_circle ( const vec3_t& origin, float radius, col_t color );

	void arc ( float x, float y, float radius, float min_angle, float max_angle, col_t col, float thickness );

	bool world_to_screen ( const vec3_t& in, vec3_t& out );

	void draw_rect_filled ( float x, float y, float w, float h, col_t clr, float rounding, ImDrawCornerFlags rounding_corners = 15 );

	void draw_rect ( float x1, float y1, float x2, float y2, col_t color );

	void add_to_draw_list ( );

	void handle_drawings ( );

	extern std::mutex m_mutex;

	extern vec2_t m_screen_size;

	extern std::shared_ptr<ImDrawList> m_draw_list;
	extern std::shared_ptr<ImDrawList> m_data_draw_list;
	extern std::shared_ptr<ImDrawList> m_replace_draw_list;
}

namespace fonts {
	extern ImFont* m_tahoma14;
}

#include "impl/visuals.inl"