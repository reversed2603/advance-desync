#include "..\..\Neverlose.h"

C_Element& C_Element::RectFilled( ImVec2 sStart, ImVec2 sEnd, ImVec4 sColor )
{
    m_eType = ET_RECT_FILLED;

    m_cMapVars[_h( "Start" )] = C_Var( sStart );
    m_cMapVars[_h( "End" )] = C_Var( sEnd );
    m_cMapVars[_h( "Color" )] = C_Var( sColor );

    m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "RectFilledCreateCallback" ) );

    return *this;
}

void C_Nl::RectFilledCreateCallback( C_Element* pElement )
{
    pElement->GetVar( _h( "Color" ) ).GetVec4( ).w = ImGui::GetStyle( ).Alpha;
    
    ImGui::GetForegroundDrawList( )->AddRectFilled(
        pElement->GetVar( _h( "Start" ) ).GetVec2( ),
        pElement->GetVar( _h( "End" ) ).GetVec2( ),
        ImGui::ColorConvertFloat4ToU32( pElement->GetVar( _h( "Color" ) ).GetVec4( ) ) );
}
