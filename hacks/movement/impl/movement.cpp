#include "../../../nem.hpp"
#define what(x) (fpclassify(x) == FP_INFINITE || fpclassify(x) == FP_NAN || fpclassify(x) == FP_SUBNORMAL)
namespace nem::hacks {
	void c_movement::accelerate(
		const valve::user_cmd_t& user_cmd, const vec3_t& wishdir,
		const float wishspeed, vec3_t& velocity, float acceleration
	) const {
		const auto cur_speed = velocity.dot( wishdir );

		const auto add_speed = wishspeed - cur_speed;
		if ( add_speed <= 0.f )
			return;

		const auto v57 = std::max( cur_speed, 0.f );

		const auto ducking =
			user_cmd.m_buttons & valve::e_buttons::in_duck
			|| valve___g_local_player->ducking( )
			|| valve___g_local_player->flags( ) & valve::e_ent_flags::ducking;

		auto v20 = true;
		if ( ducking
			|| !( user_cmd.m_buttons & valve::e_buttons::in_speed ) )
			v20 = false;

		auto finalwishspeed = std::max( wishspeed, 250.f );
		auto abs_finalwishspeed = finalwishspeed;

		const auto weapon = valve___g_local_player->weapon( );

		bool slow_down_to_fast_nigga{};

		if ( weapon
			&& g_context->cvars( ).m_sv_accelerate_use_weapon_speed->get_bool( ) ) {
			const auto item_index = static_cast< std::uint16_t >( weapon->item_index( ) );
			if ( weapon->zoom_lvl( ) > 0
				&& ( item_index == 11 || item_index == 38 || item_index == 9 || item_index == 8 || item_index == 39 || item_index == 40 ) )
				slow_down_to_fast_nigga = ( m_max_weapon_speed * 0.52f ) < 110.f;

			const auto modifier = std::min( 1.f, m_max_weapon_speed / 250.f );

			abs_finalwishspeed *= modifier;

			if ( ( !ducking && !v20 )
				|| slow_down_to_fast_nigga )
				finalwishspeed *= modifier;
		}

		if ( ducking ) {
			if ( !slow_down_to_fast_nigga )
				finalwishspeed *= 0.34f;

			abs_finalwishspeed *= 0.34f;
		}

		if ( v20 ) {
			if ( !slow_down_to_fast_nigga )
				finalwishspeed *= 0.52f;

			abs_finalwishspeed *= 0.52f;
		}

		const auto abs_finalwishspeed_minus5 = abs_finalwishspeed - 5.f;
		if ( v57 < abs_finalwishspeed_minus5 ) {
			const auto v30 =
				std::max( v57 - abs_finalwishspeed_minus5, 0.f )
				/ std::max( abs_finalwishspeed - abs_finalwishspeed_minus5, 0.f );

			const auto v27 = 1.f - v30;
			if ( v27 >= 0.f )
				acceleration = std::min( v27, 1.f ) * acceleration;
			else
				acceleration = 0.f;
		}

		float v30 = 0.0f;
		if ( v20 && v57 > ( abs_finalwishspeed - 5.0f ) ) {
			float v29 = fmaxf( v57 - ( abs_finalwishspeed - 5.0f ), 0.0f ) / fmaxf( abs_finalwishspeed - ( abs_finalwishspeed - 5.0f ), 0.0f );
			if ( ( 1.0f - v29 ) >= 0.0f )
				v30 = fminf( 1.0f - v29, 1.0f ) * acceleration;
			else
				v30 = 0.0f * acceleration;
		}
		else {
			v30 = acceleration;
		}

		const auto v33 = std::min(
			add_speed,
			( ( valve___g_global_vars->m_interval_per_tick * v30 ) * finalwishspeed )
			* valve___g_local_player->surface_friction( )
		);

		velocity += wishdir * v33;

		const auto len = velocity.length( );
		if ( len
			&& len > m_max_weapon_speed )
			velocity *= m_max_weapon_speed / len;

	}

	void c_movement::walk_move(
		const valve::user_cmd_t& user_cmd, vec3_t& move,
		vec3_t& fwd, vec3_t& right, vec3_t& velocity
	) const {
		if ( fwd.z != 0.f )
			fwd.normalize( );

		if ( right.z != 0.f )
			right.normalize( );

		vec3_t wishvel{
			fwd.x * move.x + right.x * move.y,
			fwd.y * move.x + right.y * move.y,
			0.f
		};

		auto wishdir = wishvel;

		auto wishspeed = wishdir.normalize( );
		if ( wishspeed
			&& wishspeed > m_max_player_speed ) {
			wishvel *= m_max_player_speed / wishspeed;

			wishspeed = m_max_player_speed;
		}

		velocity.z = 0.f;
		accelerate( user_cmd, wishdir, wishspeed, velocity, g_context->cvars( ).m_sv_accelerate->get_float( ) );
		velocity.z = 0.f;

		const auto speed_sqr = velocity.length_sqr( );
		if ( speed_sqr > ( m_max_player_speed * m_max_player_speed ) )
			velocity *= m_max_player_speed / std::sqrt( speed_sqr );

		if ( velocity.length( ) < 1.f )
			velocity = {};
	}

	void c_movement::full_walk_move(
		const valve::user_cmd_t& user_cmd, vec3_t& move,
		vec3_t& fwd, vec3_t& right, vec3_t& velocity
	) const {
		if ( valve___g_local_player->ground_entity( ) != valve::e_ent_handle::invalid ) {
			velocity.z = 0.f;

			const auto speed = velocity.length( );
			if ( speed >= 0.1f ) {
				static auto sv_stopspeed = valve::g_cvar->find_var ( xorstr_ ( "sv_stopspeed" ) )->get_float ( );
				const auto friction = g_context->cvars( ).m_sv_friction->get_float( );
				const auto control = speed < sv_stopspeed ? sv_stopspeed : speed;

				const auto new_speed = std::max( 0.f, speed - ( ( control * friction ) * valve___g_global_vars->m_interval_per_tick ) );
				if ( speed != new_speed )
					velocity *= new_speed / speed;
			}

			walk_move( user_cmd, move, fwd, right, velocity );

			velocity.z = 0.f;
		}

		const auto sv_maxvelocity = g_context->cvars( ).m_sv_maxvelocity->get_float( );
		for ( std::size_t i{}; i < 3u; ++i ) {
			auto& element = velocity[ i ];

			if ( element > sv_maxvelocity )
				element = sv_maxvelocity;
			else if ( element < -sv_maxvelocity )
				element = -sv_maxvelocity;
		}
	}

	void c_movement::modify_move( valve::user_cmd_t& user_cmd, vec3_t& velocity ) const {
		vec3_t fwd{}, right{};

		math::angle_vectors( user_cmd.m_view_angles, &fwd, &right );

		const auto speed_sqr = user_cmd.m_move.length_sqr( );
		if ( speed_sqr > ( m_max_player_speed * m_max_player_speed ) )
			user_cmd.m_move *= m_max_player_speed / std::sqrt( speed_sqr );

		full_walk_move( user_cmd, user_cmd.m_move, fwd, right, velocity );
	}

	void c_movement::predict_move( const valve::user_cmd_t& user_cmd, vec3_t& velocity ) const {
		vec3_t fwd{}, right{};

		math::angle_vectors( user_cmd.m_view_angles, &fwd, &right );

		auto move = user_cmd.m_move;

		const auto speed_sqr = user_cmd.m_move.length_sqr( );
		if ( speed_sqr > ( m_max_player_speed * m_max_player_speed ) )
			move *= m_max_player_speed / std::sqrt( speed_sqr );

		full_walk_move( user_cmd, move, fwd, right, velocity );
	}

	void c_movement::auto_strafe( valve::user_cmd_t& user_cmd ) {
		if ( !hacks::g_aim_bot->cfg ( ).m_iAutoStrafeType
			|| ( hacks::g_aim_bot->cfg ( ).m_iAutoStrafeType != 2 && ( user_cmd.m_move.x != 0.f || user_cmd.m_move.y != 0.f ) )
			|| valve___g_local_player->flags( ) & valve::e_ent_flags::on_ground )
			return;

		auto wish_angles = user_cmd.m_view_angles;

		const auto velocity = valve::g_local_player->velocity ( );

		auto speed_2d = velocity.length_2d ( );

		auto ideal_0 = ( speed_2d > 0.f ) ? math::to_deg ( std::asin ( 15.f / speed_2d ) ) : 90.f;
		auto ideal_1 = ( speed_2d > 0.f ) ? math::to_deg ( std::asin ( 30.f / speed_2d ) ) : 90.f;

		ideal_0 = std::clamp ( ideal_0, 0.f, 90.f );
		ideal_1 = std::clamp ( ideal_1, 0.f, 90.f );

		const auto mult = m_strafe_switch ? 1.f : -1.f;

		m_strafe_switch = !m_strafe_switch;

		enum e_dirs {
			e_forwards = 0,
			e_backwards = 180,
			e_left = 90,
			e_right = -90,
			e_back_left = 135,
			e_back_right = -135
		};

		float wish_dir {};

		bool hold_w = user_cmd.m_buttons & valve::e_buttons::in_forward;
		bool hold_a = user_cmd.m_buttons & valve::e_buttons::in_move_left;
		bool hold_s = user_cmd.m_buttons & valve::e_buttons::in_back;
		bool hold_d = user_cmd.m_buttons & valve::e_buttons::in_move_right;

		if ( hold_w ) {
			if ( hold_a )
				wish_dir += ( e_dirs::e_left / 2 );
			else if ( hold_d )
				wish_dir += ( e_dirs::e_right / 2 );
			else
				wish_dir += e_dirs::e_forwards;
		}
		else if ( hold_s ) {
			if ( hold_a )
				wish_dir += e_dirs::e_back_left;
			else if ( hold_d )
				wish_dir += e_dirs::e_back_right;
			else
				wish_dir += e_dirs::e_backwards;

			user_cmd.m_move.x = 0.f;
		}
		else if ( hold_a )
			wish_dir += e_dirs::e_left;
		else if ( hold_d )
			wish_dir += e_dirs::e_right;

		wish_angles.y += math::normalize_yaw ( wish_dir );

		user_cmd.m_move.x = 0.f;

		auto delta = math::normalize_yaw ( wish_angles.y - m_prev_view_yaw );

		auto abs_delta = std::abs ( delta );

		if ( delta > 0.f )
			user_cmd.m_move.y = -450.f;
		else if ( delta < 0.f )
			user_cmd.m_move.y = 450.f;

		if ( abs_delta <= ideal_0
			|| abs_delta >= 30.f ) {
			vec3_t vel_ang {};

			math::vector_angles ( velocity, vel_ang );

			auto vel_delta = math::normalize_yaw ( wish_angles.y - vel_ang.y );

			auto correct = ideal_0;

			if ( vel_delta <= correct ||
				speed_2d <= 15.f ) {
				if ( -correct <= vel_delta || speed_2d <= 15.f ) {
					wish_angles.y += ( ideal_0 * mult );
					user_cmd.m_move.y = 450.f * mult;
				}
				else {
					wish_angles.y = vel_ang.y - correct;
					user_cmd.m_move.y = 450.f;
				}
			}
			else {
				wish_angles.y = vel_ang.y + correct;
				user_cmd.m_move.y = -450.f;
			}


			rotate (
				user_cmd, wish_angles,
				valve___g_local_player->flags ( ),
				valve___g_local_player->move_type ( )
			);
		}
	}

	bool should_pred_auto_stop( ) {
		if ( !valve___g_local_player || !valve___g_local_player->alive( ) )
			return 0;

		auto wpn = valve___g_local_player->weapon( );

		if ( !wpn )
			return 0;

		switch ( wpn->item_index( ) )
		{
		case valve::e_item_index::awp:
			return hacks::g_aim_bot->cfg( ).m_bPredAutoStopAwp;
		case valve::e_item_index::ssg08:
			return hacks::g_aim_bot->cfg( ).m_bPredAutoStopSSG08;
		case valve::e_item_index::scar20:
		case valve::e_item_index::g3sg1:
			return hacks::g_aim_bot->cfg( ).m_bPredAutoStopScar;
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
			return hacks::g_aim_bot->cfg( ).m_bPredAutoStopRifles;
		case valve::e_item_index::revolver:
		case valve::e_item_index::deagle:
			return hacks::g_aim_bot->cfg( ).m_bPredAutoStopR8;
		case valve::e_item_index::cz75a:
		case valve::e_item_index::elite:
		case valve::e_item_index::fiveseven:
		case valve::e_item_index::hkp2000:
		case valve::e_item_index::glock:
		case valve::e_item_index::p250:
		case valve::e_item_index::tec9:
		case valve::e_item_index::usp_silencer:
			return hacks::g_aim_bot->cfg( ).m_bPredAutoStopPistols;
		default:
			return 0;
		}

		return 0;
	}

	bool c_movement::stop( valve::user_cmd_t& user_cmd ) {
		static qangle_t wish_ang{};
		bool predict_available{};

		if ( const auto weapon = g_context->weapon ( ) ) {
			if ( weapon->wpn_data( ) ) {
				if ( weapon->wpn_data( )->m_type == 8
					|| weapon->wpn_data( )->m_type == 7
					|| weapon->wpn_data( )->m_type == 0 )
					return false;
			}
		}

		if ( g_context->flags( ) & e_context_flags::can_shoot
			&& should_pred_auto_stop ( )
			&& m_allow_early
			&& hacks::g_exploits->is_peeking( wish_ang, 2.f ) )
			predict_available = true;

		if ( !predict_available )
		if ( !hacks::g_movement->stop_type( ) )
			return false;

		auto stop_type = hacks::g_movement->stop_type( );

		hacks::g_movement->stop_type( ) = 0;

		if ( predict_available ) {
			stop_type = 1;
		}

		if ( !( valve::g_local_player->flags( ) & valve::e_ent_flags::on_ground )
			|| user_cmd.m_buttons & valve::e_buttons::in_jump )
			return false;

		static auto sv_accelerate_use_weapon_speed = valve::g_cvar->find_var( xorstr_( "sv_accelerate_use_weapon_speed" ) );

		static auto sv_accelerate = valve::g_cvar->find_var( xorstr_( "sv_accelerate" ) );

		const auto weapon = valve::g_local_player->weapon( );
		if ( !weapon )
			return false;

		const auto wpn_data = weapon->wpn_data( );
		if ( !wpn_data )
			return false;

		auto max_speed{ 260.f };

		if ( valve::g_local_player->weapon( )
			&& valve::g_local_player->weapon( )->wpn_data( ) ) {
			max_speed = valve::g_local_player->scoped( ) ?
				valve::g_local_player->weapon( )->wpn_data( )->m_max_speed_alt :
				valve::g_local_player->weapon( )->wpn_data( )->m_max_speed;
		}

		float target_speed{};

		if ( stop_type == 1 )
			target_speed = 36.5f;

		vec3_t cur_velocity{ valve::g_local_player->velocity( ) };

		if ( predict_available )
			predict_move( user_cmd, cur_velocity );

		const auto speed_2d = cur_velocity.length_2d ( );

		if ( stop_type == 2 ) {
			if ( speed_2d < 1.f ) {
				user_cmd.m_move.x = user_cmd.m_move.y = 0.f;

				return false;
			}
		}
		else if ( target_speed >= speed_2d ) {
			m_max_player_speed = m_max_weapon_speed = target_speed;

			modify_move( user_cmd, cur_velocity );

			return false;
		}

		user_cmd.m_buttons &= ~valve::e_buttons::in_speed;

		auto finalwishspeed = std::min( max_speed, 250.f );

		const auto ducking =
			user_cmd.m_buttons & valve::e_buttons::in_duck
			|| valve::g_local_player->flags( ) & valve::e_ent_flags::ducking;

		bool slow_down_to_fast_nigga{};

		if ( sv_accelerate_use_weapon_speed->get_int( ) ) {
			const auto item_index = static_cast< std::uint16_t >( weapon->item_index( ) );
			if ( weapon->zoom_lvl( ) > 0
				&& ( item_index == crypt_int( 11 ) || item_index == crypt_int( 38 ) || item_index == crypt_int( 9 ) || item_index == crypt_int( 8 ) || item_index == crypt_int( 39 ) || item_index == crypt_int( 40 ) ) )
				slow_down_to_fast_nigga = ( max_speed * crypt_float( 0.52f ) ) < crypt_float( 110.f );

			if ( !ducking
				|| slow_down_to_fast_nigga )
				finalwishspeed *= std::min( 1.f, max_speed / 250.f );
		}

		if ( ducking
			&& !slow_down_to_fast_nigga )
			finalwishspeed *= crypt_float( 0.34f );

		finalwishspeed =
			( ( valve___g_global_vars->m_interval_per_tick * sv_accelerate->get_float( ) ) * finalwishspeed )
			* valve::g_local_player->surface_friction( );

		if ( stop_type == 1 ) {
			if ( max_speed * 0.52f <= speed_2d ) {
				qangle_t dir{};
				math::vector_angles( cur_velocity *= -1.f, dir );

				dir.y = user_cmd.m_view_angles.y - dir.y;

				vec3_t dir_ang_handler{};

				math::angle_vectors( dir, &dir_ang_handler, nullptr, nullptr );

				user_cmd.m_move.x = dir_ang_handler.x * finalwishspeed;
				user_cmd.m_move.y = dir_ang_handler.y * finalwishspeed;
			}
			else {
				m_max_player_speed = m_max_weapon_speed = target_speed;

				modify_move( user_cmd, cur_velocity );
			}
		}
		else {
			qangle_t dir{};
			math::vector_angles( cur_velocity *= -1.f, dir );

			dir.y = user_cmd.m_view_angles.y - dir.y;

			vec3_t dir_ang_handler{};

			math::angle_vectors( dir, &dir_ang_handler, nullptr, nullptr );

			user_cmd.m_move.x = dir_ang_handler.x * finalwishspeed;
			user_cmd.m_move.y = dir_ang_handler.y * finalwishspeed;
		}

		return true;
	}

	void c_movement::fast_stop ( valve::user_cmd_t& user_cmd ) {
		if ( !hacks::g_aim_bot->cfg ( ).m_bFastStop )
			return;

		if ( !valve::g_local_player->alive ( ) )
			return;

		if ( valve::g_local_player->move_type ( ) == valve::e_move_type::ladder )
			return;

		if ( user_cmd.m_buttons & ( valve::e_buttons::in_jump | valve::e_buttons::in_move_left | valve::e_buttons::in_move_right | valve::e_buttons::in_forward | valve::e_buttons::in_back ) )
			return;

		if ( ! ( valve::g_local_player->flags ( ) & valve::e_ent_flags::on_ground ) )
			return;

		float max_speed = 260.f;

		if ( valve::g_local_player->weapon ( ) )
		{
			auto weapon_data = valve::g_local_player->weapon ( )->wpn_data ( );

			if ( weapon_data )
				max_speed = valve::g_local_player->scoped ( ) ? weapon_data->m_max_speed_alt : weapon_data->m_max_speed;
		}

		if ( valve::g_local_player->velocity ( ).length_2d ( ) <= max_speed * 0.34f )
		{
			user_cmd.m_move.x = user_cmd.m_move.y = 0.f;
			return;
		}

		qangle_t resistance_ang = qangle_t ( );

		math::vector_angles ( valve::g_local_player->velocity ( ) * -1.f, resistance_ang );

		resistance_ang.y = user_cmd.m_view_angles.y - resistance_ang.y;
		resistance_ang.x = user_cmd.m_view_angles.x - resistance_ang.x;

		vec3_t resistance_vec = vec3_t ( );

		math::angle_vectors ( resistance_ang, &resistance_vec );

		user_cmd.m_move.x = std::clamp ( resistance_vec.x, -450.f, 450.0f );
		user_cmd.m_move.y = std::clamp ( resistance_vec.y, -450.f, 450.0f );
	}

	bool c_movement::force_lby_update( valve::user_cmd_t& user_cmd ) {
		return false;
	}
	template< class T >

	__forceinline void normalize_ghetto ( T& vec )
	{
		for ( auto i = 0; i < 2; i++ ) {
			while ( vec[ i ] < -180.0f ) vec[ i ] += 360.0f;
			while ( vec[ i ] > 180.0f ) vec[ i ] -= 360.0f;
		}

		while ( vec[ 2 ] < -50.0f ) vec[ 2 ] += 100.0f;
		while ( vec[ 2 ] > 50.0f ) vec[ 2 ] -= 100.0f;
	}

	__forceinline void clamp_angles ( qangle_t& angles )
	{
		if ( angles.x > 89.0f )
			angles.x = 89.0f;
		else if ( angles.x < -89.0f )
			angles.x = -89.0f;

		if ( angles.y > 180.0f )
			angles.y = 180.0f;
		else if ( angles.y < -180.0f )
			angles.y = -180.0f;

		angles.z = std::clamp ( angles.z, -50.0f, 50.0f );
	}

	void c_movement::normalize( valve::user_cmd_t& user_cmd ) const {
		if ( !hacks::g_aim_bot->cfg ( ).m_bRollEnabled )
		{
			user_cmd.m_view_angles.x = std::remainder ( user_cmd.m_view_angles.x, 360.f );
			user_cmd.m_view_angles.y = std::remainder ( user_cmd.m_view_angles.y, 360.f );
			user_cmd.m_view_angles.z = std::remainder ( user_cmd.m_view_angles.z, 360.f );

			user_cmd.m_view_angles.x = std::clamp ( user_cmd.m_view_angles.x, -89.f, 89.f );
			user_cmd.m_view_angles.y = std::clamp ( user_cmd.m_view_angles.y, -180.f, 180.f );
			user_cmd.m_view_angles.z = std::clamp ( user_cmd.m_view_angles.z, -90.f, 90.f );

			user_cmd.m_move.x = std::clamp ( user_cmd.m_move.x, -450.f, 450.f );
			user_cmd.m_move.y = std::clamp ( user_cmd.m_move.y, -450.f, 450.f );
			user_cmd.m_move.z = std::clamp ( user_cmd.m_move.z, -320.f, 320.f );
		}
		else
		{
			normalize_ghetto ( user_cmd.m_view_angles );
			clamp_angles ( user_cmd.m_view_angles );
		}

		if ( valve___g_local_player->move_type( ) != valve::e_move_type::walk )
			return;

		user_cmd.m_buttons &= ~(
			valve::e_buttons::in_forward
			| valve::e_buttons::in_back
			| valve::e_buttons::in_move_right
			| valve::e_buttons::in_move_left
		);

		if ( user_cmd.m_move.x != 0.f )
			user_cmd.m_buttons |=
			( hacks::g_aim_bot->cfg ( ).m_bSlideWalk ? user_cmd.m_move.x < 0.f: user_cmd.m_move.x > 0.f )
			? valve::e_buttons::in_forward : valve::e_buttons::in_back;

		if ( user_cmd.m_move.y == 0.f )
			return;

		user_cmd.m_buttons |=
			( hacks::g_aim_bot->cfg ( ).m_bSlideWalk ? user_cmd.m_move.y < 0.f : user_cmd.m_move.y > 0.f )
			? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;
	}

	__forceinline bool compare_vec ( vec3_t& first, vec3_t& second )
	{
		if ( first.x == second.x &&
			first.y == second.y &&
			first.z == second.z )
			return true;

		return false;
	}

	__forceinline float vector_normalize ( vec3_t& vec )
	{
		float radius = sqrtf ( vec.x * vec.x + vec.y * vec.y + vec.z * vec.z );
		float iradius = 1.f / ( radius + FLT_EPSILON );

		vec.x *= iradius;
		vec.y *= iradius;
		vec.z *= iradius;

		return radius;
	}

	void c_movement::rotate (
		valve::user_cmd_t& user_cmd, const qangle_t& wish_angles,
		const valve::e_ent_flags flags, const valve::e_move_type move_type
	) const
	{

		if ( !hacks::g_aim_bot->cfg ( ).m_bRollEnabled )
		{
			if ( user_cmd.m_view_angles.z != 0.f
				&& !( flags & valve::e_ent_flags::on_ground ) )
				user_cmd.m_move.y = 0.f;

			auto move_2d = vec2_t ( user_cmd.m_move.x, user_cmd.m_move.y );

			if ( const auto speed_2d = move_2d.length ( ) ) {
				const auto delta = user_cmd.m_view_angles.y - wish_angles.y;

				vec2_t v1;

				math::sin_cos (
					math::to_rad (
						std::remainder (
							math::to_deg (
								std::atan2 ( move_2d.y / speed_2d, move_2d.x / speed_2d )
							) + delta, 360.f
						)
					), v1.x, v1.y
				);

				const auto cos_x = std::cos (
					math::to_rad (
						std::remainder (
							math::to_deg ( std::atan2 ( 0.f, speed_2d ) ), 360.f
						)
					)
				);

				move_2d.x = cos_x * v1.y * speed_2d;
				move_2d.y = cos_x * v1.x * speed_2d;

				if ( move_type == valve::e_move_type::ladder ) {
					if ( wish_angles.x < 45.f
						&& std::abs ( delta ) <= crypt_float ( 65.f )
						&& user_cmd.m_view_angles.x >= 45.f ) {
						move_2d.x *= -1.f;
					}
				}
				else if ( std::abs ( user_cmd.m_view_angles.x ) > crypt_float ( 90.f ) )
					move_2d.x *= -1.f;
			}

			user_cmd.m_move.x = move_2d.x;
			user_cmd.m_move.y = move_2d.y;
		}
		else
		{
			vec3_t pure_fwd, pure_right, pure_up, curr_fwd, curr_right, curr_up;
			math::angle_vectors ( wish_angles, &pure_fwd, &pure_right, &pure_up );
			math::angle_vectors ( user_cmd.m_view_angles, &curr_fwd, &curr_right, &curr_up );

			pure_fwd[ 2 ] = pure_right[ 2 ] = curr_fwd[ 2 ] = curr_right[ 2 ] = 0.f;

			vector_normalize ( pure_fwd );
			vector_normalize ( pure_right );
			vector_normalize ( curr_fwd );
			vector_normalize ( curr_right );
			vec3_t pure_target_dir;
			for ( auto i = 0u; i < 2; i++ )
				pure_target_dir[ i ] = pure_fwd[ i ] * user_cmd.m_move.x + pure_right[ i ] * user_cmd.m_move.y;
			pure_target_dir[ 2 ] = crypt_float ( 0.f );

			vec3_t curr_target_dir;
			for ( auto i = 0u; i < 2; i++ )
				curr_target_dir[ i ] = curr_fwd[ i ] * user_cmd.m_move.x + curr_right[ i ] * user_cmd.m_move.y;
			curr_target_dir[ 2 ] = crypt_float ( 0.f );

			if ( !compare_vec ( pure_target_dir, curr_target_dir ) ) {
				user_cmd.m_move.x = ( pure_target_dir.x * curr_right.y - curr_right.x * pure_target_dir.y ) / ( curr_right.y * curr_fwd.x - curr_right.x * curr_fwd.y );
				user_cmd.m_move.y = ( pure_target_dir.y * curr_fwd.x - curr_fwd.y * pure_target_dir.x ) / ( curr_right.y * curr_fwd.x - curr_right.x * curr_fwd.y );
			}
		}
		user_cmd.m_buttons &= ~(
			valve::e_buttons::in_forward
			| valve::e_buttons::in_back
			| valve::e_buttons::in_move_right
			| valve::e_buttons::in_move_left
		);

		if ( move_type == valve::e_move_type::ladder ) {
			if ( std::abs( user_cmd.m_move.x ) > 200.f )
				user_cmd.m_buttons |=
					user_cmd.m_move.x > 0.f
					? valve::e_buttons::in_forward : valve::e_buttons::in_back;

			if ( std::abs( user_cmd.m_move.y ) <= 200.f )
				return;

			user_cmd.m_buttons |=
				user_cmd.m_move.y > 0.f
				? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;

			return;
		}

		if ( hacks::g_aim_bot->cfg ( ).m_bSlideWalk
			&& move_type == valve::e_move_type::walk ) {
			if ( user_cmd.m_move.x != 0.f )
				user_cmd.m_buttons |=
					user_cmd.m_move.x < 0.f
					? valve::e_buttons::in_forward : valve::e_buttons::in_back;

			if ( user_cmd.m_move.y == 0.f )
				return;

			user_cmd.m_buttons |=
				user_cmd.m_move.y < 0.f
				? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;

			return;
		}

		if ( user_cmd.m_move.x != 0.f )
			user_cmd.m_buttons |=
				user_cmd.m_move.x > 0.f
				? valve::e_buttons::in_forward : valve::e_buttons::in_back;

		if ( user_cmd.m_move.y == 0.f )
			return;

		user_cmd.m_buttons |=
			user_cmd.m_move.y > 0.f
			? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;
	}

	__forceinline bool is_zero( vec3_t vec )
	{
		return (vec.x > -0.01f && vec.x < 0.01f &&
			vec.y > -0.01f && vec.y < 0.01f &&
			vec.z > -0.01f && vec.z < 0.01f);
	}

	__forceinline float dist_sqr ( const vec3_t from, const vec3_t& v )
	{
		return ( ( from - v ).length_sqr ( ) );
	}

	void c_movement::auto_peek(qangle_t& wish_ang, valve::user_cmd_t& user_cmd)
	{
		if ( !valve___g_local_player )
			return;

		if ( !valve::g_local_player->weapon ( ) )
			return;

		if ( valve::g_local_player->weapon ( )->is_knife ( ) )
			return;

		if ( valve::g_local_player->weapon ( )->item_index ( ) == valve::e_item_index::taser )
			return;

		if ( !valve::g_local_player->weapon ( )->wpn_data ( ) )
			return;

		if ( valve::g_local_player->weapon ( )->wpn_data ( )->m_type == 9 )
			return;

		if ( hacks::g_aim_bot->cfg ( ).m_bAutoPeek )
		{
			if (is_zero ( g_context->get_auto_peek_info().m_start_pos ) )
			{
				g_context->get_auto_peek_info().m_start_pos = valve___g_local_player->abs_origin ( );

				if (! ( valve___g_local_player->flags ( ) & valve::e_ent_flags::on_ground ) )
				{
					valve::ray_t* ray = new valve::ray_t(g_context->get_auto_peek_info().m_start_pos, g_context->get_auto_peek_info().m_start_pos - vec3_t(0.0f, 0.0f, 1000.0f));
					valve::trace_filter_world_only_t filter;
					valve::trace_t trace;

					valve___g_engine_trace->trace_ray(*ray, valve::e_mask::solid, &filter, &trace);

					if (trace.m_fraction < 1.f)
					{
						g_context->get_auto_peek_info().m_start_pos = trace.m_end_pos + vec3_t(0.f, 0.f, 2.f);
					}
				}
			}
			else
			{
				if (user_cmd.m_buttons & valve::e_buttons::in_attack && valve::g_local_player->weapon ( ) )
				{
					if ( valve::g_local_player->weapon ( )->item_index ( ) != valve::e_item_index::revolver )
					g_context->get_auto_peek_info().m_is_firing = true;
				}

				if ( g_context->get_auto_peek_info().m_is_firing )
				{
					auto current_position = valve___g_local_player->abs_origin ( );
					auto difference = current_position - g_context->get_auto_peek_info().m_start_pos;

					if ( difference.length_2d ( ) > 0.5f )
					{
						user_cmd.m_buttons &= ~valve::e_buttons::in_jump;
						const auto chocked_ticks = ( user_cmd.m_number % 2 ) != 1 ? ( crypt_int ( 14 ) - valve::g_client_state->m_choked_cmds ) : valve::g_client_state->m_choked_cmds;
						wish_ang = math::calc_ang ( valve::g_local_player->abs_origin ( ), g_context->get_auto_peek_info ( ).m_start_pos );

						static auto cl_forwardspeed = valve::g_cvar->find_var ( xorstr_ ( "cl_forwardspeed" ) );
						user_cmd.m_move.x = cl_forwardspeed->get_float ( ) - ( 1.2f * chocked_ticks );
						user_cmd.m_move.y = 0.0f;
					}
					else
					{
						g_context->get_auto_peek_info().m_is_firing = false;
						g_context->get_auto_peek_info().m_start_pos = vec3_t();
					}

					rotate (
						user_cmd, wish_ang,
						valve___g_local_player->flags ( ),
						valve___g_local_player->move_type ( )
					);
				}
			}
		}
		else
		{
			g_context->get_auto_peek_info().m_is_firing = false;
			g_context->get_auto_peek_info().m_start_pos = vec3_t();
		}
	}

	void c_movement::on_create_move( valve::user_cmd_t& user_cmd ) {
		if ( valve___g_local_player->move_type( ) != valve::e_move_type::walk )
			return;

		if ( valve::g_local_player &&
			valve::g_local_player->weapon ( )
			&& valve::g_local_player->weapon ( )->wpn_data ( ) ) {
			m_max_player_speed = m_max_weapon_speed = valve::g_local_player->scoped ( ) ?
				valve::g_local_player->weapon ( )->wpn_data ( )->m_max_speed_alt :
				valve::g_local_player->weapon ( )->wpn_data ( )->m_max_speed;
		}
		else {
			m_max_player_speed = m_max_weapon_speed = 260.f;
		}

		m_velocity = valve___g_local_player->velocity( );

		predict_move( user_cmd, m_velocity );

		if ( hacks::g_aim_bot->cfg ( ).m_bInfinityDuck )
			user_cmd.m_buttons |= valve::e_buttons::in_bullrush;

		if ( m_should_fake_duck ) {
			if ( g_exploits->type( ) ) {
				if (valve___g_local_player->duck_amount() > 0.5f)
					user_cmd.m_buttons |= valve::e_buttons::in_duck;
				else
					user_cmd.m_buttons &= ~valve::e_buttons::in_duck;
			}
			else {
				user_cmd.m_buttons |= valve::e_buttons::in_bullrush;

				const auto pred_duck_amount = std::min( 1.f, valve___g_local_player->duck_speed( ) * valve::to_time( 7 ) );
				if ( valve___g_local_player->duck_amount( ) <= pred_duck_amount ) {
					if ( valve___g_client_state->m_choked_cmds < 7 )
						user_cmd.m_buttons &= ~valve::e_buttons::in_duck;
					else
						user_cmd.m_buttons |= valve::e_buttons::in_duck;

					if ( g_context->flags( ) & e_context_flags::can_choke )
						g_context->flags( ) |= e_context_flags::choke;
					else
						g_context->flags( ) &= ~e_context_flags::choke;
				}
				else {
					user_cmd.m_buttons &= ~valve::e_buttons::in_duck;

					g_context->flags( ) &= ~e_context_flags::choke;
				}
			}
		}


		const auto stopped = stop ( user_cmd );

		if ( !stopped ) {
			if ( hacks::g_aim_bot->cfg ( ).m_bSlowWalk
				&& !( user_cmd.m_buttons & valve::e_buttons::in_jump ) 
				&& valve___g_local_player->flags( ) & valve::e_ent_flags::on_ground 
				&& ( user_cmd.m_move.x != 0.f || user_cmd.m_move.y != 0.f )
				&& 30.f <= m_velocity.length( ) ) {
				m_max_player_speed = m_max_weapon_speed = 30.f;

				auto velocity = valve___g_local_player->velocity( );

				modify_move( user_cmd, velocity );
			}

			if ( hacks::g_aim_bot->cfg ( ).m_bBunnyHop
				&& user_cmd.m_buttons & valve::e_buttons::in_jump
				&& !g_context->cvars( ).m_sv_auto_bhop->get_bool( ) ) {
				if ( valve___g_local_player->flags( ) & valve::e_ent_flags::on_ground )
					user_cmd.m_buttons |= valve::e_buttons::in_jump;
				else
					user_cmd.m_buttons &= ~valve::e_buttons::in_jump;

			}

			auto_strafe( user_cmd );
		}

		fast_stop ( user_cmd );

		user_cmd.m_buttons &= ~(
			valve::e_buttons::in_forward
			| valve::e_buttons::in_back
			| valve::e_buttons::in_move_right
			| valve::e_buttons::in_move_left
		);

		if ( hacks::g_aim_bot->cfg ( ).m_bSlideWalk
			&& valve___g_local_player->move_type( ) == valve::e_move_type::walk ) {
			if ( user_cmd.m_move.x != 0.f )
				user_cmd.m_buttons |=
					user_cmd.m_move.x < 0.f
						? valve::e_buttons::in_forward : valve::e_buttons::in_back;

			if ( user_cmd.m_move.y == 0.f )
				return;

			user_cmd.m_buttons |=
				user_cmd.m_move.y < 0.f
					? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;

			return;
		}

		if ( user_cmd.m_move.x != 0.f )
			user_cmd.m_buttons |=
				user_cmd.m_move.x > 0.f
					? valve::e_buttons::in_forward : valve::e_buttons::in_back;

		if ( user_cmd.m_move.y == 0.f )
			return;

		user_cmd.m_buttons |=
			user_cmd.m_move.y > 0.f
				? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;
	}
}