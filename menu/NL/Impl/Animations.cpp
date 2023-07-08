#include "..\Neverlose.h"

void C_Nl::InitializeAnimationCallbacks( )
{
    CALL_ONCE_START;
    {
        m_cMapAnimationCallbacks[_h( "OriginMove" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            ImVec2& sNewOrigin = pAnimation->GetVar( _h( "NewOrigin" ) ).GetVec2( );
            ImVec2& sStartOrigin = pAnimation->GetVarOnStart( _h( "Origin" ) ).GetVec2( );
            ImVec2& sCurrOrigin = pAnimation->GetElement( )->GetVar( _h( "Origin" ) ).GetVec2( );

            float flXDelta = sNewOrigin.x - sStartOrigin.x;
            float flYelta = sNewOrigin.y - sStartOrigin.y;

            sCurrOrigin.x = sStartOrigin.x + ( flXDelta * flProgress );
            sCurrOrigin.y = sStartOrigin.y + ( flYelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "AlphaAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewAlpha" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVarOnStart( _h( "Alpha" ) ).GetFloat( );
            float& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "Alpha" ) ).GetFloat( );

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );

            ImGui::GetStyle( ).Alpha = flCurrAlpha;
        };

        m_cMapAnimationCallbacks[_h( "TextRedAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewRed" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVarOnStart( _h( "TextColor" ) ).GetVec4( ).x;
            float& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "TextColor" ) ).GetVec4( ).x;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "TextGreenAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewGreen" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVarOnStart( _h( "TextColor" ) ).GetVec4( ).y;
            float& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "TextColor" ) ).GetVec4( ).y;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "TextBlueAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewBlue" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVarOnStart( _h( "TextColor" ) ).GetVec4( ).z;
            float& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "TextColor" ) ).GetVec4( ).z;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "CircleXOffsetAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            int flNewAlpha = pAnimation->GetVar( _h( "NewCircleXOffset" ) ).GetInt( );
            int flAlphaOnStart = pAnimation->GetVarOnStart( _h( "CircleXOffset" ) ).GetInt( );
            int& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "CircleXOffset" ) ).GetInt( );

            int flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "ChildRedAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewRed" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVarOnStart( _h( "ChildBgColor" ) ).GetVec4( ).x;
            float& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "ChildBgColor" ) ).GetVec4( ).x;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "ChildGreenAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewGreen" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVarOnStart( _h( "ChildBgColor" ) ).GetVec4( ).y;
            float& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "ChildBgColor" ) ).GetVec4( ).y;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "ChildBlueAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewBlue" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVarOnStart( _h( "ChildBgColor" ) ).GetVec4( ).z;
            float& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "ChildBgColor" ) ).GetVec4( ).z;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "CircleRedAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewRed" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVarOnStart( _h( "CircleColor" ) ).GetVec4( ).x;
            float& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "CircleColor" ) ).GetVec4( ).x;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "CircleGreenAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewGreen" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVarOnStart( _h( "CircleColor" ) ).GetVec4( ).y;
            float& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "CircleColor" ) ).GetVec4( ).y;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "CircleBlueAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewBlue" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVarOnStart( _h( "CircleColor" ) ).GetVec4( ).z;
            float& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "CircleColor" ) ).GetVec4( ).z;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "OffsetXAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            int flNewAlpha = pAnimation->GetVar( _h( "NewOffsetX" ) ).GetInt( );
            int flAlphaOnStart = pAnimation->GetVarOnStart( _h( "OffsetX" ) ).GetInt( );
            int& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "OffsetX" ) ).GetInt( );

            int flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "ChildBorderRedAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewRed" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVarOnStart( _h( "ChildBorder" ) ).GetVec4( ).x;
            float& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "ChildBorder" ) ).GetVec4( ).x;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "ChildBorderGreenAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewGreen" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVarOnStart( _h( "ChildBorder" ) ).GetVec4( ).y;
            float& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "ChildBorder" ) ).GetVec4( ).y;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "ChildBorderBlueAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewBlue" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVarOnStart( _h( "ChildBorder" ) ).GetVec4( ).z;
            float& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "ChildBorder" ) ).GetVec4( ).z;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "OpenScaleAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewScale" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVarOnStart( _h( "OpenScale" ) ).GetFloat( );
            float& flCurrAlpha = pAnimation->GetElement( )->GetVar( _h( "OpenScale" ) ).GetFloat( );

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "BindComboAlphaTextAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            int& iIndex = pAnimation->GetVar( _h( "Index" ) ).GetInt( );
            int& iLastSize = pAnimation->GetVar( _h( "LastSize" ) ).GetInt( );
            std::vector < C_BindComboItemProperty >* pElemsProps = pAnimation->GetElement( )->GetVar( _h( "ElementsProps" ) ).GetRef< std::vector < C_BindComboItemProperty > >( );

            if ( iLastSize != pElemsProps->size( ) )
            {
                iIndex -= iLastSize - pElemsProps->size( );
                iLastSize = pElemsProps->size( );
            }

            float flNewAlpha = pAnimation->GetVar( _h( "NewAlpha" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVar( _h( "StartAlpha" ) ).GetFloat( );
            float& flCurrAlpha = pElemsProps->at( iIndex ).m_flTextAlpha;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "BindComboOffsetTextAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            int& iIndex = pAnimation->GetVar( _h( "Index" ) ).GetInt( );
            int& iLastSize = pAnimation->GetVar( _h( "LastSize" ) ).GetInt( );
            std::vector < C_BindComboItemProperty >* pElemsProps = pAnimation->GetElement( )->GetVar( _h( "ElementsProps" ) ).GetRef< std::vector < C_BindComboItemProperty > >( );

            if ( iLastSize != pElemsProps->size( ) )
            {
                iIndex -= iLastSize - pElemsProps->size( );
                iLastSize = pElemsProps->size( );
            }
            
            int flNewAlpha = pAnimation->GetVar( _h( "NewOffset" ) ).GetInt( );
            int flAlphaOnStart = pAnimation->GetVar( _h( "StartOffset" ) ).GetInt( );
            int& flCurrAlpha = pElemsProps->at( iIndex ).m_iOffsetY;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "BindWindowAlphaAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewAlpha" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVar( _h( "StartAlpha" ) ).GetFloat( );
            float& flCurrAlpha = g_Keybinds->m_flWindowAlpha;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };

        m_cMapAnimationCallbacks[_h( "ColorPickerWindowAlphaAnimate" )] = [ ]( C_Animation* pAnimation, float flProgress ) -> void
        {
            float flNewAlpha = pAnimation->GetVar( _h( "NewAlpha" ) ).GetFloat( );
            float flAlphaOnStart = pAnimation->GetVar( _h( "StartAlpha" ) ).GetFloat( );
            float& flCurrAlpha = g_ColorPicker->m_flWindowAlpha;

            float flDelta = flNewAlpha - flAlphaOnStart;
            flCurrAlpha = flAlphaOnStart + ( flDelta * flProgress );
        };
    }
    CALL_ONCE_END;
}

void C_Nl::ProcessAnimations( )
{
    for ( int i = 0; i < m_cVecAnimations.size( ); ++i )
    {
        auto& cAnimation = m_cVecAnimations[i];

        bool bLastAnimCall = false;

        if ( !cAnimation.Active( ) )
            bLastAnimCall = true;

        auto& AnimateCallback = m_cMapAnimationCallbacks[cAnimation.GetVar( _h( "Callback" ) ).GetUInt32( )];

        if ( bLastAnimCall )
        {
            AnimateCallback( &cAnimation, 1.f );

            cAnimation.GetCallback( _h( "EndCallback" ) )( &cAnimation );

            m_cVecAnimations.erase( m_cVecAnimations.begin( ) + i );
            continue;
        }

        switch ( cAnimation.GetType( ) )
        {
            case AT_STABLE_MOVE:
            {
                AnimateCallback( &cAnimation, cAnimation.GetTime( ) / cAnimation.GetMaxTime( ) );
                break;
            }
            case AT_HIGH_TO_LOW:
            {
                float flProgress = cAnimation.GetTime( ) / cAnimation.GetMaxTime( );
                AnimateCallback( &cAnimation, ( 1.f - std::pow( -flProgress + 1.f, 2.f ) ) );

                break;
            }
        }

        cAnimation.AddTime( 1.0f / ImGui::GetIO( ).Framerate );
    }
}

