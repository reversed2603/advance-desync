//#include "..\Neverlose.h"
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
    g_worker->push_cfg_save( NULL );
}

void OnLoad( )
{
    g_worker->push_load( );
}

void OnShare( )
{
    g_worker->push_share( );
}

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

        ADD_ELEMENT( C_Element( ).WindowBegin( "Tabs", ImVec2( m_iMenuPosX, m_iMenuPosY ), ImVec2( 237, 871 ), RGB_VEC( 15.f, 27.f, 42.f, 255.f ), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar, true ) );
        {
            constexpr float flOffsetX = 0.f;
            constexpr float flOffsetY = -6.f;

            ADD_ELEMENT( C_Element( ).Text( "OFF-WHITE", ImVec2( flOffsetX + 17.f, flOffsetY + 29.f ), RGB_VEC( 6.f, 65.f, 97.f, 255.f ), pLogoFont ) );
            ADD_ELEMENT( C_Element( ).Text( "OFF-WHITE", ImVec2( flOffsetX + 18.f, flOffsetY + 30.f ), RGB_VEC( 4.f, 141.f, 206.f, 255.f ), pLogoFont ) );
            ADD_ELEMENT( C_Element( ).Text( "OFF-WHITE", ImVec2( flOffsetX + 19.f, flOffsetY + 31.f ), RGB_VEC( 255.f, 255.f, 255.f, 255.f ), pLogoFont ) );

            ADD_ELEMENT( C_Element( ).Text( "Aimbot", ImVec2( 25.f, 91.f ), RGB_VEC( 50, 70.f, 83.f, 255.f ), pTabGroupsNameFont ) );

            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 91.f + 33.f ), m_pIcons[IC_AIMBOT], ImVec2( 18.f, 18.f ), "Ragebot", &m_iSelectedTab, 0 ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 91.f + 33.f + 44.f ), m_pIcons[IC_ANTIAIM], ImVec2( 18.f, 18.f ), "Anti Aim", &m_iSelectedTab, 1 ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 91.f + 33.f + 44.f + 44.f ), m_pIcons[IC_LEGITBOT], ImVec2( 18.f, 19.f ), "Soon...", &m_iSelectedTab, 2 ) );


            ADD_ELEMENT( C_Element( ).Text( "Visuals", ImVec2( 25.f, 267.f ), RGB_VEC( 50, 70.f, 83.f, 255.f ), pTabGroupsNameFont ) );

            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 267.f + 33.f ), m_pIcons[IC_PLAYERS], ImVec2( 18.f, 19.f ), "Players", &m_iSelectedTab, 3 ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 267.f + 33.f + 44.f ), m_pIcons[IC_BOMB], ImVec2( 18.f, 18.f ), "Extra", &m_iSelectedTab, 4 ) );
            //ADD_ELEMENT( C_Element( ).Tab( ImVec2( 25.f, 267.f + 33.f + 44.f + 44.f ), m_pIcons[IC_WEAPONS], ImVec2( 25.f, 18.f ), "Weapon", &m_iSelectedTab, 5, ImVec2( 38.f, 1.f ) ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 267.f + 33.f + 44.f + 44.f ), m_pIcons[IC_GRENADES], ImVec2( 18.f, 18.f ), "Grenades", &m_iSelectedTab, 5 ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 267.f + 33.f + 44.f + 44.f + 44.f ), m_pIcons[IC_WORLD], ImVec2( 18.f, 18.f ), "World", &m_iSelectedTab, 6 ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 267.f + 33.f + 44.f + 44.f + 44.f + 44.f ), m_pIcons[IC_VIEW], ImVec2( 18.f, 18.f ), "View", &m_iSelectedTab, 7 ) );


            ADD_ELEMENT( C_Element( ).Text( "Miscellaneous", ImVec2( 25.f, 574.f ), RGB_VEC( 50, 70.f, 83.f, 255.f ), pTabGroupsNameFont ) );

            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 574.f + 33.f ), m_pIcons[IC_MAIN], ImVec2( 18.f, 18.f ), "Main", &m_iSelectedTab, 8 ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 28.f, 574.f + 33.f + 44.f ), m_pIcons[IC_INVENTORY], ImVec2( 20.f, 18.f ), "Skins", &m_iSelectedTab, 9 ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 26.f, 574.f + 33.f + 44.f + 44.f ), m_pIcons[IC_SCRIPTS], ImVec2( 23.f, 18.f ), "Scripts", &m_iSelectedTab, 10, ImVec2( 37.f, 1.f ) ) );
            ADD_ELEMENT( C_Element( ).Tab( ImVec2( 26.f, 574.f + 33.f + 44.f + 44.f + 44.f ), m_pIcons[IC_CONFIGS], ImVec2( 18.f, 18.f ), "Configs", &m_iSelectedTab, 11 ) );

        }
        ADD_ELEMENT( C_Element( ).WindowEnd( ) );

        ADD_ELEMENT( C_Element( ).WindowBegin( "Main", ImVec2( m_iMenuPosX + 235, m_iMenuPosY ), ImVec2( 762, 871 ), RGB_VEC( 8.f, 8.f, 13.f, 255.f ), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse, true ) );
        {
            // Ragebot
            ADD_ELEMENT( C_Element( ).ChildBegin( "Main ", ImVec2( 28, 116 ), ImVec2( 352, 269 ), false, 0 ) );
            {
                auto& cfg = nem::hacks::g_aim_bot->cfg( );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_ENABLE_RAGEBOT" ), "Enable Ragebot", CHILD_ELEMENT_POS( 0 ), &cfg.m_enabled ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_RECOIL" ), "Remove Recoil", CHILD_ELEMENT_POS( 1 ), &cfg.m_antirecoil ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_AUTO_SCOPE" ), "Auto Scope", CHILD_ELEMENT_POS( 2 ), &cfg.m_auto_scope ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_MULTI_THREAD" ), "Multithread", CHILD_ELEMENT_POS( 3 ), &cfg.m_multi_thread ) );
                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_CURRENT_WEAPON" ), "Current Weapon", CHILD_ELEMENT_POS( 4 ), { "Pistol", "Deagle/R8", "Rifles", "Scar", "SSG 08", "AWP" }, &m_iSelectedWeaponGroup ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 0 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Expoits", ImVec2( 392, 116 ), ImVec2( 346, 146 ), false, 0 ) );
            {
                auto& cfg = nem::hacks::g_aim_bot->cfg( );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_ENABLE_DT" ), "Double Tap", CHILD_ELEMENT_POS( 0 ), &cfg.m_double_tap ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_ENABLE_HS" ), "Hide Shots", CHILD_ELEMENT_POS( 1 ), &cfg.m_hide_shots ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 0 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Min. Damage ", ImVec2( 392, 275 ), ImVec2( 346, 186 ), false, 0 ) );
            {
                auto& cfg = nem::hacks::g_aim_bot->cfg( );

                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_MIN_DAMAGE" ), "Min. Damage", CHILD_ELEMENT_POS( 0 ), 0, 100, &cfg.m_pistols_min_dmg ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_MIN_DAMAGE_OV" ), "Override Enabled", CHILD_ELEMENT_POS( 1 ), &cfg.m_min_dmg_override_enabled ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_MIN_DAMAGE_OV" ), "Override Value", CHILD_ELEMENT_POS( 2 ), 0, 100, &cfg.m_pistols_min_dmg_override ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 0 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Accuracy ", ImVec2( 28, 398 ), ImVec2( 352, 356 ), false, 0 ) );
            {
                auto& cfg = nem::hacks::g_aim_bot->cfg( );

                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_HITCHANCE" ), "Hit Chance", CHILD_ELEMENT_POS( 0 ), 0, 100, &cfg.m_pistols_hit_chance ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_STATIC_PS" ), "Static Point Scale", CHILD_ELEMENT_POS( 1 ), &cfg.m_pistols_static_point_scale ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_HEAD_SCALE" ), "Head Scale", CHILD_ELEMENT_POS( 2 ), 0, 100, &cfg.m_pistols_head_scale ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_BODY_SCALE" ), "Body Scale", CHILD_ELEMENT_POS( 3 ), 0, 100, &cfg.m_pistols_body_scale ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_FORCE_SP" ), "Force Safe Points", CHILD_ELEMENT_POS( 4 ), &cfg.m_force_safe_points ) );
                ADD_ELEMENT( C_Element( ).MultiCombo( _h( "MCMB_HITBOXES" ), "Hitboxes", CHILD_ELEMENT_POS( 5 ), { "Head", "Body", "Stomach", "Arms", "Legs", "Feet" }, &cfg.m_pistols_hitgroups ) );
                ADD_ELEMENT( C_Element( ).MultiCombo( _h( "MCMB_MULTIPOINTS" ), "Multipoints", CHILD_ELEMENT_POS( 6 ), { "Head", "Body", "Stomach", "Arms", "Legs", "Feet" }, &cfg.m_pistols_multi_points ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 0 ) );


            // Anti-Aim
            ADD_ELEMENT( C_Element( ).ChildBegin( "Main  ", ImVec2( 28, 116 ), ImVec2( 352, 315 ), false, 1 ) );
            {
                auto& cfg = nem::hacks::g_anti_aim->cfg( );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_ANTI_AIM" ), "Enable Anti Aim", CHILD_ELEMENT_POS( 0 ), &cfg.m_enabled ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_YAW" ), "Yaw", CHILD_ELEMENT_POS( 2 ), -180, 180, &cfg.m_yaw_offset ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_JITTER_SIDE" ), "Jitter Side", CHILD_ELEMENT_POS( 3 ), &cfg.m_jitter_side ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_AT_TARGET" ), "At Target", CHILD_ELEMENT_POS( 4 ), &cfg.m_at_target ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_JITTER_AMT" ), "Jitter Amount", CHILD_ELEMENT_POS( 5 ), 0, 180, &cfg.m_jitter_amount ) );
                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_PITCH" ), "Pitch", CHILD_ELEMENT_POS( 1 ), { "None", "Down", "Up", "Zero" }, &cfg.m_pitch ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 1 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Fake Angle", ImVec2( 392, 116 ), ImVec2( 346, 315 ), false, 1 ) );
            {
                auto& cfg = nem::hacks::g_anti_aim->cfg( );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_DESYNC" ), "Enable Fake Angle", CHILD_ELEMENT_POS( 0 ), &cfg.m_desync ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_DESYNC_INVERT" ), "Inverter", CHILD_ELEMENT_POS( 1 ), &cfg.m_inverter ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_DESYNC_DELTA" ), "Limit", CHILD_ELEMENT_POS( 2 ), 0, 60, &cfg.m_desync_limit ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_ROLL_LEAN" ), "Enable Roll Lean", CHILD_ELEMENT_POS( 3 ), &cfg.m_roll_enabled ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_ROLL_DELTA" ), "Roll Limit", CHILD_ELEMENT_POS( 4 ), -50, 50, &cfg.m_roll_amt ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_AUTO_DIR" ), "Auto Direction", CHILD_ELEMENT_POS( 5 ), &cfg.m_auto_dir_type ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 1 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Fake Lag  ", ImVec2( 28, 444 ), ImVec2( 352, 186 ), false, 1 ) );
            {
                auto& cfg = nem::hacks::g_anti_aim->cfg( );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_FAKE_LAG" ), "Enable Fake Lag", CHILD_ELEMENT_POS( 0 ), &cfg.m_fake_lag.m_enabled ) );
                ADD_ELEMENT( C_Element( ).MultiCombo( _h( "MCMB_FL_CONDITIONS" ), "Triggers", CHILD_ELEMENT_POS( 1 ), { "Always", "In Move", "In Air" }, &cfg.m_fake_lag.m_conditions ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_FAKE_LAG_LIMIT" ), "Limit", CHILD_ELEMENT_POS( 2 ), 0, 14, &cfg.m_fake_lag.m_limit ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 1 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Misc        ", ImVec2( 392, 444 ), ImVec2( 346, 146 ), false, 1 ) );
            {
                auto& cfg = nem::hacks::g_anti_aim->cfg( );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_SLOW_WALK" ), "Slow Walk", CHILD_ELEMENT_POS( 0 ), &cfg.m_slow_walk ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_FAKE_DUCK" ), "Fake Duck", CHILD_ELEMENT_POS( 1 ), &cfg.m_fake_duck ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 1 ) );


            // Players
            ADD_ELEMENT( C_Element( ).ChildBegin( "ESP   ", ImVec2( 28, 116 ), ImVec2( 352, 521 ), false, 3 ) );
            {
                auto& cfg = nem::hacks::g_visuals->cfg( );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_NAME" ), "Name", CHILD_ELEMENT_POS( 0 ), &cfg.m_player.m_name ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_HEALTH" ), "Health", CHILD_ELEMENT_POS( 1 ), &cfg.m_player.m_health_bar ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_BOX" ), "Box", CHILD_ELEMENT_POS( 2 ), &cfg.m_player.m_bbox ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_FLAGS" ), "Flags", CHILD_ELEMENT_POS( 3 ), &cfg.m_player.m_flags ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_HIT_MARKER" ), "Hit Marker", CHILD_ELEMENT_POS( 4 ), &cfg.m_player.m_hit_marker ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_WEAPON_ICON" ), "Weapon Icon", CHILD_ELEMENT_POS( 5 ), &cfg.m_player.m_wpn_icon ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_WEAPON_TEXT" ), "Weapon Text", CHILD_ELEMENT_POS( 6 ), &cfg.m_player.m_wpn_text ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_AMMO" ), "Ammo", CHILD_ELEMENT_POS( 7 ), &cfg.m_player.m_ammo ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_SKELETON" ), "Skeleton", CHILD_ELEMENT_POS( 8 ), &cfg.m_player.m_skeleton ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_OOF_ARROW" ), "OOF Arrow", CHILD_ELEMENT_POS( 9 ), &cfg.m_player.m_oof_arrow ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 263.f, CHILD_ELEMENT_POS( 10 ), cfg.m_player.m_glow_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_GLOW" ), "Glow", CHILD_ELEMENT_POS( 10 ), &cfg.m_player.m_glow ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 3 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Chams     ", ImVec2( 392, 116 ), ImVec2( 346, 688 ), false, 3 ) );
            {
                auto& cfg = nem::hacks::g_visuals->cfg( );
                
                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 0 ), cfg.m_clred_mdls.m_player_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_PLAYER_CHAMS" ), "Player Chams", CHILD_ELEMENT_POS( 0 ), &cfg.m_clred_mdls.m_player ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 1 ), cfg.m_clred_mdls.m_player_behind_wall_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_INV_PLAYER_CHAMS" ), "Invisible Player Chams", CHILD_ELEMENT_POS( 1 ), &cfg.m_clred_mdls.m_player_behind_wall ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_CHAMS_TYPE" ), "Chams Type", CHILD_ELEMENT_POS( 2 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & cfg.m_clred_mdls.m_player_mdl_type ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 3 ), cfg.m_clred_mdls.m_player_glow_outline_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_PLAYER_GLOW_OUTLINE" ), "Glow Outline", CHILD_ELEMENT_POS( 3 ), &cfg.m_clred_mdls.m_glow_outline_player ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 4 ), cfg.m_clred_mdls.m_player_glow_outline_invisible_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_INV_PLAYER_GLOW_OUTLINE" ), "Invisible Glow Outline", CHILD_ELEMENT_POS( 4 ), &cfg.m_clred_mdls.m_glow_outline_player_behind_wall ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 5 ), cfg.m_clred_mdls.m_shadow_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_PLAYER_HISTORY" ), "Player History", CHILD_ELEMENT_POS( 5 ), &cfg.m_clred_mdls.m_shadow ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_HISTORY_CHAMS_TYPE" ), "History Chams Type", CHILD_ELEMENT_POS( 6 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & cfg.m_clred_mdls.m_shadow_mdl_type ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 7 ), cfg.m_clred_mdls.m_local_player_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_LOCAL_PLAYER" ), "Local Chams", CHILD_ELEMENT_POS( 7 ), &cfg.m_clred_mdls.m_local_player ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_LOCAL_CHAMS_TYPE" ), "Local Chams Type", CHILD_ELEMENT_POS( 8 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & cfg.m_clred_mdls.m_local_player_mdl_type ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 9 ), cfg.m_clred_mdls.m_local_player_glow_outline_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_LOCAL_GLOW_OUTLINE" ), "Local Glow Outline", CHILD_ELEMENT_POS( 9 ), &cfg.m_clred_mdls.m_glow_outline_m_local_player ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 10 ), cfg.m_clred_mdls.m_local_player_fake_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_LOCAL_FAKE" ), "Local Fake", CHILD_ELEMENT_POS( 10 ), &cfg.m_clred_mdls.m_local_player_fake ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_LOCAL_FAKE_CHAMS_TYPE" ), "Local Fake Type", CHILD_ELEMENT_POS( 11 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & cfg.m_clred_mdls.m_local_player_fake_mdl_type ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 12 ), cfg.m_clred_mdls.m_local_player_glow_outline_fake_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_LOCAL_FAKE_GLOW_OUTLINE" ), "Local Fake Glow Outline", CHILD_ELEMENT_POS( 12 ), &cfg.m_clred_mdls.m_glow_outline_local_player_fake ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 13 ), cfg.m_clred_mdls.m_ragdoll_mdl_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_RAGDOLLS" ), "Ragdolls", CHILD_ELEMENT_POS( 13 ), &cfg.m_clred_mdls.m_ragdoll ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_RAGDOLLS_TYPE" ), "Ragdolls Type", CHILD_ELEMENT_POS( 14 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & cfg.m_clred_mdls.m_ragdoll_mdl_type ) );

            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 3 ) );

            // Extra

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main 1     ", ImVec2( 28, 116 ), ImVec2( 352, 318 ), false, 4 ) );
            {
                auto& cfg = nem::hacks::g_visuals->cfg( );

                ADD_ELEMENT( C_Element( ).ColorPicker( 263.f, CHILD_ELEMENT_POS( 0 ), cfg.m_clred_mdls.m_hands_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_HAND_CHAMS" ), "Hands Chams", CHILD_ELEMENT_POS( 0 ), &cfg.m_clred_mdls.m_hands ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_HANDLE_CHAMS_TYPE" ), "Hands Chams Type", CHILD_ELEMENT_POS( 1 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & cfg.m_clred_mdls.m_hands_mdl_type ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 263.f, CHILD_ELEMENT_POS( 2 ), cfg.m_clred_mdls.m_hands_outline_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_HAND_GLOW_OUTLINE_CHAMS" ), "Hands Glow Outline Chams", CHILD_ELEMENT_POS( 2 ), &cfg.m_clred_mdls.m_glow_outline_hands ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 263.f, CHILD_ELEMENT_POS( 3 ), cfg.m_clred_mdls.m_wpn_view_model_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_LOCAL_WEAPON_CHAMS" ), "Local Weapon Chams", CHILD_ELEMENT_POS( 3 ), &cfg.m_clred_mdls.m_wpn_view_model ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_LOCAL_WEAPON_CHAMS_TYPE" ), "Local Weapon Type", CHILD_ELEMENT_POS( 4 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & cfg.m_clred_mdls.m_wpn_view_model_mdl_type ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 263.f, CHILD_ELEMENT_POS( 5 ), cfg.m_clred_mdls.m_weapon_local_outline_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_LOCAL_WEAPON_GLOW_OUTLINE" ), "Local Weapon Glow Outline", CHILD_ELEMENT_POS( 5 ), &cfg.m_clred_mdls.m_glow_outline_wpn_view_model ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 4 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main 2        ", ImVec2( 392, 116 ), ImVec2( 346, 318 ), false, 4 ) );
            {
                auto& cfg = nem::hacks::g_visuals->cfg( );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 0 ), cfg.m_clred_mdls.m_weapons_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_WEAPON_CHAMS" ), "Weapon Chams", CHILD_ELEMENT_POS( 0 ), &cfg.m_clred_mdls.m_weapons ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_WEAPON_CHAMS_TYPE" ), "Weapon Chams Type", CHILD_ELEMENT_POS( 1 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & cfg.m_clred_mdls.m_weapons_mdl_type ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 2 ), cfg.m_clred_mdls.m_weapon_outline_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_WEAPON_GLOW_OUTLINE" ), "Weapon Glow Outline", CHILD_ELEMENT_POS( 2 ), &cfg.m_clred_mdls.m_glow_outline_weapons ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 3 ), cfg.m_clred_mdls.m_on_shot_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_SHOT_RECORD" ), "Shot Record", CHILD_ELEMENT_POS( 3 ), &cfg.m_clred_mdls.m_on_shot ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_SHOT_RECORD_TYPE" ), "Shot Record Type", CHILD_ELEMENT_POS( 4 ), { "Regular", "Flat", "Glow", "Metallic", "Overlay" }, & cfg.m_clred_mdls.m_on_shot_mdl_type ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 5 ), cfg.m_clred_mdls.m_shot_record_glow_outline_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_SHOT_RECORD_GLOW_OUTLINE" ), "Shot Record Glow Outline", CHILD_ELEMENT_POS( 5 ), &cfg.m_clred_mdls.m_glow_outline_on_shot ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 4 ) );


            // Grenades

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main           ", ImVec2( 28, 116 ), ImVec2( 352, 192 ), false, 5 ) );
            {
                auto& cfg = nem::hacks::g_visuals->cfg( );
                
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_GRENADES" ), "Grenades", CHILD_ELEMENT_POS( 0 ), &cfg.m_other.m_grenades ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_GRENADE_PREDICTION" ), "Grenade Prediction", CHILD_ELEMENT_POS( 1 ), &cfg.m_other.m_grenade_trajectory ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_GRENADE_PROXIMITY_WARNING" ), "Grenade Proximity Warning", CHILD_ELEMENT_POS( 2 ), &cfg.m_other.m_grenade_warning ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 5 ) );


            // World

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main                 ", ImVec2( 28, 116 ), ImVec2( 352, 643 ), false, 6 ) );
            {
                auto& cfg = nem::hacks::g_visuals->cfg( );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_DROPPED_WEAPON" ), "Dropped Weapon", CHILD_ELEMENT_POS( 0 ), { "None", "Name", "Icon" }, & cfg.m_other.m_dropped_wpns ) );
                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_WORLD_TYPE" ), "World Type", CHILD_ELEMENT_POS( 1 ), { "None", "Full Bright", "Nightmode" }, & cfg.m_effects.m_brightness_adjustment ) );

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
                     ( "Rainy" ) }, & cfg.m_effects.m_sky_box_type ) );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_SHADOWS_MOD" ), "Shadows Modulation", CHILD_ELEMENT_POS( 3 ), &cfg.m_other.m_change_shadows ) );

                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_SHADOWS_X" ), "X Direction", CHILD_ELEMENT_POS( 4 ), -100, 100, &cfg.m_other.m_light_x ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_SHADOWS_Y" ), "Y Direction", CHILD_ELEMENT_POS( 5 ), -100, 100, &cfg.m_other.m_light_y ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_SHADOWS_Z" ), "Z Direction", CHILD_ELEMENT_POS( 6 ), -100, 100, &cfg.m_other.m_light_z ) );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_TONE_MAP" ), "Tone Map Modulation", CHILD_ELEMENT_POS( 7 ), &cfg.m_other.m_tone_map_modulation ) );

                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_BLOOM" ), "Bloom", CHILD_ELEMENT_POS( 8 ), 0, 750, &cfg.m_other.m_world_bloom ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_EXPOSURE" ), "Exposure", CHILD_ELEMENT_POS( 9 ), 0, 2000, &cfg.m_other.m_world_exposure ) );

                ADD_ELEMENT( C_Element( ).ColorPicker( 258.f, CHILD_ELEMENT_POS( 10 ), cfg.m_effects.m_fog_clr ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_FOG" ), "Fog", CHILD_ELEMENT_POS( 10 ), &cfg.m_effects.m_force_fog ) );

                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_FOG_START" ), "Fog Start", CHILD_ELEMENT_POS( 11 ), 0, 1000, &cfg.m_effects.m_start_dist ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_FOG_END" ), "Fog End", CHILD_ELEMENT_POS( 12 ), 100, 1100, &cfg.m_effects.m_end_dist ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_FOG_DENSITY" ), "Density", CHILD_ELEMENT_POS( 13 ), 0, 1000, &cfg.m_effects.m_density ) );

            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 6 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Additional  ", ImVec2( 392, 116 ), ImVec2( 346, 643 ), false, 6 ) );
            {
                auto& cfg = nem::hacks::g_visuals->cfg( );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_BULLET_TRACERS" ), "Bullet Tracers", CHILD_ELEMENT_POS( 0 ), &cfg.m_effects.m_bullet_tracers ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_BULLET_IMPACTS" ), "Bullet Impacts", CHILD_ELEMENT_POS( 1 ), &cfg.m_effects.m_bullet_impacts ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_DAMAGE_MARKER" ), "Damage Marker", CHILD_ELEMENT_POS( 2 ), &cfg.m_effects.m_damage_markers ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_VISUAL_RECOIL" ), "Remove Visual Recoil", CHILD_ELEMENT_POS( 3 ), &cfg.m_effects.m_recoil_adjustment ) );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_FLASH" ), "Remove Flash", CHILD_ELEMENT_POS( 4 ), &cfg.m_effects.m_remove_flash ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_SMOKE" ), "Remove Smoke", CHILD_ELEMENT_POS( 5 ), &cfg.m_effects.m_remove_smoke ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_FOG" ), "Remove Fog", CHILD_ELEMENT_POS( 6 ), &cfg.m_effects.m_remove_fog ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_DISABLE_POST_PROCESSING" ), "Disable Post Processing", CHILD_ELEMENT_POS( 7 ), &cfg.m_effects.m_disable_post_processing ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_SCOPE" ), "Remove Scope", CHILD_ELEMENT_POS( 8 ), &cfg.m_effects.m_remove_scope_overlay ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_SHADOWS" ), "Remove Shadows", CHILD_ELEMENT_POS( 9 ), &cfg.m_effects.m_remove_shadows ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_LANDING_BOB" ), "Remove Landing Bob", CHILD_ELEMENT_POS( 10 ), &cfg.m_effects.m_remove_landing_bob ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_REMOVE_HANDS_SHAKING" ), "Remove Hands Shaking", CHILD_ELEMENT_POS( 11 ), &cfg.m_effects.m_remove_hand_shaking ) );

                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_PROP_TARNSPARENCY" ), "Props Transp.", CHILD_ELEMENT_POS( 12 ), 0, 100, &cfg.m_effects.m_transparent_props ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_WALL_TARNSPARENCY" ), "Walls Transp.", CHILD_ELEMENT_POS( 13 ), 0, 100, &cfg.m_effects.m_transparent_walls ) );

            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 6 ) );

            // View

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main                     ", ImVec2( 28, 116 ), ImVec2( 352, 356 ), false, 7 ) );
            {
                auto& cfg = nem::hacks::g_visuals->cfg( );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_THIRD_PERSON" ), "Third Person", CHILD_ELEMENT_POS( 0 ), &cfg.m_effects.m_thirdperson ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_THIRD_PERSON_DEAD" ), "Third Person While Dead", CHILD_ELEMENT_POS( 1 ), &cfg.m_effects.m_force_thirdperson_dead ) );

                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_TP_DISTANCE" ), "Third Person Dist.", CHILD_ELEMENT_POS( 2 ), 60, 180, &cfg.m_effects.m_third_person_distance ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_CMR_DISTANCE" ), "Camera Distance", CHILD_ELEMENT_POS( 3 ), 60, 140, &cfg.m_other.m_camera_distance ) );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_OV_IN_SCOPE" ), "Override In Scope", CHILD_ELEMENT_POS( 4 ), &cfg.m_other.m_remove_zoom_on_second_scope ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_ASP_RATIO" ), "Aspect Ratio", CHILD_ELEMENT_POS( 5 ), &cfg.m_other.m_aspect_ratio ) );
                ADD_ELEMENT( C_Element( ).Slider( _h( "SL_ASP_RATIO_VAL" ), "Aspect Ratio Value", CHILD_ELEMENT_POS( 6 ), 0, 30, &cfg.m_other.m_aspect_ratio_value ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 7 ) );

            // Misc

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main                          ", ImVec2( 28, 116 ), ImVec2( 352, 397 ), false, 8 ) );
            {
                auto& cfg = nem::hacks::g_movement->cfg( );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_BUNNY_HOP" ), "Bunny Hop", CHILD_ELEMENT_POS( 0 ), &cfg.m_auto_bhop ) );
                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_AUTO_STRAFE_TYPE" ), "Auto Strafe", CHILD_ELEMENT_POS( 1 ), { "None", "View Angles", "Directional" }, & cfg.m_auto_strafe_type ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_AUTO_PEEK" ), "Auto Peek", CHILD_ELEMENT_POS( 2 ), &cfg.m_auto_peek ) );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_INFINITY_DUCK" ), "Infinity Duck", CHILD_ELEMENT_POS( 3 ), &cfg.m_remove_crouch_cooldown ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_CLAN_TAB" ), "Clan Tag", CHILD_ELEMENT_POS( 4 ), &cfg.g_clan_tag ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_SLIDE_WALK" ), "Slide Walk", CHILD_ELEMENT_POS( 5 ), &cfg.m_slide_walk ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_FAST_STOP" ), "Fast Stop", CHILD_ELEMENT_POS( 6 ), &cfg.m_fast_stop ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_DISABLE_RAGDOLL_PHYSICS" ), "Disable Ragdoll Physics", CHILD_ELEMENT_POS( 7 ), &cfg.m_disable_ragdoll_phys ) );

            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 8 ) );

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main 2                             ", ImVec2( 392, 116 ), ImVec2( 346, 397 ), false, 8 ) );
            {
                auto& cfg = nem::hacks::g_visuals->cfg( );

                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_HITSOUND" ), "Hitsound", CHILD_ELEMENT_POS( 0 ), &cfg.m_player.m_hit_marker_sound ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_SPECTATORS" ), "Spectators", CHILD_ELEMENT_POS( 1 ), &cfg.m_other.m_spectators ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_KEYBINDS" ), "Keybinds", CHILD_ELEMENT_POS( 2 ), &cfg.m_other.m_key_binds ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_PRESERVE_KILLFEED" ), "Preserve Killfeed", CHILD_ELEMENT_POS( 3 ), &cfg.m_other.m_preserve_killfeed ) );
                ADD_ELEMENT( C_Element( ).Checkbox( _h( "CB_BUYBOT" ), "BuyBot", CHILD_ELEMENT_POS( 4 ), &cfg.m_clred_mdls.m_buy_bot ) );

                ADD_ELEMENT( C_Element( ).Combo( _h( "CMB_BUYBOT_SNIPERS" ), "Snipers", CHILD_ELEMENT_POS( 5 ), { "None", "Auto", "AWP", "SSG 08" }, & cfg.m_clred_mdls.m_buy_bot_1 ) );
                ADD_ELEMENT( C_Element( ).Combo( _h( "CMD_BUYBOT_PISTOLS" ), "Pistols", CHILD_ELEMENT_POS( 6 ), { "None", "Dual Berettas", "Deagle/R8" }, & cfg.m_clred_mdls.m_buy_bot_2 ) );

                ADD_ELEMENT( C_Element( ).MultiCombo( _h( "MCMB_BUYBOT_ADDITIONAL" ), "Additional", CHILD_ELEMENT_POS( 7 ), { "Grenades", "Armor", "Taser", "Defuser" }, & cfg.m_buy_bot_stuff.m_buy_bot_stuff ) );
            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 8 ) );

            // Configs

            ADD_ELEMENT( C_Element( ).ChildBegin( "Main                                   ", ImVec2( 28, 116 ), ImVec2( 352, 731 ), false, 11 ) );
            {
                m_iConfigsElementIndex = m_cVecElemets.size( );
                ADD_ELEMENT( C_Element( ).Combo( 0, "Configs", CHILD_ELEMENT_POS( 0 ), { }, & g_configs->m_choosed_item ) );

                ADD_ELEMENT( C_Element( ).TextInput( "##cn", CHILD_ELEMENT_POS( 1 ), ImVec2( 0.f, 0.f ), m_chConfigName ) );
                ADD_ELEMENT( C_Element( ).Button( "Create", CHILD_ELEMENT_POS( 2 ), ImVec2( 70.f, 30.f ), OnCreate ) );

                ADD_ELEMENT( C_Element( ).Button( "Delete", CHILD_ELEMENT_POS( 3 ), ImVec2( 70.f, 30.f ), OnDelete ) );
                ADD_ELEMENT( C_Element( ).Button( "Refresh", CHILD_ELEMENT_POS( 4 ), ImVec2( 70.f, 30.f ), OnRefresh ) );
                ADD_ELEMENT( C_Element( ).Button( "Save", CHILD_ELEMENT_POS( 5 ), ImVec2( 70.f, 30.f ), OnSave ) );
                ADD_ELEMENT( C_Element( ).Button( "Load", CHILD_ELEMENT_POS( 6 ), ImVec2( 70.f, 30.f ), OnLoad ) );
                ADD_ELEMENT( C_Element( ).Button( "Share", CHILD_ELEMENT_POS( 7 ), ImVec2( 70.f, 30.f ), OnShare ) );
                ADD_ELEMENT( C_Element( ).Button( "Import", CHILD_ELEMENT_POS( 8 ), ImVec2( 70.f, 30.f ), OnImport ) );

            }
            ADD_ELEMENT( C_Element( ).ChildEnd( false, 11 ) );

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
