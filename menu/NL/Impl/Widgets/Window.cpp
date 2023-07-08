#include "..\..\Neverlose.h"

C_Element& C_Element::WindowBegin( const char* szText, ImVec2 sPos, ImVec2 sSize, ImVec4 sColor, ImGuiWindowFlags uFlags, bool bZeroBorder )
{
    m_eType = ET_WINDOW_BEGIN;

    m_cMapVars[_h( "Text" )] = C_Var( std::string( szText ) );
    m_cMapVars[_h( "Origin" )] = C_Var( sPos );
    m_cMapVars[_h( "Size" )] = C_Var( sSize );
    m_cMapVars[_h( "Color" )] = C_Var( sColor );
    m_cMapVars[_h( "Flags" )] = C_Var( uFlags );
    m_cMapVars[_h( "ZeroBorder" )] = C_Var( bZeroBorder );

    m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "WindowBeginCreateCallback" ) );

    return *this;
}

C_Element& C_Element::WindowEnd( )
{
    m_eType = ET_WINDOW_END;

    m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "WindowEndCreateCallback" ) );

    return *this;
}

void C_Nl::WindowBeginCreateCallback( C_Element* pElement )
{
    auto& cText = pElement->GetVar( _h( "Text" ) ).GetString( );

    bool bZeroBorder = pElement->GetVar( _h( "ZeroBorder" ) ).GetInt( );

    if ( bZeroBorder )
        ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );

    ImGui::PushStyleColor( ImGuiCol_WindowBg, pElement->GetVar( _h( "Color" ) ).GetVec4( ) );

    ImGui::SetNextWindowPos( pElement->GetVar( _h( "Origin" ) ).GetVec2( ) );
    ImGui::SetNextWindowSize( pElement->GetVar( _h( "Size" ) ).GetVec2( ) );

    ImGui::Begin( cText.c_str( ), 0, pElement->GetVar( _h( "Flags" ) ).GetInt( ) | ImGuiWindowFlags_NoBringToFrontOnFocus );
    
    if ( cText == "Tabs" )
    {
        PostProcessing::performFullscreenBlur( ImGui::GetWindowDrawList( ), 0.70f * ImGui::GetStyle( ).Alpha );
    }

    ImGui::PopStyleColor( );

    if ( bZeroBorder )
        ImGui::PopStyleVar( );
}

void C_Nl::WindowEndCreateCallback( C_Element* pElement )
{
    ImGui::End( );
}
