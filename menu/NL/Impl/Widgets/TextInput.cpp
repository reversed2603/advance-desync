#include "..\..\Neverlose.h"

C_Element& C_Element::TextInput( const char* szText, ImVec2 sPos, ImVec2 sSize, char* pOut )
{
	m_eType = ET_TEXT_INPUT;

    m_cMapVars[_h( "Text" )] = C_Var( std::string( szText ) );
    m_cMapVars[_h( "Origin" )] = C_Var( sPos );
    m_cMapVars[_h( "Size" )] = C_Var( sSize );
    m_cMapVars[_h( "Out" )] = C_Var( pOut );

    m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "TextInputCreateCallback" ) );

    m_cMapVars[_h( "TextColor" )] = C_Var( RGB_VEC( 153.f, 176.f, 189.f, 255.f ) );
    m_cMapVars[_h( "ChildBorder" )] = C_Var( RGB_VEC( 3.f, 23.f, 37.f, 255.f ) );
    m_cMapVars[_h( "TextState" )] = C_Var( 0 );
    m_cMapVars[_h( "BorderState" )] = C_Var( 0 );
    m_cMapVars[_h( "Alpha" )] = C_Var( 0.f );
    m_cMapVars[_h( "OpenScale" )] = C_Var( 0.f );
    m_cMapVars[_h( "OpenState" )] = C_Var( 0 );
    m_cMapVars[_h( "BgState" )] = C_Var( 0 );

    m_cMapVars[_h( "ChildBgColor" )] = C_Var( RGB_VEC( 4.f, 5.f, 14.f, 255.f ) );

	return *this;
}

void C_Nl::TextInputCreateCallback( C_Element* pElement )
{
    ImVec2 sPos = pElement->GetVar( _h( "Origin" ) ).GetVec2( );
    char* pOut = pElement->GetVar( _h( "Out" ) ).GetRef<char>( );

    ImGui::PushStyleColor( ImGuiCol_FrameBg, RGB_VEC( 13.f, 13.f, 13.f, 255.f ) );
    ImGui::PushStyleColor( ImGuiCol_Border, RGB_VEC( 28.f, 28.f, 31.f, 200.f ) );
    ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.5f );
    ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.f );

    ImGui::SetCursorPos( sPos );

    ImGui::PushFont( g_NL->pMenuFont );

    ImGui::PushItemWidth( 265.f );
    ImGui::InputText( pElement->GetVar( _h( "Text" ) ).GetString( ).c_str( ), pOut, 256 );
    ImGui::PopItemWidth( );

    ImGui::PopFont( );

    ImGui::PopStyleVar( 2 );
    ImGui::PopStyleColor( 2 );
}