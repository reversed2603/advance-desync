#include "..\..\Neverlose.h"

float g_flLastAlpha = 0.f;

C_Element& C_Element::ChildBegin( const char* pText, ImVec2 sPos, ImVec2 sSize, bool bWindowed, int iTabIndex )
{
    m_eType = ET_CHILD_BEGIN;

    m_cMapVars[_h( "Text" )] = C_Var( std::string( pText ) );
    m_cMapVars[_h( "Origin" )] = C_Var( sPos );
    m_cMapVars[_h( "StartOrigin" )] = C_Var( sPos );
    m_cMapVars[_h( "Size" )] = C_Var( sSize );
    m_cMapVars[_h( "Windowed" )] = C_Var( bWindowed );
    m_cMapVars[_h( "Alpha" )] = C_Var( 1.f );
    m_cMapVars[_h( "ConnectedTab" )] = C_Var( iTabIndex );

    m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "ChildBeginCreateCallback" ) );

    return *this;
}

C_Element& C_Element::ChildEnd( bool bWindowed, int iTabIndex )
{
    m_eType = ET_CHILD_END;

    m_cMapVars[_h( "Windowed" )] = C_Var( bWindowed );

    m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "ChildEndCreateCallback" ) );

    return *this;
}

void C_Nl::ChildBeginCreateCallback( C_Element* pElement )
{
    int iTab = pElement->GetVar( _h( "ConnectedTab" ) ).GetInt( );

    g_NL->m_iCurrentTabRendering = iTab;

    if ( g_NL->m_iSelectedTab != iTab && g_NL->m_bTabFullySwitched )
    {
        g_NL->m_bSkipElements = true;
        return;
    }
    
    g_flLastAlpha = ImGui::GetStyle( ).Alpha;
    ImGui::GetStyle( ).Alpha = std::clamp( pElement->GetVar( _h( "Alpha" ) ).GetFloat( ), 0.f, g_flLastAlpha );

    ImVec2 sPos = pElement->GetVar( _h( "Origin" ) ).GetVec2( );
    ImVec2 sSize = pElement->GetVar( _h( "Size" ) ).GetVec2( );

    ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 6.f );
    ImGui::PushStyleColor( ImGuiCol_ChildBg, RGB_VEC( 12.f, 11.f, 14.f, 255.f ) );

    if ( pElement->GetVar( _h( "Windowed" ) ).GetInt( ) )
    {
        ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
        ImGui::PushStyleColor( ImGuiCol_WindowBg, RGB_VEC( 12.f, 11.f, 14.f, 255.f ) );

        ImGui::SetNextWindowPos( sPos );
        ImGui::SetNextWindowSize( sSize );

        ImGui::Begin( pElement->GetVar( _h( "Text" ) ).GetString( ).c_str( ), 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus );
    }
    else
    {
        ImGui::SetCursorPos( sPos );
        ImGui::BeginChild( std::string( "##she_ne_vmerla_" ).append( pElement->GetVar( _h( "Text" ) ).GetString( ) ).c_str( ), 0, sSize, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus );
    }

    ImGui::PushFont( g_NL->pMenuFont );

    ImGui::SetCursorPos( ImVec2( 13.f, 8.f ) );
    ImGui::Text( pElement->GetVar( _h( "Text" ) ).GetString( ).c_str( ) );

    ImGui::PopFont( );

    ImVec2 sLineStart = ImVec2( 0, 0 );

    sLineStart.x += ImGui::GetWindowPos( ).x;
    sLineStart.y += ImGui::GetWindowPos( ).y;

    sLineStart.x += 7.f;
    sLineStart.y += 38.f;

    ImVec2 sLineEnd = sLineStart;

    sLineEnd.x = sLineStart.x + sSize.x - 14.f;
    sLineEnd.y = sLineStart.y + 3.f;

    ImGui::GetWindowDrawList( )->AddRectFilled( sLineStart, sLineEnd, ImGui::ColorConvertFloat4ToU32( RGB_VEC( 26.f, 25.f, 28.f, ImGui::GetStyle( ).Alpha * 255.f ) ) );
}

void C_Nl::ChildEndCreateCallback( C_Element* pElement )
{
    if ( g_NL->m_bSkipElements )
    {
        g_NL->m_bSkipElements = false;
        return;
    }

    ImGui::GetStyle( ).Alpha = g_flLastAlpha;

    if ( pElement->GetVar( _h( "Windowed" ) ).GetInt( ) )
    {
        ImGui::PopStyleColor( );
        ImGui::PopStyleVar( );

        ImGui::End( );
    }
    else
        ImGui::EndChild( );

    ImGui::PopStyleColor( );
    ImGui::PopStyleVar( );
}
