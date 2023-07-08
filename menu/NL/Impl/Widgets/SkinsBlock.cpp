#include "..\..\..\..\nem.hpp"

C_Element& C_Element::SkinsBlock( )
{
	m_eType = ET_SKINS_COMBO;

	m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "SkinsBlockCreateCallback" ) );

	return *this;
}

void C_Nl::SkinsBlockCreateCallback( C_Element* pElement )
{
	ImGui::PushFont( g_NL->pMenuFont );

	ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 4.f );
	ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 7.f, 3.f ) );
	ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 0.5f );
	ImGui::PushStyleVar( ImGuiStyleVar_PopupRounding, 4.f );
	ImGui::PushStyleVar( ImGuiStyleVar_ScrollbarSize, 3.f );

	ImGui::PushStyleColor( ImGuiCol_Text, RGB_VEC( 153.f, 176.f, 189.f, 255.f ) );
	ImGui::PushStyleColor( ImGuiCol_FrameBg, ImVec4( 13.f / 255.f, 13.f / 255.f, 13.f / 255.f, 255.f / 255.f ) );
	ImGui::PushStyleColor( ImGuiCol_FrameBgHovered, ImVec4( 20.f / 255.f, 20.f / 255.f, 20.f / 255.f, 255.f / 255.f ) );
	ImGui::PushStyleColor( ImGuiCol_PopupBg, ImVec4( 20.f / 255.f, 20.f / 255.f, 20.f / 255.f, 255.f / 255.f ) );
	ImGui::PushStyleColor( ImGuiCol_Header, ImVec4( 45.f / 255.f, 45.f / 255.f, 45.f / 255.f, 80.f / 255.f ) );
	ImGui::PushStyleColor( ImGuiCol_Border, ImVec4( 26.f / 255.f, 26.f / 255.f, 26.f / 255.f, 255.f / 255.f ) );

	static std::size_t cur_wpn{}, cur_paint_kit[nem::hacks::c_skins::k_max_wpns]{};

	ImGui::SetCursorPosY( ImGui::GetCursorPosY() + 10.f );

	auto sPos = ImGui::GetCursorPos();

	ImGui::SetCursorPosX( ImGui::GetCursorPosX() + 13.f );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY() + 10.f );
	ImGui::Text( "Knife" );

	ImGui::SetCursorPos( sPos );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 207.f );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + 10.f );

	ImGui::PushItemWidth( 75.f );

	if ( ImGui::BeginCombo (  "##knife", nem::hacks::g_skins->knives ( ).at ( nem::hacks::g_aim_bot->cfg( ).knife_index ).m_name.data ( ), ImGuiComboFlags_NoArrowButton ) ) {
		for ( std::size_t i {}; i < nem::hacks::c_skins::k_max_knives; ++i ) {
			const auto& knife = nem::hacks::g_skins->knives ( ).at ( i );

			if ( ImGui::Selectable ( knife.m_name.data ( ), nem::hacks::g_aim_bot->cfg( ).knife_index == i ) )
				nem::hacks::g_aim_bot->cfg ( ).knife_index = i;
		}

		ImGui::EndCombo ( );
	}

	ImGui::PopItemWidth();

	ImGui::Spacing ( ); 			ImGui::Spacing ( ); 			ImGui::Spacing ( ); 			ImGui::Spacing ( ); 			ImGui::Spacing ( );
	
	sPos = ImGui::GetCursorPos();

	ImGui::SetCursorPosX( ImGui::GetCursorPosX() + 13.f );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY() + 10.f );
	ImGui::Text( "Weapon" );

	ImGui::SetCursorPos( sPos );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 207.f );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + 10.f );

	ImGui::PushItemWidth( 75.f );

	if ( ImGui::BeginCombo ( "##weapon", nem::hacks::g_skins->weapons ( ).at ( cur_wpn ).m_name.data ( ), ImGuiComboFlags_NoArrowButton ) ) {
		for ( std::size_t i {}; i < nem::hacks::c_skins::k_max_wpns; ++i ) {
			const auto& weapon = nem::hacks::g_skins->weapons ( ).at ( i );

			if ( ImGui::Selectable ( weapon.m_name.data ( ), cur_wpn == i ) )
				cur_wpn = i;
		}

		ImGui::EndCombo ( );
	}

	ImGui::PopItemWidth();

	auto& menu_paint_kit = cur_paint_kit[ cur_wpn ];
	auto& cfg_paint_kit = nem::hacks::g_aim_bot->cfg( ).paint_kits[cur_wpn];

	static const auto paint_kits_count = nem::hacks::g_skins->paint_kits ( ).size ( );
	for ( std::size_t i {}; i < paint_kits_count; ++i )
		if ( cfg_paint_kit == nem::hacks::g_skins->paint_kits ( ).at ( i ).m_id )
			menu_paint_kit = i;

	ImGui::Spacing ( ); 			ImGui::Spacing ( ); 			ImGui::Spacing ( ); 			ImGui::Spacing ( ); 			ImGui::Spacing ( );
	
	sPos = ImGui::GetCursorPos();

	ImGui::SetCursorPosX( ImGui::GetCursorPosX() + 13.f );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY() + 10.f );
	ImGui::Text( "Paint Kit" );

	ImGui::SetCursorPos( sPos );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 207.f );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + 10.f );

	ImGui::PushItemWidth( 75.f );

	if ( ImGui::BeginCombo ( "##paint kit", nem::hacks::g_skins->paint_kits ( ).at ( menu_paint_kit ).m_name.data ( ), ImGuiComboFlags_NoArrowButton ) ) {
		for ( std::size_t i {}; i < paint_kits_count; ++i ) {
			const auto& paint_kit = nem::hacks::g_skins->paint_kits ( ).at ( i );

			if ( ImGui::Selectable ( paint_kit.m_name.data ( ), cfg_paint_kit == paint_kit.m_id ) ) {
				cfg_paint_kit = paint_kit.m_id;
				menu_paint_kit = i;
			}
		}

		ImGui::EndCombo ( );
	}

	ImGui::PopItemWidth();

	ImGui::PopStyleColor( 6 );
	ImGui::PopStyleVar( 5 );

	ImGui::PopFont();
}