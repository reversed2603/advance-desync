#include "..\..\Neverlose.h"

#include "..\..\Deps\KeysList\KeysList.h"

C_Element& C_Element::KeybindValue( const char* szText, ImVec2 sPos, int* pResult )
{
    m_eType = ET_SLIDER;

    m_cMapVars[_h( "Text" )] = C_Var( std::string( szText ) );
    m_cMapVars[_h( "Origin" )] = C_Var( sPos );
    m_cMapVars[_h( "ValuePointer" )] = C_Var( pResult );

    m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "KeybindValueCreateCallback" ) );

    m_cMapVars[_h( "TextColor" )] = C_Var( RGB_VEC( 153.f, 176.f, 189.f, 255.f ) );
    m_cMapVars[_h( "ChildBorder" )] = C_Var( RGB_VEC( 26.f, 26.f, 26.f, 255.f ) );
    m_cMapVars[_h( "TextState" )] = C_Var( 0 );
    m_cMapVars[_h( "BorderState" )] = C_Var( 0 );
    m_cMapVars[_h( "Alpha" )] = C_Var( 0.f );
    m_cMapVars[_h( "OpenScale" )] = C_Var( 0.f );
    m_cMapVars[_h( "OpenState" )] = C_Var( 0 );
    m_cMapVars[_h( "BgState" )] = C_Var( 0 );

    m_cMapVars[_h( "ChildBgColor" )] = C_Var( RGB_VEC( 13.f, 13.f, 13.f, 255.f ) );

    return *this;
}

void C_Nl::KeybindValueCreateCallback( C_Element* pElement )
{
    std::string& cText = pElement->GetVar( _h( "Text" ) ).GetString( );
    ImVec2 sPos = pElement->GetVar( _h( "Origin" ) ).GetVec2( );
    int* pValue = pElement->GetVar( _h( "ValuePointer" ) ).GetRef<int>( );
    float flGlowAlpha = pElement->GetVar( _h( "Alpha" ) ).GetFloat( );
    ImVec4& sChildBg = pElement->GetVar( _h( "ChildBgColor" ) ).GetVec4( );
    int& iOpenState = pElement->GetVar( _h( "OpenState" ) ).GetInt( );

    if ( iOpenState && g_Keybinds->m_iLastPressedKey )
    {
        *pValue = g_Keybinds->m_iLastPressedKey;

        g_NL->ClearElementAnimations( pElement, _h( "OpenScaleAnimate" ) );

        ADD_ANIMATION( C_Animation( ).OpenScaleAnimate( pElement, AT_HIGH_TO_LOW, 0.15f, 0.f ) );
        iOpenState = 0;
        g_NL->m_bSelectingShit = 2;
    }

    ImGui::SetCursorPos( sPos );

    ImGui::PushFont( g_NL->pMenuFont );
    ImGui::PushStyleColor( ImGuiCol_Text, pElement->GetVar( _h( "TextColor" ) ).GetVec4( ) );
    ImGui::Text( cText.c_str( ) );
    ImGui::PopStyleColor( );
    ImGui::PopFont( );

    ImGui::PushFont( g_NL->pMenuFont );

    constexpr float flOffsetX = -63.f;
    constexpr float flOffsetY = -4.f;

    constexpr float flAddSizeX = 55.f;
    float flAddSizeY = 4.f;

    ImGui::SetCursorPosY( flOffsetY + sPos.y - 1.f );
    ImGui::SetCursorPosX( flOffsetX + ImGui::GetWindowSize( ).x - 77.f );

    ImGui::PushStyleColor( ImGuiCol_ChildBg, RGB_VEC( 4.f, 5.f, 14.f, flGlowAlpha ) );
    ImGui::PushStyleColor( ImGuiCol_Border, RGB_VEC( 30.f, 30.f, 32.f, flGlowAlpha * 0.5f ) );
    ImGui::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 4.f );
    ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 3.f );


    ImGui::BeginChild( "##test_chld1", 0, ImVec2( 74.f + flAddSizeX, 27.f + flAddSizeY ), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus );

    ImGui::EndChild( );

    ImGui::PopStyleVar( 2 );
    ImGui::PopStyleColor( 2 );


    ImGui::SetCursorPosY( flOffsetY + sPos.y );
    ImGui::SetCursorPosX( flOffsetX + ImGui::GetWindowSize( ).x - 76.f );

    ImGui::PushStyleColor( ImGuiCol_ChildBg, RGB_VEC( 4.f, 5.f, 14.f, flGlowAlpha ) );
    ImGui::PushStyleColor( ImGuiCol_Border, RGB_VEC( 3.f, 18.f, 33.f, flGlowAlpha ) );
    ImGui::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 2.f );
    ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 3.f );

    ImGui::BeginChild( std::string( "##test_chld2" ).append( cText ).c_str( ), 0, ImVec2( 72.f + flAddSizeX, 25.f + flAddSizeY ), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus );
    ImGui::EndChild( );

    ImGui::PopStyleVar( 2 );
    ImGui::PopStyleColor( 2 );

    ImGui::SetCursorPosY( flOffsetY + sPos.y + 1.f );
    ImGui::SetCursorPosX( flOffsetX + ImGui::GetWindowSize( ).x - 75.f );

    ImGui::PushStyleColor( ImGuiCol_ChildBg, RGB_VEC( sChildBg.x * 255.f, sChildBg.y * 255.f, sChildBg.z * 255.f, ImGui::GetStyle( ).Alpha * 255.f ) );
    ImGui::PushStyleColor( ImGuiCol_Border, pElement->GetVar( _h( "ChildBorder" ) ).GetVec4( ) );
    ImGui::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 1.5f );
    ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 3.f );

    ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 5.f, 3.f ) );
    ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0.f, 8.f ) );

    ImGui::BeginChild( "##test_chld3", 0, ImVec2( 70.f + flAddSizeX, 23.f + flAddSizeY ), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus );

    ImGui::PushStyleColor( ImGuiCol_Text, pElement->GetVar( _h( "TextColor" ) ).GetVec4( ) );

    if ( iOpenState )
        ImGui::Text( "<Press a key>" );
    else
        ImGui::Text( *pValue ? szKeys[*pValue] : "None" );

    ImGui::PopStyleColor( );

    auto AnimateTextColor = [ ]( float flProgress, float flAlpha ) -> ImVec4
    {
        ImVec4 sTextFrom = RGB_VEC( 153.f, 176.f, 189.f, 255.f );
        ImVec4 sTextTo = RGB_VEC( 255.f, 255.f, 255.f, 255.f );

        return ImVec4(
            sTextFrom.x + ( ( sTextTo.x - sTextFrom.x ) * flProgress ),
            sTextFrom.y + ( ( sTextTo.y - sTextFrom.y ) * flProgress ),
            sTextFrom.z + ( ( sTextTo.z - sTextFrom.z ) * flProgress ),
            flAlpha );
    };

    ImGui::PopStyleVar( );

    ImGui::EndChild( );
    ImGui::PopStyleVar( );

    ImGui::PopStyleVar( 2 );
    ImGui::PopStyleColor( 2 );

    ImGui::PopFont( );

    bool bIntersected = g_NL->CheckMouseIntersectionLocal( ImVec2( flOffsetX + ImGui::GetWindowSize( ).x - 75.f, flOffsetY + sPos.y + 1.f ), ImVec2( 70.f + flAddSizeX, 23.f + flAddSizeY ) );

    g_NL->KeybindValueMouseIntersectCallback( pElement, bIntersected );

    bool bIntersectedFull = g_NL->CheckMouseIntersectionLocal( sPos, ImVec2( ImGui::GetWindowSize( ).x - 20.f, 30.f ) );

    g_NL->KeybindValueFullMouseIntersectCallback( pElement, bIntersectedFull );

    if ( ImGui::GetIO( ).MouseClicked[0] )
        g_NL->KeybindValueClickCallback( pElement, bIntersected );
}

void C_Nl::KeybindValueMouseIntersectCallback( C_Element* pElement, bool bIntersected )
{
    int& iBgState = pElement->GetVar( _h( "BgState" ) ).GetInt( );
    int& iBorderState = pElement->GetVar( _h( "BorderState" ) ).GetInt( );
    int& iOpenState = pElement->GetVar( _h( "OpenState" ) ).GetInt( );

    if ( iOpenState )
        bIntersected = true;

    if ( !iOpenState )
    {
        if ( bIntersected && !iBgState )
        {
            ADD_ANIMATION( C_Animation( ).ChildRedAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 20.f / 255.f ) );
            ADD_ANIMATION( C_Animation( ).ChildGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 20.f / 255.f ) );
            ADD_ANIMATION( C_Animation( ).ChildBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 20.f / 255.f ) );

            iBgState = 1;
        }
        else if ( !bIntersected && iBgState )
        {
            ADD_ANIMATION( C_Animation( ).ChildRedAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 13.f / 255.f ) );
            ADD_ANIMATION( C_Animation( ).ChildGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 13.f / 255.f ) );
            ADD_ANIMATION( C_Animation( ).ChildBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 13.f / 255.f ) );

            iBgState = 0;
        }
    }

    if ( bIntersected && !iBorderState && iOpenState )
    {
        ADD_ANIMATION( C_Animation( ).AlphaAnimate( pElement, AT_HIGH_TO_LOW, 0.07f, 255.f ) );

        ADD_ANIMATION( C_Animation( ).ChildBorderRedAnimate( pElement, AT_HIGH_TO_LOW, 0.07f, 50.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildBorderGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.07f, 50.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildBorderBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.07f, 50.f / 255.f ) );

        ADD_ANIMATION( C_Animation( ).ChildRedAnimate( pElement, AT_HIGH_TO_LOW, 0.07f, 28.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.07f, 28.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.07f, 28.f / 255.f ) );

        iBorderState = 1;
        iBgState = 0;
    }
    else if ( !bIntersected && iBorderState )
    {
        ADD_ANIMATION( C_Animation( ).AlphaAnimate( pElement, AT_HIGH_TO_LOW, 0.07f, 0.f ) );

        ADD_ANIMATION( C_Animation( ).ChildBorderRedAnimate( pElement, AT_HIGH_TO_LOW, 0.07f, 26.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildBorderGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.07f, 26.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildBorderBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.07f, 26.f / 255.f ) );

        ADD_ANIMATION( C_Animation( ).ChildRedAnimate( pElement, AT_HIGH_TO_LOW, 0.07f, 13.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.07f, 13.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).ChildBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.07f, 13.f / 255.f ) );

        iBorderState = 0;
        iBgState = 0;
    }
}

void C_Nl::KeybindValueFullMouseIntersectCallback( C_Element* pElement, bool bIntersected )
{
    int& iState = pElement->GetVar( _h( "TextState" ) ).GetInt( );
    int& iOpenState = pElement->GetVar( _h( "OpenState" ) ).GetInt( );

    if ( iOpenState )
        bIntersected = true;

    if ( !iState && bIntersected )
    {
        g_NL->ClearElementAnimations( pElement, _h( "TextRedAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "TextGreenAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "TextBlueAnimate" ) );

        ADD_ANIMATION( C_Animation( ).TextRedAnimate( pElement, AT_HIGH_TO_LOW, 0.3f, 242.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).TextGreenAnimate( pElement, AT_HIGH_TO_LOW, 0.3f, 242.f / 255.f ) );
        ADD_ANIMATION( C_Animation( ).TextBlueAnimate( pElement, AT_HIGH_TO_LOW, 0.3f, 242.f / 255.f ) );

        iState = 1;
    }
    else if ( iState && !bIntersected )
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

void C_Nl::KeybindValueClickCallback( C_Element* pElement, bool bIntersected )
{
    int& iOpenState = pElement->GetVar( _h( "OpenState" ) ).GetInt( );

    if ( !bIntersected )
    {
        if ( iOpenState )
        {
            g_NL->ClearElementAnimations( pElement, _h( "OpenScaleAnimate" ) );

            ADD_ANIMATION( C_Animation( ).OpenScaleAnimate( pElement, AT_HIGH_TO_LOW, 0.15f, 0.f ) );
            iOpenState = 0;
            g_NL->m_bSelectingShit = 2;
        }

        return;
    }

    if ( !iOpenState && !g_NL->m_bSelectingShit )
    {
        g_NL->ClearElementAnimations( pElement, _h( "OpenScaleAnimate" ) );

        ADD_ANIMATION( C_Animation( ).OpenScaleAnimate( pElement, AT_HIGH_TO_LOW, 0.05f, 1.f ) );
        iOpenState = 1;
        g_NL->m_bSelectingShit = true;
        g_Keybinds->m_iLastPressedKey = 0;
    }
}
