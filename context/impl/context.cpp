#include "../../nem.hpp"

//#include "..\..\init_security\init_security.h"
//#include "..\..\patterns_security\patterns_security.h"
#include "..\..\allah.h"

#include "..\..\menu\NL\Neverlose.h"


int __stdcall CheatInit( _In_ HINSTANCE instance, _In_ LPVOID reserved )
{
	return 1;
}

int __stdcall DllMain( _In_ HINSTANCE instance, _In_ DWORD reason, _In_ LPVOID reserved ) {

	if ( reason != DLL_PROCESS_ATTACH )
		return 0;

	AllocConsole( );

	FILE* file{};
	freopen_s( &file, "CONOUT$", "w", stdout );

	std::jthread { []( ) {
		nem::g_context->init( );
	} }.detach( );


	return 1;
}



std::uintptr_t find_byte_seq( const nem::code_section_t& section, const std::string& seq ) {
	using seq_byte_t = std::pair< std::uint8_t, bool >;

	std::string str{};
	std::vector< seq_byte_t > byte_seq{};

	std::stringstream stream{ seq };
	while ( stream >> str ) {
		if ( str[0u] == '?' ) {
			byte_seq.emplace_back( 0u, true );

			continue;
		}

		if ( !std::isxdigit( str[0u] )
			|| !std::isxdigit( str[1u] ) )
			continue;

		byte_seq.emplace_back(
			static_cast< std::uint8_t >(
				std::strtoul( str.data( ), nullptr, 16 )
				), false
		);
	}

	const auto end = reinterpret_cast< std::uint8_t* >( section.m_base + section.m_size );

	const auto ret = std::search(
		reinterpret_cast< std::uint8_t* >( section.m_base ), end, byte_seq.begin( ), byte_seq.end( ),
		[ ]( const std::uint8_t byte, const seq_byte_t& seq_byte ) {
			return std::get< bool >( seq_byte ) || byte == std::get< std::uint8_t >( seq_byte );
		}
	);

	return reinterpret_cast< std::uintptr_t >( ret );
}


namespace nem {

	template< class T >
	T* get_interface ( std::string module_name, std::string interface_name, bool skip = false )
	{
		if ( module_name.empty ( ) || interface_name.empty ( ) )
			return nullptr;
		typedef PVOID ( *CreateInterfaceFn )( const char* pszName, int* piReturnCode );
		CreateInterfaceFn hInterface = nullptr;
		while ( !hInterface )
		{
			hInterface = ( CreateInterfaceFn )GetProcAddress ( GetModuleHandleA ( module_name.c_str ( ) ), "CreateInterface" );
			Sleep ( 5 );
		}

		char pszBuffer[ 256 ];
		for ( int i = 0; i < 100; i++ )
		{
			sprintf_s ( pszBuffer, "%s%0.3d", interface_name.c_str ( ), i );
			PVOID pInterface = hInterface ( pszBuffer, nullptr );

			if ( pInterface && pInterface != NULL )
			{
				if ( skip )
					sprintf_s ( pszBuffer, "%s%0.3d", interface_name.c_str ( ), i + 1 );

				Sleep ( 5 );
				break;
			}
		}

		return ( T* )hInterface ( pszBuffer, nullptr );
	}
	void c_context::init ( )
	{
		while ( !GetModuleHandle ( xorstr_ ( "serverbrowser.dll" ) ) )
			std::this_thread::sleep_for ( std::chrono::milliseconds { 200 } );

		if ( MH_Initialize ( ) != MH_OK )
			return;

		const auto d3d9_device = **reinterpret_cast< IDirect3DDevice9*** >(
			find_byte_seq( GetModuleHandle ( xorstr_ ( "shaderapidx9.dll" ) ), "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) + 1u
			);


		SetPriorityClass ( GetCurrentProcess ( ), HIGH_PRIORITY_CLASS );

		/* init imgui */
		{
			D3DDEVICE_CREATION_PARAMETERS creation_params {};
			if ( d3d9_device->GetCreationParameters ( &creation_params ) != D3D_OK )
				return;

			ImGui::CreateContext ( );
			ImGui::StyleColorsDark ( );

			/*ImGui::GetStyle( ).WindowMinSize = { 450, 350 };*/

			auto& imgui_io = ImGui::GetIO ( );

			imgui_io.IniFilename = imgui_io.LogFilename = nullptr;


			ImGui_ImplWin32_Init ( creation_params.hFocusWindow );
			ImGui_ImplDX9_Init ( d3d9_device );

			render::m_draw_list = std::make_shared<ImDrawList> ( ImGui::GetDrawListSharedData ( ) );
			render::m_data_draw_list = std::make_shared<ImDrawList> ( ImGui::GetDrawListSharedData ( ) );
			render::m_replace_draw_list = std::make_shared<ImDrawList> ( ImGui::GetDrawListSharedData ( ) );
			ImFontConfig cfg;

			static const ImWchar ranges [ ] =
			{
				0x0020, 0x00FF, // Basic Latin + Latin Supplement
				0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
				0x2DE0, 0x2DFF, // Cyrillic Extended-A
				0xA640, 0xA69F, // Cyrillic Extended-B
				0xE000, 0xE226, // icons
				0,
			};

			cfg.GlyphRanges = ranges;
			cfg.RasterizerFlags = ImGuiFreeType::RasterizerFlags::ForceAutoHint;
			cfg.RasterizerMultiply = 1.2f;
			cfg.OversampleH = cfg.OversampleV = 3;
			cfg.PixelSnapH = false;
			auto& io = ImGui::GetIO ( );

			g_NL->InitializeFonts( );

			hacks::g_visuals->m_fonts.m_tahoma14 = io.Fonts->AddFontFromFileTTF( xorstr_( "C:\\Windows\\Fonts\\Tahoma.ttf" ), 14.f, &cfg );

			hacks::g_visuals->m_fonts.m_verdana = io.Fonts->AddFontFromFileTTF( xorstr_( "C:\\Windows\\Fonts\\Verdana.ttf" ), 12.f, &cfg );
			hacks::g_visuals->m_fonts.m_icon_font = io.Fonts->AddFontFromMemoryCompressedBase85TTF( WeaponIcons_compressed_data_base85, 16.f, &cfg );
			hacks::g_visuals->m_fonts.m_warning_icon_font = io.Fonts->AddFontFromMemoryCompressedBase85TTF( WeaponIcons_compressed_data_base85, 24.f, &cfg );
			hacks::g_visuals->m_fonts.m_log_font = io.Fonts->AddFontFromFileTTF( xorstr_( "C:/windows/fonts/seguisb.ttf" ), 16.0f, &cfg );
			hacks::g_visuals->m_fonts.m_mojesh_ubrat_ee_ya_razreshayu = io.Fonts->AddFontFromFileTTF( xorstr_( "C:/windows/fonts/seguisb.ttf" ), 30.0f, &cfg );
			hacks::g_visuals->m_fonts.m_log_icons = io.Fonts->AddFontFromMemoryCompressedTTF( Icons_compressed_data123, Icons_compressed_size123, 15.f, &cfg );
			hacks::g_visuals->m_fonts.m_museo_sans_name = io.Fonts->AddFontFromMemoryTTF( museo_sans_esp_name, sizeof( museo_sans_esp_name ), 14.f, &cfg );
			hacks::g_visuals->m_fonts.m_segoe_ui_esp = io.Fonts->AddFontFromFileTTF( xorstr_( "C:/windows/fonts/seguisb.ttf" ), 13.0f, &cfg );
			hacks::g_visuals->m_fonts.m_xxxtentacion_icons = io.Fonts->AddFontFromMemoryTTF( xxx_tentacion_icons, sizeof( xxx_tentacion_icons ), 12.f, &cfg );
			hacks::g_visuals->m_fonts.m_skeet_font_esp = io.Fonts->AddFontFromMemoryTTF( _04B03, sizeof( _04B03 ), 8.f, &cfg );
			hacks::g_visuals->m_fonts.m_04b = io.Fonts->AddFontFromMemoryTTF( _04B03, sizeof( _04B03 ), 8.f, &cfg );
			hacks::g_visuals->m_fonts.m_smallest_pixel = io.Fonts->AddFontFromMemoryTTF( smallest_pixel_7, sizeof( smallest_pixel_7 ), 7.f, &cfg );
			hacks::g_visuals->m_fonts.m_menu_icons_fontik = io.Fonts->AddFontFromMemoryTTF( ( void* ) ya_sobaka_ti_soabaka, sizeof( ya_sobaka_ti_soabaka ), 19.f, &cfg, ranges );
			hacks::g_visuals->m_fonts.m_visitor_tt2_brk = io.Fonts->AddFontFromMemoryTTF( _visitor_tt2_brk, sizeof( _visitor_tt2_brk ), 10.f, &cfg );

			ImGuiFreeType::BuildFontAtlas( io.Fonts );

			g_menu->init ( d3d9_device );

			hooks::orig_wnd_proc = reinterpret_cast< WNDPROC >(
				SetWindowLong (
					creation_params.hFocusWindow, GWL_WNDPROC,
					reinterpret_cast< LONG >( hooks::wnd_proc )
				)
				);
		}

		g_lua->init ( );

		const auto cl_code_section = code_section_t { GetModuleHandle ( xorstr_ ( "client.dll" ) ) };
		const auto eng_code_section = code_section_t { GetModuleHandle ( xorstr_ ( "engine.dll" ) ) };
		const auto vgui_code_section = code_section_t { GetModuleHandle ( xorstr_ ( "vguimatsurface.dll" ) ) };
		const auto vgui2_code_section = code_section_t { GetModuleHandle ( xorstr_ ( "vgui2.dll" ) ) };
		const auto server_code_section = code_section_t { GetModuleHandle ( xorstr_ ( "server.dll" ) ) };
		const auto studio_render_code_section = code_section_t { GetModuleHandle ( xorstr_ ( "studiorender.dll" ) ) };
		const auto material_system_code_section = code_section_t { GetModuleHandle ( xorstr_ ( "materialsystem.dll" ) ) };
		const auto shader_pidx9_code_section = code_section_t { GetModuleHandle ( xorstr_ ( "shaderapidx9.dll" ) ) };
		const auto vstdlib_code_section = code_section_t { GetModuleHandle ( xorstr_ ( "vstdlib.dll" ) ) };

		code_section_t fart[ ] { cl_code_section, eng_code_section, server_code_section,
			studio_render_code_section, material_system_code_section, shader_pidx9_code_section, vstdlib_code_section, vgui_code_section };
		long long amongus = 0x69690004C201B0;
		for ( auto sex : fart )
			WriteProcessMemory ( GetCurrentProcess ( ), ( LPVOID ) find_byte_seq( sex, "55 8B EC 56 8B F1 33 C0 57 8B 7D 08" ), &amongus, 7, 0 );

#ifdef _DEBUG
		std::printf ( "initializing valve interfaces...\n" );
#endif

		/* init valve stuff */
		{
			valve::g_local_player = *reinterpret_cast< valve::c_local_player* >(
				find_byte_seq( cl_code_section, ( "8B 0D ? ? ? ? 83 FF FF 74 07" ) ) + 0x2u
				);

			valve::g_client = *reinterpret_cast< valve::c_client** >(
				find_byte_seq( cl_code_section, ( "A1 ? ? ? ? B9 ? ? ? ? FF 90 ? ? ? ? 8B 35 ? ? ? ?" ) ) + 0x1u
				);

			valve::g_panel = get_interface < valve::c_panel > ( "vgui2.dll", "VGUI_Panel" );

			valve::g_effects = get_interface < valve::c_effects > ( "engine.dll", "VEngineEffects" );

			valve::g_engine_sound = get_interface < valve::engine_sound_t > ( "engine.dll", "IEngineSoundClient" );

			valve::g_render_view = get_interface < valve::render_view_t > ( "engine.dll", "VEngineRenderView" );

			valve::g_global_vars = **reinterpret_cast< valve::global_vars_t*** >(
				( *reinterpret_cast< std::uintptr_t** >( valve::g_client ) )[ 11u ] + 0xau
				);

			valve::g_engine = **reinterpret_cast< valve::c_engine*** >(
				find_byte_seq( cl_code_section, ( "8B 0D ? ? ? ? 8B 01 8B 40 38 FF" ) ) + 0x2u
				);

			valve::g_entity_list = **reinterpret_cast< valve::c_entity_list*** >(
				find_byte_seq( eng_code_section, ( "8B 0D ? ? ? ? 88 45 FC 6A 00" ) ) + 0x2u
				);

			valve::g_client_state = **reinterpret_cast< valve::client_state_t*** >(
				find_byte_seq( eng_code_section, ( "A1 ? ? ? ? 8B 88 ? ? ? ? 85 C9 75 07" ) ) + 0x1u
				);

			valve::g_move_helper = **reinterpret_cast< valve::c_move_helper*** >(
				find_byte_seq( cl_code_section, ( "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ) ) + 0x2u
				);

			valve::g_prediction = *reinterpret_cast< valve::prediction_t** >(
				find_byte_seq( cl_code_section, ( "A1 ? ? ? ? 8B 40 38 FF D0 84 C0" ) ) + 0x1u
				);

			valve::g_movement = *reinterpret_cast< valve::c_movement** >(
				find_byte_seq( cl_code_section, ( "A1 ? ? ? ? B9 ? ? ? ? FF 50 04 EB 07" ) ) + 0x1u
				);

			valve::g_cvar = **reinterpret_cast< valve::c_cvar*** >(
				find_byte_seq( cl_code_section, ( "8B 0D ? ? ? ? 68 ? ? ? ? 8B 01 FF 50 44 8B C8 89" ) ) + 0x2u
				);

			valve::g_engine_trace = **reinterpret_cast< valve::c_engine_trace*** >(
				find_byte_seq( cl_code_section, ( "A1 ? ? ? ? F3 0F 11 45 ? 8B 30" ) ) + 0x1u
				);

			valve::g_surface_data = **reinterpret_cast< valve::c_surface_data*** >(
				find_byte_seq( cl_code_section, ( "8B 0D ? ? ? ? 0F BF 45 C8" ) ) + 0x2u
				);

			valve::g_input = *reinterpret_cast< valve::input_t** >(
				find_byte_seq( cl_code_section, ( "B9 ? ? ? ? FF 75 08 FF 50 0C" ) ) + 0x1u
				);

			valve::g_game_event_mgr = **reinterpret_cast< valve::c_game_event_mgr*** >(
				find_byte_seq( cl_code_section, ( "8B 0D ? ? ? ? 50 8B 11 FF 52 24 FF 15 ? ? ? ?" ) ) + 0x2u
				);

			valve::g_studio_render = **reinterpret_cast< valve::studio_render_t*** >(
				find_byte_seq( cl_code_section, ( "8B 0D ? ? ? ? 8D 54 24 40 52 8B 01 FF 50 34" ) ) + 0x2u
				);

			valve::g_mat_system = **reinterpret_cast< valve::c_mat_system*** >(
				find_byte_seq( cl_code_section, ( "8B 0D ?? ?? ?? ?? 8B 01 8B 80 ?? ?? ?? ?? FF D0 8B F0 89 74 24 48" ) ) + 0x2u
				);

			valve::g_view_render = **reinterpret_cast< valve::view_render_t*** >(
				find_byte_seq( cl_code_section, ( "8B 0D ? ? ? ? 8B 01 FF 50 4C 8B 06" ) ) + 0x2u
				);

			valve::g_surface = **reinterpret_cast< valve::c_surface*** >(
				find_byte_seq( cl_code_section, ( "8B 0D ? ? ? ? FF B7 ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B 7C 24 1C" ) ) + 0x2u
				);

			valve::g_localize = reinterpret_cast< valve::c_localize * ( __cdecl* )( const char*, int* ) >(
				GetProcAddress ( GetModuleHandle ( xorstr_ ( "localize.dll" ) ), xorstr_ ( "CreateInterface" ) )
				)( xorstr_ ( "Localize_001" ), nullptr );

			valve::g_font_mgr = *reinterpret_cast< valve::c_font_mgr** >(
				find_byte_seq( vgui_code_section, ( "74 1D 8B 0D ? ? ? ? 68 ? ? ? ?" ) ) + 0x4u
				);

			valve::g_model_info = *reinterpret_cast< valve::c_model_info** >(
				find_byte_seq( eng_code_section, ( "B9 ? ? ? ? 8B 44 B8 04 83 C0 04 50 A1 ? ? ? ? FF 50 08 8B D0 8B 4C BB 0C" ) ) + 0x1u
				);

			valve::g_beams = *reinterpret_cast< valve::c_beams** >(
				find_byte_seq( cl_code_section, ( "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9" ) ) + 0x1u
				);

			valve::g_model_render = reinterpret_cast< valve::c_model_render * ( __cdecl* )( const char*, int* ) >(
				GetProcAddress ( GetModuleHandle ( xorstr_ ( "engine.dll" ) ), xorstr_ ( "CreateInterface" ) )
				)( xorstr_ ( "VEngineModel016" ), nullptr );

			valve::g_mdl_cache = reinterpret_cast< valve::c_mdl_cache * ( __cdecl* )( const char*, int* ) >(
				GetProcAddress ( GetModuleHandle ( xorstr_ ( "datacache.dll" ) ), xorstr_ ( "CreateInterface" ) )
				)( xorstr_ ( "MDLCache004" ), nullptr );

			valve::g_glow_mgr = *reinterpret_cast< valve::glow_mgr_t** >(
				find_byte_seq( cl_code_section, ( "0F 11 05 ? ? ? ? 83 C8 01" ) ) + 0x3u
				);

			valve::g_debug_overlay = reinterpret_cast< valve::c_debug_overlay * ( __cdecl* )( const char*, int* ) >(
				GetProcAddress ( GetModuleHandle ( xorstr_ ( "engine.dll" ) ), xorstr_ ( "CreateInterface" ) )
				)( xorstr_ ( "VDebugOverlay004" ), nullptr );

#if DEBUG_DUMP_MODE

			DUMP_INTERFACE ( "client_int", valve::g_client );
			DUMP_INTERFACE ( "global_vars_int", valve::g_global_vars );
			DUMP_INTERFACE ( "engine_int", valve::g_engine );
			DUMP_INTERFACE ( "entity_list_int", valve::g_entity_list );
			DUMP_INTERFACE ( "client_state_int", valve::g_client_state );
			DUMP_INTERFACE ( "move_helper_int", valve::g_move_helper );
			DUMP_INTERFACE ( "prediction_int", valve::g_prediction );
			DUMP_INTERFACE ( "movement_int", valve::g_movement );
			DUMP_INTERFACE ( "cvar_int", valve::g_cvar );
			DUMP_INTERFACE ( "engine_trace_int", valve::g_engine_trace );
			DUMP_INTERFACE ( "panel_int", valve::g_panel );
			DUMP_INTERFACE ( "effects_int", valve::g_effects );
			DUMP_INTERFACE ( "surface_data_int", valve::g_surface_data );
			DUMP_INTERFACE ( "input_int", valve::g_input );
			DUMP_INTERFACE ( "game_event_mgr_int", valve::g_game_event_mgr );
			DUMP_INTERFACE ( "studio_render_int", valve::g_studio_render );
			DUMP_INTERFACE ( "model_render_int", valve::g_model_render );
			DUMP_INTERFACE ( "mdl_cache_int", valve::g_mdl_cache );
			DUMP_INTERFACE ( "mat_system_int", valve::g_mat_system );
			DUMP_INTERFACE ( "view_render_int", valve::g_view_render );
			DUMP_INTERFACE ( "surface_int", valve::g_surface );
			DUMP_INTERFACE ( "font_mgr_int", valve::g_font_mgr );
			DUMP_INTERFACE ( "localize_int", valve::g_localize );
			DUMP_INTERFACE ( "model_info_int", valve::g_model_info );
			DUMP_INTERFACE ( "glow_mgr_int", valve::g_glow_mgr );
			DUMP_INTERFACE ( "beams_int", valve::g_beams );
			DUMP_INTERFACE ( "debu_int", valve::g_debug_overlay );

#endif // DEBUG_DUMP_MODE



#ifdef _DEBUG
			std::printf ( "parsing client classes...\n" );
#endif
			valve::g_net_vars->parse_client_classes ( );

#ifdef _DEBUG
			std::printf ( "parsing data_maps...\n" );
#endif


			{
				std::vector< valve::data_map_t* > data_maps {};

				auto start = cl_code_section.m_base;
				auto size = cl_code_section.m_size;

				std::uintptr_t addr {};
				for ( ; ; ) {
					code_section_t section = { size, start };

					addr = find_byte_seq( section, "C7 05 ? ? ? ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C3 CC" );
					if ( addr == ( start + size ) )
						break;

					const auto data_map = reinterpret_cast< valve::data_map_t* >(
						*reinterpret_cast< std::uintptr_t* >( addr + 0x2u ) - 0x4u
						);
					if ( data_map
						&& data_map->m_size
						&& data_map->m_size <= 200
						&& data_map->m_description
						&& data_map->m_name )
						data_maps.emplace_back ( data_map );

					size = ( start + size ) - ( addr + 1u );

					start = addr + 1u;
				}

				if ( data_maps.empty ( ) )
					return;

				nem::valve::type_description_t saved_desc;

				for ( const auto& data_map : data_maps )
				{
					if ( IsBadReadPtr ( data_map->m_name, 1 ) )
						continue;

					if ( std::string ( data_map->m_name ) == "C_BasePlayer" )
					{
						for ( int i {}; i < data_map->m_size; ++i ) {
							const auto& description = data_map->m_description[ i ];

							if ( std::string ( description.m_name ) == "m_vphysicsCollisionState" )
							{
								saved_desc = description;
								break;
							}
						}
					}
				}

				for ( const auto& data_map : data_maps )
				{
					if ( IsBadReadPtr ( data_map->m_name, 1 ) )
						continue;
					if ( std::string ( data_map->m_name ) == "C_CSPlayer" )
					{
						nem::valve::type_description_t* saved_descs = new nem::valve::type_description_t[ data_map->m_size + 2 ];

						memcpy ( saved_descs, data_map->m_description, data_map->m_size * sizeof ( nem::valve::type_description_t ) );

						saved_desc.m_name = "m_chief_keef";
						saved_desc.m_external_name = "m_chief_keef";
						saved_descs[ data_map->m_size ] = saved_desc;

						saved_desc.m_name = "m_bitch_love_sosa";  
						saved_desc.m_external_name = "m_bitch_love_sosa";
						saved_descs[ data_map->m_size + 1 ] = saved_desc;

						data_map->m_description = saved_descs;
						data_map->m_size += 2;


					}
				}

				valve::g_net_vars->parse_data_maps ( data_maps );
			}
		}

#ifdef _DEBUG
		std::printf ( "initializing addresses...\n" );
#endif
		/* init addresses */
		{
			const auto vstdlib_dll = GetModuleHandle ( xorstr_ ( "vstdlib.dll" ) );

			m_addresses.m_random_float = reinterpret_cast< addresses_t::random_float_t >( GetProcAddress( vstdlib_dll, xorstr_( "RandomFloat" ) ) );
			m_addresses.m_random_int = reinterpret_cast< addresses_t::random_int_t >( GetProcAddress ( vstdlib_dll, xorstr_ ( "RandomInt" ) ) );
			m_addresses.m_random_seed = reinterpret_cast< addresses_t::random_seed_t >( GetProcAddress ( vstdlib_dll, xorstr_ ( "RandomSeed" ) ) );

			m_addresses.m_cl_read_packets = reinterpret_cast < addresses_t::cl_read_packets_t > ( find_byte_seq ( eng_code_section, ( "53 8A D9 8B 0D ? ? ? ? 56 57 8B B9" ) ) );

			m_addresses.m_reset_anim_state = find_byte_seq ( cl_code_section, ( "56 6A 01 68 ? ? ? ? 8B F1" ) );
			m_addresses.m_update_anim_state = find_byte_seq (
				cl_code_section, ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" )
			);
			m_addresses.m_set_abs_angles = find_byte_seq ( cl_code_section, ( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1" ) );
			m_addresses.m_set_abs_origin = find_byte_seq ( cl_code_section, ( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D" ) );
			m_addresses.m_lookup_seq_act = find_byte_seq ( cl_code_section, ( "55 8B EC 53 8B 5D 08 56 8B F1 83" ) );
			m_addresses.m_breakable = find_byte_seq ( cl_code_section, ( "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE" ) );
			m_addresses.m_user_cmd_calc_checksum = find_byte_seq( cl_code_section, ( "53 8B D9 83 C8" ) );
			m_addresses.m_upd_collision_bounds = find_byte_seq ( cl_code_section, ( "56 57 8B F9 8B 0D FC D9 ?? ?? F6 87 04 01 00" ) );

			m_addresses.m_pred_seed = *reinterpret_cast< int** >(
				find_byte_seq( cl_code_section, ( "A3 ? ? ? ? 66 0F 6E 86" ) ) + 0x1u
				);

			m_addresses.m_pred_player = *reinterpret_cast< valve::c_player** >(
				find_byte_seq( cl_code_section, ( "89 35 ? ? ? ? F3 0F 10 48" ) ) + 0x2u
				);


			const auto angle_matrix_rel = find_byte_seq( cl_code_section, ( "E8 ? ? ? ? 8B 07 89 46 0C" ) );
			m_addresses.m_angle_matrix = reinterpret_cast< addresses_t::angle_matrix_t >(
				angle_matrix_rel + 0x1u + sizeof ( std::uintptr_t ) + *reinterpret_cast< std::ptrdiff_t* >( angle_matrix_rel + 0x1u )
				);

			m_addresses.m_view_matrix = find_byte_seq(
				cl_code_section, ( "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" )
			);

			m_addresses.m_allow_to_extrapolate = find_byte_seq( cl_code_section, ( "A2 ? ? ? ? 8B 45 E8" )
			);

			m_addresses.m_line_goes_thru_smoke = reinterpret_cast< addresses_t::line_goes_thru_smoke_t >(
				find_byte_seq( cl_code_section, "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F" )
				);

			m_addresses.m_set_collision_bounds = find_byte_seq(
				cl_code_section, ( "53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 18 56 57 8B 7B 08 8B D1 8B 4B 0C" )
			);

			m_addresses.m_invalidate_bone_cache = find_byte_seq( cl_code_section, ( "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81" ) );
			m_addresses.m_lookup_bone = find_byte_seq( cl_code_section, ( "55 8B EC 53 56 8B F1 57 83 BE ?? ?? ?? ?? ?? 75 14 8B 46 04 8D 4E 04 FF 50 20" ) );
			m_addresses.m_trace_filter_simple_vtable = *reinterpret_cast< std::uintptr_t* >(
				find_byte_seq( cl_code_section, ( "55 8B EC 83 E4 F0 83 EC 7C 56 52" ) ) + 0x3du
				);

			m_addresses.m_trace_filter_skip_two_entities_vtable = *reinterpret_cast< std::uintptr_t* >(
				find_byte_seq( cl_code_section, ( "C7 45 ? ? ? ? ? 89 45 E4 8B 01" ) ) + 0x3u
				);

			m_addresses.m_bone_counter = find_byte_seq( cl_code_section, ( "FF 05 ? ? ? ? FF 05 ? ? ? ? " ) );

			m_addresses.m_compute_hitbox_surround_box = find_byte_seq(
				cl_code_section, ( "E9 ? ? ? ? 32 C0 5D" )
			);

			m_addresses.m_write_user_cmd = find_byte_seq ( cl_code_section, ( "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D" ) );
			m_addresses.m_weapon_system = *reinterpret_cast< std::uintptr_t* >(
				find_byte_seq( cl_code_section, ( "8B 35 ? ? ? ? FF 10 0F B7 C0" ) ) + 0x2u
				);

			const auto calc_shotgun_spread_rel = find_byte_seq( cl_code_section, ( "E8 ? ? ? ? EB 38 83 EC 08" ) );

			m_addresses.m_calc_shotgun_spread =
				calc_shotgun_spread_rel + 0x1u + sizeof ( std::uintptr_t )
				+ *reinterpret_cast< std::ptrdiff_t* >( calc_shotgun_spread_rel + 0x1u );

			m_addresses.m_key_values_init = find_byte_seq( cl_code_section, ( "55 8B EC 51 33 C0 C7 45" ) );
			m_addresses.m_key_values_load_from_buffer = find_byte_seq( cl_code_section, ( "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89" ) );
			m_addresses.m_attachment_helper = find_byte_seq( cl_code_section, "55 8B EC 83 EC 48 53 8B 5D 08 89 4D F4" );
			m_addresses.m_cam_sv_cheats_ret = find_byte_seq( cl_code_section, ( "85 C0 75 30 38 86" ) );
			m_addresses.m_add_notify = find_byte_seq( eng_code_section, ( "55 8B EC 81 EC 10 08 00 00 80 3D" ) );
			m_addresses.m_ret_to_scope_arc = find_byte_seq( cl_code_section, ( "6A 00 FF 50 3C 8B 0D ? ? ? ? FF B7 ? ? ? ?" ) ) + 0x5u;
			m_addresses.m_ret_to_scope_lens = find_byte_seq( cl_code_section, ( "FF 50 3C 8B 4C 24 20" ) ) + 0x3u;
			m_addresses.m_set_clan_tag = find_byte_seq( eng_code_section, ( "53 56 57 8B DA 8B F9 FF 15" ) );
			m_addresses.m_start_drawing = find_byte_seq( vgui_code_section, ( "55 8B EC 83 E4 C0 83 EC 38" ) );
			m_addresses.m_finish_drawing = find_byte_seq( vgui_code_section, ( "8B 0D ? ? ? ? 56 C6 05 ? ? ? ? ?" ) );
			m_addresses.m_add_clred_text = find_byte_seq( vgui_code_section, ( "55 8B EC 81 EC ? ? ? ? 56 57 FF 75 10" ) );
			m_addresses.m_calc_text_size = find_byte_seq( vgui_code_section, ( "55 8B EC 83 EC 18 83 7D 0C 00" ) );
			m_addresses.m_set_font_glyph_set = find_byte_seq( vgui_code_section, ( "55 8B EC 83 EC 08 8B 45 08 53" ) );
			m_addresses.m_has_c4 = find_byte_seq( cl_code_section, ( "56 8B F1 85 F6 74 31" ) );
			m_addresses.m_ret_insert_into_tree = find_byte_seq( cl_code_section, ( "89 44 24 14 EB 08 C7 44 24 ? ? ? ? ? 8B 45" ) );
			m_addresses.m_smoke_count = find_byte_seq( cl_code_section, ( "A3 ? ? ? ? 57 8B CB" ) ) + 0x1u;
			m_addresses.m_disable_post_processing = find_byte_seq( cl_code_section, ( "80 3D ? ? ? ? ? 53 56 57 0F 85" ) ) + 0x2u;
			m_addresses.m_crosshair_ret = find_byte_seq( cl_code_section, ( "85 C0 0F 84 ? ? ? ? E8 ? ? ? ? 99" ) );
			m_addresses.m_ret_to_scope_clear = find_byte_seq( cl_code_section, ( "0F 82 ? ? ? ? FF 50 3C" ) ) + 0x9u;
			m_addresses.m_ret_to_scope_blurry = find_byte_seq( cl_code_section, ( "FF B7 ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B 4C 24 24" ) ) - 0x6u;
			m_addresses.m_ret_to_eye_pos_and_vectors = find_byte_seq( cl_code_section, ( "8B 55 0C 8B C8 E8 ? ? ? ? 83 C4 08 5E 8B E5" ) );
			m_addresses.m_ret_to_extrapolation = find_byte_seq( cl_code_section, ( "0F B6 0D ? ? ? ? 84 C0 0F 44 CF 88 0D ? ? ? ?" ) );
			m_addresses.m_ret_to_maintain_seq_transitions = find_byte_seq( cl_code_section, ( "84 C0 74 17 8B 87" ) );
			m_addresses.m_ret_to_accumulate_layers = find_byte_seq( cl_code_section, ( "84 C0 75 0D F6 87" ) );
			m_addresses.m_ret_to_setup_velocity = find_byte_seq( cl_code_section, ( "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0" ) );
			m_addresses.m_clamp_bones_in_box = find_byte_seq( cl_code_section, ( "55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 38 83 BF ? ? ? ? ? 75" ) );
			m_addresses.m_item_system = find_byte_seq( cl_code_section, ( "A1 ? ? ? ? 85 C0 75 ? A1 ? ? ? ? 56 68 E8 07 00 00" ) );
			m_addresses.m_hud = *reinterpret_cast< std::uintptr_t* >(
				find_byte_seq( cl_code_section, ( "B9 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 89" ) ) + 0x1u
				);
			m_addresses.m_find_hud_element = find_byte_seq( cl_code_section, ( "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28" ) );
			m_addresses.m_clear_hud_wpn_icon = find_byte_seq( cl_code_section, ( "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B" ) );

			m_addresses.m_invalidate_physics_recursive = find_byte_seq( cl_code_section, ( "55 8B EC 83 E4 F8 83 EC 0C 53 8B 5D 08 8B C3 56 83 E0 04" ) );

			m_addresses.m_sky_box = ( addresses_t::sky_box_set_fn ) find_byte_seq( eng_code_section, ( "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45" ) );

			m_addresses.m_clear_hud_wpn = find_byte_seq( cl_code_section, ( "55 8B EC 83 EC 0C 53 56 8B 71 58" ) );

			//m_addresses.m_ayo_who_asked =  ( addresses_t::who_asked_fn )find_byte_seq(cl_code_section, xorstr_("55 8B EC 83 EC 0C 53 56 8B 71 58"));
		}

#ifdef _DEBUG
		std::printf ( "initializing cvars...\n" );
#endif

		/* init cvars */
		{
			m_cvars.m_r_jiggle_bones = valve::g_cvar->find_var ( xorstr_ ( "r_jiggle_bones" ) );
			m_cvars.m_cl_interp = valve::g_cvar->find_var ( xorstr_ ( "cl_interp" ) );
			m_cvars.m_cl_interp_ratio = valve::g_cvar->find_var ( xorstr_ ( "cl_interp_ratio" ) );
			m_cvars.m_cl_updaterate = valve::g_cvar->find_var ( xorstr_ ( "cl_updaterate" ) );
			m_cvars.m_ff_dmg_reduction_bullets = valve::g_cvar->find_var ( xorstr_ ( "ff_damage_reduction_bullets" ) );
			m_cvars.m_ff_dmg_bullet_pen = valve::g_cvar->find_var ( xorstr_ ( "ff_damage_bullet_penetration" ) );
			m_cvars.m_sv_auto_bhop = valve::g_cvar->find_var ( xorstr_ ( "sv_autobunnyhopping" ) );
			m_cvars.m_sv_clockcorrection_msecs = valve::g_cvar->find_var ( xorstr_ ( "sv_clockcorrection_msecs" ) );
			m_cvars.m_sv_maxvelocity = valve::g_cvar->find_var ( xorstr_ ( "sv_maxvelocity" ) );
			m_cvars.m_sv_friction = valve::g_cvar->find_var ( xorstr_ ( "sv_friction" ) );
			m_cvars.m_sv_accelerate = valve::g_cvar->find_var ( xorstr_ ( "sv_accelerate" ) );
			m_cvars.m_sv_accelerate_use_weapon_speed = valve::g_cvar->find_var ( xorstr_ ( "sv_accelerate_use_weapon_speed" ) );
			m_cvars.m_weapon_accuracy_nospread = valve::g_cvar->find_var ( xorstr_ ( "weapon_accuracy_nospread" ) );
			m_cvars.m_sv_maxunlag = valve::g_cvar->find_var ( xorstr_ ( "sv_maxunlag" ) );
			m_cvars.m_sv_enable_bhop = valve::g_cvar->find_var ( xorstr_ ( "sv_enablebunnyhopping" ) );
			m_cvars.m_sv_jump_impulse = valve::g_cvar->find_var ( xorstr_ ( "sv_jump_impulse" ) );
			m_cvars.m_sv_gravity = valve::g_cvar->find_var ( xorstr_ ( "sv_gravity" ) );
			m_cvars.m_weapon_recoil_scale = valve::g_cvar->find_var ( xorstr_ ( "weapon_recoil_scale" ) );
			m_cvars.m_weapon_accuracy_shotgun_spread_patterns = valve::g_cvar->find_var ( xorstr_ ( "weapon_accuracy_shotgun_spread_patterns" ) );
			m_cvars.m_molotov_throw_detonate_time = valve::g_cvar->find_var ( xorstr_ ( "molotov_throw_detonate_time" ) );
			m_cvars.m_weapon_molotov_maxdetonateslope = valve::g_cvar->find_var ( xorstr_ ( "weapon_molotov_maxdetonateslope" ) );
			m_cvars.m_cl_fullupdate = valve::g_cvar->find_var ( xorstr_ ( "cl_fullupdate" ) );
			m_cvars.m_sv_stopspeed = valve::g_cvar->find_var ( xorstr_ ( "sv_stopspeed" ) );
		}


#ifdef _DEBUG
		std::printf ( "setting up event listener...\n" );
#endif
		{
			hacks::g_shots->register_self ( );
		}

		hacks::g_visuals->init ( );
		hacks::g_skins->init ( );

#ifdef _DEBUG
		std::printf ( "setting up hooks...\n" );
#endif
		{
			auto& vel_mod_prop = std::get< valve::recv_prop_t* > ( valve::g_net_vars->entry ( xorstr_ ( "CCSPlayer->m_flVelocityModifier" ) ) );

			hooks::orig_velocity_modifier = reinterpret_cast< decltype( hooks::orig_velocity_modifier ) >( vel_mod_prop->m_proxy_fn );

			vel_mod_prop->m_proxy_fn = reinterpret_cast< std::uintptr_t >( &hooks::velocity_modifier );

			auto& sequence_prop = std::get< valve::recv_prop_t* > ( valve::g_net_vars->entry ( xorstr_ ( "CBaseViewModel->m_nSequence" ) ) );

			hooks::orig_sequence = reinterpret_cast< decltype( hooks::orig_sequence ) >( sequence_prop->m_proxy_fn );

			sequence_prop->m_proxy_fn = reinterpret_cast< std::uintptr_t >( &hooks::sequence );
		}

		auto engine = GetModuleHandleA ( xorstr_ ( "engine.dll" ) );

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( d3d9_device ) )[ 16u ],
			reinterpret_cast< LPVOID >( &hooks::d3d9_reset ),
			reinterpret_cast< LPVOID* >( &hooks::orig_d3d9_reset )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( d3d9_device ) )[ 17u ],
			reinterpret_cast< LPVOID >( &hooks::d3d9_present ),
			reinterpret_cast< LPVOID* >( &hooks::orig_d3d9_present )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq ( eng_code_section, ( "55 8B EC 83 EC 40 53 8B D9 8B 0D ? ? ? ? 89 5D F8" ) )
				),
			reinterpret_cast< LPVOID >( &hooks::paint ),
			reinterpret_cast< LPVOID* >( &hooks::orig_paint )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq ( eng_code_section, ( "55 8B EC 81 EC ? ? ? ? 53 56 8A F9 F3 0F 11 45 ? 8B" ) )
				),
			reinterpret_cast< LPVOID >( &hooks::cl_move ),
			reinterpret_cast< LPVOID* >( &hooks::orig_cl_move )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_cvar->find_var ( xorstr_ ( "sv_cheats" ) ) ) )[ 13u ],
			reinterpret_cast< LPVOID >( &hooks::sv_cheats_get_int ),
			reinterpret_cast< LPVOID* >( &hooks::orig_sv_cheats_get_int )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_studio_render ) )[ 29u ],
			reinterpret_cast< LPVOID >( &hooks::draw_model ),
			reinterpret_cast< LPVOID* >( &hooks::orig_draw_model )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_model_render ) ) [ 21u ],
			reinterpret_cast< LPVOID >( &hooks::draw_mdl_exec ),
			reinterpret_cast< LPVOID* >( &hooks::orig_draw_mdl_exec )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_surface ) )[ 67u ],
			reinterpret_cast< LPVOID >( &hooks::lock_cursor ),
			reinterpret_cast< LPVOID* >( &hooks::orig_lock_cursor )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_surface ) )[ 15u ],
			reinterpret_cast< LPVOID >( &hooks::draw_set_clr ),
			reinterpret_cast< LPVOID* >( &hooks::orig_draw_set_clr )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_prediction ) )[ 14u ],
			reinterpret_cast< LPVOID >( &hooks::in_prediction ),
			reinterpret_cast< LPVOID* >( &hooks::orig_in_prediction )
		) != MH_OK )
			return;

		if (MH_CreateHook(
			( *reinterpret_cast< LPVOID** >( valve::g_prediction ) )[ 19u ],
			reinterpret_cast< LPVOID >( &hooks::run_cmd ),
			reinterpret_cast< LPVOID* >( &hooks::orig_run_cmd )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_prediction ) ) [ 20u ],
			reinterpret_cast< LPVOID >( &hooks::setup_move ),
			reinterpret_cast< LPVOID* >( &hooks::orig_setup_move )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_panel ) )[ 41u ],
			reinterpret_cast< LPVOID >( &hooks::paint_traverse ),
			reinterpret_cast< LPVOID* >( &hooks::orig_paint_traverse )
		) != MH_OK )
			return;

		if (MH_CreateHook(
			( *reinterpret_cast< LPVOID** >( valve::g_client ) )[ 22u ],
			reinterpret_cast< LPVOID >( &hooks::create_move_proxy ),
			reinterpret_cast< LPVOID* >( &hooks::orig_create_move )
		) != MH_OK )
			return;
			
		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_movement ) )[ 1u ],
			reinterpret_cast< LPVOID >( &hooks::process_movement ),
			reinterpret_cast< LPVOID* >( &hooks::orig_process_movement )
		) != MH_OK )
			return;

		if (MH_CreateHook(
			( *reinterpret_cast< LPVOID** >( valve::g_client ) )[ 37u ],
			reinterpret_cast< LPVOID >( &hooks::frame_stage_notify ),
			reinterpret_cast< LPVOID* >( &hooks::orig_frame_stage_notify )
		) != MH_OK )
			return;
			
		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq ( cl_code_section, ( "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 57 8B F9 8B 0D ? ? ? ?" ) )
				),
			reinterpret_cast< LPVOID >( &hooks::setup_bones ),
			reinterpret_cast< LPVOID* >( &hooks::orig_setup_bones )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq ( cl_code_section, ( "55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 38 83 BF ? ? ? ? ? 75" ) )
				),
			reinterpret_cast< LPVOID >( &hooks::clamp_bones_in_box ),
			reinterpret_cast< LPVOID* >( &hooks::orig_clamp_bones_in_box )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq ( cl_code_section, ( "55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74 36" ) )
				),
			reinterpret_cast< LPVOID >( &hooks::update_client_side_anim ),
			reinterpret_cast< LPVOID* >( &hooks::orig_update_client_side_anim )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq ( cl_code_section, ( "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 89 74 24 1C" ) )
				),
			reinterpret_cast< LPVOID >( &hooks::do_extra_bones_processing ),
			reinterpret_cast< LPVOID* >( &hooks::orig_do_extra_bones_processing )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq ( cl_code_section, ( "55 8B EC 57 8B F9 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ?" ) )
				),
			reinterpret_cast< LPVOID >( &hooks::accumulate_layers ),
			reinterpret_cast< LPVOID* >( &hooks::orig_accumulate_layers )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq ( cl_code_section, ( "55 8B EC 51 53 8B 5D 08 56 8B F1 57 85" ) )
				),
			reinterpret_cast< LPVOID >( &hooks::check_for_seq_change ),
			reinterpret_cast< LPVOID* >( &hooks::orig_check_for_seq_change )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq ( cl_code_section, ( "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 8B 75 08 57 8B F9 85 F6" ) )
				),
			reinterpret_cast< LPVOID >( &hooks::standard_blending_rules ),
			reinterpret_cast< LPVOID* >( &hooks::orig_standard_blending_rules )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq ( cl_code_section, ( "57 8B F9 8B 07 8B 80 78" ) )
				),
			reinterpret_cast< LPVOID >( &hooks::should_skip_anim_frame ),
			reinterpret_cast< LPVOID* >( &hooks::orig_should_skip_anim_frame )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq (
					eng_code_section,
					( "55 8B EC 8B 45 08 89 81 ? ? ? ? 8B 45 0C 89 81 ? ? ? ? 5D C2 08 00 CC CC CC CC CC CC CC 56" )
				)
				),
			reinterpret_cast< LPVOID >( &hooks::packet_start ),
			reinterpret_cast< LPVOID* >( &hooks::orig_packet_start )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq (
					eng_code_section,
					( "56 8B F1 E8 ? ? ? ? 8B 8E ? ? ? ? 3B 8E ? ? ? ?" )
				)
				),
			reinterpret_cast< LPVOID >( &hooks::packet_end ),
			reinterpret_cast< LPVOID* >( &hooks::orig_packet_end )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq (
					cl_code_section,
					( "56 8B F1 8B 8E ? ? ? ? 83 F9 FF 74 23" )
				)
				),
			reinterpret_cast< LPVOID >( &hooks::physics_simulate ),
			reinterpret_cast< LPVOID* >( &hooks::orig_physics_simulate )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq (
					cl_code_section,
					( "55 8B EC 83 EC 10 53 56 8B F1 57 80 BE ? ? ? ? ? 75 41" )
				)
				),
			reinterpret_cast< LPVOID >( &hooks::on_latch_interpolated_vars ),
			reinterpret_cast< LPVOID* >( &hooks::orig_on_latch_interpolated_vars )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq (
					cl_code_section,
					( "56 8B F1 E8 ? ? ? ? 3B F0" )
				)
				),
			reinterpret_cast< LPVOID >( &hooks::should_interpolate ),
			reinterpret_cast< LPVOID* >( &hooks::orig_should_interpolate )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_client ) )[ 24u ],
			reinterpret_cast< LPVOID >( &hooks::write_user_cmd_delta_to_buffer ),
			reinterpret_cast< LPVOID* >( &hooks::orig_write_user_cmd_delta_to_buffer )
		) != MH_OK )
			return;

		const auto modify_eye_pos_rel = find_byte_seq ( cl_code_section, ( "E8 ? ? ? ? 8B 06 8B CE FF 90 ? ? ? ? 85 C0 74 50" ) );
		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				modify_eye_pos_rel + 0x1u + sizeof ( std::uintptr_t )
				+ *reinterpret_cast< std::ptrdiff_t* >( modify_eye_pos_rel + 0x1u )
				),
			reinterpret_cast< LPVOID >( &hooks::modify_eye_pos ),
			reinterpret_cast< LPVOID* >( &hooks::orig_modify_eye_pos )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq (
					eng_code_section,
					( "55 8B EC 83 E4 F8 83 EC 4C A1 ? ? ? ? 80 B8 ? ? ? ? ?" )
				)
				),
			reinterpret_cast< LPVOID >( &hooks::process_temp_entities ),
			reinterpret_cast< LPVOID* >( &hooks::orig_process_temp_entities )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_client ) )[ 7u ],
			reinterpret_cast< LPVOID >( &hooks::level_shutdown ),
			reinterpret_cast< LPVOID* >( &hooks::orig_level_shutdown )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq (
					cl_code_section,
					( "55 8B EC 83 EC 14 53 8B 5D 0C 56 57 85 DB 74" )
				)
				),
			reinterpret_cast< LPVOID >( &hooks::glow_effect_spectator ),
			reinterpret_cast< LPVOID* >( &hooks::orig_glow_effect_spectator )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq (
					GetModuleHandle ( xorstr_ ( "materialsystem.dll" ) ),
					( "55 8B EC 83 EC ? 56 8B F1 8A 46" )
				)
				),
			reinterpret_cast< LPVOID >( &hooks::get_clr_modulation ),
			reinterpret_cast< LPVOID* >( &hooks::orig_get_clr_modulation )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq (
					eng_code_section,
					( "8B 0D ? ? ? ? 81 F9 ? ? ? ? 75 ? A1 ? ? ? ? 35 ? ? ? ? EB ? 8B 01 FF 50 ? 83 F8 ? 0F 85 ? ? ? ? 8B 0D" )
				)
				),
			reinterpret_cast< LPVOID >( &hooks::is_using_static_prop_debug_modes ),
			reinterpret_cast< LPVOID* >( &hooks::orig_is_using_static_prop_debug_modes )
		) != MH_OK )
			return;


		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq (
					cl_code_section,
					( "8B 0D ? ? ? ? 56 8B 01 FF 50 34 8B F0 85 F6 75 04" )
				)
				),
			reinterpret_cast< LPVOID >( &hooks::is_depth_something_in_nigga_ass ),
			reinterpret_cast< LPVOID* >( &hooks::orig_nigga_hack )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_engine->bsp_tree_query ( ) ) )[ 6u ],
			reinterpret_cast< LPVOID >( &hooks::list_leaves_in_box ),
			reinterpret_cast< LPVOID* >( &hooks::orig_list_leaves_in_box )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_mat_system ) )[ 21u ],
			reinterpret_cast< LPVOID >( &hooks::override_cfg ),
			reinterpret_cast< LPVOID* >( &hooks::orig_override_cfg )
		) != MH_OK )
			return;

		const auto client_mode = **reinterpret_cast< std::uintptr_t*** >(
			( *reinterpret_cast< std::uintptr_t** >( valve::g_client ) )[ 10u ] + 0x5u
			);

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq ( cl_code_section, ( "53 56 8B F1 8A DA 8B 0D ? ? ? ? 81 F9 ? ? ? ? 75 0C A1 ? ? ? ? 35 ? ? ? ? EB 05 8B 01" ) )
				),
			reinterpret_cast< LPVOID >( &hooks::fog_enabled ),
			reinterpret_cast< LPVOID* >( &hooks::orig_fog_enabled )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq ( cl_code_section, ( "53 0F B7 1D ? ? ? ? 56" ) )
				),
			reinterpret_cast< LPVOID >( &hooks::process_interpolated_list ),
			reinterpret_cast< LPVOID* >( &hooks::orig_process_interp_list )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( client_mode ) )[ 18u ],
			reinterpret_cast< LPVOID >( &hooks::override_view ),
			reinterpret_cast< LPVOID* >( &hooks::orig_override_view )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( client_mode ) )[ 44u ],
			reinterpret_cast< LPVOID >( &hooks::do_post_screen_space_effects ),
			reinterpret_cast< LPVOID* >( &hooks::orig_do_post_screen_space_effects )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq ( cl_code_section, ( "55 8B EC 83 EC 14 53 56 57 FF 75 18" ) )
				),
			reinterpret_cast< LPVOID >( &hooks::calc_view ),
			reinterpret_cast< LPVOID* >( &hooks::orig_calc_view )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq ( cl_code_section, ( "56 8B F1 85 F6 74 32" ) )
				),
			reinterpret_cast< LPVOID >( &hooks::get_eye_angles ),
			reinterpret_cast< LPVOID* >( &hooks::orig_get_eye_angles )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			reinterpret_cast< LPVOID >(
				find_byte_seq ( cl_code_section, ( "55 8B EC A1 ? ? ? ? 83 EC 10 56 8B F1 B9" ) )
				),
			reinterpret_cast< LPVOID >( &hooks::calc_viewmodel_bob ),
			reinterpret_cast< LPVOID* >( &hooks::orig_viewmodel_bob )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_engine ) )[ 90u ],
			reinterpret_cast< LPVOID >( &hooks::is_paused ),
			reinterpret_cast< LPVOID* >( &hooks::orig_is_paused )
		) != MH_OK )
			return;


		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_engine ) )[ 93u ],
			reinterpret_cast< LPVOID >( &hooks::is_hltv ),
			reinterpret_cast< LPVOID* >( &hooks::orig_is_hltv )
		) != MH_OK )
			return;

		if ( MH_CreateHook (
			( *reinterpret_cast< LPVOID** >( valve::g_engine ) ) [ 101u ],
			reinterpret_cast< LPVOID >( &hooks::aspect_ratio ),
			reinterpret_cast< LPVOID* >( &hooks::orig_aspect_ratio )
		) != MH_OK )
			return;



		if ( MH_EnableHook( MH_ALL_HOOKS ) != MH_OK )
			return;

		auto fp = fopen( xorstr_( "threads_log.txt" ), "wb" );
		fprintf( fp, xorstr_( "threads created: %d" ), std::jthread::hardware_concurrency( ) );
		fclose( fp );

	}

	bool c_context::will_shoot( valve::c_weapon* const weapon, const valve::user_cmd_t& user_cmd ) const {
		if ( !weapon )
			return false;

		const auto item_index = weapon->item_index( );
		const auto wpn_data = weapon->wpn_data( );

		if ( ( item_index < valve::e_item_index::flashbang || item_index > valve::e_item_index::inc_grenade )
			&& !( user_cmd.m_buttons & valve::e_buttons::in_attack )
			&& ( !( user_cmd.m_buttons & valve::e_buttons::in_attack2 )
				|| weapon->next_secondary_attack( ) >= valve::g_global_vars->m_cur_time
				|| ( item_index != valve::e_item_index::revolver && ( !wpn_data || wpn_data->m_unk_type != 1 ) )
				)
			)
			return false;

		return true;
	}

	bool c_context::can_shoot(
		valve::c_weapon* const weapon, const valve::user_cmd_t& user_cmd,
		bool r8_ret, const int shift_amount, const bool normal
	) const {
		const auto& anim_data = hacks::g_anim_sync->local_data( );
		if ( !normal
			&& anim_data.m_shot_valid_wpn
			&& anim_data.m_shot_cmd_number > valve::g_client_state->m_last_cmd_out
			&& anim_data.m_shot_cmd_number < ( valve::g_client_state->m_last_cmd_out + 100 ) )
			return false;

		if ( !weapon
			|| m_freeze_time
			|| !valve::g_local_player
			|| user_cmd.m_weapon_select
			|| valve::g_local_player->flags( ) & valve::e_ent_flags::frozen
			|| !valve::g_local_player->alive( ) )
			return false;

		const auto wpn_data = weapon->wpn_data( );
		if ( !wpn_data )
			return false;

		const auto item_index = weapon->item_index( );

		const auto v10 = static_cast< std::size_t >( wpn_data->m_unk_type - 2 ) <= 5 || item_index == valve::e_item_index::taser;

		const auto cur_time = valve::to_time( 
			v10 ? valve::g_local_player->tick_base( ) - shift_amount : valve::g_local_player->tick_base( )
		);

		if ( valve::g_local_player->next_attack( ) > cur_time )
			return false;

		if ( item_index == valve::e_item_index::c4 )
			return true;

		if ( item_index >= valve::e_item_index::flashbang
			&& item_index <= valve::e_item_index::inc_grenade )
			return weapon->throw_time( ) > 0.f && cur_time > weapon->throw_time( );

		if ( ( v10 || item_index == valve::e_item_index::taser ) && ( weapon->ammo( ) <= 0 || weapon->in_reload( ) ) )
			return false;

		if ( weapon->next_primary_attack( ) > cur_time )
			return false;

		return item_index == valve::e_item_index::revolver ? r8_ret : true;
	}

	void c_context::add_packet( const int cmd_number ) {
		const auto user_cmd = &valve::g_input->m_cmds[ cmd_number % 150 ];
		if ( !user_cmd ) {
			m_last_sent_cmd_number = cmd_number;
			m_sented_cmds.emplace_back ( cmd_number );
			return;
		}

		m_sented_cmds.emplace_back ( cmd_number );

       /* const auto latency = m_net_info.m_latency.m_out + m_net_info.m_latency.m_in;
		for ( auto& shot : hacks::g_shots->elements( ) ) {
			if ( shot.m_cmd_number != -1 )
				continue;

			shot.m_cmd_number = cmd_number;

            shot.m_latency = latency;
            shot.m_sent_time = valve::g_global_vars->m_real_time;
		}
		*/
	}
}
/*
bool c_key_binds::get_keybind_state ( const s_keybind* bind )
{
	switch ( bind->m_mode )
	{
		case 0: return bind->m_code && GetAsyncKeyState ( bind->m_code ); break;
		case 1: return bind->m_code && GetKeyState ( bind->m_code ); break;
		case 2: return true; break;
		case 3: return false; break;
	}

	return false;
}
*/

const char* Keys__[ ] = {
"[-]",
"[M1]",
"[M2]",
"[CN]",
"[M3]",
"[M4]",
"[M5]",
"[-]",
"[BAC]",
"[TAB]",
"[-]",
"[-]",
"[CLR]",
"[RET]",
"[-]",
"[-]",
"[SHI]",
"[CTL]",
"[ALT]",
"[PAU]",
"[CAP]",
"[KAN]",
"[-]",
"[JUN]",
"[FIN]",
"[KAN]",
"[-]",
"[ESC]",
"[CON]",
"[NCO]",
"[ACC]",
"[MAD]",
"[SPA]",
"[PGU]",
"[PGD]",
"[END]",
"[HOM]",
"[LEF]",
"[UP]",
"[RIG]",
"[DOW]",
"[SEL]",
"[PRI]",
"[EXE]",
"[PRI]",
"[INS]",
"[DEL]",
"[HEL]",
"[0]",
"[1]",
"[2]",
"[3]",
"[4]",
"[5]",
"[6]",
"[7]",
"[8]",
"[9]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[A]",
"[B]",
"[C]",
"[D]",
"[E]",
"[F]",
"[G]",
"[H]",
"[I]",
"[J]",
"[K]",
"[L]",
"[M]",
"[N]",
"[O]",
"[P]",
"[Q]",
"[R]",
"[S]",
"[T]",
"[U]",
"[V]",
"[W]",
"[X]",
"[Y]",
"[Z]",
"[WIN]",
"[WIN]",
"[APP]",
"[-]",
"[SLE]",
"[NUM]",
"[NUM]",
"[NUM]",
"[NUM]",
"[NUM]",
"[NUM]",
"[NUM]",
"[NUM]",
"[NUM]",
"[NUM]",
"[MUL]",
"[ADD]",
"[SEP]",
"[MIN]",
"[DEC]",
"[DIV]",
"[F1]",
"[F2]",
"[F3]",
"[F4]",
"[F5]",
"[F6]",
"[F7]",
"[F8]",
"[F9]",
"[F10]",
"[F11]",
"[F12]",
"[F13]",
"[F14]",
"[F15]",
"[F16]",
"[F17]",
"[F18]",
"[F19]",
"[F20]",
"[F21]",
"[F22]",
"[F23]",
"[F24]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[NUM]",
"[SCR]",
"[EQU]",
"[MAS]",
"[TOY]",
"[OYA]",
"[OYA]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[SHI]",
"[SHI]",
"[CTR]",
"[CTR]",
"[ALT]",
"[ALT]"
};
/*
void c_key_binds::KeybindNelfo ( const char* label, s_keybind* keybind )
{
	ImGui::Spacing ( );
	ImGui::Spacing ( );
	ImGui::SetCursorPosX ( 20.f );

	ImGui::SetCursorPosY ( ImGui::GetCursorPosY ( ) - 3 );
	ImGui::Text ( label );

	ImGui::PushItemWidth ( 90.f );
	ImGui::Combo ( ( std::string ( xorstr_ ( "##BindModeCombo_" ) ) + label ).c_str ( ), &keybind->m_mode, xorstr_ ( "Hold\0Toggle\0Always On\0Always Off" ) );
	ImGui::PopItemWidth ( );

	std::string ButtonString;

	if ( keybind->m_is_binding )
		ButtonString = xorstr_ ( "..." );
	else if ( !keybind->m_code )
		ButtonString = xorstr_ ( "-" );
	else
	{
		ButtonString = Keys__[ keybind->m_code ];

		ButtonString[ 0 ] = ' ';
		ButtonString.back ( ) = ' ';
	}

	ImGui::SameLine ( );
	ImGui::SetCursorPosX ( ImGui::GetCursorPosX ( ) + 13 );

	if ( ImGui::Button ( ( ButtonString[1] + std::string ( xorstr_ ( "###BindButton_" ) ) + label ).c_str ( ), ImVec2 ( 23, 26 ) ) )
	{
		keybind->m_is_binding = true;
		m_last_code = 0;
	}

	if ( keybind->m_is_binding && m_last_code )
	{
		keybind->m_code = m_last_code;
		keybind->m_is_binding = false;
		m_last_code = 0;
	}
}
*/

void c_dlights::alloc_dlight ( col_t col, float exponent, float radius, float decay, int key, vec3_t direction, vec3_t origin, float die )
{
	nem::valve::dlight_t* temp = valve___g_effects->cl_alloc_dlight ( key );
	temp->flags = 0x2;
	temp->color.r = col.r ( );
	temp->color.g = col.g ( );
	temp->color.b = col.b ( );
	temp->color.exponent = exponent; // 0, 133, 255, 161 <= ideal col
	temp->radius = radius;
	temp->decay = decay;
	temp->m_Direction = direction;
	temp->origin = origin;
	temp->die = valve___g_global_vars->m_cur_time + 0.05;
}