#pragma once
#include "..\..\allah.h"

#include "random.h"
#include "fnv1a.h"

//#define __builtin_bswap32 _byteswap_ulong


#define valve___g_local_player nem::valve::g_local_player
#define valve___g_client nem::valve::g_client
#define valve___g_global_vars nem::valve::g_global_vars
#define valve___g_engine nem::valve::g_engine
#define valve___g_entity_list nem::valve::g_entity_list
#define valve___g_client_state nem::valve::g_client_state
#define valve___g_move_helper nem::valve::g_move_helper
#define valve___g_prediction nem::valve::g_prediction
#define valve___g_movement nem::valve::g_movement
#define valve___g_cvar nem::valve::g_cvar
#define valve___g_engine_trace nem::valve::g_engine_trace
#define valve___g_panel nem::valve::g_panel
#define valve___g_effects nem::valve::g_effects
#define valve___g_surface_data nem::valve::g_surface_data
#define valve___g_input nem::valve::g_input
#define valve___g_game_event_mgr nem::valve::g_game_event_mgr
#define valve___g_studio_render nem::valve::g_studio_render
#define valve___g_model_render nem::valve::g_model_render
#define valve___g_mdl_cache nem::valve::g_mdl_cache
#define valve___g_mat_system nem::valve::g_mat_system
#define valve___g_view_render nem::valve::g_view_render
#define valve___g_surface nem::valve::g_surface
#define valve___g_font_mgr nem::valve::g_font_mgr
#define valve___g_localize nem::valve::g_localize
#define valve___g_model_info nem::valve::g_model_info
#define valve___g_glow_mgr nem::valve::g_glow_mgr
#define valve___g_beams nem::valve::g_beams
#define valve___g_debug_overlay nem::valve::g_debug_overlay

namespace nem::valve {
	class c_client {
	public:
		__forceinline client_class_t* all_classes( );
	} inline* g_client{};

	struct global_vars_t {
		float			m_real_time{};
		int				m_frame_count{};
		float			m_absolute_frame_time{};
		float			m_absolute_frame_start_time_std_dev{};
		float			m_cur_time{};
		float			m_frame_time{};
		int				m_max_clients{};
		int				m_tick_count{};
		float			m_interval_per_tick{};
		float			m_interpolation_amount{};
		int				m_sim_ticks_this_frame{};
		int				m_network_protocol{};
		std::uintptr_t	m_save_data{};
		bool			m_client{};
		bool			m_remote_client{};
	} inline* g_global_vars{};

	class c_engine {
	public:
		__forceinline bool in_game( );

		__forceinline int index_for_uid( const int uid );

		__forceinline std::optional< player_info_t > player_info( const int index );

		__forceinline net_channel_info_t* net_channel_info( );

		__forceinline qangle_t view_angles( );

		__forceinline int get_tick ( );

		__forceinline float last_time_stamp( );

		__forceinline void set_view_angles( const qangle_t& angles );

		__forceinline void fire_events( );

		__forceinline const mat4x4_t& w2s_matrix( );

		__forceinline void exec_cmd( const char* cmd );

		__forceinline int local_player_index( );

		__forceinline void get_screen_size ( int& width, int& height );

		__forceinline std::uintptr_t* bsp_tree_query( );
	} inline* g_engine{};

	class c_entity;

	class c_entity_list {
	public:
		__forceinline c_entity* find_entity( const int index );

		__forceinline c_entity* find_entity( const e_ent_handle handle );

		__forceinline int highest_index( );
	} inline* g_entity_list{};

	struct client_state_t {
		char			pad0[ 156u ]{};
		net_channel_t*	m_net_channel{};
		int				m_challenge_number{};
		char			pad1[ 4u ]{};
		double			m_connect_time{};
		int				m_retry_number{};
		char			pad2[ 84u ]{};
		int				m_signon_state{};
		char			pad3[ 4u ]{};
		double			m_next_cmd_time{};
		int				m_server_count{};
		int				m_cur_seq{};
		char			pad4[ 8u ]{};
		float			m_clock_offsets[ 16u ]{};
		int				m_cur_clock_offset{};
		int				m_server_tick{};
		int				m_client_tick{};
		int				m_delta_tick{};
		char			pad5[ 4u ]{};
		int				m_view_entity{};
		int				m_player_slot{};
		bool			m_paused{};
		char			pad6[ 3u ]{};
		char			m_level_name[ 260u ]{};
		char			m_level_name_short[ 40u ]{};
		char			pad7[ 212u ]{};
		int				m_max_clients{};
		char			pad8[ 18836u ]{};
		int				m_old_tick_count{};
		float			m_tick_remainder{};
		float			m_frame_time{};
		int				m_last_cmd_out{};
		int				m_choked_cmds{};
		int				m_last_cmd_ack{};
		int				m_last_server_tick{};
		int				m_cmd_ack{};
		int				m_sound_seq{};
		int				m_last_progress_percent{};
		bool			m_is_hltv{};
		char			pad9[ 75u ]{};
		qangle_t		m_view_angles{};
		char			pad10[ 208u ]{};
	} inline* g_client_state{};
	
	class c_move_helper {
	public:
		__forceinline void set_host( c_player* const player );
		__forceinline void process_impacts( );
	} inline* g_move_helper{};

	struct prediction_t {
		__forceinline void update(
			const int start_frame, const bool valid_frame,
			const int in_acked, int out_cmd
		);

		__forceinline void setup_move(
			c_player* const player, user_cmd_t* const user_cmd,
			c_move_helper* const move_helper, move_data_t* const move_data
		);

		__forceinline void check_moving_on_ground(
			c_entity* entity, double frame_time
		);

		__forceinline void finish_move(
			c_player* const player,
			user_cmd_t* const user_cmd, move_data_t* const move_data
		);

		char	pad0[ 8u ]{};
		bool	m_in_prediction{};
		char	pad1{};
		bool	m_engine_paused{};
		bool	m_old_cl_predict_value{};
		int		m_prev_start_frame{};
		int		m_incoming_packet_number{};
		float	m_last_server_world_time_stamp{};
		bool	m_first_time_predicted{};
		char	pad2[ 3u ]{};
		int		m_cmds_predicted{};
		int		m_server_cmds_acked{};
		bool	m_prev_ack_had_errors{};
	} inline* g_prediction{};

	typedef void* file_name_handle_t;
	struct snd_info_t {
		int			m_nGuid;
		file_name_handle_t m_filenameHandle;
		int			m_nSoundSource;
		int			m_nChannel;
		int			m_nSpeakerEntity;
		float		m_flVolume;
		float		m_flLastSpatializedVolume;
		float		m_flRadius;
		int			m_nPitch;
		vec3_t* m_pOrigin;
		vec3_t* m_pDirection;
		bool		m_bUpdatePositions;
		bool		m_bIsSentence;
		bool		m_bDryMix;
		bool		m_bSpeaker;
		bool		m_bFromServer;
	};


	struct engine_sound_t { 

		__forceinline void get_act_sounds ( utl_vec_t < snd_info_t >& snd_list ) { 
			using fn_t = void ( __thiscall* )( decltype( this ), utl_vec_t < snd_info_t >& );

			return ( *reinterpret_cast< fn_t** >( this ) )[ 19u ] ( this, snd_list );
		};

	} inline* g_engine_sound {};

	class c_movement {
	public:
		__forceinline void process_movement(
			c_player* const player, move_data_t* const move_data
		);

		__forceinline void start_track_prediction_errors( c_player* const player );

		__forceinline void finish_track_prediction_errors( c_player* const player );
		__forceinline vec3_t& get_player_view_offset(bool ducked);
		__forceinline vec3_t& get_player_mins ( bool ducked );
	} inline* g_movement{};

	class c_cvar {
	public:
		__forceinline cvar_t* find_var( const char* name );

		__forceinline void con_print( const bool notify, const std::uint8_t& clr, const char* str );
	} inline* g_cvar{};
	
	class c_engine_trace {
	public:
		__forceinline e_mask point_contents(
			const vec3_t& point, const e_mask mask = e_mask::all,
			c_entity** const entity = nullptr
		);

		__forceinline void clip_ray_to_entity(
			const ray_t& ray, const e_mask mask,
			c_entity* entity, trace_t* const trace
		);

		__forceinline void trace_ray(
			const ray_t& ray, const e_mask mask,
			trace_filter_t* const filter, trace_t* const trace
		);
	} inline* g_engine_trace{};

	class c_panel {

	public:
		__forceinline const char* get_name ( uint32_t ayo );

	}inline* g_panel{};

	struct ColorRGBExp32 {
		unsigned char r, g, b;
		signed char exponent;
	};

	struct dlight_t {
		int flags;
		vec3_t origin;
		float radius;
		ColorRGBExp32 color;      // Light color with exponent
		float die;                // stop lighting after this time
		float decay;              // drop this each second
		float minlight;           // don't add when contributing less
		int key;
		int style;                // lightstyle

								  // For spotlights. Use m_OuterAngle == 0 for point lights
		vec3_t m_Direction;       // center of the light cone
		float m_InnerAngle;
		float m_OuterAngle;

		// see comments above about HL2_BROKEN_MIN_LIGHTING_VALUE and MIN_LIGHTING_VALUE
		// THIS SHOULD ONLY GET CALLED FROM THE ENGINE
		float GetRadius ( ) const
		{
			// return FastSqrt( radius * radius * ( HL2_BROKEN_MIN_LIGHTING_VALUE / MIN_LIGHTING_VALUE ) );
			return radius;
		}

		// see comments above about HL2_BROKEN_MIN_LIGHTING_VALUE and MIN_LIGHTING_VALUE
		// THIS SHOULD ONLY GET CALLED FROM THE ENGINE
		float GetRadiusSquared ( ) const
		{
			// return radius * radius * ( HL2_BROKEN_MIN_LIGHTING_VALUE / MIN_LIGHTING_VALUE );
			return radius * radius;
		}

		// THIS SHOULD ONLY GET CALLED FROM THE ENGINE
		float IsRadiusGreaterThanZero ( ) const
		{
			// don't bother calculating the new radius if you just want to know if it is greater than zero.
			return radius > 0.0f;
		}
	};
	struct color32;
	class c_effects {
	public:
		virtual int draw_decal_index_who_asked ( char* name ) = 0;

		virtual void decal_shoot ( int textureIndex, int entity, const model_t* model, const vec3_t& model_origin, const qangle_t& model_angles, const vec3_t& position, const vec3_t* saxis, int flags ) = 0;

		virtual void decal_clr_shot ( int textureIndex, int entity, const model_t* model, const vec3_t& model_origin, const qangle_t& model_angles, const vec3_t& position, const vec3_t* saxis, int flags, const color32& rgbaColor ) = 0;

		virtual void player_decal_shot ( valve::material_t* material, void* userdata, int entity, const model_t* model, const vec3_t& model_origin, const qangle_t& model_angles, const vec3_t& position, const vec3_t* saxis, int flags, const color32& rgbaColor ) = 0;

		virtual dlight_t* cl_alloc_dlight ( int key ) = 0;

		virtual dlight_t* cl_alloc_elight ( int key ) = 0;

		virtual int cl_get_active_dlights ( dlight_t* pList[ 32 ] ) = 0;

		virtual const char* draw_decal_from_mother ( int nIndex ) = 0;

		virtual dlight_t* get_elight_by_key ( int key ) = 0;
	} inline* g_effects{};

	class c_surface_data {
	public:
		__forceinline surface_data_t* find( const int index );
	} inline* g_surface_data{};

	struct input_t {
	public:
		__forceinline user_cmd_t* user_cmd( const int slot, const int seq_number );

		__forceinline bool is_in_tp ( int islot = -1 ) {
			using fn_t = int ( __thiscall* )( decltype( this ), int );

			return ( *reinterpret_cast< fn_t** >( this ) ) [ 32u ] ( this, islot );
		};

		char					pad0[ 12u ]{};
		bool					m_track_ir_available{};
		bool					m_mouse_initialized{};
		bool					m_mouse_active{};
		char					pad1[ 154u ]{};
		bool					m_camera_in_third_person{};
		char					pad2[ 2u ]{};
		vec3_t					m_camera_offset{};
		char					pad3[ 56u ]{};
		user_cmd_t*				m_cmds{};
		verified_user_cmd_t*	m_verified_cmds{};
	} inline* g_input{};

	class c_game_event_mgr {
	public:
		__forceinline void add_listener( base_event_listener_t* listener, const char* event_name, const bool server_side );
	} inline* g_game_event_mgr{};

	struct studio_render_t {
		__forceinline void set_alpha( const float value );

		__forceinline void override_material( material_t* const material, const int type = 0 );

		__forceinline bool forced_material_override( );

		char		pad0[ 592u ]{};
		material_t* m_override_material{};
		char		pad1[ 12u ]{};
		int			m_override_type{};
	} inline* g_studio_render{};

	class c_model_render {
	public:
		__forceinline void draw_model(
			const std::uintptr_t context, const draw_model_state_t& state,
			const model_render_info_t& info, mat3x4_t* const bones
		);
	} inline* g_model_render{};

	class c_mdl_cache {
	public:
		__forceinline std::uintptr_t lookup_hw_data( const std::uint16_t handle );
	} inline* g_mdl_cache{};

	class c_mat_system {
	public:
		__forceinline std::uint16_t first_material( );

		__forceinline std::uint16_t next_material( const std::uint16_t handle );

		__forceinline std::uint16_t invalid_material( );
		
		__forceinline material_t* material( const std::uint16_t handle );

		__forceinline material_t* find_material(
			const char* name, const char* group, const bool complain = true, const char* complain_prefix = nullptr
		);

		__forceinline material_t* create_material( const char* name, const std::uintptr_t key_values );

		__forceinline std::uintptr_t render_context( );
	} inline* g_mat_system{};

	class render_view_t {
	public:
		virtual void draw_brush_mdl ( void*, valve::model_t* mdl, const vec3_t& origin, const qangle_t& angle, bool sort ) = 0;
		virtual void draw_identity_brush_model ( void* who, valve::model_t* mdl ) = 0;
		virtual void touch_light ( void* idi_naxui ) = 0;
		virtual void draw_3d_debug_overlays ( void ) = 0;
		virtual void set_blend ( float blend ) = 0;

	} inline* g_render_view {};

	struct view_render_t {
		char			pad0[ 4u ];
		view_setup_t	m_setup{};
	} inline* g_view_render{};

	struct c_surface {
		__forceinline void start_drawing( );

		__forceinline void finish_drawing( );

		__forceinline void set_text_font( const unsigned long font );

		__forceinline void set_text_pos( const vec2_t& pos );

		__forceinline void set_text_clr( const std::uint32_t clr );

		__forceinline void add_text( const wchar_t* const str, const int len );

		__forceinline void set_clr( const std::uint32_t clr );

		__forceinline void set_texture( const int id );

		__forceinline unsigned long create_font( );

		__forceinline void add_poly_line(
			int* const x_points, int* const y_points, const int points_count
		);

		__forceinline void add_clred_text(
			const unsigned long font, const vec2_t& pos,
			const int r, const int g, const int b, const int a, const char* txt
		);

		__forceinline void add_line( const vec2_t& from, const vec2_t& to );

		__forceinline void add_rect( const vec2_t& min, const vec2_t& max );

		__forceinline void add_textured_rect( const vec2_t& min, const vec2_t& max );

		__forceinline void add_rect_filled( const vec2_t& min, const vec2_t& max );

		__forceinline void set_texture_rgba(
			const int id, const std::uint8_t* const clrs, const int width, const int height
		);

		__forceinline int create_new_texture( const bool procedural = false );

		__forceinline void add_textured_polygon(
			const int vertices_count, const vertex_t* const vertices, const bool clip = true
		);
	} inline* g_surface{};

	struct c_font_mgr {
		__forceinline void calc_text_size( const unsigned long font, const wchar_t* str, int& width, int& height );

		__forceinline void set_font_glyph_set(
			const unsigned long font, const char* name, const int tall, const int weight, const int blur,
			const int scanlines, const int flags, const int range_min = 0, const int range_max = 0
		);
	} inline* g_font_mgr{};

	struct c_localize {
		__forceinline wchar_t* find_safe( const char* token );
	} inline* g_localize{};

	class c_model_info {
	public:
		__forceinline int lookup_model_index( const char* name );

	} inline* g_model_info{};

	struct glow_mgr_t {
		struct object_t {
			int			m_next_free_slot{};
			c_entity*	m_entity{};
			float		m_red{};
			float		m_green{};
			float		m_blue{};
			float		m_alpha{};
			bool		m_alpha_capped_by_render_alpha{};
			char		pad0[ 3u ]{};
			float		m_alpha_fn_of_max_vel{};
			float		m_bloom_amt{};
			float		m_pusle_overdrive{};
			bool		m_render_when_occluded{};
			bool		m_render_when_unoccluded{};
			bool		m_full_bloom_render{};
			char		pad1[ 1u ]{};
			int			m_full_bloom_stencil_test_value{};
			int			m_render_style{};
			int			m_split_screen_slot{};
		};

		utl_vec_t< object_t >	m_objects{};
	} inline* g_glow_mgr{};

	class c_beams {
	public:
		__forceinline void draw_beam( const std::uintptr_t beam );

		__forceinline std::uintptr_t create_beam_points( beam_info_t& info );

		__forceinline std::uintptr_t create_beam_ring_point( beam_info_t& info );
	} inline* g_beams{};

	class c_debug_overlay {
	public:
		__forceinline void add_box_overlay(
			const vec3_t& pos, const vec3_t& min, const vec3_t& max, const qangle_t& rotation,
			const int r, const int g, const int b, const int a, const float duration
		);

		__forceinline int screen_pos ( const vec3_t& pos, const vec3_t& screen );

	} inline* g_debug_overlay{};

	__forceinline int to_ticks( const float time );

	__forceinline float to_time( const int ticks );
}

#include "impl/interfaces.inl"