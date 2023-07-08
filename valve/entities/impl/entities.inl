#pragma once

#include "../entities.hpp"
enum e_class_id {
	C_AI_BASE_NPC,
	C_WEAPON_AK47,
	C_BASE_ANIMATING,
	C_BASE_ANIMATING_OVERLAY,
	C_BASE_ATTRIBUTABLE_ITEM,
	C_BASE_BUTTON,
	C_BASE_COMBAT_CHARACTER,
	C_BASE_COMBAT_WEAPON,
	C_BASE_CS_GRENADE,
	C_BASE_CS_GRENADE_PROJECTILE,
	C_BASE_DOOR,
	C_BASE_ENTITY,
	C_BASE_FLEX,
	C_BASE_GRENADE,
	C_BASE_PARTICLE_ENTITY,
	C_BASE_PLAYER,
	C_BASE_PROP_DOOR,
	C_BASE_TEAM_OBJECTIVE_RESOURCE,
	C_BASE_TEMP_ENTITY,
	C_BASE_TOGGLE,
	C_BASE_TRIGGER,
	C_BASE_VIEWMODEL,
	C_BASE_VPHYSICS_TRIGGER,
	C_BASE_WEAPON_WORLD_MODEL,
	C_BEAM,
	C_BEAM_SPOTLIGHT,
	C_BONE_FOLLOWER,
	C_BR_C4_TARGET,
	C_BREACH_CHARGE,
	C_BREACH_CHARGE_PROJECTILE,
	C_BREAKABLE_PROP,
	C_BREAKABLE_SURFACE,
	C_BUMP_MINE,
	C_BUMP_MINE_PROJECTILE,
	C_C4,
	C_CASCADE_LIGHT,
	C_CHICKEN,
	C_COLOR_CORRECTION,
	C_COLOR_CORRECTION_VOLUME,
	C_CS_GAMERULES_PROXY,
	C_CS_PLAYER,
	C_CS_PLAYER_RESOURCE,
	C_CS_RAGDOLL,
	C_CS_TEAM,
	C_DANGERZONE,
	C_DANGERZONE_CONTROLLER,
	C_DEAGLE,
	C_DECOY_GRENADE,
	C_DECOY_PROJECTILE,
	C_DRONE,
	C_DRONE_GUN,
	C_DYNAMIC_LIGHT,
	C_DYNAMIC_PROP,
	C_ECON_ENTITY,
	C_ECON_WEARABLE,
	C_EMBERS,
	C_ENTITY_DISSOLVE,
	C_ENTITY_FLAME,
	C_ENTITY_FREEZING,
	C_ENTITY_PARTICLE_TRAIL,
	C_ENV_AMBIENT_LIGHT,
	C_ENV_DETAIL_CONTROLLER,
	C_ENV_DOF_CONTROLLER,
	C_ENV_GAS_CANISTER,
	C_ENV_PARTICLE_SCRIPT,
	C_ENV_PROJECTED_TEXTURE,
	C_ENV_QUADRATIC_BEAM,
	C_ENV_SCREEN_EFFECT,
	C_ENV_SCREEN_OVERLAY,
	C_ENV_TONEMAP_CONTROLLER,
	C_ENV_WIND,
	C_FE_PLAYER_DECAL,
	C_FIRECRACKER_BLAST,
	C_FIRE_SMOKE,
	C_FIRE_TRAIL,
	C_FISH,
	C_FISTS,
	C_FLASHBANG,
	C_FOG_CONTROLLER,
	C_FOOTSTEP_CONTROL,
	C_FUNC_DUST,
	C_FUNC_LOD,
	C_FUNC_AREA_PORTAL_WINDOW,
	C_FUNC_BRUSH,
	C_FUNC_CONVEYOR,
	C_FUNC_LADDER,
	C_FUNC_MONITOR,
	C_FUNC_MOVE_LINEAR,
	C_FUNC_OCCLUDER,
	C_FUNC_REFLECTIVE_GLASS,
	C_FUNC_ROTATING,
	C_FUNC_SMOKE_VOLUME,
	C_FUNC_TRACK_TRAIN,
	C_GAMERULES_PROXY,
	C_GRASS_BURN,
	C_HANDLE_TEST,
	C_HE_GRENADE,
	C_HOSTAGE,
	C_HOSTAGE_CARRIABLE_PROP,
	C_INCENDIARY_GRENADE,
	C_INFERNO,
	C_INFO_LADDER_DISMOUNT,
	C_INFO_MAP_REGION,
	C_INFO_OVERLAY_ACCESSOR,
	C_ITEM_HEALTHSHOT,
	C_ITEM_CASH,
	C_ITEM_DOGTAGS,
	C_KNIFE,
	C_KNIFE_GG,
	C_LIGHT_GLOW,
	C_MAP_VETO_PICK_CONTROLLER,
	C_MATERIAL_MODIFY_CONTROL,
	C_MELEE,
	C_MOLOTOV_GRENADE,
	C_MOLOTOV_PROJECTILE,
	C_MOVIE_DISPLAY,
	C_PARADROP_CHOPPER,
	C_PARTICLE_FIRE,
	C_PARTICLE_PERFORMANCE_MONITOR,
	C_PARTICLE_SYSTEM,
	C_PHYS_BOX,
	C_PHYS_BOX_MULTIPLAYER,
	C_PHYSICS_PROP,
	C_PHYSICS_PROP_MULTIPLAYER,
	C_PHYS_MAGNET,
	C_PHYS_PROP_AMMO_BOX,
	C_PHYS_PROP_LOOT_CRATE,
	C_PHYS_PROP_RADAR_JAMMER,
	C_PHYS_PROP_WEAPON_UPGRADE,
	C_PLANTED_C4,
	C_PLASMA,
	C_PLAYER_PING,
	C_PLAYER_RESOURCE,
	C_POINT_CAMERA,
	C_POINT_COMMENT_ARYNODE,
	C_POINT_WORLD_TEXT,
	C_POSE_CONTROLLER,
	C_POST_PROCESS_CONTROLLER,
	C_PRECIPITATION,
	C_PRECIPITATION_BLOCKER,
	C_PREDICTED_VIEW_MODEL,
	C_PROP_HALLUCINATION,
	C_PROP_COUNTER,
	C_PROP_DOOR_ROTATING,
	C_PROP_JEEP,
	C_PROP_VEHICLE_DRIVEABLE,
	C_RAGDOLL_MANAGER,
	C_RAGDOLL_PROP,
	C_RAGDOLL_PROP_ATTACHED,
	C_ROPE_KEYFRAME,
	C_WEAPON_SCAR17,
	C_SCENE_ENTITY,
	C_SENSOR_GRENADE,
	C_SENSOR_GRENADE_PROJECTILE,
	C_SHADOW_CONTROL,
	C_SLIDE_SHOW_DISPLAY,
	C_SMOKE_GRENADE,
	C_SMOKE_GRENADE_PROJECTILE,
	C_SMOKE_STACK,
	C_SNOWBALL,
	C_SNOWBALL_PILE,
	C_SNOWBALL_PROJECTILE,
	C_SPATIAL_ENTITY,
	C_SPOTLIGHT_END,
	C_SPRITE,
	C_SPRITE_ORIENTED,
	C_SPRITE_TRAIL,
	C_STATUE_PROP,
	C_STEAMJET,
	C_SUN,
	C_SUNLIGHT_SHADOW_CONTROL,
	C_SURVIVAL_SPAWN_CHOPPER,
	C_TABLET,
	C_TEAM,
	C_TEAM_PLAYROUND_BASED_RULES_PROXY,
	C_TEAR_MORRICOCHET,
	C_TE_BASE_BEAM,
	C_TE_BEAM_ENT_POINT,
	C_TE_BEAM_ENTS,
	C_TE_BEAM_FOLLOW,
	C_TE_BEAM_LASER,
	C_TE_BEAM_POINTS,
	C_TE_BEAM_RING,
	C_TE_BEAM_RING_POINT,
	C_TE_BEAM_SPLINE,
	C_TE_BLOOD_SPRITE,
	C_TE_BLOOD_STREAM,
	C_TE_BREAK_MODEL,
	C_TE_BSP_DECAL,
	C_TE_BUBBLES,
	C_TE_BUBBLE_TRAIL,
	C_TE_CLIENT_PROJECTILE,
	C_TE_DECAL,
	C_TE_DUST,
	C_TE_DYNAMIC_LIGHT,
	C_TE_EFFECT_DISPATCH,
	C_TE_ENERGY_SPLASH,
	C_TE_EXPLOSION,
	C_TE_FIRE_BULLETS,
	C_TE_FIZZ,
	C_TE_FOOTPRINT_DECAL,
	C_TE_FOUNDRY_HELPERS,
	C_TE_GAUSS_EXPLOSION,
	C_TE_GLOW_SPRITE,
	C_TE_IMPACT,
	C_TE_KILL_PLAYER_ATTACHMENTS,
	C_TE_LARGE_FUNNEL,
	C_TE_METAL_SPARKS,
	C_TE_MUZZLE_FLASH,
	C_TE_PARTICLE_SYSTEM,
	C_TE_PHYSICS_PROP,
	C_TE_PLANT_BOMB,
	C_TE_PLAYER_ANIM_EVENT,
	C_TE_PLAYER_DECAL,
	C_TE_PROJECTED_DECAL,
	C_TE_RADIOICON,
	C_TE_SHATTER_SURFACE,
	C_TE_SHOW_LINE,
	C_TE_SLA,
	C_TE_SMOKE,
	C_TE_SPARKS,
	C_TE_SPRITE,
	C_TE_SPRITE_SPRAY,
	C_TEST_PROXY_TOGGLE_NETWORKABLE,
	C_TEST_TRACE_LINE,
	C_TE_WORLD_DECAL,
	C_TRIGGER_PLAYER_MOVEMENT,
	C_TRIGGER_SOUND_OPERATOR,
	C_VGUI_SCREEN,
	C_VOTE_CONTROLLER,
	C_WATER_BULLET,
	C_WATER_LOD_CONTROL,
	C_WEAPON_AUG,
	C_WEAPON_AWP,
	C_WEAPON_BASE_ITEM,
	C_WEAPON_PPBIZON,
	C_WEAPON_CS_BASE,
	C_WEAPON_CS_BASE_GUN,
	C_WEAPON_CYCLER,
	C_WEAPON_DUAL_BERETTAS,
	C_WEAPON_FAMAS,
	C_WEAPON_FIVESEVEN,
	C_WEAPON_G3SG1,
	C_WEAPON_GALIL,
	C_WEAPON_GALIL_AR,
	C_WEAPON_GLOCK18,
	C_WEAPON_P2000,
	C_WEAPON_M249,
	C_WEAPON_M3,
	C_WEAPON_M4A4,
	C_WEAPON_MAC10,
	C_WEAPON_MAG7,
	C_WEAPON_MP5SD,
	C_WEAPON_MP7,
	C_WEAPON_MP9,
	C_WEAPON_NEGEV,
	C_WEAPON_NOVA,
	C_WEAPON_P228,
	C_WEAPON_P250,
	C_WEAPON_P90,
	C_WEAPON_SAWEDOFF,
	C_WEAPON_SCAR20,
	C_WEAPON_SCOUT,
	C_WEAPON_SG550,
	C_WEAPON_SG552,
	C_WEAPON_SG553,
	C_WEAPON_BALLISTIC_SHIELD,
	C_WEAPON_SSG08,
	C_WEAPON_ZEUSX27,
	C_WEAPON_TEC9,
	C_WEAPON_TMP,
	C_WEAPON_UMP45,
	C_WEAPON_USP,
	C_WEAPON_XM1014,
	C_WEAPON_ZONE_REPULSOR,
	C_WORLD,
	C_WORLD_VGUI_TEXT,
	DUST_TRAIL,
	MOVIE_EXPLOSION,
	PARTICLE_SMOKE_GRENADE,
	ROCKET_TRAIL,
	SMOKE_TRAIL,
	SPORE_EXPLOSION,
	SPORE_TRAIL
};

enum e_damage_type {
	DAMAGE_NO,
	DAMAGE_EVENTS_ONLY,
	DAMAGE_YES,
	DAMAGE_AIM
};

namespace nem::valve {
	__forceinline std::uintptr_t c_entity::renderable( ) {
		return reinterpret_cast< std::uintptr_t >( this ) + sizeof( std::uintptr_t );
	}

	__forceinline std::uintptr_t c_entity::networkable( ) {
		return reinterpret_cast< std::uintptr_t >( this ) + sizeof( std::uintptr_t ) * 2u;
	}

	__forceinline int c_entity::index( ) {
		using fn_t = int( __thiscall* )( const std::uintptr_t );

		return ( *reinterpret_cast< fn_t** >( networkable( ) ) )[ 10u ]( networkable( ) );
	}

	__forceinline e_ent_handle c_entity::handle( ) {
		using fn_t = e_ent_handle( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 2u ]( this );
	}

	__forceinline bool c_entity::dormant( ) {
		using fn_t = bool( __thiscall* )( const std::uintptr_t );

		return ( *reinterpret_cast< fn_t** >( networkable( ) ) )[ 9u ]( networkable( ) );
	}

	__forceinline client_class_t* c_entity::client_class( ) {
		using fn_t = client_class_t*( __thiscall* )( const std::uintptr_t );
	
		return ( *reinterpret_cast< fn_t** >( networkable( ) ) )[ 2u ]( networkable( ) );
	}

	__forceinline bool c_entity::setup_bones(
		mat3x4_t* const bones, const int max_bones,
		const int mask, const float time
	) {
		using fn_t = bool( __thiscall* )(
			const std::uintptr_t, mat3x4_t* const,
			const int, const int, const float
		);

		return ( *reinterpret_cast< fn_t** >( renderable( ) ) )[ 13u ](
			renderable( ), bones, max_bones, mask, time
		);
	}

	__forceinline float& c_entity::sim_time( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseEntity->m_flSimulationTime" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_entity::old_sim_time( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseEntity->m_flSimulationTime" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var ) + 0x4u
		);
	}

	__forceinline vec3_t& c_entity::origin( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseEntity->m_vecOrigin" ) );

		return *reinterpret_cast< vec3_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline qangle_t& c_entity::abs_angles( ) {
		return *reinterpret_cast< qangle_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0xc4u
		);
	}

	__forceinline vec3_t& c_entity::abs_origin( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "m_vecAbsOrigin" ) );

		return *reinterpret_cast< vec3_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_entity::precip_type ( ) {
		static const auto& net_var = g_net_vars->entry ( xorstr_ ( "CPrecipitation->m_nPrecipType" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t > ( net_var )
			);
	}

	__forceinline vec3_t& c_entity::abs_velocity( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "m_vecAbsVelocity" ) );

		return *reinterpret_cast< vec3_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline void c_entity::set_abs_angles( const qangle_t& abs_angles ) {
		using fn_t = void( __thiscall* )( decltype( this ), const qangle_t& );

		return reinterpret_cast< fn_t >(
			g_context->addresses( ).m_set_abs_angles.get( )
		)( this, abs_angles );
	}

	__forceinline void c_entity::set_abs_origin( const vec3_t& abs_origin ) {
		using fn_t = void( __thiscall* )( decltype( this ), const vec3_t& );

		return reinterpret_cast< fn_t >(
			g_context->addresses( ).m_set_abs_origin.get( )
		)( this, abs_origin );
	}

	__forceinline e_ent_flags& c_entity::flags( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_fFlags" ) );

		return *reinterpret_cast< e_ent_flags* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline std::uint32_t& c_entity::eflags( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "m_iEFlags" ) );

		return *reinterpret_cast< std::uint32_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline model_t* c_entity::model( ) {
		using fn_t = model_t*( __thiscall* )( const std::uintptr_t );

		return ( *reinterpret_cast< fn_t** >( renderable( ) ) )[ 8u ]( renderable( ) );
	}

	__forceinline model_data_t*& c_entity::mdl_data( ) {
		return *reinterpret_cast< model_data_t** >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x2950u
		);
	}

	__forceinline std::uint32_t& c_entity::effects( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "m_fEffects" ) );

		return *reinterpret_cast< std::uint32_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline std::uint32_t& c_entity::occlusion_flags( ) {
		return *reinterpret_cast< std::uint32_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0xa28u
		);
	}

	__forceinline int& c_entity::occlusion_frame( ) {
		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0xa30u
		);
	}

	__forceinline ik_context_t*& c_entity::ik_context( ) {
		return *reinterpret_cast< ik_context_t** >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x2670u
		);
	}

	__forceinline void*& c_entity::inverse_kinematics ( ) {
		return *reinterpret_cast< void** >(
			reinterpret_cast< std::uintptr_t >( this ) + 9840u
			);
	}

	__forceinline int& c_entity::last_setup_bones_frame( ) {
		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0xa68u
		);
	}

	__forceinline float& c_entity::last_setup_bones_time( ) {
		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x2928u
		);
	}

	__forceinline unsigned long& c_entity::mdl_bone_counter( ) {
		return *reinterpret_cast< unsigned long* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x2690u
		);
	}

	__forceinline bone_accessor_t& c_entity::bone_accessor( ) {
		return *reinterpret_cast< bone_accessor_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x26a4u
		);
	}

	__forceinline utl_vec_t< mat3x4_t >& c_entity::bone_cache( ) {
		return *reinterpret_cast< utl_vec_t< mat3x4_t >* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x2914u
		);
	}

	__forceinline std::uint8_t& c_entity::ent_client_flags( ) {
		return *reinterpret_cast< std::uint8_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x68u
		);
	}

	__forceinline studio_hdr_t* c_entity::studio_hdr_ ( ) { 
		return *reinterpret_cast< studio_hdr_t** >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x2950u
			);
	}

	__forceinline void c_player::attachment_helper ( ) { 
		using fn = void ( __thiscall* )( valve::c_player*, valve::studio_hdr_t* );

		static fn attachment_help = reinterpret_cast < fn > ( g_context->addresses ( ).m_attachment_helper.get( ) );

		attachment_help ( this, this->studio_hdr_ ( ) );
	}

	__forceinline bool c_entity::breakable( ) {
		using fn_t = bool ( __thiscall* )( decltype( this ) );

		if ( reinterpret_cast< fn_t >( g_context->addresses ( ).m_breakable.get ( ) )( this ) )
			return true;

		const auto cc = client_class ( );
		if ( !cc )
			return false;

		return ( *reinterpret_cast< const std::uint32_t* >( cc->m_network_name ) == 'erBC'
			&& *reinterpret_cast< const std::uint32_t* >( cc->m_network_name + 7 ) == 'Selb' )
			|| ( *reinterpret_cast< const std::uint32_t* >( cc->m_network_name ) == 'saBC'
				&& *reinterpret_cast< const std::uint32_t* >( cc->m_network_name + 7 ) == 'ytit' );
	}

	__forceinline int& c_entity::team( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseEntity->m_iTeamNum" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline bool c_entity::friendly( ) {
		return team( ) == valve___g_local_player->team( );
	}

	__forceinline int& c_entity::sim_tick( ) {
		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x2acu
		);
	}

	__forceinline vec3_t& c_entity::obb_min( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseEntity->m_vecMins" ) );

		return *reinterpret_cast< vec3_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline vec3_t& c_entity::obb_max( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseEntity->m_vecMaxs" ) );

		return *reinterpret_cast< vec3_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_entity::obb_change_time ( ) {
		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x9924u
			);
	}

	__forceinline float& c_entity::obb_change_height ( ) {
		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x9920u
			);
	}

	__forceinline void c_entity::set_collision_bounds(
		const vec3_t& obb_min, const vec3_t& obb_max, float change_time
	) {
		using collideable_fn_t = std::uintptr_t( __thiscall* )( decltype( this ) );

		const auto collideable = ( *reinterpret_cast< collideable_fn_t** >( this ) )[ 3u ]( this );
		if ( !collideable )
			return;

		if ( !this )
			return;

		/* 55 8B EC 8B 45 10 F3 0F 10 81 ? ? ? ? */
	
		const auto backup_obb_height = obb_change_height ( );
		const auto backup_obb_change_time = obb_change_time ( );

		using fn_t = void( __thiscall* )( const std::uintptr_t, const vec3_t&, const vec3_t& );

		reinterpret_cast< fn_t >(
			g_context->addresses( ).m_set_collision_bounds.get( )
		)( collideable, obb_min, obb_max );

		obb_change_height ( ) = backup_obb_height;
		obb_change_time ( ) = backup_obb_change_time;
	}

	__forceinline const vec3_t& c_entity::world_space_center( ) {
		using fn_t = const vec3_t&( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 79u ]( this );
	}

	__forceinline bool c_entity::is_player( ) {
		using fn_t = bool( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 158u ]( this );
	}

	__forceinline valve::data_map_t* c_entity::get_pred_desc_map ( )
	{
		using fn_t = valve::data_map_t* ( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 17u ] ( this );
	}

	__forceinline mat3x4_t& c_entity::get_rgfl ( )
	{
		static const auto& net_var = g_net_vars->entry ( xorstr_ ( "CBaseEntity->m_CollisionGroup" ) );
		return *reinterpret_cast< mat3x4_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t > ( net_var )
			- 0x30 );
	}

	__forceinline bool c_entity::is_weapon( ) {
		using fn_t = bool( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 166u ]( this );
	}

	__forceinline var_mapping_t& c_entity::var_mapping( ) {
		return *reinterpret_cast< var_mapping_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x24u
		);
	}

	__forceinline float& c_entity::cycle( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseAnimating->m_flCycle" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_entity::anim_time( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseEntity->m_flAnimTime" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_entity::anim_parity ( ) {
		static const auto& net_var = g_net_vars->entry ( xorstr_ ( "CBaseViewModel->m_nAnimationParity" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t > ( net_var )
			);
	}

	__forceinline e_ent_handle& c_entity::move_parent( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseEntity->moveparent" ) );

		return *reinterpret_cast< e_ent_handle* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline e_ent_handle& c_entity::rag_doll ( ) {
		static const auto& net_var = g_net_vars->entry ( xorstr_ ( "CCSPlayer->m_hRagdoll" ) );

		return *reinterpret_cast< e_ent_handle* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t > ( net_var )
			);
	}

	__forceinline bool& c_entity::spotted( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseEntity->m_bSpotted" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline e_ent_handle& c_entity::thrower( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseGrenade->m_hThrower" ) );

		return *reinterpret_cast< e_ent_handle* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_entity::explode_effect_tick_begin( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseCSGrenadeProjectile->m_nExplodeEffectTickBegin" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_entity::grenade_spawn_time( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseCSGrenadeProjectile->m_vecExplodeEffectOrigin" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var ) + 0xcu
		);
	}

	__forceinline bool& c_entity::use_fast_path( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseAnimating->m_flFrozen" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var ) + 0x4u
		);
	}

	__forceinline int& c_entity::bomb_site( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CPlantedC4->m_nBombSite" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline bool& c_entity::is_bomb_ticking( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CPlantedC4->m_bBombTicking" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_entity::c4_blow( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CPlantedC4->m_flC4Blow" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline vec3_t& c_entity::base_velocity ( ) {
		static const auto& net_var = g_net_vars->entry ( xorstr_ ( "CBasePlayer->m_vecBaseVelocity" ) );

		return *reinterpret_cast< vec3_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t > ( net_var )
			);
	}

	__forceinline std::uint16_t c_entity::model_instance( ) {
		using fn_t = std::uint16_t( __thiscall* )( const std::uintptr_t );

		return ( *reinterpret_cast< fn_t** >( renderable( ) ) )[ 30u ]( renderable( ) );
	}

	__forceinline int& c_entity::model_index( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseEntity->m_nModelIndex" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline e_ent_handle& c_entity::world_model_handle( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseCombatWeapon->m_hWeaponWorldModel" ) );

		return *reinterpret_cast< e_ent_handle* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_entity::sequence( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseViewModel->m_nSequence" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline e_ent_handle& c_entity::view_model_weapon_handle( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseViewModel->m_hWeapon" ) );

		return *reinterpret_cast< e_ent_handle* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline e_ent_handle& c_entity::view_model_owner( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseViewModel->m_hOwner" ) );

		return *reinterpret_cast< e_ent_handle* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline void c_entity::set_model_index( const int index ) {
		using fn_t = void( __thiscall* )( decltype( this ), const int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 75u ]( this, index );
	}

	__forceinline float c_weapon::max_speed( ) {
		using fn_t = float( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 442u ]( this );
	}

	__forceinline e_item_index& c_weapon::item_index( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseAttributableItem->m_iItemDefinitionIndex" ) );

		return *reinterpret_cast< e_item_index* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_weapon::postpone_fire_ready_time( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CWeaponCSBase->m_flPostponeFireReadyTime" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var  )
		);
	}

	__forceinline float& c_weapon::accuracy_penalty( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CWeaponCSBase->m_fAccuracyPenalty" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_weapon::last_shot_time( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CWeaponCSBase->m_fLastShotTime" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_weapon::recoil_index( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CWeaponCSBase->m_flRecoilIndex" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_weapon::ammo( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseCombatWeapon->m_iClip1" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_weapon::next_primary_attack( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseCombatWeapon->m_flNextPrimaryAttack" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_weapon::next_secondary_attack( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseCombatWeapon->m_flNextSecondaryAttack" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_weapon::throw_time( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseCSGrenade->m_fThrowTime" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_weapon::throw_strength( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseCSGrenade->m_flThrowStrength" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline bool& c_weapon::pin_pulled( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseCSGrenade->m_bPinPulled" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float c_weapon::spread( ) {
		using fn_t = float( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 453u ]( this );
	}

	__forceinline float c_weapon::inaccuracy( ) {
		using fn_t = float( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 483u ]( this );
	}

	__forceinline void c_weapon::update_inaccuracy( ) {
		using fn_t = void( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 484u ]( this );
	}

	__forceinline float c_player::get_layer_seq_cycle_rate ( valve::anim_layer_t* layer, int seq )
	{
		using fn_t = float ( __thiscall* )( decltype( this ), valve::anim_layer_t*, int );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 223u ] ( this, layer, seq );
	}

	__forceinline void c_player::eye_pos ( vec3_t* to )	
	{
		using fn_t = void ( __thiscall* )( decltype( this ), vec3_t*  );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 169u ] ( this, to );
	}

	__forceinline weapon_data_t* c_weapon::wpn_data( ) {
		using fn_t = weapon_data_t*( __thiscall* )( const std::uintptr_t, const e_item_index );

		const auto wpn_sys = g_context->addresses( ).m_weapon_system.get( );

		return ( *reinterpret_cast< fn_t** >( wpn_sys ) )[ 2u ]( wpn_sys, item_index( ) );
	}

	__forceinline bool& c_weapon::in_reload( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "m_bInReload" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_weapon::zoom_lvl( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CWeaponCSBaseGun->m_zoomLevel" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_weapon::item_id_high( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseAttributableItem->m_iItemIDHigh" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_weapon::account_id( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseAttributableItem->m_iAccountID" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_weapon::fallback_paint_kit( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseAttributableItem->m_nFallbackPaintKit" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_weapon::item_quality( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseAttributableItem->m_iEntityQuality" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_weapon::fallback_stat_trak( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseAttributableItem->m_nFallbackStatTrak" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_weapon::fallback_wear( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseAttributableItem->m_flFallbackWear" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_weapon::owner_xuid_low( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseAttributableItem->m_OriginalOwnerXuidLow" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_weapon::owner_xuid_high( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseAttributableItem->m_OriginalOwnerXuidHigh" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline bool& c_weapon::custom_material_initialized( ) {
		return *reinterpret_cast< bool* > (
			reinterpret_cast< std::uintptr_t >( this ) + 0x3370u
		);
	}

	__forceinline utl_vec_t< ret_counted_t* >& c_weapon::custom_materials( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseCombatWeapon->m_Item" ) );

		return *reinterpret_cast< utl_vec_t< ret_counted_t* >* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var ) + 0x14u
		);
	}

	__forceinline utl_vec_t< ret_counted_t* >& c_weapon::custom_materials2( ) {
		return *reinterpret_cast< utl_vec_t< ret_counted_t* >* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x9dcu
		);
	}

	__forceinline utl_vec_t< ret_counted_t* >& c_weapon::visuals_data_processors( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseCombatWeapon->m_Item" ) );

		return *reinterpret_cast< utl_vec_t< ret_counted_t* >* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var ) + 0x230u
		);
	}

	__forceinline bool& c_entity::get_use_custom_bloom_scale ( )
	{
		static const auto& net_var = g_net_vars->entry ( xorstr_ ( "CEnvTonemapController->m_bUseCustomBloomScale" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t > ( net_var )
			);
	}

	__forceinline bool& c_entity::get_user_custom_auto_exposure_min ( )
	{
		static const auto& net_var = g_net_vars->entry ( xorstr_ ( "CEnvTonemapController->m_bUseCustomAutoExposureMin" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t > ( net_var )
			);
	}

	__forceinline bool& c_entity::get_user_custom_auto_exposure_max ( )
	{
		static const auto& net_var = g_net_vars->entry ( xorstr_ ( "CEnvTonemapController->m_bUseCustomAutoExposureMax" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t > ( net_var )
			);
	}

	__forceinline float& c_entity::get_custom_bloom_scale ( )
	{
		static const auto& net_var = g_net_vars->entry ( xorstr_ ( "CEnvTonemapController->m_flCustomBloomScale" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t > ( net_var )
			);
	}

	__forceinline float& c_entity::get_auto_custom_exposure_min ( )
	{
		static const auto& net_var = g_net_vars->entry ( xorstr_ ( "CEnvTonemapController->m_flCustomAutoExposureMin" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t > ( net_var )
			);
	}

	__forceinline float& c_entity::get_auto_custom_exposure_max ( )
	{
		static const auto& net_var = g_net_vars->entry ( xorstr_ ( "CEnvTonemapController->m_flCustomAutoExposureMax" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t > ( net_var )
			);
	}

	__forceinline void c_player::set_local_view_angles( const qangle_t& angle ) {
		using fn_t = void( __thiscall* )( decltype( this ), const qangle_t& );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 373u ]( this, angle );
	}

	__forceinline vec3_t& c_cascade_light::get_shadow_direction ( )
	{
		static const auto& net_var = g_net_vars->entry ( xorstr_ ( "CCascadeLight->m_envLightShadowDirection" ) );

		return *reinterpret_cast< vec3_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t > ( net_var )
			);
	}

	__forceinline int& c_player::tick_base( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_nTickBase" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_player::physics_collision_state ( )
	{
		static const auto& net_var = g_net_vars->entry ( xorstr_ ( "CBasePlayer->m_vphysicsCollisionState" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t > ( net_var )
			);
	}

	__forceinline e_life_state& c_player::life_state( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_lifeState" ) );

		return *reinterpret_cast< e_life_state* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_player::health( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_iHealth" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline bool c_player::alive( ) {
		return ( health( ) > 0 ) && ( life_state( ) == e_life_state::alive );
	}

	__forceinline pose_params_t& c_player::pose_params( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseAnimating->m_flPoseParameter" ) );

		return *reinterpret_cast< pose_params_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_player::spawn_time( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_iAddonBits" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var ) - 0x4u
		);
	}

	__forceinline anim_layers_t& c_player::anim_layers( ) {
		return **reinterpret_cast< anim_layers_t** >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x2990u
		);
	}

	__forceinline anim_state_t*& c_player::anim_state( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_bIsScoped" ) );

		return *reinterpret_cast< anim_state_t** >(
			reinterpret_cast< std::uintptr_t >( this )
			+ std::get< std::uint32_t >( net_var ) - 0x14u
		);
	}

	__forceinline int& c_player::final_predicted_tick ( ) {
		static const auto& net_var = g_net_vars->entry ( xorstr_ ( "CBasePlayer->m_nTickBase" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t > ( net_var ) + 0x4u
			);
	}

	__forceinline bool& c_player::client_side_anim( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseAnimating->m_bClientSideAnimation" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline void c_player::update_client_side_anim( ) {
		using fn_t = void( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 224u ]( this );
	}

	__forceinline void c_player::update_collision_bounds ( ) {
		using fn_t = void ( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) ) [ 340u ] ( this );
	}

	__forceinline qangle_t& c_player::eye_angles( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_angEyeAngles" ) );

		return *reinterpret_cast< qangle_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline vec3_t& c_player::velocity( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_vecVelocity[0]" ) );

		return *reinterpret_cast< vec3_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float c_player::max_delta ( )
	{
		auto v9 = anim_state ( );

		if ( !v9 )
			return -1;

		auto v7 = -1.f;

		auto v6 = -1.f;

		auto v5 = -1.f;

		auto v4 = -1.f;

		auto v3 = -1.f;

		auto v2 = -1.f;

		auto v8 = -1.f;

		if ( v9->m_speed_as_portion_of_walk_speed >= 1.0 )
			v7 = 1.0;
		else
			v7 = v9->m_speed_as_portion_of_walk_speed;
		if ( v7 >= 0.0 )
		{
			if ( v9->m_speed_as_portion_of_walk_speed >= 1.0 )
				v6 = 1.0;
			else
				v6 = v9->m_speed_as_portion_of_walk_speed;
			v5 = v6;
		}
		else
		{
			v5 = 0.0;
		}
		if ( v9->m_speed_as_portion_of_crouch_speed >= 1.0 )
			v4 = 1.0;
		else
			v4 = v9->m_speed_as_portion_of_crouch_speed;
		if ( v4 >= 0.0 )
		{
			if ( v9->m_speed_as_portion_of_crouch_speed >= 1.0 )
				v3 = 1.0;
			else
				v3 = v9->m_speed_as_portion_of_crouch_speed;
			v2 = v3;
		}
		else
		{
			v2 = 0.0;
		}
		v8 = ( ( ( v9->m_walk_to_run_transition * -0.30000001 ) - 0.2 ) * v5 ) + 1.0;
		if ( v9->m_duck_amount > 0.0 )
			v8 = ( ( v9->m_duck_amount * v2 ) * ( 0.5 - v8 ) ) + v8;
		return ( v9->m_max_body_yaw * v8 );
	}

	__forceinline float& c_player::fall_velocity ( )
	{
		static const auto& net_var = g_net_vars->entry ( xorstr_ ( "CBasePlayer->m_flFallVelocity" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t > ( net_var )
			);
	}
	

	__forceinline float& c_player::duck_amount( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "m_flDuckAmount" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline e_ent_handle& c_player::wpn_handle( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseCombatCharacter->m_hActiveWeapon" ) );

		return *reinterpret_cast< e_ent_handle* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline e_ent_handle& c_player::ground_entity( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "m_hGroundEntity" ) );

		return *reinterpret_cast< e_ent_handle* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline c_weapon* c_player::weapon( ) {
		const auto handle = wpn_handle( );
		if ( handle == e_ent_handle::invalid )
			return nullptr;

		return static_cast< c_weapon* >( g_entity_list->find_entity( handle ) );
	}

	__forceinline int& c_player::hitbox_set_index( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseAnimating->m_nHitboxSet" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_player::skin( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "m_nSkin" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_player::body( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "m_nBody" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_player::lby( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_flLowerBodyYawTarget" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int c_player::lookup_seq_act( const int sequence ) {
		const auto model_data = mdl_data( );
		if ( !model_data
			|| !model_data->m_studio_hdr )
			return -1;

		using fn_t = int( __fastcall* )( decltype( this ), studio_hdr_t* const, const int );

		return reinterpret_cast< fn_t >(
			g_context->addresses( ).m_lookup_seq_act.get( )
		)( this, model_data->m_studio_hdr, sequence );
	}

	__forceinline vec3_t& c_player::view_offset( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_vecViewOffset[0]" ) );

		return *reinterpret_cast< vec3_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline qangle_t& c_player::aim_punch( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_aimPunchAngle" ) );

		return *reinterpret_cast< qangle_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline vec3_t& c_player::aim_punch_vel( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_aimPunchAngleVel" ) );

		return *reinterpret_cast< vec3_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_player::next_attack( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "m_flNextAttack" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline bool& c_player::ducking( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_bDucking" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline bool& c_player::ducked( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_bDucked" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline bool& c_player::walking( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_bIsWalking" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline bool& c_player::strafing( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_bStrafing" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline e_move_type& c_player::move_type( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "m_MoveType" ) );

		return *reinterpret_cast< e_move_type* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_player::velocity_modifier( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_flVelocityModifier" ) ); // physics_coll_state in data map is velocity_modifier

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline user_cmd_t*& c_player::cur_user_cmd( ) {
		return *reinterpret_cast< user_cmd_t** >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x3348u
		);
	}

	__forceinline user_cmd_t& c_player::last_user_cmd( ) {
		return *reinterpret_cast< user_cmd_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x3298u
		);
	}

	__forceinline cmd_context_t& c_player::cmd_context( ) {
		return *reinterpret_cast< cmd_context_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x350cu
		);
	}

	__forceinline int& c_player::unknown_offset ( )
	{
		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x9B14
			);
	}


	__forceinline bool& c_player::scoped( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_bIsScoped" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline vec3_t c_player::get_shoot_pos ( valve::bones_t& bones, bool fake_ducking )
	{
		if ( !this )
			return vec3_t ( );

		vec3_t shoot_pos;

		shoot_pos = abs_origin ( ) + view_offset ( );

		if ( unknown_offset ( ) )
			modify_eye_pos ( shoot_pos, bones );

		return shoot_pos;
	}

	__forceinline qangle_t& c_player::view_punch( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_viewPunchAngle" ) );

		return *reinterpret_cast< qangle_t* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_player::duck_speed( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_flDuckSpeed" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_player::move_state( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_iMoveState" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_player::third_person_recoil( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_flThirdpersonRecoil" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int c_player::lookup_bone( const char* name ) {
		using fn_t = int( __thiscall* )( decltype( this ), const char* );

		return reinterpret_cast< fn_t >( g_context->addresses( ).m_lookup_bone.get( ) )( this, name );
	}

	__forceinline int& c_player::armor_value( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_ArmorValue" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline bool& c_player::has_helmet( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_bHasHelmet" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline bool& c_player::has_heavy_armor( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_bHasHeavyArmor" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_player::max_speed( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_flMaxspeed" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_player::surface_friction( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "m_surfaceFriction" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline bool& c_player::immune( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_bGunGameImmunity" ) );

		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline e_ent_handle c_player::view_model_handle( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_hViewModel[0]" ) );

		return *reinterpret_cast< e_ent_handle* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline c_entity* c_player::view_model( ) {
		return g_entity_list->find_entity( view_model_handle( ) );
	}

	__forceinline int& c_player::money( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_iAccount" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float c_player::fov( ) {
		using fn_t = float( __thiscall* )( decltype( this ) );

		return ( *reinterpret_cast< fn_t** >( this ) )[ 332u ]( this );
	}

	__forceinline bool c_player::has_c4( ) {
		using fn_t = bool( __thiscall* )( decltype( this ) );

		return reinterpret_cast< fn_t >( g_context->addresses( ).m_has_c4.get( ) )( this );
	}

	__forceinline int& c_player::observer_mode( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_iObserverMode" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline e_ent_handle& c_player::observer_target( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBasePlayer->m_hObserverTarget" ) );

		return *reinterpret_cast< e_ent_handle* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline bool& c_player::use_new_anim_state( ) {
		return *reinterpret_cast< bool* >(
			reinterpret_cast< std::uintptr_t >( this ) + 0x9b14u
		);
	}

	__forceinline e_ent_handle* c_player::weapons( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CBaseCombatCharacter->m_hMyWeapons" ) );

		return reinterpret_cast< e_ent_handle* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline float& c_player::flash_duration( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_flFlashDuration" ) );

		return *reinterpret_cast< float* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline int& c_player::shots_fired( ) {
		static const auto& net_var = g_net_vars->entry( xorstr_( "CCSPlayer->m_iShotsFired" ) );

		return *reinterpret_cast< int* >(
			reinterpret_cast< std::uintptr_t >( this ) + std::get< std::uint32_t >( net_var )
		);
	}

	__forceinline c_local_player::operator bool( ) const {
		return !!*m_local;
	}

	__forceinline c_local_player::operator c_player* ( ) const {
		return *m_local;
	}

	__forceinline c_player* c_local_player::operator ->( ) const {
		return *m_local;
	}
}