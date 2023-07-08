#include "../../nem.hpp"

#include "..\..\cloud\cloud.h"

#pragma pack( push, 1 )
static int tab = 0;

#pragma pack( pop )

namespace nem
{
	void c_menu::init ( IDirect3DDevice9* const device )
	{
		auto& io = ImGui::GetIO ( );

		static const ImWchar ranges[ ] =
		{
			0x0020, 0x00FF,
			0x0400, 0x052F,
			0x2DE0, 0x2DFF,
			0xA640, 0xA69F,
			0,
		};

		io.Fonts->AddFontFromMemoryTTF ( k_font_data, sizeof ( k_font_data ), 18, 0, ranges );

		g_font_big = io.Fonts->AddFontFromMemoryTTF ( k_font_data, sizeof ( k_font_data ), 30, 0, ranges );
		g_font_medium = io.Fonts->AddFontFromMemoryTTF ( k_font_data, sizeof ( k_font_data ), 23, 0, ranges );

		D3DXCreateTextureFromFileInMemory ( device, k_logo_data, sizeof ( k_logo_data ), &m_logo );

	    std::filesystem::create_directories ( xorstr_ ( "off-white/luas" ) );
	}

	void c_menu::draw_multicombo ( const char* name, std::vector<int>& variable, char* labels[ ], int count )
	{
		for ( int i = 0; i < 3; ++i )
			ImGui::Spacing ( );

		static auto howmuchsel = [ ] ( std::vector<int> e ) -> int {
			int s = 0;
			for ( int i = 0; i < e.size ( ); i++ )
				if ( e[ i ] )
					s++;
			return s;
		};

		ImGui::SameLine ( 39.f );
		ImGui::PushItemWidth ( 160.f );

		if ( ImGui::BeginCombo ( name, ( howmuchsel ( variable ) > 0 ? std::to_string ( howmuchsel ( variable ) ) + " selected" : "None" ).c_str ( ) ) ) // draw start
		{
			for ( auto i = 0; i < count; i++ )
				ImGui::Selectable ( labels[ i ], ( bool* )&variable[ i ], ImGuiSelectableFlags_DontClosePopups );

			ImGui::EndCombo ( );
		}

		ImGui::PopItemWidth ( );

		ImGui::Spacing ( );
	}
}