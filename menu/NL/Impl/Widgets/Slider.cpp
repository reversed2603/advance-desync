#include "..\..\Neverlose.h"

void C_Element::ResetSlider( bool bZeroOffset )
{
    if ( bZeroOffset )
        m_cMapVars[_h( "OffsetX" )] = C_Var( 0 );

    m_cMapVars[_h( "AnimatingStage" )] = C_Var( 0 );
}

C_Element& C_Element::Slider( uint32_t u32Hash, const char* szText, ImVec2 sPos, int iMin, int iMax, int* pValue )
{
    m_eType = ET_SLIDER;

    m_cMapVars[_h( "Text" )] = C_Var( std::string( szText ) );
    m_cMapVars[_h( "Origin" )] = C_Var( sPos );
    m_cMapVars[_h( "ValuePointer" )] = C_Var( pValue );
    m_cMapVars[_h( "Min" )] = C_Var( iMin );
    m_cMapVars[_h( "Max" )] = C_Var( iMax );
    m_cMapVars[_h( "HashIndex" )] = C_Var( u32Hash );
    m_cMapVars[_h( "AnimatingStage" )] = C_Var( 0 );
    m_cMapVars[_h( "TextState" )] = C_Var( 0 );
    m_cMapVars[_h( "MouseDownSaved" )] = C_Var( 0 );
    m_cMapVars[_h( "XAnimateToOffset" )] = C_Var( 0 );
    m_cMapVars[_h( "AnimatingSide" )] = C_Var( 0 );

    m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "SliderCreateCallback" ) );

    m_cMapVars[_h( "TextColor" )] = C_Var( RGB_VEC( 153.f, 176.f, 189.f, 255.f ) );
    m_cMapVars[_h( "OffsetX" )] = C_Var( 0 );

    return *this;
}

void C_Nl::SliderCreateCallback( C_Element* pElement )
{
    std::string& cText = pElement->GetVar( _h( "Text" ) ).GetString( );
    ImVec2 sPos = pElement->GetVar( _h( "Origin" ) ).GetVec2( );
    int* pValue = pElement->GetVar( _h( "ValuePointer" ) ).GetRef<int>( );
    int& iOffsetX = pElement->GetVar( _h( "OffsetX" ) ).GetInt( );
    int& iAnimatingStage = pElement->GetVar( _h( "AnimatingStage" ) ).GetInt( );
    int& iMouseDownSaved = pElement->GetVar( _h( "MouseDownSaved" ) ).GetInt( );
    int& iXAnimateToOffset = pElement->GetVar( _h( "XAnimateToOffset" ) ).GetInt( );
    int& iAnimatingSide = pElement->GetVar( _h( "AnimatingSide" ) ).GetInt( );
    uint32_t u32Hash = pElement->GetVar( _h( "HashIndex" ) ).GetUInt32( );

    *pValue = std::clamp( *pValue, pElement->GetVar( _h( "Min" ) ).GetInt( ), pElement->GetVar( _h( "Max" ) ).GetInt( ) );

    int iSliderWidth = ( int ) ( ImGui::GetWindowWidth( ) / 2.9468f );

    auto SliderAnimationEnd = [ ]( C_Animation* pAnimation ) -> void
    {
        pAnimation->GetElement( )->GetVar( _h( "AnimatingStage" ) ).GetInt( ) = 2;
        pAnimation->GetElement( )->GetVar( _h( "XAnimateToOffset" ) ).GetInt( ) = 0;
    };

    auto CalcOffsetX = [ ]( C_Element* pElement, int* pValue, int iSliderWidth ) -> int
    {
        float flDelta = *pValue - pElement->GetVar( _h( "Min" ) ).GetInt( );
        float flScale = flDelta / ( pElement->GetVar( _h( "Max" ) ).GetInt( ) - pElement->GetVar( _h( "Min" ) ).GetInt( ) );

        //int iDelta = abs( pElement->GetVar( _h( "Max" ) ).GetInt( ) - pElement->GetVar( _h( "Min" ) ).GetInt( ) );
        //float flScale = *pValue / ( float ) iDelta;

        return iSliderWidth * flScale;
    };

    ImGui::SetCursorPos( sPos );

    ImGui::PushFont( g_NL->pMenuFont );
    ImGui::PushStyleColor( ImGuiCol_Text, pElement->GetVar( _h( "TextColor" ) ).GetVec4( ) );
    ImGui::Text( cText.c_str( ) );
    ImGui::PopStyleColor( );
    ImGui::PopFont( );

    ImGui::SetCursorPosY( sPos.y );
    ImGui::SetCursorPosX( ImGui::GetWindowSize( ).x - 45.f );

    ImGui::PushFont( g_NL->pInputFont );
    ImGui::PushStyleColor( ImGuiCol_FrameBg, RGB_VEC( 13.f, 13.f, 13.f, 255.f ) );
    ImGui::PushStyleColor( ImGuiCol_Border, RGB_VEC( 28.f, 28.f, 31.f, 200.f ) );
    ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.5f );
    ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.f );
    
    ImGui::PushItemWidth( 33.f );

    int iTemp = *pValue;
    ImGui::InputInt( std::string( "##test_inp" ).append( cText ).c_str( ), &iTemp, 0 );

    ImGui::PopItemWidth( );

    if ( iTemp != *pValue && iAnimatingStage == 2 )
    {
        *pValue = std::clamp( iTemp, pElement->GetVar( _h( "Min" ) ).GetInt( ), pElement->GetVar( _h( "Max" ) ).GetInt( ) );

        g_NL->ClearElementAnimations( pElement, _h( "OffsetXAnimate" ) );

        iAnimatingStage = 1;
        ADD_ANIMATION( C_Animation( ).OffsetXAnimate( pElement, AT_HIGH_TO_LOW, 0.13f, CalcOffsetX( pElement, pValue, iSliderWidth ), SliderAnimationEnd ) );
    }

    ImGui::PopStyleVar( 2 );
    ImGui::PopStyleColor( 2 );
    ImGui::PopFont( );

    if ( !iAnimatingStage )
    {
        g_NL->ClearElementAnimations( pElement, _h( "OffsetXAnimate" ) );

        iAnimatingStage = 1;
        ADD_ANIMATION( C_Animation( ).OffsetXAnimate( pElement, AT_HIGH_TO_LOW, 0.13f, CalcOffsetX( pElement, pValue, iSliderWidth ), SliderAnimationEnd ) );
    }

    ImVec2 sLineStart = ImGui::GetWindowPos( );

    sLineStart.x += sPos.x;
    sLineStart.y += sPos.y;

    sLineStart.x += ImGui::GetWindowSize( ).x - 45.f - iSliderWidth - 23.f;
    sLineStart.y += 9.f;

    ImVec2 sLineEnd = sLineStart;

    sLineEnd.x += iSliderWidth;
    sLineEnd.y += 3.f;

    ImVec2 sStartProgressBig = sLineStart;
    ImVec2 sEndProgressBig = ImVec2( sStartProgressBig.x + iOffsetX, sLineStart.y + 3.f );
    ImVec2 sEndProgressBigTotal = ImVec2( sStartProgressBig.x + iSliderWidth, sLineStart.y + 3.f );

    ImVec2 sStartProgressLittle = ImVec2( sStartProgressBig.x, sStartProgressBig.y + 1.f );
    ImVec2 sEndProgressLittle = ImVec2( sStartProgressLittle.x + iOffsetX, sStartProgressLittle.y + 1.f );
    ImVec2 sEndProgressLittleTotal = ImVec2( sStartProgressLittle.x + iSliderWidth, sStartProgressLittle.y + 1.f );

    ImGui::GetWindowDrawList( )->AddRectFilled( sStartProgressBig, sEndProgressBigTotal, ImGui::ColorConvertFloat4ToU32( RGB_VEC( 30.f, 30.f, 32.f, ImGui::GetStyle().Alpha * 255.f ) ) );
    ImGui::GetWindowDrawList( )->AddRectFilled( sStartProgressBig, sEndProgressBig, ImGui::ColorConvertFloat4ToU32( RGB_VEC( 100.f, 100.f, 102.f, ImGui::GetStyle( ).Alpha * 255.f ) ) );

    ImGui::GetWindowDrawList( )->AddRectFilled( sStartProgressLittle, sEndProgressLittleTotal, ImGui::ColorConvertFloat4ToU32( RGB_VEC( 54.f, 53.f, 56.f, ImGui::GetStyle( ).Alpha * 255.f ) ) );
    ImGui::GetWindowDrawList( )->AddRectFilled( sStartProgressLittle, sEndProgressLittle, ImGui::ColorConvertFloat4ToU32( RGB_VEC( 150.f, 150.f, 150.f, ImGui::GetStyle( ).Alpha * 255.f ) ) );

    ImGui::GetWindowDrawList( )->AddCircleFilled( sEndProgressLittle, 8.f, ImGui::ColorConvertFloat4ToU32( RGB_VEC( 242.f, 242.f, 242.f, ImGui::GetStyle( ).Alpha * 255.f ) ), 21 );

    bool bIntersected = g_NL->CheckMouseIntersectionLocal( sPos, ImVec2( ImGui::GetWindowSize( ).x - 20.f, 30.f ) );

    g_NL->OnMouseCallbackTextColorChangeSlider( pElement, bIntersected );

    if ( bIntersected && ImGui::GetIO( ).MouseClicked[1] )
        g_Keybinds->EnableKeybindSelection( pElement, ET_SLIDER, u32Hash );

    bool bIntersectedLine = g_NL->CheckMouseIntersectionLocal( ImVec2( sStartProgressBig.x - ImGui::GetWindowPos( ).x, sStartProgressBig.y - ImGui::GetWindowPos( ).y - 4.f ), ImVec2( iSliderWidth, 10.f ) );

    if ( g_NL->m_bSelectingShit )
        bIntersectedLine = false;

    if ( (iMouseDownSaved || ( bIntersectedLine && ImGui::GetIO( ).MouseClicked[0] )) )
    {
        if ( ImGui::GetIO ( ).MouseDown [ 0 ] )
        {
            g_NL->m_bSelectingShit = true;
            iMouseDownSaved = 1;
        }

        int iNewOffsetX = ImGui::GetMousePos( ).x - sStartProgressBig.x;
        float flScale = std::clamp( ( float ) iNewOffsetX, 0.f, ( float ) iSliderWidth ) / iSliderWidth;

        *pValue = pElement->GetVar( _h( "Min" ) ).GetInt( ) + ( flScale * ( pElement->GetVar( _h( "Max" ) ).GetInt( ) - pElement->GetVar( _h( "Min" ) ).GetInt( ) ) );

        *pValue = std::clamp( *pValue, pElement->GetVar( _h( "Min" ) ).GetInt( ), pElement->GetVar( _h( "Max" ) ).GetInt( ) );

        if ( ImGui::GetIO( ).MouseClicked[0] && iAnimatingStage == 2 )
        {
            iXAnimateToOffset = CalcOffsetX( pElement, pValue, iSliderWidth );
            iAnimatingSide = iXAnimateToOffset < iOffsetX;

            g_NL->ClearElementAnimations( pElement, _h( "OffsetXAnimate" ) );

            iAnimatingStage = 1;
            ADD_ANIMATION( C_Animation( ).OffsetXAnimate( pElement, AT_HIGH_TO_LOW, 0.1f, iXAnimateToOffset, SliderAnimationEnd ) );
        }
        else if ( ImGui::GetIO( ).MouseDown[0] && iAnimatingStage != 1 )
        {
            iOffsetX = CalcOffsetX( pElement, pValue, iSliderWidth );
        }
    }

    if ( iAnimatingStage == 1 && iXAnimateToOffset && ImGui::GetIO( ).MouseDown[0] && ( ( !iAnimatingSide && ( ImGui::GetMousePos( ).x - sStartProgressBig.x ) < iOffsetX ) || ( iAnimatingSide && ( ImGui::GetMousePos( ).x - sStartProgressBig.x ) > iOffsetX ) ) )
    {
        g_NL->ClearElementAnimations( pElement, _h( "OffsetXAnimate" ) );

        iXAnimateToOffset = 0;
        iAnimatingStage = 2;
    }
    
    if ( !ImGui::GetIO ( ).MouseDown [ 0 ] )
    {
        iMouseDownSaved = 0;
        g_NL->m_bSelectingShit = false;
    }
}

void C_Nl::OnMouseCallbackTextColorChangeSlider( C_Element* pElement, bool bMouseIntersect )
{
    int& iState = pElement->GetVar( _h( "TextState" ) ).GetInt( );

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
