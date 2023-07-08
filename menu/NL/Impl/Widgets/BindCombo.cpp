#include "..\..\Neverlose.h"


C_Element& C_Element::BindCombo( const char* szText, ImVec2 sPos, std::vector<std::string>* cVecStrings, int* pValue )
{
    m_eType = ET_COMBO;

    m_cMapVars[_h( "Text" )] = C_Var( std::string( szText ) );
    m_cMapVars[_h( "Origin" )] = C_Var( sPos );
    m_cMapVars[_h( "ValuePointer" )] = C_Var( pValue );
    m_cMapVars[_h( "Strings" )] = C_Var( cVecStrings );
    m_cMapVars[_h( "Rebuild" )] = C_Var( 1 );
    m_cMapVars[_h( "LastEraseIndex" )] = C_Var( 0 );
    m_cMapVars[_h( "ClearFirst" )] = C_Var( 0 );

    m_cMapVars[_h( "ElementsProps" )] = C_Var( new std::vector < C_BindComboItemProperty >( ) );

    m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "BindComboCreateCallback" ) );

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

void C_Nl::BindComboCreateCallback( C_Element* pElement )
{
    std::string& cText = pElement->GetVar( _h( "Text" ) ).GetString( );
    ImVec2 sPos = pElement->GetVar( _h( "Origin" ) ).GetVec2( );
    int* pValue = pElement->GetVar( _h( "ValuePointer" ) ).GetRef<int>( );
    float flGlowAlpha = pElement->GetVar( _h( "Alpha" ) ).GetFloat( );
    float flOpenScale = pElement->GetVar( _h( "OpenScale" ) ).GetFloat( );
    ImVec4& sChildBg = pElement->GetVar( _h( "ChildBgColor" ) ).GetVec4( );
    std::vector < std::string >* pVec = pElement->GetVar( _h( "Strings" ) ).GetRef< std::vector < std::string > >( );
    std::vector < C_BindComboItemProperty >* pElemsProps = pElement->GetVar( _h( "ElementsProps" ) ).GetRef< std::vector < C_BindComboItemProperty > >( );
    int& iOpenState = pElement->GetVar( _h( "OpenState" ) ).GetInt( );
    int& iRebuild = pElement->GetVar( _h( "Rebuild" ) ).GetInt( );
    int& iLastEraseIndex = pElement->GetVar( _h( "LastEraseIndex" ) ).GetInt( );

    if ( iOpenState )
    {
        ImGui::SetNextWindowPos( ImVec2( ImGui::GetWindowPos( ).x, ImGui::GetWindowPos( ).y ) );
        ImGui::SetNextWindowSize( ImVec2( ImGui::GetWindowSize( ).x, 5000.f ) );
        ImGui::Begin( std::string( "##issaaa_" ).append( cText ).c_str( ), 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings );
    }
    
    for ( int i = 0; i < pElemsProps->size( ); ++i )
    {
        if ( pElemsProps->at( i ).m_bEraseOnZero && pElemsProps->at( i ).m_flTextAlpha == 0.f )
        {
            pVec->erase( pVec->begin( ) + i );
            pElemsProps->erase( pElemsProps->begin( ) + i );
        }
    }

    ImGui::PushFont( g_NL->pMenuFont );

    if ( iRebuild )
    {
        pElemsProps->clear( );

        for ( int i = 0; i < pVec->size( ); ++i )
            pElemsProps->push_back( C_BindComboItemProperty( ) );

        int iCurrentOffset = 0;

        for ( int i = 0; i < pElemsProps->size(); ++i )
        {
            pElemsProps->at( i ).m_iOffsetY = iCurrentOffset;
            iCurrentOffset += ImGui::CalcTextSize( pVec->at( i ).c_str( ) ).y + 8;
            pElemsProps->at( i ).m_iSizeY = iCurrentOffset;
        }

        iRebuild = 0;
    }

    ImGui::SetCursorPos( sPos );

    ImGui::PushStyleColor( ImGuiCol_Text, pElement->GetVar( _h( "TextColor" ) ).GetVec4( ) );
    ImGui::Text( cText.c_str( ) );
    ImGui::PopStyleColor( );
    ImGui::PopFont( );

    ImGui::PushFont( g_NL->pMenuFont );

    float flSizeYCalculated = 0.f;
    int iCurrentOffset = 0;

    for ( int i = 0; i < pElemsProps->size( ); ++i )
    {
        flSizeYCalculated += pElemsProps->at( 0 ).m_iSizeY;

        if ( i == pElemsProps->size( ) - 1 )
        {
            flSizeYCalculated -= iCurrentOffset - pElemsProps->at( i ).m_iOffsetY;
        }

        iCurrentOffset += ( int ) ImGui::CalcTextSize( pVec->at( i ).c_str( ) ).y + 8;
    }

    if ( iOpenState )
        flSizeYCalculated -= pElemsProps->size( );

    constexpr float flOffsetX = -106.f;
    constexpr float flOffsetY = -4.f;

    constexpr float flAddSizeX = 99.f;
    float flAddSizeY = 4.f + flSizeYCalculated * flOpenScale;

    ImGui::SetCursorPosY( flOffsetY + sPos.y - 1.f );
    ImGui::SetCursorPosX( flOffsetX + ImGui::GetWindowSize( ).x - 77.f );

    ImGui::PushStyleColor( ImGuiCol_ChildBg, RGB_VEC( 4.f, 5.f, 14.f, flGlowAlpha ) );
    ImGui::PushStyleColor( ImGuiCol_Border, RGB_VEC( 30.f, 30.f, 32.f, flGlowAlpha * 0.5f ) );
    ImGui::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 4.f );
    ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 3.f );

    ImGui::BeginChild( std::string( "##test_chld1_" ).append( cText ).c_str( ), 0, ImVec2( 74.f + flAddSizeX, 27.f + flAddSizeY ), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
    ImGui::EndChild( );

    ImGui::PopStyleVar( 2 );
    ImGui::PopStyleColor( 2 );


    ImGui::SetCursorPosY( flOffsetY + sPos.y );
    ImGui::SetCursorPosX( flOffsetX + ImGui::GetWindowSize( ).x - 76.f );

    ImGui::PushStyleColor( ImGuiCol_ChildBg, RGB_VEC( 4.f, 5.f, 14.f, flGlowAlpha ) );
    ImGui::PushStyleColor( ImGuiCol_Border, RGB_VEC( 54.f, 53.f, 56.f, flGlowAlpha * 0.5f ) );
    ImGui::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 2.f );
    ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 3.f );

    ImGui::BeginChild( std::string( "##test_chld2_" ).append( cText ).c_str( ), 0, ImVec2( 72.f + flAddSizeX, 25.f + flAddSizeY ), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
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
    ImGui::BeginChild( std::string( "##test_chld3_" ).append( cText ).c_str( ), 0, ImVec2( 70.f + flAddSizeX, 23.f + flAddSizeY ), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );

    ImGui::PushStyleColor( ImGuiCol_Text, pElement->GetVar( _h( "TextColor" ) ).GetVec4( ) );
    ImGui::Text( pVec->at( *pValue ).c_str( ) );
    ImGui::PopStyleColor( );

    auto AnimateTextColor = [ ]( float flProgress, float flAlphaScale ) -> ImVec4
    {
        ImVec4 sTextFrom = RGB_VEC( 153.f, 176.f, 189.f, 255.f );
        ImVec4 sTextTo = RGB_VEC( 255.f, 255.f, 255.f, 255.f );

        return ImVec4(
            sTextFrom.x + ( ( sTextTo.x - sTextFrom.x ) * flProgress ),
            sTextFrom.y + ( ( sTextTo.y - sTextFrom.y ) * flProgress ),
            sTextFrom.z + ( ( sTextTo.z - sTextFrom.z ) * flProgress ),
            flAlphaScale );
    };

    auto AnimateCircleColor = [ ]( float flProgress, float flAlphaScale ) -> ImVec4
    {
        ImVec4 sTextFrom = RGB_VEC( 200.f, 200.f, 200.f, 255.f );
        ImVec4 sTextTo = RGB_VEC( 255.f, 255.f, 255.f, 255.f );

        return ImVec4(
            sTextFrom.x + ( ( sTextTo.x - sTextFrom.x ) * flProgress ),
            sTextFrom.y + ( ( sTextTo.y - sTextFrom.y ) * flProgress ),
            sTextFrom.z + ( ( sTextTo.z - sTextFrom.z ) * flProgress ),
            flAlphaScale );
    };

    bool bAdd = false;

    for ( int i = 0; i < pVec->size( ); ++i )
    {
        ImVec2 sCursorBackup = ImGui::GetCursorPos( );
        ImVec2 sCurrPos = sCursorBackup;

        sCurrPos.x += ImGui::GetWindowPos( ).x;
        sCurrPos.y += ImGui::GetWindowPos( ).y;

        sCurrPos.x += 6.f;
        sCurrPos.y = ImGui::GetWindowPos( ).y + pElemsProps->at( i ).m_iOffsetY + 41.f;

        if ( i != pVec->size( ) - 1 )
        {
            ImGui::GetWindowDrawList( )->AddCircleFilled( sCurrPos, 5.f, ImGui::ColorConvertFloat4ToU32( AnimateCircleColor( pElemsProps->at( i ).m_flCircleScale, pElemsProps->at( i ).m_flTextAlpha ) ) );

            if ( i )
            {
                ImGui::GetWindowDrawList( )->AddRectFilled(
                    ImVec2( sCurrPos.x - 3.f, sCurrPos.y - 1.f ),
                    ImVec2( sCurrPos.x + 3.f, sCurrPos.y + 1.f ),
                    ImGui::ColorConvertFloat4ToU32( RGB_VEC( 3.f, 18.f, 33.f, 230.f * pElemsProps->at( i ).m_flTextAlpha ) ) );
            }
        }
        else
        {
            ImGui::GetWindowDrawList( )->AddCircleFilled( sCurrPos, 5.f, ImGui::ColorConvertFloat4ToU32( AnimateCircleColor( pElemsProps->at( i ).m_flCircleScale, pElemsProps->at( i ).m_flTextAlpha ) ) );

            ImGui::GetWindowDrawList( )->AddRectFilled(
                ImVec2( sCurrPos.x - 3.f, sCurrPos.y - 1.f ),
                ImVec2( sCurrPos.x + 3.f, sCurrPos.y + 1.f ),
                ImGui::ColorConvertFloat4ToU32( RGB_VEC( 3.f, 18.f, 33.f, 230.f * pElemsProps->at( i ).m_flTextAlpha ) ) );

            ImGui::GetWindowDrawList( )->AddRectFilled(
                ImVec2( sCurrPos.x - 1.f, sCurrPos.y - 3.f ),
                ImVec2( sCurrPos.x + 1.f, sCurrPos.y + 3.f ),
                ImGui::ColorConvertFloat4ToU32( RGB_VEC( 3.f, 18.f, 33.f, 230.f * pElemsProps->at( i ).m_flTextAlpha ) ) );
        }

        ImGui::SetCursorPos( sCursorBackup );
        ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 18.f );

        bool bIntersectedText = g_NL->CheckMouseIntersectionLocal( ImGui::GetCursorPos( ), ImVec2( ImGui::GetWindowSize( ).x, ImGui::CalcTextSize( pVec->at( i ).c_str( ) ).y ) );

        if ( i != pVec->size() - 1 && bIntersectedText && iOpenState && ImGui::GetIO( ).MouseClicked[0] )
            *pValue = i;

        bool bIntersectedIcon = g_NL->CheckMouseIntersectionLocal( ImVec2( ImGui::GetCursorPos( ).x - 15.f, ImGui::GetCursorPosY( ) ), ImVec2( 10.f, 15.f ) );

        if ( bIntersectedIcon )
            pElemsProps->at( i ).m_flCircleScale += ( 1.0f / ImGui::GetIO( ).Framerate ) * 10.f;
        else
            pElemsProps->at( i ).m_flCircleScale -= ( 1.0f / ImGui::GetIO( ).Framerate ) * 10.f;

        pElemsProps->at( i ).m_flCircleScale = std::clamp( pElemsProps->at( i ).m_flCircleScale, 0.f, 1.f );

        if ( ( bIntersectedIcon || bIntersectedText ) && iOpenState && i == pVec->size( ) - 1 && ImGui::GetIO( ).MouseClicked[0] )
        {
            pVec->insert( pVec->end( ) - 1, "Empty bind" );
            pElemsProps->insert( pElemsProps->end( ) - 1, C_BindComboItemProperty( ) );

            int iIndexAdded = pElemsProps->size( ) - 2;

            *pValue = iIndexAdded;

            pElemsProps->at( iIndexAdded ).m_flTextAlpha = 0.f;

            int iCurrentOffset = 0;

            for ( int j = 0; j < pElemsProps->size( ); ++j )
            {
                if ( j < iIndexAdded )
                {
                    iCurrentOffset += ImGui::CalcTextSize( pVec->at( j ).c_str( ) ).y + 8;
                    continue;
                }

                if ( j == iIndexAdded )
                {
                    pElemsProps->at( j ).m_iOffsetY = iCurrentOffset;
                    iCurrentOffset += ImGui::CalcTextSize( pVec->at( j ).c_str( ) ).y + 8;
                    pElemsProps->at( j ).m_iSizeY = iCurrentOffset;

                    ADD_ANIMATION( C_Animation( ).BindComboAlphaTextAnimate( pElement, AT_HIGH_TO_LOW, 0.2f, 1.f, j ) );
                }
                else
                {
                    ADD_ANIMATION( C_Animation( ).BindComboOffsetTextAnimate( pElement, AT_HIGH_TO_LOW, 0.2f, iCurrentOffset, j ) );
                    iCurrentOffset += ImGui::CalcTextSize( pVec->at( j ).c_str( ) ).y + 8;
                }
            }

            if ( !bIntersectedText )
                bAdd = true;
        }
        
        if ( bIntersectedIcon && !bAdd && iOpenState && i != ( pVec->size( ) - 1 ) && ImGui::GetIO( ).MouseClicked[0] )
        {
            if ( i )
            {
                int iEraseIndex = i;
                int iCurrentOffset = 0;

                *pValue = iEraseIndex - 1;

                iLastEraseIndex = iEraseIndex;

                for ( int j = 0; j < pElemsProps->size( ); ++j )
                {
                    if ( j < iEraseIndex )
                    {
                        iCurrentOffset += ImGui::CalcTextSize( pVec->at( j ).c_str( ) ).y + 8;
                        continue;
                    }

                    if ( j == iEraseIndex )
                    {
                        pElemsProps->at( j ).m_bEraseOnZero = true;
                        ADD_ANIMATION( C_Animation( ).BindComboAlphaTextAnimate( pElement, AT_HIGH_TO_LOW, 0.2f, 0.f, j ) );
                    }
                    else
                    {
                        ADD_ANIMATION( C_Animation( ).BindComboOffsetTextAnimate( pElement, AT_HIGH_TO_LOW, 0.2f, iCurrentOffset, j ) );
                        iCurrentOffset += ImGui::CalcTextSize( pVec->at( j ).c_str( ) ).y + 8;
                    }
                }

                bAdd = true;
            }
            else
                pElement->GetVar( _h( "ClearFirst" ) ).GetInt( ) = 1;
        }
        
        if ( bIntersectedText )
            pElemsProps->at( i ).m_flColorScale += ( 1.0f / ImGui::GetIO( ).Framerate ) * 6.f;
        else
            pElemsProps->at( i ).m_flColorScale -= ( 1.0f / ImGui::GetIO( ).Framerate ) * 6.f;

        pElemsProps->at( i ).m_flColorScale = std::clamp( pElemsProps->at( i ).m_flColorScale, 0.f, 1.f );

        ImGui::PushStyleColor( ImGuiCol_Text, *pValue == i ? RGB_VEC( 255.f, 255.f, 255.f, 255.f * pElemsProps->at( i ).m_flTextAlpha ) : AnimateTextColor( pElemsProps->at( i ).m_flColorScale, pElemsProps->at( i ).m_flTextAlpha ) );

        ImGui::SetCursorPosY( 31.f + pElemsProps->at( i ).m_iOffsetY );
        ImGui::Text( pVec->at( i ).c_str( ) );
        ImGui::PopStyleColor( );
    }

    ImGui::PopStyleVar( );

    ImGui::EndChild( );
    ImGui::PopStyleVar( );

    ImGui::PopStyleVar( 2 );
    ImGui::PopStyleColor( 2 );

    ImGui::PopFont( );

    if ( iOpenState )
        ImGui::End( );

    bool bIntersected = g_NL->CheckMouseIntersectionLocal( ImVec2( flOffsetX + ImGui::GetWindowSize( ).x - 75.f, flOffsetY + sPos.y + 1.f ), ImVec2( 70.f + flAddSizeX, 23.f + flAddSizeY ) );

    g_NL->BindComboMouseIntersectCallback( pElement, bIntersected );

    bool bIntersectedFull = g_NL->CheckMouseIntersectionLocal( sPos, ImVec2( ImGui::GetWindowSize( ).x - 20.f, 30.f ) );

    g_NL->BindComboFullMouseIntersectCallback( pElement, bIntersectedFull );

    if ( !bAdd && ImGui::GetIO( ).MouseClicked[0] )
        g_NL->BindComboClickCallback( pElement, bIntersected );
}

void C_Nl::BindComboMouseIntersectCallback( C_Element* pElement, bool bIntersected )
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
        g_NL->ClearElementAnimations( pElement, _h( "AlphaAnimate" ) );

        g_NL->ClearElementAnimations( pElement, _h( "ChildBorderRedAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "ChildBorderGreenAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "ChildBorderBlueAnimate" ) );

        g_NL->ClearElementAnimations( pElement, _h( "ChildRedAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "ChildGreenAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "ChildBlueAnimate" ) );

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
        g_NL->ClearElementAnimations( pElement, _h( "AlphaAnimate" ) );

        g_NL->ClearElementAnimations( pElement, _h( "ChildBorderRedAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "ChildBorderGreenAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "ChildBorderBlueAnimate" ) );

        g_NL->ClearElementAnimations( pElement, _h( "ChildRedAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "ChildGreenAnimate" ) );
        g_NL->ClearElementAnimations( pElement, _h( "ChildBlueAnimate" ) );

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

void C_Nl::BindComboFullMouseIntersectCallback( C_Element* pElement, bool bIntersected )
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

void C_Nl::BindComboClickCallback( C_Element* pElement, bool bIntersected )
{
    int& iOpenState = pElement->GetVar( _h( "OpenState" ) ).GetInt( );
    std::vector < C_BindComboItemProperty >* pElemsProps = pElement->GetVar( _h( "ElementsProps" ) ).GetRef< std::vector < C_BindComboItemProperty > >( );

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

        ADD_ANIMATION( C_Animation( ).OpenScaleAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, 1.f ) );
        iOpenState = 1;

        for ( int i = 0; i < pElemsProps->size( ); ++i )
        {
            int iOffsetTo = pElemsProps->at( i ).m_iOffsetY;

            pElemsProps->at( i ).m_iOffsetY = 0;
            pElemsProps->at( i ).m_flTextAlpha = 0;

            ADD_ANIMATION( C_Animation( ).BindComboOffsetTextAnimate( pElement, AT_HIGH_TO_LOW, 0.1f * i, iOffsetTo, i ) );
            ADD_ANIMATION( C_Animation( ).BindComboAlphaTextAnimate( pElement, AT_HIGH_TO_LOW, 0.2f * i, 1.f, i ) );
        }

        g_NL->m_bSelectingShit = true;
    }
    else if ( iOpenState )
    {
        g_NL->ClearElementAnimations( pElement, _h( "OpenScaleAnimate" ) );

        ADD_ANIMATION( C_Animation( ).OpenScaleAnimate( pElement, AT_HIGH_TO_LOW, 0.15f, 0.f ) );
        iOpenState = 0;
        g_NL->m_bSelectingShit = 2;
    }
}
