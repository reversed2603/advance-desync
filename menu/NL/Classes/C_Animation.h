#pragma once

enum E_AnimationType : int
{
    AT_NONE,
    AT_STABLE_MOVE,
    AT_HIGH_TO_LOW,
};

class C_Animation
{
private:
    E_AnimationType m_eType = AT_NONE;
    C_Element* m_pElement = nullptr;
    std::map < uint32_t, C_Var > m_cMapVarsOnStart = { };
    std::map < uint32_t, C_Var > m_cMapAnimationVars = { };
    std::map < uint32_t, std::function < void( C_Animation* pAnimation ) > > m_cMapCallbacks = {};
    float m_flCurrentAnimTime = 0.f;
    float m_flMaxAnimTime = 0.f;

public:
    C_Animation& OriginMove( C_Element* pElement, E_AnimationType eType, float flAnimTime, ImVec2 sNewOrigin, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewOrigin" )] = C_Var( sNewOrigin );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "OriginMove" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& AlphaAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, float flAlpha, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewAlpha" )] = C_Var( flAlpha );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "AlphaAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& TextRedAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, float flRed, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewRed" )] = C_Var( flRed );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "TextRedAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& TextGreenAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, float flGreen, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewGreen" )] = C_Var( flGreen );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "TextGreenAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& TextBlueAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, float flBlue, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewBlue" )] = C_Var( flBlue );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "TextBlueAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& CircleXOffsetAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, int iNewOffset, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewCircleXOffset" )] = C_Var( iNewOffset );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "CircleXOffsetAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& ChildRedAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, float flRed, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewRed" )] = C_Var( flRed );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "ChildRedAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& ChildGreenAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, float flGreen, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewGreen" )] = C_Var( flGreen );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "ChildGreenAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& ChildBlueAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, float flBlue, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewBlue" )] = C_Var( flBlue );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "ChildBlueAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& CircleRedAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, float flRed, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewRed" )] = C_Var( flRed );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "CircleRedAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& CircleGreenAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, float flGreen, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewGreen" )] = C_Var( flGreen );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "CircleGreenAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& CircleBlueAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, float flBlue, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewBlue" )] = C_Var( flBlue );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "CircleBlueAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& OffsetXAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, int iNewX, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewOffsetX" )] = C_Var( iNewX );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "OffsetXAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& ChildBorderRedAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, float flRed, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewRed" )] = C_Var( flRed );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "ChildBorderRedAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& ChildBorderGreenAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, float flGreen, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewGreen" )] = C_Var( flGreen );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "ChildBorderGreenAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& ChildBorderBlueAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, float flBlue, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewBlue" )] = C_Var( flBlue );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "ChildBorderBlueAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& OpenScaleAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, float flNewScale, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewScale" )] = C_Var( flNewScale );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "OpenScaleAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& BindComboAlphaTextAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, float flNewAlpha, int iIndex, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "Index" )] = C_Var( iIndex );
        m_cMapAnimationVars[_h( "NewAlpha" )] = C_Var( flNewAlpha );
        m_cMapAnimationVars[_h( "StartAlpha" )] = C_Var( pElement->GetVar( _h( "ElementsProps" ) ).GetRef< std::vector < C_BindComboItemProperty > >( )->at( iIndex ).m_flTextAlpha );
        m_cMapAnimationVars[_h( "LastSize" )] = C_Var( ( int ) pElement->GetVar( _h( "ElementsProps" ) ).GetRef< std::vector < C_BindComboItemProperty > >( )->size( ) );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "BindComboAlphaTextAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& BindComboOffsetTextAnimate( C_Element* pElement, E_AnimationType eType, float flAnimTime, int iNewOffset, int iIndex, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_pElement = pElement;
        m_cMapVarsOnStart = pElement->GetVars( );
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "Index" )] = C_Var( iIndex );
        m_cMapAnimationVars[_h( "NewOffset" )] = C_Var( iNewOffset );
        m_cMapAnimationVars[_h( "StartOffset" )] = C_Var( pElement->GetVar( _h( "ElementsProps" ) ).GetRef< std::vector < C_BindComboItemProperty > >( )->at( iIndex ).m_iOffsetY );
        m_cMapAnimationVars[_h( "LastSize" )] = C_Var( ( int ) pElement->GetVar( _h( "ElementsProps" ) ).GetRef< std::vector < C_BindComboItemProperty > >( )->size( ) );

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "BindComboOffsetTextAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& BindWindowAlphaAnimate( E_AnimationType eType, float flAnimTime, float flNewAlpha, float flCurrAlpha, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewAlpha" )] = C_Var( flNewAlpha );
        m_cMapAnimationVars[_h( "StartAlpha" )] = flCurrAlpha;

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "BindWindowAlphaAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

    C_Animation& ColorPickerWindowAlphaAnimate( E_AnimationType eType, float flAnimTime, float flNewAlpha, float flCurrAlpha, std::function < void( C_Animation* ) > pEndCallback = [ ]( C_Animation* ) -> void { } )
    {
        m_eType = eType;
        m_flCurrentAnimTime = 0.f;
        m_flMaxAnimTime = flAnimTime;

        m_cMapAnimationVars[_h( "NewAlpha" )] = C_Var( flNewAlpha );
        m_cMapAnimationVars[_h( "StartAlpha" )] = flCurrAlpha;

        m_cMapAnimationVars[_h( "Callback" )] = C_Var( _h( "ColorPickerWindowAlphaAnimate" ) );
        m_cMapCallbacks[_h( "EndCallback" )] = pEndCallback;

        return *this;
    }

public:
    C_Var& GetVar( uint32_t u32VarHash ) { return m_cMapAnimationVars[u32VarHash]; }
    C_Var& GetVarOnStart( uint32_t u32VarHash ) { return m_cMapVarsOnStart[u32VarHash]; }

    bool Active( ) { return m_flCurrentAnimTime < m_flMaxAnimTime; }
    E_AnimationType GetType( ) { return m_eType; }
    void AddTime( float flTime ) { m_flCurrentAnimTime += flTime; }

    float& GetTime( ) { return m_flCurrentAnimTime; }
    float& GetMaxTime( ) { return m_flMaxAnimTime; }

    C_Element* GetElement( ) { return m_pElement; }
    std::function < void( C_Animation* pAnimation ) >& GetCallback( uint32_t u32VarHash ) { return m_cMapCallbacks[u32VarHash]; }
};
