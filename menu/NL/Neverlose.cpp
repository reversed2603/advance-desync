#include "Neverlose.h"
#include "..\..\cloud\cloud.h"

void C_Nl::ChangeWeaponGroupRefs( int iWeaponGroup )
{
    //m_cVecElemets[m_iIndexMinDamage].GetVar( _h( "ValuePointer" ) );

    switch ( iWeaponGroup )
    {
        case 0:
        {
            m_cVecElemets[m_iIndexMinDamage].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iMinDamage_Pistols );
            m_cVecElemets[ m_iIndexAutoStopType ].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iStopTypePistols );
            m_cVecElemets[ m_iIndexAutoStopPred ].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg( ).m_bPredAutoStopPistols );
            m_cVecElemets[m_iIndexHitchance].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHitchance_Pistols );
            m_cVecElemets[m_iIndexStaticPoints].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_bStaticPoints_Pistols );
            m_cVecElemets[m_iIndexHeadScale].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHeadScale_Pistols );
            m_cVecElemets[m_iIndexBodyScale].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iBodyScale_Pistols );
            m_cVecElemets[m_iIndexHitboxes].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHitboxes_Pistols );
            m_cVecElemets[m_iIndexMultipoints].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iMultipoints_Pistols );

            m_cVecElemets[m_iIndexMinDamage].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_MIN_DAMAGE_PISTOLS" );
            m_cVecElemets[ m_iIndexAutoStopType ].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_STOP_TYPE_PISTOLS" );
            m_cVecElemets[ m_iIndexAutoStopPred ].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_STOP_PRED_PISTOLS" );
            m_cVecElemets[m_iIndexHitchance].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_HITCHANCE_PISTOLS" );
            m_cVecElemets[m_iIndexStaticPoints].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "CB_STATIC_PS_PISTOLS" );
            m_cVecElemets[m_iIndexHeadScale].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_HEAD_SCALE_PISTOLS" );
            m_cVecElemets[m_iIndexBodyScale].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_BODY_SCALE_PISTOLS" );
            m_cVecElemets[m_iIndexHitboxes].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "MCMB_HITBOXES_PISTOLS" );
            m_cVecElemets[m_iIndexMultipoints].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "MCMB_MULTIPOINTS_PISTOLS" );

            break;
        }
        case 1:
        {
            m_cVecElemets[m_iIndexMinDamage].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iMinDamage_DeagleR8 );
            m_cVecElemets[ m_iIndexAutoStopType ].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iStopTypeDeagleR8 );
            m_cVecElemets[ m_iIndexAutoStopPred ].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg( ).m_bPredAutoStopR8 );
            m_cVecElemets[m_iIndexHitchance].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHitchance_DeagleR8 );
            m_cVecElemets[m_iIndexStaticPoints].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_bStaticPoints_DeagleR8 );
            m_cVecElemets[m_iIndexHeadScale].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHeadScale_DeagleR8 );
            m_cVecElemets[m_iIndexBodyScale].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iBodyScale_DeagleR8 );
            m_cVecElemets[m_iIndexHitboxes].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHitboxes_DeagleR8 );
            m_cVecElemets[m_iIndexMultipoints].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iMultipoints_DeagleR8 );

            m_cVecElemets[m_iIndexMinDamage].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_MIN_DAMAGE_DEAGLER8" );
            m_cVecElemets[ m_iIndexAutoStopType ].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_STOP_TYPE_R6" );
            m_cVecElemets[ m_iIndexAutoStopPred ].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_STOP_PRED_R6" );
            m_cVecElemets[m_iIndexHitchance].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_HITCHANCE_DEAGLER8" );
            m_cVecElemets[m_iIndexStaticPoints].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "CB_STATIC_PS_DEAGLER8" );
            m_cVecElemets[m_iIndexHeadScale].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_HEAD_SCALE_DEAGLER8" );
            m_cVecElemets[m_iIndexBodyScale].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_BODY_SCALE_DEAGLER8" );
            m_cVecElemets[m_iIndexHitboxes].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "MCMB_HITBOXES_DEAGLER8" );
            m_cVecElemets[m_iIndexMultipoints].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "MCMB_MULTIPOINTS_DEAGLER8" );

            break;
        }
        case 2:
        {
            m_cVecElemets[m_iIndexMinDamage].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iMinDamage_Rifles );
            m_cVecElemets[ m_iIndexAutoStopType ].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iStopTypeRifles );
            m_cVecElemets[ m_iIndexAutoStopPred ].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg( ).m_bPredAutoStopRifles );
            m_cVecElemets[m_iIndexHitchance].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHitchance_Rifles );
            m_cVecElemets[m_iIndexStaticPoints].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_bStaticPoints_Rifles );
            m_cVecElemets[m_iIndexHeadScale].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHeadScale_Rifles );
            m_cVecElemets[m_iIndexBodyScale].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iBodyScale_Rifles );
            m_cVecElemets[m_iIndexHitboxes].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHitboxes_Rifles );
            m_cVecElemets[m_iIndexMultipoints].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iMultipoints_Rifles );

            m_cVecElemets[m_iIndexMinDamage].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_MIN_DAMAGE_RIFLES" );
            m_cVecElemets[ m_iIndexAutoStopType ].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_STOP_TYPE_RIFLES" );
            m_cVecElemets[ m_iIndexAutoStopPred ].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_STOP_PRED_RIFLES" );
            m_cVecElemets[m_iIndexHitchance].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_HITCHANCE_RIFLES" );
            m_cVecElemets[m_iIndexStaticPoints].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "CB_STATIC_PS_RIFLES" );
            m_cVecElemets[m_iIndexHeadScale].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_HEAD_SCALE_RIFLES" );
            m_cVecElemets[m_iIndexBodyScale].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_BODY_SCALE_RIFLES" );
            m_cVecElemets[m_iIndexHitboxes].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "MCMB_HITBOXES_RIFLES" );
            m_cVecElemets[m_iIndexMultipoints].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "MCMB_MULTIPOINTS_RIFLES" );

            break;
        }
        case 3:
        {
            m_cVecElemets[m_iIndexMinDamage].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iMinDamage_Scar );
            m_cVecElemets[ m_iIndexAutoStopType ].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iStopTypeScar );
            m_cVecElemets[ m_iIndexAutoStopPred ].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg( ).m_bPredAutoStopScar );
            m_cVecElemets[m_iIndexHitchance].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHitchance_Scar );
            m_cVecElemets[m_iIndexStaticPoints].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_bStaticPoints_Scar );
            m_cVecElemets[m_iIndexHeadScale].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHeadScale_Scar );
            m_cVecElemets[m_iIndexBodyScale].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iBodyScale_Scar );
            m_cVecElemets[m_iIndexHitboxes].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHitboxes_Scar );
            m_cVecElemets[m_iIndexMultipoints].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iMultipoints_Scar );

            m_cVecElemets[m_iIndexMinDamage].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_MIN_DAMAGE_SCAR" );
            m_cVecElemets[ m_iIndexAutoStopType ].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_STOP_TYPE_SCAR" );
            m_cVecElemets[ m_iIndexAutoStopPred ].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_STOP_PRED_SCAR" );
            m_cVecElemets[m_iIndexHitchance].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_HITCHANCE_SCAR" );
            m_cVecElemets[m_iIndexStaticPoints].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "CB_STATIC_PS_SCAR" );
            m_cVecElemets[m_iIndexHeadScale].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_HEAD_SCALE_SCAR" );
            m_cVecElemets[m_iIndexBodyScale].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_BODY_SCALE_SCAR" );
            m_cVecElemets[m_iIndexHitboxes].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "MCMB_HITBOXES_SCAR" );
            m_cVecElemets[m_iIndexMultipoints].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "MCMB_MULTIPOINTS_SCAR" );

            break;
        }
        case 4:
        {
            m_cVecElemets[m_iIndexMinDamage].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iMinDamage_SSG08 );
            m_cVecElemets[ m_iIndexAutoStopType ].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iStopTypeSSG08 );
            m_cVecElemets[ m_iIndexAutoStopPred ].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg( ).m_bPredAutoStopSSG08 );
            m_cVecElemets[m_iIndexHitchance].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHitchance_SSG08 );
            m_cVecElemets[m_iIndexStaticPoints].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_bStaticPoints_SSG08 );
            m_cVecElemets[m_iIndexHeadScale].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHeadScale_SSG08 );
            m_cVecElemets[m_iIndexBodyScale].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iBodyScale_SSG08 );
            m_cVecElemets[m_iIndexHitboxes].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHitboxes_SSG08 );
            m_cVecElemets[m_iIndexMultipoints].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iMultipoints_SSG08 );

            m_cVecElemets[m_iIndexMinDamage].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_MIN_DAMAGE_SSG08" );
            m_cVecElemets[ m_iIndexAutoStopType ].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_STOP_TYPE_SSG08" );
            m_cVecElemets[ m_iIndexAutoStopPred ].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_STOP_PRED_SSG08" );
            m_cVecElemets[m_iIndexHitchance].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_HITCHANCE_SSG08" );
            m_cVecElemets[m_iIndexStaticPoints].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "CB_STATIC_PS_SSG08" );
            m_cVecElemets[m_iIndexHeadScale].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_HEAD_SCALE_SSG08" );
            m_cVecElemets[m_iIndexBodyScale].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_BODY_SCALE_SSG08" );
            m_cVecElemets[m_iIndexHitboxes].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "MCMB_HITBOXES_SSG08" );
            m_cVecElemets[m_iIndexMultipoints].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "MCMB_MULTIPOINTS_SSG08" );

            break;
        }
        case 5:
        {
            m_cVecElemets[m_iIndexMinDamage].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iMinDamage_AWP );
            m_cVecElemets[ m_iIndexAutoStopType ].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iStopTypeAwp );
            m_cVecElemets[ m_iIndexAutoStopPred ].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg( ).m_bPredAutoStopAwp );
            m_cVecElemets[m_iIndexHitchance].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHitchance_AWP );
            m_cVecElemets[m_iIndexStaticPoints].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_bStaticPoints_AWP );
            m_cVecElemets[m_iIndexHeadScale].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHeadScale_AWP );
            m_cVecElemets[m_iIndexBodyScale].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iBodyScale_AWP );
            m_cVecElemets[m_iIndexHitboxes].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iHitboxes_AWP );
            m_cVecElemets[m_iIndexMultipoints].GetVar( _h( "ValuePointer" ) ).SetRef( &nem::hacks::g_aim_bot->cfg ( ).m_iMultipoints_AWP );

            m_cVecElemets[m_iIndexMinDamage].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_MIN_DAMAGE_AWP" );
            m_cVecElemets[ m_iIndexAutoStopType ].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_STOP_TYPE_AWP" );
            m_cVecElemets[ m_iIndexAutoStopPred ].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_STOP_PRED_AWP" );
            m_cVecElemets[m_iIndexHitchance].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_HITCHANCE_AWP" );
            m_cVecElemets[m_iIndexStaticPoints].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "CB_STATIC_PS_AWP" );
            m_cVecElemets[m_iIndexHeadScale].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_HEAD_SCALE_AWP" );
            m_cVecElemets[m_iIndexBodyScale].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "SL_BODY_SCALE_AWP" );
            m_cVecElemets[m_iIndexHitboxes].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "MCMB_HITBOXES_AWP" );
            m_cVecElemets[m_iIndexMultipoints].GetVar( _h( "HashIndex" ) ).GetUInt32( ) = _h( "MCMB_MULTIPOINTS_AWP" );

            break;
        }
    }

    for ( auto& cElem : m_cVecElemets )
        if ( cElem.GetType( ) == ET_SLIDER )
            cElem.ResetSlider( false );
}

void C_Nl::AfterMenu( )
{
    if ( m_iConfigsElementIndex == -1 )
        return;

    static int iLastSelectedWeaponGroup = 0;

    if ( m_iSelectedWeaponGroup != iLastSelectedWeaponGroup )
    {
        ChangeWeaponGroupRefs( m_iSelectedWeaponGroup );
        iLastSelectedWeaponGroup = m_iSelectedWeaponGroup;
    }

    g_cloud->init_update( );

    //std::vector<std::string>* pList = m_cVecElemets[m_iConfigsElementIndex].GetVar( _h( "Strings" ) ).GetRef<std::vector<std::string>>( );
    //int& iRebuild = m_cVecElemets[m_iConfigsElementIndex].GetVar( _h( "Rebuild" ) ).GetInt( );

    auto GetVecHash = [ ]( std::vector<std::string>& cVec ) -> uint32_t
    {
        uint32_t u32Result = 0;

        for ( auto& cElem : cVec )
        {
            u32Result = _h_RT( cElem.c_str( ) );
            u32Result = _rotl( u32Result, 3 );
        }

        return u32Result;
    };

   // pList->clear( );
    //pList->shrink_to_fit( );

    //for ( auto& cfg : sdk::g_cfg->m_cfg_list )
    //    pList->push_back( cfg );

   // iRebuild = 1;
}

void C_Nl::MoveWindows( )
{
    static int iOffsetX = 0;
    static int iOffsetY = 0;

    static bool bLastState = ( ImGui::GetIO( ).MouseDown[0] && ImGui::GetMousePos( ).x >= m_iMenuPosX && ImGui::GetMousePos( ).x <= m_iMenuPosX + 999 && ImGui::GetMousePos( ).y >= m_iMenuPosY && ImGui::GetMousePos( ).y <= m_iMenuPosY + 50.f );
    bool bCurrestState = ( ImGui::GetIO( ).MouseDown[0] && ImGui::GetMousePos( ).x >= m_iMenuPosX && ImGui::GetMousePos( ).x <= m_iMenuPosX + 999 && ImGui::GetMousePos( ).y >= m_iMenuPosY && ImGui::GetMousePos( ).y <= m_iMenuPosY + 50.f ) || ( bLastState && ImGui::GetIO( ).MouseDown[0] );

    if ( bCurrestState && !bLastState )
    {
        iOffsetX = ImGui::GetMousePos( ).x - m_iMenuPosX;
        iOffsetY = ImGui::GetMousePos( ).y - m_iMenuPosY;
    }
    else if ( bCurrestState && bLastState )
    {
        m_iMenuPosX = ImGui::GetMousePos( ).x - iOffsetX;
        m_iMenuPosY = ImGui::GetMousePos( ).y - iOffsetY;
    }

    for ( auto& cElem : m_cVecElemets )
    {
        if ( cElem.GetType( ) == ET_WINDOW_BEGIN && cElem.GetVar( _h( "Text" ) ).GetString( ) == "Tabs" )
        {
            cElem.GetVar( _h( "Origin" ) ).GetVec2( ) = ImVec2( m_iMenuPosX, m_iMenuPosY );
        }
        else if ( cElem.GetType( ) == ET_WINDOW_BEGIN && cElem.GetVar( _h( "Text" ) ).GetString( ) == "Main" )
        {
            cElem.GetVar( _h( "Origin" ) ).GetVec2( ) = ImVec2( m_iMenuPosX + 235.f, m_iMenuPosY );
        }
    }

    bLastState = bCurrestState;
}

void C_Nl::Draw( LPDIRECT3DDEVICE9 pDevice, bool bState )
{
    static bool iLastState = false;
    bool iCurrentState = bState;
    
    m_pDevice = pDevice;

    InitializeTextures( );

    InitializeCreateCallbacks( );

    InitializeAnimationCallbacks( );

    InitializeStyle( );

    InitializeElements( );

    if ( iCurrentState && iCurrentState != iLastState )
        ADD_ANIMATION( C_Animation( ).AlphaAnimate( &m_cVecElemets[0], AT_HIGH_TO_LOW, 0.1f, 1.f ) );
    else if ( !iCurrentState && iCurrentState != iLastState )
        ADD_ANIMATION( C_Animation( ).AlphaAnimate( &m_cVecElemets[0], AT_HIGH_TO_LOW, 0.1f, 0.f ) );

    ProcessAnimations( );

    m_cMapCreateCallbacks[m_cVecElemets[0].GetVar( _h( "CreateCallback" ) ).GetUInt32( )]( &m_cVecElemets[0] );

    if ( ImGui::GetStyle( ).Alpha == 0.f )
    {
        iLastState = iCurrentState;
        ResetAnimations( );
        return;
    }

    for ( auto& cElem : m_cVecElemets )
        if ( !m_bSkipElements || cElem.GetType( ) == ET_CHILD_BEGIN || cElem.GetType( ) == ET_CHILD_END )
            ProcessElement( &cElem );

    g_Keybinds->DrawKeybindWindow( );
    g_ColorPicker->DrawColorpickerWindow( );

    ImGui::GetForegroundDrawList( )->AddRectFilled( ImVec2( m_iMenuPosX + 234 + 1, m_iMenuPosY ), ImVec2( m_iMenuPosX + 235 + 3, m_iMenuPosY + 871 ), ImGui::ColorConvertFloat4ToU32( RGB_VEC( 24.f, 24.f, 24.f, 255.f * ImGui::GetStyle( ).Alpha ) ) );
    //ImGui::GetForegroundDrawList( )->AddRectFilled( ImVec2( m_iMenuPosX, m_iMenuPosY + 792.f ), ImVec2( m_iMenuPosX + 237.f, m_iMenuPosY + 795.f ), ImGui::ColorConvertFloat4ToU32( RGB_VEC( 24.f, 24.f, 24.f, 255.f * ImGui::GetStyle( ).Alpha ) ) );
    ImGui::GetForegroundDrawList( )->AddRectFilled( ImVec2( m_iMenuPosX + 234 + 1, m_iMenuPosY + 88 ), ImVec2( m_iMenuPosX + 235 + 762, m_iMenuPosY + 91 ), ImGui::ColorConvertFloat4ToU32( RGB_VEC( 24.f, 24.f, 24.f, 255.f * ImGui::GetStyle( ).Alpha ) ) );

    MoveWindows( );

    if ( m_bSelectingShit == 2 )
        m_bSelectingShit = 0;

    iLastState = iCurrentState;
}

void C_Nl::ProcessElement( C_Element* pElement )
{
    m_cMapCreateCallbacks[pElement->GetVar( _h( "CreateCallback" ) ).GetUInt32( )]( pElement );
}

bool C_Nl::CheckMouseIntersectionLocal( ImVec2 sStart, ImVec2 sSize )
{
    auto sMousePos = ImGui::GetIO( ).MousePos;

    sMousePos.x -= ImGui::GetWindowPos( ).x;
    sMousePos.y -= ImGui::GetWindowPos( ).y;

    return sMousePos.x >= sStart.x && sMousePos.x <= ( sStart.x + sSize.x ) && sMousePos.y >= sStart.y && sMousePos.y <= ( sStart.y + sSize.y );
}

void C_Nl::ClearElementAnimations( C_Element* pElement, uint32_t u32CallbackType )
{
    for ( int i = 0; i < m_cVecAnimations.size( ); ++i )
        if ( m_cVecAnimations[i].GetElement( ) == pElement && m_cVecAnimations[i].GetVar( _h( "Callback" ) ).GetUInt32( ) == u32CallbackType )
            m_cVecAnimations.erase( m_cVecAnimations.begin( ) + i );
}

bool C_Nl::IsAnimationPresent ( C_Element* pElement, uint32_t u32CallbackType )
{
    for ( int i = 0; i < m_cVecAnimations.size ( ); ++i )
        if ( m_cVecAnimations [ i ].GetElement ( ) == pElement && m_cVecAnimations [ i ].GetVar ( _h ( "Callback" ) ).GetUInt32 ( ) == u32CallbackType )
            return true;

    return false;
}

void C_Nl::ResetAnimations( )
{
    for ( auto& cElem : m_cVecElemets )
    {
        if ( cElem.GetType( ) == ET_CHECKBOX )
            cElem.ResetCheckbox( );

        if ( cElem.GetType( ) == ET_SLIDER )
            cElem.ResetSlider( );
    }
}
