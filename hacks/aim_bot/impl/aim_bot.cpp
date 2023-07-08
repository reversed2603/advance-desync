#include "../../../nem.hpp"

namespace nem::hacks {

	int get_baim_conditions()
	{
		return 0;
	}

	std::size_t c_aim_bot::calc_points_count( const int hitgroups, const int multi_points ) const {
		std::size_t ret{};

		for ( std::size_t i{}, fl = 1; i < m_hitgroups.size( ); ++i, fl <<= 1 ) {
			if ( hitgroups & fl
				|| ( i == 2 && get_baim_conditions ( ) ) ) {
				if ( !( multi_points & fl ) )
					++ret;
				else {
					for ( auto& hitbox : m_hitgroups.at( i ) ) {
						if ( hitbox == 11 || hitbox == 12 )
							ret += 3u;
						else if ( hitbox ) {
							if ( hitbox == crypt_int ( 3 ) || hitbox == 2 )
								ret += 4u;
							else if ( hitbox == crypt_int ( 6 ) || hitbox == 4 || hitbox == 5 )
								ret += 2u;
							else
								ret += 1u;
						}
						else
							ret += 6u;
					}
				}
			}
		}

		return ret;
	}
	
	int get_stop_type( ) {
		if ( !valve___g_local_player || !valve___g_local_player->alive( ) )
			return 0;

		auto wpn = valve___g_local_player->weapon( );

		if ( !wpn )
			return 0;

		switch ( wpn->item_index( ) )
		{
		case valve::e_item_index::awp:
			return hacks::g_aim_bot->cfg ( ).m_iStopTypeAwp;
		case valve::e_item_index::ssg08:
			return hacks::g_aim_bot->cfg ( ).m_iStopTypeSSG08;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return hacks::g_aim_bot->cfg ( ).m_iStopTypeScar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a1:
		case valve::e_item_index::m4a1_silencer:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawedoff:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return hacks::g_aim_bot->cfg ( ).m_iStopTypeRifles;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return hacks::g_aim_bot->cfg ( ).m_iStopTypeDeagleR8;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::fiveseven:
		case valve::e_item_index::hkp2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usp_silencer:
			return hacks::g_aim_bot->cfg ( ).m_iStopTypePistols;
		default:
			return 0;
		}

		return 0;
	}

	int get_min_dmg_to_set_up()
	{
		if (!valve___g_local_player || !valve___g_local_player->alive())
			return 0;

		auto wpn = valve___g_local_player->weapon();

		if (!wpn)
			return 0;

		switch (wpn->item_index())
		{
		case valve::e_item_index::awp:
			return hacks::g_aim_bot->cfg ( ).m_iMinDamage_AWP;
		case valve::e_item_index::ssg08:
			return hacks::g_aim_bot->cfg ( ).m_iMinDamage_SSG08;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return hacks::g_aim_bot->cfg ( ).m_iMinDamage_Scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a1:
		case valve::e_item_index::m4a1_silencer:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawedoff:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return hacks::g_aim_bot->cfg ( ).m_iMinDamage_Rifles;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return hacks::g_aim_bot->cfg ( ).m_iMinDamage_DeagleR8;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::fiveseven:
		case valve::e_item_index::hkp2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usp_silencer:
			return hacks::g_aim_bot->cfg ( ).m_iMinDamage_Pistols;
		default:
			return 0;
		}

		return 0;
	}

	
	int get_hitgroups_to_set()
	{
		if (!valve___g_local_player || !valve___g_local_player->alive())
			return 0;

		auto wpn = valve___g_local_player->weapon();

		if (!wpn)
			return 0;

		switch (wpn->item_index())
		{
		case valve::e_item_index::awp:
			return hacks::g_aim_bot->cfg ( ).m_iHitboxes_AWP;
		case valve::e_item_index::ssg08:
			return hacks::g_aim_bot->cfg ( ).m_iHitboxes_SSG08;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return hacks::g_aim_bot->cfg ( ).m_iHitboxes_Scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a1:
		case valve::e_item_index::m4a1_silencer:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawedoff:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return hacks::g_aim_bot->cfg ( ).m_iHitboxes_Rifles;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return hacks::g_aim_bot->cfg ( ).m_iHitboxes_DeagleR8;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::fiveseven:
		case valve::e_item_index::hkp2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usp_silencer:
			return hacks::g_aim_bot->cfg ( ).m_iHitboxes_Pistols;
		default:
			return 0;
		}

		return 0;
	}

	int get_head_scale()
	{
		if (!valve___g_local_player || !valve___g_local_player->alive())
			return 0;

		auto wpn = valve___g_local_player->weapon();

		if (!wpn)
			return 0;

		switch (wpn->item_index())
		{
		case valve::e_item_index::awp:
			return hacks::g_aim_bot->cfg ( ).m_iHeadScale_AWP;
		case valve::e_item_index::ssg08:
			return hacks::g_aim_bot->cfg ( ).m_iHeadScale_SSG08;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return hacks::g_aim_bot->cfg ( ).m_iHeadScale_Scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a1:
		case valve::e_item_index::m4a1_silencer:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawedoff:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return hacks::g_aim_bot->cfg ( ).m_iHeadScale_Rifles;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return hacks::g_aim_bot->cfg ( ).m_iHeadScale_DeagleR8;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::fiveseven:
		case valve::e_item_index::hkp2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usp_silencer:
			return hacks::g_aim_bot->cfg ( ).m_iHeadScale_Pistols;
		default:
			return 0;
		}

		return 0;
	}

	int get_body_scale()
	{
		if (!valve___g_local_player || !valve___g_local_player->alive())
			return 0;

		auto wpn = valve___g_local_player->weapon();

		if (!wpn)
			return 0;

		switch (wpn->item_index())
		{
		case valve::e_item_index::awp:
			return hacks::g_aim_bot->cfg ( ).m_iBodyScale_AWP;
		case valve::e_item_index::ssg08:
			return hacks::g_aim_bot->cfg ( ).m_iBodyScale_SSG08;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return hacks::g_aim_bot->cfg ( ).m_iBodyScale_Scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a1:
		case valve::e_item_index::m4a1_silencer:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawedoff:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return hacks::g_aim_bot->cfg ( ).m_iBodyScale_Rifles;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return hacks::g_aim_bot->cfg ( ).m_iBodyScale_DeagleR8;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::fiveseven:
		case valve::e_item_index::hkp2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usp_silencer:
			return hacks::g_aim_bot->cfg ( ).m_iBodyScale_Pistols;
		default:
			return 0;
		}

		return 0;
	}

	int get_multi_points_to_set()
	{
		if (!valve___g_local_player || !valve___g_local_player->alive())
			return 0;

		auto wpn = valve___g_local_player->weapon();

		if (!wpn)
			return 0;

		switch (wpn->item_index())
		{
		case valve::e_item_index::awp:
			return hacks::g_aim_bot->cfg ( ).m_iMultipoints_AWP;
		case valve::e_item_index::ssg08:
			return hacks::g_aim_bot->cfg ( ).m_iMultipoints_SSG08;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return hacks::g_aim_bot->cfg ( ).m_iMultipoints_Scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a1:
		case valve::e_item_index::m4a1_silencer:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawedoff:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return hacks::g_aim_bot->cfg ( ).m_iMultipoints_Rifles;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return hacks::g_aim_bot->cfg ( ).m_iMultipoints_DeagleR8;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::fiveseven:
		case valve::e_item_index::hkp2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usp_silencer:
			return hacks::g_aim_bot->cfg ( ).m_iMultipoints_Pistols;
		default:
			return 0;
		}

		return 0;
	}

	bool should_static_point_scale()
	{
		if (!valve___g_local_player || !valve___g_local_player->alive())
			return false;

		auto wpn = valve___g_local_player->weapon();

		if (!wpn)
			return false;

		switch (wpn->item_index())
		{
		case valve::e_item_index::awp:
			return hacks::g_aim_bot->cfg ( ).m_bStaticPoints_AWP;
		case valve::e_item_index::ssg08:
			return hacks::g_aim_bot->cfg ( ).m_bStaticPoints_SSG08;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return hacks::g_aim_bot->cfg ( ).m_bStaticPoints_Scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a1:
		case valve::e_item_index::m4a1_silencer:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawedoff:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return hacks::g_aim_bot->cfg ( ).m_bStaticPoints_Rifles;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return hacks::g_aim_bot->cfg ( ).m_bStaticPoints_DeagleR8;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::fiveseven:
		case valve::e_item_index::hkp2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usp_silencer:
			return hacks::g_aim_bot->cfg ( ).m_bStaticPoints_Pistols;
		default:
			return false;
		}

		return false;
	}

	void c_aim_bot::scan_point ( const aim_target_t& target, aim_point_t& point, const bool ignore_dmg, vec3_t shoot_pos ) const {
		const auto hitbox_set = target.m_entry->m_player->mdl_data ( )->m_studio_hdr->hitbox_set ( target.m_entry->m_player->hitbox_set_index ( ) );

		point.m_pen_data = g_auto_wall->fire_bullet (
			valve___g_local_player, target.m_entry->m_player, g_context->wpn_data ( ),
			g_context->weapon ( )->item_index ( ) == valve::e_item_index::taser, shoot_pos, point.m_pos
		);

		if ( point.m_pen_data.m_dmg < 1 )
			return;

		const auto& pen = point.m_pen_data;

		if ( !ignore_dmg ) {
			auto min_dmg = get_min_dmg_to_set_up ( ) + 1;
			
			if ( pen.m_dmg < target.m_entry->m_player->health ( ) )
			if ( pen.m_dmg < min_dmg )
				return;
		}

		if ( point.m_needed_intersections <= 0
			|| !target.m_lag_record->m_trying_to_resolve ) {
			point.m_valid = true;
			point.m_intersections = 3;

			return;
		}


		const auto intersect = [ ] (
			const vec3_t& shoot_pos, const vec3_t& point,
			const valve::studio_hitbox_t* const hitbox, const mat3x4_t& matrix
		) {
			vec3_t min{}, max{};

			math::vector_transform( hitbox->m_min, matrix, min );
			math::vector_transform( hitbox->m_max, matrix, max );

			if ( hitbox->m_radius != 1.f )
				return math::segment_to_segment( shoot_pos, point, min, max ) < hitbox->m_radius;

			math::vector_i_transform( shoot_pos, matrix, min );
			math::vector_i_transform( point, matrix, max );

			return math::line_vs_bb( min, max, hitbox->m_min, hitbox->m_max );
		};

		point.m_intersections = 1;
		point.m_head_intersections = 1;

		auto second_bones = target.m_lag_record->m_anim_sides.at ( 2 ).m_bones;
		auto first_bones_ = target.m_lag_record->m_anim_sides.at ( 1 ).m_bones;

		const auto& first_bones = target.m_lag_record->m_anim_sides.at( target.m_lag_record->m_anim_side ).m_bones;

		for (int i{}; i < hitbox_set->m_hitboxes_count; ++i) {
			const auto hitbox = hitbox_set->hitbox( i );

			if ( !hitbox
				|| !intersect ( shoot_pos, point.m_pos, hitbox, first_bones[ hitbox->m_bone ] ) || !intersect ( shoot_pos, point.m_pos, hitbox, target.m_lag_record->m_anim_sides.at ( 0 ).m_bones[ hitbox->m_bone ] ) )
				continue;

			point.m_intersections = crypt_int ( 2 );

			break;
		}

		for ( int i {}; i < hitbox_set->m_hitboxes_count; ++i ) {
			const auto hitbox = hitbox_set->hitbox ( i );
			if ( !hitbox
				|| !intersect ( shoot_pos, point.m_pos, hitbox, second_bones [ hitbox->m_bone ] ) )
				continue;

			++point.m_intersections;

			break;
		}


		point.m_valid = point.m_intersections >= point.m_needed_intersections;

	}

	void c_aim_bot::scan_center_points( aim_target_t& target, const int hitgroups, vec3_t shoot_pos ) const {
		const auto& anim_side = target.m_lag_record->m_anim_sides.at( target.m_lag_record->m_anim_side );

		const auto hitbox_set = target.m_entry->m_player->mdl_data( )->m_studio_hdr->hitbox_set( target.m_entry->m_player->hitbox_set_index( ) );

		const auto points_count = calc_points_count( hitgroups, 0 );
		
		target.m_points.reserve( points_count );

		for ( std::size_t i{}; i < m_hitgroups.size( ); ++i ) {
			if ( !( ( 1 << i ) & hitgroups ) )
				continue;

			for ( const auto& index : m_hitgroups.at( i ) ) {
				const auto hitbox = hitbox_set->hitbox( index );
				if ( !hitbox )
					continue;

				vec3_t point{};

				math::vector_transform(
					( hitbox->m_min + hitbox->m_max ) / 2.f,
					anim_side.m_bones[ hitbox->m_bone ], point
				);

				target.m_points.emplace_back( std::move( point ), true, index, hitbox->m_group, 0 );
			}
		}

		target.m_lag_record->restore( target.m_entry->m_player, target.m_lag_record->m_anim_side );

		for ( auto& point : target.m_points )
			scan_point( target, point, true, shoot_pos );
	}

	void c_aim_bot::calc_capsule_points(
		aim_target_t& target, const valve::studio_hitbox_t* const hitbox,
		const int index, const mat3x4_t& matrix, float scale 
	) const {
		vec3_t min{}, max{};

		math::vector_transform( hitbox->m_min, matrix, min );
		math::vector_transform( hitbox->m_max, matrix, max );

		static auto matrix0 = math::vector_matrix( { 0.f, 0.f, 1.f } );

		auto matrix1 = math::vector_matrix( ( max - min ).normalized( ) );

		for ( const auto& vertices : {
				vec3_t{ 0.95f, 0.f, 0.f },
				vec3_t{ -0.95f, 0.f, 0.f },
				vec3_t{ 0.f, 0.95f, 0.f },
				vec3_t{ 0.f, -0.95f, 0.f },
				vec3_t{ 0.f, 0.f, 0.95f },
				vec3_t{ 0.f, 0.f, -0.95f }
			} ) {
			vec3_t point{};

			math::vector_rotate( vertices, matrix0, point );
			math::vector_rotate( point, matrix1, point );

			point *= scale;

			if ( vertices.z > 0.f )
				point += min - max;

			target.m_points.emplace_back( point + max, false, index, hitbox->m_group, 0 );
		}
	}

	float c_aim_bot::calc_point_scale(
		const float spread, const float max,
		const float dist, const vec3_t& dir,
		const vec3_t& right, const vec3_t& up
	) const {
		const auto v1 = g_eng_pred->inaccuracy( ) + spread;

		auto v28 = right * v1 + dir + up * v1;

		v28.normalize( );

		const auto delta = math::angle_diff( math::to_deg( std::atan2( dir.y, dir.x ) ), math::to_deg( std::atan2( v28.y, v28.x ) ) );

		const auto v23 = max + dist / std::tan( math::to_rad( crypt_float ( 180.f ) - ( delta + 90.f ) ) );
		if ( v23 > max )
			return 1.f;

		float v25{};
		if ( v23 >= 0.f )
			v25 = v23;

		return v25 / max;
	}

	void c_aim_bot::calc_multi_points(
		aim_target_t& target, const valve::studio_hitbox_t* const hitbox,
		const int index, const mat3x4_t& matrix, float scale
	) const {
		vec3_t point{};

		const auto center = ( hitbox->m_min + hitbox->m_max ) / crypt_float ( 2.f );

		math::vector_transform( center, matrix, point );

		target.m_points.emplace_back( point, true, index, hitbox->m_group, 0 );

		if ( !should_static_point_scale ( ) ) {
			const auto max = ( hitbox->m_max - hitbox->m_min ).length( ) * crypt_float ( 0.5f ) + hitbox->m_radius;

			auto dir = ( point - g_context->shoot_pos( ) );

			const auto dist = dir.normalize( );

			vec3_t right{}, up{};

			if ( dir.x == 0.f
				&& dir.y == 0.f ) {
				right = { 0.f, -1.f, 0.f };
				up = { -dir.z, 0.f, 0.f };
			}
			else {
				right = dir.cross( { 0.f, 0.f, 1.f } ).normalized( );
				up = right.cross( dir ).normalized( );
			}

			scale = calc_point_scale( g_eng_pred->spread( ), max, dist, dir, right, up );
			if ( scale <= 0.f
				&& g_eng_pred->spread( ) > g_eng_pred->min_inaccuracy( ) )
				scale = calc_point_scale( g_eng_pred->min_inaccuracy( ), max, dist, dir, right, up );
		}

		if ( scale <= 0.f )
			return;

		if ( hitbox->m_radius <= crypt_float ( 0.f ) ) {
			point = { center.x + ( hitbox->m_min.x - center.x ) * scale, center.y, center.z };

			math::vector_transform( point, matrix, point );

			target.m_points.emplace_back( point, false, index, hitbox->m_group, 0 );

			point = { center.x + ( hitbox->m_max.x - center.x ) * scale, center.y, center.z };

			math::vector_transform( point, matrix, point );

			target.m_points.emplace_back( point, false, index, hitbox->m_group, 0 );

			return;
		}

		if ( index ) {
			if ( index == 3 ) {
				if ( !should_static_point_scale ( ) && scale > crypt_float ( 0.9f ) )
					scale = 0.9f;
			}
			else {
				if ( index != 2
					&& index != 6 ) {
					if ( index == 4
						|| index == 5 ) {
						if ( !should_static_point_scale ( ) && scale > crypt_float ( 0.9f ) )
							scale = crypt_float ( 0.9f );

						point = { center.x, hitbox->m_max.y - hitbox->m_radius * scale, center.z };

						math::vector_transform( point, matrix, point );

						target.m_points.emplace_back( point, false, index, hitbox->m_group, 0 );
					}

					return;
				}

				if ( !should_static_point_scale ( ) && scale > crypt_float ( 0.9f ) )
					scale = crypt_float ( 0.9f );

				if ( index == 6 ) {
					point = { center.x, hitbox->m_max.y - hitbox->m_radius * scale, center.z };

					math::vector_transform( point, matrix, point );

					target.m_points.emplace_back( point, false, index, hitbox->m_group, 0 );

					return;
				}
			}

			return calc_capsule_points( target, hitbox, index, matrix, scale );
		}

		point = { hitbox->m_max.x + 0.70710678f * ( hitbox->m_radius * scale ), hitbox->m_max.y - 0.70710678f * ( hitbox->m_radius * scale ), hitbox->m_max.z };

		math::vector_transform ( point, matrix, point );

		target.m_points.emplace_back ( point, false, index, hitbox->m_group, 0 );

		point = { hitbox->m_max.x, hitbox->m_max.y, hitbox->m_max.z + hitbox->m_radius * scale };

		math::vector_transform ( point, matrix, point );

		target.m_points.emplace_back ( point, false, index, hitbox->m_group, 0 );

		point = { hitbox->m_max.x, hitbox->m_max.y, hitbox->m_max.z - hitbox->m_radius * scale };

		math::vector_transform ( point, matrix, point );

		target.m_points.emplace_back ( point, false, index, hitbox->m_group, 0 );

		point = { hitbox->m_max.x, hitbox->m_max.y - hitbox->m_radius * scale, hitbox->m_max.z };

		math::vector_transform ( point, matrix, point );

		target.m_points.emplace_back ( point, false, index, hitbox->m_group, 0 );

		if ( target.m_entry->m_player->anim_state ( ) && target.m_lag_record->m_velocity.length_2d ( ) <= 0.1f && target.m_lag_record->m_eye_angles.x <= target.m_entry->m_player->anim_state ( )->m_max_body_pitch )
		{
			point = { hitbox->m_max.x - hitbox->m_radius * scale, hitbox->m_max.y, hitbox->m_max.z };

			math::vector_transform ( point, matrix, point );

			target.m_points.emplace_back ( point, false, index, hitbox->m_group, 0 );
		}
	}

	void c_aim_bot::scan_points(
		aim_target_t& target, const int hitgroups, const int multi_points, const bool trace, vec3_t shoot_pos
	) const {
		const auto hp = target.m_entry->m_player->health( );

		const auto& anim_side = target.m_lag_record->m_anim_sides.at( target.m_lag_record->m_anim_side );

		const auto hitbox_set = target.m_entry->m_player->mdl_data( )->m_studio_hdr->hitbox_set( target.m_entry->m_player->hitbox_set_index( ) );

		const auto points_count = calc_points_count( hitgroups, multi_points );
		static auto cl_lagcompensation = valve___g_cvar->find_var ( xorstr_ ( "cl_lagcompensation" ) );
		target.m_points.reserve( points_count );

		const auto force_safe_point = hacks::g_aim_bot->cfg ( ).m_bForceSafePoints;

		const auto is_taser = g_context->weapon( )->item_index( ) == valve::e_item_index::taser;
		const auto is_ssg08 = g_context->weapon( )->item_index( ) == valve::e_item_index::ssg08;

		const auto target_standing = target.m_lag_record->m_velocity.length( ) <= 5.f;

		for ( std::size_t i{}; i < m_hitgroups.size( ); ++i ) {

            if ( !( ( 1 << i ) & hitgroups )
				&& ( i != 2 ) )
				continue;

			if ( ( ( 1 << i ) & multi_points ) ) {
				const auto scale = (i ? get_body_scale() : get_head_scale()) / 100.f;

				for ( const auto& index : m_hitgroups.at( i ) ) {
					const auto hitbox = hitbox_set->hitbox( index );
					if ( !hitbox )
						continue;

					calc_multi_points( target, hitbox, index, anim_side.m_bones[ hitbox->m_bone ], scale );
				}
			}
			else {
				for ( const auto& index : m_hitgroups.at( i ) ) {
					const auto hitbox = hitbox_set->hitbox( index );
					if ( !hitbox )
						continue;

					vec3_t point{};

					math::vector_transform(
						( hitbox->m_min + hitbox->m_max ) / 2.f,
						anim_side.m_bones[ hitbox->m_bone ], point
					);

					target.m_points.emplace_back( std::move( point ), true, index, hitbox->m_group, 0 );
				}
			}
		}

		for ( auto& point : target.m_points ) {

			if ( is_taser
				|| force_safe_point
				|| target.m_lag_record->m_force_safe_points ) {
				point.m_needed_intersections = 3;

				continue;
			}

			if ( point.m_hitbox != 0 ) {
				if ( static_cast< std::size_t >( point.m_hitbox - 7 ) <= 5 )
					point.m_needed_intersections = 2;
			}
			else if ( !is_ssg08
				&& target.m_lag_record->m_priority > 3 )
				point.m_needed_intersections = 2;

			if ( target.m_lag_record->m_reforce_safe_points )
				point.m_needed_intersections = 2;
		}

		if ( !trace )
			return;

		lag_backup_t lag_backup{ target.m_entry->m_player };

		target.m_lag_record->restore( target.m_entry->m_player, target.m_lag_record->m_anim_side );

		for ( auto& point : target.m_points )
			scan_point( target, point, false, shoot_pos );

		lag_backup.restore( target.m_entry->m_player );
	}

	float c_aim_bot::get_hit_chance_to_set_up()
	{
		if (!valve___g_local_player || !valve___g_local_player->alive())
			return 0.f;

		auto wpn = valve___g_local_player->weapon();

		if (!wpn)
			return 0.f;

		switch (wpn->item_index())
		{
		case valve::e_item_index::awp:
			return ( float ) hacks::g_aim_bot->cfg ( ).m_iHitchance_AWP;
		case valve::e_item_index::ssg08:
			return (float) hacks::g_aim_bot->cfg ( ).m_iHitchance_SSG08;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return (float) hacks::g_aim_bot->cfg ( ).m_iHitchance_Scar;
		case valve::e_item_index::ak47:
		case valve::e_item_index::aug:
		case valve::e_item_index::bizon:
		case valve::e_item_index::famas:
		case valve::e_item_index::galil:
		case valve::e_item_index::m249:
		case valve::e_item_index::m4a1:
		case valve::e_item_index::m4a1_silencer:
		case valve::e_item_index::mac10:
		case valve::e_item_index::mag7:
		case valve::e_item_index::mp5sd:
		case valve::e_item_index::mp7:
		case valve::e_item_index::mp9:
		case valve::e_item_index::negev:
		case valve::e_item_index::nova:
		case valve::e_item_index::sawedoff:
		case valve::e_item_index::sg553:
		case valve::e_item_index::ump45:
		case valve::e_item_index::xm1014:
		case valve::e_item_index::p90:
			return (float) hacks::g_aim_bot->cfg ( ).m_iHitchance_Rifles;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return (float) hacks::g_aim_bot->cfg ( ).m_iHitchance_DeagleR8;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::fiveseven:
		case valve::e_item_index::hkp2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usp_silencer:
			return (float) hacks::g_aim_bot->cfg ( ).m_iHitchance_Pistols;
		default:
			return 0.f;
		}

		return 0.f;
	}

	void c_aim_bot::player_move( extrapolation_data_t& data ) const {
		if ( !( data.m_flags & valve::e_ent_flags::on_ground ) ) {
			if ( !g_context->cvars ( ).m_sv_enable_bhop->get_bool ( ) ) {
				const auto speed = data.m_velocity.length ( );

				const auto max_speed = data.m_player->max_speed ( ) * 1.1f;
				if ( max_speed > 0.f
					&& speed > max_speed )
					data.m_velocity *= ( max_speed / speed );
			}

			if ( !data.m_on_ground )
				data.m_velocity.z = g_context->cvars ( ).m_sv_jump_impulse->get_float ( );
		}
		else
			data.m_velocity.z -=
			g_context->cvars ( ).m_sv_gravity->get_float ( ) * valve::g_global_vars->m_interval_per_tick;

		valve::trace_t trace {};
		valve::trace_filter_world_only_t trace_filter {};

		valve::g_engine_trace->trace_ray (
			{
			 data.m_origin,
			 data.m_origin + data.m_velocity * valve::g_global_vars->m_interval_per_tick,
			 data.m_obb_min, data.m_obb_max
			},
			valve::e_mask::contents_solid, &trace_filter, &trace
		);

		if ( trace.m_fraction != 1.f ) {
			for ( int i {}; i < 2; ++i ) {
				data.m_velocity -= trace.m_plane.m_normal * data.m_velocity.dot ( trace.m_plane.m_normal );

				const auto adjust = data.m_velocity.dot ( trace.m_plane.m_normal );
				if ( adjust < 0.f )
					data.m_velocity -= trace.m_plane.m_normal * adjust;

				valve::g_engine_trace->trace_ray (
					{
					 trace.m_end_pos,
					 trace.m_end_pos + ( data.m_velocity * ( valve::g_global_vars->m_interval_per_tick * ( 1.f - trace.m_fraction ) ) ),
					 data.m_obb_min, data.m_obb_max
					},
					valve::e_mask::contents_solid, &trace_filter, &trace
				);

				if ( trace.m_fraction == 1.f )
					break;
			}
		}

		data.m_origin = trace.m_end_pos;

		valve::g_engine_trace->trace_ray (
			{
			 trace.m_end_pos,
			 { trace.m_end_pos.x, trace.m_end_pos.y, trace.m_end_pos.z - 2.f },
			 data.m_obb_min, data.m_obb_max
			},
			valve::e_mask::contents_solid, &trace_filter, &trace
		);

		data.m_flags &= ~valve::e_ent_flags::on_ground;

		if ( trace.m_fraction != 1.f
			&& trace.m_plane.m_normal.z > 0.7f )
			data.m_flags |= valve::e_ent_flags::on_ground;
	}
	
	bool c_aim_bot::select_points( aim_target_t& target, bool additional_scan ) const {
		std::array< aim_point_t*, 11u > best_points{};

		const auto hp = target.m_entry->m_player->health( );
		lag_backup_t lag_backup{ target.m_entry->m_player };
		if ( additional_scan ) {
			target.m_lag_record->restore( target.m_entry->m_player, target.m_lag_record->m_anim_side );
		}

		for ( auto& point : target.m_points ) {
			if ( additional_scan )
				scan_point( target, point, false, g_context->shoot_pos( ) );

			if ( !point.m_valid
				|| point.m_pen_data.m_dmg < 1 )
				continue;

			auto& best_point = best_points.at( point.m_hitgroup );
			if ( !best_point ) {
				best_point = &point;

				continue;
			}

			const auto& best_pen_data = best_point->m_pen_data;
			const auto& pen_data = point.m_pen_data;

			if ( point.m_center ) {
				if ( ( best_pen_data.m_hitgroup == pen_data.m_hitgroup && best_pen_data.m_remaining_pen == pen_data.m_remaining_pen )
					|| ( best_pen_data.m_remaining_pen == pen_data.m_remaining_pen && std::abs( best_pen_data.m_dmg - pen_data.m_dmg ) <= 1 )
					|| ( best_pen_data.m_dmg > hp && pen_data.m_dmg > hp ) ) {
					best_point = &point;
				}

				continue;
			}

			auto& cur_dmg = pen_data.m_dmg;
			auto& last_dmg = best_pen_data.m_dmg;

			if ( last_dmg == cur_dmg ) {
				continue;
			}

			if ( cur_dmg >= hp
				&& last_dmg < hp ) {
				best_point = &point;
				break; // lethal point, its perfect
			}

			if ( best_pen_data.m_hitgroup != pen_data.m_hitgroup
				|| best_pen_data.m_remaining_pen != pen_data.m_remaining_pen ) {
				if ( best_pen_data.m_remaining_pen != pen_data.m_remaining_pen
					|| std::abs( best_pen_data.m_dmg - pen_data.m_dmg ) > 1 ) {
					if ( best_pen_data.m_dmg <= hp || pen_data.m_dmg <= hp ) {
						if ( pen_data.m_dmg > best_pen_data.m_dmg )
							best_point = &point;

						break; // nah escape from scan
					}
				}
			}

			if ( last_dmg < hp
				&& cur_dmg < hp ) {
				if ( std::abs( last_dmg - cur_dmg ) <= 1 ) {
					continue;
				}

				if ( cur_dmg > last_dmg + 5 ) {
					best_point = &point;
					break; // point is fine
				}
			}
		}

		if ( additional_scan )
			lag_backup.restore( target.m_entry->m_player );

		{
			std::vector< aim_point_t > new_points{};

			for ( auto& best_point : best_points )
				if ( best_point )
					new_points.emplace_back( std::move( *best_point ) );

			target.m_points = new_points;
		}

		if ( target.m_points.empty( ) )
			return false;

		for ( auto& point : target.m_points ) {
			if ( !target.m_best_point )
				target.m_best_point = &point;
			else {
				const auto& best_pen_data = target.m_best_point->m_pen_data;
				const auto& pen_data = point.m_pen_data;

				bool should_change_point{};

				if ( std::abs( best_pen_data.m_dmg - pen_data.m_dmg ) > 1 && ( pen_data.m_dmg <= hp || best_pen_data.m_dmg <= hp ) )
					should_change_point = pen_data.m_dmg > best_pen_data.m_dmg;

				if ( should_change_point )
					target.m_best_point = &point;
			}

			if ( point.m_hitgroup == 3 ) {
				if ( !target.m_best_body_point )
					target.m_best_body_point = &point;
				else {
					const auto& best_pen_data = target.m_best_body_point->m_pen_data;
					const auto& pen_data = point.m_pen_data;

					bool should_change_point{};
					if ( std::abs( best_pen_data.m_dmg - pen_data.m_dmg ) > 1 && ( pen_data.m_dmg <= hp || best_pen_data.m_dmg <= hp ) )
						should_change_point = pen_data.m_dmg > best_pen_data.m_dmg;

					if ( should_change_point )
						target.m_best_body_point = &point;
				}
			}
		}

		return true;
	}

	std::optional< aim_record_t > c_aim_bot::extrapolate( const player_entry_t& entry ) const {
		if ( entry.m_lag_records.empty( ) )
			return std::nullopt;

		const auto& latest = entry.m_lag_records.back ( );

		latest->m_extrapolated = false;

		if ( latest->m_sim_ticks < 0
			|| latest->m_total_cmds > 20
			|| latest->m_dormant
			|| latest->m_total_cmds <= 0 )
			return std::nullopt;

		const auto time_delta = g_lag_comp->calc_time_delta ( latest->m_sim_time );
		if ( time_delta < 0.2f ) {
			aim_record_t ret{ const_cast< player_entry_t* >( &entry ), latest };

			return ret;
		}
			
		const auto delay = g_movement->should_fake_duck( )
			? 15 - valve___g_client_state->m_choked_cmds : g_exploits->force_choke( ) != 0;

		const auto& net_info = g_context->net_info( );

		const auto latency_ticks = valve::to_ticks( net_info.m_latency.m_in + net_info.m_latency.m_out );
		const auto delta = ( delay + valve___g_client_state->m_server_tick + latency_ticks - latest->m_receive_tick ) / latest->m_total_cmds;
		if ( delta <= 0
			|| delta > 20 ) {
			aim_record_t ret{ const_cast< player_entry_t* >( &entry ), latest };

			return ret;
		}

		const auto max = valve::to_ticks( time_delta - 0.2f ) / latest->m_total_cmds;

		auto v18 = std::min( delta, max );
		if ( v18 <= 0 ) {
			aim_record_t ret{ const_cast< player_entry_t* >( &entry ), latest };

			return ret;
		}

		/* S/O ES0, SLAZYY, NAVE, D3X, POLAK, SOUFIW, PHILIPP, LLAMA, POTROSHITEL`, PLATINA, CHIEF KEEF, DUCARII */

		extrapolation_data_t data{ entry.m_player, latest.get( ) };

		do {
			for ( int i{}; i < latest->m_total_cmds; ++i ) {

				data.m_sim_time += valve___g_global_vars->m_interval_per_tick;

				player_move ( data );
			}
		} while ( --v18 );

		aim_record_t ret{ const_cast< player_entry_t* >( &entry ), std::make_shared< lag_record_t >( ) };

		latest->m_extrapolated = true;

		*ret.m_lag_record = *latest;

		ret.m_lag_record->m_sim_time = data.m_sim_time;
		ret.m_lag_record->m_flags = data.m_flags;

		ret.m_lag_record->m_origin = data.m_origin;
		ret.m_lag_record->m_velocity = data.m_velocity;

		const auto origin_delta = data.m_origin - latest->m_origin;

		for ( auto& anim_side : ret.m_lag_record->m_anim_sides )
			for ( std::size_t i{}; i < anim_side.m_bones_count; ++i ) {
				auto& bone = anim_side.m_bones.at( i );

				bone[ 0 ][ 3 ] += origin_delta.x;
				bone[ 1 ][ 3 ] += origin_delta.y;
				bone[ 2 ][ 3 ] += origin_delta.z;
			}

		return ret;
	}

	std::optional< aim_record_t > c_aim_bot::select_latest_record ( const player_entry_t& entry ) const {
		if ( entry.m_lag_records.empty ( ) )
			return std::nullopt;

		const auto& latest = entry.m_lag_records.back ( );
		if ( latest->m_sim_ticks <= 0
			|| latest->m_sim_ticks >= 20
			|| latest->m_dormant
			|| !latest->valid ( ) 
			|| latest->m_invalid )
			return std::nullopt;

		if ( latest->m_broke_lc ) {
			int delay {};

			if ( g_movement->should_fake_duck ( ) )
				delay = 15 - valve___g_client_state->m_choked_cmds;
			else
				delay = g_exploits->force_choke ( ) != 0;

			const auto v17 = std::clamp (
				valve::to_ticks (
					(
						( valve::to_time ( delay ) + g_context->net_info ( ).m_latency.m_out )
						+ valve___g_global_vars->m_real_time
						) - entry.m_receive_time
				),
				0, 100
			);

			if ( ( v17 - latest->m_total_cmds ) >= 0 )
				return std::nullopt;

			return extrapolate ( entry );
		}

		aim_record_t ret { const_cast< player_entry_t* >( &entry ), latest };

		return ret;
	}

	std::optional< aim_record_t > c_aim_bot::select_record( player_entry_t& entry ) const {
		if ( entry.m_lag_records.empty( ) )
			return std::nullopt;

		const auto mdl_data = entry.m_player->mdl_data( );
		if ( !mdl_data 
			|| !mdl_data->m_studio_hdr )
			return std::nullopt;
		static auto cl_lagcompensation = valve___g_cvar->find_var ( xorstr_ ( "cl_lagcompensation" ) );
		const auto hitbox_set = mdl_data->m_studio_hdr->hitbox_set( entry.m_player->hitbox_set_index( ) );
		if ( !hitbox_set 
			|| hitbox_set->m_hitboxes_count <= 0 )
			return std::nullopt;

		if ( cl_lagcompensation->get_int ( ) == 0 )
			return extrapolate ( entry );

		if ( entry.m_lag_records.size ( ) == 1u )
			return select_latest_record ( entry );

		int cfg_hitgroups {};

		cfg_hitgroups = 32 | 5;
		std::optional< aim_point_t > best_point {};
		std::shared_ptr< lag_record_t > best_record{};
		lag_backup_t lag_backup { entry.m_player };

		const auto hp = entry.m_player->health ( );

		std::size_t scanned_count{};

		const auto rend = entry.m_lag_records.rend ( );

		for ( auto i = entry.m_lag_records.rbegin ( ); i != rend; i++ ) {
			const auto& lag_record = *i;

			if ( !lag_record->valid ( ) ) {
				continue;
			}

			if ( lag_record->m_invalid )
				continue;

			if ( lag_record->m_total_cmds < 20
				&& !lag_record->m_dormant ) {
				aim_target_t target {};

				target.m_entry = const_cast < player_entry_t* > ( &entry );
				target.m_lag_record = lag_record;

				++scanned_count;
				auto hitgroups = 1;
				if ( ( scanned_count & 3 ) == 0 )
					hitgroups = cfg_hitgroups;

				scan_center_points ( target, hitgroups, g_context->shoot_pos ( ) );

				if ( !select_points ( target, false ) ) {
					if ( !best_record ) {
						best_record = lag_record;

					}

					continue;
				
				}

				if ( !best_record
					|| !best_point.has_value ( ) ) {
					best_record = lag_record;
					best_point = *target.m_best_point;

					continue;
				}

				const auto& pen_data = target.m_best_point->m_pen_data;
				const auto& best_pen_data = best_point.value ( ).m_pen_data;

				if ( std::abs ( pen_data.m_dmg - best_pen_data.m_dmg ) > 10 ) {
					if ( pen_data.m_dmg <= hp
						|| best_pen_data.m_dmg <= hp ) {
						if ( pen_data.m_dmg > best_pen_data.m_dmg ) {
							best_record = lag_record;
							best_point = *target.m_best_point;

							continue;
						}
					}
				}

				if ( target.m_best_point->m_intersections != best_point.value ( ).m_intersections ) {
					if ( target.m_best_point->m_intersections > best_point.value ( ).m_intersections ) {
						best_record = lag_record;
						best_point = *target.m_best_point;
					}

					continue;
				}

				if ( lag_record->m_shot && !best_record->m_shot ) {
					best_record = lag_record;
					best_point = *target.m_best_point;

					continue;
				}

				if ( lag_record->m_anim_updated && !best_record->m_anim_updated ) {
					best_record = lag_record;
					best_point = *target.m_best_point;

					continue;
				}

				if ( pen_data.m_dmg > best_pen_data.m_dmg ) {
					best_record = lag_record;
					best_point = *target.m_best_point;
				}
			}
		}

		lag_backup.restore ( entry.m_player );

		if ( !best_record ) {
			return std::nullopt;
		}

		if ( best_record->m_broke_lc ) // somehow who knows
			return extrapolate ( entry );


		aim_record_t ret { const_cast< player_entry_t* >( &entry ), best_record };

		return ret;
	}

	aim_point_t* c_aim_bot::select_point( const aim_target_t& target, const int cmd_number ) const {
		if ( !target.m_best_body_point
			|| target.m_best_body_point->m_pen_data.m_dmg < 1 )
			return target.m_best_point;

		const auto& shots = g_shots->elements( );
		if ( !shots.empty( ) ) {
			const auto& last_shot = shots.back( );
			if ( last_shot.m_target.m_entry == target.m_entry
				&& std::abs( last_shot.m_cmd_number - cmd_number ) <= crypt_int ( 150 ) ) {
				const auto after_shot_hp = last_shot.m_target.m_entry->m_player->health( ) - last_shot.m_dmg;
				if ( after_shot_hp > 0
					&& target.m_best_body_point->m_pen_data.m_dmg > after_shot_hp )
					return target.m_best_body_point;
			}
		}

		if ( target.m_best_body_point->m_pen_data.m_dmg >= target.m_entry->m_player->health( ) )
			return target.m_best_body_point;

		const auto dt_enabled = g_exploits->type( ) == 2 || g_exploits->type( ) == crypt_int ( 3 );

		if ( valve::g_local_player && valve::g_local_player->weapon ( ) && ( valve___g_local_player->weapon ( )->item_index ( ) == valve::e_item_index::ssg08
			|| valve___g_local_player->weapon()->item_index() == valve::e_item_index::awp ) )
		{
			if ( dt_enabled ) {
				const auto& shots = g_shots->elements ( );
				if ( !shots.empty ( ) ) {
					const auto& last_shot = shots.back ( );
					if (  last_shot.m_next_shift_amount
						&& last_shot.m_target.m_entry == target.m_entry
						&& std::abs ( last_shot.m_cmd_number - cmd_number ) <= crypt_int ( 150 )
						&& ( target.m_best_body_point->m_pen_data.m_dmg + last_shot.m_dmg ) >= target.m_entry->m_player->health ( ) )
						return target.m_best_body_point;
				}
			}
		}
		else
		{
			if ( dt_enabled
				&& g_exploits->next_shift_amount ( )
				&& ( target.m_best_body_point->m_pen_data.m_dmg * crypt_int ( 2 ) ) >= target.m_entry->m_player->health ( ) )
				return target.m_best_body_point;

			if ( dt_enabled ) {
				const auto& shots = g_shots->elements ( );
				if ( !shots.empty ( ) ) {
					const auto& last_shot = shots.back ( );
					if ( last_shot.m_next_shift_amount
						&& last_shot.m_target.m_entry == target.m_entry
						&& ( target.m_best_body_point->m_pen_data.m_dmg + last_shot.m_dmg ) >= target.m_entry->m_player->health ( ) )
						return target.m_best_body_point;
				}
			}
		}

		return target.m_best_point;
	}

	void c_aim_bot::find_targets( ) {
		std::vector< std::future< std::optional< aim_record_t > > > results{};
		std::vector < std::optional < aim_record_t > > results_no_thread {};

        m_targets.reserve( valve___g_global_vars->m_max_clients );

		for ( auto i = 1; i <= valve___g_global_vars->m_max_clients; ++i ) {
		    auto& entry = g_lag_comp->entry( i - 1 );
			if ( !entry.m_player
				|| entry.m_player->dormant( )
				|| !entry.m_player->alive( )
				|| entry.m_player->immune( )
				|| entry.m_player->friendly( )
				|| entry.m_lag_records.empty ( ) )
				continue;

				results_no_thread.emplace_back ( select_record ( entry ) );

		}

		for ( auto& result : results_no_thread ) {
			const auto& record = result;
			if ( !record.has_value ( ) )
				continue;

			int count { 0 };

			++count;
			static auto cl_lagcompensation = valve___g_cvar->find_var ( xorstr_ ( "cl_lagcompensation" ) );

			if ( cl_lagcompensation->get_int ( ) <= 0 && count > 1 )
				break;

			m_targets.emplace_back ( record.value ( ) );
		}
	}

	vec3_t get_hitbox ( valve::c_player* ent, std::ptrdiff_t hitbox, const valve::bones_t& bones )
	{
		if ( hitbox < 0 || hitbox > 19 )
			return vec3_t ( );

		const valve::model_t* const model = ent->model ( );

		if ( !model )
			return vec3_t ( );

		valve::studio_hdr_t* const studio_hdr = ent->mdl_data ( )->m_studio_hdr;

		if ( !studio_hdr )
			return vec3_t ( );

		auto const studio_hitbox = studio_hdr->p_hitbox ( hitbox, ent->hitbox_set_index ( ) );

		if ( !studio_hitbox )
			return vec3_t ( );

		if ( studio_hitbox->m_bone > crypt_int ( 128 ) || studio_hitbox->m_bone < 0 )
			return vec3_t ( );

		vec3_t min, max;
		vec3_t top_point;
		constexpr float rotation = 0.70710678f;
		math::vector_transform ( studio_hitbox->m_min, bones [ studio_hitbox->m_bone ], min );
		math::vector_transform ( studio_hitbox->m_min, bones [ studio_hitbox->m_bone ], max );

		auto center = ( min + max ) / 2.f;

		return center;
	}

	void c_aim_bot::scan_targets( ) {

		for ( auto& target : m_targets ) {
			g_aim_bot->scan_points ( target, get_hitgroups_to_set ( ), get_multi_points_to_set ( ), false );
		}

		m_targets.erase(
			std::remove_if(
				m_targets.begin( ), m_targets.end( ),
				[ & ] ( aim_target_t& target ) {
					return !select_points( target, true );
				}
			),
			m_targets.end( )
		);
	}

	aim_target_t* c_aim_bot::select_target( const int cmd_number ) {
		if ( m_targets.empty( ) )
			return nullptr;

		const auto& shots = g_shots->elements( );
		if ( !shots.empty( ) ) {
			const auto& last_shot = shots.back( );
			if ( last_shot.m_target.m_entry
				&& !last_shot.m_process_tick
				&& last_shot.m_target.m_entry->m_player
				&& std::abs( last_shot.m_cmd_number - cmd_number ) <= 150
				&& last_shot.m_dmg < last_shot.m_target.m_entry->m_player->health( ) ) {
				const auto target = std::find_if(
					m_targets.begin( ), m_targets.end( ),
					[ & ] ( const aim_target_t& target ) {
						return last_shot.m_target.m_entry->m_player == target.m_entry->m_player;
					}
				);

				if ( target != m_targets.end( ) )
					return &*target;
			}
		}

		auto best_target = &m_targets.front( );

		const auto end = m_targets.end( );
		for ( auto it = std::next ( m_targets.begin ( ) ); it != end; it = std::next ( it ) ) {
			const auto hp = it->m_entry->m_player->health ( );

			const auto& best_lag = best_target->m_lag_record;
			const auto& lag = it->m_lag_record;

			const auto& best_pen_data = best_target->m_best_point->m_pen_data;
			const auto& pen_data = it->m_best_point->m_pen_data;

			if ( lag->m_broke_lc == best_lag->m_broke_lc ) {
				
				if ( it->m_best_point->m_intersections == best_target->m_best_point->m_intersections ) {
					if ( best_pen_data.m_dmg <= hp
						&& pen_data.m_dmg > best_pen_data.m_dmg )
						best_target = &*it;
				}
				else if ( it->m_best_point->m_intersections > best_target->m_best_point->m_intersections ) {
					best_target = &*it;
				}
			}
			else if ( !lag->m_broke_lc )
			{
				best_target = &*it;
			}
		}

		return best_target;
	}

	vec2_t c_aim_bot::calc_spread_angle(
		const int bullets, const valve::e_item_index item_index,
		const float recoil_index, const std::size_t i
	) const {
		g_context->addresses ( ).m_random_seed.get( ) ( i + 1u );

		auto v1 = g_context->addresses ( ).m_random_float.get( ) ( 0.f, 1.f );
		auto v2 = g_context->addresses ( ).m_random_float.get( ) ( 0.f, math::k_pi2 );

		float v3 {}, v4 {};

		using fn_t = void ( __stdcall* )( valve::e_item_index, int, int, float*, float* );

		if ( g_context->cvars ( ).m_weapon_accuracy_shotgun_spread_patterns->get_int ( ) > 0 )
			reinterpret_cast< fn_t >( g_context->addresses ( ).m_calc_shotgun_spread.get( ) )( item_index, 0, static_cast< int >( bullets * recoil_index ), &v4, &v3 );
		else {
			v3 = g_context->addresses ( ).m_random_float.get( ) ( 0.f, 1.f );
			v4 = g_context->addresses ( ).m_random_float.get( ) ( 0.f, math::k_pi2 );
		}

		if ( recoil_index < 3.f
			&& item_index == valve::e_item_index::negev ) {
			for ( auto i = crypt_int ( 3 ); i > recoil_index; --i ) {
				v1 *= v1;
				v3 *= v3;
			}

			v1 = 1.f - v1;
			v3 = 1.f - v3;
		}

		const auto inaccuracy = v1 * g_eng_pred->inaccuracy ( );
		const auto spread = v3 * g_eng_pred->spread ( );

		return {
			std::cos ( v2 ) * inaccuracy + std::cos ( v4 ) * spread,
			std::sin ( v2 ) * inaccuracy + std::sin ( v4 ) * spread
		};
	}

	void c_aim_bot::get_hitbox_data ( c_hitbox_* rtn, valve::c_player* ent, int ihitbox, valve::bones_t& matrix )
	{
		if ( ihitbox < 0 || ihitbox > 19 ) return;

		if ( !ent ) return;

		const valve::model_t* const model = ent->model ( );

		if ( !model )
			return;

		valve::studio_hdr_t* const studio_hdr = ent->mdl_data ( )->m_studio_hdr;

		if ( !studio_hdr )
			return;

		valve::studio_hitbox_t* const hitbox = studio_hdr->p_hitbox ( ihitbox, ent->hitbox_set_index ( ) );

		if ( !hitbox )
			return;

		const auto is_capsule = hitbox->m_radius != -1.f;

		vec3_t min, max;
		if ( is_capsule ) {
			math::vector_transform ( hitbox->m_min, matrix[ hitbox->m_bone ], min );
			math::vector_transform ( hitbox->m_max, matrix[ hitbox->m_bone ], max );
		}
		else
		{
			min = math::vector_rotate ( hitbox->m_min, hitbox->m_rotation );
			max = math::vector_rotate ( hitbox->m_max, hitbox->m_rotation );

			math::vector_transform ( min, matrix[ hitbox->m_bone ], min );
			math::vector_transform ( max, matrix[ hitbox->m_bone ], max );
		}

		rtn->m_hitbox_id = ihitbox;
		rtn->m_is_oob = !is_capsule;
		rtn->m_radius = hitbox->m_radius;
		rtn->m_mins = min;
		rtn->m_maxs = max;
		rtn->m_hitgroup = hitbox->m_group;
		rtn->m_hitbox = hitbox;
		math::vector_i_transform ( g_context->shoot_pos ( ), matrix [ hitbox->m_bone ], rtn->m_start_scaled );
		rtn->m_bone = hitbox->m_bone;
	}


	bool c_aim_bot::new_hit_chance ( qangle_t angle, valve::c_player* player, int hitbox, float chance, valve::bones_t* bones )
	{
		return false;
	}

	static std::vector<std::tuple<float, float, float>> precomputed_seeds = {};
	static const int total_seeds = crypt_int ( 128 );
	void build_seed_table ( ) {
		if ( !precomputed_seeds.empty ( ) )
			return;

		for ( auto i = 0; i < total_seeds; i++ ) {
			g_context->addresses ( ).m_random_seed.get ( ) ( i + 1 );

			const auto pi_seed = g_context->addresses ( ).m_random_float.get ( ) ( 0.f, math::pi * 2 );

			precomputed_seeds.emplace_back ( g_context->addresses ( ).m_random_float.get ( ) ( 0.f, 1.f ),
				sin ( pi_seed ), cos ( pi_seed ) );
		}
	}

	int c_aim_bot::calc_hit_chance(
		const aim_target_t* const target, const qangle_t& angle, const aim_point_t* const aim_point
	) {
		build_seed_table ( );

		if ( g_context->weapon( )->item_index( ) == valve::e_item_index::scar20
			|| g_context->weapon( )->item_index( ) == valve::e_item_index::g3sg1 ) {
			if ( g_eng_pred->inaccuracy( ) <= 0.0380f
				&& g_context->weapon( )->zoom_lvl( ) == 0 ) {
				return 57;
			}
		}

		if ( ( g_context->weapon( )->item_index( ) == valve::e_item_index::ssg08 || g_context->weapon( )->item_index( ) == valve::e_item_index::revolver )
			&& !( valve___g_local_player->flags( ) & valve::e_ent_flags::on_ground ) ) {
			if ( g_eng_pred->inaccuracy( ) < 0.009f )
				return 100;
		}


		c_hitbox_ ht;
		get_hitbox_data ( &ht, target->m_entry->m_player, aim_point->m_hitbox, target->m_lag_record->m_anim_sides.at ( target->m_lag_record->m_anim_side ).m_bones );

		auto min_dmg = get_min_dmg_to_set_up ( ) + 1;

		min_dmg = std::clamp ( static_cast< int >( min_dmg * ( 2.f / crypt_float ( 100.f ) ) ), 1, crypt_int ( 100 ) );


		vec3_t fwd {}, right {}, up {};
		math::angle_vectors ( angle, &fwd, &right, &up );

		const auto item_index = g_context->weapon ( )->item_index ( );
		const auto recoil_index = g_context->weapon ( )->recoil_index ( );

		int hits {};

		const auto trace_spread = [ ] (
			const vec3_t& fwd, const vec3_t& right, const vec3_t& up,
			const aim_target_t* const target, const int accuracy_boost,
			const valve::e_item_index item_index, const float recoil_index,
			const int min_dmg, const std::size_t i, bool& hit ) {
				const auto spread_angle = g_aim_bot->calc_spread_angle ( g_context->wpn_data ( )->m_bullets, item_index, recoil_index, i );

				auto dir = fwd + ( right * spread_angle.x ) + ( up * spread_angle.y );

				dir.normalize ( );

				const auto pen_data = g_auto_wall->fire_bullet (
					valve::g_local_player, target->m_entry->m_player,
					g_context->wpn_data ( ), item_index == valve::e_item_index::taser,
					g_context->shoot_pos ( ), g_context->shoot_pos ( ) + dir * g_context->wpn_data ( )->m_range
				);

				hit = pen_data.m_dmg > 0;
		};

		constexpr auto k_max_seeds = 128u;

		const auto weapon_inaccuracy = g_context->weapon ( )->inaccuracy ( );
		vec3_t total_spread {}, end {};
		float inaccuracy {}, spread_x {}, spread_y {};
		std::tuple<float, float, float>* seed {};
		std::array< bool, k_max_seeds > seeds {};
		for ( std::size_t i {}; i < k_max_seeds; ++i ) {
			seed = &precomputed_seeds [ i ];

			inaccuracy = std::get<0> ( *seed ) * weapon_inaccuracy;
			spread_x = std::get<2> ( *seed ) * inaccuracy;
			spread_y = std::get<1> ( *seed ) * inaccuracy;

			total_spread = ( fwd + right * spread_x + up * spread_y ).normalized ( );

			end = g_context->shoot_pos ( ) + ( total_spread * crypt_float ( 8192.f ) );
			bool intersected = false;

			if ( ht.m_is_oob )  {
				vec3_t delta;
				math::vector_i_rotate ( ( total_spread * crypt_float ( 8192.f ) ), 
					target->m_lag_record->m_anim_sides.at ( target->m_lag_record->m_anim_side ).m_bones [ ht.m_bone ],
					delta );

				intersected = math::intersect_bb ( ht.m_start_scaled, delta, ht.m_mins, ht.m_maxs );
			}
			else
			{
				intersected = math::intersect ( g_context->shoot_pos ( ), end, ht.m_mins, ht.m_maxs, ht.m_radius );
			}

			if ( intersected )
				trace_spread ( fwd, right, up, target, 0, item_index, recoil_index, min_dmg, i, seeds.at ( i ) );
		}

		for ( auto& hit : seeds )
			if ( hit )
				++hits; 

		return static_cast< int >( ( hits / static_cast< float >( k_max_seeds ) ) * 100.f );
	}

	void c_aim_bot::on_create_move( valve::user_cmd_t& user_cmd, bool& send_packet ) {
	
		m_last_target.m_lag_record.reset();

		hacks::g_movement->m_allow_early = true;

		if ( !hacks::g_aim_bot->cfg ( ).m_bEnableRagebot
			|| !g_context->weapon( ) || !g_context->wpn_data( )
			|| g_context->weapon( )->in_reload( )
			|| ( static_cast< std::size_t >( g_context->wpn_data( )->m_unk_type - 2 ) > 5
				&& g_context->weapon( )->item_index( ) != valve::e_item_index::taser )
			|| valve___g_local_player->move_type( ) == valve::e_move_type::ladder
			|| valve___g_local_player->move_type( ) == valve::e_move_type::noclip
			|| g_context->weapon ( )->is_knife ( ) )
		return;

		find_targets( );

		scan_targets( );

		const auto target = select_target( user_cmd.m_number );
		if ( !target )
			return m_targets.clear( );

		hacks::g_movement->m_allow_early = false;

		g_context->flags( ) |= e_context_flags::has_target;

		const auto point = select_point( *target, user_cmd.m_number );
		if ( !point
			|| !point->m_valid )
			return m_targets.clear( );

		lag_backup_t lag_backup{ target->m_entry->m_player };

		target->m_lag_record->restore( target->m_entry->m_player, target->m_lag_record->m_anim_side );

		qangle_t angle{};

		math::vector_angles( point->m_pos - g_context->shoot_pos( ), angle );

		const auto item_index = g_context->weapon( )->item_index( );

		if ( g_context->flags( ) & e_context_flags::can_shoot ) {
			if ( hacks::g_aim_bot->cfg ( ).m_bAutoScope
				&& ( item_index == valve::e_item_index::g3sg1
					|| item_index == valve::e_item_index::scar20
					|| item_index == valve::e_item_index::awp
					|| item_index == valve::e_item_index::aug
					|| item_index == valve::e_item_index::sg553
					|| item_index == valve::e_item_index::ssg08
					)
				&& !valve___g_local_player->scoped( )
				&& valve___g_local_player->flags( ) & valve::e_ent_flags::on_ground
				&& !( g_eng_pred->old_user_cmd( ).m_buttons & valve::e_buttons::in_jump ) )
				user_cmd.m_buttons |= valve::e_buttons::in_attack2;
			else if ( const auto hit_chance = calc_hit_chance ( target, angle, point ); hit_chance >= ( item_index == valve::e_item_index::taser ? 66 : get_hit_chance_to_set_up ( ) ) ) {
				user_cmd.m_view_angles = angle - valve___g_local_player->aim_punch( ) * g_context->cvars( ).m_weapon_recoil_scale->get_float( );

				user_cmd.m_view_angles.x = std::remainder( user_cmd.m_view_angles.x, 360.f );
				user_cmd.m_view_angles.y = std::remainder( user_cmd.m_view_angles.y, 360.f );
				user_cmd.m_view_angles.z = std::remainder( user_cmd.m_view_angles.z, 360.f );

				user_cmd.m_view_angles.x = std::clamp( user_cmd.m_view_angles.x, -89.f, 89.f );
				user_cmd.m_view_angles.y = std::clamp( user_cmd.m_view_angles.y, -180.f, 180.f );
				user_cmd.m_view_angles.z = std::clamp( user_cmd.m_view_angles.z, -90.f, 90.f );

				user_cmd.m_buttons |= valve::e_buttons::in_attack;

				static auto cl_lagcompensation = valve___g_cvar->find_var ( xorstr_ ( "cl_lagcompensation" ) );

				g_context->defensive_allowed( ) = false;

				if ( cl_lagcompensation->get_int ( ) != 0 )
				user_cmd.m_tick = valve::to_ticks( target->m_lag_record->m_sim_time + g_context->net_info( ).m_lerp );

				g_context->aim_bot_shoot_pos ( ) = g_context->shoot_pos ( );

				m_last_target.m_entry = target->m_entry;
				m_last_target.m_lag_record = target->m_lag_record;
				m_last_target.m_hitbox = point->m_hitbox;
				m_last_target.m_dmg = static_cast< int >( point->m_pen_data.m_dmg );
				m_last_target.m_hit_chance = hit_chance;
				m_last_target.m_point = *point;
				g_context->get_auto_peek_info ( ).m_is_firing = true;
				g_context->flags( ) |= e_context_flags::aim_fire;
			}
			else {
				if ( g_context->flags( ) & e_context_flags::can_shoot )
					if ( !( user_cmd.m_buttons & valve::e_buttons::in_jump ) )
						g_movement->stop_type( ) = get_stop_type( ) + 1;
			}
		}

		lag_backup.restore( target->m_entry->m_player );

		m_targets.clear( );
	}
}