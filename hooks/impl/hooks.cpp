#include "../../nem.hpp"

//#include "..\..\menu\NL\Neverlose.h"

auto g_imgui_opened = true;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

namespace nem::hooks {
	LRESULT __stdcall wnd_proc ( const HWND hwnd, const UINT msg, const WPARAM wparam, const LPARAM lparam )
	{
		switch ( msg )
		{
			case WM_CHAR:
			{
				wchar_t wch;
				MultiByteToWideChar ( CP_ACP, MB_PRECOMPOSED, ( char* )&wparam, 1, &wch, 1 );
				ImGui::GetIO ( ).AddInputCharacter ( wch );

				return CallWindowProcA ( orig_wnd_proc, hwnd, msg, wparam, lparam );
			}

			case WM_XBUTTONDOWN:

				switch ( GET_KEYSTATE_WPARAM ( wparam ) )
				{
					case MK_XBUTTON1: g_Keybinds->m_iLastPressedKey = VK_XBUTTON1; break;
					case MK_XBUTTON2: g_Keybinds->m_iLastPressedKey = VK_XBUTTON2; break;
				}

				break;

			case WM_MBUTTONDOWN:

				g_Keybinds->m_iLastPressedKey = VK_MBUTTON;
				break;

			case WM_RBUTTONDOWN:
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			case WM_LBUTTONDOWN:

				g_Keybinds->m_iLastPressedKey = wparam;
				break;
		}

		if ( msg == WM_KEYUP
			&& wparam == VK_INSERT )
			g_imgui_opened ^= 1;

		if ( g_imgui_opened ) {
			ImGui_ImplWin32_WndProcHandler ( hwnd, msg, wparam, lparam );

			return 1;
		}

		return CallWindowProc ( orig_wnd_proc, hwnd, msg, wparam, lparam );
	}

	long D3DAPI	d3d9_reset ( IDirect3DDevice9* const device, D3DPRESENT_PARAMETERS* const present_params )
	{
		PostProcessing::onDeviceReset ( );

		ImGui_ImplDX9_InvalidateDeviceObjects ( );

		const auto ret = orig_d3d9_reset ( device, present_params );

		ImGui_ImplDX9_CreateDeviceObjects ( );

		return ret;
	}

	long D3DAPI d3d9_present (
		IDirect3DDevice9* const device, RECT* const src_rect,
		RECT* const dst_rect, const HWND dst_wnd_override, RGNDATA* const dirty_region
	)
	{
		thread_local bool pr_set = false;

		if ( !pr_set )
		{
			SetThreadPriorityBoost ( GetCurrentThread ( ), FALSE );
			SetThreadPriority ( GetCurrentThread ( ), THREAD_PRIORITY_HIGHEST );
			pr_set = true;
		}

		DWORD colorwrite, srgbwrite;
		IDirect3DVertexDeclaration9* vert_dec = nullptr;
		IDirect3DVertexShader9* vert_shader = nullptr;
		DWORD dwOld_D3DRS_COLORWRITEENABLE = NULL;
		device->GetRenderState ( D3DRS_COLORWRITEENABLE, &colorwrite );
		device->GetRenderState ( D3DRS_SRGBWRITEENABLE, &srgbwrite );

		device->SetRenderState ( D3DRS_COLORWRITEENABLE, 0xffffffff );
		//removes the source engine color correction
		device->SetRenderState ( D3DRS_SRGBWRITEENABLE, false );

		device->GetRenderState ( D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE );
		device->GetVertexDeclaration ( &vert_dec );
		device->GetVertexShader ( &vert_shader );
		device->SetRenderState ( D3DRS_COLORWRITEENABLE, 0xffffffff );
		device->SetRenderState ( D3DRS_SRGBWRITEENABLE, false );
		device->SetSamplerState ( NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
		device->SetSamplerState ( NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
		device->SetSamplerState ( NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );
		device->SetSamplerState ( NULL, D3DSAMP_SRGBTEXTURE, NULL );

		PostProcessing::setDevice ( device );
		PostProcessing::newFrame ( );

		ImGui_ImplDX9_NewFrame ( );
		ImGui_ImplWin32_NewFrame ( );

		ImGui::NewFrame ( );

		float flLastAlpha = ImGui::GetStyle( ).Alpha;

		static float flCurrAlpha = 1.f;
		ImGui::GetStyle( ).Alpha = flCurrAlpha;

		g_NL->Draw( device, g_imgui_opened );

		g_NL->AfterMenu( );

		g_Keybinds->SetAllKeybinds( g_NL->m_cVecElemets, true );
		g_Keybinds->SetAllKeybinds( g_NL->m_cAdditionalElemets, false );

		flCurrAlpha = ImGui::GetStyle( ).Alpha;

		ImGui::GetStyle( ).Alpha = flLastAlpha;

		hacks::g_visuals->draw_watermark ( );

		hacks::g_visuals->draw_key_binds ( g_imgui_opened );

		hacks::g_visuals->draw_spectators ( g_imgui_opened );

		hacks::g_visuals->draw_c4_status ( g_imgui_opened );

		render::add_to_draw_list ( );

		ImGui::EndFrame ( );

		ImGui::Render ( );

		ImGui_ImplDX9_RenderDrawData ( ImGui::GetDrawData ( ) );

		device->SetRenderState ( D3DRS_COLORWRITEENABLE, colorwrite );
		device->SetRenderState ( D3DRS_SRGBWRITEENABLE, srgbwrite );
		device->SetRenderState ( D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE );
		device->SetRenderState ( D3DRS_SRGBWRITEENABLE, true );
		device->SetVertexDeclaration ( vert_dec );
		device->SetVertexShader ( vert_shader );

		return orig_d3d9_present ( device, src_rect, dst_rect, dst_wnd_override, dirty_region );
	}

	void __fastcall paint ( const std::uintptr_t ecx, const std::uintptr_t edx, const int mode )
	{
		thread_local bool pr_set = false;

		if ( !pr_set )
		{
			SetThreadPriorityBoost ( GetCurrentThread ( ), FALSE );
			SetThreadPriority ( GetCurrentThread ( ), THREAD_PRIORITY_HIGHEST );
			pr_set = true;
		}

		orig_paint ( ecx, edx, mode );

		if ( mode & 1 || mode & 2 ) {

			valve___g_surface->start_drawing ( );

			hacks::g_visuals->on_paint ( );

			valve___g_surface->finish_drawing ( );
		}
	}

	int __fastcall sv_cheats_get_int ( valve::cvar_t* const ecx, const std::uintptr_t edx )
	{
		if ( *reinterpret_cast< std::uintptr_t* >( _AddressOfReturnAddress ( ) ) != g_context->addresses ( ).m_cam_sv_cheats_ret.get( ) )
			return orig_sv_cheats_get_int ( ecx, edx );

		return 1;
	}

	void __fastcall draw_model (
		valve::studio_render_t* const ecx, const std::uintptr_t edx,
		const std::uintptr_t a0, const valve::draw_model_info_t& info,
		mat3x4_t* const bones, float* const a3, float* const a4, const vec3_t& origin, int flags
	)
	{
		thread_local bool pr_set = false;

		if ( !pr_set )
		{
			SetThreadPriorityBoost ( GetCurrentThread ( ), FALSE );
			SetThreadPriority ( GetCurrentThread ( ), THREAD_PRIORITY_HIGHEST );
			pr_set = true;
		}

		if ( !hacks::g_visuals->on_draw_model ( ecx, edx, a0, info, bones, a3, a4, origin, flags ) )
			return;

		orig_draw_model ( ecx, edx, a0, info, bones, a3, a4, origin, flags );
	}

	void __fastcall lock_cursor ( const std::uintptr_t ecx, const std::uintptr_t edx )
	{
		using unlock_cursor_t = void ( __thiscall* )( const std::uintptr_t );

		if ( g_imgui_opened )
			return ( *reinterpret_cast< unlock_cursor_t** >( ecx ) )[ 66u ] ( ecx );

		orig_lock_cursor ( ecx, edx );
	}

	void __fastcall draw_set_clr (
		valve::c_surface* const ecx, const std::uintptr_t edx, const int r, const int g, const int b, const int a
	)
	{
		if ( !hacks::g_aim_bot->cfg ( ).m_bRemoveScope )
			return orig_draw_set_clr ( ecx, edx, r, g, b, a );

		const auto ret_addr = *reinterpret_cast< std::uintptr_t* >( _AddressOfReturnAddress ( ) );
		if ( ret_addr == g_context->addresses ( ).m_ret_to_scope_arc.get( )
			|| ret_addr == g_context->addresses ( ).m_ret_to_scope_lens.get( ) )
			return orig_draw_set_clr ( ecx, edx, r, g, b, 0 );

		if ( ret_addr != g_context->addresses ( ).m_ret_to_scope_clear.get( )
			&& ret_addr != g_context->addresses ( ).m_ret_to_scope_blurry.get( ) )
			return orig_draw_set_clr ( ecx, edx, r, g, b, a );

		orig_draw_set_clr ( ecx, edx, 0, 0, 0, 255 );

		const auto& screen_size = ImGui::GetIO ( ).DisplaySize;
		const auto center = screen_size / 2.f;

		ecx->add_line ( { center.x, 0.f }, { center.x, screen_size.y } );
		ecx->add_line ( { 0.f, center.y }, { screen_size.x, center.y } );

		orig_draw_set_clr ( ecx, edx, r, g, b, 0 );
	}


	__declspec ( naked ) void __stdcall create_move_proxy ( const int seq_number, const float input_sample_frame_time, const bool active )
	{
		__asm {
			push ebp
			mov ebp, esp
			push ebx
			push esp
			push dword ptr[ ebp + 16 ]
			push dword ptr[ ebp + 12 ]
			push dword ptr[ ebp + 8 ]
			call create_move
			pop ebx
			pop ebp
			retn 12
		}
	}

	void __fastcall draw_mdl_exec ( std::uintptr_t ecx, std::uintptr_t edx, void* ctx, const valve::draw_model_state_t& state, const valve::model_render_info_t& info, mat3x4_t* bones ) {

		if ( !info.m_renderable
			|| !valve::g_local_player
			|| !valve::g_local_player->alive ( ) )
			return orig_draw_mdl_exec ( ecx, edx, ctx, state, info, bones );

		const auto entity = reinterpret_cast< valve::c_entity* const >( info.m_renderable - 0x4 );

		if ( entity
			&& !entity->dormant ( )
			&& entity->is_player ( ) ) {
			if ( valve::g_local_player
				&& valve::g_local_player->alive ( ) ) {
				if ( entity == valve::g_local_player.operator nem::valve::c_player * ( ) ) {
					if ( hacks::g_aim_bot->cfg ( ).m_bBlend && valve___g_input->m_camera_in_third_person ) {
						if ( valve::g_local_player->scoped ( ) ) {
							valve::g_render_view->set_blend ( hacks::g_aim_bot->cfg ( ).m_iBlendValue / 100.f );
						}
					}
				}
			}
		}

		orig_draw_mdl_exec ( ecx, edx, ctx, state, info, bones );
	}

	void _cdecl cl_move ( float samples, bool final_tick )
	{
		thread_local bool pr_set = false;

		if ( !pr_set )
		{
			SetThreadPriorityBoost ( GetCurrentThread ( ), FALSE );
			SetThreadPriority ( GetCurrentThread ( ), THREAD_PRIORITY_HIGHEST );
			pr_set = true;
		}

		if ( !valve___g_engine->in_game ( ) || !valve___g_local_player || !valve___g_local_player->alive ( ) )
		{
			orig_cl_move ( samples, final_tick );
			return;
		}

		valve::global_vars_t backup = *valve___g_global_vars;
		auto cl_tick_count = valve___g_client_state->m_client_tick;
		auto server_tick = valve___g_client_state->m_server_tick;

		g_context->addresses ( ).m_cl_read_packets.get( ) ( final_tick );

		valve___g_client_state->m_client_tick = cl_tick_count;
		valve___g_client_state->m_server_tick = server_tick;
		*valve___g_global_vars = backup;

		valve::g_prediction->update(
			valve::g_client_state->m_delta_tick,
			valve::g_client_state->m_delta_tick > 0,
			valve::g_client_state->m_last_cmd_ack,
			valve::g_client_state->m_last_cmd_out + valve::g_client_state->m_choked_cmds
		);

		orig_cl_move ( samples, final_tick );
	}

	int process_interpolated_list ( )
	{
		static auto allow_to_extrp = *( bool** )( g_context->addresses ( ).m_allow_to_extrapolate.get( ) + 0x1 );

		if ( allow_to_extrp )
			*allow_to_extrp = false;

		return orig_process_interp_list ( );
	}

	void __stdcall create_move( const int seq_number, const float input_sample_frame_time, const bool active, bool& send_packet ) {
		thread_local bool pr_set = false;

		if ( !pr_set )
		{
			SetThreadPriorityBoost ( GetCurrentThread ( ), FALSE );
			SetThreadPriority ( GetCurrentThread ( ), THREAD_PRIORITY_HIGHEST );
			pr_set = true;
		}
		
		orig_create_move( valve___g_client, seq_number, input_sample_frame_time, active );

		send_packet = g_context->send_packet ( ) = true;

		hacks::g_movement->should_fake_duck( ) = false;

		if ( !valve___g_local_player )
			return;

		auto& user_cmd = valve___g_input->m_cmds[ seq_number % 150 ];
		if ( !user_cmd.m_number )
			return;

		const auto net_channel_info = valve___g_engine->net_channel_info( );
		if ( !net_channel_info )
			return;

		{
			auto& net_info = g_context->net_info( );

			net_info.m_lerp = std::max(
				g_context->cvars( ).m_cl_interp->get_float( ),
				g_context->cvars( ).m_cl_interp_ratio->get_float( ) / g_context->cvars( ).m_cl_updaterate->get_float( )
			);
			net_info.m_latency = { net_channel_info->latency( 1 ), net_channel_info->latency( 0 ) };
		}

		if ( !valve___g_local_player->alive( ) )
			return;

		g_context->left_create_move ( ) = false;

		if ( ( g_context->weapon( ) = valve___g_local_player->weapon( ) ) )
			g_context->wpn_data( ) = g_context->weapon( )->wpn_data( );
		else
			g_context->wpn_data( ) = nullptr;

		{
			static float prev_spawn_time = valve___g_local_player->spawn_time( );
			if ( prev_spawn_time != valve___g_local_player->spawn_time( ) ) {
				hacks::g_eng_pred->reset_on_spawn( );

				hacks::g_exploits->ticks_allowed( ) = 0;

				hacks::g_anim_sync->local_data ( ).reset ( );

				prev_spawn_time = valve___g_local_player->spawn_time( );
			}
		}

		valve::g_local_player->eye_angles ( ) = user_cmd.m_view_angles;
		valve::g_local_player->eye_angles ( ).z = 0;

		hacks::g_movement->should_fake_duck( )
			= hacks::g_aim_bot->cfg ( ).m_bFakeDuck
			&& valve___g_local_player->flags( ) & valve::e_ent_flags::on_ground
			&& !( user_cmd.m_buttons & valve::e_buttons::in_jump );

		g_context->defensive_allowed ( ) = true;
 
		hacks::g_eng_pred->old_user_cmd( ) = user_cmd;

		bool force_choke{};
		qangle_t wish_ang = hacks::g_movement->m_wish_ang = user_cmd.m_view_angles;
		bool break_lc{};
		if ( !hacks::g_exploits->try_to_recharge( ) ) {
			hacks::g_eng_pred->prepare( );

			hacks::g_exploits->manage_wpn( user_cmd );
			{
				lua_user_cmd_t lua_cmd {};

				lua_cmd.m_choked_cmds = valve::g_client_state->m_choked_cmds;
				lua_cmd.m_cmd_number = user_cmd.m_number;
				lua_cmd.m_tick_count = user_cmd.m_tick;
				lua_cmd.m_pitch = user_cmd.m_view_angles.x;
				lua_cmd.m_yaw = user_cmd.m_view_angles.y;
				lua_cmd.m_fwd_move = user_cmd.m_move.x;
				lua_cmd.m_side_move = user_cmd.m_move.y;
				lua_cmd.m_allow_send_packet = true;
#define ADD_FLAG( var ) lua_cmd.m_##var = user_cmd.m_buttons & valve::e_buttons::var
				ADD_FLAG ( in_attack ); ADD_FLAG ( in_jump ); ADD_FLAG ( in_duck );
				ADD_FLAG ( in_forward ); ADD_FLAG ( in_back ); ADD_FLAG ( in_use );
				ADD_FLAG ( in_cancel ); ADD_FLAG ( in_left ); ADD_FLAG ( in_right );
				ADD_FLAG ( in_move_left ); ADD_FLAG ( in_move_right ); ADD_FLAG ( in_attack2 );
				ADD_FLAG ( in_run ); ADD_FLAG ( in_reload ); ADD_FLAG ( in_alt1 );
				ADD_FLAG ( in_alt2 ); ADD_FLAG ( in_score ); ADD_FLAG ( in_speed );
				ADD_FLAG ( in_walk ); ADD_FLAG ( in_zoom ); ADD_FLAG ( in_weapon1 );
				ADD_FLAG ( in_weapon2 ); ADD_FLAG ( in_bullrush ); ADD_FLAG ( in_grenade1 );
				ADD_FLAG ( in_grenade2 ); ADD_FLAG ( in_attack3 );
#undef ADD_FLAG

				static const auto& callbacks = g_lua->events ( )[ xorstr_ ( "on_create_move" ) ];
				for ( const auto& callback : callbacks )
					callback.m_fn ( &lua_cmd );

				user_cmd.m_view_angles.x = lua_cmd.m_pitch;
				user_cmd.m_view_angles.y = lua_cmd.m_yaw;
				user_cmd.m_move.x = lua_cmd.m_fwd_move;
				user_cmd.m_move.y = lua_cmd.m_side_move;

				if ( !lua_cmd.m_allow_send_packet
					&& g_context->flags ( ) & e_context_flags::can_choke )
					g_context->flags ( ) |= e_context_flags::choke;

#define PROCESS_FLAG( var ) lua_cmd.m_##var ? user_cmd.m_buttons |= valve::e_buttons::var : user_cmd.m_buttons &= ~valve::e_buttons::var
				PROCESS_FLAG ( in_attack ); PROCESS_FLAG ( in_jump ); PROCESS_FLAG ( in_duck );
				PROCESS_FLAG ( in_forward ); PROCESS_FLAG ( in_back ); PROCESS_FLAG ( in_use );
				PROCESS_FLAG ( in_cancel ); PROCESS_FLAG ( in_left ); PROCESS_FLAG ( in_right );
				PROCESS_FLAG ( in_move_left ); PROCESS_FLAG ( in_move_right ); PROCESS_FLAG ( in_attack2 );
				PROCESS_FLAG ( in_run ); PROCESS_FLAG ( in_reload ); PROCESS_FLAG ( in_alt1 );
				PROCESS_FLAG ( in_alt2 ); PROCESS_FLAG ( in_score ); PROCESS_FLAG ( in_speed );
				PROCESS_FLAG ( in_walk ); PROCESS_FLAG ( in_zoom ); PROCESS_FLAG ( in_weapon1 );
				PROCESS_FLAG ( in_weapon2 ); PROCESS_FLAG ( in_bullrush ); PROCESS_FLAG ( in_grenade1 );
				PROCESS_FLAG ( in_grenade2 ); PROCESS_FLAG ( in_attack3 );
#undef PROCESS_FLAG
			}

			if ( g_context->weapon( ) && g_context->wpn_data( )
				&& !( g_context->flags( ) & e_context_flags::can_shoot )
				&& ( ( static_cast< std::size_t >( g_context->wpn_data( )->m_unk_type - 2 ) <= 5
					|| g_context->weapon( )->item_index( ) == valve::e_item_index::taser )
					&& g_context->weapon( )->item_index( ) != valve::e_item_index::revolver ) )
				user_cmd.m_buttons &= ~valve::e_buttons::in_attack;

			hacks::g_movement->on_create_move( user_cmd );

			hacks::g_movement->auto_peek ( wish_ang, user_cmd );

			hacks::g_visuals->on_create_move( user_cmd );

			hacks::g_eng_pred->process( &user_cmd, true );

			hacks::g_anti_aim->choke( &user_cmd );

			if ( g_context->flags( ) & e_context_flags::recharge ) {
				if ( !hacks::g_movement->should_fake_duck( )
					&& !hacks::g_exploits->shift( )
					&& !hacks::g_exploits->charged( ) )
					g_context->flags( ) &= ~e_context_flags::choke;

				user_cmd.m_buttons &= ~valve::e_buttons::in_attack;
			}
			
			hacks::g_aim_bot->on_create_move( user_cmd, send_packet );

			hacks::g_knife_bot->handle_knife_bot( user_cmd );

			if ( ( user_cmd.m_buttons & valve::e_buttons::in_attack )
				&& hacks::g_aim_bot->cfg ( ).m_bRemoveRecoil
				&& !( g_context->flags( ) & e_context_flags::aim_fire )
				&& g_context->flags( ) & e_context_flags::can_shoot ) {
				user_cmd.m_view_angles -= valve___g_local_player->aim_punch( ) * g_context->cvars( ).m_weapon_recoil_scale->get_float( );

				user_cmd.m_view_angles.x = std::remainder( user_cmd.m_view_angles.x, 360.f );
				user_cmd.m_view_angles.y = std::remainder( user_cmd.m_view_angles.y, 360.f );
				user_cmd.m_view_angles.z = std::remainder( user_cmd.m_view_angles.z, 360.f );

				user_cmd.m_view_angles.x = std::clamp( user_cmd.m_view_angles.x, -89.f, 89.f );
				user_cmd.m_view_angles.y = std::clamp( user_cmd.m_view_angles.y, -180.f, 180.f );
				user_cmd.m_view_angles.z = std::clamp( user_cmd.m_view_angles.z, -90.f, 90.f );
			}

			if ( hacks::g_exploits->is_shifting( ) )
				force_choke = true;

			if ( g_context->flags( ) & e_context_flags::can_shoot
				&& g_context->will_shoot( g_context->weapon( ), user_cmd ) ) {
				auto& anim_data = hacks::g_anim_sync->local_data( );

				anim_data.m_shot_cmd_number = user_cmd.m_number;

				if ( g_context->wpn_data( ) ) {
					if ( static_cast< std::size_t >( g_context->wpn_data( )->m_unk_type - 2 ) > 5 )
						anim_data.m_shot_valid_wpn = g_context->weapon( )->item_index( ) == valve::e_item_index::taser;
					else
						anim_data.m_shot_valid_wpn = true;
				}
				else
					anim_data.m_shot_valid_wpn = false;

				if ( anim_data.m_shot_valid_wpn ) {
					if ( !hacks::g_movement->should_fake_duck( ) ) {
						hacks::g_exploits->cur_shift_amount( ) = hacks::g_exploits->next_shift_amount( );

						if ( hacks::g_exploits->cur_shift_amount( ) ) {
							send_packet = g_context->send_packet ( ) = true;
							force_choke = true;
						}

						if ( hacks::g_exploits->force_choke( )
							&& g_context->flags( ) & e_context_flags::can_choke )
							g_context->flags( ) |= e_context_flags::choke;
						else
							g_context->flags( ) &= ~e_context_flags::choke;
					}

					if ( g_context->flags( ) & e_context_flags::aim_fire ) {
						const auto& aim_target = hacks::g_aim_bot->last_target( );

						if ( hacks::g_aim_bot->cfg( ).m_bShotRecord )
							hacks::g_visuals->add_shot_mdl( aim_target.m_entry->m_player, aim_target.m_lag_record );

						hacks::g_shots->add(
							g_context->shoot_pos( ), &aim_target,
							hacks::g_exploits->next_shift_amount( ), user_cmd.m_number, valve___g_global_vars->m_real_time, g_context->net_info ( ).m_latency.m_out + g_context->net_info ( ).m_latency.m_in
						);
					}
					else
						hacks::g_shots->add(
							g_context->shoot_pos( ), nullptr,
							hacks::g_exploits->next_shift_amount( ), user_cmd.m_number, valve___g_global_vars->m_real_time, g_context->net_info ( ).m_latency.m_out + g_context->net_info ( ).m_latency.m_in
						);
				}
			}

			if ( g_context->weapon( ) ) {
				g_context->weapon( )->recoil_index( ) = hacks::g_eng_pred->recoil_index( );
				g_context->weapon( )->accuracy_penalty( ) = hacks::g_eng_pred->accuracy_penalty( );
			}

			if ( !force_choke )
				send_packet = g_context->send_packet( ) = !( g_context->flags( ) & e_context_flags::choke );
			else {
				send_packet = g_context->send_packet( ) = true;
			}

			if ( valve___g_client_state->m_last_cmd_out != hacks::g_exploits->recharge_cmd( ) && g_context->weapon ( ) && 
				!g_context->weapon ( )->is_knife ( ) && hacks::g_exploits->is_peeking( wish_ang )
				&& ( hacks::g_exploits->type( ) == 2 || hacks::g_exploits->type( ) == 3 ) && g_context->defensive_allowed( ) ) {
				hacks::g_exploits->type( ) = 5;

				auto& local_data = hacks::g_eng_pred->local_data( ).at( user_cmd.m_number % 150 );

				local_data.m_override_tick_base = local_data.m_restore_tick_base = true;
				local_data.m_adjusted_tick_base = local_data.m_tick_base - hacks::g_exploits->next_shift_amount( ) - 1;

				break_lc = true;

				send_packet = g_context->send_packet( ) = true; // force send_packet for fake cmds process ( aka defensive double_tap )
			}

			hacks::g_anti_aim->set_pitch( user_cmd );

			hacks::g_movement->rotate(
				user_cmd, hacks::g_eng_pred->old_user_cmd( ).m_view_angles,
				valve___g_local_player->flags( ), valve___g_local_player->move_type( )
			);

			hacks::g_movement->normalize( user_cmd );

			hacks::g_anim_sync->update_local_real( user_cmd, send_packet );

			if ( send_packet )
				hacks::g_anim_sync->local_data ( ).m_fake_angle = user_cmd.m_view_angles;

			hacks::g_anim_sync->local_data ( ).m_real_angle = user_cmd.m_view_angles;
		}
		else {
			send_packet = g_context->send_packet ( ) = false;

			hacks::g_exploits->manage_wpn ( user_cmd );

			hacks::g_anim_sync->setup_local_bones( );

			user_cmd.m_tick = std::numeric_limits< int >::max( );

			auto& local_data = hacks::g_eng_pred->local_data( ).at( user_cmd.m_number % 150 );

			local_data.init( user_cmd );
			local_data.save( user_cmd.m_number );
		}

		hacks::g_movement->prev_view_yaw( ) = hacks::g_eng_pred->old_user_cmd( ).m_view_angles.y;

		hacks::g_movement->normalize( user_cmd );

		if ( !send_packet ) {
			auto& net_channel = valve___g_client_state->m_net_channel;

			const auto backup_choked_packets = net_channel->m_choked_packets;

			net_channel->m_choked_packets = 0;
			net_channel->send_datagram( 0 );
			--net_channel->m_out_seq_number;

			net_channel->m_choked_packets = backup_choked_packets;
		}
		else {
			g_context->broke_lc( ) = ( valve___g_local_player->origin( ) - g_context->last_sent_origin( ) ).length_2d_sqr( ) > crypt_float ( 4096.f );

			g_context->last_sent_origin( ) = valve___g_local_player->origin( );

			if ( !hacks::g_exploits->cur_shift_amount( )
				|| ( hacks::g_exploits->type( ) != 3 && hacks::g_exploits->type( ) != 4 ) )
				g_context->add_packet( user_cmd.m_number );

		
			if ( valve___g_client_state->m_last_cmd_out == hacks::g_exploits->recharge_cmd( ) ) {
				auto& local_data = hacks::g_eng_pred->local_data( ).at( user_cmd.m_number % 150 );

				local_data.m_override_tick_base = true;
				local_data.m_adjusted_tick_base = hacks::g_exploits->adjust_tick_base(
					valve___g_client_state->m_choked_cmds + 1, 1, -valve___g_client_state->m_choked_cmds
				);
			}
			if ( break_lc ) {
				hacks::g_exploits->type ( ) = crypt_int ( 5 );
				hacks::g_exploits->cur_shift_amount ( ) = hacks::g_exploits->next_shift_amount ( );
			}
		}

		hacks::g_eng_pred->restore ( );

		g_context->last_cmd_number( ) = user_cmd.m_number;

		hacks::g_exploits->charged( ) = false;

		bool has_exploits = hacks::g_exploits->type( ) == 3 || hacks::g_exploits->type( ) == 4 || break_lc;

		bool can_send_cmd_with_exploits = false;

		if ( has_exploits && !hacks::g_exploits->cur_shift_amount( ) 
			&& !( user_cmd.m_buttons & valve::e_buttons::in_attack ) )
			can_send_cmd_with_exploits = true;

		if ( user_cmd.m_tick != std::numeric_limits < float > ::max( )
			|| ( has_exploits && can_send_cmd_with_exploits ) ) {
			auto& out = g_context->get_out_cmds( ).emplace_back( );
			out.m_is_outgoing = send_packet;
			out.m_command_nr = user_cmd.m_number;
			out.m_is_used = false;
			out.m_prev_command_nr = 0;
		}
		while ( int( g_context->get_out_cmds( ).size( ) ) > int( 1.0f / valve___g_global_vars->m_interval_per_tick ) ) {
			g_context->get_out_cmds( ).pop_front( );
		}

		if ( user_cmd.m_tick != std::numeric_limits< int >::max( ) 
			&& send_packet ) {
			if ( !hacks::g_exploits->cur_shift_amount( )
				|| hacks::g_exploits->type( ) == 3
				|| hacks::g_exploits->type( ) == 4 )
				if ( valve::g_client_state->m_last_cmd_out != hacks::g_exploits->recharge_cmd( ) )
					g_context->last_sent_cmd_number ( ) = user_cmd.m_number;
		}

		if ( hacks::g_aim_bot->cfg ( ).m_bBuyBot && g_context->should_buy ( ) )
		{
			--g_context->should_buy ( );

			if ( !g_context->should_buy ( ) )
			{
				std::string buy;

				switch ( hacks::g_aim_bot->cfg ( ).m_iBuyBotSnipers )
				{
					case 1:
						buy += xorstr_ ( "buy g3sg1; " );
						break;
					case 2:
						buy += xorstr_ ( "buy awp; " );
						break;
					case 3:
						buy += xorstr_ ( "buy ssg08; " );
						break;
				}

				switch ( hacks::g_aim_bot->cfg ( ).m_iBuyBotPistols )
				{
					case 1:
						buy += xorstr_ ( "buy elite; " );
						break;
					case 2:
						buy += xorstr_ ( "buy deagle; buy revolver; " );
						break;
				}

				if ( hacks::g_aim_bot->cfg ( ).m_iBuyBotAdditional & 2 )
					buy += xorstr_ ( "buy vesthelm; buy vest; " );

				if ( hacks::g_aim_bot->cfg ( ).m_iBuyBotAdditional & 4 )
					buy += xorstr_ ( "buy taser; " );

				if ( hacks::g_aim_bot->cfg ( ).m_iBuyBotAdditional & 1 )
					buy += xorstr_ ( "buy molotov; buy hegrenade; buy smokegrenade; " );

				if ( hacks::g_aim_bot->cfg ( ).m_iBuyBotAdditional & 8 )
					buy += xorstr_ ( "buy defuser; " );

				valve::g_engine->exec_cmd ( buy.data ( ) );
			}
		}

		if ( g_context->flags( ) & e_context_flags::aim_fire
			|| user_cmd.m_buttons & valve::e_buttons::in_attack )
			hacks::g_anim_sync->local_data( ).m_last_shot_time = valve___g_global_vars->m_cur_time;

		hacks::g_eng_pred->last_user_cmd( ) = user_cmd;

		g_context->get_auto_peek_info( ).m_old_shot = g_context->get_auto_peek_info( ).m_is_firing;

		g_context->left_create_move ( ) = true;

		hacks::g_eng_pred->local_data ( ).at ( user_cmd.m_number % 150 ).m_move = user_cmd.m_move;

		valve___g_input->m_verified_cmds[ seq_number % 150 ] = { user_cmd, user_cmd.calc_checksum( ) };
	}

	void __stdcall frame_stage_notify ( const valve::e_frame_stage stage ) {
		hacks::g_eng_pred->last_frame_stage ( ) = stage;

		if ( !valve___g_local_player )
			return orig_frame_stage_notify ( stage );

		hacks::g_skins->on_frame_stage ( stage );

		const auto in_game = valve___g_engine->in_game ( );

		if ( in_game
			&& stage == valve::e_frame_stage::net_update_start ) {
		}

		if ( !valve::g_local_player
			|| !valve___g_engine->in_game ( ) ) {
			hacks::g_weather->reset_weather ( );
			hacks::g_weather->m_weather_created = false;
		}

		if ( stage == valve::e_frame_stage::render_start ) {

			hacks::g_visuals->on_render_start ( );
			hacks::g_exploits->skip_lag_interpolation ( false );

			if ( in_game )
			{
				hacks::g_visuals->change_sky_box ( );
				hacks::g_shots->on_net_update ( );

				static bool nigga_big_balls_monkey = false;

				if ( nigga_big_balls_monkey && !hacks::g_aim_bot->cfg ( ).m_bRain ) {
					hacks::g_weather->reset_weather ( );
					nigga_big_balls_monkey = false;
				}

				if ( !nigga_big_balls_monkey && hacks::g_aim_bot->cfg ( ).m_bRain ) {
					hacks::g_weather->m_weather_created = false;
					nigga_big_balls_monkey = true;
				}

				if ( nigga_big_balls_monkey )
					hacks::g_weather->update_weather( );

				for ( size_t i { 1 }; i <= valve___g_global_vars->m_max_clients; ++i ) {
					const auto player = static_cast < valve::c_player* > ( valve___g_entity_list->find_entity ( i ) );

					if ( !player ||
						!player->alive ( )
						|| player->dormant ( )
						|| player->friendly ( ) )
						continue;

					auto& entry = hacks::g_lag_comp->entry ( i - 1 );

					if ( entry.m_lag_records.empty ( ) )
						continue;

					if ( player->sim_time ( ) <= player->old_sim_time ( ) )
						continue;

					auto& var_mapping = player->var_mapping ( );

					for ( size_t j {}; j < var_mapping.m_interpolated_entries; ++j )
						var_mapping.m_entries.at ( j ).m_needs_to_interpolate = false;
				}
			}
		}

		orig_frame_stage_notify( stage );

		if ( stage == valve::e_frame_stage::render_start )
		{
			hacks::g_exploits->skip_lag_interpolation ( true );
		}

		if ( stage == valve::e_frame_stage::net_update_end ) {
			if ( in_game ) {

				if ( hacks::g_aim_bot->cfg ( ).m_bRagdolls ) {
					auto ent_index = valve___g_entity_list->highest_index ( );

					for ( std::size_t i { 1 }; i <= ent_index; ++i ) {
						const auto entity = valve___g_entity_list->find_entity ( i );

						if ( !entity )
							continue;

						const auto client_class = entity->client_class ( );

						if ( !client_class ||
							client_class->m_class_id != valve::e_class_id::ragdoll )
							continue;

						entity->use_fast_path ( ) = false;
					}
				}

				const auto correction_ticks = hacks::g_exploits->calc_correction_ticks( );
				if ( correction_ticks == -1 )
					hacks::g_exploits->correction_amount( ) = 0;
				else {
					if ( valve___g_local_player->sim_time( ) > valve___g_local_player->old_sim_time( ) ) {
						const auto delta = valve::to_ticks( valve___g_local_player->sim_time( ) ) - valve___g_client_state->m_server_tick;
						if ( std::abs( delta ) <= correction_ticks )
							hacks::g_exploits->correction_amount( ) = delta;
					}
				}

				valve___g_engine->fire_events( );

				hacks::g_eng_pred->velocity_modifier_to_data_map ( );
				hacks::g_eng_pred->third_person_recoil_to_data_map ( );

				hacks::g_lag_comp->on_net_update ( );
			}
		}
		else if ( stage == valve::e_frame_stage::render_end )
			hacks::g_visuals->on_render_end( );

		if ( in_game ) {
			if ( const auto view_model = valve___g_local_player->view_model( ) ) {
				if ( stage == valve::e_frame_stage::net_update_post_data_update_start ) {
					hacks::g_eng_pred->adjust_view_model ( ); // call it after orig_frame_stage so he won't interpolate view_model	
				}

			}
		}

		hacks::g_visuals->preserve_killfeed();
	}
	
	void __fastcall update_client_side_anim( valve::c_player* const ecx, const std::uintptr_t edx ) {
		if ( !ecx || !ecx->alive( )
			|| ecx->index( ) < 1
			|| ecx->index( ) > 64 )
			return orig_update_client_side_anim( ecx, edx );

		if ( ecx->team ( ) == valve::g_local_player->team ( ) )
			if ( ecx != valve::g_local_player.operator nem::valve::c_player * ( ) )
				return orig_update_client_side_anim ( ecx, edx );

		if ( !g_context->allow_anim_update( ) ) {
		
			if ( ecx->team ( ) != valve::g_local_player->team ( ) ) { 
				if ( ecx != valve::g_local_player )
				{
					auto& entry = hacks::g_lag_comp->entry ( ecx->index ( ) - 1 );
					for ( int i = 0; i < 128; i++ )
						entry.m_bones.at ( i ).set_origin ( ecx->abs_origin ( ) - entry.m_bone_origins.at ( i ) );

				}
			}
			else {
				const auto mdl_bone_counter = **reinterpret_cast< unsigned long** >(
					g_context->addresses( ).m_invalidate_bone_cache.get( ) + 0xau
					);

				static auto prev_mdl_bone_counter = ecx->mdl_bone_counter( );

				if ( mdl_bone_counter != prev_mdl_bone_counter )
					hacks::g_anim_sync->setup_local_bones( );

				prev_mdl_bone_counter = mdl_bone_counter;

			}

			ecx->attachment_helper ( );

			return;
		}

		for ( auto& layer : ecx->anim_layers( ) )
			layer.m_owner = ecx;

		orig_update_client_side_anim( ecx, edx );
	}

	bool __fastcall setup_bones(
		const std::uintptr_t ecx, const std::uintptr_t edx, mat3x4_t* const bones, int max_bones, int mask, float time
	) {
		const auto player = reinterpret_cast< valve::c_player* >( ecx - sizeof( std::uintptr_t ) );
		if ( !player || !player->alive( )
			|| player->index( ) < 1
			|| player->index( ) > 64 )
			return orig_setup_bones( ecx, edx, bones, max_bones, mask, time );


		if ( player->team ( ) == valve::g_local_player->team ( ) )
			if ( player != valve::g_local_player.operator nem::valve::c_player * ( ) )
				return orig_setup_bones ( ecx, edx, bones, max_bones, mask, time );

		if ( !g_context->allow_setup_bones( ) ) {
			if ( !bones 
				|| max_bones == -1 )
				return true;

			if ( player == valve___g_local_player ) {
				std::memcpy(
					bones, hacks::g_anim_sync->local_data( ).m_real.m_bones.data( ),
					std::min( max_bones, 128 ) * sizeof( mat3x4_t )
				);
			}
			else {
				const auto& entry = hacks::g_lag_comp->entry ( player->index ( ) - 1 );

				std::memcpy(
					bones, entry.m_bones.data( ),
					std::min( max_bones, 128 ) * sizeof( mat3x4_t )
				);
			}

			return true;
		}

		return orig_setup_bones( ecx, edx, bones, max_bones, mask, time );
	}

	void __fastcall do_extra_bones_processing(
		valve::c_player* const ecx, const std::uintptr_t edx, int a0, int a1, int a2, int a3, int a4, int a5
	) {}

	void __fastcall accumulate_layers(
		valve::c_player* const ecx, const std::uintptr_t edx, int a0, int a1, float a2, int a3
	) {
		if ( ecx->index( ) < 1
			|| ecx->index( ) > 64 )
			return orig_accumulate_layers( ecx, edx, a0, a1, a2, a3 );

		if ( const auto anim_state = ecx->anim_state( ) ) {
			const auto backup_first_update = anim_state->m_first_update;

			anim_state->m_first_update = true;

			orig_accumulate_layers( ecx, edx, a0, a1, a2, a3 );

			anim_state->m_first_update = backup_first_update;

			return;
		}

		orig_accumulate_layers( ecx, edx, a0, a1, a2, a3 );
	}

	void __fastcall on_latch_interpolated_vars (
		valve::c_player* const ecx, const std::uintptr_t edx, const int flags
	)
	{
		if ( !valve___g_engine->in_game ( )
			|| ecx != valve___g_local_player )
			return orig_on_latch_interpolated_vars ( ecx, edx, flags );

		const auto backup_sim_time = ecx->sim_time ( );

		const auto& local_data = hacks::g_eng_pred->local_data ( ).at ( valve___g_client_state->m_cmd_ack % 150 );
		if ( local_data.m_spawn_time == valve___g_local_player->spawn_time ( )
			&& local_data.m_shift_amount > 0 )
			ecx->sim_time ( ) = ecx->sim_time ( ) + ( local_data.m_shift_amount * valve___g_global_vars->m_interval_per_tick );

		orig_on_latch_interpolated_vars ( ecx, edx, flags );

		ecx->sim_time ( ) = backup_sim_time;
	}

	bool __fastcall is_bone_available ( void* ecx, std::ptrdiff_t bone_index ) {

		if ( g_context->is_bone_available ( ) )
			return true;

		return orig_is_bone_available ( ecx, bone_index );
	}

	void __fastcall standard_blending_rules(
		valve::c_player* const ecx, const std::uintptr_t edx, valve::model_data_t* const mdl_data, int a1, int a2, float a3, int mask
	) {
		if (!ecx || ecx->index( ) < 1
			|| ecx->index( ) > 64 )
			return orig_standard_blending_rules( ecx, edx, mdl_data, a1, a2, a3, mask );

		if ( g_context->force_bone_mask( ) )
			mask = 0x100;

		const auto bone_index = ecx->lookup_bone( xorstr_ ( "lean_root" ) );
		if ( bone_index == -1 )
			return orig_standard_blending_rules( ecx, edx, mdl_data, a1, a2, a3, mask );

		auto& bone_flags = mdl_data->m_bone_flags.at( bone_index );

		const auto backup_bone_flags = bone_flags;
		
		bone_flags = 0u;

		orig_standard_blending_rules( ecx, edx, mdl_data, a1, a2, a3, mask );

		bone_flags = backup_bone_flags;
	}

	bool __fastcall should_skip_anim_frame ( const std::uintptr_t ecx, const std::uintptr_t edx ) {
		const auto player = reinterpret_cast< valve::c_player* >( ecx - sizeof ( std::uintptr_t ) );
		if ( !player || !player->alive ( )
			|| player->index ( ) < 1
			|| player->index ( ) > 64
			|| !valve::g_local_player )
			return orig_should_skip_anim_frame ( ecx, edx );

		if ( player == valve::g_local_player
			|| player->team ( ) != valve::g_local_player->team ( ) )
			return false;
	}

	void __fastcall packet_start(
		const std::uintptr_t ecx, const std::uintptr_t edx, const int in_seq, const int out_acked
	) {
		if ( !valve___g_engine->in_game ( )
			|| !valve::g_local_player->alive ( ) )
			return orig_packet_start ( ecx, edx, in_seq, out_acked );

		if ( !g_context->get_out_cmds( ).empty( ) ) {
			for ( auto it = g_context->get_out_cmds( ).rbegin( ); it != g_context->get_out_cmds( ).rend( ); ++it ) {
				if ( !it->m_is_outgoing ) {
					continue;
				}

				if ( it->m_command_nr == out_acked
					|| out_acked > it->m_command_nr && ( !it->m_is_used || it->m_prev_command_nr == out_acked ) ) {

					it->m_prev_command_nr = out_acked;
					it->m_is_used = true;
					orig_packet_start( ecx, edx, in_seq, it->m_command_nr );

					break;
				}
			}

			auto result = false;

			for ( auto it = g_context->get_out_cmds( ).begin( ); it != g_context->get_out_cmds( ).end( ); ) {
				if ( out_acked == it->m_command_nr || out_acked == it->m_prev_command_nr )
					result = true;

				if ( out_acked > it->m_command_nr && out_acked > it->m_prev_command_nr ) {
					it = g_context->get_out_cmds( ).erase( it );
				}
				else {
					it++;
				}
			}

			if ( !result )
				orig_packet_start( ecx, edx, in_seq, out_acked );
		}
		else {
			orig_packet_start( ecx, edx, in_seq, out_acked );
		}
	}

	void __fastcall packet_end( const std::uintptr_t ecx, const std::uintptr_t edx ) {
		if ( !valve___g_local_player
			|| valve___g_client_state->m_server_tick != valve___g_client_state->m_delta_tick )
			return orig_packet_end( ecx, edx );

		const auto& local_data = hacks::g_eng_pred->local_data( ).at( valve___g_client_state->m_cmd_ack % 150 );
		if ( local_data.m_spawn_time == valve___g_local_player->spawn_time( )
			&& local_data.m_shift_amount > 0
			&& local_data.m_tick_base > valve___g_local_player->tick_base( )
			&& ( local_data.m_tick_base - valve___g_local_player->tick_base( ) ) <= 17 )
			valve___g_local_player->tick_base( ) = local_data.m_tick_base + 1;

		orig_packet_end( ecx, edx );
	}

	void __fastcall setup_move ( const std::uintptr_t ecx, const std::uintptr_t edx, valve::c_player* player, valve::user_cmd_t* user_cmd, valve::c_move_helper* move_helper, valve::move_data_t* move_data ) {	
		orig_setup_move ( ecx, edx, player, user_cmd, move_helper, move_data );
	}

	void __fastcall process_movement ( std::uintptr_t ecx, std::uintptr_t edx, valve::c_player* player, valve::move_data_t* move_data ) {

		move_data->m_game_code_moved_player = false;

		return orig_process_movement ( ecx, edx, player, move_data );
	}

	void __fastcall physics_simulate( valve::c_player* const ecx, const std::uintptr_t edx ) {
		if ( ecx != valve___g_local_player
			|| !ecx->alive( )
			|| ecx->sim_tick( ) == valve___g_global_vars->m_tick_count )
			return orig_physics_simulate( ecx, edx );

		auto& user_cmd = ecx->cmd_context( ).m_user_cmd;
		if ( user_cmd.m_tick >= std::numeric_limits< int >::max( ) ) {
			ecx->sim_tick( ) = valve___g_global_vars->m_tick_count;

			return hacks::g_eng_pred->net_vars( ).at( user_cmd.m_number % 150 ).store( user_cmd.m_number );
		}

		if ( user_cmd.m_tick >= ( valve___g_global_vars->m_tick_count + ( 1.f / valve___g_global_vars->m_interval_per_tick ) + 8 ) ) {
			user_cmd.m_predicted = true;
			return;
		}

		if ( user_cmd.m_number == ( valve___g_client_state->m_cmd_ack + 1 ) ) {
			ecx->velocity_modifier ( ) = hacks::g_eng_pred->net_velocity_modifier ( );
		}
		
		hacks::g_eng_pred->net_vars( ).at( ( user_cmd.m_number - 1 ) % 150 ).restore( user_cmd.m_number - 1 );

		const auto backup_tick_base = ecx->tick_base( );
			
		const auto& local_data = hacks::g_eng_pred->local_data( ).at( user_cmd.m_number % 150 );
		if ( local_data.m_spawn_time == ecx->spawn_time( ) && local_data.m_override_tick_base )
			ecx->tick_base( ) = local_data.m_adjusted_tick_base;

		const auto weapon = ecx->weapon( );
		if ( weapon
			&& weapon->item_index( ) == valve::e_item_index::revolver
			&& weapon->postpone_fire_ready_time( ) == std::numeric_limits< float >::max( ) )
			weapon->postpone_fire_ready_time( ) = hacks::g_eng_pred->postpone_fire_ready_time( );

		orig_physics_simulate( ecx, edx );

		if ( weapon
			&& weapon->item_index( ) == valve::e_item_index::revolver
			&& weapon->postpone_fire_ready_time( ) == std::numeric_limits< float >::max( ) )
			weapon->postpone_fire_ready_time( ) = hacks::g_eng_pred->postpone_fire_ready_time( );

		if ( local_data.m_spawn_time == ecx->spawn_time ( )
			&& local_data.m_override_tick_base && local_data.m_restore_tick_base ) {
			ecx->tick_base ( ) = backup_tick_base + ecx->tick_base ( ) - local_data.m_adjusted_tick_base;
		}

		hacks::g_eng_pred->net_vars( ).at( user_cmd.m_number % 150 ).store( user_cmd.m_number );

		hacks::g_eng_pred->save_view_model( );
	}

	bool __fastcall should_interpolate( valve::c_player* const ecx, const std::uintptr_t edx ) {

		if ( hacks::g_exploits->type ( ) == 5 && ecx == valve___g_local_player )
		{
			auto& var_mapping = ecx->var_mapping ( );

			for ( int i {}; i < var_mapping.m_interpolated_entries; ++i )
				var_mapping.m_entries.at ( i ).m_needs_to_interpolate = false;

			return false;
		}

		if ( ecx != valve___g_local_player
			|| valve___g_client_state->m_last_cmd_out != hacks::g_exploits->recharge_cmd( ) )
			return orig_should_interpolate( ecx, edx );

		auto& var_mapping = ecx->var_mapping( );

		for ( int i{}; i < var_mapping.m_interpolated_entries; ++i )
			var_mapping.m_entries.at( i ).m_needs_to_interpolate = false;

		return false;
	}

	bool __fastcall write_user_cmd_delta_to_buffer(
		const std::uintptr_t ecx, const std::uintptr_t edx,
		const int slot, valve::bf_write_t* const buffer,
		int from, int to, const bool is_new_cmd
	) {
		if ( !valve___g_local_player )
			return orig_write_user_cmd_delta_to_buffer( ecx, edx, slot, buffer, from, to, is_new_cmd );

		const auto move_msg = reinterpret_cast< valve::move_msg_t* >( 
			*reinterpret_cast< std::uintptr_t* >(
				reinterpret_cast< std::uintptr_t >( _AddressOfReturnAddress( ) ) - sizeof( std::uintptr_t )
			) - 0x58u
		);

		if ( hacks::g_exploits->cur_shift_amount( )
			|| valve___g_client_state->m_last_cmd_out == hacks::g_exploits->recharge_cmd( ) ) {
			if ( from == -1 ) {
				if ( valve___g_client_state->m_last_cmd_out == hacks::g_exploits->recharge_cmd( ) ) {
					move_msg->m_new_cmds = 1;
					move_msg->m_backup_cmds = 0;

					const auto next_cmd_number = valve___g_client_state->m_choked_cmds + valve___g_client_state->m_last_cmd_out + 1;

					for ( to = next_cmd_number - move_msg->m_new_cmds + 1; to <= next_cmd_number; ++to ) {
						if ( !orig_write_user_cmd_delta_to_buffer( ecx, edx, slot, buffer, from, to, true ) )
							break;

						from = to;
					}
				}
				else if ( hacks::g_exploits->type ( ) == 5 )
				{
					hacks::g_exploits->handle_break_lc ( ecx, edx, slot, buffer, from, to, move_msg );
					return 1;
				}
				else
					hacks::g_exploits->process_real_cmds ( ecx, edx, slot, buffer, from, to, move_msg );
			}

			return true;
		}
		else
		{
			if ( from == -1 ) {
				const auto v86 = std::min ( move_msg->m_new_cmds + hacks::g_exploits->ticks_allowed ( ), 16 );

				int v12 {};

				const auto v70 = v86 - move_msg->m_new_cmds;
				if ( v70 >= 0 )
					v12 = v70;

				hacks::g_exploits->ticks_allowed ( ) = v12;
			}
		}

		return orig_write_user_cmd_delta_to_buffer( ecx, edx, slot, buffer, from, to, is_new_cmd );
	}

	void __cdecl velocity_modifier( valve::recv_proxy_data_t* const data, valve::c_entity* const entity, void* const out ) {
		orig_velocity_modifier( data, entity, out );

		if ( !valve___g_local_player
			|| entity->index( ) != valve___g_local_player->index( )
			|| data->m_value.m_float == hacks::g_eng_pred->net_velocity_modifier( ) )
			return;

		hacks::g_eng_pred->net_velocity_modifier( ) = data->m_value.m_float;

		valve___g_prediction->m_prev_ack_had_errors = true;
	}

	void __cdecl sequence( valve::recv_proxy_data_t* const data, valve::c_entity* const entity, void* const out ) {
		if ( valve___g_local_player
			&& valve___g_local_player->alive( ) )
				if ( const auto owner = valve___g_entity_list->find_entity( entity->view_model_owner( ) ) )
					if ( owner->index( ) == valve___g_local_player->index( ) )
						if ( const auto weapon = static_cast< valve::c_weapon* >( valve___g_entity_list->find_entity( entity->view_model_weapon_handle( ) ) ) )
							if ( const auto client_class = weapon->client_class( ); client_class && client_class->m_class_id == valve::e_class_id::knife )
								data->m_value.m_int = hacks::g_skins->correct_sequence( weapon->item_index( ), data->m_value.m_int );

		orig_sequence( data, entity, out );
	}

	void __fastcall modify_eye_pos( valve::anim_state_t* const ecx, const std::uintptr_t edx, vec3_t& eye_pos ) {
		ecx->m_smooth_height_valid = false;

		if ( g_context->left_create_move ( ) )
			return orig_modify_eye_pos ( ecx, edx, eye_pos );
		else
			return;
	}

	void __fastcall check_for_seq_change ( std::uintptr_t ecx, std::uintptr_t edx, std::uintptr_t hdr, int cur_seq, bool force_new_seq, bool interp ) {
		return orig_check_for_seq_change ( ecx, edx, hdr, cur_seq, force_new_seq, false );
	}

	bool __fastcall process_temp_entities( const std::uintptr_t ecx, const std::uintptr_t edx, const std::uintptr_t msg ) {
		const auto backup_max_clients = valve___g_client_state->m_max_clients;

		valve___g_client_state->m_max_clients = 1;

		const auto ret = orig_process_temp_entities( ecx, edx, msg );

		valve___g_client_state->m_max_clients = backup_max_clients;

		valve___g_engine->fire_events( );

		return ret;
	}

	void __fastcall level_shutdown( const std::uintptr_t ecx, const std::uintptr_t edx ) {
		hacks::g_eng_pred->reset_on_spawn( );

		hacks::g_shots->elements( ).clear( );

		hacks::g_exploits->recharge_cmd( ) = 
			hacks::g_exploits->ticks_allowed( ) =
				hacks::g_exploits->cur_shift_amount( ) = 
					hacks::g_exploits->next_shift_amount( ) = 0;

		g_context->freeze_time( ) = false;

		orig_level_shutdown( ecx, edx );
	}

	bool __cdecl glow_effect_spectator(
		valve::c_player* const player, valve::c_player* const local,
		int& style, vec3_t& clr, float& alpha_from, float& alpha_to,
		float& time_from, float& time_to, bool& animate
	) {
		if ( !hacks::g_aim_bot->cfg ( ).m_bGlow
			|| player->friendly( ) )
			return orig_glow_effect_spectator(
				player, local, style, clr, alpha_from, alpha_to,
				time_from, time_to, animate
			);

		style = 0;

		clr.x = hacks::g_aim_bot->cfg ( ).m_flGlowColor[ 0u ];
		clr.y = hacks::g_aim_bot->cfg ( ).m_flGlowColor[ 1u ];
		clr.z = hacks::g_aim_bot->cfg ( ).m_flGlowColor[ 2u ];

		alpha_to = hacks::g_aim_bot->cfg ( ).m_flGlowColor[ 3u ];

		return true;
	}

	void __fastcall paint_traverse ( void* ecx, void* edx, uint32_t id, bool force_repaint, bool allow_force )
	{
		std::string panel_name = valve___g_panel->get_name ( id );
		orig_paint_traverse ( ecx, edx, id, force_repaint, allow_force );

		if ( !panel_name.empty ( ) && !panel_name.compare ( xorstr_ ( "MatSystemTopPanel" ) ) ) {

			render::handle_drawings ( );
		}
	}

	void __fastcall clamp_bones_in_box ( valve::c_player* ecx, void* edx, mat3x4_t* matrix, int mask ) {
		return orig_clamp_bones_in_box ( ecx, edx, matrix, mask );
	}

	void __fastcall run_cmd ( std::uintptr_t ecx, std::uintptr_t edx, valve::c_player* player, valve::user_cmd_t* user_cmd, valve::c_move_helper* move_helper ) {
		orig_run_cmd ( ecx, edx, player, user_cmd, move_helper );
	}

	void __fastcall get_clr_modulation(
		valve::material_t* const ecx, const std::uintptr_t edx,
		float* const r, float* const g, float* const b
	) {
		orig_get_clr_modulation( ecx, edx, r, g, b );

		const auto texture_group_name = ecx->texture_group( );
		const auto prop_mat = *reinterpret_cast< const std::uint32_t* >( texture_group_name + 8u ) == 't po';


		if ( hacks::g_aim_bot->cfg ( ).m_iWorldType != 2 )
			return;

		if ( !prop_mat
			&& *reinterpret_cast< const std::uint32_t* >( texture_group_name + 3u ) != 't dl' )
			return;

		*r *= hacks::g_aim_bot->cfg ( ).m_flNightModeColor[ 0u ];
		*g *= hacks::g_aim_bot->cfg ( ).m_flNightModeColor[ 1u ];
		*b *= hacks::g_aim_bot->cfg ( ).m_flNightModeColor[ 2u ];
	}

	bool __stdcall is_depth_something_in_nigga_ass ( ) {
		hacks::g_visuals->motion_blur ( nullptr );
		return false;
	}

	bool __stdcall is_using_static_prop_debug_modes( ) {

		return hacks::g_aim_bot->cfg ( ).m_iWallTransparency != 100
			|| hacks::g_aim_bot->cfg ( ).m_iPropTransparency != 100
			|| hacks::g_aim_bot->cfg ( ).m_iWorldType == 2;
	}

	int __fastcall list_leaves_in_box(
		const std::uintptr_t ecx, const std::uintptr_t edx,
		const vec3_t& mins, const vec3_t& maxs, const uint16_t* const list, const int max
	) {

		if ( !valve___g_local_player )
			return orig_list_leaves_in_box ( ecx, edx, mins, maxs, list, max );

		if ( *( uint32_t* )_AddressOfReturnAddress ( ) != g_context->addresses ( ).m_ret_insert_into_tree.get( ) )
			return orig_list_leaves_in_box( ecx, edx, mins, maxs, list, max );

		struct renderable_info_t {
			valve::i_client_renderable*	m_renderable{};
			std::uintptr_t	m_alpha_property{};
			int				m_enum_count{};
			int				m_render_frame{};
			std::uint16_t	m_first_shadow{};
			std::uint16_t	m_leaf_list{};
			short			m_area{};
			std::uint16_t	m_flags0{};
			std::uint16_t	m_flags1{};
			vec3_t			m_bloated_abs_min{};
			vec3_t			m_bloated_abs_max{};
			vec3_t			m_abs_min{};
			vec3_t			m_abs_max{};
			char			pad0[ 4u ]{};
		};

		const auto info = *reinterpret_cast< renderable_info_t** >(
			reinterpret_cast< std::uintptr_t >( _AddressOfReturnAddress( ) ) + 0x14u
		);
		if ( !info
			|| !info->m_renderable )
			return orig_list_leaves_in_box( ecx, edx, mins, maxs, list, max );

		const auto entity = info->m_renderable->get_client_unknown ( )->get_base_entity ( );
		if ( !entity
			|| !entity->is_player( ) )
			return orig_list_leaves_in_box( ecx, edx, mins, maxs, list, max );

		info->m_flags0 &= ~0x100;
		info->m_flags1 |= 0xC0;

		return orig_list_leaves_in_box(
			ecx, edx,
			{ -16384.f, -16384.f, -16384.f },
			{ 16384.f, 16384.f, 16384.f },
			list, max
		);
	}

	bool __fastcall override_cfg(
		valve::c_mat_system* const ecx, const std::uintptr_t edx,
		valve::mat_sys_cfg_t* const cfg, const bool update
	) {
		if ( hacks::g_aim_bot->cfg ( ).m_iWorldType == 1 )
			cfg->m_fullbright = true;

		return orig_override_cfg( ecx, edx, cfg, update );
	}

	bool __fastcall fog_enabled( const std::uintptr_t ecx, const std::uintptr_t edx ) {
		if ( hacks::g_aim_bot->cfg ( ).m_bRemoveFog )
			return false;

		return orig_fog_enabled( ecx, edx );
	}

	void __fastcall override_view(
		const std::uintptr_t ecx, const std::uintptr_t edx, valve::view_setup_t* const setup
	) {
		hacks::g_visuals->on_override_view( setup );

		orig_override_view( ecx, edx, setup );

		hacks::g_visuals->motion_blur ( setup );
	}

	int __fastcall do_post_screen_space_effects(
		const std::uintptr_t ecx, const std::uintptr_t edx, valve::view_setup_t* const setup
	) {
		hacks::g_visuals->on_post_screen_effects( );

		return orig_do_post_screen_space_effects( ecx, edx, setup );
	}

	int __fastcall weapon_debug_spread_show_get_int( valve::cvar_t* const ecx, const std::uintptr_t edx ) {
	}

	void __fastcall calc_view(
		valve::c_player* const ecx, const std::uintptr_t edx,
		vec3_t& origin, qangle_t& angles, float& z_near, float& z_far, float& fov
	) {
		if ( ecx != valve___g_local_player )
			return orig_calc_view( ecx, edx, origin, angles, z_near, z_far, fov );

		hacks::g_visuals->on_calc_view( ecx, edx, origin, angles, z_near, z_far, fov );
	}

	bool __stdcall in_prediction ( )
	{
		if ( g_context->allow_setup_bones ( ) && *reinterpret_cast< std::uintptr_t* >( _AddressOfReturnAddress ( ) ) == g_context->addresses ( ).m_ret_to_maintain_seq_transitions.get( ) ) // maintain seq trans leave please ^^
			return true;

		return orig_in_prediction ( valve___g_prediction );
	}

	qangle_t* __fastcall get_eye_angles( valve::c_player* const ecx, const std::uintptr_t edx ) {
		return orig_get_eye_angles ( ecx, edx );
	}

	bool __fastcall is_paused( const std::uintptr_t ecx, const std::uintptr_t edx ) {
		if ( *reinterpret_cast< std::uintptr_t* >( _AddressOfReturnAddress( ) ) == g_context->addresses( ).m_ret_to_extrapolation.get( ) )
			return true;

		return orig_is_paused( ecx, edx );
	}

	bool __fastcall is_hltv(const std::uintptr_t ecx, const std::uintptr_t edx) {

		if (g_context->allow_setup_bones())
			return true;

		if (g_context->allow_anim_update())
			return true;

		if (*reinterpret_cast<std::uintptr_t*>(_AddressOfReturnAddress()) == g_context->addresses().m_ret_to_accumulate_layers.get( ) ||
			*reinterpret_cast<std::uintptr_t*>(_AddressOfReturnAddress()) == g_context->addresses().m_ret_to_setup_velocity.get( ) )
			return true;

		return orig_is_hltv(ecx, edx);
	}

	float __stdcall aspect_ratio(int width, int height)
	{
		if (!hacks::g_aim_bot->cfg ( ).m_bAspectRatio)
			return orig_aspect_ratio(width, height);
		else
			return hacks::g_aim_bot->cfg ( ).m_iAspectRatioValue / 10.f;
	}

	void __fastcall calc_viewmodel_bob ( valve::c_player* ecx, const std::uintptr_t edx, vec3_t& view_bob )
	{
		if ( ecx != valve___g_local_player || !hacks::g_aim_bot->cfg ( ).m_bRemoveLandingBob )
			return orig_viewmodel_bob ( ecx, edx, view_bob );
	}

	const char* __fastcall get_foreign_fall_back_font_name ( void* ecx, std::uint32_t i )
	{
		if ( hacks::g_visuals->m_fonts.m_last_font.empty ( ) )
			return orig_i_hate_niggas ( ecx, i );

		return hacks::g_visuals->m_fonts.m_last_font.c_str ( );
	}
}