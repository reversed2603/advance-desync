#include "..\..\Neverlose.h"

C_Element& C_Element::ColorPicker( float flOffsetX, ImVec2 sPos, float* pResult )
{
    m_eType = ET_COLOR_PICKER;

    m_cMapVars[_h( "Origin" )] = C_Var( sPos );
    m_cMapVars[_h( "ValuePointer" )] = C_Var( pResult );
    m_cMapVars[_h( "OffsetX" )] = C_Var( flOffsetX );

    m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "ColorPickerCreateCallback" ) );

    return *this;
}

void C_Nl::ColorPickerCreateCallback( C_Element* pElement )
{
    ImVec2 sPos = pElement->GetVar( _h( "Origin" ) ).GetVec2( );
    float* pValue = pElement->GetVar( _h( "ValuePointer" ) ).GetRef<float>( );
    float flOffsetX = pElement->GetVar( _h( "OffsetX" ) ).GetFloat( );

    ImGui::SetCursorPos( sPos );
    ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + flOffsetX );

    if ( g_NL->CheckMouseIntersectionLocal( ImGui::GetCursorPos( ), ImVec2( 19.f, 20.f ) ) && ImGui::GetIO( ).MouseClicked[0] )
    {
        g_NL->m_bSelectingShit = true;
        g_ColorPicker->EnableColorpicker( pValue );
    }

    ImGui::Image( g_NL->m_pIcons[IC_COLOR_PICKER], ImVec2( 19.f, 20.f ) );
}
