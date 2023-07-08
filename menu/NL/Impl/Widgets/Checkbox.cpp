#include "..\..\Neverlose.h"

void C_Element::ResetCheckbox( )
{
    m_cMapVars[_h( "TextColor" )] = C_Var( RGB_VEC( 153.f, 176.f, 189.f, 255.f ) );
    m_cMapVars[_h( "ChildBgColor" )] = C_Var( RGB_VEC( 5.f, 5.f, 5.f, 255.f ) );
    m_cMapVars[_h( "CircleColor" )] = C_Var( RGB_VEC( 125.f, 138.f, 148.f, 255.f ) );

    m_cMapVars[_h( "LastValue" )] = C_Var( 0 );
    m_cMapVars[_h( "TextState" )] = C_Var( 0 );
    m_cMapVars[_h( "BoxState" )] = C_Var( 0 );
    m_cMapVars[_h( "CircleState" )] = C_Var( 0 );
    m_cMapVars[_h( "CircleXOffset" )] = C_Var( 0 );
}

C_Element& C_Element::Checkbox( uint32_t u32HashIndex, const char* szText, ImVec2 sPos, bool* pValue )
{
    m_eType = ET_CHECKBOX;

    m_cMapVars[_h( "Text" )] = C_Var( std::string( szText ) );
    m_cMapVars[_h( "Origin" )] = C_Var( sPos );
    m_cMapVars[_h( "ValuePointer" )] = C_Var( pValue );
    m_cMapVars[_h( "HashIndex" )] = C_Var( u32HashIndex );

    m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "CheckboxCreateCallback" ) );

    m_cMapVars[_h( "TextColor" )] = C_Var( RGB_VEC( 153.f, 176.f, 189.f, 255.f ) );
    m_cMapVars[_h( "ChildBgColor" )] = C_Var( RGB_VEC( 5.f, 5.f, 5.f, 255.f ) );
    m_cMapVars[_h( "CircleColor" )] = C_Var( RGB_VEC( 125.f, 138.f, 148.f, 255.f ) );

    m_cMapVars[_h( "LastValue" )] = C_Var( 0 );
    m_cMapVars[_h( "TextState" )] = C_Var( 0 );
    m_cMapVars[_h( "BoxState" )] = C_Var( 0 );
    m_cMapVars[_h( "CircleState" )] = C_Var( 0 );
    m_cMapVars[_h( "CircleXOffset" )] = C_Var( 0 );

    m_cMapVars[_h( "ConnectedTab" )] = C_Var( g_NL->m_iCurrentTabSetup );

    return *this;
}

void C_Nl::CheckboxCreateCallback( C_Element* pElement )
{
    //ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO( ).Framerate, ImGui::GetIO( ).Framerate );

    std::string& cText = pElement->GetVar( _h( "Text" ) ).GetString( );
    ImVec2 sPos = pElement->GetVar( _h( "Origin" ) ).GetVec2( );
    bool* pValue = pElement->GetVar( _h( "ValuePointer" ) ).GetRef<bool>( );
    int& iLastValue = pElement->GetVar( _h( "LastValue" ) ).GetInt( );
    int& iConnectedTab = pElement->GetVar( _h( "ConnectedTab" ) ).GetInt( );
    uint32_t u32Hash = pElement->GetVar( _h( "HashIndex" ) ).GetUInt32( );

    pElement->GetVar( _h( "TextColor" ) ).GetVec4( ).w = ImGui::GetStyle( ).Alpha;
    pElement->GetVar( _h( "ChildBgColor" ) ).GetVec4( ).w = ImGui::GetStyle( ).Alpha;
    pElement->GetVar( _h( "CircleColor" ) ).GetVec4( ).w = ImGui::GetStyle( ).Alpha;

    ImGui::SetCursorPos( sPos );

    ImGui::PushFont( g_NL->pMenuFont );
    ImGui::PushStyleColor( ImGuiCol_Text, pElement->GetVar( _h( "TextColor" ) ).GetVec4( ) );
    ImGui::Text( cText.c_str( ) );
    ImGui::PopStyleColor( );
    ImGui::PopFont( );

    ImGui::SetCursorPosY( sPos.y + 3.f );
    ImGui::SetCursorPosX( ImGui::GetWindowSize( ).x - 44.f );

    ImGui::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 1.3f );
    ImGui::PushStyleColor( ImGuiCol_Border, RGB_VEC( 3.f, 23.f, 40.f, ImGui::GetStyle( ).Alpha ) );
    ImGui::PushStyleColor( ImGuiCol_ChildBg, pElement->GetVar( _h( "ChildBgColor" ) ).GetVec4( ) );
    ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 9.f );

    auto pDrawList = ImGui::GetWindowDrawList( );

    ImGui::BeginChild( std::string( "##sel_chld_" ).append( cText ).c_str( ), 0, ImVec2( 31.f, 15.f ), true );
    ImGui::GetStyle ( ).AntiAliasedFill = true;
    ImGui::GetStyle ( ).AntiAliasedLines = true;
    pDrawList->AddCircleFilled( ImVec2( ImGui::GetWindowPos( ).x + 7.f + pElement->GetVar( _h( "CircleXOffset" ) ).GetInt( ), ImGui::GetWindowPos( ).y + 7.25f ), 9.1f, ImGui::ColorConvertFloat4ToU32( pElement->GetVar( _h( "CircleColor" ) ).GetVec4( ) ), 25 );

    ImGui::EndChild( );

    ImGui::PopStyleVar( );
    ImGui::PopStyleColor( 2 );
    ImGui::PopStyleVar( );

    bool bIntersected = g_NL->CheckMouseIntersectionLocal( sPos, ImVec2( ImGui::GetWindowSize( ).x - 20.f, 30.f ) );

    if ( !g_NL->m_bSelectingShit && bIntersected && ImGui::GetIO( ).MouseClicked[0] && iConnectedTab == g_NL->m_iCurrentTabRendering )
        *pValue = !*pValue;

    if ( u32Hash && bIntersected && ImGui::GetIO( ).MouseClicked[1] )
        g_Keybinds->EnableKeybindSelection( pElement, ET_CHECKBOX, u32Hash );

    if ( iLastValue != *pValue )
    {
        iLastValue = *pValue;
        g_NL->OnMouseClickIntersectCheckboxCallback( pElement );
    }

    g_NL->OnMouseCallbackTextColorChangeCheckbox( pElement, bIntersected );
    g_NL->OnMouseCheckboxCallbackBoxColorChange( pElement, bIntersected );
}

void C_Nl::OnMouseCallbackTextColorChangeCheckbox( C_Element* pElement, bool bMouseIntersect )
{
    int& iState = pElement->GetVar( _h( "TextState" ) ).GetInt( );
    int& iCircleState = pElement->GetVar( _h( "CircleState" ) ).GetInt( );

    if ( iCircleState )
        bMouseIntersect = true;
    
    if ( !iState && bMouseIntersect )
    {
        g_NL->ClearElementAnimations( pElement, _h( "TextRedAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "TextGreenAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "TextBlueAnimate" ) );

        ADD_ANIMATION( C_Animation( ).TextRedAnimate( pElement, AT_HIGH_TO_LOW, 0.3f, 242.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).TextGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.3f, 242.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).TextBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.3f, 242.f / 255.f ) );

        iState = 1;
    }
    else if ( iState && !bMouseIntersect )
    {
        g_NL->ClearElementAnimations( pElement, _h( "TextRedAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "TextGreenAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "TextBlueAnimate" ) );

        ADD_ANIMATION( C_Animation( ).TextRedAnimate( pElement, AT_HIGH_TO_LOW, 0.3f, 153.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).TextGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.3f, 176.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).TextBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.3f, 189.f / 255.f ) );

        iState = 0;
    }
}

void C_Nl::OnMouseClickIntersectCheckboxCallback( C_Element* pElement )
{
    bool* pValue = pElement->GetVar( _h( "ValuePointer" ) ).GetRef<bool>( );
    int& iCircleState = pElement->GetVar( _h( "CircleState" ) ).GetInt( );

    iCircleState = !iCircleState;
    *pValue = iCircleState;

    g_NL->ClearElementAnimations( pElement, _h( "CircleXOffsetAnimate" ) );

    g_NL->ClearElementAnimations( pElement, _h( "ChildRedAnimate" ) );
    g_NL->ClearElementAnimations( pElement, _h( "ChildGreenAnimate" ) );
    g_NL->ClearElementAnimations( pElement, _h( "ChildBlueAnimate" ) );

    g_NL->ClearElementAnimations( pElement, _h( "CircleRedAnimate" ) );
    g_NL->ClearElementAnimations( pElement, _h( "CircleGreenAnimate" ) );
    g_NL->ClearElementAnimations( pElement, _h( "CircleBlueAnimate" ) );

    if ( iCircleState )
    {
        ADD_ANIMATION( C_Animation( ).CircleXOffsetAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 16.f ) );

        ADD_ANIMATION( C_Animation( ).ChildRedAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 30.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 30.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 32.f / 255.f ) );

        ADD_ANIMATION( C_Animation( ).CircleRedAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 242.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).CircleGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 242.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).CircleBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 242.f / 255.f ) );
    }
    else
    {
        ADD_ANIMATION( C_Animation( ).CircleXOffsetAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 0.f ) );

        ADD_ANIMATION( C_Animation( ).ChildRedAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 5.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 5.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 5.f / 255.f ) );

        ADD_ANIMATION( C_Animation( ).CircleRedAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 125.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).CircleGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 138.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).CircleBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 148.f / 255.f ) );
    }
}

void C_Nl::OnMouseCheckboxCallbackBoxColorChange( C_Element* pElement, bool bMouseIntersect )
{
    int& iCircleState = pElement->GetVar( _h( "CircleState" ) ).GetInt( );
    int& iBoxState = pElement->GetVar( _h( "BoxState" ) ).GetInt( );

    if ( iCircleState )
    {
        iBoxState = 1;
        return;
    }

    if ( !iBoxState && bMouseIntersect )
    {
        g_NL->ClearElementAnimations( pElement, _h( "CircleRedAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "CircleGreenAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "CircleBlueAnimate" ) );

        g_NL->ClearElementAnimations( pElement, _h( "ChildRedAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "ChildGreenAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "ChildBlueAnimate" ) );

        ADD_ANIMATION( C_Animation( ).CircleRedAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 148.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).CircleGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 155.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).CircleBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 155.f / 255.f ) );

        ADD_ANIMATION( C_Animation( ).ChildRedAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 9.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 9.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 9.f / 255.f ) );

        iBoxState = 1;
    }
    else if ( iBoxState && !bMouseIntersect )
    {
        g_NL->ClearElementAnimations( pElement, _h( "CircleRedAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "CircleGreenAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "CircleBlueAnimate" ) );

        g_NL->ClearElementAnimations( pElement, _h( "ChildRedAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "ChildGreenAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "ChildBlueAnimate" ) );

        ADD_ANIMATION( C_Animation( ).CircleRedAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 125.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).CircleGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 138.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).CircleBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 148.f / 255.f ) );

        ADD_ANIMATION( C_Animation( ).ChildRedAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 3.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 5.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 7.f / 255.f ) );

        iBoxState = 0;
    }
}
