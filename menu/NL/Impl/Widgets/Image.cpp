#include "..\..\Neverlose.h"

C_Element& C_Element::Image( IDirect3DTexture9* pTexture, ImVec2 sPos, ImVec2 sSize )
{
    m_eType = ET_IMAGE;

    m_cMapVars[_h( "Texture" )] = C_Var( pTexture );
    m_cMapVars[_h( "Pos" )] = C_Var( sPos );
    m_cMapVars[_h( "Size" )] = C_Var( sSize );

    m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "ImageCreateCallback" ) );

    return *this;
}

void C_Nl::ImageCreateCallback( C_Element* pElement )
{
    ImGui::SetCursorPos( pElement->GetVar( _h( "Pos" ) ).GetVec2( ) );
    ImGui::Image( pElement->GetVar( _h( "Texture" ) ).GetRef< IDirect3DTexture9 >( ), pElement->GetVar( _h( "Size" ) ).GetVec2( ) );
}
