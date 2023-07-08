#include "..\..\..\Neverlose.h"

C_Element& C_Element::DrawDropText( const char* szText, ImVec2 sPos, ImVec4 sColor, ImFont* pFont )
{
    m_eType = ET_DRAG_DROP_TEXT_CONVERT;

    m_cMapVars[_h( "Text" )] = C_Var( std::string( szText ) );
    m_cMapVars[_h( "Origin" )] = C_Var( sPos );
    m_cMapVars[_h( "Font" )] = C_Var( pFont );
    m_cMapVars[_h( "Color" )] = C_Var( sColor );

    m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "DrawDropTextCreateCallback" ) );

    return *this;
}

void C_Nl::DrawDropTextCreateCallback( C_Element* pElement )
{
    pElement->GetVar( _h( "Color" ) ).GetVec4( ).w = ImGui::GetStyle( ).Alpha;

    ImVec2 sPos = pElement->GetVar( _h( "Origin" ) ).GetVec2( );

    ImVec2 sTextPos = ImGui::GetWindowPos( );

    sTextPos.x += sPos.x;
    sTextPos.y += sPos.y;

    ImGui::PushFont( pElement->GetVar( _h( "Font" ) ).GetRef<ImFont>( ) );

    ImGui::GetForegroundDrawList( )->AddText( sTextPos,
        ImGui::ColorConvertFloat4ToU32( pElement->GetVar( _h( "Color" ) ).GetVec4( ) ),
        pElement->GetVar( _h( "Text" ) ).GetString( ).c_str( ) );

    ImGui::PopFont( );
}

int C_Nl::DynamicTextMouseIntersectCheck( C_Element* pElement )
{
    return g_NL->CheckMouseIntersectionLocal( pElement->GetVar( _h( "Origin" ) ).GetVec2( ), ImGui::CalcTextSize( pElement->GetVar( _h( "Text" ) ).GetString( ).c_str( ) ) );
}
