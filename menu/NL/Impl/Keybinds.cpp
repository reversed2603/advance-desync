#include "..\Neverlose.h"

#include "..\Deps\KeysList\KeysList.h"

#define ADD_KEYBIND(hash, keybind) g_Keybinds->m_cMapKeybinds[hash] = keybind

void C_Keybinds::ApplyDefValues( )
{
    auto IsKeyActive = [ ]( int iVKCode, E_KeybindType eType ) -> bool
    {
        if ( eType == E_TOGGLE )
            return GetKeyState( iVKCode );
        else if ( eType == E_HOLD )
            return GetAsyncKeyState( iVKCode );

        return false;
    };

    for ( auto& cKeybindEntry : m_cMapKeybinds )
    {
        auto& cKeybind = cKeybindEntry.second;

        for ( auto& cElem : g_NL->m_cVecElemets )
        {
            if ( cElem.GetType( ) == ET_CHECKBOX && cElem.GetVar( _h( "HashIndex" ) ).GetUInt32( ) == cKeybindEntry.first )
            {
                bool* pValue = cElem.GetVar( _h( "ValuePointer" ) ).GetRef<bool>( );
                bool bIsAnyActive = false;
                int iActiveIndex = 0;

                for ( int i = 0; i < cKeybind.m_cVecKeys.size( ); ++i )
                {
                    auto& cKeyInfo = cKeybind.m_cVecKeys[i];
                    if ( IsKeyActive( cKeyInfo.m_iKeyValue, cKeyInfo.m_eKeyType ) )
                    {
                        bIsAnyActive = true;
                        iActiveIndex = i;
                        break;
                    }
                }

                if ( cKeybind.m_bOverriding )
                {
                    *pValue = cKeybind.m_bBackupCheckbox;
                }
            }
            else if ( cElem.GetType( ) == ET_SLIDER && cElem.GetVar( _h( "HashIndex" ) ).GetUInt32( ) == cKeybindEntry.first )
            {
                int* pValue = cElem.GetVar( _h( "ValuePointer" ) ).GetRef<int>( );
                bool bIsAnyActive = false;
                int iActiveIndex = 0;

                for ( int i = 0; i < cKeybind.m_cVecKeys.size( ); ++i )
                {
                    auto& cKeyInfo = cKeybind.m_cVecKeys[i];
                    if ( IsKeyActive( cKeyInfo.m_iKeyValue, cKeyInfo.m_eKeyType ) )
                    {
                        bIsAnyActive = true;
                        iActiveIndex = i;
                        break;
                    }
                }

                if ( cKeybind.m_bOverriding )
                {
                    *pValue = cKeybind.m_iBackupSlider;
                }
            }
            else if ( cElem.GetType( ) == ET_COMBO && cElem.GetVar( _h( "HashIndex" ) ).GetUInt32( ) == cKeybindEntry.first )
            {
                int* pValue = cElem.GetVar( _h( "ValuePointer" ) ).GetRef<int>( );
                bool bIsAnyActive = false;
                int iActiveIndex = 0;

                for ( int i = 0; i < cKeybind.m_cVecKeys.size( ); ++i )
                {
                    auto& cKeyInfo = cKeybind.m_cVecKeys[i];
                    if ( IsKeyActive( cKeyInfo.m_iKeyValue, cKeyInfo.m_eKeyType ) )
                    {
                        bIsAnyActive = true;
                        iActiveIndex = i;
                        break;
                    }
                }

                if ( cKeybind.m_bOverriding )
                {
                    *pValue = cKeybind.m_iBackupCombo;
                }
            }
            else if ( cElem.GetType( ) == ET_MULTI_COMBO && cElem.GetVar( _h( "HashIndex" ) ).GetUInt32( ) == cKeybindEntry.first )
            {
                int* pValue = cElem.GetVar( _h( "ValuePointer" ) ).GetRef<int>( );
                bool bIsAnyActive = false;
                int iActiveIndex = 0;

                for ( int i = 0; i < cKeybind.m_cVecKeys.size( ); ++i )
                {
                    auto& cKeyInfo = cKeybind.m_cVecKeys[i];
                    if ( IsKeyActive( cKeyInfo.m_iKeyValue, cKeyInfo.m_eKeyType ) )
                    {
                        bIsAnyActive = true;
                        iActiveIndex = i;
                        break;
                    }
                }

                if ( cKeybind.m_bOverriding )
                {
                    *pValue = cKeybind.m_iBackupMultiCombo;
                }
            }
        }
    }
}


void C_Keybinds::InitializeKeybindVars( )
{
    CALL_ONCE_START;
    {
        // Ragebot
        ADD_KEYBIND( _h( "CB_ENABLE_RAGEBOT" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_REMOVE_RECOIL" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_AUTO_SCOPE" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_MULTI_THREAD" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CMB_CURRENT_WEAPON" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "CB_ENABLE_DT" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_ENABLE_HS" ), C_KeybindInfo( ET_CHECKBOX ) );

        // // // 

        ADD_KEYBIND( _h( "SL_MIN_DAMAGE_PISTOLS" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_HITCHANCE_PISTOLS" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "CB_STATIC_PS_PISTOLS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "SL_HEAD_SCALE_PISTOLS" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_BODY_SCALE_PISTOLS" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "CB_FORCE_SP" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "MCMB_HITBOXES_PISTOLS" ), C_KeybindInfo( ET_MULTI_COMBO ) );
        ADD_KEYBIND( _h( "MCMB_MULTIPOINTS_PISTOLS" ), C_KeybindInfo( ET_MULTI_COMBO ) );

        ADD_KEYBIND( _h( "SL_MIN_DAMAGE_DEAGLER8" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_HITCHANCE_DEAGLER8" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "CB_STATIC_PS_DEAGLER8" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "SL_HEAD_SCALE_DEAGLER8" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_BODY_SCALE_DEAGLER8" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "MCMB_HITBOXES_DEAGLER8" ), C_KeybindInfo( ET_MULTI_COMBO ) );
        ADD_KEYBIND( _h( "MCMB_MULTIPOINTS_DEAGLER8" ), C_KeybindInfo( ET_MULTI_COMBO ) );

        ADD_KEYBIND( _h( "SL_MIN_DAMAGE_RIFLES" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_HITCHANCE_RIFLES" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "CB_STATIC_PS_RIFLES" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "SL_HEAD_SCALE_RIFLES" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_BODY_SCALE_RIFLES" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "MCMB_HITBOXES_RIFLES" ), C_KeybindInfo( ET_MULTI_COMBO ) );
        ADD_KEYBIND( _h( "MCMB_MULTIPOINTS_RIFLES" ), C_KeybindInfo( ET_MULTI_COMBO ) );

        ADD_KEYBIND( _h( "SL_MIN_DAMAGE_SCAR" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_HITCHANCE_SCAR" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "CB_STATIC_PS_SCAR" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "SL_HEAD_SCALE_SCAR" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_BODY_SCALE_SCAR" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "MCMB_HITBOXES_SCAR" ), C_KeybindInfo( ET_MULTI_COMBO ) );
        ADD_KEYBIND( _h( "MCMB_MULTIPOINTS_SCAR" ), C_KeybindInfo( ET_MULTI_COMBO ) );

        ADD_KEYBIND( _h( "SL_MIN_DAMAGE_SSG08" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_HITCHANCE_SSG08" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "CB_STATIC_PS_SSG08" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "SL_HEAD_SCALE_SSG08" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_BODY_SCALE_SSG08" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "MCMB_HITBOXES_SSG08" ), C_KeybindInfo( ET_MULTI_COMBO ) );
        ADD_KEYBIND( _h( "MCMB_MULTIPOINTS_SSG08" ), C_KeybindInfo( ET_MULTI_COMBO ) );

        ADD_KEYBIND( _h( "SL_MIN_DAMAGE_AWP" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_HITCHANCE_AWP" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "CB_STATIC_PS_AWP" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "SL_HEAD_SCALE_AWP" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_BODY_SCALE_AWP" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "MCMB_HITBOXES_AWP" ), C_KeybindInfo( ET_MULTI_COMBO ) );
        ADD_KEYBIND( _h( "MCMB_MULTIPOINTS_AWP" ), C_KeybindInfo( ET_MULTI_COMBO ) );

        // // // 

        // Anti-Aims
        ADD_KEYBIND( _h( "CB_ANTI_AIM" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CMB_PITCH" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "SL_YAW" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "CB_JITTER_SIDE" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_AT_TARGET" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "SL_JITTER_AMT" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "CB_DESYNC" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_DESYNC_INVERT" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "SL_DESYNC_DELTA" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "CB_ROLL_LEAN" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "SL_ROLL_DELTA" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "CB_AUTO_DIR" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "MCMB_FL_CONDITIONS" ), C_KeybindInfo( ET_MULTI_COMBO ) );
        ADD_KEYBIND( _h( "CB_FAKE_LAG" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "SL_FAKE_LAG_LIMIT" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "CB_SLOW_WALK" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_FAKE_DUCK" ), C_KeybindInfo( ET_CHECKBOX ) );

        // Players
        ADD_KEYBIND( _h( "CB_NAME" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_HEALTH" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_BOX" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_FLAGS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_HIT_MARKER" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_WEAPON_ICON" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_WEAPON_TEXT" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_AMMO" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_SKELETON" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_OOF_ARROW" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_GLOW" ), C_KeybindInfo( ET_CHECKBOX ) );

        ADD_KEYBIND( _h( "CB_PLAYER_CHAMS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_INV_PLAYER_CHAMS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CMB_CHAMS_TYPE" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "CB_PLAYER_GLOW_OUTLINE" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_INV_PLAYER_GLOW_OUTLINE" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_PLAYER_HISTORY" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CMB_HISTORY_CHAMS_TYPE" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "CB_LOCAL_PLAYER" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CMB_LOCAL_CHAMS_TYPE" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "CB_LOCAL_GLOW_OUTLINE" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_LOCAL_FAKE" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CMB_LOCAL_FAKE_CHAMS_TYPE" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "CB_LOCAL_FAKE_GLOW_OUTLINE" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_HAND_CHAMS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CMB_HANDLE_CHAMS_TYPE" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "CB_HAND_GLOW_OUTLINE_CHAMS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_LOCAL_WEAPON_CHAMS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CMB_LOCAL_WEAPON_CHAMS_TYPE" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "CB_LOCAL_WEAPON_GLOW_OUTLINE" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_WEAPON_CHAMS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_WEAPON_GLOW_OUTLINE" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_SHOT_RECORD" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CMB_SHOT_RECORD_TYPE" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "CB_SHOT_RECORD_GLOW_OUTLINE" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_RAGDOLLS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CMB_RAGDOLLS_TYPE" ), C_KeybindInfo( ET_COMBO ) );

        // Grenades
        ADD_KEYBIND( _h( "CB_GRENADES" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_GRENADE_PREDICTION" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_GRENADE_PROXIMITY_WARNING" ), C_KeybindInfo( ET_CHECKBOX ) );

        // World
        ADD_KEYBIND( _h( "CMB_DROPPED_WEAPON" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "CMB_WORLD_TYPE" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "CMB_SKYBOX_TYPE" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "CB_SHADOWS_MOD" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "SL_SHADOWS_X" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_SHADOWS_Y" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_SHADOWS_Z" ), C_KeybindInfo( ET_SLIDER ) );

        ADD_KEYBIND( _h( "CB_TONE_MAP" ), C_KeybindInfo( ET_CHECKBOX ) );

        ADD_KEYBIND( _h( "SL_BLOOM" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_EXPOSURE" ), C_KeybindInfo( ET_SLIDER ) );

        ADD_KEYBIND( _h( "CB_FOG" ), C_KeybindInfo( ET_CHECKBOX ) );

        ADD_KEYBIND( _h( "SL_FOG_START" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_FOG_END" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_FOG_DENSITY" ), C_KeybindInfo( ET_SLIDER ) );

        ADD_KEYBIND( _h( "CB_BULLET_TRACERS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_BULLET_IMPACTS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_DAMAGE_MARKER" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_REMOVE_VISUAL_RECOIL" ), C_KeybindInfo( ET_CHECKBOX ) );

        ADD_KEYBIND( _h( "CB_REMOVE_FLASH" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_REMOVE_SMOKE" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_REMOVE_FOG" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_DISABLE_POST_PROCESSING" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_REMOVE_SCOPE" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_REMOVE_SHADOWS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_REMOVE_LANDING_BOB" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_REMOVE_HANDS_SHAKING" ), C_KeybindInfo( ET_CHECKBOX ) );

        ADD_KEYBIND( _h( "SL_PROP_TARNSPARENCY" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_WALL_TARNSPARENCY" ), C_KeybindInfo( ET_SLIDER ) );

        // View
        ADD_KEYBIND( _h( "CB_THIRD_PERSON" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_THIRD_PERSON_DEAD" ), C_KeybindInfo( ET_CHECKBOX ) );

        ADD_KEYBIND( _h( "SL_TP_DISTANCE" ), C_KeybindInfo( ET_SLIDER ) );
        ADD_KEYBIND( _h( "SL_CMR_DISTANCE" ), C_KeybindInfo( ET_SLIDER ) );

        ADD_KEYBIND( _h( "CB_OV_IN_SCOPE" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_ASP_RATIO" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "SL_ASP_RATIO_VAL" ), C_KeybindInfo( ET_SLIDER ) );

        // Misc
        ADD_KEYBIND( _h( "CB_BUNNY_HOP" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CMB_AUTO_STRAFE_TYPE" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "CB_AUTO_PEEK" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_INFINITY_DUCK" ), C_KeybindInfo( ET_CHECKBOX ) );

        ADD_KEYBIND( _h( "CB_CLAN_TAB" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_SLIDE_WALK" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_FAST_STOP" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_DISABLE_RAGDOLL_PHYSICS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_HITSOUND" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_SPECTATORS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_KEYBINDS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_PRESERVE_KILLFEED" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "CB_BUYBOT" ), C_KeybindInfo( ET_CHECKBOX ) );

        ADD_KEYBIND( _h( "CMB_BUYBOT_SNIPERS" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "CMD_BUYBOT_PISTOLS" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "MCMB_BUYBOT_ADDITIONAL" ), C_KeybindInfo( ET_MULTI_COMBO ) );
        ADD_KEYBIND( _h( "NIGGAS_LOVE_NIGGASSS" ), C_KeybindInfo( ET_COMBO ) );
        ADD_KEYBIND( _h( "CMD_CONFIGS" ), C_KeybindInfo( ET_COMBO ) );

        ADD_KEYBIND( _h( "CB_BLEND" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND( _h( "SL_BLEND_VAL" ), C_KeybindInfo( ET_SLIDER ) );

        ADD_KEYBIND( _h( "MCMG_T_AGENTS" ), C_KeybindInfo( ET_MULTI_COMBO ) );
        ADD_KEYBIND( _h( "MCMG_CT_AGENTS" ), C_KeybindInfo( ET_MULTI_COMBO ) );

        ADD_KEYBIND( _h( "CB_RAIN_PENIS" ), C_KeybindInfo( ET_CHECKBOX ) );
        ADD_KEYBIND ( _h ( "CB_MOTION_PENIS" ), C_KeybindInfo ( ET_CHECKBOX ) );
        ADD_KEYBIND ( _h ( "CB_C4_NIGGA" ), C_KeybindInfo ( ET_CHECKBOX ) );
    }
    CALL_ONCE_END;
}

void C_Keybinds::GetAllBinds( nlohmann::json& jsonIn )
{
    auto& json = jsonIn["binds"];

    for ( auto& cKeybind : m_cMapKeybinds )
    {
        auto& cVecKeys = cKeybind.second.m_cVecKeys;

        auto& object = json[std::to_string( cKeybind.first )];

        object["size"] = cVecKeys.size( );

        for ( int i = 0; i < cVecKeys.size( ); ++i )
        {
            auto& object_key = object[std::to_string( i )];

            object_key = {
                cVecKeys[i].m_eKeyType,
                cVecKeys[i].m_iKeyValue,
                cVecKeys[i].m_bShowInKeyBinds,

                cVecKeys[i].m_bNewValueCheckbox,
                cVecKeys[i].m_iNewValueSlider,
                cVecKeys[i].m_iNewValueCombo,
                cVecKeys[i].m_iNewValueMultiCombo,
            };
        }
    }
}

void C_Keybinds::LoadAllBinds( nlohmann::json& jsonIn )
{
    auto& json = jsonIn["binds"];

    for ( auto& cKeybind : m_cMapKeybinds )
    {
        auto& cVecKeys = cKeybind.second.m_cVecKeys;
        auto& object = json[std::to_string( cKeybind.first )];

        if ( !object.size() )
            continue;

        int iSize = object["size"].get<int>( );

        cVecKeys.clear( );

        for ( int i = 0; i < iSize; ++i )
        {
            cVecKeys.push_back( C_KeybindKeyInfo( ) );

            auto& object_key = object[std::to_string( i )];

            cVecKeys[i].m_eKeyType = ( E_KeybindType ) object_key.at( 0 ).get<int>( );
            cVecKeys[i].m_iKeyValue = object_key.at( 1 ).get<int>( );
            cVecKeys[i].m_bShowInKeyBinds = object_key.at( 2 ).get<bool>( );

            cVecKeys[i].m_bNewValueCheckbox = object_key.at( 3 ).get<bool>( );
            cVecKeys[i].m_iNewValueSlider = object_key.at( 4 ).get<int>( );
            cVecKeys[i].m_iNewValueCombo = object_key.at( 5 ).get<int>( );
            cVecKeys[i].m_iNewValueMultiCombo = object_key.at( 6 ).get<int>( );
        }
    }
}

void C_Keybinds::SetAllKeybinds( std::vector<C_Element>& cVecElements, bool bReset )
{
    auto IsKeyActive = [ ]( int iVKCode, E_KeybindType eType ) -> bool
    {
        if ( eType == E_TOGGLE )
            return GetKeyState( iVKCode );
        else if ( eType == E_HOLD )
            return GetAsyncKeyState( iVKCode );

        return false;
    };

    if ( bReset )
    {
        m_cVecKeybindsToShow.clear( );
        m_cVecKeybindsToShow.shrink_to_fit( );

        m_cVecKeybindsStates.clear( );
        m_cVecKeybindsStates.shrink_to_fit( );
    }
    
    for ( auto& cKeybindEntry : m_cMapKeybinds )
    {
        auto& cKeybind = cKeybindEntry.second;

        for ( auto& cElem : cVecElements )
        {
            if ( cElem.GetType( ) == ET_CHECKBOX && cElem.GetVar( _h( "HashIndex" ) ).GetUInt32( ) == cKeybindEntry.first )
            {
                bool* pValue = cElem.GetVar( _h( "ValuePointer" ) ).GetRef<bool>( );
                bool bIsAnyActive = false;
                int iActiveIndex = 0;

                for ( int i = 0; i < cKeybind.m_cVecKeys.size(); ++i )
                {
                    auto& cKeyInfo = cKeybind.m_cVecKeys[i];
                    if ( IsKeyActive( cKeyInfo.m_iKeyValue, cKeyInfo.m_eKeyType ) )
                    {
                        bIsAnyActive = true;
                        iActiveIndex = i;
                        break;
                    }
                }

                if ( !bIsAnyActive && cKeybind.m_bOverriding )
                {
                    cKeybind.m_bOverriding = false;
                    *pValue = cKeybind.m_bBackupCheckbox;
                }
                else if ( bIsAnyActive && !cKeybind.m_bOverriding )
                {
                    cKeybind.m_bOverriding = true;
                    cKeybind.m_bBackupCheckbox = *pValue;
                    *pValue = cKeybind.m_cVecKeys[iActiveIndex].m_bNewValueCheckbox;
                }

                if ( cKeybind.m_bOverriding && cKeybind.m_cVecKeys[iActiveIndex].m_bShowInKeyBinds )
                {
                    m_cVecKeybindsToShow.push_back( cElem.GetVar( _h( "Text" ) ).GetString( ) );
                    m_cVecKeybindsStates.push_back( *pValue ? "On" : "Off" );
                }
            }
            else if ( cElem.GetType( ) == ET_SLIDER && cElem.GetVar( _h( "HashIndex" ) ).GetUInt32( ) == cKeybindEntry.first )
            {
                int* pValue = cElem.GetVar( _h( "ValuePointer" ) ).GetRef<int>( );
                bool bIsAnyActive = false;
                int iActiveIndex = 0;

                for ( int i = 0; i < cKeybind.m_cVecKeys.size( ); ++i )
                {
                    auto& cKeyInfo = cKeybind.m_cVecKeys[i];
                    if ( IsKeyActive( cKeyInfo.m_iKeyValue, cKeyInfo.m_eKeyType ) )
                    {
                        bIsAnyActive = true;
                        iActiveIndex = i;
                        break;
                    }
                }

                if ( !bIsAnyActive && cKeybind.m_bOverriding )
                {
                    cKeybind.m_bOverriding = false;
                    *pValue = cKeybind.m_iBackupSlider;

                    cElem.ResetSlider( false );
                }
                else if ( bIsAnyActive && !cKeybind.m_bOverriding )
                {
                    cKeybind.m_bOverriding = true;
                    cKeybind.m_iBackupSlider = *pValue;
                    *pValue = cKeybind.m_cVecKeys[iActiveIndex].m_iNewValueSlider;

                    cElem.ResetSlider( false );
                }

                if ( cKeybind.m_bOverriding && cKeybind.m_cVecKeys[iActiveIndex].m_bShowInKeyBinds )
                {
                    m_cVecKeybindsToShow.push_back( cElem.GetVar( _h( "Text" ) ).GetString( ) );
                    m_cVecKeybindsStates.push_back( std::to_string( *pValue ) );
                }
            }
            else if ( cElem.GetType( ) == ET_COMBO && cElem.GetVar( _h( "HashIndex" ) ).GetUInt32( ) == cKeybindEntry.first )
            {
                int* pValue = cElem.GetVar( _h( "ValuePointer" ) ).GetRef<int>( );
                bool bIsAnyActive = false;
                int iActiveIndex = 0;

                for ( int i = 0; i < cKeybind.m_cVecKeys.size( ); ++i )
                {
                    auto& cKeyInfo = cKeybind.m_cVecKeys[i];
                    if ( IsKeyActive( cKeyInfo.m_iKeyValue, cKeyInfo.m_eKeyType ) )
                    {
                        bIsAnyActive = true;
                        iActiveIndex = i;
                        break;
                    }
                }

                if ( !bIsAnyActive && cKeybind.m_bOverriding )
                {
                    cKeybind.m_bOverriding = false;
                    *pValue = cKeybind.m_iBackupCombo;
                }
                else if ( bIsAnyActive && !cKeybind.m_bOverriding )
                {
                    cKeybind.m_bOverriding = true;
                    cKeybind.m_iBackupCombo = *pValue;
                    *pValue = cKeybind.m_cVecKeys[iActiveIndex].m_iNewValueCombo;
                }

                if ( cKeybind.m_bOverriding && cKeybind.m_cVecKeys[iActiveIndex].m_bShowInKeyBinds )
                {
                    m_cVecKeybindsToShow.push_back( cElem.GetVar( _h( "Text" ) ).GetString( ) );
                    m_cVecKeybindsStates.push_back( cElem.GetVar( _h( "Strings" ) ).GetRef<std::vector<std::string>>( )->at( *pValue ) );
                }
            }
            else if ( cElem.GetType( ) == ET_MULTI_COMBO && cElem.GetVar( _h( "HashIndex" ) ).GetUInt32( ) == cKeybindEntry.first )
            {
                int* pValue = cElem.GetVar( _h( "ValuePointer" ) ).GetRef<int>( );
                bool bIsAnyActive = false;
                int iActiveIndex = 0;

                for ( int i = 0; i < cKeybind.m_cVecKeys.size( ); ++i )
                {
                    auto& cKeyInfo = cKeybind.m_cVecKeys[i];
                    if ( IsKeyActive( cKeyInfo.m_iKeyValue, cKeyInfo.m_eKeyType ) )
                    {
                        bIsAnyActive = true;
                        iActiveIndex = i;
                        break;
                    }
                }

                if ( !bIsAnyActive && cKeybind.m_bOverriding )
                {
                    cKeybind.m_bOverriding = false;
                    *pValue = cKeybind.m_iBackupMultiCombo;
                }
                else if ( bIsAnyActive && !cKeybind.m_bOverriding )
                {
                    cKeybind.m_bOverriding = true;
                    cKeybind.m_iBackupMultiCombo = *pValue;
                    *pValue = cKeybind.m_cVecKeys[iActiveIndex].m_iNewValueMultiCombo;
                }

                if ( cKeybind.m_bOverriding && cKeybind.m_cVecKeys[iActiveIndex].m_bShowInKeyBinds )
                {
                    m_cVecKeybindsToShow.push_back( cElem.GetVar( _h( "Text" ) ).GetString( ) );
                    m_cVecKeybindsStates.push_back( "..." );
                }
            }
        }
    }
}

void C_Keybinds::CheckKeybind( )
{
    int& iClearFirst = m_cBindCombo.GetVar( _h( "ClearFirst" ) ).GetInt( );

    if ( iClearFirst )
    {
        m_pCurrentKeybind->m_cVecKeys[0] = C_KeybindKeyInfo( );
        m_iLastChoosenString = -1;

        iClearFirst = 0;
    }

    if ( m_iChoosenString != m_iLastChoosenString )
    {
        if ( m_iChoosenString >= m_pCurrentKeybind->m_cVecKeys.size( ) )
        {
            m_pCurrentKeybind->m_cVecKeys.push_back( C_KeybindKeyInfo( ) );
        }

        m_iKeyValue = m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_iKeyValue;
        m_iKeybindMode = m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_eKeyType;
        m_bShowInBinds = m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_bShowInKeyBinds;

        if ( m_pCurrentKeybind->m_eKeybindElementType == ET_CHECKBOX )
            m_bNewValueCheckbox = m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_bNewValueCheckbox;
        else if ( m_pCurrentKeybind->m_eKeybindElementType == ET_SLIDER )
        {
            m_iNewValueSlider = m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_iNewValueSlider;
            m_cBindNewValueSlider.ResetSlider( false );
        }
        else if ( m_pCurrentKeybind->m_eKeybindElementType == ET_COMBO )
            m_iNewValueCombo = m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_iNewValueCombo;
        else if ( m_pCurrentKeybind->m_eKeybindElementType == ET_MULTI_COMBO )
            m_iNewValueMultiCombo = m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_iNewValueMultiCombo;

        m_iLastChoosenString = m_iChoosenString;
    }

    if ( m_cVecTempStrings.size( ) < m_iLastSize )
    {
        m_iChoosenString = std::clamp( m_iChoosenString, 0, ( int ) ( m_cVecTempStrings.size( ) - 2 ) );

        m_pCurrentKeybind->m_cVecKeys.erase( m_pCurrentKeybind->m_cVecKeys.begin( ) + m_cBindCombo.GetVar( _h( "LastEraseIndex" ) ).GetInt( ) );

        m_iKeyValue = m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_iKeyValue;
        m_iKeybindMode = m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_eKeyType;
        m_bShowInBinds = m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_bShowInKeyBinds;

        if ( m_pCurrentKeybind->m_eKeybindElementType == ET_CHECKBOX )
            m_bNewValueCheckbox = m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_bNewValueCheckbox;
        else if ( m_pCurrentKeybind->m_eKeybindElementType == ET_SLIDER )
            m_iNewValueSlider = m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_iNewValueSlider;
        else if ( m_pCurrentKeybind->m_eKeybindElementType == ET_COMBO )
            m_iNewValueCombo = m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_iNewValueCombo;
        else if ( m_pCurrentKeybind->m_eKeybindElementType == ET_MULTI_COMBO )
            m_iNewValueMultiCombo = m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_iNewValueMultiCombo;
    }

    m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_iKeyValue = m_iKeyValue;
    m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_eKeyType = ( E_KeybindType ) m_iKeybindMode;
    m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_bShowInKeyBinds = m_bShowInBinds;

    if ( m_pCurrentKeybind->m_eKeybindElementType == ET_CHECKBOX )
        m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_bNewValueCheckbox = m_bNewValueCheckbox;
    else if ( m_pCurrentKeybind->m_eKeybindElementType == ET_SLIDER )
        m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_iNewValueSlider = m_iNewValueSlider;
    else if ( m_pCurrentKeybind->m_eKeybindElementType == ET_COMBO )
        m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_iNewValueCombo = m_iNewValueCombo;
    else if ( m_pCurrentKeybind->m_eKeybindElementType == ET_MULTI_COMBO )
        m_pCurrentKeybind->m_cVecKeys[m_iChoosenString].m_iNewValueMultiCombo = m_iNewValueMultiCombo;

    for ( int i = 0; i < m_pCurrentKeybind->m_cVecKeys.size( ); ++i )
    {
        if ( m_pCurrentKeybind->m_cVecKeys[i].m_iKeyValue )
            m_cVecTempStrings[i] = std::string( "Bind \"" ).append( szKeys[m_pCurrentKeybind->m_cVecKeys[i].m_iKeyValue] ).append( "\"" );
        else
            m_cVecTempStrings[i] = "Empty bind";
    }
    
    m_iLastSize = m_cVecTempStrings.size( );
}

void C_Keybinds::EnableKeybindSelection( C_Element* pElement, E_ElementType eElementType, uint32_t u32Hash )
{
    if ( !u32Hash )
        return;

    if ( m_bWindowState )
        return;

    InitializeElements( );

    m_iChoosenString = 0;
    m_iLastChoosenString = 0;
    m_pCurrentKeybind = &m_cMapKeybinds[u32Hash];

    m_pCurrentKeybind->m_eKeybindElementType = eElementType;

    m_iKeyValue = m_pCurrentKeybind->m_cVecKeys[0].m_iKeyValue;
    m_iKeybindMode = m_pCurrentKeybind->m_cVecKeys[0].m_eKeyType;
    m_bShowInBinds = m_pCurrentKeybind->m_cVecKeys[0].m_bShowInKeyBinds;

    if ( m_pCurrentKeybind->m_eKeybindElementType == ET_CHECKBOX )
        m_bNewValueCheckbox = m_pCurrentKeybind->m_cVecKeys[0].m_bNewValueCheckbox;
    else if ( m_pCurrentKeybind->m_eKeybindElementType == ET_SLIDER )
    {
        m_cBindNewValueSlider.ResetSlider( );

        m_iNewValueSlider = m_pCurrentKeybind->m_cVecKeys[0].m_iNewValueSlider;

        m_cBindNewValueSlider.GetVar( _h( "Min" ) ).GetInt( ) = pElement->GetVar( _h( "Min" ) ).GetInt( );
        m_cBindNewValueSlider.GetVar( _h( "Max" ) ).GetInt( ) = pElement->GetVar( _h( "Max" ) ).GetInt( );
    }
    else if ( m_pCurrentKeybind->m_eKeybindElementType == ET_COMBO )
    {
        m_iNewValueCombo = m_pCurrentKeybind->m_cVecKeys[0].m_iNewValueCombo;

        m_cBindNewValueCombo.GetVar( _h( "Strings" ) ).SetRef( pElement->GetVar( _h( "Strings" ) ).GetRef<void>( ) );
        m_cBindNewValueCombo.GetVar( _h( "Rebuild" ) ) = 1;
    }
    else if ( m_pCurrentKeybind->m_eKeybindElementType == ET_MULTI_COMBO )
    {
        m_iNewValueMultiCombo = m_pCurrentKeybind->m_cVecKeys[0].m_iNewValueMultiCombo;

        m_cBindNewValueMultiCombo.GetVar( _h( "Strings" ) ).SetRef( pElement->GetVar( _h( "Strings" ) ).GetRef<void>( ) );
        m_cBindNewValueMultiCombo.GetVar( _h( "Rebuild" ) ) = 1;
    }

    m_cVecTempStrings.clear( );

    for ( int i = 0; i < m_pCurrentKeybind->m_cVecKeys.size( ); ++i )
    {
        m_cVecTempStrings.push_back( "Empty bind" );

        if ( m_pCurrentKeybind->m_cVecKeys[i].m_iKeyValue )
            m_cVecTempStrings[i] = std::string( "Bind \"" ).append( szKeys[m_pCurrentKeybind->m_cVecKeys[i].m_iKeyValue] ).append( "\"" );
    }

    m_cVecTempStrings.push_back( "New bind" );

    m_cBindCombo.GetVar( _h( "Rebuild" ) ).GetInt( ) = 1;

    m_iLastSize = m_cVecTempStrings.size( );

    m_iKeybindWindowPosX = ImGui::GetMousePos( ).x;
    m_iKeybindWindowPosY = ImGui::GetMousePos( ).y;

    m_bWindowState = true;
    
    ADD_ANIMATION( C_Animation( ).BindWindowAlphaAnimate( AT_HIGH_TO_LOW, 0.1f, 1.f, m_flWindowAlpha ) );
}

void C_Keybinds::InitializeElements( )
{
    CALL_ONCE_START;
    {
        m_cBindCombo = C_Element( ).BindCombo( "Bind", ImVec2( 13.f, 7.f ), &m_cVecTempStrings, &m_iChoosenString );
        m_cBindValue = C_Element( ).KeybindValue( "Key", ImVec2( 13.f, 68.f ), &m_iKeyValue );
        m_cBindMode = C_Element( ).Combo( 0, "Mode", ImVec2( 13.f, 109.f ), { "Toggle", "Hold" }, &m_iKeybindMode );
        m_cBindShowInBinds = C_Element( ).Checkbox( 0, "Show In Binds", ImVec2( 13.f, 150.f ), &m_bShowInBinds );

        m_cBindNewValueCheckbox = C_Element( ).Checkbox( 0, "New Value", ImVec2( 13.f, 191.f ), &m_bNewValueCheckbox );
        m_cBindNewValueSlider = C_Element( ).Slider( 0, "New Value", ImVec2( 13.f, 191.f ), 0, 0, &m_iNewValueSlider );
        m_cBindNewValueCombo = C_Element( ).Combo( 0, "New Value", ImVec2( 13.f, 191.f ), { "None" }, &m_iNewValueCombo );
        m_cBindNewValueMultiCombo = C_Element( ).MultiCombo( 0, "New Value", ImVec2( 13.f, 191.f ), { "None" }, &m_iNewValueMultiCombo );
    }
    CALL_ONCE_END;
}

void C_Keybinds::DrawKeybindWindow( )
{
    InitializeKeybindVars( );
    InitializeElements( );

    if ( !m_bWindowState )
        return;

    g_NL->m_bSelectingShit = false;

    auto flBackupAlpha = ImGui::GetStyle( ).Alpha;
    ImGui::GetStyle( ).Alpha = m_flWindowAlpha;

    ImGui::PushStyleColor( ImGuiCol_WindowBg, RGB_VEC( 13.f, 13.f, 13.f, 255.f * ImGui::GetStyle( ).Alpha ) );

    {
        ImGui::PushStyleColor( ImGuiCol_Border, RGB_VEC( 30.f, 30.f, 32.f, 70.f * ImGui::GetStyle( ).Alpha ) );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 4.f );

        ImGui::SetNextWindowSize( ImVec2( 287.f, 261.f ) );
        ImGui::SetNextWindowPos( ImVec2( m_iKeybindWindowPosX - 3.f, m_iKeybindWindowPosY - 3.f ) );
        ImGui::SetNextWindowBgAlpha( 0.f );

        ImGui::Begin( "KeybindWindow_1", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
        ImGui::End( );

        ImGui::PopStyleVar( );
        ImGui::PopStyleColor( );
    }
    
    {
        ImGui::PushStyleColor( ImGuiCol_Border, RGB_VEC( 26.f, 26.f, 26.f, 255.f * ImGui::GetStyle( ).Alpha ) );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 2.f );

        ImGui::SetNextWindowSize( ImVec2( 283.f, 257.f ) );
        ImGui::SetNextWindowPos( ImVec2( m_iKeybindWindowPosX - 1.f, m_iKeybindWindowPosY - 1.f ) );
        ImGui::SetNextWindowBgAlpha( 0.f );

        ImGui::Begin( "KeybindWindow_2", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
        ImGui::End( );

        ImGui::PopStyleVar( );
        ImGui::PopStyleColor( );
    }

    ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.f );

    ImGui::SetNextWindowSize( ImVec2( 281.f, 255.f ) );
    ImGui::SetNextWindowPos( ImVec2( m_iKeybindWindowPosX, m_iKeybindWindowPosY ) );
    ImGui::Begin( "KeybindWindow_Main", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
    {
        // // //

        ImVec2 sLineStart = ImGui::GetWindowPos( );

        sLineStart.x += 7.f;
        sLineStart.y += 37.f;

        ImVec2 sLineEnd = sLineStart;

        sLineEnd.x += ImGui::GetWindowSize( ).x - 14.f;
        sLineEnd.y += 3.f;

        ImGui::GetWindowDrawList( )->AddRectFilled( sLineStart, sLineEnd, ImGui::ColorConvertFloat4ToU32( RGB_VEC( 26.f, 25.f, 28.f, 255.f * ImGui::GetStyle( ).Alpha ) ) );

        // // //

        if ( !m_cBindNewValueMultiCombo.GetVar( _h( "OpenState" ) ).GetInt( ) && !m_cBindCombo.GetVar( _h( "OpenState" ) ).GetInt( ) && !m_cBindNewValueCombo.GetVar( _h( "OpenState" ) ).GetInt( ) && ImGui::GetIO( ).MouseClicked[0] && !g_NL->CheckMouseIntersectionLocal( ImVec2( 0.f, 0.f ), ImGui::GetWindowSize( ) ) )
            ADD_ANIMATION( C_Animation( ).BindWindowAlphaAnimate( AT_HIGH_TO_LOW, 0.1f, 0.f, m_flWindowAlpha, [ ]( C_Animation* ) -> void { g_Keybinds->m_bWindowState = false; g_NL->m_bSelectingShit = false; } ) );

        if ( m_cBindCombo.GetVar( _h( "OpenState" ) ).GetInt( ) )
            g_NL->m_bSelectingShit = true;

        if ( m_cBindMode.GetVar( _h( "OpenState" ) ).GetInt( ) )
            g_NL->m_bSelectingShit = true;

        if ( m_pCurrentKeybind->m_eKeybindElementType == ET_CHECKBOX )
            g_NL->ProcessElement( &m_cBindNewValueCheckbox );
        else if ( m_pCurrentKeybind->m_eKeybindElementType == ET_SLIDER )
            g_NL->ProcessElement( &m_cBindNewValueSlider );
        else if ( m_pCurrentKeybind->m_eKeybindElementType == ET_COMBO )
            g_NL->ProcessElement( &m_cBindNewValueCombo );
        else if ( m_pCurrentKeybind->m_eKeybindElementType == ET_MULTI_COMBO )
            g_NL->ProcessElement( &m_cBindNewValueMultiCombo );

        g_NL->ProcessElement( &m_cBindValue );
        g_NL->ProcessElement( &m_cBindShowInBinds );
        g_NL->ProcessElement( &m_cBindMode );
        g_NL->ProcessElement( &m_cBindCombo );

        static float clr[4] = { 1.f, 1.f, 1.f, 1.f };

        //ImGui::ColorPicker4( "fdsfsd", clr );
        //ImGui::ColorEdit4( "TestEdit", clr, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoLabel );
    }
    ImGui::End( );

    ImGui::PopStyleColor( );
    ImGui::PopStyleVar( );

    ImGui::GetStyle( ).Alpha = flBackupAlpha;

    CheckKeybind( );

    g_NL->m_bSelectingShit = true;
}
