#pragma once

namespace nem::hacks {
	struct extrapolation_data_t {
		__forceinline constexpr extrapolation_data_t( ) = default;

		__forceinline extrapolation_data_t(
			valve::c_player* const player, const lag_record_t* const lag_record
		) : m_player{ player }, m_sim_time{ lag_record->m_sim_time }, m_flags{ lag_record->m_flags },
			 m_origin{ lag_record->m_origin },
			m_velocity{ lag_record->m_velocity }, m_obb_min{ lag_record->m_obb_min }, m_obb_max{ lag_record->m_obb_max } { m_on_ground = lag_record->m_flags & valve::e_ent_flags::on_ground; }

		valve::c_player*	m_player{};

		float				m_sim_time{};

		valve::e_ent_flags	m_flags{};
		bool				m_on_ground{};

		float               m_change {}, m_dir {};

		vec3_t				m_origin{}, m_velocity{},
							m_obb_min{}, m_obb_max{};
	};

	struct aim_point_t {
		__forceinline constexpr aim_point_t( ) = default;

		__forceinline constexpr aim_point_t(
			const vec3_t& pos, const bool center,
			const int hitbox, const int hitgroup,
			const int needed_intersections
		) : m_pos{ pos }, m_center{ center },
			m_hitbox{ hitbox }, m_hitgroup{ hitgroup },
			m_needed_intersections{ needed_intersections } {}

		vec3_t		m_pos{};
		pen_data_t	m_pen_data{};
		bool		m_center{}, m_valid{}, m_safe_head{ };
		int			m_intersections{},
					m_hitbox{}, m_hitgroup{},
			        m_head_intersections{},
					m_needed_intersections{};
	};

	struct aim_record_t {
		player_entry_t*					m_entry{};
		std::shared_ptr< lag_record_t > m_lag_record{};
	};

	struct aim_target_t {
		__forceinline constexpr aim_target_t( ) = default;

		__forceinline aim_target_t( const aim_record_t& record )
			: m_entry{ record.m_entry }, m_lag_record{ record.m_lag_record } {}

		player_entry_t*					m_entry{};
		std::shared_ptr< lag_record_t >	m_lag_record{};

		std::vector< aim_point_t >		m_points{};
		aim_point_t*					m_best_point{};
		aim_point_t*					m_best_body_point{};
	};

	struct aim_last_target_t {
		player_entry_t*					m_entry{};
		std::shared_ptr< lag_record_t > m_lag_record{};

		int								m_hitbox{}, m_dmg{},
										m_hit_chance{};

		aim_point_t						m_point{};
	};

	class c_hitbox_ {
	public:
		int m_hitbox_id = 0;
		bool m_is_oob;
		vec3_t m_mins;
		vec3_t m_maxs;
		vec3_t m_start_scaled;
		float m_radius;
		valve::studio_hitbox_t* m_hitbox;
		int m_bone;
		int m_hitgroup;
	};


	class c_aim_bot {
	private:
		std::size_t calc_points_count( const int hitgroups, const int multi_points ) const;
	public:
		void scan_points (
			aim_target_t& target, const int hitgroups, const int multi_points, const bool trace, vec3_t shoot_pos = g_context->shoot_pos ( )
		) const;
		void scan_point( const aim_target_t& target, aim_point_t& point, const bool ignore_dmg, vec3_t shoot_pos = g_context->shoot_pos ( ) ) const;
		void scan_center_points ( aim_target_t& target, const int hitgroups, vec3_t shoot_pos ) const;
		bool select_points ( aim_target_t& target, bool additional_scan ) const;
	private:
	
		void calc_capsule_points(
			aim_target_t& target, const valve::studio_hitbox_t* const hitbox,
			const int index, const mat3x4_t& matrix, float scale
		) const;

		float calc_point_scale(
			const float spread, const float max,
			const float dist, const vec3_t& dir,
			const vec3_t& right, const vec3_t& up
		) const;

		void calc_multi_points(
			aim_target_t& target, const valve::studio_hitbox_t* const hitbox,
			const int index, const mat3x4_t& matrix, float scale
		) const;

		void player_move( extrapolation_data_t& data ) const;

		float get_hit_chance_to_set_up();

		int get_sort_type_for_set_up();

		std::optional< aim_record_t > extrapolate( const player_entry_t& entry ) const;

		std::optional< aim_record_t > select_latest_record( const player_entry_t& entry ) const;

		std::optional< aim_record_t > select_record( player_entry_t& entry ) const;

		aim_point_t* select_point( const aim_target_t& target, const int cmd_number ) const;

		void find_targets( );

		void scan_targets( );

		aim_target_t* select_target( const int cmd_number );

        struct cfg_t {

            bool m_bEnableRagebot = false;
            bool m_bRemoveRecoil = false;
            bool m_bAutoScope = false;
            bool m_bMultiThread = false;
            bool m_bDoubleTap = false;
            bool m_bHideShots = false;
            bool m_bOverrideMinDamage = false;
            bool m_bForceSafePoints = false;

            int m_iMinDamage_Pistols = 0;
            int m_iHitchance_Pistols = 0;
            bool m_bStaticPoints_Pistols = false;
            int m_iMinDamageOv_Pistols = 0;
            int m_iHeadScale_Pistols = 0;
            int m_iBodyScale_Pistols = 0;
            int m_iHitboxes_Pistols = 0;
            int m_iMultipoints_Pistols = 0;

            int m_iMinDamage_DeagleR8 = 0;
            int m_iHitchance_DeagleR8 = 0;
            bool m_bStaticPoints_DeagleR8 = false;
            int m_iMinDamageOv_DeagleR8 = 0;
            int m_iHeadScale_DeagleR8 = 0;
            int m_iBodyScale_DeagleR8 = 0;
            int m_iHitboxes_DeagleR8 = 0;
            int m_iMultipoints_DeagleR8 = 0;

            int m_iMinDamage_Rifles = 0;
            int m_iHitchance_Rifles = 0;
            bool m_bStaticPoints_Rifles = false;
            int m_iMinDamageOv_Rifles = 0;
            int m_iHeadScale_Rifles = 0;
            int m_iBodyScale_Rifles = 0;
            int m_iHitboxes_Rifles = 0;
            int m_iMultipoints_Rifles = 0;

            int m_iMinDamage_Scar = 0;
            int m_iHitchance_Scar = 0;
            bool m_bStaticPoints_Scar = false;
            int m_iMinDamageOv_Scar = 0;
            int m_iHeadScale_Scar = 0;
            int m_iBodyScale_Scar = 0;
            int m_iHitboxes_Scar = 0;
            int m_iMultipoints_Scar = 0;

            int m_iMinDamage_SSG08 = 0;
            int m_iHitchance_SSG08 = 0;
            bool m_bStaticPoints_SSG08 = false;
            int m_iMinDamageOv_SSG08 = 0;
            int m_iHeadScale_SSG08 = 0;
            int m_iBodyScale_SSG08 = 0;
            int m_iHitboxes_SSG08 = 0;
            int m_iMultipoints_SSG08 = 0;

            int m_iMinDamage_AWP = 0;
            int m_iHitchance_AWP = 0;
            bool m_bStaticPoints_AWP = false;
            int m_iMinDamageOv_AWP = 0;
            int m_iHeadScale_AWP = 0;
            int m_iBodyScale_AWP = 0;
            int m_iHitboxes_AWP = 0;
            int m_iMultipoints_AWP = 0;

            // Anti-Aims
            bool m_bAntiAimEnabled = false;
            int m_iPitch = 0;
            int m_iYaw = 0;
            bool m_bJitterSide = false;
            bool m_bAtTarget = false;
            int m_iJitterAmount = 0;
            bool m_bDesync = false;
            bool m_bInverter = false;
            int m_iDesyncDelta = 0;
            bool m_bRollEnabled = false;
            int m_iRollLimit = 0;
            bool m_bAutoDirection = false;
            bool m_bFakeLagEnabled = false;
            int m_iFakeLagsTriggers = 0;
            int m_iFakeLagsLimit = 0;
            bool m_bSlowWalk = false;
            bool m_bFakeDuck = false;

            // Players
            bool m_bName = false;
            bool m_bHealth = false;
            bool m_bBox = false;
            bool m_bFlags = false;
            bool m_bHitMarker = false;
            bool m_bWeaponIcon = false;
            bool m_bWeaponText = false;
            bool m_bAmmo = false;
            bool m_bSkeleton = false;
            bool m_bOOFArrows = false;
            bool m_bGlow = false;
            float m_flGlowColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            bool m_bPlayerChams = false;
            float m_flPlayerChamsColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            bool m_bInvisiblePlayerChams = false;
            float m_flInvisiblePlayerChamsColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            int m_iChamsType = 0;

            bool m_bGlowOutline = false;
            float m_flGlowOutlineColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            bool m_bInvisibleGlowOutline = false;
            float m_flInvisibleGlowOutlineColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            bool m_bHistory = false;
            float m_flHistoryColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            int m_iHistoryChamsType = 0;

            bool m_bLocalPlayerChams = false;
            float m_flLocalPlayerChamsColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            int m_iLocalPlayerChamsType = 0;

            bool m_bLocalPlayerGlowOutline = false;
            float m_flLocalPlayerGlowOutlineColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            bool m_bLocalPlayerFake = false;
            float m_flLocalPlayerFakeColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            bool m_bLocalPlayerFakeGlowOutline = false;
            float m_flLocalPlayerFakeGlowOutlineColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            int m_iLocalFakeType = 0;

            bool m_bHandChams = false;
            float m_flHandChamsColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            int m_iHandsChamsType = 0;

            bool m_bHandGlowOutlineChams = false;
            float m_flHandGlowOutlineChamsColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            bool m_bLocalWeaponChams = false;
            float m_flLocalWeaponChamsColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            int m_iLocalWeaponChamsType = 0;

            bool m_bLocalWeaponGlowOutlineChams = false;
            float m_flLocalWeaponGlowOutlineChamsColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            bool m_bWeaponChams = false;
            float m_flWeaponChamsColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            int m_iWeaponChamsType = 0;

            bool m_bWeaponGlowOutlineChams = false;
            float m_flWeaponGlowOutlineChamsColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            bool m_bShotRecord = false;
            float m_flShotRecordColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            int m_iShotRecordType = 0;

            bool m_bShotRecordGlowOutline = false;
            float m_flShotRecordGlowOutlineColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            bool m_bRagdolls = false;
            float m_flRagdollsColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            int m_iRagdollsType = 0;

            bool m_bGrenades = false;
            bool m_bGrenadePrediction = false;
            bool m_bGrenadeProximityWarning = false;

            int m_iDroppedWeapon = 0;
            int m_iWorldType = 0;
            int m_iSkyboxType = 0;

            bool m_bShadowsModulation = false;

            int m_iShadowsX = 0;
            int m_iShadowsY = 0;
            int m_iShadowsZ = 0;

            bool m_bToneMapModulation = false;

            int m_iBloom = 0;
            int m_iExposure = 0;

            bool m_bFog = false;
            float m_flFogColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            int m_iFogStart = 0;
            int m_iFogEnd = 0;
            int m_iDensity = 0;

            bool m_bBulletTracers = false;
            bool m_bBulletImpacts = false;
            bool m_bDamageMarker = false;
            bool m_bRemoveVisualRecoil = false;

            bool m_bRemoveFlash = false;
            bool m_bRemoveSmoke = false;
            bool m_bRemoveFog = false;
            bool m_bDisablePostProcessing = false;
            bool m_bRemoveScope = false;
            bool m_bRemoveShadows = false;
            bool m_bRemoveLandingBob = false;
            bool m_bRemoveHandsShaking = false;

            int m_iPropTransparency = 100;
            int m_iWallTransparency = 100;

            bool m_bThirdPerson = false;
            bool m_bThirdPersonDead = false;
            int m_iThirdPersonDistance = 60;
            int m_iCameraDistance = 90;
            bool m_bOvInScope = false;
            bool m_bAspectRatio = false;
            int m_iAspectRatioValue = 10;

            bool m_bBunnyHop = false;

            int m_iAutoStrafeType = 0;
            bool m_bAutoPeek = false;
            bool m_bInfinityDuck = false;
            bool m_bSlideWalk = false;
            bool m_bFastStop = false;
            bool m_bDisableRagdollPhysics = false;

            bool m_bHitsound = false;
            bool m_bSpectators = false;
            bool m_bKeybinds = false;
            bool m_bPreserveKillfeed = false;
            bool m_bBuyBot = false;

            int m_iBuyBotSnipers = 0;
            int m_iBuyBotPistols = 0;
            int m_iBuyBotAdditional = 0;

            int m_iChoosedConfig = 0;

            bool m_bBlend = false;
            int m_iBlendValue = 0;

            float m_flNightModeColor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            int knife_index = 0;
            std::size_t paint_kits[ 35 ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

            int m_iTAgent = 0;
            int m_iCTAgent = 0;

            bool m_bRain = false;
            bool m_bMotionBlur = false;
            bool m_bC4Indicator = false;

            ImVec2 m_vecBombIndPos = ImVec2( 100, 100 );

            int m_iStopTypePistols{};
            int m_iStopTypeDeagleR8{};
            int m_iStopTypeRifles{};
            int m_iStopTypeScar{};
            int m_iStopTypeSSG08{};
            int m_iStopTypeAwp{};
            bool m_bPredAutoStopPistols{};
            bool m_bPredAutoStopR8{};
            bool m_bPredAutoStopRifles{};
            bool m_bPredAutoStopScar{};
            bool m_bPredAutoStopSSG08{};
            bool m_bPredAutoStopAwp{};
        };

        sdk::cfg_var_t< cfg_t > m_cfg{ 0x05562b71u, {} };

	public:
		vec2_t calc_spread_angle(
			const int bullets, const valve::e_item_index item_index,
			const float recoil_index, const std::size_t i
		) const;

		void get_hitbox_data ( c_hitbox_* rtn, valve::c_player* ent, int ihitbox, valve::bones_t& matrix );

        __forceinline cfg_t& cfg( ) { return m_cfg.value( ); };

	private:

		int calc_hit_chance(
			const aim_target_t* const target, const qangle_t& angle, const aim_point_t* const aim_point
		);

		bool new_hit_chance ( qangle_t angle, valve::c_player* player, int hitbox, float chance, valve::bones_t* bones );

		void parse_auto_wall_side ( );

		std::array< std::vector< int >, 6u >	m_hitgroups{
			std::vector< int >{ 0 },
			{ 2, 4, 5, 6 },
			{ 3 },
			{ 13, 14, 15, 16, 17, 18 },
			{ 7, 8, 9, 10 },
			{ 11, 12 }
		};

		aim_last_target_t						m_last_target{};

		std::vector< aim_target_t >				m_targets{};
	public:
		void on_create_move( valve::user_cmd_t& user_cmd, bool& send_packet );

		__forceinline const aim_last_target_t& last_target( ) const;
	};

	inline const auto g_aim_bot = std::make_unique< c_aim_bot >( );
}

#include "impl/aim_bot.inl"