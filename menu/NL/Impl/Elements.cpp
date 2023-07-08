
#include "..\..\..\nem.hpp"
#include "..\..\..\cloud\cloud.h"
void OnCreate( )
{
    if ( strlen( g_NL->m_chConfigName ) < 20 )
    {
        g_worker->push_create( g_NL->m_chConfigName );
        g_worker->push_refresh( );
        g_worker->push_cfg_save( g_NL->m_chConfigName );

        memset( g_NL->m_chConfigName, 0x00, 256 );
    }
}

void OnDelete( )
{
    g_worker->push_delete( );
    g_worker->push_refresh( );
}

void OnRefresh( )
{
    g_worker->push_refresh( );
}

void OnSave( )
{
    sdk::g_cfg->save( sdk::g_cfg->m_cfg_idx );
}

void OnLoad( )
{
    sdk::g_cfg->load( sdk::g_cfg->m_cfg_idx );
}

void OnShare( )
{
    g_worker->push_share( );
}

int wtf{};

void OnImport( )
{
    g_worker->push_import( );
    g_worker->push_refresh( );
}

void C_Nl::InitializeElements( )
{
    CALL_ONCE_START;
    {
        memset( m_chConfigName, 0x00, 256 );

        ADD_ELEMENT( C_Element( ).GlobalAlphaController( ) );

        ADD_ELEMENT( C_Element( ).WindowBegin( "Tabs", ImVec2( m_iMenuPosX, m_iMenuPosY ), ImVec2( 237, 871 ), RGB_VEC( 26.f, 25.f, 28.f, 200.f ), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar, true ) );
        {
            constexpr float flOffsetX = 0.f;
            constexpr float flOffsetY = -6.f;

            ADD_ELEMENT( C_Element( ).Text( "OFF-WHITE", ImVec2( flOffsetX + 23.f, flOffsetY + 29.f ), RGB_VEC( 6.f, 65.f, 97.f, 255.f ), pLogoFont ) );
            ADD_ELEMENT( C_Element( ).Text( "OFF-WHITE", ImVec2( flOffsetX + 24.f, flOffsetY + 30.f ), RGB_VEC( 4.f, 141.f, 206.f, 255.f ), pLogoFont ) );
            ADD_ELEMENT( C_Element( ).Text( "OFF-WHITE", ImVec2( flOffsetX + 25.f, flOffsetY + 31.f ), RGB_VEC( 255.f, 255.f, 255.f, 255.f ), pLogoFont ) );

            ADD_ELEMENT( C_Element( ).Text( "Aimbot", ImVec2( 25.f, 91.f ), RGB_VEC( 175.f, 175.f, 175.f, 255.f ), pTabGroupsNameFont ) );

            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 91.f + 33.f ), m_pIcons[IC_AIMBOT], ImVec2( 18.f, 18.f ), "Ragebot", &m_iSelectedTab, 0 ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 91.f + 33.f + 44.f ), m_pIcons[IC_ANTIAIM], ImVec2( 18.f, 18.f ), "Anti Aim", &m_iSelectedTab, 1 ) );
            //ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 91.f + 33.f + 44.f + 44.f ), m_pIcons[IC_LEGITBOT], ImVec2( 18.f, 19.f ), "Soon...", &m_iSelectedTab, 2 ) );


            ADD_ELEMENT( C_Element( ).Text( "Visuals", ImVec2( 25.f, 227.f ), RGB_VEC( 175.f, 175.f, 175.f, 255.f ), pTabGroupsNameFont ) );

            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 227.f + 33.f ), m_pIcons[IC_PLAYERS], ImVec2( 18.f, 19.f ), "Players", &m_iSelectedTab, 2 ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 227.f + 33.f + 44.f ), m_pIcons[IC_BOMB], ImVec2( 18.f, 18.f ), "Extra", &m_iSelectedTab, 3 ) );
            //ADD_ELEMENT( C_Element( ).Tab( ImVec2( 25.f, 267.f + 33.f + 44.f + 44.f ), m_pIcons[IC_WEAPONS], ImVec2( 25.f, 18.f ), "Weapon", &m_iSelectedTab, 5, ImVec2( 38.f, 1.f ) ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 227.f + 33.f + 44.f + 44.f ), m_pIcons[IC_GRENADES], ImVec2( 18.f, 18.f ), "Grenades", &m_iSelectedTab, 4 ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 227.f + 33.f + 44.f + 44.f + 44.f ), m_pIcons[IC_WORLD], ImVec2( 18.f, 18.f ), "World", &m_iSelectedTab, 5 ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 227.f + 33.f + 44.f + 44.f + 44.f + 44.f ), m_pIcons[IC_VIEW], ImVec2( 18.f, 18.f ), "View", &m_iSelectedTab, 6 ) );


            ADD_ELEMENT( C_Element( ).Text( "Miscellaneous", ImVec2( 25.f, 500.f ), RGB_VEC( 175.f, 175.f, 175.f, 255.f ), pTabGroupsNameFont ) );

            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 500.f + 33.f ), m_pIcons[IC_MAIN], ImVec2( 18.f, 18.f ), "Main", &m_iSelectedTab, 7 ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 500.f + 33.f + 44.f ), m_pIcons[IC_INVENTORY], ImVec2( 20.f, 18.f ), "Skins", &m_iSelectedTab, 8 ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 26.f, 500.f + 33.f + 44.f + 44.f ), m_pIcons[IC_CONFIGS], ImVec2( 18.f, 18.f ), "Configs", &m_iSelectedTab, 9 ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 26.f, 500.f + 33.f + 44.f + 44.f + 44.f ), m_pIcons[IC_SCRIPTS], ImVec2( 23.f, 18.f ), "Scripts", &m_iSelectedTab, 10, ImVec2( 37.f, 1.f ) ) );

        }
        ADD_ELEMENT( C_Element( ).WindowEnd( ) );

        ADD_ELEMENT( C_Element( ).WindowBegin( "Main", ImVec2( m_iMenuPosX + 235, m_iMenuPosY ), ImVec2( 762, 871 ), RGB_VEC( 8.f, 8.f, 8.f, 255.f ), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse, true ) );
        {
            m_iCurrentTabSetup = 0;

            // Ragebot
            ADD_ELEMENT ( C_Element ( ).ChildBegin ( "Main ", ImVec2 ( 28, 116 ), ImVec2 ( 352, 269 ), false, 0 ) );
            {
                ADD_ELEMENT ( C_Element ( ).Checkbox ( _h ( "CB_ENABLE_RAGEBOT" ), "Enable Ragebot", CHILD_ELEMENT_POS ( 0 ), &nem::hacks::g_aim_bot->cfg ( ).m_bEnableRagebot ) );
                ADD_ELEMENT ( C_Element ( ).Checkbox ( _h ( "CB_REMOVE_RECOIL" ), "Remove Recoil", CHILD_ELEMENT_POS ( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bRemoveRecoil ) );
                ADD_ELEMENT ( C_Element ( ).Checkbox ( _h ( "CB_AUTO_SCOPE" ), "Auto Scope", CHILD_ELEMENT_POS ( 2 ), &nem::hacks::g_aim_bot->cfg ( ).m_bAutoScope ) );
                ADD_ELEMENT ( C_Element ( ).Checkbox ( _h ( "CB_MULTI_THREAD" ), "Multithread", CHILD_ELEMENT_POS ( 3 ), &nem::hacks::g_aim_bot->cfg ( ).m_bMultiThread ) );
                ADD_ELEMENT ( C_Element ( ).Combo ( _h ( "CMB_CURRENT_WEAPON" ), "Current Weapon", CHILD_ELEMENT_POS ( 4 ), { "Pistol", "Deagle/R8", "Rifles", "Scar", "SSG 08", "AWP" }, &m_iSelectedWeaponGroup ) );
            }
            ADD_ELEMENT ( C_Element ( ).ChildEnd ( false, 0 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Expoits", ImVec2( 392, 116 ), ImVec2( 346, 146 ), false, 0 ) );
            {
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_ENABLE_DT" ), "Double Tap", CHILD_ELEMENT_POS( 0 ), &nem::hacks::g_aim_bot->cfg ( ).m_bDoubleTap ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_ENABLE_HS" ), "Hide Shots", CHILD_ELEMENT_POS( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bHideShots ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 0 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Min. Damage ", ImVec2( 392, 275 ), ImVec2( 346, 193 ), false, 0 ) );
            {
                m_iIndexMinDamage = m_cVecElemets.size( );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_MIN_DAMAGE_PISTOLS" ), "Min. Damage", CHILD_ELEMENT_POS( 0 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iMinDamage_Pistols ) );
                m_iIndexAutoStopType = m_cVecElemets.size( );
                ADD_ELEMENT( C_Element( ).Combo( _h( "SL_STOP_TYPE_PISTOLS" ), "Stop Type", CHILD_ELEMENT_POS( 1 ), { "Minimal", "Full" }, &nem::hacks::g_aim_bot->cfg ( ).m_iStopTypePistols ) );
                m_iIndexAutoStopPred = m_cVecElemets.size( );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "SL_STOP_PRED_PISTOLS" ), "Early Stop", CHILD_ELEMENT_POS( 2 ), &nem::hacks::g_aim_bot->cfg( ).m_bPredAutoStopPistols ) );

            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 0 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Accuracy ", ImVec2( 28, 398 ), ImVec2( 352, 356 ), false, 0 ) );
            {
                m_iIndexHitchance = m_cVecElemets.size( );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_HITCHANCE_PISTOLS" ), "Hit Chance", CHILD_ELEMENT_POS( 0 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iHitchance_Pistols ) );
                m_iIndexStaticPoints = m_cVecElemets.size( );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_STATIC_PS_PISTOLS" ), "Static Point Scale", CHILD_ELEMENT_POS( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bStaticPoints_Pistols ) );
                m_iIndexHeadScale = m_cVecElemets.size( );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_HEAD_SCALE_PISTOLS" ), "Head Scale", CHILD_ELEMENT_POS( 2 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iHeadScale_Pistols ) );
                m_iIndexBodyScale = m_cVecElemets.size( );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_BODY_SCALE_PISTOLS" ), "Body Scale", CHILD_ELEMENT_POS( 3 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iBodyScale_Pistols ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_FORCE_SP" ), "Force Safe Points", CHILD_ELEMENT_POS( 4 ), &nem::hacks::g_aim_bot->cfg ( ).m_bForceSafePoints ) );
                m_iIndexHitboxes = m_cVecElemets.size( );
                ADD_ELEMENT( C_Element( ).MultiCombo( _h( "MCMB_HITBOXES_PISTOLS" ), "Hitboxes", CHILD_ELEMENT_POS( 5 ), { "Head", "Body", "Stomach", "Arms", "Legs", "Feet" }, &nem::hacks::g_aim_bot->cfg ( ).m_iHitboxes_Pistols ) );
                m_iIndexMultipoints = m_cVecElemets.size( );
                ADD_ELEMENT( C_Element( ).MultiCombo( _h( "MCMB_MULTIPOINTS_PISTOLS" ), "Multipoints", CHILD_ELEMENT_POS( 6 ), { "Head", "Body", "Stomach", "Arms", "Legs", "Feet" }, &nem::hacks::g_aim_bot->cfg ( ).m_iMultipoints_Pistols ) );

            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 0 ) );

            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_MIN_DAMAGE_DEAGLER8" ), "Min. Damage", CHILD_ELEMENT_POS( 0 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iMinDamage_DeagleR8 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Combo( _h( "SL_STOP_TYPE_R6" ), "Stop Type", CHILD_ELEMENT_POS( 1 ), { "Minimal", "Full" }, &nem::hacks::g_aim_bot->cfg ( ).m_iStopTypeDeagleR8 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Checkbox( _h( "SL_STOP_PRED_R6" ), "Early Stop", CHILD_ELEMENT_POS( 2 ), &nem::hacks::g_aim_bot->cfg( ).m_bPredAutoStopR8 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_HITCHANCE_DEAGLER8" ), "Hit Chance", CHILD_ELEMENT_POS( 0 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iHitchance_DeagleR8 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Checkbox( _h( "CB_STATIC_PS_DEAGLER8" ), "Static Point Scale", CHILD_ELEMENT_POS( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bStaticPoints_DeagleR8 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_HEAD_SCALE_DEAGLER8" ), "Head Scale", CHILD_ELEMENT_POS( 2 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iHeadScale_DeagleR8 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_BODY_SCALE_DEAGLER8" ), "Body Scale", CHILD_ELEMENT_POS( 3 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iBodyScale_DeagleR8 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).MultiCombo( _h( "MCMB_HITBOXES_DEAGLER8" ), "Hitboxes", CHILD_ELEMENT_POS( 5 ), { "Head", "Body", "Stomach", "Arms", "Legs", "Feet" }, & nem::hacks::g_aim_bot->cfg ( ).m_iHitboxes_DeagleR8 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).MultiCombo( _h( "MCMB_MULTIPOINTS_DEAGLER8" ), "Multipoints", CHILD_ELEMENT_POS( 6 ), { "Head", "Body", "Stomach", "Arms", "Legs", "Feet" }, & nem::hacks::g_aim_bot->cfg ( ).m_iMultipoints_DeagleR8 ) );

            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_MIN_DAMAGE_RIFLES" ), "Min. Damage", CHILD_ELEMENT_POS( 0 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iMinDamage_Rifles ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Combo( _h( "SL_STOP_TYPE_RIFLES" ), "Stop Type", CHILD_ELEMENT_POS( 1 ), { "Minimal", "Full" }, & nem::hacks::g_aim_bot->cfg ( ).m_iStopTypeRifles ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Checkbox( _h( "SL_STOP_PRED_RIFLES" ), "Early Stop", CHILD_ELEMENT_POS( 2 ), &nem::hacks::g_aim_bot->cfg( ).m_bPredAutoStopRifles ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_HITCHANCE_RIFLES" ), "Hit Chance", CHILD_ELEMENT_POS( 0 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iHitchance_Rifles ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Checkbox( _h( "CB_STATIC_PS_RIFLES" ), "Static Point Scale", CHILD_ELEMENT_POS( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bStaticPoints_Rifles ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_HEAD_SCALE_RIFLES" ), "Head Scale", CHILD_ELEMENT_POS( 2 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iHeadScale_Rifles ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_BODY_SCALE_RIFLES" ), "Body Scale", CHILD_ELEMENT_POS( 3 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iBodyScale_Rifles ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).MultiCombo( _h( "MCMB_HITBOXES_RIFLES" ), "Hitboxes", CHILD_ELEMENT_POS( 5 ), { "Head", "Body", "Stomach", "Arms", "Legs", "Feet" }, & nem::hacks::g_aim_bot->cfg ( ).m_iHitboxes_Rifles ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).MultiCombo( _h( "MCMB_MULTIPOINTS_RIFLES" ), "Multipoints", CHILD_ELEMENT_POS( 6 ), { "Head", "Body", "Stomach", "Arms", "Legs", "Feet" }, & nem::hacks::g_aim_bot->cfg ( ).m_iMultipoints_Rifles ) );

            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_MIN_DAMAGE_SCAR" ), "Min. Damage", CHILD_ELEMENT_POS( 0 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iMinDamage_Scar ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Combo( _h( "SL_STOP_TYPE_SCAR" ), "Stop Type", CHILD_ELEMENT_POS( 1 ), { "Minimal", "Full" }, & nem::hacks::g_aim_bot->cfg ( ).m_iStopTypeScar ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Checkbox( _h( "SL_STOP_PRED_SCAR" ), "Early Stop", CHILD_ELEMENT_POS( 2 ), &nem::hacks::g_aim_bot->cfg( ).m_bPredAutoStopScar ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_HITCHANCE_SCAR" ), "Hit Chance", CHILD_ELEMENT_POS( 0 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iHitchance_Scar ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Checkbox( _h( "CB_STATIC_PS_SCAR" ), "Static Point Scale", CHILD_ELEMENT_POS( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bStaticPoints_Scar ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_HEAD_SCALE_SCAR" ), "Head Scale", CHILD_ELEMENT_POS( 2 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iHeadScale_Scar ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_BODY_SCALE_SCAR" ), "Body Scale", CHILD_ELEMENT_POS( 3 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iBodyScale_Scar ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).MultiCombo( _h( "MCMB_HITBOXES_SCAR" ), "Hitboxes", CHILD_ELEMENT_POS( 5 ), { "Head", "Body", "Stomach", "Arms", "Legs", "Feet" }, & nem::hacks::g_aim_bot->cfg ( ).m_iHitboxes_Scar ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).MultiCombo( _h( "MCMB_MULTIPOINTS_SCAR" ), "Multipoints", CHILD_ELEMENT_POS( 6 ), { "Head", "Body", "Stomach", "Arms", "Legs", "Feet" }, & nem::hacks::g_aim_bot->cfg ( ).m_iMultipoints_Scar ) );

            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_MIN_DAMAGE_SSG08" ), "Min. Damage", CHILD_ELEMENT_POS( 0 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iMinDamage_SSG08 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Combo( _h( "SL_STOP_TYPE_SSG08" ), "Stop Type", CHILD_ELEMENT_POS( 1 ), { "Minimal", "Full" }, & nem::hacks::g_aim_bot->cfg ( ).m_iStopTypeSSG08 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Checkbox( _h( "SL_STOP_PRED_SSG08" ), "Early Stop", CHILD_ELEMENT_POS( 2 ), &nem::hacks::g_aim_bot->cfg( ).m_bPredAutoStopSSG08 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_HITCHANCE_SSG08" ), "Hit Chance", CHILD_ELEMENT_POS( 0 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iHitchance_SSG08 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Checkbox( _h( "CB_STATIC_PS_SSG08" ), "Static Point Scale", CHILD_ELEMENT_POS( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bStaticPoints_SSG08 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_HEAD_SCALE_SSG08" ), "Head Scale", CHILD_ELEMENT_POS( 2 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iHeadScale_SSG08 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_BODY_SCALE_SSG08" ), "Body Scale", CHILD_ELEMENT_POS( 3 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iBodyScale_SSG08 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).MultiCombo( _h( "MCMB_HITBOXES_SSG08" ), "Hitboxes", CHILD_ELEMENT_POS( 5 ), { "Head", "Body", "Stomach", "Arms", "Legs", "Feet" }, & nem::hacks::g_aim_bot->cfg ( ).m_iHitboxes_SSG08 ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).MultiCombo( _h( "MCMB_MULTIPOINTS_SSG08" ), "Multipoints", CHILD_ELEMENT_POS( 6 ), { "Head", "Body", "Stomach", "Arms", "Legs", "Feet" }, & nem::hacks::g_aim_bot->cfg ( ).m_iMultipoints_SSG08 ) );

            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_MIN_DAMAGE_AWP" ), "Min. Damage", CHILD_ELEMENT_POS( 0 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iMinDamage_AWP ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Combo( _h( "SL_STOP_TYPE_AWP" ), "Stop Type", CHILD_ELEMENT_POS( 1 ), { "Minimal", "Full" }, & nem::hacks::g_aim_bot->cfg ( ).m_iStopTypeAwp ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Checkbox( _h( "SL_STOP_PRED_AWP" ), "Early Stop", CHILD_ELEMENT_POS( 2 ), &nem::hacks::g_aim_bot->cfg( ).m_bPredAutoStopAwp ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_HITCHANCE_AWP" ), "Hit Chance", CHILD_ELEMENT_POS( 0 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iHitchance_AWP ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Checkbox( _h( "CB_STATIC_PS_AWP" ), "Static Point Scale", CHILD_ELEMENT_POS( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bStaticPoints_AWP ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_HEAD_SCALE_AWP" ), "Head Scale", CHILD_ELEMENT_POS( 2 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iHeadScale_AWP ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).Slider( _h( "SL_BODY_SCALE_AWP" ), "Body Scale", CHILD_ELEMENT_POS( 3 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iBodyScale_AWP ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).MultiCombo( _h( "MCMB_HITBOXES_AWP" ), "Hitboxes", CHILD_ELEMENT_POS( 5 ), { "Head", "Body", "Stomach", "Arms", "Legs", "Feet" }, & nem::hacks::g_aim_bot->cfg ( ).m_iHitboxes_AWP ) );
            ADD_ELEMENT_ADDITIONAL( C_Element( ).MultiCombo( _h( "MCMB_MULTIPOINTS_AWP" ), "Multipoints", CHILD_ELEMENT_POS( 6 ), { "Head", "Body", "Stomach", "Arms", "Legs", "Feet" }, & nem::hacks::g_aim_bot->cfg ( ).m_iMultipoints_AWP ) );

            m_iCurrentTabSetup = 1;

            // Anti-Aim
            ADD_ELEMENT( C_Element( ).ChildBegin( "Main  ", ImVec2( 28, 116 ), ImVec2( 352, 315 ), false, 1 ) );
            {
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_ANTI_AIM" ), "Enable Anti Aim", CHILD_ELEMENT_POS( 0 ), &nem::hacks::g_aim_bot->cfg ( ).m_bAntiAimEnabled ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_YAW" ), "Yaw", CHILD_ELEMENT_POS( 2 ), -180, 180, &nem::hacks::g_aim_bot->cfg ( ).m_iYaw ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_JITTER_SIDE" ), "Jitter Side", CHILD_ELEMENT_POS( 3 ), &nem::hacks::g_aim_bot->cfg ( ).m_bJitterSide ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_AT_TARGET" ), "At Target", CHILD_ELEMENT_POS( 4 ), &nem::hacks::g_aim_bot->cfg ( ).m_bAtTarget ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_JITTER_AMT" ), "Jitter Amount", CHILD_ELEMENT_POS( 5 ), 0, 180, &nem::hacks::g_aim_bot->cfg ( ).m_iJitterAmount ) );
                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_PITCH" ), "Pitch", CHILD_ELEMENT_POS( 1 ), { "None", "Down", "Up", "Zero" }, & nem::hacks::g_aim_bot->cfg ( ).m_iPitch ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 1 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Fake Angle", ImVec2( 392, 116 ), ImVec2( 346, 315 ), false, 1 ) );
            {
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_DESYNC" ), "Enable Fake Angle", CHILD_ELEMENT_POS( 0 ), &nem::hacks::g_aim_bot->cfg ( ).m_bDesync ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_DESYNC_INVERT" ), "Inverter", CHILD_ELEMENT_POS( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bInverter ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_DESYNC_DELTA" ), "Limit", CHILD_ELEMENT_POS( 2 ), 0, 58, &nem::hacks::g_aim_bot->cfg ( ).m_iDesyncDelta ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_ROLL_LEAN" ), "Enable Roll Lean", CHILD_ELEMENT_POS( 3 ), &nem::hacks::g_aim_bot->cfg ( ).m_bRollEnabled ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_ROLL_DELTA" ), "Roll Limit", CHILD_ELEMENT_POS( 4 ), -50, 50, &nem::hacks::g_aim_bot->cfg ( ).m_iRollLimit ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_AUTO_DIR" ), "Auto Direction", CHILD_ELEMENT_POS( 5 ), &nem::hacks::g_aim_bot->cfg ( ).m_bAutoDirection ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 1 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Fake Lag  ", ImVec2( 28, 444 ), ImVec2( 352, 186 ), false, 1 ) );
            {
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_FAKE_LAG" ), "Enable Fake Lag", CHILD_ELEMENT_POS( 0 ), &nem::hacks::g_aim_bot->cfg ( ).m_bFakeLagEnabled ) );
                ADD_ELEMENT( C_Element( ).MultiCombo( _h( "MCMB_FL_CONDITIONS" ), "Triggers", CHILD_ELEMENT_POS( 1 ), { "Always", "In Move", "In Air" }, & nem::hacks::g_aim_bot->cfg ( ).m_iFakeLagsTriggers ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_FAKE_LAG_LIMIT" ), "Limit", CHILD_ELEMENT_POS( 2 ), 0, 14, &nem::hacks::g_aim_bot->cfg ( ).m_iFakeLagsLimit ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 1 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Misc        ", ImVec2( 392, 444 ), ImVec2( 346, 146 ), false, 1 ) );
            {
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_SLOW_WALK" ), "Slow Walk", CHILD_ELEMENT_POS( 0 ), &nem::hacks::g_aim_bot->cfg ( ).m_bSlowWalk ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_FAKE_DUCK" ), "Fake Duck", CHILD_ELEMENT_POS( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bFakeDuck ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 1 ) );

            m_iCurrentTabSetup = 2;
            // Players
            ADD_ELEMENT( C_Element( ).ChildBegin( "ESP   ", ImVec2( 28, 116 ), ImVec2( 352, 521 ), false, 2 ) );
            {
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_NAME" ), "Name", CHILD_ELEMENT_POS( 0 ), &nem::hacks::g_aim_bot->cfg ( ).m_bName ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_HEALTH" ), "Health", CHILD_ELEMENT_POS( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bHealth ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_BOX" ), "Box", CHILD_ELEMENT_POS( 2 ), &nem::hacks::g_aim_bot->cfg ( ).m_bBox ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_FLAGS" ), "Flags", CHILD_ELEMENT_POS( 3 ), &nem::hacks::g_aim_bot->cfg ( ).m_bFlags ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_HIT_MARKER" ), "Hit Marker", CHILD_ELEMENT_POS( 4 ), &nem::hacks::g_aim_bot->cfg ( ).m_bHitMarker ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_WEAPON_ICON" ), "Weapon Icon", CHILD_ELEMENT_POS( 5 ), &nem::hacks::g_aim_bot->cfg ( ).m_bWeaponIcon ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_WEAPON_TEXT" ), "Weapon Text", CHILD_ELEMENT_POS( 6 ), &nem::hacks::g_aim_bot->cfg ( ).m_bWeaponText ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_AMMO" ), "Ammo", CHILD_ELEMENT_POS( 7 ), &nem::hacks::g_aim_bot->cfg ( ).m_bAmmo ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_SKELETON" ), "Skeleton", CHILD_ELEMENT_POS( 8 ), &nem::hacks::g_aim_bot->cfg ( ).m_bSkeleton ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_OOF_ARROW" ), "OOF Arrow", CHILD_ELEMENT_POS( 9 ), &nem::hacks::g_aim_bot->cfg ( ).m_bOOFArrows ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 263.f, CHILD_ELEMENT_POS( 10 ), nem::hacks::g_aim_bot->cfg ( ).m_flGlowColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_GLOW" ), "Glow", CHILD_ELEMENT_POS( 10 ), &nem::hacks::g_aim_bot->cfg ( ).m_bGlow ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 2 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Chams     ", ImVec2( 392, 116 ), ImVec2( 346, 688 ), false, 2 ) );
            {
                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 0 ), nem::hacks::g_aim_bot->cfg ( ).m_flPlayerChamsColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_PLAYER_CHAMS" ), "Player Chams", CHILD_ELEMENT_POS( 0 ), &nem::hacks::g_aim_bot->cfg ( ).m_bPlayerChams ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 1 ), nem::hacks::g_aim_bot->cfg ( ).m_flInvisiblePlayerChamsColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_INV_PLAYER_CHAMS" ), "Invisible Player Chams", CHILD_ELEMENT_POS( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bInvisiblePlayerChams ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_CHAMS_TYPE" ), "Chams Type", CHILD_ELEMENT_POS( 2 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & nem::hacks::g_aim_bot->cfg ( ).m_iChamsType ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 3 ), nem::hacks::g_aim_bot->cfg ( ).m_flGlowOutlineColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_PLAYER_GLOW_OUTLINE" ), "Glow Outline", CHILD_ELEMENT_POS( 3 ), &nem::hacks::g_aim_bot->cfg ( ).m_bGlowOutline ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 4 ), nem::hacks::g_aim_bot->cfg ( ).m_flInvisibleGlowOutlineColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_INV_PLAYER_GLOW_OUTLINE" ), "Invisible Glow Outline", CHILD_ELEMENT_POS( 4 ), &nem::hacks::g_aim_bot->cfg ( ).m_bInvisibleGlowOutline ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 5 ), nem::hacks::g_aim_bot->cfg ( ).m_flHistoryColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_PLAYER_HISTORY" ), "Player History", CHILD_ELEMENT_POS( 5 ), &nem::hacks::g_aim_bot->cfg ( ).m_bHistory ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_HISTORY_CHAMS_TYPE" ), "History Chams Type", CHILD_ELEMENT_POS( 6 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & nem::hacks::g_aim_bot->cfg ( ).m_iHistoryChamsType ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 7 ), nem::hacks::g_aim_bot->cfg ( ).m_flLocalPlayerChamsColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_LOCAL_PLAYER" ), "Local Chams", CHILD_ELEMENT_POS( 7 ), &nem::hacks::g_aim_bot->cfg ( ).m_bLocalPlayerChams ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_LOCAL_CHAMS_TYPE" ), "Local Chams Type", CHILD_ELEMENT_POS( 8 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & nem::hacks::g_aim_bot->cfg ( ).m_iLocalPlayerChamsType ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 9 ), nem::hacks::g_aim_bot->cfg ( ).m_flLocalPlayerGlowOutlineColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_LOCAL_GLOW_OUTLINE" ), "Local Glow Outline", CHILD_ELEMENT_POS( 9 ), &nem::hacks::g_aim_bot->cfg ( ).m_bLocalPlayerGlowOutline ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 10 ), nem::hacks::g_aim_bot->cfg ( ).m_flLocalPlayerFakeColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_LOCAL_FAKE" ), "Local Fake", CHILD_ELEMENT_POS( 10 ), &nem::hacks::g_aim_bot->cfg ( ).m_bLocalPlayerFake ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_LOCAL_FAKE_CHAMS_TYPE" ), "Local Fake Type", CHILD_ELEMENT_POS( 11 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & nem::hacks::g_aim_bot->cfg ( ).m_iLocalFakeType ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 12 ), nem::hacks::g_aim_bot->cfg ( ).m_flLocalPlayerFakeGlowOutlineColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_LOCAL_FAKE_GLOW_OUTLINE" ), "Local Fake Glow Outline", CHILD_ELEMENT_POS( 12 ), &nem::hacks::g_aim_bot->cfg ( ).m_bLocalPlayerFakeGlowOutline ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 13 ), nem::hacks::g_aim_bot->cfg ( ).m_flRagdollsColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_RAGDOLLS" ), "Ragdolls", CHILD_ELEMENT_POS( 13 ), &nem::hacks::g_aim_bot->cfg ( ).m_bRagdolls ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_RAGDOLLS_TYPE" ), "Ragdolls Type", CHILD_ELEMENT_POS( 14 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & nem::hacks::g_aim_bot->cfg ( ).m_iRagdollsType ) );

            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 2 ) );


            m_iCurrentTabSetup = 3;
            // Extra

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main 1     ", ImVec2( 28, 116 ), ImVec2( 352, 318 ), false, 3 ) );
            {
                ADD_ELEMENT( C_Element( ).ColorPicker( 263.f, CHILD_ELEMENT_POS( 0 ), nem::hacks::g_aim_bot->cfg ( ).m_flHandChamsColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_HAND_CHAMS" ), "Hands Chams", CHILD_ELEMENT_POS( 0 ), &nem::hacks::g_aim_bot->cfg ( ).m_bHandChams ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_HANDLE_CHAMS_TYPE" ), "Hands Chams Type", CHILD_ELEMENT_POS( 1 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & nem::hacks::g_aim_bot->cfg ( ).m_iHandsChamsType ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 263.f, CHILD_ELEMENT_POS( 2 ), nem::hacks::g_aim_bot->cfg ( ).m_flHandGlowOutlineChamsColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_HAND_GLOW_OUTLINE_CHAMS" ), "Hands Glow Outline Chams", CHILD_ELEMENT_POS( 2 ), &nem::hacks::g_aim_bot->cfg ( ).m_bHandGlowOutlineChams ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 263.f, CHILD_ELEMENT_POS( 3 ), nem::hacks::g_aim_bot->cfg ( ).m_flLocalWeaponChamsColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_LOCAL_WEAPON_CHAMS" ), "Local Weapon Chams", CHILD_ELEMENT_POS( 3 ), &nem::hacks::g_aim_bot->cfg ( ).m_bLocalWeaponChams ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_LOCAL_WEAPON_CHAMS_TYPE" ), "Local Weapon Type", CHILD_ELEMENT_POS( 4 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & nem::hacks::g_aim_bot->cfg ( ).m_iLocalWeaponChamsType ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 263.f, CHILD_ELEMENT_POS( 5 ), nem::hacks::g_aim_bot->cfg ( ).m_flLocalWeaponGlowOutlineChamsColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_LOCAL_WEAPON_GLOW_OUTLINE" ), "Local Weapon Glow Outline", CHILD_ELEMENT_POS( 5 ), &nem::hacks::g_aim_bot->cfg ( ).m_bLocalWeaponGlowOutlineChams ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 3 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main 2        ", ImVec2( 392, 116 ), ImVec2( 346, 318 ), false, 3 ) );
            {
                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 0 ), nem::hacks::g_aim_bot->cfg ( ).m_flWeaponChamsColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_WEAPON_CHAMS" ), "Weapon Chams", CHILD_ELEMENT_POS( 0 ), &nem::hacks::g_aim_bot->cfg ( ).m_bWeaponChams ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_WEAPON_CHAMS_TYPE" ), "Weapon Chams Type", CHILD_ELEMENT_POS( 1 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & nem::hacks::g_aim_bot->cfg ( ).m_iWeaponChamsType ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 2 ), nem::hacks::g_aim_bot->cfg ( ).m_flWeaponGlowOutlineChamsColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_WEAPON_GLOW_OUTLINE" ), "Weapon Glow Outline", CHILD_ELEMENT_POS( 2 ), &nem::hacks::g_aim_bot->cfg ( ).m_bWeaponGlowOutlineChams ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 3 ), nem::hacks::g_aim_bot->cfg ( ).m_flShotRecordColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_SHOT_RECORD" ), "Shot Record", CHILD_ELEMENT_POS( 3 ), &nem::hacks::g_aim_bot->cfg ( ).m_bShotRecord ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_SHOT_RECORD_TYPE" ), "Shot Record Type", CHILD_ELEMENT_POS( 4 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & nem::hacks::g_aim_bot->cfg ( ).m_iShotRecordType ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 5 ), nem::hacks::g_aim_bot->cfg ( ).m_flShotRecordGlowOutlineColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_SHOT_RECORD_GLOW_OUTLINE" ), "Shot Record Glow Outline", CHILD_ELEMENT_POS( 5 ), &nem::hacks::g_aim_bot->cfg ( ).m_bShotRecordGlowOutline ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 3 ) );


            m_iCurrentTabSetup = 4;
            // Grenades

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main           ", ImVec2( 28, 116 ), ImVec2( 352, 224 ), false, 4 ) );
            {
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_GRENADES" ), "Grenades", CHILD_ELEMENT_POS( 0 ), &nem::hacks::g_aim_bot->cfg ( ).m_bGrenades ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_GRENADE_PREDICTION" ), "Grenade Prediction", CHILD_ELEMENT_POS( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bGrenadePrediction ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_GRENADE_PROXIMITY_WARNING" ), "Grenade Proximity Warning", CHILD_ELEMENT_POS( 2 ), &nem::hacks::g_aim_bot->cfg ( ).m_bGrenadeProximityWarning ) );
                ADD_ELEMENT ( C_Element ( ).Checkbox ( _h ( "CB_C4_NIGGA" ), "C4 Indicator", CHILD_ELEMENT_POS ( 3 ), &nem::hacks::g_aim_bot->cfg ( ).m_bC4Indicator ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 4 ) );

            m_iCurrentTabSetup = 5;
            // World

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main                 ", ImVec2( 28, 116 ), ImVec2( 352, 643 ), false, 5 ) );
            {
                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_DROPPED_WEAPON" ), "Dropped Weapon", CHILD_ELEMENT_POS( 0 ), { "None", "Name", "Icon" }, & nem::hacks::g_aim_bot->cfg ( ).m_iDroppedWeapon ) );
                ADD_ELEMENT( C_Element( ).ColorPicker( 168.f, CHILD_ELEMENT_POS( 1 ), nem::hacks::g_aim_bot->cfg ( ).m_flNightModeColor ) );
                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_WORLD_TYPE" ), "World Type", CHILD_ELEMENT_POS( 1 ), { "None", "Full Bright", "Nightmode" }, & nem::hacks::g_aim_bot->cfg ( ).m_iWorldType ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_SKYBOX_TYPE" ), "Skybox Type", CHILD_ELEMENT_POS( 2 ), { ( "None" ),
                     ( "Tibet" ),
                     ( "Baggage" ),
                     ( "Italy" ),
                     ( "Aztec" ),
                     ( "Vertigo" ),
                     ( "Daylight" ),
                     ( "Daylight 2" ),
                     ( "Clouds" ),
                     ( "Clouds 2" ),
                     ( "Gray" ),
                     ( "Clear" ),
                     ( "Canals" ),
                     ( "Cobblestone" ),
                     ( "Assault" ),
                     ( "Clouds dark" ),
                     ( "Night" ),
                     ( "Night 2" ),
                     ( "Night flat" ),
                     ( "Dusty" ),
                     ( "Rainy" ) }, & nem::hacks::g_aim_bot->cfg ( ).m_iSkyboxType ) );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_SHADOWS_MOD" ), "Shadows Modulation", CHILD_ELEMENT_POS( 3 ), &nem::hacks::g_aim_bot->cfg ( ).m_bShadowsModulation ) );

                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_SHADOWS_X" ), "X Direction", CHILD_ELEMENT_POS( 4 ), -100, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iShadowsX ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_SHADOWS_Y" ), "Y Direction", CHILD_ELEMENT_POS( 5 ), -100, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iShadowsY ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_SHADOWS_Z" ), "Z Direction", CHILD_ELEMENT_POS( 6 ), -100, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iShadowsZ ) );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_TONE_MAP" ), "Tone Map Modulation", CHILD_ELEMENT_POS( 7 ), &nem::hacks::g_aim_bot->cfg ( ).m_bToneMapModulation ) );

                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_BLOOM" ), "Bloom", CHILD_ELEMENT_POS( 8 ), 0, 750, &nem::hacks::g_aim_bot->cfg ( ).m_iBloom ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_EXPOSURE" ), "Exposure", CHILD_ELEMENT_POS( 9 ), 0, 2000, &nem::hacks::g_aim_bot->cfg ( ).m_iExposure ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 10 ), nem::hacks::g_aim_bot->cfg ( ).m_flFogColor ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_FOG" ), "Fog", CHILD_ELEMENT_POS( 10 ), &nem::hacks::g_aim_bot->cfg ( ).m_bFog ) );

                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_FOG_START" ), "Fog Start", CHILD_ELEMENT_POS( 11 ), 0, 1000, &nem::hacks::g_aim_bot->cfg ( ).m_iFogStart ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_FOG_END" ), "Fog End", CHILD_ELEMENT_POS( 12 ), 100, 1100, &nem::hacks::g_aim_bot->cfg ( ).m_iFogEnd ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_FOG_DENSITY" ), "Density", CHILD_ELEMENT_POS( 13 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iDensity ) );


            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 5 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Additional  ", ImVec2( 392, 116 ), ImVec2( 346, 731 ), false, 5 ) );
            {
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_BULLET_TRACERS" ), "Bullet Tracers", CHILD_ELEMENT_POS( 0 ), &nem::hacks::g_aim_bot->cfg ( ).m_bBulletTracers ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_BULLET_IMPACTS" ), "Bullet Impacts", CHILD_ELEMENT_POS( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bBulletImpacts ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_DAMAGE_MARKER" ), "Damage Marker", CHILD_ELEMENT_POS( 2 ), &nem::hacks::g_aim_bot->cfg ( ).m_bDamageMarker ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_VISUAL_RECOIL" ), "Remove Visual Recoil", CHILD_ELEMENT_POS( 3 ), &nem::hacks::g_aim_bot->cfg ( ).m_bRemoveVisualRecoil ) );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_FLASH" ), "Remove Flash", CHILD_ELEMENT_POS( 4 ), &nem::hacks::g_aim_bot->cfg ( ).m_bRemoveFlash ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_SMOKE" ), "Remove Smoke", CHILD_ELEMENT_POS( 5 ), &nem::hacks::g_aim_bot->cfg ( ).m_bRemoveSmoke ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_FOG" ), "Remove Fog", CHILD_ELEMENT_POS( 6 ), &nem::hacks::g_aim_bot->cfg ( ).m_bRemoveFog ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_DISABLE_POST_PROCESSING" ), "Disable Post Processing", CHILD_ELEMENT_POS( 7 ), &nem::hacks::g_aim_bot->cfg ( ).m_bDisablePostProcessing ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_SCOPE" ), "Remove Scope", CHILD_ELEMENT_POS( 8 ), &nem::hacks::g_aim_bot->cfg ( ).m_bRemoveScope ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_SHADOWS" ), "Remove Shadows", CHILD_ELEMENT_POS( 9 ), &nem::hacks::g_aim_bot->cfg ( ).m_bRemoveShadows ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_LANDING_BOB" ), "Remove Landing Bob", CHILD_ELEMENT_POS( 10 ), &nem::hacks::g_aim_bot->cfg ( ).m_bRemoveLandingBob ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_HANDS_SHAKING" ), "Remove Hands Shaking", CHILD_ELEMENT_POS( 11 ), &nem::hacks::g_aim_bot->cfg ( ).m_bRemoveHandsShaking ) );

                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_PROP_TARNSPARENCY" ), "Props Transp.", CHILD_ELEMENT_POS( 12 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iPropTransparency ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_WALL_TARNSPARENCY" ), "Walls Transp.", CHILD_ELEMENT_POS( 13 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iWallTransparency ) );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_BLEND" ), "Enable Blend", CHILD_ELEMENT_POS( 14 ), &nem::hacks::g_aim_bot->cfg ( ).m_bBlend ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_BLEND_VAL" ), "Blend Value", CHILD_ELEMENT_POS( 15 ), 0, 100, &nem::hacks::g_aim_bot->cfg ( ).m_iBlendValue ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 5 ) );

            m_iCurrentTabSetup = 6;
            // View

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main                     ", ImVec2( 28, 116 ), ImVec2( 352, 428 ), false, 6 ) );
            {
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_THIRD_PERSON" ), "Third Person", CHILD_ELEMENT_POS( 0 ), &nem::hacks::g_aim_bot->cfg ( ).m_bThirdPerson ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_THIRD_PERSON_DEAD" ), "Third Person While Dead", CHILD_ELEMENT_POS( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bThirdPersonDead ) );

                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_TP_DISTANCE" ), "Third Person Dist.", CHILD_ELEMENT_POS( 2 ), 60, 180, &nem::hacks::g_aim_bot->cfg ( ).m_iThirdPersonDistance ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_CMR_DISTANCE" ), "Camera Distance", CHILD_ELEMENT_POS( 3 ), 60, 140, &nem::hacks::g_aim_bot->cfg ( ).m_iCameraDistance ) );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_OV_IN_SCOPE" ), "Override In Scope", CHILD_ELEMENT_POS( 4 ), &nem::hacks::g_aim_bot->cfg ( ).m_bOvInScope ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_ASP_RATIO" ), "Aspect Ratio", CHILD_ELEMENT_POS( 5 ), &nem::hacks::g_aim_bot->cfg ( ).m_bAspectRatio ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_ASP_RATIO_VAL" ), "Aspect Ratio Value", CHILD_ELEMENT_POS( 6 ), 0, 30, &nem::hacks::g_aim_bot->cfg ( ).m_iAspectRatioValue ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_RAIN_PENIS" ), "Rain", CHILD_ELEMENT_POS( 7 ), &nem::hacks::g_aim_bot->cfg ( ).m_bRain ) );
                ADD_ELEMENT ( C_Element ( ).Checkbox ( _h ( "CB_MOTION_PENIS" ), "Motion Blur", CHILD_ELEMENT_POS ( 8 ), &nem::hacks::g_aim_bot->cfg ( ).m_bMotionBlur ) );
                
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 6 ) );

            m_iCurrentTabSetup = 7;
            // Misc

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main                          ", ImVec2( 28, 116 ), ImVec2( 352, 397 ), false, 7 ) );
            {
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_BUNNY_HOP" ), "Bunny Hop", CHILD_ELEMENT_POS( 0 ), &nem::hacks::g_aim_bot->cfg ( ).m_bBunnyHop ) );
                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_AUTO_STRAFE_TYPE" ), "Auto Strafe", CHILD_ELEMENT_POS( 1 ), { "None", "View Angles", "Directional" }, & nem::hacks::g_aim_bot->cfg ( ).m_iAutoStrafeType ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_AUTO_PEEK" ), "Auto Peek", CHILD_ELEMENT_POS( 2 ), &nem::hacks::g_aim_bot->cfg ( ).m_bAutoPeek ) );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_INFINITY_DUCK" ), "Infinity Duck", CHILD_ELEMENT_POS( 3 ), &nem::hacks::g_aim_bot->cfg ( ).m_bInfinityDuck ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_SLIDE_WALK" ), "Slide Walk", CHILD_ELEMENT_POS( 4 ), &nem::hacks::g_aim_bot->cfg ( ).m_bSlideWalk ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_FAST_STOP" ), "Fast Stop", CHILD_ELEMENT_POS( 5 ), &nem::hacks::g_aim_bot->cfg ( ).m_bFastStop ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_DISABLE_RAGDOLL_PHYSICS" ), "Disable Ragdoll Physics", CHILD_ELEMENT_POS( 6 ), &nem::hacks::g_aim_bot->cfg ( ).m_bDisableRagdollPhysics ) );

            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 7 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main 2                             ", ImVec2( 392, 150 ), ImVec2( 346, 397 ), false, 7 ) );
            {
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_HITSOUND" ), "Hitsound", CHILD_ELEMENT_POS( 0 ), &nem::hacks::g_aim_bot->cfg ( ).m_bHitsound ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_SPECTATORS" ), "Spectators", CHILD_ELEMENT_POS( 1 ), &nem::hacks::g_aim_bot->cfg ( ).m_bSpectators ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_KEYBINDS" ), "Keybinds", CHILD_ELEMENT_POS( 2 ), &nem::hacks::g_aim_bot->cfg ( ).m_bKeybinds ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_PRESERVE_KILLFEED" ), "Preserve Killfeed", CHILD_ELEMENT_POS( 3 ), &nem::hacks::g_aim_bot->cfg ( ).m_bPreserveKillfeed ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_BUYBOT" ), "BuyBot", CHILD_ELEMENT_POS( 4 ), &nem::hacks::g_aim_bot->cfg ( ).m_bBuyBot ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_BUYBOT_SNIPERS" ), "Snipers", CHILD_ELEMENT_POS( 5 ), { "None", "Auto", "AWP", "SSG 08" }, & nem::hacks::g_aim_bot->cfg ( ).m_iBuyBotSnipers ) );
                ADD_ELEMENT( C_Element( ).Combo( _h( "CMD_BUYBOT_PISTOLS" ), "Pistols", CHILD_ELEMENT_POS( 6 ), { "None", "Dual Berettas", "Deagle/R8" }, & nem::hacks::g_aim_bot->cfg ( ).m_iBuyBotPistols ) );

                ADD_ELEMENT( C_Element( ).MultiCombo( _h( "MCMB_BUYBOT_ADDITIONAL" ), "Additional", CHILD_ELEMENT_POS( 7 ), { "Grenades", "Armor", "Taser", "Defuser" }, & nem::hacks::g_aim_bot->cfg ( ).m_iBuyBotAdditional ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 7 ) );

            // Configs

            m_iCurrentTabSetup = 8;

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main                                             ", ImVec2( 28, 116 ), ImVec2( 352, 731 ), false, 8 ) );
            {
                ADD_ELEMENT( C_Element( ).SkinsBlock( ) );

         
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 8 ) );

            m_iCurrentTabSetup = 9;
            ADD_ELEMENT( C_Element( ).ChildBegin( "Main                                   ", ImVec2( 28, 116 ), ImVec2( 352, 731 ), false, 9 ) );
            {
                m_iConfigsElementIndex = m_cVecElemets.size( );
                ADD_ELEMENT( C_Element( ).Combo( 0, "Configs list", CHILD_ELEMENT_POS( 0 ), { "Slot 1", "Slot 2", "Slot 3", "Slot 4", "Slot 5" }, & sdk::g_cfg->m_cfg_idx ) );
                ADD_ELEMENT( C_Element( ).Button( "Save", CHILD_ELEMENT_POS( 1 ), ImVec2( 70.f, 30.f ), OnSave ) );
                ADD_ELEMENT( C_Element( ).Button( "Load", CHILD_ELEMENT_POS( 2 ), ImVec2( 70.f, 30.f ), OnLoad ) );

            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 9 ) );

            m_iCurrentTabSetup = 10;

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main                                       ", ImVec2( 28, 116 ), ImVec2( 352, 731 ), false, 10 ) );
            {
                ADD_ELEMENT( C_Element( ).LuaBlock( ) );

            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 10 ) );

        }
        ADD_ELEMENT( C_Element( ).WindowEnd( ) );

        /*
        ADD_ELEMENT( C_Element( ).WindowBegin( "Preview", ImVec2( m_sMenuPos.x + 784 + 235, m_sMenuPos.y + 67.f ), ImVec2( 375, 738 ), RGB_VEC( 8.f, 8.f, 13.f, 255.f ), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus, true ) );
        {



        }
        ADD_ELEMENT( C_Element( ).WindowEnd( ) );

        ADD_ELEMENT( C_Element( ).ChildBegin( "Drag & Drop Elements", ImVec2( m_sMenuPos.x + 784 + 235, m_sMenuPos.y + 67.f + 607.f ), ImVec2( 375, 131 ), true ) );
        {
            Preview( m_vecPreviw, ImVec2( ), ImVec2( ), true );
            ADD_ELEMENT( C_Element( ).Preview( ) );
        }
        ADD_ELEMENT( C_Element( ).ChildEnd( true ) );
        */
    }
    CALL_ONCE_END;
    
}
