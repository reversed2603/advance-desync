#pragma once

namespace nem::hooks {
	LRESULT CALLBACK wnd_proc ( const HWND hwnd, const UINT msg, const WPARAM wparam, const LPARAM lparam );
	inline decltype( &wnd_proc ) orig_wnd_proc {};

	long D3DAPI	d3d9_reset ( IDirect3DDevice9* const device, D3DPRESENT_PARAMETERS* const present_params );
	inline decltype( &d3d9_reset ) orig_d3d9_reset {};

	long D3DAPI d3d9_present (
		IDirect3DDevice9* const device, RECT* const src_rect,
		RECT* const dst_rect, const HWND dst_wnd_override, RGNDATA* const dirty_region
	);
	inline decltype( &d3d9_present ) orig_d3d9_present {};

	void __fastcall paint ( const std::uintptr_t ecx, const std::uintptr_t edx, const int mode );
	inline decltype( &paint ) orig_paint {};

	bool __fastcall trace_filter_for_head_collision ( std::uintptr_t ecx, std::uintptr_t edx, valve::c_player* player, void* trace_params );
	inline decltype ( &trace_filter_for_head_collision ) orig_trace_filter_for_head_collision {};

	int __fastcall sv_cheats_get_int ( valve::cvar_t* const ecx, const std::uintptr_t edx );
	inline decltype( &sv_cheats_get_int ) orig_sv_cheats_get_int {};

	void __fastcall draw_model (
		valve::studio_render_t* const ecx, const std::uintptr_t edx,
		const std::uintptr_t a0, const valve::draw_model_info_t& info,
		mat3x4_t* const bones, float* const a3, float* const a4, const vec3_t& origin, int flags
	);
	inline decltype( &draw_model ) orig_draw_model {};

	void __fastcall lock_cursor ( const std::uintptr_t ecx, const std::uintptr_t edx );
	inline decltype( &lock_cursor ) orig_lock_cursor {};

	void __fastcall clip_ray_collideable ( std::uintptr_t ecx, std::uintptr_t edx, const valve::ray_t& ray, std::size_t mask, valve::c_collideable* collideable, valve::trace_t* trace );
	inline decltype ( &clip_ray_collideable ) orig_clip_ray_collideable {};

	void __fastcall draw_set_clr (
		valve::c_surface* const ecx, const std::uintptr_t edx, const int r, const int g, const int b, const int a
	);
	inline decltype( &draw_set_clr ) orig_draw_set_clr {};

	void __stdcall create_move_proxy ( const int seq_number, const float input_sample_frame_time, const bool active );
	void __stdcall create_move ( const int seq_number, const float input_sample_frame_time, const bool active, bool& send_packet );

	void __cdecl  cl_move ( float accumulate_extra_samples, bool final_tick );
	inline decltype ( &cl_move ) orig_cl_move {};

	void __fastcall draw_mdl_exec ( std::uintptr_t ecx, std::uintptr_t edx, void* ctx, const valve::draw_model_state_t& state, const valve::model_render_info_t& info, mat3x4_t* bones );
	inline decltype ( &draw_mdl_exec ) orig_draw_mdl_exec {};

	int process_interpolated_list ( );
	inline decltype ( &process_interpolated_list ) orig_process_interp_list {};

	using orig_create_move_t = void ( __thiscall* )( valve::c_client* const, const int, const float, const bool );
	inline orig_create_move_t orig_create_move {};

	void __stdcall frame_stage_notify ( const valve::e_frame_stage stage );
	inline decltype( &frame_stage_notify ) orig_frame_stage_notify {};

	void __fastcall update_client_side_anim ( valve::c_player* const ecx, const std::uintptr_t edx );
	inline decltype( &update_client_side_anim ) orig_update_client_side_anim {};

	bool __fastcall setup_bones (
		const std::uintptr_t ecx, const std::uintptr_t edx, mat3x4_t* const bones, int max_bones, int mask, float time
	);
	inline decltype( &setup_bones ) orig_setup_bones {};

	bool __fastcall should_skip_anim_frame ( const std::uintptr_t ecx, const std::uintptr_t edx );
	inline decltype ( &should_skip_anim_frame ) orig_should_skip_anim_frame {};

	void __fastcall do_extra_bones_processing (
		valve::c_player* const ecx, const std::uintptr_t edx, int a0, int a1, int a2, int a3, int a4, int a5
	);
	inline decltype( &do_extra_bones_processing ) orig_do_extra_bones_processing {};

	void __fastcall accumulate_layers (
		valve::c_player* const ecx, const std::uintptr_t edx, int a0, int a1, float a2, int a3
	);
	inline decltype( &accumulate_layers ) orig_accumulate_layers {};

	void __fastcall standard_blending_rules (
		valve::c_player* const ecx, const std::uintptr_t edx, valve::model_data_t* const mdl_data, int a1, int a2, float a3, int mask
	);
	inline decltype( &standard_blending_rules ) orig_standard_blending_rules {};

	void __fastcall packet_start (
		const std::uintptr_t ecx, const std::uintptr_t edx, const int in_seq, const int out_acked
	);
	inline decltype( &packet_start ) orig_packet_start {};

	void __fastcall setup_move ( const std::uintptr_t ecx, const std::uintptr_t edx, valve::c_player* player, valve::user_cmd_t* user_cmd, valve::c_move_helper* move_helper, valve::move_data_t* move_data );
	inline decltype ( &setup_move ) orig_setup_move {};

	bool __fastcall is_bone_available ( void* ecx, std::ptrdiff_t bone_index );
	inline decltype ( &is_bone_available ) orig_is_bone_available {};

	void __fastcall packet_end ( const std::uintptr_t ecx, const std::uintptr_t edx );
	inline decltype( &packet_end ) orig_packet_end {};

	void __fastcall physics_simulate ( valve::c_player* const ecx, const std::uintptr_t edx );
	inline decltype( &physics_simulate ) orig_physics_simulate {};

	void __fastcall process_movement ( std::uintptr_t ecx, std::uintptr_t edx, valve::c_player* player, valve::move_data_t* move_data );
	inline decltype( &process_movement ) orig_process_movement {};

	void __fastcall on_latch_interpolated_vars (
		valve::c_player* const ecx, const std::uintptr_t edx, const int flags
	);
	inline decltype( &on_latch_interpolated_vars ) orig_on_latch_interpolated_vars {};

	bool __fastcall should_interpolate ( valve::c_player* const ecx, const std::uintptr_t edx );
	inline decltype( &should_interpolate ) orig_should_interpolate {};

	void __fastcall check_for_seq_change ( std::uintptr_t ecx, std::uintptr_t edx, std::uintptr_t hdr, int cur_seq, bool force_new_seq, bool interp );
	inline decltype ( &check_for_seq_change ) orig_check_for_seq_change {};

	bool __fastcall write_user_cmd_delta_to_buffer (
		const std::uintptr_t ecx, const std::uintptr_t edx,
		const int slot, valve::bf_write_t* const buffer, int from, int to, const bool is_new_cmd
	);
	inline decltype( &write_user_cmd_delta_to_buffer ) orig_write_user_cmd_delta_to_buffer {};

	void __cdecl velocity_modifier ( valve::recv_proxy_data_t* const data, valve::c_entity* const entity, void* const out );
	inline decltype( &velocity_modifier ) orig_velocity_modifier {};

	void __fastcall run_cmd ( std::uintptr_t ecx, std::uintptr_t edx, valve::c_player* player, valve::user_cmd_t* user_cmd, valve::c_move_helper* move_helper );
	inline decltype ( &run_cmd ) orig_run_cmd;

	void __fastcall clamp_bones_in_box ( valve::c_player* ecx, void* edx, mat3x4_t* matrix, int mask );
	inline decltype ( &clamp_bones_in_box ) orig_clamp_bones_in_box;

	void __cdecl sequence ( valve::recv_proxy_data_t* const data, valve::c_entity* const entity, void* const out );
	inline decltype( &sequence ) orig_sequence {};

	void __fastcall modify_eye_pos ( valve::anim_state_t* const ecx, const std::uintptr_t edx, vec3_t& eye_pos );
	inline decltype( &modify_eye_pos ) orig_modify_eye_pos {};

	bool __fastcall process_temp_entities ( const std::uintptr_t ecx, const std::uintptr_t edx, const std::uintptr_t msg );
	inline decltype( &process_temp_entities ) orig_process_temp_entities {};

	void __fastcall level_shutdown ( const std::uintptr_t ecx, const std::uintptr_t edx );
	inline decltype( &level_shutdown ) orig_level_shutdown {};

	bool __cdecl glow_effect_spectator (
		valve::c_player* player, valve::c_player* local,
		int& style, vec3_t& clr, float& alpha_from, float& alpha_to,
		float& time_from, float& time_to, bool& animate
	);
	inline decltype( &glow_effect_spectator ) orig_glow_effect_spectator {};

	void __fastcall get_clr_modulation (
		valve::material_t* const ecx, const std::uintptr_t edx,
		float* const r, float* const g, float* const b
	);
	inline decltype( &get_clr_modulation ) orig_get_clr_modulation {};

	void __fastcall paint_traverse ( void* ecx, void* edx, std::uint32_t id, bool force_repaint, bool allow_force );
	inline decltype ( &paint_traverse ) orig_paint_traverse {};

	bool __stdcall is_using_static_prop_debug_modes ( );
	inline decltype( &is_using_static_prop_debug_modes ) orig_is_using_static_prop_debug_modes {};

	bool __stdcall is_depth_something_in_nigga_ass ( );
	inline decltype ( &is_depth_something_in_nigga_ass ) orig_nigga_hack {};

	int __fastcall list_leaves_in_box (
		const std::uintptr_t ecx, const std::uintptr_t edx,
		const vec3_t& mins, const vec3_t& maxs, const uint16_t* const list, const int max
	);
	inline decltype( &list_leaves_in_box ) orig_list_leaves_in_box {};

	bool __fastcall override_cfg (
		valve::c_mat_system* const ecx, const std::uintptr_t edx,
		valve::mat_sys_cfg_t* const cfg, const bool update
	);
	inline decltype( &override_cfg ) orig_override_cfg {};

	bool __fastcall fog_enabled ( const std::uintptr_t ecx, const std::uintptr_t edx );
	inline decltype( &fog_enabled ) orig_fog_enabled {};

	void __fastcall override_view (
		const std::uintptr_t ecx, const std::uintptr_t edx, valve::view_setup_t* const setup
	);
	inline decltype( &override_view ) orig_override_view {};

	int __fastcall do_post_screen_space_effects (
		const std::uintptr_t ecx, const std::uintptr_t edx, valve::view_setup_t* const setup
	);
	inline decltype( &do_post_screen_space_effects ) orig_do_post_screen_space_effects {};

	int __fastcall weapon_debug_spread_show_get_int ( valve::cvar_t* const ecx, const std::uintptr_t edx );
	inline decltype( &weapon_debug_spread_show_get_int ) orig_weapon_debug_spread_show_get_int {};

	void __fastcall calc_view (
		valve::c_player* const ecx, const std::uintptr_t edx,
		vec3_t& origin, qangle_t& angles, float& z_near, float& z_far, float& fov
	);
	inline decltype( &calc_view ) orig_calc_view {};

	bool __stdcall in_prediction ( );
	using fn_t = bool ( __thiscall* ) ( void* );

	inline fn_t orig_in_prediction {};

	qangle_t* __fastcall get_eye_angles( valve::c_player* const ecx, const std::uintptr_t edx );
	inline decltype( &get_eye_angles ) orig_get_eye_angles{};

	bool __fastcall is_paused( const std::uintptr_t ecx, const std::uintptr_t edx );
	inline decltype( &is_paused ) orig_is_paused{};

	bool __fastcall is_hltv(const std::uintptr_t ecx, const std::uintptr_t edx);
	inline decltype (&is_hltv) orig_is_hltv{ };

	float __stdcall aspect_ratio(int width, int height);
	inline decltype (&aspect_ratio) orig_aspect_ratio{ };

	void __fastcall calc_viewmodel_bob ( valve::c_player* ecx, const std::uintptr_t edx, vec3_t& view_bob );
	inline decltype ( &calc_viewmodel_bob ) orig_viewmodel_bob {};

	const char* __fastcall get_foreign_fall_back_font_name ( void* ecx, std::uint32_t i );
	inline decltype ( &get_foreign_fall_back_font_name ) orig_i_hate_niggas {};

	/*struct event_listener_t : public valve::base_event_listener_t {
		void fire_game_event( valve::game_event_t* const event ) override;
	} inline g_event_listener{};*/
}