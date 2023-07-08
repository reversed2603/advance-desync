#include "..\..\Neverlose.h"

C_Element& C_Element::Tab( ImVec2 sPos, IDirect3DTexture9* pIcon, ImVec2 sIconSize, const char* pText, int* pSelectedTab, int iCurrentTab, ImVec2 sTextOffset )
{
    m_eType = ET_TAB;

    m_cMapVars[_h( "Pos" )] = C_Var( sPos );
    m_cMapVars[_h( "Icon" )] = C_Var( pIcon );
    m_cMapVars[_h( "IconSize" )] = C_Var( sIconSize );
    m_cMapVars[_h( "Text" )] = C_Var( std::string( pText ) );
    m_cMapVars[_h( "TextOffset" )] = C_Var( sTextOffset );
    m_cMapVars[_h( "SelectedTab" )] = C_Var( pSelectedTab );
    m_cMapVars[_h( "CurrentTab" )] = C_Var( iCurrentTab );
    m_cMapVars[_h( "Alpha" )] = C_Var( *pSelectedTab == iCurrentTab ? 255.f : 0.f );
    m_cMapVars[_h( "ConXOffset" )] = C_Var( sTextOffset.x != 35.f ? 28.f - sPos.x : 0.f );

    m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "TabCreateCallback" ) );

    return *this;
}

void C_Nl::TabCreateCallback( C_Element* pElement )
{
    auto sPos = pElement->GetVar( _h( "Pos" ) ).GetVec2( );
    auto pIcon = pElement->GetVar( _h( "Icon" ) ).GetRef< IDirect3DTexture9 >( );
    auto sIconSize = pElement->GetVar( _h( "IconSize" ) ).GetVec2( );
    auto& sText = pElement->GetVar( _h( "Text" ) ).GetString( );
    auto sTextOffset = pElement->GetVar( _h( "TextOffset" ) ).GetVec2( );
    int* pSelectedTab = pElement->GetVar( _h( "SelectedTab" ) ).GetRef<int>( );
    int iCurrentTab = pElement->GetVar( _h( "CurrentTab" ) ).GetInt( );

    float flConXOffset = pElement->GetVar( _h( "ConXOffset" ) ).GetFloat( );

    if ( g_NL->m_pTabs.size( ) < iCurrentTab + 1 )
        g_NL->m_pTabs.push_back( pElement );

    auto sStartPos = ImGui::GetWindowPos( );

    sStartPos.x += sPos.x - 16.f;
    sStartPos.y += sPos.y - 8.f;

    auto sEndPos = sStartPos;

    sEndPos.x += 213.f;
    sEndPos.y += 38.f;

    auto sMousePos = ImGui::GetIO( ).MousePos;

    bool bClickedOnObject = sMousePos.x >= sStartPos.x &&
        sMousePos.x <= sEndPos.x &&
        sMousePos.y >= sStartPos.y &&
        sMousePos.y <= sEndPos.y &&
        ImGui::GetIO( ).MouseClicked[0];

    if ( bClickedOnObject && g_NL->m_iSelectedTab != iCurrentTab )
    {
        ADD_ANIMATION( C_Animation( ).AlphaAnimate( g_NL->m_pTabs[g_NL->m_iSelectedTab], AT_HIGH_TO_LOW, 0.2f, 0.f ) );
        ADD_ANIMATION( C_Animation( ).AlphaAnimate( g_NL->m_pTabs[iCurrentTab], AT_HIGH_TO_LOW, 0.2f, 255.f ) );

        int iPrevTab = g_NL->m_iSelectedTab;

        g_NL->m_iSelectedTab = iCurrentTab;

        OnTabChangeCallback( pElement, iPrevTab, iCurrentTab );
    }

    ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 6.f );
    ImGui::PushStyleColor( ImGuiCol_ChildBg, RGB_VEC( 57.f, 57.f, 58.f, pElement->GetVar( _h( "Alpha" ) ).GetFloat( ) ) );

    ImGui::SetCursorPos( ImVec2( sPos.x - 16.f + flConXOffset, sPos.y - 8.f ) );

    ImGui::BeginChild( std::string( "##allah_" ).append( sText ).c_str( ), 0, ImVec2( 213.f, 38.f ) );
    PostProcessing::performFullscreenBlur( ImGui::GetWindowDrawList( ), 0.2f * ImGui::GetStyle( ).Alpha * ( pElement->GetVar( _h( "Alpha" ) ).GetFloat( ) / 255.f ) );
    ImGui::EndChild( );

    ImGui::PopStyleColor( );
    ImGui::PopStyleVar( );

    ImGui::SetCursorPos( sPos );
    ImGui::Image( pIcon, sIconSize );

    ImGui::PushFont( g_NL->pTabNameFont );
    ImGui::SetCursorPos( ImVec2( sPos.x + sTextOffset.x, sPos.y + sTextOffset.y ) );
    ImGui::Text( sText.c_str( ) );
    ImGui::PopFont( );
}

void C_Nl::OnTabChangeCallback( C_Element* pElement, int iPrevTab, int iCurrentTab )
{
    g_NL->m_bTabFullySwitched = false;

    bool bReset = true;
    bool bClear = false;

    for ( auto& cElem : g_NL->m_cVecElemets )
    {
        if ( cElem.GetType( ) == ET_CHILD_BEGIN && cElem.GetVar( _h( "ConnectedTab" ) ).GetInt( ) != iPrevTab && cElem.GetVar( _h( "ConnectedTab" ) ).GetInt( ) != g_NL->m_iSelectedTab )
        {
            ImVec2 sCurrOrigin = cElem.GetVar( _h( "Origin" ) ).GetVec2( );
            sCurrOrigin.y += 15.f;

            cElem.GetVar( _h( "Alpha" ) ).GetFloat( ) = 0.f;
            cElem.GetVar( _h( "Origin" ) ).GetVec2( ) = sCurrOrigin;
        }

        if ( cElem.GetType( ) == ET_CHILD_BEGIN && cElem.GetVar( _h( "ConnectedTab" ) ).GetInt( ) == iPrevTab )
        {
            ImVec2 sCurrOrigin = cElem.GetVar( _h( "Origin" ) ).GetVec2( );
            sCurrOrigin.y += 15.f;

            ADD_ANIMATION( C_Animation( ).AlphaAnimate( &cElem, AT_HIGH_TO_LOW, 0.1f, 0.f ) );
            ADD_ANIMATION( C_Animation( ).OriginMove( &cElem, AT_HIGH_TO_LOW, 0.1f, sCurrOrigin, [ ]( C_Animation* pAnimation ) -> void { g_NL->m_bTabFullySwitched = true; } ) );
        }

        if ( cElem.GetType( ) == ET_CHILD_BEGIN && cElem.GetVar( _h( "ConnectedTab" ) ).GetInt( ) == g_NL->m_iSelectedTab )
        {
            ImVec2 sCurrOrigin = cElem.GetVar( _h( "StartOrigin" ) ).GetVec2( );
            sCurrOrigin.y += 15.f;

            cElem.GetVar( _h( "Origin" ) ).GetVec2( ) = sCurrOrigin;
            cElem.GetVar( _h( "Alpha" ) ).GetFloat( ) = 0.f;

            ADD_ANIMATION( C_Animation( ).AlphaAnimate( &cElem, AT_HIGH_TO_LOW, 0.1f * 2.f, 1.f ) );
            ADD_ANIMATION( C_Animation( ).OriginMove( &cElem, AT_HIGH_TO_LOW, 0.1f * 2.f, cElem.GetVar( _h( "StartOrigin" ) ).GetVec2( ) ) );

            bClear = true;
        }

        if ( cElem.GetType( ) == ET_CHILD_END )
            bClear = false;
        
        if ( bClear && cElem.GetType( ) == ET_CHECKBOX )
            cElem.ResetCheckbox( );

        if ( bClear && cElem.GetType( ) == ET_SLIDER )
            cElem.ResetSlider( );
    }
}
