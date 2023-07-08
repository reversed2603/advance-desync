#include "../../../nem.hpp"

#include "..\..\..\cloud\cloud.h"
#include <sstream>
enum e_cs_weapon_type {
	WEAPON_TYPE_KNIFE,
	WEAPON_TYPE_PISTOL,
	WEAPON_TYPE_SMG,
	WEAPON_TYPE_RIFLE,
	WEAPON_TYPE_SHOTGUN,
	WEAPON_TYPE_SNIPER,
	WEAPON_TYPE_MACHINE_GUN,
	WEAPON_TYPE_C4,
	WEAPON_TYPE_PLACE_HOLDER,
	WEAPON_TYPE_GRENADE,
	WEAPON_TYPE_UNKNOWN,
	WEAPON_TYPE_MEDISHOT,
	WEAPON_TYPE_FISTS,
	WEAPON_TYPE_BREACH_CHARGE,
	WEAPON_TYPE_BUMP_MINE,
	WEAPON_TYPE_TABLET,
	WEAPON_TYPE_MELEE
};
namespace nem::hacks {

	std::vector < valve::c_entity* > broken_entities {};

	void clear_broken_entities ( ) { broken_entities.clear ( ); }

	void mark_entity_as_broken ( valve::c_entity* entity ) { broken_entities.emplace_back ( entity ); }

     bool c_visuals::is_entity_broken ( valve::c_entity* entity ) {
		return std::find ( broken_entities.begin ( ), broken_entities.end ( ), entity ) != broken_entities.end ( );
	}

	void c_visuals::grenade_simulation_t::predict( const vec3_t& origin, const vec3_t& velocity, const float throw_time, const int offset ) {
		m_origin = origin;
		m_velocity = velocity;
		m_collision_group = 13;
		clear_broken_entities ( );

		const auto tick = valve::to_ticks( 1.f / 30.f );

		m_last_update_tick = -tick;

		switch ( m_index ) {
		case valve::e_item_index::smoke_grenade: m_next_think_tick = valve::to_ticks( 1.5f ); break;
		case valve::e_item_index::decoy: m_next_think_tick = valve::to_ticks( 2.f ); break;
		case valve::e_item_index::flashbang:
		case valve::e_item_index::he_grenade:
			m_detonate_time = 1.5f;
			m_next_think_tick = valve::to_ticks( 0.02f );

			break;
		case valve::e_item_index::molotov:
		case valve::e_item_index::inc_grenade:
			m_detonate_time = g_context->cvars( ).m_molotov_throw_detonate_time->get_float( );
			m_next_think_tick = valve::to_ticks( 0.02f );

			break;
		}

		m_source_time = throw_time;

		const auto max_sim_amt = valve::to_ticks( 60.f );
		for ( ; m_tick < max_sim_amt; ++m_tick ) {
			if ( m_next_think_tick <= m_tick )
				think( );

			if ( m_tick < offset )
				continue;

			if ( physics_simulate( ) )
				break;

			if ( ( m_last_update_tick + tick ) > m_tick )
				continue;

			update_path( false );
		}

		if ( ( m_last_update_tick + tick ) <= m_tick )
			update_path( false );

		m_expire_time = throw_time + valve::to_time( m_tick );
	}

	bool c_visuals::grenade_simulation_t::physics_simulate( ) {
		if ( m_detonated )
			return true;

		const auto new_velocity_z = m_velocity.z - ( g_context->cvars( ).m_sv_gravity->get_float( ) * 0.4f ) * valve___g_global_vars->m_interval_per_tick;

		auto move = m_velocity;

		move.z += new_velocity_z;
		move.z /= 2.f;

		move *= valve___g_global_vars->m_interval_per_tick;

		m_velocity.z = new_velocity_z;

		valve::trace_t trace{};

		physics_push_entity( move, trace );

		if ( m_detonated )
			return true;

		if ( trace.m_fraction != 1.f ) {
			update_path( true );

			perform_fly_collision_resolution( trace );
		}

		return false;
	}

	void c_visuals::grenade_simulation_t::physics_trace_entity(
		const vec3_t& src, const vec3_t& dst,
		const valve::e_mask mask, valve::trace_t& trace
	) {
		valve::trace_filter_skip_two_entities_t trace_filter { m_owner, m_last_breakable, m_collision_group };

		valve___g_engine_trace->trace_ray(
			{ src, dst, { -2.f, -2.f, -2.f }, { 2.f, 2.f, 2.f } }, mask,
			reinterpret_cast< valve::trace_filter_t* >( &trace_filter ), &trace
		);

		if ( trace.m_start_solid
			&& ( trace.m_contents & valve::e_mask::contents_current_90 ) ) {
			trace.clear( );

			valve___g_engine_trace->trace_ray(
				{ src, dst, { -2.f, -2.f, -2.f }, { 2.f, 2.f, 2.f } },
				mask & ~valve::e_mask::contents_current_90,
				reinterpret_cast< valve::trace_filter_t* >( &trace_filter ), &trace
			);
		}

		if ( !trace.hit( )
			|| !trace.m_hit_entity
			|| !trace.m_hit_entity->is_player( ) )
			return;

		trace.clear( );

		valve___g_engine_trace->trace_ray(
			{ src, dst }, mask,
			reinterpret_cast< valve::trace_filter_t* >( &trace_filter ), &trace
		);
	}

	void c_visuals::grenade_simulation_t::physics_push_entity( const vec3_t& push, valve::trace_t& trace ) {
		physics_trace_entity( m_origin, m_origin + push,
			m_collision_group == 1
			? ( valve::e_mask::solid | valve::e_mask::contents_current_90 ) & ~valve::e_mask::contents_monster
			: valve::e_mask::solid | valve::e_mask::contents_opaque
			| valve::e_mask::contents_ignore_nodraw_opaque | valve::e_mask::contents_current_90 | valve::e_mask::contents_hitbox,
			trace
		);

		valve::trace_filter_skip_two_entities_t trace_filter { m_owner, m_last_breakable, m_collision_group };

		if ( trace.m_start_solid ) {
			m_collision_group = 3;

			valve___g_engine_trace->trace_ray(
				{ m_origin - push, m_origin + push },
				( valve::e_mask::solid | valve::e_mask::contents_current_90 ) & ~valve::e_mask::contents_monster,
				reinterpret_cast< valve::trace_filter_t* >( &trace_filter ), &trace
			);
		}

		if ( trace.m_fraction != 0.f )
			m_origin = trace.m_end_pos;

		if ( !trace.m_hit_entity 
			|| trace.m_hit_entity->is_player( )
			|| ( m_index != valve::e_item_index::ta_grenade
				&& m_index != valve::e_item_index::molotov && m_index != valve::e_item_index::inc_grenade )
			|| ( m_index != valve::e_item_index::ta_grenade
				&& trace.m_plane.m_normal.z < std::cos( math::to_rad( g_context->cvars( ).m_weapon_molotov_maxdetonateslope->get_float( ) ) ) ) )
			return;

		detonate( true );
	}

	void c_visuals::grenade_simulation_t::perform_fly_collision_resolution( valve::trace_t& trace ) {
		auto surface_elasticity = 1.f;

		if ( trace.m_hit_entity ) {
			if ( trace.m_hit_entity->breakable( ) ) {
				m_last_breakable = trace.m_hit_entity;

				mark_entity_as_broken ( trace.m_hit_entity );

				m_velocity *= 0.4f;

				return;
			}

			const auto is_player = trace.m_hit_entity->is_player( );
			if ( is_player )
				surface_elasticity = 0.3f;

			if ( trace.m_hit_entity->index( ) ) {
				if ( is_player
					&& m_last_hit_entity == trace.m_hit_entity ) {
					m_collision_group = 1;

					return;
				}

				m_last_hit_entity = trace.m_hit_entity;
			}
		}

		vec3_t velocity{};

		const auto back_off = m_velocity.dot( trace.m_plane.m_normal ) * 2.f;

		for ( std::size_t i{}; i < 3u; ++i ) {
			const auto change = trace.m_plane.m_normal[ i ] * back_off;

			velocity[ i ] = m_velocity[ i ] - change;

			if ( std::fabsf( velocity[ i ] ) >= 1.f )
				continue;

			velocity[ i ] = 0.f;
		}

		velocity *= std::clamp( surface_elasticity * 0.45f, 0.f, 0.9f );

		if ( trace.m_plane.m_normal.z > 0.7f ) {
			const auto speed_sqr = velocity.length_sqr( );
			if ( speed_sqr > 96000.f ) {
				const auto l = velocity.normalized( ).dot( trace.m_plane.m_normal );
				if ( l > 0.5f )
					velocity *= 1.f - l + 0.5f;
			}

			if ( speed_sqr < 400.f )
				m_velocity = {};
			else {
				m_velocity = velocity;

				physics_push_entity( velocity * ( ( 1.f - trace.m_fraction ) * valve___g_global_vars->m_interval_per_tick ), trace );
			}
		}
		else {
			m_velocity = velocity;

			physics_push_entity( velocity * ( ( 1.f - trace.m_fraction ) * valve___g_global_vars->m_interval_per_tick ), trace );
		}

		if ( m_bounces_count > 20 )
			return detonate( false );

		++m_bounces_count;
	}

	void c_visuals::grenade_simulation_t::think( ) {
		switch ( m_index ) {
		case valve::e_item_index::smoke_grenade:
			if ( m_velocity.length_sqr( ) <= 0.01f )
				detonate( false );

			break;
		case valve::e_item_index::decoy:
			if ( m_velocity.length_sqr( ) <= 0.04f )
				detonate( false );

			break;
		case valve::e_item_index::flashbang:
		case valve::e_item_index::he_grenade:
		case valve::e_item_index::molotov:
		case valve::e_item_index::inc_grenade:
			if ( valve::to_time( m_tick ) > m_detonate_time )
				detonate( false );

			break;
		}

		m_next_think_tick = m_tick + valve::to_ticks( 0.2f );
	}

	void c_visuals::grenade_simulation_t::detonate( const bool bounced ) {
		m_detonated = true;

		update_path( bounced );
	}

	void c_visuals::grenade_simulation_t::update_path( const bool bounced ) {
		m_last_update_tick = m_tick;

		if ( m_path.size ( ) < 200 )
		m_path.emplace_back( m_origin, bounced );
	}

	__forceinline float csgo_armor ( float flDamage, int ArmorValue )
	{
		float flArmorRatio = 0.5f;
		float flArmorBonus = 0.5f;
		if ( ArmorValue > 0 ) {
			float flNew = flDamage * flArmorRatio;
			float flArmor = ( flDamage - flNew ) * flArmorBonus;

			if ( flArmor > static_cast< float >( ArmorValue ) ) {
				flArmor = static_cast< float >( ArmorValue ) * ( 1.f / flArmorBonus );
				flNew = flDamage - flArmor;
			}

			flDamage = flNew;
		}
		return flDamage;
	}

	bool c_visuals::add_grenade_simulation( const grenade_simulation_t& sim, const bool warning ) const {

		const auto points_count = sim.m_path.size ( );
		if ( points_count < 2u
			|| valve::g_global_vars->m_cur_time >= sim.m_expire_time )
			return false;

		const auto& screen_size = ImGui::GetIO ( ).DisplaySize;
		if ( warning ) {
			const auto& explode_pos = sim.m_path.back ( ).first;
			auto dist = ( valve::g_local_player->origin ( ) - explode_pos ).length ( );

			vec2_t prev_screen_pos {};
			auto prev_on_screen = math::to_screen ( sim.m_path.front ( ).first,
				screen_size, valve::g_engine->w2s_matrix ( ), prev_screen_pos
			);

			const auto mod = std::clamp (
				( sim.m_expire_time - valve::g_global_vars->m_cur_time )
				/ valve::to_time ( sim.m_tick ),
				0.f, 1.f
			);

			for ( auto i = 1u; i < points_count; ++i ) {
				vec2_t cur_screen_pos {};
				const auto cur_on_screen = math::to_screen ( sim.m_path.at ( i ).first,
					screen_size, valve::g_engine->w2s_matrix ( ), cur_screen_pos
				);
				if ( prev_on_screen
					&& cur_on_screen ) {
					render::line ( prev_screen_pos, cur_screen_pos, col_t ( 255, 255, 255, 255 * mod ) );

				}
				prev_screen_pos = cur_screen_pos;
				prev_on_screen = cur_on_screen;
			}

			vec2_t screen_pos {};
			const auto on_screen = math::to_screen ( explode_pos, screen_size, valve::g_engine->w2s_matrix ( ), screen_pos );
			if ( !on_screen
				&& dist > 400.f )
				return true;

			const auto unk = screen_size / 18.f;
			if ( !on_screen
				|| screen_pos.x < -unk.x
				|| screen_pos.x > ( screen_size.x + unk.x )
				|| screen_pos.y < -unk.y
				|| screen_pos.y > ( screen_size.y + unk.y ) ) {
				vec3_t dir {};
				math::angle_vectors ( valve::g_view_render->m_setup.m_angles, &dir );

				dir.z = 0.f;
				dir.normalize ( );

				const auto radius = 210.f * ( screen_size.y / 480.f );

				auto delta = explode_pos - valve::g_view_render->m_setup.m_origin;

				delta.normalize ( );

				screen_pos.x = radius * -delta.dot ( vec3_t { 0.f, 0.f, 1.f }.cross ( dir ) );
				screen_pos.y = radius * -delta.dot ( dir );

				const auto radians = math::to_rad (
					-math::to_deg ( std::atan2 ( screen_pos.x, screen_pos.y ) + math::k_pi )
				);

				screen_pos.x = static_cast< int >( screen_size.x / 2.f + radius * std::sin ( radians ) );
				screen_pos.y = static_cast< int >( screen_size.y / 2.f - radius * std::cos ( radians ) );
			}

			render::rect_filled ( screen_pos - vec2_t ( 12, 5 ), vec2_t ( 42.f, 42.f ), col_t ( 1.f, 1.f, 1.f, 80.f ), 2.f, true );
			render::rect ( screen_pos - vec2_t ( 12, 5 ), vec2_t ( 42.f, 42.f ), col_t ( 255, 255, 255, 255 ), 2.f, true );
			std::string icon = "";
			switch ( sim.m_index )
			{
			case valve::e_item_index::he_grenade: icon = xorstr_ ( "j" ); break;
			case valve::e_item_index::smoke_grenade: icon = xorstr_ ( "k" ); break;
			case valve::e_item_index::flashbang: icon = xorstr_ ( "i" ); break;
			case valve::e_item_index::decoy: icon = xorstr_ ( "m" ); break;
			case valve::e_item_index::inc_grenade: icon = xorstr_ ( "n" ); break;
			case valve::e_item_index::molotov: icon = xorstr_ ( "l" ); break;
			}

			render::text ( icon, vec2_t ( screen_pos.x, screen_pos.y - 2 ), col_t ( 255, 255, 255, 255 ), m_fonts.m_warning_icon_font, FONT_OUTLINE );

			constexpr auto k_meters_per_inch = 0.0254f;
			constexpr auto k_feet_per_meter = 3.281f;

			dist *= k_meters_per_inch * k_feet_per_meter;

			int offset { 2 };

			if ( dist >= 100 )
				offset += 3;

			render::text ( std::format ( ( "{} u" ), static_cast< int >( dist ) ).data ( ), vec2_t ( screen_pos.x - offset, screen_pos.y ) - vec2_t ( 0, -21 ), col_t ( 255, 255, 255, 255 ), m_fonts.m_verdana, FONT_OUTLINE );

			return true;
		}

		vec2_t prev_screen_pos {};
		auto prev_on_screen = math::to_screen ( sim.m_path.front ( ).first,
			screen_size, valve::g_engine->w2s_matrix ( ), prev_screen_pos
		);

		for ( auto i = 1u; i < points_count; ++i ) {
			vec2_t cur_screen_pos {};
			const auto cur_on_screen = math::to_screen ( sim.m_path.at ( i ).first,
				screen_size, valve::g_engine->w2s_matrix ( ), cur_screen_pos
			);

			if ( prev_on_screen
				&& cur_on_screen ) {
				render::line ( prev_screen_pos, cur_screen_pos, col_t ( 255, 255, 255, 255 ) );
				if ( i == points_count - 1 )
					render::render_3d_circle ( std::get < vec3_t > ( sim.m_path.at ( i ) ), 32, col_t ( 255, 255, 255, 255 ) );
			}

			prev_screen_pos = cur_screen_pos;
			prev_on_screen = cur_on_screen;
		}

		return true;
	}

	bool c_visuals::calc_bbox( valve::c_player* const player, bbox_t& bbox ) const {
		auto origin = player->origin( );
		
		vec2_t bottom{};
		const auto on_screen = math::to_screen( origin, ImGui::GetIO( ).DisplaySize, valve___g_engine->w2s_matrix( ), bottom );

		origin.z += player->obb_max( ).z;

		if ( !math::to_screen( origin, ImGui::GetIO( ).DisplaySize, valve___g_engine->w2s_matrix( ), bbox.m_min ) && !on_screen )
			return false;

		bbox.m_size.y = bottom.y - bbox.m_min.y + 6;
		bbox.m_size.x = bbox.m_size.y / 2.f;

		bbox.m_min.x -= bbox.m_size.x / 2.f;

		return true;
	}

	std::optional< valve::bones_t > c_visuals::try_to_lerp_bones( const int index ) const {
		const auto& entry = g_lag_comp->entry( index - 1 );
		if ( entry.m_lag_records.size( ) < 2u )
			return std::nullopt;

		const auto max_unlag = g_context->cvars( ).m_sv_maxunlag->get_float( );

		const auto nci = valve___g_engine->net_channel_info( );
		const auto total_latency = std::clamp( nci->avg_latency( 0 ) + nci->avg_latency( 1 ), 0.f, max_unlag );
		const auto correct = total_latency + g_context->net_info( ).m_lerp;

		const auto end = entry.m_lag_records.end( );
		for ( auto it = entry.m_lag_records.begin( ); it != end; ++it ) {
			const auto current = it->get( );

			const auto latest = std::next( it ) == end;

			if ( current && current->valid( )
				&& ( !latest && ( std::next( it )->get( ) ) ) ) {
				if ( ( current->m_origin - entry.m_player->origin( ) ).length( ) < 1.f )
					continue;

				const auto next_origin = latest ? entry.m_player->origin( ) : std::next( it )->get( )->m_origin;
				const auto next_time = latest ? entry.m_player->sim_time( ) : std::next( it )->get( )->m_sim_time;

				const auto add = latest ? 1.f : ( next_time - current->m_sim_time );
				const auto delta = ( current->m_sim_time + correct + add ) - valve___g_global_vars->m_cur_time;

				const auto mult = 1.f / add;
				const auto lerp_amt = std::clamp( delta * mult, 0.f, 1.f );

				const vec3_t lerped_origin{
					std::lerp( next_origin.x, current->m_origin.x, lerp_amt ),
					std::lerp( next_origin.y, current->m_origin.y, lerp_amt ),
					std::lerp( next_origin.z, current->m_origin.z, lerp_amt )
				};

				const auto& anim_side = current->m_anim_sides.at( current->m_anim_side );

				auto lerped_bones = anim_side.m_bones;

				const auto origin_delta = lerped_origin - current->m_origin;

				for ( std::size_t i{}; i < lerped_bones.size( ); ++i ) {
					lerped_bones[ i ][ 0 ][ 3 ] += origin_delta.x;
					lerped_bones[ i ][ 1 ][ 3 ] += origin_delta.y;
					lerped_bones[ i ][ 2 ][ 3 ] += origin_delta.z;
				}

				return lerped_bones;
			}
		}

		return std::nullopt;
	}

	valve::material_t* c_visuals::create_material(
		const std::string_view name,
		const std::string_view shader, const std::string_view data
	) const {
		using init_t = void( __thiscall* )( const std::uintptr_t, const char* const, 
			std::ptrdiff_t, std::ptrdiff_t );

		using load_from_buffer_t = void( __thiscall* )(
			const std::uintptr_t, const char* const, const char* const,
			const std::uintptr_t, const std::uintptr_t, const std::uintptr_t, const std::uintptr_t
		);

		const auto key_values = reinterpret_cast< std::uintptr_t >( std::malloc( 36u ) );

		reinterpret_cast< init_t >( g_context->addresses( ).m_key_values_init.get( ) )( key_values, shader.data( ), 0, 0 );

		reinterpret_cast< load_from_buffer_t >( g_context->addresses( ).m_key_values_load_from_buffer.get( ) )(
			key_values, name.data( ), data.data( ), 0, 0, 0, 0
		);

		return valve___g_mat_system->create_material( name.data( ), key_values );
	}

	void c_visuals::override_material(
		const int type, const bool ignore_z,
		const float r, const float g, const float b, const float a
	) const {
		static const auto solid_mat = create_material(
			xorstr_( "desync_solid.vmt" ),
			xorstr_( "UnlitGeneric" ),
			xorstr_(
				R"#("UnlitGeneric" {
					"$basetexture" "vgui/white_additive"
					"$ignorez"      "0"
					"$model"		"1"
					"$flat"			"1"
					"$nocull"		"1"
					"$selfillum"	"1"
					"$halflambert"	"1"
					"$nofog"		"1"
					"$wireframe"	"0"
				})#"
			)
		);

		static const auto regular_mat = create_material(
			xorstr_( "desync_regular.vmt" ),
			xorstr_( "VertexLitGeneric" ),
			xorstr_(
				R"#("VertexLitGeneric" {
					"$basetexture" "vgui/white_additive"
					"$ignorez"      "0"
					"$model"		"1"
					"$flat"			"0"
					"$nocull"		"1"
					"$halflambert"	"1"
					"$nofog"		"1"
					"$wireframe"	"0"
				})#"
			)
		);

		static const auto glow_mat = create_material(
			xorstr_( "desync_glow.vmt" ),
			xorstr_( "VertexLitGeneric" ),
			xorstr_(
				R"#("VertexLitGeneric" {
					"$additive"					"1"
					"$envmap"					"models/effects/cube_white"
					"$envmaptint"				"[1 1 1]"
					"$envmapfresnel"			"1"
					"$envmapfresnelminmaxexp"	"[0 1 2]"
					"$alpha"					"1"
				})#"
			)
		);

		static const auto metallic_mat = create_material(
			xorstr_( "desync_metallic.vmt" ),
			xorstr_( "VertexLitGeneric" ),
			xorstr_(
				R"#("VertexLitGeneric" {
					 "$basetexture"					"vgui/white_additive"
					 "$ignorez"						"0"
					 "$envmap"						"env_cubemap"
					 "$normalmapalphaenvmapmask"	"1"
					 "$envmapcontrast"				"1"
					 "$nofog"						"1"
					 "$model"						"1"
					 "$nocull"						"0"
					 "$selfillum"					"1"
					 "$halflambert"					"1"
					 "$znearer"						"0"
					 "$flat"						"1" 
				})#"
			)
		);

		static const auto glow_overlay = valve___g_mat_system->find_material ( xorstr_ ( "dev/glow_armsrace" ), nullptr );

		valve::material_t* material{};

		switch ( type ) {
		case 0: material = regular_mat; break;
		case 1: material = solid_mat; break;
		case 2: material = glow_mat; break;
		case 3: material = metallic_mat; break;
		case 4: material = glow_overlay; break;
		}

		material->set_alpha( a );
		material->set_clr( r, g, b );
		material->set_flag( 1u << 15u, ignore_z );

		if ( const auto $envmaptint = material->find_var( xorstr_( "$envmaptint" ), nullptr, false ) )
			$envmaptint->set_vec( r, g, b );

		valve___g_studio_render->override_material( material );
	}

	const char* c_visuals::char_code_for_item_index( const valve::e_item_index index ) const {
		return nullptr;
	}

	void c_visuals::add_line(
		const vec2_t& from, const vec2_t& to, const std::uint32_t clr
	) const {
		valve___g_surface->set_clr( clr );
		valve___g_surface->add_line( from, to );
	}

	void c_visuals::add_rect(
		const vec2_t& pos, const vec2_t& size, const std::uint32_t clr
	) const {
		valve___g_surface->set_clr( clr );
		valve___g_surface->add_rect( pos, pos + size );
	}

	void c_visuals::add_rect_filled(
		const vec2_t& pos, const vec2_t& size, const std::uint32_t clr
	) const {
		valve___g_surface->set_clr( clr );
		valve___g_surface->add_rect_filled( pos, pos + size );
	}

	void c_visuals::bloom_modulation ( valve::c_entity* entity )
	{
		if ( !hacks::g_aim_bot->cfg ( ).m_bToneMapModulation )
			return;

		entity->get_use_custom_bloom_scale ( ) = true;
		entity->get_custom_bloom_scale ( ) = hacks::g_aim_bot->cfg ( ).m_iBloom * 0.01f;

		entity->get_user_custom_auto_exposure_min ( ) = true;
		entity->get_auto_custom_exposure_min ( ) = hacks::g_aim_bot->cfg ( ).m_iExposure * 0.001f;

		entity->get_user_custom_auto_exposure_max ( ) = true;
		entity->get_auto_custom_exposure_max ( ) = hacks::g_aim_bot->cfg ( ).m_iExposure * 0.001f;
	}

	void c_visuals::change_shadows ( valve::c_entity* entity )
	{
		if ( !hacks::g_aim_bot->cfg ( ).m_bShadowsModulation )
			return;

		auto ent = reinterpret_cast < valve::c_cascade_light* > ( entity );

		ent->get_shadow_direction ( ) = vec3_t ( hacks::g_aim_bot->cfg ( ).m_iShadowsX, hacks::g_aim_bot->cfg ( ).m_iShadowsY, hacks::g_aim_bot->cfg ( ).m_iShadowsZ );
	}

	void c_visuals::add_text(
		vec2_t pos, const unsigned long font,
		const std::string_view str, const std::uint32_t clr, const int flags
	) const {
		const auto wstr = util::to_utf16( str );

		if ( flags ) {
			int width{}, height{};
			valve___g_font_mgr->calc_text_size( font, wstr.data( ), width, height );

			if ( flags & 1 )
				pos.x -= width / 2;

			if ( flags & 2 )
				pos.y -= height / 2;

			if ( flags & 4 )
				pos.x -= width;
		}

		valve___g_surface->set_text_pos( pos );
		valve___g_surface->set_text_clr( clr );
		valve___g_surface->set_text_font( font );

		valve___g_surface->add_text( wstr.data( ), wstr.size( ) );
	}

	void c_visuals::add_text(
		vec2_t pos, const unsigned long font,
		const std::wstring_view str, const std::uint32_t clr, const int flags
	) const {
		if ( flags ) {
			int width{}, height{};
			valve___g_font_mgr->calc_text_size( font, str.data( ), width, height );

			if ( flags & 1 )
				pos.x -= width / 2;

			if ( flags & 2 )
				pos.y -= height / 2;

			if ( flags & 4 )
				pos.x -= width;
		}

		valve___g_surface->set_text_pos( pos );
		valve___g_surface->set_text_clr( clr );
		valve___g_surface->set_text_font( font );

		valve___g_surface->add_text( str.data( ), str.size( ) );
	}

	void c_visuals::add_vertices(
		const valve::vertex_t* const vertices,
		const std::size_t vertices_count, const std::uint32_t clr
	) const {
		static const auto id = valve___g_surface->create_new_texture( true );
		static const std::uint8_t clr_buffer[ 4 ]{ 0xff, 0xff, 0xff, 0xff };

		valve___g_surface->set_texture_rgba( id, clr_buffer, 1, 1 );

		valve___g_surface->set_clr( clr );
		valve___g_surface->set_texture( id );

		valve___g_surface->add_textured_polygon( vertices_count, vertices );
	}

	void c_visuals::add_3d_circle(const vec3_t& origin, float radius, const std::uint32_t clr)
	{
		static vec2_t previous_screen_pos = vec2_t(0, 0);
		static float_t step = 3.14159265358979323846f * 2.0f / 72.0f;

		vec2_t screen_position = vec2_t(0, 0 );
		if (!math::to_screen(origin, ImGui::GetIO().DisplaySize, valve___g_engine->w2s_matrix(), screen_position))
			return;

		for (float_t rotation = 0.0f; rotation <= 3.14159265358979323846f * 2.0f; rotation += step)
		{
			vec3_t world_position = vec3_t(radius * cos(rotation) + origin.x, radius * sin(rotation) + origin.y, origin.z);
			if (!math::to_screen(world_position, ImGui::GetIO().DisplaySize, valve___g_engine->w2s_matrix(), screen_position))
				continue;

			add_line(vec2_t ( previous_screen_pos.x, previous_screen_pos.y ), vec2_t ( screen_position.x, screen_position.y ), clr);

			previous_screen_pos = screen_position;
		}
	}

	void c_visuals::add_circle(
		const vec2_t& pos, const float radius,
		const int segments_count, const std::uint32_t clr
	) const {
		std::vector< int > x_points{}, y_points{};

		const auto step = math::k_pi2 / segments_count;
		for ( auto i = 0.f; i < math::k_pi2; i += step ) {
			x_points.emplace_back( pos.x + radius * std::cos( i ) );
			y_points.emplace_back( pos.y + radius * std::sin( i ) );
		}

		valve___g_surface->set_clr( clr );
		valve___g_surface->add_poly_line( x_points.data( ), y_points.data( ), x_points.size( ) );
	}

	void c_visuals::add_filled_circle(
		const vec2_t& pos, const float radius,
		const int segments_count, const std::uint32_t clr
	) const {
		static const auto id = valve___g_surface->create_new_texture( true );
		static const std::uint8_t clr_buffer[ 4 ]{ 0xff, 0xff, 0xff, 0xff };

		valve___g_surface->set_texture_rgba( id, clr_buffer, 1, 1 );

		valve___g_surface->set_clr( clr );
		valve___g_surface->set_texture( id );

		std::vector< valve::vertex_t > vertices{};

		const auto step = math::k_pi2 / segments_count;
		for ( auto i = 0.f; i < math::k_pi2; i += step ) {
			auto& vertex = vertices.emplace_back( );

			vertex.m_pos = { pos.x + radius * std::cos( i ), pos.y + radius * std::sin( i ) };
		}

		valve___g_surface->add_textured_polygon( vertices.size( ), vertices.data( ) );
	}

	void c_visuals::add_beam_ring( const vec3_t& pos, const float* const clr ) const {
		valve::beam_info_t info{};

		info.m_type = 7;
		info.m_center = pos;
		info.m_center.z += 5.f;

		info.m_model_index = valve___g_model_info->lookup_model_index( xorstr_( "sprites/purplelaser1.vmt" ) );
		info.m_model_name = xorstr_( "sprites/purplelaser1.vmt" );
		info.m_life = 0.5f;
		info.m_width = 7.f;
		info.m_end_width = 7.f;
		info.m_brightness = 255.f;
		info.m_speed = 1.f;
		info.m_renderable = true;
		info.m_red = clr[ 0 ] * 255.f;
		info.m_green = clr[ 1 ] * 255.f;
		info.m_blue = clr[ 2 ] * 255.f;
		info.m_segments = 1;
		info.m_speed = 150.f;
		info.m_fade_length = 1.f;

		info.m_start_radius = 10.f;
		info.m_end_radius = 440.f;

		const auto beam = valve___g_beams->create_beam_ring_point( info );
		if ( !beam )
			return;

		valve___g_beams->draw_beam( beam );
	}

	void c_visuals::add_beam( const vec3_t& from, const vec3_t& to, const float* const clr ) const {
		
	}

	void c_visuals::add_grenade_warning_beam( const vec3_t& from, const vec3_t& to, const float* const clr ) const {
	
	}

	void c_visuals::oof_arrow( const vec3_t& pos ) const {
	
	}

	static DWORD* find_hud_element(const char* name)
	{
		using find_t = std::uintptr_t* (__thiscall*)(const std::uintptr_t, const char*);

		auto find_da_hud = reinterpret_cast<find_t>(g_context->addresses().m_find_hud_element.get( ) ) ( g_context->addresses ( ).m_hud.get( ), name );

		return (DWORD*)find_da_hud;
	}

	void c_visuals::preserve_killfeed()
	{
		static DWORD* death_notice = nullptr;

		if (valve___g_engine->in_game())
		{
			death_notice = find_hud_element("CCSGO_HudDeathNotice");

			if (death_notice)
			{
				auto local_death_notice = (float*)((uintptr_t)death_notice + 0x50);

				if (local_death_notice)
					*local_death_notice = hacks::g_aim_bot->cfg ( ).m_bPreserveKillfeed ? FLT_MAX : 1.5f;

				if (g_context->should_reset_k_feed())
				{
					using find_t = void (__thiscall*)(const std::uintptr_t);

					auto reset_kill_feed = reinterpret_cast < find_t >( g_context->addresses().m_clear_hud_wpn.get( ) );

					reset_kill_feed((uintptr_t)death_notice - 0x14);

					g_context->should_reset_k_feed() = false;
				}
			}
		}
		else
			death_notice = 0;
	}

	void c_visuals::handle_player( valve::c_player* const player ) const {
		if ( !player->alive( )
			|| player == valve___g_local_player )
			return;

		const auto dormant = player->dormant( );
		if ( !dormant
			&& 0 )
			player->spotted( ) = true;

		if ( ( dormant && 0 )
			|| ( player->friendly( ) ) )
			return;

		const auto weapon = player->weapon( );
		const auto wpn_data = weapon ? weapon->wpn_data( ) : nullptr;

		bbox_t bbox{};
		if ( !calc_bbox( player, bbox ) )
			return;

		std::size_t offset{};

		offset = 0u;
	}

	void molly ( valve::c_entity* ent )
	{
		auto inferno = reinterpret_cast< valve::inferno_t* >( ent );
		auto origin = inferno->abs_origin ( );

		vec3_t screen_origin;

		if ( render::world_to_screen ( origin, screen_origin ) )
		{
			auto spawn_time = inferno->get_spwn_time ( );
			auto factor = ( spawn_time + valve::inferno_t::get_expiry_time ( ) - valve___g_global_vars->m_cur_time ) / valve::inferno_t::get_expiry_time ( );

			static auto size = vec2_t ( 35.0f, 5.0f );

			render::draw_rect_filled ( screen_origin.x - size.x * 0.5f, screen_origin.y - size.y * 0.5f - 1.0f, size.x, size.y, col_t ( 37, 37, 37, 255 ), 0 );
			render::draw_rect_filled ( screen_origin.x - size.x * 0.5f + 2.0f, screen_origin.y - size.y * 0.5f, ( size.x - 4.0f ) * factor, size.y - 2.0f, col_t ( 255, 255, 255, 255 ), 0 );

			render::text ( xorstr_ ( "MOLLY" ), vec2_t ( screen_origin.x, screen_origin.y - size.y * 0.5f + 12.0f ), col_t ( 255, 255, 255, 255 ), hacks::g_visuals->m_fonts.m_skeet_font_esp, FONT_OUTLINE | FONT_CENTERED_X );
		}
	}

	void c_visuals::handle_projectile( valve::c_entity* const entity, const valve::e_class_id class_id ) {

		if ( hacks::g_aim_bot->cfg ( ).m_bGrenadeProximityWarning
			&& ( class_id == valve::e_class_id::molotov_projectile || class_id == valve::e_class_id::base_cs_grenade_projectile ) ) {

			bool can_do = true;

			if ( class_id == valve::e_class_id::base_cs_grenade_projectile ) {
				const auto studio_model = entity->model ( );
				if ( !studio_model
					|| std::string_view ( studio_model->m_name ).find ( "fraggrenade" ) == std::string::npos )
					can_do = false;
			}

			if ( !entity->dormant ( ) && can_do )
			{
				const auto handle = entity->handle ( );

				if ( entity->explode_effect_tick_begin ( ) )
					m_throwed_grenades.erase ( handle );
				else {
					if ( m_throwed_grenades.find ( handle ) == m_throwed_grenades.end ( ) ) {
						m_throwed_grenades.emplace (
							std::piecewise_construct,
							std::forward_as_tuple ( handle ),
							std::forward_as_tuple (
								static_cast< valve::c_player* >( valve___g_entity_list->find_entity ( entity->thrower ( ) ) ),
								class_id == valve::e_class_id::molotov_projectile ? valve::e_item_index::molotov : valve::e_item_index::he_grenade,
								entity->origin ( ), static_cast< valve::c_player* >( entity )->velocity ( ),
								entity->grenade_spawn_time ( ), valve::to_ticks ( entity->sim_time ( ) - entity->grenade_spawn_time ( ) )
							)
						);
					}

					if ( !add_grenade_simulation ( m_throwed_grenades.at ( handle ), true ) )
						m_throwed_grenades.erase ( handle );
				}
			}
		}

		if ( !hacks::g_aim_bot->cfg ( ).m_bGrenades )
			return;

		vec3_t screen_pos{};

		
		if ( !render::world_to_screen ( entity->origin ( ), screen_pos ) )
			return;

		switch ( class_id ) {
		case valve::e_class_id::molotov_projectile:

			render::text ( xorstr_ ( "MOLOTOV" ), vec2_t ( screen_pos.x, screen_pos.y ), col_t ( 255, 255, 255, 255 ), m_fonts.m_skeet_font_esp, FONT_OUTLINE );


			break;
		case valve::e_class_id::decoy_projectile:
			render::text ( xorstr_ ( "DECOY" ), vec2_t ( screen_pos.x, screen_pos.y ), col_t ( 255, 255, 255, 255 ), m_fonts.m_skeet_font_esp, FONT_OUTLINE );

			break;
		case valve::e_class_id::base_cs_grenade_projectile:
			if ( const auto model = entity->model( ) ) {
				const std::string_view name{ model->m_name };

				if ( name.find( xorstr_( "flashbang" ) ) != std::string::npos )
					render::text ( xorstr_ ( "FLASH" ), vec2_t ( screen_pos.x, screen_pos.y ), col_t ( 255, 255, 255, 255 ), m_fonts.m_skeet_font_esp, FONT_OUTLINE );

				else if ( name.find( xorstr_( "fraggrenade" ) ) != std::string::npos )
					render::text ( xorstr_ ( "FRAG" ), vec2_t ( screen_pos.x, screen_pos.y ), col_t ( 255, 255, 255, 255 ), m_fonts.m_skeet_font_esp, FONT_OUTLINE );
			}

			break;
		case valve::e_class_id::inferno:
			molly ( entity );

			break;
		case valve::e_class_id::smoke_grenade_projectile:
			render::text ( xorstr_ ( "SMOKE" ), vec2_t ( screen_pos.x, screen_pos.y ), col_t ( 255, 255, 255, 255 ), m_fonts.m_skeet_font_esp, FONT_OUTLINE );

			break;
		}
	}

	void c_visuals::third_person()
	{
		bool is_enable = hacks::g_aim_bot->cfg ( ).m_bThirdPerson;
		static float distance = 0.f;

		if (!valve___g_local_player)
			return;

		if (!valve___g_local_player->alive())
		{
			if (hacks::g_aim_bot->cfg ( ).m_bThirdPersonDead)
			{
				valve___g_input->m_camera_in_third_person = false;
				distance = 50.f;

				valve::g_local_player->observer_mode() = 5;
			}
			return;
		}

		if (is_enable)
			distance = hacks::g_aim_bot->cfg ( ).m_iThirdPersonDistance;
		else
			distance = std::max(distance - 6.0f, 30.0f);

		if (distance <= (is_enable ? 50.0f : 30.0f))
		{
			valve___g_input->m_camera_in_third_person = false;
			return;
		}

		vec3_t eye_pos = valve___g_local_player->abs_origin() + valve___g_local_player->view_offset();

		qangle_t view_angles = valve___g_engine->view_angles();

		valve___g_input->m_camera_in_third_person = true;
		valve___g_input->m_camera_offset = vec3_t(view_angles.x, view_angles.y, distance);

		valve::trace_t trace;

		float extent = 12.f + hacks::g_aim_bot->cfg ( ).m_iThirdPersonDistance / 4.8f - 18.f;

		vec3_t vec_forward = vec3_t(0, 0, 0);
		math::angle_vectors(qangle_t(view_angles.x, view_angles.y, 0.0f), &vec_forward);

		valve::trace_filter_world_only_t filter;

		valve::ray_t ray(eye_pos, eye_pos - vec_forward * valve___g_input->m_camera_offset.z);
		ray.m_extents = vec3_t(extent, extent, extent);
		ray.m_is_ray = false;

		valve___g_engine_trace->trace_ray(ray, valve::e_mask::solid & ~valve::e_mask::contents_monster, &filter, &trace);

		valve___g_input->m_camera_offset.z *= trace.m_fraction;

		if (valve___g_input->m_camera_offset.z < std::min(30.0f, (float)hacks::g_aim_bot->cfg ( ).m_iThirdPersonDistance))
			valve___g_input->m_camera_in_third_person = false;
	}

	__forceinline bool is_zero(vec2_t vec)
	{
		return (vec.x > -0.01f && vec.x < 0.01f &&
			vec.y > -0.01f && vec.y < 0.01f );
	}

	__forceinline bool is_zero_vec3_t(vec3_t vec)
	{
		return (vec.x > -0.01f && vec.x < 0.01f &&
			vec.y > -0.01f && vec.y < 0.01f && 
			vec.z > -0.01f && vec.z < 0.01f );
	}

	void c_visuals::draw_auto_peek()
	{
		if (!valve___g_local_player ||
			!valve___g_local_player->alive())
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


		static auto pos = vec3_t();

		if (!is_zero_vec3_t(g_context->get_auto_peek_info().m_start_pos))
			pos = g_context->get_auto_peek_info().m_start_pos;

		if (is_zero_vec3_t ( pos ) )
			return;

		static auto alpha = 4.f;

		if ( hacks::g_aim_bot->cfg ( ).m_bAutoPeek || alpha)
		{
			if ( hacks::g_aim_bot->cfg ( ).m_bAutoPeek )
				alpha += 9.0f * valve___g_global_vars->m_frame_time;
			else
				alpha -= 9.0f * valve___g_global_vars->m_frame_time;;

			alpha = std::clamp(alpha, 0.0f, 1.0f);

			static constexpr float Step = math::k_pi * 2.0f / 60;
			std::vector<ImVec2> points;
			for ( float lat = 0.f; lat <= math::k_pi * 2.0f; lat += Step )
			{
				const auto& point3d = vec3_t ( sin ( lat ), cos ( lat ), 0.f ) * 13.f * alpha;
				vec3_t point2d;
				if ( render::world_to_screen ( g_context->get_auto_peek_info ( ).m_start_pos + point3d, point2d ) )
					points.push_back ( ImVec2 ( point2d.x, point2d.y ) );
			}
			auto flags_backup = render::m_draw_list->Flags;
			render::m_draw_list->Flags |= ImDrawListFlags_AntiAliasedFill;
			render::m_draw_list->AddConvexPolyFilled ( points.data ( ), points.size ( ), col_t ( 20, 20, 20, 127 * alpha ).hex ( ) );
			render::m_draw_list->AddPolyline ( points.data ( ), points.size ( ), col_t ( 255, 255, 255, 127 * alpha ).hex ( ), true, 2.f );
			render::m_draw_list->Flags = flags_backup;
		}
	}


	void c_visuals::change_sky_box()
	{
		static auto sv_skyname = valve___g_cvar->find_var(xorstr_("sv_skyname"));
		std::string skybox = sv_skyname->m_str;

		switch ( hacks::g_aim_bot->cfg ( ).m_iSkyboxType )
		{
		case 1:
			skybox = ("cs_tibet");
			break;
		case 2:
			skybox = ("cs_baggageskybox");
			break;
		case 3:
			skybox = ("italy");
			break;
		case 4:
			skybox = ("jungle");
			break;
		case 5:
			skybox = ("office");
			break;
		case 6:
			skybox = ("sky_cs15_daylight01_hdr");
			break;
		case 7:
			skybox = ("sky_cs15_daylight02_hdr");
			break;
		case 8:
			skybox = ("vertigoblue_hdr");
			break;
		case 9:
			skybox = ("vertigo");
			break;
		case 10:
			skybox = ("sky_day02_05_hdr");
			break;
		case 11:
			skybox = ("nukeblank");
			break;
		case 12:
			skybox = ("sky_venice");
			break;
		case 13:
			skybox = ("sky_cs15_daylight03_hdr");
			break;
		case 14:
			skybox = ("sky_cs15_daylight04_hdr");
			break;
		case 15:
			skybox = ("sky_csgo_cloudy01");
			break;
		case 16:
			skybox = ("sky_csgo_night02");
			break;
		case 17:
			skybox = ("sky_csgo_night02b");
			break;
		case 18:
			skybox = ("sky_csgo_night_flat");
			break;
		case 19:
			skybox = ("sky_dust");
			break;
		case 20:
			skybox = ("vietnam");
			break;
		}

		g_context->addresses().m_sky_box.get( )(skybox.c_str());

	}

	void c_visuals::handle_weapon( valve::c_weapon* const weapon ) const {
		if ( !hacks::g_aim_bot->cfg ( ).m_iDroppedWeapon )
			return;

		const auto& origin = weapon->origin( );
		if ( origin.x == 0.f
			&& origin.y == 0.f
			&& origin.z == 0.f )
			return;

		vec3_t screen_pos{};
		if ( ! render::world_to_screen ( origin, screen_pos ) )
			return;

		if ( hacks::g_aim_bot->cfg ( ).m_iDroppedWeapon == 1 ) {
			const auto wpn_data = weapon->wpn_data( );
			if ( !wpn_data )
				return;

			auto loc_name = util::to_utf8( valve___g_localize->find_safe( wpn_data->m_hud_name ) );

			std::transform( loc_name.begin( ), loc_name.end( ), loc_name.begin( ), ::toupper );

			render::text ( loc_name.data ( ), vec2_t ( screen_pos.x, screen_pos.y ), col_t ( 255, 255, 255, 255 ), m_fonts.m_skeet_font_esp, FONT_OUTLINE );

			return;
		}

		render::text ( weapon->get_weapon_icon ( ), vec2_t ( screen_pos.x, screen_pos.y ), col_t ( 255, 255, 255, 255 ), m_fonts.m_icon_font, FONT_OUTLINE );
	}

	void c_visuals::init( ) {

		
	}

	void c_visuals::on_paint( ) {
		if ( !valve___g_engine->in_game( ) )
			return;
		const auto& screen_size = ImGui::GetIO( ).DisplaySize;
		const auto screen_center = screen_size / 2.f;

		valve::c_entity* planted_bomb{};

		int ent_id = valve___g_entity_list->highest_index();

		for ( int i{}; i <= ent_id; ++i ) {
			const auto entity = valve___g_entity_list->find_entity( i );
			if ( !entity )
				continue;

			if ( entity->is_player( ) )
				handle_player( static_cast< valve::c_player* const >( entity ) );
			else if ( const auto client_class = entity->client_class( ) ) {
				if ( entity->dormant( ) )
					continue;

				if ( entity->is_weapon ( ) )
				{

				}
				else {
					if ( client_class->m_class_id == valve::e_class_id::planted_c4 )
						planted_bomb = entity;
				}
			}
		}


		if ( !valve___g_local_player->alive( ) )
			return;

		vec2_t indicator_pos{ 30.f, screen_center.y + screen_center.y / 4.f };

	}

	bool get_min_dmg_key_to_set_up_ ( )
	{
		if ( !valve___g_local_player || !valve___g_local_player->alive ( ) )
			return false;

		auto wpn = valve___g_local_player->weapon ( );

		if ( !wpn )
			return false;

		switch ( wpn->item_index ( ) )
		{
			case valve::e_item_index::awp:
			case valve::e_item_index::ssg08:
			case valve::e_item_index::scar20:
			case valve::e_item_index::g3sg1:
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
			case valve::e_item_index::revolver:
			case valve::e_item_index::deagle:
			case valve::e_item_index::cz75a:
			case valve::e_item_index::elite:
			case valve::e_item_index::fiveseven:
			case valve::e_item_index::hkp2000:
			case valve::e_item_index::glock:
			case valve::e_item_index::p250:
			case valve::e_item_index::tec9:
			case valve::e_item_index::usp_silencer:
				return hacks::g_aim_bot->cfg ( ).m_bOverrideMinDamage;
			default:
				return false;
		}

		return false;
	}

	void c_visuals::draw_c4_status ( bool im_gui_suck ) {
		valve::c_entity* c4 = nullptr;
		vec3_t expl_origin {}, expl_origin_adjusted {};
		valve::trace_t trace;
		valve::trace_filter_simple_t filter;

		if ( !valve::g_local_player
			|| !valve___g_engine->in_game ( ) )
			return;

		int idk = valve___g_entity_list->highest_index ( );

		for ( std::ptrdiff_t i {}; i <= idk; ++i ) {
			const auto ent = valve___g_entity_list->find_entity ( i );

			if ( !ent )
				continue;

			if ( ent->is_player ( ) )
				continue;

			if ( !ent->client_class ( ) )
				continue;

			if ( ent->dormant ( ) )
				continue;

			if ( ent->is_weapon ( ) )
				continue;

			if ( ent->client_class ( )->m_class_id != valve::e_class_id::planted_c4 )
				continue;

			c4 = ent;
		}

		if ( !c4 )
			return;

		if ( !c4->is_bomb_ticking ( ) )
			return;

		g_context->is_c4_planted ( ) = true;
		g_context->c4_explode_time ( ) = c4->c4_blow ( );

		expl_origin = c4->abs_origin ( );
		expl_origin_adjusted = expl_origin;
		expl_origin_adjusted.z += 8.f;

		filter.m_ignore_entity = c4;

		valve___g_engine_trace->trace_ray ( valve::ray_t ( expl_origin_adjusted, expl_origin_adjusted + vec3_t ( 0.f, 0.f, 40.f ) ),
			valve::e_mask::solid, reinterpret_cast < valve::trace_filter_t* > ( &filter ), &trace );

		if ( trace.m_fraction != 1.f )
			expl_origin = trace.m_end_pos = ( trace.m_plane.m_normal * 0.6f );

		expl_origin.z += 1.f;

		g_context->c4_explode_origin ( ) = expl_origin;
		g_context->c4_dmg ( ) = 500.f;
		g_context->c4_radius ( ) = g_context->c4_dmg ( ) * 3.5f;
		g_context->c4_radius_scaled ( ) = g_context->c4_radius ( ) / 3.f;
		static auto scale_damage = [ ] ( float damage, int armor_value ) {
			float new_damage, armor;

			if ( armor_value > 0 ) {
				new_damage = damage * 0.5f;
				armor = ( damage - new_damage ) * 0.5f;

				if ( armor > ( float ) armor_value ) {
					armor = ( float ) armor_value * 2.f;
					new_damage = damage - armor;
				}

				damage = new_damage;
			}

			return std::max ( 0, ( int ) std::floor ( damage ) );
		};
		auto dst = valve::g_local_player->world_space_center ( );
		auto to_target = g_context->c4_explode_origin ( ) - dst;
		auto dist = to_target.length ( );

		auto range_dmg = g_context->c4_dmg ( ) * std::exp ( ( dist * dist ) / ( ( g_context->c4_radius_scaled ( ) * -2.f ) * g_context->c4_radius_scaled ( ) ) );
		auto final_dmg = std::clamp ( scale_damage ( range_dmg, valve::g_local_player->armor_value ( ) ), 0, 100 );

		auto time_to_explode = g_context->c4_explode_time ( ) - valve___g_global_vars->m_cur_time;

		int advanced_flag = 0;
		if ( !im_gui_suck )
			advanced_flag = ImGuiWindowFlags_NoMove;

		ImGui::SetNextWindowSize ( ImVec2 ( 181, 43 ) );

		ImGui::Begin ( xorstr_ ( "C4 hueta " ), &hacks::g_aim_bot->cfg ( ).m_bC4Indicator, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | advanced_flag );
		{
			PostProcessing::performFullscreenBlur ( ImGui::GetWindowDrawList ( ), 0.9f );

			int32_t x = ImGui::GetCurrentWindow ( )->Pos.x;
			int32_t y = ImGui::GetCurrentWindow ( )->Pos.y;

			ImGui::PushFont ( m_fonts.m_warning_icon_font );
			ImGui::GetForegroundDrawList ( )->AddText ( ImVec2 ( x + 8, y + 10 ), ImColor ( 255, 255, 255 ), xorstr_ ( "o" ) );
			ImGui::PopFont ( );

			auto str = std::to_string ( final_dmg );

			std::transform ( str.begin ( ), str.end ( ), str.begin ( ), ::toupper );

			auto str_ = std::to_string ( static_cast < int > ( time_to_explode ) );

			std::transform ( str_.begin ( ), str_.end ( ), str_.begin ( ), ::toupper );

			ImGui::PushFont ( m_fonts.m_museo_sans_name );
			ImGui::GetForegroundDrawList ( )->AddText ( ImVec2 ( x + 40, y + 16 ), ImColor ( 255, 255, 255 ), xorstr_ ( "Damage: " ) );
			ImGui::GetForegroundDrawList ( )->AddText ( ImVec2 ( x + 94, y + 16 ), ImColor ( 255, 255, 255 ), str.c_str ( ) );

			ImGui::GetForegroundDrawList ( )->AddText ( ImVec2 ( x + 123, y + 16 ), ImColor ( 255, 255, 255 ), xorstr_ ( "Time: " ) );
			ImGui::GetForegroundDrawList ( )->AddText ( ImVec2 ( x + 158, y + 16 ), ImColor ( 255, 255, 255 ), str_.c_str ( ) );
			ImGui::PopFont ( );
		}
		ImGui::End ( );
	}

	void c_visuals::draw_spectators ( bool im_gui_suck )
	{
		std::vector < std::string > spectator_list;
		if ( !hacks::g_aim_bot->cfg ( ).m_bSpectators )
			return;

		if ( valve___g_local_player && valve___g_local_player->alive ( ) )
		{
			for ( int i = 1; i <= valve___g_global_vars->m_max_clients; i++ )
			{
				auto player = ( valve::c_player* )valve___g_entity_list->find_entity ( i );

				if ( !player || player->alive ( ) || !player->is_player ( ) )
					continue;

				auto observer_target = valve___g_entity_list->find_entity ( player->observer_target ( ) );

				if ( !observer_target || observer_target != valve___g_local_player )
					continue;

				std::optional< valve::player_info_t > spec_info = valve___g_engine->player_info ( player->index ( ) );

				if ( spec_info.has_value ( ) )
					spectator_list.emplace_back ( ( std::string )( spec_info.value ( ).m_name ) );
			}
		}

		if ( g_context->freeze_time ( ) )
			spectator_list.clear ( );

		if ( !im_gui_suck && spectator_list.empty ( ) )
			return;

		int advanced_flag = 0;
		if ( !im_gui_suck )
			advanced_flag = ImGuiWindowFlags_NoMove;

		ImGui::SetNextWindowSize ( ImVec2 ( 181, spectator_list.empty ( ) ? 0 : 35 + ( 18.5f * spectator_list.size ( ) ) ) );
		ImGui::Begin ( xorstr_ ( "Spectators " ), &hacks::g_aim_bot->cfg ( ).m_bSpectators, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | advanced_flag );
		{
			PostProcessing::performFullscreenBlur ( ImGui::GetWindowDrawList ( ), 0.9f );
			int32_t x = ImGui::GetCurrentWindow ( )->Pos.x;
			int32_t y = ImGui::GetCurrentWindow ( )->Pos.y;

			ImGui::GetForegroundDrawList ( )->AddRectFilled ( ImVec2 ( x, y ), ImVec2 ( x + 181, y + 22 ), col_t ( 35, 35, 35, 255 ).hex ( ), 3.f );

			ImGui::PushFont ( m_fonts.m_xxxtentacion_icons );
			ImGui::GetForegroundDrawList ( )->AddText ( ImVec2 ( x + 6, y + 4 ), ImColor ( 255, 255, 255 ), xorstr_ ( "b" ) );
			ImGui::PopFont ( );

			ImGui::PushFont ( m_fonts.m_museo_sans_name );
			ImGui::GetForegroundDrawList ( )->AddText ( ImVec2 ( x + 20, y + 3 ), ImColor ( 255, 255, 255 ), xorstr_ ( "| " ) );
			ImGui::GetForegroundDrawList ( )->AddText ( ImVec2 ( x + 27, y + 4 ), ImColor ( 255, 255, 255 ), xorstr_ ( "Spectators " ) );
			ImGui::PopFont ( );

			int m_Last = 0;
			for ( auto& spectator : spectator_list )
			{
				ImGui::PushFont ( m_fonts.m_museo_sans_name );
				ImGui::GetForegroundDrawList ( )->AddText ( ImVec2 ( x + 2, 23 + ( y + 16 * m_Last ) ), ImColor ( 255, 255, 255, 255 ), spectator.c_str ( ) );
				ImGui::PopFont ( );

				m_Last++;
			}
		}
		ImGui::End ( );
	}

	void c_visuals::draw_key_binds ( bool imgui_suck )
	{
		if ( !hacks::g_aim_bot->cfg ( ).m_bKeybinds )
			return;

		int m_last = 0;

		m_binds.clear( );
		
		for ( int i = 0; i < g_Keybinds->m_cVecKeybindsToShow.size(); ++i )
		{
			binds_data_t data{ };

			data.m_alpha_modifier = 1.f;
			data.m_name = g_Keybinds->m_cVecKeybindsToShow[i];
			data.m_state = g_Keybinds->m_cVecKeybindsStates[i];

			m_binds[data.m_name] = data;
		}

		int binds_count = 0;
		for ( auto& bind : m_binds )
		{
			if ( bind.second.m_name.length ( ) )
				binds_count++;
		}

		if ( binds_count <= 0 )
			return;

		int advanced_flag = 0;
		if ( !imgui_suck )
			advanced_flag = ImGuiWindowFlags_NoMove;
		ImGui::SetNextWindowSize ( ImVec2 ( 181, m_binds.empty ( ) ? 0 : 25.f + ( 18.5f * binds_count ) ) );

		ImGui::Begin ( xorstr_ ( "Keybind List" ), &hacks::g_aim_bot->cfg ( ).m_bKeybinds, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | advanced_flag );
		{
			int32_t x = ImGui::GetCurrentWindow ( )->Pos.x;
			int32_t y = ImGui::GetCurrentWindow ( )->Pos.y;
			PostProcessing::performFullscreenBlur ( ImGui::GetWindowDrawList ( ), 0.9f );
			ImGui::GetForegroundDrawList( )->AddRectFilled( ImVec2( x - 4, y - 4 ), ImVec2( x + 185, y + 26 ), col_t( 25, 25, 25, 255 ).hex( ), 3 );
			ImGui::GetForegroundDrawList( )->AddRectFilled( ImVec2( x - 3, y - 3 ), ImVec2( x + 184, y + 25 ), col_t( 40, 40, 40, 255 ).hex( ), 3 );
			ImGui::GetForegroundDrawList( )->AddRectFilled( ImVec2( x - 2, y - 2 ), ImVec2( x + 183, y + 24 ), col_t( 55, 55, 55, 255 ).hex( ), 3 );
			ImGui::GetForegroundDrawList( )->AddRectFilled( ImVec2( x - 1, y - 1 ), ImVec2( x + 182, y + 23 ), col_t( 75, 75, 75, 255 ).hex( ), 3 );
			ImGui::GetForegroundDrawList ( )->AddRectFilled ( ImVec2 ( x, y ), ImVec2 ( x + 181, y + 22 ), col_t ( 35, 35, 35, 255 ).hex ( ), 3 );
			ImGui::PushFont ( m_fonts.m_xxxtentacion_icons );
			ImGui::GetForegroundDrawList ( )->AddText ( ImVec2 ( x + 6, y + 4 ), ImColor ( 255, 255, 255 ), xorstr_ ( "a" ) );
			ImGui::PopFont ( );

			ImGui::PushFont ( m_fonts.m_museo_sans_name );
			ImGui::GetForegroundDrawList ( )->AddText ( ImVec2 ( x + 20, y + 3 ), ImColor ( 255, 255, 255 ), xorstr_ ( "| " ) );
			ImGui::GetForegroundDrawList ( )->AddText ( ImVec2 ( x + 27, y + 4 ), ImColor ( 255, 255, 255 ), xorstr_ ( "Keybinds " ) );
			ImGui::PopFont ( );

			for ( auto& bind : m_binds )
			{
				if ( !bind.second.m_name.length ( ) )
					continue;

				ImGui::PushFont ( m_fonts.m_museo_sans_name );
				ImGui::GetForegroundDrawList ( )->AddText ( ImVec2 ( x + 2, 27 + ( y + 16 * m_last ) ), ImColor ( 255, 255, 255, static_cast < int >( bind.second.m_alpha_modifier * 255.0f ) ), bind.second.m_name.c_str ( ) );
				ImGui::GetForegroundDrawList( )->AddText( ImVec2( x + 160, 27 + ( y + 16 * m_last ) ), ImColor( 255, 255, 255, static_cast < int >( bind.second.m_alpha_modifier * 255.0f ) ), bind.second.m_state.c_str( ) );
				ImGui::PopFont ( );

				m_last++;
			}

		}
		ImGui::End ( );
	}

	void c_visuals::draw_watermark ( )
	{

		static std::string name = std::string( g_cloud->get_username( ) );

		std::string water_mark = xorstr_ ( "off-white | " ) + name + xorstr_ ( " |" );

		int allah = 0;

		auto net_channel = valve___g_engine->net_channel_info ( );
		if ( valve___g_engine->in_game ( ) )
		{
			if ( net_channel )
			{
				auto latency = net_channel->avg_latency ( 0 );

				if ( latency )
				{
					static auto cl_updaterate = valve___g_cvar->find_var ( xorstr_ ( "cl_updaterate" ) );
					latency -= 0.5f / cl_updaterate->get_float ( );
				}

				water_mark += std::string ( " ms: " ) + std::to_string ( ( int )( std::max ( 0.0f, latency ) * 1000.0f ) ) + " | ";
		}
	}
		else
			water_mark += xorstr_ ( " not connected |" );

		int screen_size_x, screen_size_y;
		valve___g_engine->get_screen_size ( screen_size_x, screen_size_y );

		int text_length = m_fonts.m_tahoma14->CalcTextSizeA ( 15.f, FLT_MAX, NULL, water_mark.c_str ( ) ).x + 35;

		static float current_time = 0.f;
		static int last_fps = ( int )( 1.0f / valve___g_global_vars->m_absolute_frame_time );

		if ( current_time > 0.5f )
		{
			last_fps = ( int )( 1.0f / valve___g_global_vars->m_absolute_frame_time );
			current_time = 0.f;
		}

		current_time += valve___g_global_vars->m_absolute_frame_time;

		water_mark += xorstr_ ( " FPS: " ) + std::to_string ( last_fps );

		ImGui::GetForegroundDrawList ( )->AddRectFilled ( ImVec2 ( screen_size_x - text_length - 30, 11 ), ImVec2 ( screen_size_x - 7, 32 ), col_t ( 35, 35, 35, 255 ).hex ( ), 3 );


		ImGui::PushFont ( m_fonts.m_museo_sans_name );
		ImGui::GetForegroundDrawList ( )->AddText ( ImVec2 ( screen_size_x - text_length - 23 + allah, 14 ), ImColor ( 255, 255, 255, 255 ), water_mark.c_str ( ) );
		ImGui::PopFont ( );
	}

#include <Psapi.h>

	static std::pair<void*, std::size_t> getModuleInformation ( const char* name ) noexcept
	{
		if ( HMODULE handle = GetModuleHandleA ( name ) ) {
			if ( MODULEINFO moduleInfo; GetModuleInformation ( GetCurrentProcess ( ), handle, &moduleInfo, sizeof ( moduleInfo ) ) )
				return std::make_pair ( moduleInfo.lpBaseOfDll, moduleInfo.SizeOfImage );
		}
		return {};
	}

	[[nodiscard]] static auto generateBadCharTable ( std::string_view pattern ) noexcept
	{
		assert ( !pattern.empty ( ) );

		std::array<std::size_t, ( std::numeric_limits<std::uint8_t>::max )( ) + 1> table;

		auto lastWildcard = pattern.rfind ( '?' );
		if ( lastWildcard == std::string_view::npos )
			lastWildcard = 0;

		const auto defaultShift = ( std::max ) ( std::size_t ( 1 ), pattern.length ( ) - 1 - lastWildcard );
		table.fill ( defaultShift );

		for ( auto i = lastWildcard; i < pattern.length ( ) - 1; ++i )
			table [ static_cast< std::uint8_t >( pattern [ i ] ) ] = pattern.length ( ) - 1 - i;

		return table;
	}

	static std::uintptr_t findPattern ( const char* moduleName, std::string_view pattern, bool reportNotFound = true ) noexcept
	{
		static auto id = 0;
		++id;

		const auto [moduleBase, moduleSize] = getModuleInformation ( moduleName );

		if ( moduleBase && moduleSize ) {
			const auto lastIdx = pattern.length ( ) - 1;
			const auto badCharTable = generateBadCharTable ( pattern );

			auto start = static_cast< const char* >( moduleBase );
			const auto end = start + moduleSize - pattern.length ( );

			while ( start <= end ) {
				int i = lastIdx;
				while ( i >= 0 && ( pattern [ i ] == '?' || start [ i ] == pattern [ i ] ) )
					--i;

				if ( i < 0 )
					return reinterpret_cast< std::uintptr_t >( start );

				start += badCharTable [ static_cast< std::uint8_t >( start [ lastIdx ] ) ];
			}
		}

		return 0;
	}
	constexpr auto CLIENT_DLL = "client";

	template <typename T>
	static constexpr auto relativeToAbsolute ( uintptr_t address ) noexcept
	{
		return ( T ) ( address + 4 + *reinterpret_cast< std::int32_t* >( address ) );
	}
	auto drawScreenEffectMaterial = relativeToAbsolute<uintptr_t> ( findPattern ( CLIENT_DLL, "\xE8????\x83\xC4\x0C\x8D\x4D\xF8" ) + 1 );

#undef xor
#define DRAW_SCREEN_EFFECT(material) \
{ \
    const auto drawFunction = drawScreenEffectMaterial; \
    int w, h; \
    valve___g_engine->get_screen_size(w, h); \
    __asm { \
        __asm push h \
        __asm push w \
        __asm push 0 \
        __asm xor edx, edx \
        __asm mov ecx, material \
        __asm call drawFunction \
        __asm add esp, 12 \
    } \
}
	static auto from_angle_all ( const vec3_t& angle, vec3_t* forward, vec3_t* right, vec3_t* up ) noexcept
	{
		float sr = std::sin ( math::to_rad ( angle.z ) )
			, sp = std::sin ( math::to_rad ( angle.x ) )
			, sy = std::sin ( math::to_rad ( angle.y ) )
			, cr = std::cos ( math::to_rad ( angle.z ) )
			, cp = std::cos ( math::to_rad ( angle.x ) )
			, cy = std::cos ( math::to_rad ( angle.y ) );

		if ( forward )
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if ( right )
		{
			right->x = ( -1 * sr * sp * cy + -1 * cr * -sy );
			right->y = ( -1 * sr * sp * sy + -1 * cr * cy );
			right->z = -1 * sr * cp;
		}

		if ( up )
		{
			up->x = ( cr * sp * cy + -sr * -sy );
			up->y = ( cr * sp * sy + -sr * cy );
			up->z = cr * cp;
		}
	}
	void c_visuals::motion_blur ( valve::view_setup_t* view_setup ) noexcept {

		if ( !hacks::g_aim_bot->cfg ( ).m_bMotionBlur )
			return;

		static motion_blur_history_t history;
		static float motion_blur_values [ 4 ] = {};

		if ( view_setup ) {
			const float time_elapsed = valve___g_global_vars->m_real_time - history.m_last_time_upd;
			const auto view_ang = view_setup->m_angles;
			const auto cur_pitch = math::normalize_yaw ( view_ang.x );
			const auto cur_yaw = math::normalize_yaw ( view_ang.y );

			vec3_t currentSideVector;
			vec3_t currentForwardVector;
			vec3_t currentUpVector;
			from_angle_all ( view_setup->m_angles, &currentForwardVector, &currentSideVector, &currentUpVector );

			vec3_t currentPosition = view_setup->m_origin;
			vec3_t positionChange = history.m_prev_pos - currentPosition;

			if ( ( positionChange.length ( ) > 30.0f ) && ( time_elapsed >= 0.5f ) )
			{
				motion_blur_values [ 0 ] = 0.0f;
				motion_blur_values [ 1 ] = 0.0f;
				motion_blur_values [ 2 ] = 0.0f;
				motion_blur_values [ 3 ] = 0.0f;
			}
			else if ( time_elapsed > ( 1.0f / 15.0f ) )
			{
				motion_blur_values [ 0 ] = 0.0f;
				motion_blur_values [ 1 ] = 0.0f;
				motion_blur_values [ 2 ] = 0.0f;
				motion_blur_values [ 3 ] = 0.0f;
			}
			else if ( positionChange.length ( ) > 50.0f )
			{
				history.m_no_rot_motion_blur_until = valve___g_global_vars->m_real_time + 1.0f;
			}
			else
			{
				const float horizontalFov = view_setup->m_fov;
				const float verticalFov = ( view_setup->m_aspect_ratio <= 0.0f ) ? ( view_setup->m_fov ) : ( view_setup->m_fov / view_setup->m_aspect_ratio );
				const float viewdotMotion = currentForwardVector.dot ( positionChange );

				motion_blur_values [ 2 ] = viewdotMotion;

				const float sidedotMotion = currentSideVector.dot ( positionChange );
				float yawdiffOriginal = history.m_prev_yaw - cur_yaw;
				if ( ( ( history.m_prev_yaw - cur_yaw > 180.0f ) || ( history.m_prev_yaw - cur_yaw < -180.0f ) ) &&
					( ( history.m_prev_yaw + cur_yaw > -180.0f ) && ( history.m_prev_yaw + cur_yaw < 180.0f ) ) )
					yawdiffOriginal = history.m_prev_yaw + cur_yaw;

				float yawdiffAdjusted = yawdiffOriginal + ( sidedotMotion / 3.0f );

				if ( yawdiffOriginal < 0.0f )
					yawdiffAdjusted = std::clamp ( yawdiffAdjusted, yawdiffOriginal, 0.0f );
				else
					yawdiffAdjusted = std::clamp ( yawdiffAdjusted, 0.0f, yawdiffOriginal );

				const float undampenedYaw = yawdiffAdjusted / horizontalFov;
				motion_blur_values [ 0 ] = undampenedYaw * ( 1.0f - ( fabsf ( cur_pitch ) / 90.0f ) );

				const float pitchCompensateMask = 1.0f - ( ( 1.0f - fabsf ( currentForwardVector [ 2 ] ) ) * ( 1.0f - fabsf ( currentForwardVector [ 2 ] ) ) );
				const float pitchdiffOriginal = history.m_prev_pitch - cur_pitch;
				float pitchdiffAdjusted = pitchdiffOriginal;

				if ( cur_pitch > 0.0f )
					pitchdiffAdjusted = pitchdiffOriginal - ( ( viewdotMotion / 2.0f ) * pitchCompensateMask );
				else
					pitchdiffAdjusted = pitchdiffOriginal + ( ( viewdotMotion / 2.0f ) * pitchCompensateMask );


				if ( pitchdiffOriginal < 0.0f )
					pitchdiffAdjusted = std::clamp ( pitchdiffAdjusted, pitchdiffOriginal, 0.0f );
				else
					pitchdiffAdjusted = std::clamp ( pitchdiffAdjusted, 0.0f, pitchdiffOriginal );

				motion_blur_values [ 1 ] = pitchdiffAdjusted / verticalFov;
				motion_blur_values [ 3 ] = undampenedYaw;
				motion_blur_values [ 3 ] *= ( fabs ( cur_pitch ) / 90.0f ) * ( fabs ( cur_pitch ) / 90.0f ) * ( fabs ( cur_pitch ) / 90.0f );

				if ( time_elapsed > 0.0f )
					motion_blur_values [ 2 ] /= time_elapsed * 30.0f;
				else
					motion_blur_values [ 2 ] = 0.0f;

				motion_blur_values [ 2 ] = std::clamp ( ( fabsf ( motion_blur_values [ 2 ] ) - 10.f ) / ( 25.f - 10.f ), 0.0f, 1.0f ) * ( motion_blur_values [ 2 ] >= 0.0f ? 1.0f : -1.0f );
				motion_blur_values [ 2 ] /= 30.0f;
				motion_blur_values [ 0 ] *= 1.6f * .15f * 1.6f;
				motion_blur_values [ 1 ] *= 1.6f * .15f * 1.6f;
				motion_blur_values [ 2 ] *= 1.6f * .15f * 1.6f;
				motion_blur_values [ 3 ] *= 1.6f * .15f * 1.6f;

			}

			if ( valve___g_global_vars->m_real_time < history.m_no_rot_motion_blur_until )
			{
				motion_blur_values [ 0 ] = 0.0f;
				motion_blur_values [ 1 ] = 0.0f;
				motion_blur_values [ 3 ] = 0.0f;
			}
			else
			{
				history.m_no_rot_motion_blur_until = 0.0f;
			}
			history.m_prev_pos = currentPosition;

			history.m_prev_pitch = cur_pitch;
			history.m_prev_yaw = cur_yaw;
			history.m_last_time_upd = valve___g_global_vars->m_real_time;
			return;
		}

		const auto material = valve___g_mat_system->find_material ( "dev/motion_blur", "RenderTargets" , false );
		if ( !material )
			return;

		const auto MotionBlurInternal = material->find_var ( "$MotionBlurInternal", nullptr, false );

		MotionBlurInternal->set_comp_vec ( motion_blur_values [ 0 ], 0 );
		MotionBlurInternal->set_comp_vec ( motion_blur_values [ 1 ], 1 );
		MotionBlurInternal->set_comp_vec ( motion_blur_values [ 2 ], 2 );
		MotionBlurInternal->set_comp_vec ( motion_blur_values [ 3 ], 3 );

		const auto MotionBlurViewPortInternal = material->find_var ( "$MotionBlurViewportInternal", nullptr, false );

		MotionBlurViewPortInternal->set_comp_vec ( 0.0f, 0 );
		MotionBlurViewPortInternal->set_comp_vec ( 0.0f, 1 );
		MotionBlurViewPortInternal->set_comp_vec ( 1.0f, 2 );
		MotionBlurViewPortInternal->set_comp_vec ( 1.0f, 3 );

		DRAW_SCREEN_EFFECT ( material )
	}

	void c_visuals::on_render_start( ) {
		static int old_walls_transp{};
		static int old_props_transp{};

		if ( !valve___g_engine->in_game( ) ) {
			old_walls_transp = old_props_transp = -1;

			return;
		}

		//change_sky_box();

		auto& smoke_count = **reinterpret_cast< int** >( g_context->addresses( ).m_smoke_count.get( ) );

		m_backup_smoke_count = smoke_count;

		if ( hacks::g_aim_bot->cfg ( ).m_bRemoveSmoke )
			smoke_count = 0;

		**reinterpret_cast< bool** >(
			g_context->addresses( ).m_disable_post_processing.get( )
		) = hacks::g_aim_bot->cfg ( ).m_bDisablePostProcessing;

		
		m_player_sounds.clear( );

		static int last_impacts_count{};

		struct client_hit_verify_t {
			vec3_t	m_pos{};
			float	m_time{};
			float	m_expires{};
		};

		/* FF 71 0C F3 0F 11 84 24 ? ? ? ? F3 0F 10 84 24 ? ? ? ? */
		const auto& client_impacts_list = *reinterpret_cast< valve::utl_vec_t< client_hit_verify_t >* >(
			reinterpret_cast< std::uintptr_t >( valve___g_local_player.operator nem::valve::c_player * ( ) ) + 0x11C50u
		);

		if ( hacks::g_aim_bot->cfg ( ).m_bBulletImpacts )
			for ( auto i = client_impacts_list.m_size; i > last_impacts_count; --i )
				valve___g_debug_overlay->add_box_overlay( client_impacts_list.at( i - 1 ).m_pos, { -2.f, -2.f, -2.f }, { 2.f, 2.f, 2.f }, {}, 255, 0, 0, 127, 4.f );

		last_impacts_count = client_impacts_list.m_size;

		if ( hacks::g_aim_bot->cfg ( ).m_bBulletTracers
			|| hacks::g_aim_bot->cfg ( ).m_bBulletImpacts )
			for ( auto i = m_bullet_impacts.begin( ); i != m_bullet_impacts.end( ); i = m_bullet_impacts.erase( i ) ) {
				if ( hacks::g_aim_bot->cfg ( ).m_bBulletImpacts )
					valve___g_debug_overlay->add_box_overlay( i->m_pos, { -2.f, -2.f, -2.f }, { 2.f, 2.f, 2.f }, {}, 0, 0, 255, 127, 4.f );
			}
		else 
			m_bullet_impacts.clear( );


		static auto cl_wpn_sway_amt = valve___g_cvar->find_var ( xorstr_ ( "cl_wpn_sway_scale" ) );

		if ( hacks::g_aim_bot->cfg ( ).m_bRemoveHandsShaking )
			cl_wpn_sway_amt->set_float ( 0.0f );
		else
			cl_wpn_sway_amt->set_float ( 1.6f );

		static auto cl_foot_contact_shadows = valve___g_cvar->find_var ( xorstr_ ( "cl_foot_contact_shadows" ) );
		static auto cl_foot_contact_shadows_ = valve___g_cvar->find_var ( xorstr_ ( "cl_csm_static_prop_shadows" ) );
		static auto cl_foot_contact_shadows_1 = valve___g_cvar->find_var ( xorstr_ ( "cl_csm_world_shadows" ) );
		static auto cl_foot_contact_shadows_2 = valve___g_cvar->find_var ( xorstr_ ( "cl_csm_shadows" ) );
		static auto cl_foot_contact_shadows_3 = valve___g_cvar->find_var ( xorstr_ ( "cl_csm_viewmodel_shadows" ) );
		static auto cl_foot_contact_shadows_4 = valve___g_cvar->find_var ( xorstr_ ( "cl_csm_sprite_shadows" ) );
		static auto cl_foot_contact_shadows_5 = valve___g_cvar->find_var ( xorstr_ ( "cl_csm_rope_shadows" ) );

		cl_foot_contact_shadows->set_int   ( !hacks::g_aim_bot->cfg ( ).m_bRemoveShadows );
		cl_foot_contact_shadows_->set_int  ( !hacks::g_aim_bot->cfg ( ).m_bRemoveShadows );
		cl_foot_contact_shadows_1->set_int ( !hacks::g_aim_bot->cfg ( ).m_bRemoveShadows );
		cl_foot_contact_shadows_2->set_int ( !hacks::g_aim_bot->cfg ( ).m_bRemoveShadows );
		cl_foot_contact_shadows_3->set_int ( !hacks::g_aim_bot->cfg ( ).m_bRemoveShadows );
		cl_foot_contact_shadows_4->set_int ( !hacks::g_aim_bot->cfg ( ).m_bRemoveShadows );
		cl_foot_contact_shadows_5->set_int ( !hacks::g_aim_bot->cfg ( ).m_bRemoveShadows );

		static const std::array< valve::material_t*, 4u > smoke_materials{
			valve___g_mat_system->find_material( xorstr_( "particle/vistasmokev1/vistasmokev1_fire" ), nullptr ),
			valve___g_mat_system->find_material( xorstr_( "particle/vistasmokev1/vistasmokev1_smokegrenade" ), nullptr ),
			valve___g_mat_system->find_material( xorstr_( "particle/vistasmokev1/vistasmokev1_emods" ), nullptr ),
			valve___g_mat_system->find_material( xorstr_( "particle/vistasmokev1/vistasmokev1_emods_impactdust" ), nullptr )
		};

		static auto enable_fog = valve___g_cvar->find_var ( xorstr_ ( "fog_enable" ) );
		static auto override_fog = valve___g_cvar->find_var ( xorstr_ ( "fog_override" ) );
		static auto fog_clr = valve___g_cvar->find_var ( xorstr_ ( "fog_color" ) );
		static auto fog_start = valve___g_cvar->find_var ( xorstr_ ( "fog_start" ) );
		static auto fog_end = valve___g_cvar->find_var ( xorstr_ ( "fog_end" ) );
		static auto fog_density = valve___g_cvar->find_var ( xorstr_ ( "fog_maxdensity" ) );
		static auto cl_ragdoll_disable = valve___g_cvar->find_var ( xorstr_ ( "cl_ragdoll_physics_enable" ) );
		
		if ( hacks::g_aim_bot->cfg ( ).m_bDisableRagdollPhysics ) {
			cl_ragdoll_disable->m_callbacks.remove_all ( );

			cl_ragdoll_disable->set_int ( 0 );
		}
		else {
			cl_ragdoll_disable->set_int ( 1 );
		}

		enable_fog->set_int ( hacks::g_aim_bot->cfg ( ).m_bFog );
		override_fog->set_int ( hacks::g_aim_bot->cfg ( ).m_bFog );
		fog_clr->set_str ( std::string ( std::to_string ( hacks::g_aim_bot->cfg ( ).m_flFogColor[ 0 ] * 255.f ) + " " + std::to_string ( hacks::g_aim_bot->cfg ( ).m_flFogColor[ 1 ] * 255.f ) + " " + std::to_string ( hacks::g_aim_bot->cfg ( ).m_flFogColor[ 2 ] * 255.f ) ).c_str ( ) );
		fog_start->set_int ( hacks::g_aim_bot->cfg ( ).m_iFogStart );
		fog_end->set_int ( hacks::g_aim_bot->cfg ( ).m_iFogEnd );
		fog_density->set_float ( hacks::g_aim_bot->cfg ( ).m_iDensity / 100.f );

		static const std::array< valve::material_t*, 2u > flash_materials{
			valve___g_mat_system->find_material( xorstr_( "effects/flashbang" ), nullptr ),
			valve___g_mat_system->find_material( xorstr_( "effects/flashbang_white" ), nullptr )
		};

		for ( auto& material : smoke_materials )
			if ( material )
				material->set_flag( 4, hacks::g_aim_bot->cfg ( ).m_bRemoveSmoke );

		for ( auto& material : flash_materials )
			if ( material )
				material->set_flag( 4, hacks::g_aim_bot->cfg ( ).m_bRemoveFlash );

		if ( old_walls_transp == hacks::g_aim_bot->cfg ( ).m_iWallTransparency
			&& old_props_transp == hacks::g_aim_bot->cfg ( ).m_iPropTransparency )
			return;

		const auto invalid = valve___g_mat_system->invalid_material( );
		for ( auto i = valve___g_mat_system->first_material( ); i != invalid; i = valve___g_mat_system->next_material( i ) ) {
			const auto mat = valve___g_mat_system->material( i );
			if ( !mat )
				continue;

			const auto texture_group_name = mat->texture_group( );

			if ( *reinterpret_cast< const std::uint32_t* >( texture_group_name + 3u ) == 't dl' )
				mat->set_alpha( hacks::g_aim_bot->cfg ( ).m_iWallTransparency / 100.f );
			else if ( *reinterpret_cast< const std::uint32_t* >( texture_group_name + 8u ) == 't po' )
				mat->set_alpha( hacks::g_aim_bot->cfg ( ).m_iPropTransparency / 100.f );
		}

		static auto blur_overlay = valve___g_mat_system->find_material ( xorstr_ ( "dev/scope_bluroverlay" ), xorstr_ ( "Other textures" ) );
		static auto scope_dirt = valve___g_mat_system->find_material ( xorstr_ ( "models/weapons/shared/scope/scope_lens_dirt" ), xorstr_ ( "Other textures" ) );

		blur_overlay->set_flag ( 1 << 2, false );
		scope_dirt->set_flag ( 1 << 2, false );

		if ( valve::g_local_player && valve::g_local_player->alive ( ) )
		{
			auto weapon = valve::g_local_player->weapon ( );
			if ( weapon )
			{
				if ( weapon->wpn_data ( ) )
				{
					if ( weapon->wpn_data ( )->m_type == 5 )

					if ( valve::g_local_player->scoped ( ) )
					{
						if ( hacks::g_aim_bot->cfg ( ).m_bRemoveScope )
						{
							blur_overlay->set_flag ( 1 << 2, true );
							scope_dirt->set_flag ( 1 << 2, true );
						}
					}
				}
			}
		}

		old_walls_transp = hacks::g_aim_bot->cfg ( ).m_iWallTransparency;
		old_props_transp = hacks::g_aim_bot->cfg ( ).m_iPropTransparency;
	}

	void c_visuals::on_render_end( ) {
		if ( !valve___g_engine->in_game( ) )
			return;

		if ( hacks::g_aim_bot->cfg ( ).m_bRemoveSmoke )
			**reinterpret_cast< int** >( g_context->addresses( ).m_smoke_count.get( ) ) = m_backup_smoke_count;
	}

	void c_visuals::on_create_move( const valve::user_cmd_t& cmd ) {
		m_grenade_trajectory = {};
		m_pen_data = std::nullopt;

		if ( !g_context->weapon( )
			|| !g_context->wpn_data( ) )
			return;

		qangle_t dir{};

		if ( 0
			&& g_context->wpn_data( )->m_type != 0
			&& g_context->wpn_data( )->m_type < 7 ) {
			math::angle_vectors( cmd.m_view_angles, &dir );

		}

		if ( !hacks::g_aim_bot->cfg ( ).m_bGrenadePrediction
			|| g_context->wpn_data( )->m_type != 9
			|| ( !g_context->weapon( )->pin_pulled( ) && g_context->weapon( )->throw_time( ) == 0.f ) )
			return;

		m_grenade_trajectory.m_owner = valve___g_local_player;
		m_grenade_trajectory.m_index = g_context->weapon( )->item_index( );

		auto view_angles = cmd.m_view_angles;

		if ( view_angles.x < -90.f )
			view_angles.x += 360.f;
		else if ( view_angles.x > 90.f )
			view_angles.x -= 360.f;

		view_angles.x -= ( 90.f - std::fabsf( view_angles.x ) ) * 10.f / 90.f;

		math::angle_vectors( view_angles, &dir );

		const auto throw_strength = std::clamp( g_context->weapon( )->throw_strength( ), 0.f, 1.f );

		auto src = g_context->shoot_pos( );

		src.z += throw_strength * 12.f - 12.f;

		valve::trace_t trace{};
		valve::trace_filter_simple_t trace_filter{ valve___g_local_player, 0 };

		valve___g_engine_trace->trace_ray(
			{ src, src + dir * 22.f, { -2.f, -2.f, -2.f }, { 2.f, 2.f, 2.f } },
			valve::e_mask::solid | valve::e_mask::contents_current_90,
			reinterpret_cast< valve::trace_filter_t* >( &trace_filter ), &trace
		);

		const auto velocity = std::clamp(
			g_context->wpn_data( )->m_throw_velocity * 0.9f, 15.f, 750.f
		) * ( throw_strength * 0.7f + 0.3f );

		m_grenade_trajectory.predict( 
			trace.m_end_pos - dir * 6.f,
			dir * velocity + valve___g_local_player->velocity( ) * 1.25f,
			valve___g_global_vars->m_cur_time, 0
		);
	}

	void c_visuals::on_override_view( valve::view_setup_t* const setup ) {
		static bool keep_height{};

		if ( !valve___g_engine->in_game( ) ) {
			keep_height = false;

			return;
		}

		if ( valve___g_local_player->alive( ) ) {
			if ( g_movement->should_fake_duck( ) ) {
				keep_height = true;

				setup->m_origin.z = valve___g_local_player->abs_origin( ).z + 64.0625f;
			}
			else if ( keep_height ) {
				const auto& local_data = g_eng_pred->local_data( ).at( g_context->last_cmd_number( ) % 150 );
				if ( local_data.m_net_vars.m_duck_amount != local_data.m_pred_net_vars.m_duck_amount ) {
					if ( local_data.m_pred_net_vars.m_duck_amount > 0.05f )
						setup->m_origin.z = valve___g_local_player->abs_origin( ).z + 64.0625f;
					else
						keep_height = false;
				}
				else
					keep_height = false;
			}
		}
		else {
			keep_height = false;
		}

		if ( valve::g_local_player->alive ( ) ) {
			if ( valve___g_local_player->scoped ( ) )
			{
				if ( !hacks::g_aim_bot->cfg ( ).m_bOvInScope )
					setup->m_fov = 60;
				else
					setup->m_fov = hacks::g_aim_bot->cfg ( ).m_iCameraDistance;
			}
			else
				setup->m_fov = hacks::g_aim_bot->cfg ( ).m_iCameraDistance;
		}

		third_person ( );
	}

	void c_cached_renderer::push_line ( vec3_t& start, vec3_t& end, bool blur, float duration, col_t color )
	{
		int av_index = get_free_shape ( );

		if ( av_index != -1 )
		{
			m_cached_shapes[ av_index ] = s_shape ( blur, duration, SH_LINE, color );

			m_cached_shapes[ av_index ].m_coordinates.push_back ( start );
			m_cached_shapes[ av_index ].m_coordinates.push_back ( end );
		}
	}

	void c_cached_renderer::push_cross ( vec3_t pos, float size, float duration, bool dynamic, col_t color )
	{
		int av_index = get_free_shape ( );

		if ( av_index != -1 )
		{
			m_cached_shapes[ av_index ] = s_shape ( false, duration, SH_2D_CROSS, color );

			m_cached_shapes[ av_index ].m_coordinates.push_back ( pos );
			m_cached_shapes[ av_index ].m_data.push_back ( size );
			m_cached_shapes[ av_index ].m_data.push_back ( dynamic );
		}
	}

	bool all_inited = false;

	void c_cached_renderer::update_shapes ( )
	{
		for ( int i = 0; i < MAX_SHAPES_COUNT; ++i )
		{
			if ( !all_inited )
				m_cached_shapes[ i ] = s_shape ( );

			if ( m_cached_shapes[ i ].m_active && m_cached_shapes[ i ].m_start_time + m_cached_shapes[ i ].m_duration < valve___g_global_vars->m_real_time )
			{
				m_cached_shapes[ i ] = s_shape ( );
			}
		}

		all_inited = true;
	}

	__forceinline float dist_to ( vec3_t& from, const vec3_t& value ) { return ( from - value ).length ( ); }

	void c_cached_renderer::render ( )
	{
		update_shapes ( );

		for ( int i = 0; i < MAX_SHAPES_COUNT; ++i )
		{
			if ( m_cached_shapes[ i ].m_active )
			{
				switch ( m_cached_shapes[ i ].m_shape_type )
				{
					case SH_LINE:
					{
						/*if ( hacks::g_visuals->cfg ( ).m_effects.m_bullet_tracers )
						{
							vec3_t start = m_cached_shapes[ i ].m_coordinates.at ( 0 );
							vec3_t end = m_cached_shapes[ i ].m_coordinates.at ( 1 );
							col_t color = m_cached_shapes[ i ].m_color;

							vec3_t screen_start;
							vec3_t screen_end;

							if ( render::world_to_screen ( start, screen_start ) && render::world_to_screen ( end, screen_end ) )
							{
								render::draw_line ( screen_start.x, screen_start.y, screen_end.x, screen_end.y, color, 1.0f );
							}
						}*/
						break;
					}
					case SH_2D_CROSS:
					{
						if ( hacks::g_aim_bot->cfg ( ).m_bHitMarker )
						{
							vec3_t pos = m_cached_shapes[ i ].m_coordinates.at ( 0 );
							int size = m_cached_shapes[ i ].m_data.at ( 0 );
							int dynamic = m_cached_shapes[ i ].m_data.at ( 1 );
							col_t color = m_cached_shapes[ i ].m_color;

							if ( dynamic && valve___g_local_player && valve___g_local_player->alive ( ) )
								size = std::clamp ( 4 - ( int )( dist_to ( valve___g_local_player->origin ( ), pos ) / 140.f ), 1, 4 );


							vec3_t screen_pos;

							if ( render::world_to_screen ( pos, screen_pos ) )
							{
								vec2_t line_1[ 2 ] = { vec2_t ( screen_pos.x - size - 1, screen_pos.y - size - 1 ), vec2_t ( screen_pos.x - 1, screen_pos.y - 1 ) };
								vec2_t line_2[ 2 ] = { vec2_t ( screen_pos.x - size - 1, screen_pos.y + size + 1 ), vec2_t ( screen_pos.x - 1, screen_pos.y + 1 ) };

								vec2_t line_3[ 2 ] = { vec2_t ( screen_pos.x + size + 1, screen_pos.y - size - 1 ), vec2_t ( screen_pos.x + 1, screen_pos.y - 1 ) };
								vec2_t line_4[ 2 ] = { vec2_t ( screen_pos.x + size + 1, screen_pos.y + size + 1 ), vec2_t ( screen_pos.x + 1, screen_pos.y + 1 ) };

								render::draw_line ( line_1[ 0 ].x, line_1[ 0 ].y, line_1[ 1 ].x, line_1[ 1 ].y, color, 1.0f );
								render::draw_line ( line_2[ 0 ].x, line_2[ 0 ].y, line_2[ 1 ].x, line_2[ 1 ].y, color, 1.0f );
								render::draw_line ( line_3[ 0 ].x, line_3[ 0 ].y, line_3[ 1 ].x, line_3[ 1 ].y, color, 1.0f );
								render::draw_line ( line_4[ 0 ].x, line_4[ 0 ].y, line_4[ 1 ].x, line_4[ 1 ].y, color, 1.0f );
							}
						}
						break;
					}
				}
			}
		}
	}

	int c_cached_renderer::get_free_shape ( )
	{
		for ( int i = 0; i < MAX_SHAPES_COUNT; ++i )
			if ( !m_cached_shapes[ i ].m_active )
				return i;

		return -1;
	}

	void c_visuals::on_post_screen_effects( ) {
		if ( !valve___g_engine->in_game( ) )
			return m_shot_mdls.clear( );
		/*
		if ( m_cfg->m_other.m_bomb ) {
			for ( int i{}; i < valve___g_glow_mgr->m_objects.m_size; ++i ) {
				auto& object = valve___g_glow_mgr->m_objects.at( i );
				if ( !object.m_entity
					|| object.m_next_free_slot != -2 )
					continue;

				const auto client_class = object.m_entity->client_class( );
				if ( !client_class
					|| client_class->m_class_id != valve::e_class_id::planted_c4 )
					continue;

				object.m_render_when_occluded = true;
				object.m_render_when_unoccluded = false;
				object.m_red = m_cfg->m_other.m_bomb_clr[ 0 ];
				object.m_green = m_cfg->m_other.m_bomb_clr[ 1 ];
				object.m_blue = m_cfg->m_other.m_bomb_clr[ 2 ];
				object.m_alpha = m_cfg->m_other.m_bomb_clr[ 3 ];
			}
		}
		*/
		if ( !hacks::g_aim_bot->cfg ( ).m_bShotRecord
			|| m_shot_mdls.empty( ) )
			return;

		const auto context = valve___g_mat_system->render_context( );
		if ( !context )
			return;

		m_rendering_shot_mdl = true;

		for ( auto i = m_shot_mdls.begin( ); i != m_shot_mdls.end( ); ) {
			const auto delta = ( i->m_time + 2 ) - valve___g_global_vars->m_real_time;
			const auto player_info = valve___g_engine->player_info( i->m_player_index );

			if ( delta <= 0.f || !player_info.has_value( ) || i->m_hash != FNV1A_RT( player_info.value( ).m_name ) ) {
				i = m_shot_mdls.erase( i );

				continue;
			}

			override_material(
				hacks::g_aim_bot->cfg ( ).m_iShotRecordType, true,
				hacks::g_aim_bot->cfg ( ).m_flShotRecordColor[ 0 ],
				hacks::g_aim_bot->cfg ( ).m_flShotRecordColor[ 1 ],
				hacks::g_aim_bot->cfg ( ).m_flShotRecordColor[ 2 ],
				hacks::g_aim_bot->cfg ( ).m_flShotRecordColor[ 3 ] * ( delta / 2 )
			);

			valve___g_model_render->draw_model( context, i->m_state, i->m_info, i->m_bones.data( ) );

			if ( hacks::g_aim_bot->cfg ( ).m_bShotRecordGlowOutline )
			{
				override_material (
					2, true,
					hacks::g_aim_bot->cfg ( ).m_flShotRecordGlowOutlineColor[ 0 ],
					hacks::g_aim_bot->cfg ( ).m_flShotRecordGlowOutlineColor[ 1 ],
					hacks::g_aim_bot->cfg ( ).m_flShotRecordGlowOutlineColor[ 2 ],
					hacks::g_aim_bot->cfg ( ).m_flShotRecordGlowOutlineColor[ 3 ] * delta / 2
				);

				valve___g_model_render->draw_model ( context, i->m_state, i->m_info, i->m_bones.data ( ) );
			}


			valve___g_studio_render->override_material( nullptr );

			i = std::next( i );
		}

		m_rendering_shot_mdl = false;
	}

	bool c_visuals::on_draw_model(
		valve::studio_render_t* const ecx, const std::uintptr_t edx,
		const std::uintptr_t a0, const valve::draw_model_info_t& info,
		mat3x4_t* const bones, float* const a3, float* const a4, const vec3_t& origin, int flags
	) const {
		if ( !valve___g_local_player
			|| !valve___g_engine->in_game( )
			|| ecx->forced_material_override( )
			|| !info.m_renderable || !info.m_studio_hdr )
			return true;

		if ( m_rendering_shot_mdl )
			return true;

		const auto entity = reinterpret_cast< valve::c_entity* const >( info.m_renderable - 0x4 );
		if ( !entity )
			return true;
		
		const auto client_class = entity->client_class( );
		if ( !client_class )
			return true;

		const auto move_parent = valve___g_entity_list->find_entity( entity->move_parent( ) );
		const auto move_parent_player = move_parent && move_parent->index( ) >= 1 && move_parent->index( ) <= 64;
		const auto player_mdl = *reinterpret_cast< const std::uint32_t* >( info.m_studio_hdr->m_name + 14u ) == 'yalp';

		if ( player_mdl ) {
			if ( hacks::g_aim_bot->cfg ( ).m_bRagdolls &&
				client_class->m_class_id == valve::e_class_id::ragdoll ) {
				if ( !entity->friendly ( ) ) {
					override_material (
						hacks::g_aim_bot->cfg ( ).m_iRagdollsType, true,
						hacks::g_aim_bot->cfg ( ).m_flRagdollsColor[ 0 ],
						hacks::g_aim_bot->cfg ( ).m_flRagdollsColor[ 1 ],
						hacks::g_aim_bot->cfg ( ).m_flRagdollsColor[ 2 ],
						hacks::g_aim_bot->cfg ( ).m_flRagdollsColor[ 3 ]
					);

					hooks::orig_draw_model (
						ecx, edx, a0, info, bones, a3, a4, origin, flags
					);

					valve___g_studio_render->override_material ( nullptr );

					override_material (
						hacks::g_aim_bot->cfg ( ).m_iRagdollsType, false,
						hacks::g_aim_bot->cfg ( ).m_flRagdollsColor[ 0 ],
						hacks::g_aim_bot->cfg ( ).m_flRagdollsColor[ 1 ],
						hacks::g_aim_bot->cfg ( ).m_flRagdollsColor[ 2 ],
						hacks::g_aim_bot->cfg ( ).m_flRagdollsColor[ 3 ]
					);

					hooks::orig_draw_model (
						ecx, edx, a0, info, bones, a3, a4, origin, flags
					);

					valve___g_studio_render->override_material ( nullptr );

					return true;
				}
			}

			const auto friendly = entity->friendly( );
			const auto local_player = entity == valve___g_local_player;

			if ( !friendly ) {
				if ( entity->is_player ( )
					&& hacks::g_aim_bot->cfg ( ).m_bHistory ) {
					auto lerped_bones = try_to_lerp_bones ( entity->index ( ) );
					if ( lerped_bones.has_value ( ) ) {
						override_material (
							hacks::g_aim_bot->cfg ( ).m_iHistoryChamsType, true,
							hacks::g_aim_bot->cfg ( ).m_flHistoryColor[ 0 ],
							hacks::g_aim_bot->cfg ( ).m_flHistoryColor[ 1 ],
							hacks::g_aim_bot->cfg ( ).m_flHistoryColor[ 2 ],
							hacks::g_aim_bot->cfg ( ).m_flHistoryColor[ 3 ]
						);

						hooks::orig_draw_model (
							ecx, edx, a0, info,
							lerped_bones.value ( ).data ( ), a3, a4, origin, flags
						);

						valve___g_studio_render->override_material ( nullptr );
					}
				}

				if ( client_class->m_class_id != valve::e_class_id::ragdoll ) {

					const auto& entry = hacks::g_lag_comp->entry( entity->index( ) - 1 );

					if ( hacks::g_aim_bot->cfg ( ).m_bInvisiblePlayerChams ) {
						override_material (
							hacks::g_aim_bot->cfg ( ).m_iChamsType, true,
							hacks::g_aim_bot->cfg ( ).m_flInvisiblePlayerChamsColor[ 0 ],
							hacks::g_aim_bot->cfg ( ).m_flInvisiblePlayerChamsColor[ 1 ],
							hacks::g_aim_bot->cfg ( ).m_flInvisiblePlayerChamsColor[ 2 ],
							hacks::g_aim_bot->cfg ( ).m_flInvisiblePlayerChamsColor[ 3 ]
						);

						hooks::orig_draw_model (
							ecx, edx, a0, info, const_cast < mat3x4_t* >( entry.m_bones.data ( ) ), a3, a4, origin, flags
						);

						if ( hacks::g_aim_bot->cfg ( ).m_bInvisibleGlowOutline )
						{
							override_material (
								2, true,
								hacks::g_aim_bot->cfg ( ).m_flInvisibleGlowOutlineColor[ 0 ],
								hacks::g_aim_bot->cfg ( ).m_flInvisibleGlowOutlineColor[ 1 ],
								hacks::g_aim_bot->cfg ( ).m_flInvisibleGlowOutlineColor[ 2 ],
								hacks::g_aim_bot->cfg ( ).m_flInvisibleGlowOutlineColor[ 3 ]
							);

							hooks::orig_draw_model (
								ecx, edx, a0, info, const_cast < mat3x4_t* >( entry.m_bones.data( ) ), a3, a4, origin, flags
							);
						}

						valve___g_studio_render->override_material ( nullptr );
					}

					if ( hacks::g_aim_bot->cfg ( ).m_bPlayerChams ) {
						override_material (
							hacks::g_aim_bot->cfg ( ).m_iChamsType, false,
							hacks::g_aim_bot->cfg ( ).m_flPlayerChamsColor[ 0 ],
							hacks::g_aim_bot->cfg ( ).m_flPlayerChamsColor[ 1 ],
							hacks::g_aim_bot->cfg ( ).m_flPlayerChamsColor[ 2 ],
							hacks::g_aim_bot->cfg ( ).m_flPlayerChamsColor[ 3 ]
						);

						hooks::orig_draw_model (
							ecx, edx, a0, info, const_cast < mat3x4_t* >( entry.m_bones.data( ) ), a3, a4, origin, flags
						);

						if ( hacks::g_aim_bot->cfg ( ).m_bGlowOutline )
						{
							override_material (
								2, false,
								hacks::g_aim_bot->cfg ( ).m_flGlowOutlineColor[ 0 ],
								hacks::g_aim_bot->cfg ( ).m_flGlowOutlineColor[ 1 ],
								hacks::g_aim_bot->cfg ( ).m_flGlowOutlineColor[ 2 ],
								hacks::g_aim_bot->cfg ( ).m_flGlowOutlineColor[ 3 ]
							);

							hooks::orig_draw_model (
								ecx, edx, a0, info, const_cast < mat3x4_t* >( entry.m_bones.data( ) ), a3, a4, origin, flags
							);
						}

						valve___g_studio_render->override_material ( nullptr );

						return false;
					}
				}
			}
			else if ( friendly && !local_player ) {
				
			}
			else if ( local_player ) {
		
				if ( hacks::g_aim_bot->cfg ( ).m_bLocalPlayerFake ) {
					override_material(
						hacks::g_aim_bot->cfg ( ).m_iLocalFakeType, false,
						hacks::g_aim_bot->cfg ( ).m_flLocalPlayerFakeColor[ 0 ],
						hacks::g_aim_bot->cfg ( ).m_flLocalPlayerFakeColor[ 1 ],
						hacks::g_aim_bot->cfg ( ).m_flLocalPlayerFakeColor[ 2 ],
						hacks::g_aim_bot->cfg ( ).m_flLocalPlayerFakeColor[ 3 ]
					);

					hooks::orig_draw_model(
						ecx, edx, a0, info,
						g_anim_sync->local_data( ).m_fake_bones.data( ),
						a3, a4, origin, flags
					);

					if ( hacks::g_aim_bot->cfg ( ).m_bLocalPlayerFakeGlowOutline )
					{
						override_material (
							2, false,
							hacks::g_aim_bot->cfg ( ).m_flLocalPlayerFakeGlowOutlineColor[ 0 ],
							hacks::g_aim_bot->cfg ( ).m_flLocalPlayerFakeGlowOutlineColor[ 1 ],
							hacks::g_aim_bot->cfg ( ).m_flLocalPlayerFakeGlowOutlineColor[ 2 ],
							hacks::g_aim_bot->cfg ( ).m_flLocalPlayerFakeGlowOutlineColor[ 3 ]
						);

						hooks::orig_draw_model (
							ecx, edx, a0, info, g_anim_sync->local_data ( ).m_fake_bones.data ( ), a3, a4, origin, flags
						);
					}

					valve___g_studio_render->override_material( nullptr );
				}

				if ( hacks::g_aim_bot->cfg ( ).m_bLocalPlayerChams ) {
					override_material(
						hacks::g_aim_bot->cfg ( ).m_iLocalPlayerChamsType, false,
						hacks::g_aim_bot->cfg ( ).m_flLocalPlayerChamsColor[ 0 ],
						hacks::g_aim_bot->cfg ( ).m_flLocalPlayerChamsColor[ 1 ],
						hacks::g_aim_bot->cfg ( ).m_flLocalPlayerChamsColor[ 2 ],
						hacks::g_aim_bot->cfg ( ).m_flLocalPlayerChamsColor[ 3 ]
					);

					hooks::orig_draw_model(
						ecx, edx, a0, info, bones, a3, a4, origin, flags
					);

					if ( hacks::g_aim_bot->cfg ( ).m_bLocalPlayerGlowOutline )
					{
						override_material (
							2, false,
							hacks::g_aim_bot->cfg ( ).m_flLocalPlayerGlowOutlineColor[ 0 ],
							hacks::g_aim_bot->cfg ( ).m_flLocalPlayerGlowOutlineColor[ 1 ],
							hacks::g_aim_bot->cfg ( ).m_flLocalPlayerGlowOutlineColor[ 2 ],
							hacks::g_aim_bot->cfg ( ).m_flLocalPlayerGlowOutlineColor[ 3 ]
						);

						hooks::orig_draw_model (
							ecx, edx, a0, info, bones, a3, a4, origin, flags
						);
					}

					valve___g_studio_render->override_material( nullptr );

					return false;
				}
			}
		}
		else if ( *reinterpret_cast< const std::uint32_t* >( info.m_studio_hdr->m_name + 17u ) == 'smra' ) {
			if ( hacks::g_aim_bot->cfg ( ).m_bHandChams ) {
				override_material(
					hacks::g_aim_bot->cfg ( ).m_iHandsChamsType, false,
					hacks::g_aim_bot->cfg ( ).m_flHandChamsColor[ 0 ],
					hacks::g_aim_bot->cfg ( ).m_flHandChamsColor[ 1 ],
					hacks::g_aim_bot->cfg ( ).m_flHandChamsColor[ 2 ],
					hacks::g_aim_bot->cfg ( ).m_flHandChamsColor[ 3 ]
				);

				hooks::orig_draw_model(
					ecx, edx, a0, info, bones, a3, a4, origin, flags
				);

				if ( hacks::g_aim_bot->cfg ( ).m_bHandGlowOutlineChams )
				{
					override_material (
						2, false,
						hacks::g_aim_bot->cfg ( ).m_flHandGlowOutlineChamsColor[ 0 ],
						hacks::g_aim_bot->cfg ( ).m_flHandGlowOutlineChamsColor[ 1 ],
						hacks::g_aim_bot->cfg ( ).m_flHandGlowOutlineChamsColor[ 2 ],
						hacks::g_aim_bot->cfg ( ).m_flHandGlowOutlineChamsColor[ 3 ]
					);

					hooks::orig_draw_model (
						ecx, edx, a0, info, bones, a3, a4, origin, flags
					);
				}

				valve___g_studio_render->override_material( nullptr );

				return false;
			}
		}
		else if ( *reinterpret_cast< const std::uint32_t* >( info.m_studio_hdr->m_name ) == 'paew'
			&& info.m_studio_hdr->m_name[ 8 ] == 'v' ) {
			if ( hacks::g_aim_bot->cfg ( ).m_bLocalWeaponChams ) {
				override_material(
					hacks::g_aim_bot->cfg ( ).m_iLocalWeaponChamsType, false,
					hacks::g_aim_bot->cfg ( ).m_flLocalWeaponChamsColor[ 0 ],
					hacks::g_aim_bot->cfg ( ).m_flLocalWeaponChamsColor[ 1 ],
					hacks::g_aim_bot->cfg ( ).m_flLocalWeaponChamsColor[ 2 ],
					hacks::g_aim_bot->cfg ( ).m_flLocalWeaponChamsColor[ 3 ]
				);

				hooks::orig_draw_model(
					ecx, edx, a0, info, bones, a3, a4, origin, flags
				);

				if ( hacks::g_aim_bot->cfg ( ).m_bLocalWeaponGlowOutlineChams )
				{
					override_material (
						2, false,
						hacks::g_aim_bot->cfg ( ).m_flLocalWeaponGlowOutlineChamsColor[ 0 ],
						hacks::g_aim_bot->cfg ( ).m_flLocalWeaponGlowOutlineChamsColor[ 1 ],
						hacks::g_aim_bot->cfg ( ).m_flLocalWeaponGlowOutlineChamsColor[ 2 ],
						hacks::g_aim_bot->cfg ( ).m_flLocalWeaponGlowOutlineChamsColor[ 3 ]
					);

					hooks::orig_draw_model (
						ecx, edx, a0, info, bones, a3, a4, origin, flags
					);
				}

				valve___g_studio_render->override_material( nullptr );

				return false;
			}
		}
		else if ( move_parent_player ) {
			if ( hacks::g_aim_bot->cfg ( ).m_bWeaponChams ) {
				override_material(
					hacks::g_aim_bot->cfg ( ).m_iWeaponChamsType, false,
					hacks::g_aim_bot->cfg ( ).m_flWeaponChamsColor[ 0 ],
					hacks::g_aim_bot->cfg ( ).m_flWeaponChamsColor[ 1 ],
					hacks::g_aim_bot->cfg ( ).m_flWeaponChamsColor[ 2 ],
					hacks::g_aim_bot->cfg ( ).m_flWeaponChamsColor[ 3 ]
				);

				hooks::orig_draw_model(
					ecx, edx, a0, info, bones, a3, a4, origin, flags
				);

				if ( hacks::g_aim_bot->cfg ( ).m_bWeaponGlowOutlineChams )
				{
					override_material (
						2, false,
						hacks::g_aim_bot->cfg ( ).m_flWeaponGlowOutlineChamsColor[ 0 ],
						hacks::g_aim_bot->cfg ( ).m_flWeaponGlowOutlineChamsColor[ 1 ],
						hacks::g_aim_bot->cfg ( ).m_flWeaponGlowOutlineChamsColor[ 2 ],
						hacks::g_aim_bot->cfg ( ).m_flWeaponGlowOutlineChamsColor[ 3 ]
					);

					hooks::orig_draw_model (
						ecx, edx, a0, info, bones, a3, a4, origin, flags
					);
				}


				valve___g_studio_render->override_material( nullptr );

				return false;
			}
		}

		return true;
	}


	void c_visuals::on_calc_view(
		valve::c_player* const ecx, const std::uintptr_t edx,
		vec3_t& origin, qangle_t& angles, float& z_near, float& z_far, float& fov
	) {
		const auto backup_aim_punch = valve___g_local_player->aim_punch( );
		const auto backup_view_punch = valve___g_local_player->view_punch( );
		const auto backup_use_new_anim_state = valve___g_local_player->use_new_anim_state( );

		valve___g_local_player->use_new_anim_state( ) = false;

		if ( hacks::g_aim_bot->cfg ( ).m_bRemoveVisualRecoil ) {

			valve___g_local_player->aim_punch( ) = valve___g_local_player->view_punch( ) = {};
		}

		hooks::orig_calc_view( ecx, edx, origin, angles, z_near, z_far, fov );

		valve___g_local_player->use_new_anim_state( ) = backup_use_new_anim_state;

		valve___g_local_player->aim_punch( ) = backup_aim_punch;
		valve___g_local_player->view_punch( ) = backup_view_punch;

		if ( g_movement->should_fake_duck ( ) )
			origin.z = valve::g_local_player->abs_origin ( ).z + valve___g_movement->get_player_view_offset ( false ).z;
	}

	void c_visuals::add_shot_mdl( valve::c_player* const player, const std::shared_ptr< lag_record_t >& lag_record ) {
		const auto model = player->model( );
		if ( !model )
			return;

		const auto mdl_data = player->mdl_data( );
		if ( !mdl_data
			|| !mdl_data->m_studio_hdr )
			return;

		auto& shot_mdl = m_shot_mdls.emplace_back( );

		const auto player_info = valve___g_engine->player_info( player->index( ) );

		shot_mdl.m_player_index = player->index( );
		shot_mdl.m_hash = player_info.has_value( ) ? FNV1A_RT( player_info.value( ).m_name ) : 0u;
		shot_mdl.m_time = valve___g_global_vars->m_real_time;
		shot_mdl.m_state.m_studio_hdr = mdl_data->m_studio_hdr;
		shot_mdl.m_state.m_studio_hw_data = valve___g_mdl_cache->lookup_hw_data( model->m_studio );
		shot_mdl.m_state.m_renderable = player->renderable( );

		const auto& anim_side = lag_record->m_anim_sides.at( lag_record->m_anim_side );

		shot_mdl.m_info.m_renderable = player->renderable( );
		shot_mdl.m_info.m_model = model;
		shot_mdl.m_info.m_hitbox_set = player->hitbox_set_index( );
		shot_mdl.m_info.m_skin = player->skin( );
		shot_mdl.m_info.m_body = player->body( );
		shot_mdl.m_info.m_index = player->index( );
		shot_mdl.m_info.m_origin = lag_record->m_origin;
		shot_mdl.m_info.m_angles.y = anim_side.m_foot_yaw;

		shot_mdl.m_info.m_instance = player->model_instance( );
		shot_mdl.m_info.m_flags = 1;

		std::memcpy(
			shot_mdl.m_bones.data( ), anim_side.m_bones.data( ),
			anim_side.m_bones_count * sizeof( mat3x4_t )
		);

		g_context->addresses( ).m_angle_matrix.get( )( shot_mdl.m_info.m_angles, shot_mdl.m_world_matrix );

		shot_mdl.m_world_matrix[ 0 ][ 3 ] = lag_record->m_origin.x;
		shot_mdl.m_world_matrix[ 1 ][ 3 ] = lag_record->m_origin.y;
		shot_mdl.m_world_matrix[ 2 ][ 3 ] = lag_record->m_origin.z;

		shot_mdl.m_info.m_model_to_world = shot_mdl.m_state.m_model_to_world = &shot_mdl.m_world_matrix;
	}

	void c_dormant_esp::start ( )
	{
		m_utlCurSoundList.remove_all ( );
		valve::g_engine_sound->get_act_sounds ( m_utlCurSoundList );

		if ( !m_utlCurSoundList.m_size )
			return;

		for ( auto i = 0; i < m_utlCurSoundList.m_size; i++ )
		{
			auto& sound = m_utlCurSoundList.at ( i );

			if ( sound.m_nSoundSource < 1 || sound.m_nSoundSource > 64 )
				continue;

			if ( sound.m_pOrigin->x == 0.f && sound.m_pOrigin->y == 0.f && sound.m_pOrigin->z == 0.f )
				continue;

			if ( !valid_sound ( sound ) )
				continue;

			auto player = static_cast< valve::c_player* >( valve___g_entity_list->find_entity ( sound.m_nSoundSource ) );


			if ( !player || !player->alive ( ) || player->friendly ( ) ||
				player == valve::g_local_player.operator nem::valve::c_player * ( ) )
				continue;

			setup_adjust ( player, sound );
			m_cSoundPlayers[ sound.m_nSoundSource ].Override ( sound );
		}

		m_utlvecSoundBuffer = m_utlCurSoundList;
	}

	void c_dormant_esp::setup_adjust ( valve::c_player* player, valve::snd_info_t& sound )
	{
		vec3_t src3D, dst3D;
		valve::trace_t tr;
		valve::trace_filter_simple_t filter;

		src3D = *sound.m_pOrigin + vec3_t ( 0.0f, 0.0f, 1.0f );
		dst3D = src3D - vec3_t ( 0.0f, 0.0f, 100.0f );

		filter.m_ignore_entity = player;
		valve::ray_t ray = { src3D, dst3D };

		valve___g_engine_trace->trace_ray ( ray, static_cast < valve::e_mask > ( MASK_PLAYERSOLID ), reinterpret_cast < valve::trace_filter_t* > ( &filter ), &tr );

		if ( tr.m_all_solid )
			m_cSoundPlayers[ sound.m_nSoundSource ].m_iReceiveTime = -1;

		*sound.m_pOrigin = tr.m_fraction <= 0.97f ? tr.m_end_pos : *sound.m_pOrigin;

		m_cSoundPlayers[ sound.m_nSoundSource ].m_nFlags = player->flags ( );
		m_cSoundPlayers[ sound.m_nSoundSource ].m_nFlags |= ( tr.m_fraction < 0.50f ? valve::e_ent_flags::ducking : 0 ) | ( tr.m_fraction < 1.0f ? valve::e_ent_flags::on_ground : 0 );
		m_cSoundPlayers[ sound.m_nSoundSource ].m_nFlags &= ( tr.m_fraction >= 0.50f ? ~valve::e_ent_flags::ducking : 0 ) | ( tr.m_fraction >= 1.0f ? ~valve::e_ent_flags::on_ground : 0 );
	}

	bool c_dormant_esp::adjust_sound ( valve::c_player* entity )
	{
		auto i = entity->index ( );
		auto sound_player = m_cSoundPlayers[ i ];

		auto expired = false;

		if ( fabs ( valve___g_global_vars->m_real_time - sound_player.m_iReceiveTime ) > 10.0f )
			expired = true;

		entity->spotted ( ) = true;
		entity->flags ( ) = ( valve::e_ent_flags )sound_player.m_nFlags;
		entity->set_abs_origin ( sound_player.m_vecOrigin );

		return !expired;
	}

	bool c_dormant_esp::valid_sound ( valve::snd_info_t& sound )
	{
		for ( auto i = 0; i < m_utlvecSoundBuffer.m_size; i++ )
			if ( m_utlvecSoundBuffer.at ( i ).m_nGuid == sound.m_nGuid )
				return false;

		return true;
	}

	void c_player_esp::draw_data ( )
	{
		g_dormant_esp->start ( );

		for ( int i = 0u; i < valve::g_global_vars->m_max_clients; ++i ) {

			auto player = static_cast < valve::c_player* > ( valve::g_entity_list->find_entity ( i ) );

			if ( !player || !valve::g_local_player
				|| player->team ( ) == valve::g_local_player->team ( ) ||
				player == valve::g_local_player || !player->is_player ( ) )
				continue;

			bool alive_check{};

			if ( !player->alive( ) ) {
				m_dormant_data[ player->index( ) ].m_alpha -= 255.f / 1.f * valve___g_global_vars->m_frame_time;
				m_dormant_data[ player->index( ) ].m_alpha = std::clamp( m_dormant_data[ player->index( ) ].m_alpha, 0.f, 255.f );
				alive_check = true;
			}
			else {
				m_alive_origin.at( player->index( ) ) = hacks::g_lag_comp->entry( player->index( ) - 1 ).m_render_origin;
				using compute_hitbox_fn = bool( __thiscall* )( void*, vec3_t*, vec3_t* );
				static auto compute_hitbox = reinterpret_cast < compute_hitbox_fn > ( g_context->addresses( ).m_compute_hitbox_surround_box.get ( ) );

				if ( !player->dormant( ) )
					compute_hitbox( player, &m_alive_mins.at( player->index( ) ), &m_alive_maxs.at( player->index( ) ) );
			}

			if ( !m_dormant_data[ player->index( ) ].m_alpha
				&& alive_check )
				continue;

			player->spotted ( ) = true;

			if ( !alive_check ) {
				if ( player->dormant( ) ) {
					g_dormant_esp->adjust_sound( player );
					if ( m_dormant_data[ player->index( ) ].m_alpha < 160.f )
					{
						m_dormant_data[ player->index( ) ].m_alpha -= 255.f / 16.f * valve::g_global_vars->m_frame_time;
						m_dormant_data[ player->index( ) ].m_alpha = std::clamp( m_dormant_data[ player->index( ) ].m_alpha, 0.f, 255.f );
					}
					else
					{
						m_dormant_data[ player->index( ) ].m_alpha -= 255.f / 1.f * valve::g_global_vars->m_frame_time;
					}
				}
				else {
					g_dormant_esp->m_cSoundPlayers[ i ].reset( true, player->abs_origin( ), player->flags( ) );
					m_dormant_data[ i ].m_origin = vec3_t( );
					m_dormant_data[ i ].m_receive_time = 0.f;
					m_dormant_data[ i ].m_alpha += 255.f / 0.68f * valve::g_global_vars->m_frame_time;
					m_dormant_data[ i ].m_alpha = std::clamp( m_dormant_data[ i ].m_alpha, 0.f, 255.f );
				}
			}

			int screen_x, screen_y;

			valve::g_engine->get_screen_size ( screen_x, screen_y );

			vec3_t screen = vec3_t ( );

			if ( !render::world_to_screen ( player->abs_origin ( ), screen ) ) {
				continue;
			}

			if ( screen.x < 0 || screen.x > screen_x || screen.y < 0 || screen.y > screen_y ) {
				oof_indicate ( player );
				continue;
			}

			auto bbox = get_bbox ( player );

			draw_box ( player, bbox );
			draw_name ( player, bbox );
			draw_health ( player, bbox );
			draw_flags ( player, bbox );
			draw_weapon ( player, bbox );
			draw_ammo ( player, bbox );
			draw_skeletons ( player, bbox );
		}
	}

	void c_player_esp::draw_box ( valve::c_player* player, RECT& rect ) {
		if ( !hacks::g_aim_bot->cfg ( ).m_bBox )
			return;

		auto bg_alpha = std::clamp ( ( int ) m_dormant_data [ player->index ( ) ].m_alpha, 0, 150 );

		auto color = col_t ( 255, 255, 255, ( int ) m_dormant_data [ player->index ( ) ].m_alpha );
		render::rect ( vec2_t ( rect.left + 1, rect.top + 1 ), vec2_t ( rect.right - 1, rect.bottom - 1 ), col_t ( 10, 10, 10, bg_alpha ) );
		render::rect ( vec2_t ( rect.left - 1, rect.top - 1 ), vec2_t ( rect.right + 1, rect.bottom + 1 ), col_t ( 10, 10, 10, bg_alpha ) );
		render::rect ( vec2_t ( rect.left, rect.top ), vec2_t ( rect.right, rect.bottom ), color );
	}

	void c_player_esp::oof_indicate ( valve::c_player* player ) {
		if ( !hacks::g_aim_bot->cfg ( ).m_bOOFArrows )
			return;

		if ( !player->weapon ( ) )
			return;

		float width = 10.f;
		vec3_t viewangles = valve::g_engine->view_angles ( );

		constexpr auto k_pi = 3.14159265358979323846;

		const auto& rot = math::to_rad ( viewangles.y - math::calc_ang ( valve::g_local_player->origin ( ), player->origin ( ) ).y - 90.f );
		auto radius = 50 + 50;
		auto size = 27;

		int w, h;

		valve::g_engine->get_screen_size ( w, h );

		auto center = ImVec2 ( w / 2.f, h / 2.f );

		auto pos = ImVec2 ( center.x + radius * cosf ( rot ) * ( 2 * ( 0.5f + 10 * 0.5f * 0.01f ) ), center.y + radius * sinf ( rot ) );
		auto line = pos - center;

		auto arrowBase = pos - ( line * ( size / ( 2 * ( tanf ( k_pi / 4 ) / 2 ) * line.length ( ) ) ) );
		auto normal = ImVec2 ( -line.y, line.x );
		auto left = arrowBase + normal * ( size / ( 2 * line.length ( ) ) );
		auto right = arrowBase + normal * ( -size / ( 2 * line.length ( ) ) );

		auto clr = col_t ( 255, 255, 255, m_dormant_data.at ( player->index ( ) ).m_alpha );
		auto flags_backup = render::m_draw_list->Flags;
		render::m_draw_list->Flags |= ImDrawListFlags_AntiAliasedFill | ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedLines;
		render::triangle_filled ( left.x, left.y, right.x, right.y, pos.x, pos.y,
			clr );
		render::m_draw_list->Flags = flags_backup;
	}

	void c_player_esp::draw_skeletons ( valve::c_player* player, RECT& rect ) {

		if ( !hacks::g_aim_bot->cfg ( ).m_bSkeleton )
			return;

		if ( player->dormant ( ) )
			return;

		const auto entry = hacks::g_lag_comp->entry( player->index( ) - 1 );

		const auto mdl_data = player->mdl_data ( );
		if ( !mdl_data
			|| !mdl_data->m_studio_hdr )
			return;

		vec2_t screen_pos {}, screen_parent_pos {};

		const auto& cached_bones = entry.m_bones;

		for ( std::size_t i {}; i < mdl_data->m_studio_hdr->m_bones_count; ++i ) {
			const auto bone = mdl_data->m_studio_hdr->bone ( i );
			if ( !bone
				|| !( bone->m_flags & 0x100 )
				|| bone->m_parent == -1 )
				continue;

			math::to_screen (
				{
					cached_bones [ i ][ 0 ][ 3 ],
					cached_bones [ i ][ 1 ][ 3 ],
					cached_bones [ i ][ 2 ][ 3 ]
				},
				ImGui::GetIO ( ).DisplaySize,
				valve::g_engine->w2s_matrix ( ),
				screen_pos
			);

			math::to_screen (
				{
					cached_bones [ bone->m_parent ][ 0 ][ 3 ],
					cached_bones [ bone->m_parent ][ 1 ][ 3 ],
					cached_bones [ bone->m_parent ][ 2 ][ 3 ]
				},
				ImGui::GetIO ( ).DisplaySize,
				valve::g_engine->w2s_matrix ( ),
				screen_parent_pos
			);

			render::draw_line ( screen_pos.x, screen_pos.y, screen_parent_pos.x, screen_parent_pos.y, col_t ( 255, 255, 255, m_dormant_data.at ( player->index ( ) ).m_alpha ) );
		}
	}

	void c_player_esp::draw_ammo ( valve::c_player* player, RECT& rect ) {

		auto wpn = player->weapon ( );

		if ( !wpn )
			return;

		auto wpn_data = wpn->wpn_data ( );

		if ( !wpn_data )
			return;

		static float prev_ammo [ 65 ];

		if ( hacks::g_aim_bot->cfg ( ).m_bAmmo
			&& wpn_data
			&& wpn_data->m_type != 0
			&& wpn_data->m_type < 7
			&& wpn_data->m_max_clip1 != -1 ) {

			if ( prev_ammo [ player->index ( ) ] > wpn->ammo ( ) )
				prev_ammo [ player->index ( ) ] -= 0.01f;
			else {
				if ( prev_ammo [ player->index ( ) ] != wpn->ammo ( ) )
					prev_ammo [ player->index ( ) ] = wpn->ammo ( );
			}

			if ( prev_ammo [ player->index ( ) ] < wpn->ammo ( ) || prev_ammo [ player->index ( ) ] > wpn_data->m_max_clip1 )
				prev_ammo [ player->index ( ) ] = wpn->ammo ( );

			float box_width = fabsf ( rect.right - rect.left );
			float current_box_width = ( box_width * prev_ammo [ player->index ( ) ] ) / wpn_data->m_max_clip1;

			if ( player->lookup_seq_act ( player->anim_layers ( ).at ( 1 ).m_sequence ) == 967 )
				current_box_width = box_width * player->anim_layers ( ).at ( 1 ).m_cycle;

			auto clr = col_t ( 255, 255, 255, m_dormant_data [ player->index ( ) ].m_alpha );

			auto magic_clr = player->dormant ( ) ? col_t ( 0.f, 0.f, 0.f, m_dormant_data.at ( player->index ( ) ).m_alpha ) : col_t ( 0.0f, 0.0f, 0.0f, 100.0f );

			render::rect_filled ( vec2_t ( rect.right + 1, rect.bottom + 2 ), vec2_t ( rect.left - 1, rect.bottom + 6 ), magic_clr );
			render::rect_filled ( vec2_t ( rect.left, rect.bottom + 3 ), vec2_t ( rect.left + current_box_width, rect.bottom + 5 ), clr );

			render::text ( std::to_string ( wpn->ammo ( ) ), vec2_t ( rect.right + 3, rect.bottom ), col_t ( 255, 255, 255, m_dormant_data.at ( player->index ( ) ).m_alpha ), hacks::g_visuals->m_fonts.m_skeet_font_esp, FONT_OUTLINE );
		}

	}

	void c_player_esp::draw_name ( valve::c_player* player, RECT& rect ) {
		if ( !hacks::g_aim_bot->cfg ( ).m_bName )
			return;

		auto player_info = valve::g_engine->player_info ( player->index ( ) );

		std::string name;

		if ( player_info.has_value ( ) )
			name = player_info.value ( ).m_name;

		if ( name.length ( ) > 36 )
		{
			name.erase ( 36, name.length ( ) - 36 );
			name.append ( xorstr_ ( "..." ) );
		}

		auto width = abs ( rect.right - rect.left );

		const auto size = hacks::g_visuals->m_fonts.m_04b->CalcTextSizeA( 9.f, FLT_MAX, NULL, name.c_str( ) );

		render::text ( name, vec2_t ( rect.left + width * 0.5f, rect.top - size.y - 2 ), col_t ( 255, 255, 255, ( int ) m_dormant_data [ player->index ( ) ].m_alpha ), hacks::g_visuals->m_fonts.m_04b, FONT_OUTLINE | FONT_CENTERED_X );
	}

	void c_player_esp::draw_flags ( valve::c_player* player, RECT& rect ) {
		if ( !player->weapon ( ) )
			return;

		if ( !hacks::g_aim_bot->cfg ( ).m_bFlags )
			return;

		int count { 1 };

		std::vector < flags_data_t > flags_data {};

		flags_data.push_back ( { std::to_string ( player->money ( ) ), 1.f, col_t ( 150, 200, 60, 255 ) } );

		const auto& entry = hacks::g_lag_comp->entry ( player->index ( ) - 1 );

		{
			static float kevlar_add_anim [ 65 ] = {};

			auto kevlar = player->armor_value ( ) > 0;
			auto helmet = player->has_helmet ( );

			std::string text;

			if ( helmet && kevlar ) {
				kevlar_add_anim [ player->index ( ) ] += ( 540.f / 1.f ) * valve::g_global_vars->m_frame_time;
				text = xorstr_ ( "HK" );
			}
			else if ( kevlar ) {
				kevlar_add_anim [ player->index ( ) ] += ( 540.f / 1.f ) * valve::g_global_vars->m_frame_time;
				text = xorstr_ ( "K" );
			}
			else {
				text = ( "" );
				kevlar_add_anim [ player->index ( ) ] -= ( 540.f / 1.f ) * valve::g_global_vars->m_frame_time;
			}

			kevlar_add_anim [ player->index ( ) ] = std::clamp ( kevlar_add_anim [ player->index ( ) ], 0.f, 255.f );

			flags_data.push_back ( { text, kevlar_add_anim [ player->index ( ) ], col_t ( 240, 240, 240, static_cast < int > ( kevlar_add_anim [ player->index ( ) ] ) ) } );
		}

		// scoped
		{
			static float scoped_alpha_anim [ 65 ] = {};

			std::string scoped_str {};

			if ( player->scoped ( ) ) {
				scoped_str = xorstr_ ( "ZOOM" );
				scoped_alpha_anim [ player->index ( ) ] += ( 540.f / 1.f ) * valve::g_global_vars->m_frame_time;
			}
			else {
				scoped_alpha_anim [ player->index ( ) ] -= ( 540.f / 1.f ) * valve::g_global_vars->m_frame_time;
				scoped_str = ( "" );
			}

			scoped_alpha_anim [ player->index ( ) ] = std::clamp ( scoped_alpha_anim [ player->index ( ) ], 0.f, 255.f );

			flags_data.push_back ( { scoped_str, scoped_alpha_anim [ player->index ( ) ], col_t ( 0, 153, 204, static_cast < int > ( scoped_alpha_anim [ player->index ( ) ] ) ) } );
		}

		// fake duck
		{
			static float fd_alpha_anim [ 65 ] = {};

			std::string fd_str {};

			auto animstate = player->anim_state ( );

			if ( animstate ) {

				auto fakeducking = [ & ] ( ) -> bool {

					static auto stored_tick = 0;
					static int crouched_ticks [ 65 ];

					if ( animstate->m_duck_amount ) {
						if ( animstate->m_duck_amount < 0.9f && animstate->m_duck_amount > 0.5f ) {
							if ( stored_tick != valve::g_global_vars->m_tick_count ) {
								crouched_ticks [ player->index ( ) ]++;
								stored_tick = valve::g_global_vars->m_tick_count;
							}

							return crouched_ticks [ player->index ( ) ] > 16;
						}
						else
							crouched_ticks [ player->index ( ) ] = 0;
					}

					return false;
				};

				if ( fakeducking ( ) && player->flags ( ) & valve::e_ent_flags::on_ground && !animstate->m_landing ) {
					fd_str = xorstr_ ( "FD" );
					fd_alpha_anim [ player->index ( ) ] += ( 540.f / 1.f ) * valve::g_global_vars->m_frame_time;
				}
				else {
					fd_alpha_anim [ player->index ( ) ] -= ( 540.f / 1.f ) * valve::g_global_vars->m_frame_time;
					fd_str = ( "" );
				}
			}

			fd_alpha_anim [ player->index ( ) ] = std::clamp ( fd_alpha_anim [ player->index ( ) ], 0.f, 255.f );

			flags_data.push_back ( { fd_str, fd_alpha_anim [ player->index ( ) ], col_t ( 212, 219, 206, static_cast < int > ( fd_alpha_anim [ player->index ( ) ] ) ) } );
		}

		// c4
		{
			static float c4_alpha_anim [ 65 ];
			std::string c4_str {};

			if ( player->has_c4 ( ) ) {
				c4_str = xorstr_ ( "BOMB" );
				c4_alpha_anim [ player->index ( ) ] += ( 540.f / 1.f ) * valve::g_global_vars->m_frame_time;
			}
			else {
				c4_alpha_anim [ player->index ( ) ] -= ( 540.f / 1.f ) * valve::g_global_vars->m_frame_time;
				c4_str = ( "" );
			}

			c4_alpha_anim [ player->index ( ) ] = std::clamp ( c4_alpha_anim [ player->index ( ) ], 0.f, 255.f );

			flags_data.push_back ( { c4_str, c4_alpha_anim [ player->index ( ) ], col_t ( 209, 212, 207, static_cast < int > ( c4_alpha_anim [ player->index ( ) ] ) ) } );
		}

		static float lc_alpha_anim [ 65 ];
		std::string lc_str {};

		if ( !entry.m_lag_records.empty ( ) ) {
			auto lag_record = entry.m_lag_records.back ( ).get ( );

			if ( lag_record && !lag_record->m_dormant ) {
				if ( lag_record->m_broke_lc ) {
					lc_str = xorstr_ ( "LC" );
					lc_alpha_anim [ player->index ( ) ] += ( 540.f / 1.f ) * valve::g_global_vars->m_frame_time;
				}
				else {
					lc_str = "";
					lc_alpha_anim [ player->index ( ) ] -= ( 540.f / 1.f ) * valve::g_global_vars->m_frame_time;
				}
			}
			else {
				lc_str = "";
				lc_alpha_anim [ player->index ( ) ] -= ( 540.f / 1.f ) * valve::g_global_vars->m_frame_time;
			}
		}
		else {
			lc_str = "";
			lc_alpha_anim [ player->index ( ) ] -= ( 540.f / 1.f ) * valve::g_global_vars->m_frame_time;
		}

		lc_alpha_anim [ player->index ( ) ] = std::clamp ( lc_alpha_anim [ player->index ( ) ], 0.f, 255.f );

		flags_data.push_back ( { lc_str, lc_alpha_anim [ player->index ( ) ], col_t ( 255, 16, 16, static_cast < int > ( lc_alpha_anim [ player->index ( ) ] ) ) } );

		for ( auto& it : flags_data ) {

			col_t clr = player->dormant ( ) ? col_t ( it.m_clr.r ( ), it.m_clr.g ( ), it.m_clr.b ( ), m_dormant_data.at ( player->index ( ) ).m_alpha ) : it.m_clr;

			if ( !it.m_name.length ( ) )
				continue;

			render::text ( it.m_name, vec2_t ( rect.right + 5, rect.top + 9 * count - 4 - 7 ), clr, hacks::g_visuals->m_fonts.m_skeet_font_esp, FONT_OUTLINE );

			count++;
		}
	}

	void c_weather::update_weather ( ) {
		if ( m_weather_created )
			return;

		if ( !hacks::g_aim_bot->cfg ( ).m_bRain )
			return;

		static valve::client_class_t* precipiation = nullptr;

		if ( !precipiation ) {
			for ( auto cl_class = valve___g_client->all_classes ( ); cl_class && !precipiation; cl_class = cl_class->m_next ) {
				if ( cl_class->m_class_id == valve::e_class_id::precipitation )
					precipiation = cl_class;
			}
		}

		if ( precipiation
			&& precipiation->m_create_fn ) {
			valve::i_client_networkable* rain_networkable = ( ( valve::i_client_networkable * ( * ) ( int, int ) )precipiation->m_create_fn ) ( max_edicts - 1, 0 );

			if ( !rain_networkable )
				return;

			auto rain_unk = ( ( valve::i_client_renderable* ) rain_networkable )->get_client_unknown ( );

			if ( !rain_unk )
				return;

			auto rain_ent = rain_unk->get_base_entity ( );

			if ( !rain_ent )
				return;

			if ( !rain_ent->networkable ( ) )
				return;

			static auto nigga_cheats = valve___g_cvar->find_var ( xorstr_ ( "r_RainCheck" ) );

			nigga_cheats->m_callbacks.m_size = 0;

			nigga_cheats->set_int ( 1 );

			rain_networkable->pre_data_update ( 0 );
			rain_networkable->on_pre_data_changed ( 0 );

			rain_ent->precip_type ( ) = 0;
			rain_ent->obb_min ( ) = vec3_t ( -32768.f, -32768.f, -32768.f );
			rain_ent->obb_max ( ) = vec3_t ( 32768.f, 32768.f, 32768.f );

			using fn_t = void ( __thiscall* )( const std::uintptr_t, const int );

			const auto networkable = rain_ent->networkable ( );
			if ( networkable ) {
				( *reinterpret_cast< fn_t** >( networkable ) ) [ 5u ] ( networkable, 0 );
				( *reinterpret_cast< fn_t** >( networkable ) ) [ 7u ] ( networkable, 0 );
			}

			m_weather_created = true;
		}
	}

	void c_player_esp::draw_weapon ( valve::c_player* player, RECT& rect ) {
		if ( !hacks::g_aim_bot->cfg ( ).m_bWeaponIcon && !hacks::g_aim_bot->cfg ( ).m_bWeaponText )
			return;

		if ( !player->weapon ( ) || !player->weapon ( )->wpn_data ( ) )
			return;

		int offset { -2 };

		bool has_smth{};

		if ( hacks::g_aim_bot->cfg ( ).m_bAmmo )
			offset += 6;

		has_smth = offset != 2;

		if ( hacks::g_aim_bot->cfg ( ).m_bWeaponText ) {
			render::text ( player->weapon ( )->get_weapon_name ( ), vec2_t ( rect.left + ( abs ( rect.right - rect.left ) * 0.5f ), rect.bottom + offset + 3 ), col_t ( 255, 255, 255, ( int ) m_dormant_data [ player->index ( ) ].m_alpha ), hacks::g_visuals->m_fonts.m_skeet_font_esp, FONT_OUTLINE | FONT_CENTERED_X );

			if ( has_smth )
				offset += 10;
			else
				offset += 7;
		}

		if ( hacks::g_aim_bot->cfg ( ).m_bWeaponIcon )
			render::text ( player->weapon ( )->get_weapon_icon ( ), vec2_t ( rect.left + ( abs ( rect.right - rect.left ) * 0.5f ), rect.bottom + offset + 1 ), col_t ( 255, 255, 255, ( int ) m_dormant_data [ player->index ( ) ].m_alpha ), hacks::g_visuals->m_fonts.m_icon_font, FONT_OUTLINE | FONT_CENTERED_X );
	}

	void c_player_esp::draw_health ( valve::c_player* player, RECT& rect ) {
		if ( !hacks::g_aim_bot->cfg ( ).m_bHealth )
			return;
		float box_height = static_cast< float >( rect.bottom - rect.top );

		int red = 150;
		int green = 0xFF;
		int blue = 0x50;

		if ( player->health ( ) >= 27 )
		{
			if ( player->health ( ) < 57 )
			{
				red = 0xD7;
				green = 0xC8;
				blue = 0x50;
			}
		}
		else
		{
			red = 0xFF;
			green = 0x32;
			blue = 0x50;
		}

		static float last_hp [ 65 ];

		if ( last_hp [ player->index ( ) ] > player->health ( ) )
			last_hp [ player->index ( ) ] -= ( 255.f / 1.f ) * valve::g_global_vars->m_frame_time;
		else
			last_hp [ player->index ( ) ] = player->health ( );

		col_t color = col_t ( red, green, blue, ( int ) m_dormant_data [ player->index ( ) ].m_alpha );
		auto bg_alpha = std::clamp ( ( int ) m_dormant_data [ player->index ( ) ].m_alpha, 0, 100 );
		float colored_bar_height = ( ( box_height * std::min ( last_hp [ player->index ( ) ], 100.f ) ) / 100.0f );
		float colored_max_bar_height = ( ( box_height * 100.0f ) / 100.0f );

		render::rect_filled ( vec2_t ( rect.left - 5.0f, rect.top - 1 ), vec2_t ( rect.left - 2.0f, rect.top + colored_max_bar_height + 1 ), col_t ( 0.0f, 0.0f, 0.0f, ( float ) bg_alpha ) );
		render::rect_filled ( vec2_t ( rect.left - 4.0f, rect.top + ( colored_max_bar_height - colored_bar_height ) ), vec2_t ( rect.left - 3.0f, rect.top + colored_max_bar_height ), color );

		if ( player->health ( ) < 100 ) {
			render::text ( std::to_string ( player->health ( ) ), vec2_t ( rect.left - 11.f,
				rect.top - 2.0f ), col_t ( 255, 255, 255, ( int ) m_dormant_data [ player->index ( ) ].m_alpha ), hacks::g_visuals->m_fonts.m_skeet_font_esp, FONT_OUTLINE | FONT_CENTERED_X );
		}
	}

	void c_visuals::change_agent ( valve::e_frame_stage stage )
	{

	}

	RECT c_player_esp::get_bbox ( valve::c_entity* ent )
	{
		RECT rect { };

		if ( ent->is_player ( ) ) { 
			vec3_t origin, mins, maxs;
			vec3_t bottom, top;

			if ( ( ( valve::c_player* ) ent )->alive( ) )
				origin = ent->dormant( ) ? ent->abs_origin( ) : hacks::g_lag_comp->entry( ent->index( ) - 1 ).m_render_origin;
			else
				origin = m_alive_origin.at( ent->index( ) );

			const auto on_screen = render::world_to_screen( origin, bottom );

			origin.z += ent->obb_max( ).z;

			if ( !render::world_to_screen( origin, top )
				&& !on_screen )
				return RECT{};

			float x, y, w, h;
			 h = bottom.y - top.y;
			 w = h / 2.f;
			 x = bottom.x - ( w / 2.f );
	         y = bottom.y - h;

			 return RECT { long ( x ), long ( y ), long ( x + w ), long ( y + h ) };
		}
		else
		{
			auto min = ent->obb_min ( );
			auto max = ent->obb_max ( );

			const mat3x4_t& trans = ent->get_rgfl ( );
			vec3_t points[ ] = {
				vec3_t ( min.x, min.y, min.z ),
				vec3_t ( min.x, max.y, min.z ),
				vec3_t ( max.x, max.y, min.z ),
				vec3_t ( max.x, min.y, min.z ),
				vec3_t ( max.x, max.y, max.z ),
				vec3_t ( min.x, max.y, max.z ),
				vec3_t ( min.x, min.y, max.z ),
				vec3_t ( max.x, min.y, max.z )
			};

			vec3_t points_transformed[ 8 ];
			for ( int i = 0; i < 8; i++ )
				math::vector_transform ( points[ i ], trans, points_transformed[ i ] );

			vec3_t screen_points[ 8 ] = { };
			for ( int i = 0; i < 8; i++ )
				if ( !render::world_to_screen ( points_transformed[ i ], screen_points[ i ] ) )
					return rect;

			auto left = screen_points[ 0 ].x;
			auto top = screen_points[ 0 ].y;
			auto right = screen_points[ 0 ].x;
			auto bottom = screen_points[ 0 ].y;

			for ( int i = 1; i < 8; i++ )
			{
				if ( left > screen_points[ i ].x )
					left = screen_points[ i ].x;
				if ( top < screen_points[ i ].y )
					top = screen_points[ i ].y;
				if ( right < screen_points[ i ].x )
					right = screen_points[ i ].x;
				if ( bottom > screen_points[ i ].y )
					bottom = screen_points[ i ].y;
			}

			return RECT { ( long )( left ), ( long )( bottom ), ( long )( right ), ( long )( top ) };
		}
	}


	void c_hit_marker::add_shot ( const hit_marker_data_t& data )
	{
		m_hits.push_back ( data );
	}

	void c_hit_marker::handle_dmg_render ( hit_marker_data_t& data, const vec3_t& screen_pos )
	{

		const auto step = 255.f / 1.0f * valve___g_global_vars->m_frame_time;

		const auto step_move = 30.f / 1.5f * valve___g_global_vars->m_frame_time;

		data.m_moved += step_move;

		if ( data.m_time + 1.2f <= valve___g_global_vars->m_cur_time )
			data.m_alpha -= step;

		const auto int_alpha = static_cast< int >( data.m_alpha );

		if ( int_alpha > 0 )
		{
			std::stringstream dmg;

			dmg << "-" << std::to_string ( data.m_dmg );

			render::text ( dmg.str ( ), vec2_t ( screen_pos.x + data.m_add_to_x, screen_pos.y - data.m_moved ), col_t( 255, 255, 255, int_alpha ), hacks::g_visuals->m_fonts.m_skeet_font_esp, FONT_OUTLINE );
		}
	}

	void c_hit_marker::handle_draw ( )
	{
		if ( !hacks::g_aim_bot->cfg ( ).m_bDamageMarker )
			return;

		for ( auto i = 0; i < m_hits.size ( ); i++ )
		{
			auto& hit = m_hits[ i ];

			if ( hit.m_time + 2.1f < valve___g_global_vars->m_cur_time )
			{
				m_hits.erase ( m_hits.begin ( ) + i );

				i--;
			}

			vec3_t screen_pos{};

			if ( render::world_to_screen ( hit.m_pos, screen_pos ) )
			{
				handle_dmg_render ( hit, screen_pos );
			}
		}
	}
}

__forceinline bool is_vec_zero ( vec3_t& vec ) { 
	return vec.x == 0.f && vec.y == 0.f && vec.z == 0.f;
}

__forceinline bool is_vec_valid ( vec3_t& vec ) {
	return std::isfinite ( vec.x ) && std::isfinite ( vec.y );
}

__forceinline bool compare_vec ( vec3_t& vec_1, vec3_t& vec_2 ) { 
	return vec_1.x == vec_2.x && vec_1.y == vec_2.y && vec_1.z == vec_2.z;
}

namespace render {

	bool world_to_screen ( const vec3_t& origin, vec3_t& screen )
	{
		const auto screen_transform = [ &origin, &screen ] ( ) -> bool
		{
			static std::uintptr_t view_matrix;
			if ( !view_matrix )
			{
				view_matrix = nem::g_context->addresses ( ).m_view_matrix.get( );
				view_matrix += 3;
				view_matrix = *reinterpret_cast< std::uintptr_t* >( view_matrix );
				view_matrix += 176;
			}

			const v_matrix& w2s_matrix = *reinterpret_cast< v_matrix* >( view_matrix );
			screen.x = w2s_matrix[ 0 ][ 0 ] * origin.x + w2s_matrix[ 0 ][ 1 ] * origin.y + w2s_matrix[ 0 ][ 2 ] * origin.z + w2s_matrix[ 0 ][ 3 ];
			screen.y = w2s_matrix[ 1 ][ 0 ] * origin.x + w2s_matrix[ 1 ][ 1 ] * origin.y + w2s_matrix[ 1 ][ 2 ] * origin.z + w2s_matrix[ 1 ][ 3 ];

			float w = w2s_matrix[ 3 ][ 0 ] * origin.x + w2s_matrix[ 3 ][ 1 ] * origin.y + w2s_matrix[ 3 ][ 2 ] * origin.z + w2s_matrix[ 3 ][ 3 ];

			if ( w < 0.001f )
			{
				screen.x *= 100000;
				screen.y *= 100000;
				return true;
			}

			float invw = 1.f / w;
			screen.x *= invw;
			screen.y *= invw;

			return false;
		};

		if ( !screen_transform ( ) )
		{
			int screen_width, screen_height;
			valve___g_engine->get_screen_size ( screen_width, screen_height );

			screen.x = ( screen_width * 0.5f ) + ( screen.x * screen_width ) * 0.5f;
			screen.y = ( screen_height * 0.5f ) - ( screen.y * screen_height ) * 0.5f;

			return true;
		}

		return false;
	}

	void render_filled_3d_circle ( const vec3_t& origin, float radius, col_t color )
	{
		static constexpr auto pi = 3.14159265358979323846;
		static auto prev_screen_pos = vec3_t ( );
		static auto step = pi * 2.0f / 72.0f;

		auto screen_pos = vec3_t ( );
		auto screen = vec3_t ( );

		if ( !world_to_screen ( origin, screen ) )
			return;

		for ( auto rotation = 0.0f; rotation <= pi * 2.0f; rotation += step )
		{
			vec3_t pos ( radius * cos ( rotation ) + origin.x, radius * sin ( rotation ) + origin.y, origin.z );

			if ( world_to_screen ( pos, screen_pos ) )
			{
				if ( !is_vec_zero ( prev_screen_pos ) && is_vec_valid ( prev_screen_pos ) && is_vec_valid ( screen_pos ) && !compare_vec ( prev_screen_pos, screen_pos ) )
				{
					line ( vec2_t ( prev_screen_pos.x, prev_screen_pos.y ), vec2_t ( screen_pos.x, screen_pos.y ), color );
					triangle ( screen.x, screen.y, screen_pos.x, screen_pos.y, prev_screen_pos.x, prev_screen_pos.y, col_t ( color.r ( ), color.g ( ), color.b ( ), color.a ( ) / 2 ), 1.f );
				}

				prev_screen_pos = screen_pos;
			}
		}
	}

	vec2_t get_text_size ( std::string_view txt, ImFont* font )
	{
		if ( !font
			|| txt.empty ( )
			|| !font->IsLoaded ( ) )
			return vec2_t ( );

		const auto size = font->CalcTextSizeA ( font->FontSize, std::numeric_limits<float>::max ( ), 0.f, txt.data ( ) );

		return vec2_t ( IM_FLOOR ( size.x + 0.95f ), size.y );
	}

	void triangle ( float x1, float y1, float x2, float y2, float x3, float y3, col_t clr, float thickness )
	{
		m_draw_list->AddTriangle ( ImVec2 ( x1, y1 ), ImVec2 ( x2, y2 ), ImVec2 ( x3, y3 ), clr.hex ( ), thickness );
	}

	void triangle_filled ( float x1, float y1, float x2, float y2, float x3, float y3, col_t clr )
	{
		m_draw_list->AddTriangleFilled ( ImVec2 ( x1, y1 ), ImVec2 ( x2, y2 ), ImVec2 ( x3, y3 ), clr.hex ( ) );
	}

	void text ( std::string_view txt, vec2_t pos, const col_t& clr, ImFont* font, bit_flag_t<uint8_t> flags )
	{
		if ( !font
			|| txt.empty ( )
			|| clr.a ( ) <= 0
			|| !font->IsLoaded ( ) )
			return;

		const auto centered_x = flags.has ( FONT_CENTERED_X );
		const auto centered_y = flags.has ( FONT_CENTERED_Y );

		if ( centered_x
			|| centered_y ) {
			const auto text_size = get_text_size ( txt, font );

			if ( centered_x ) {
				pos.x -= text_size.x / 2.f;
			}

			if ( centered_y ) {
				pos.y -= text_size.y / 2.f;
			}
		}

		m_draw_list->PushTextureID ( font->ContainerAtlas->TexID );

		if ( flags.has ( FONT_DROP_SHADOW ) ) {
			m_draw_list->AddTextSoftShadow ( font, font->FontSize, *reinterpret_cast< ImVec2* >( &pos ), clr.hex ( ), txt.data ( ) );
		}
		else if ( flags.has ( FONT_OUTLINE ) ) {
			m_draw_list->AddTextOutline ( font, font->FontSize, *reinterpret_cast< ImVec2* >( &pos ), 
				clr.hex ( ), txt.data ( ), NULL, static_cast < float > ( clr.a ( ) - 20 ) );
		}
		else {
			m_draw_list->AddText ( font, font->FontSize, *reinterpret_cast< ImVec2* >( &pos ), clr.hex ( ), txt.data ( ) );
		}

		m_draw_list->PopTextureID ( );
	}

	void line ( const vec2_t& from, const vec2_t& to, const col_t& clr )
	{
		m_draw_list->AddLine ( *reinterpret_cast< const ImVec2* >( &from ), *reinterpret_cast< const ImVec2* >( &to ), clr.hex ( ) );
	}

	void draw_line ( float x1, float y1, float x2, float y2, col_t clr, float thickness )
	{
		m_draw_list->AddLine ( ImVec2 ( x1, y1 ), ImVec2 ( x2, y2 ), clr.hex ( ), thickness );
	}

	void rect ( const vec2_t& pos, const vec2_t& size, const col_t& clr, float rounding, bool multiplied )
	{
		m_draw_list->AddRect ( *reinterpret_cast< const ImVec2* >( &pos ), multiplied ? ImVec2 ( pos.x + size.x, pos.y + size.y ) : ImVec2 ( size.x, size.y ), clr.hex ( ), rounding );
	}

	void rect_filled ( const vec2_t& pos, const vec2_t& size, const col_t& clr, float rounding, bool multiplied )
	{
		m_draw_list->AddRectFilled ( ImVec2 ( pos.x, pos.y ), multiplied ? ImVec2 ( pos.x + size.x, pos.y + size.y ) : ImVec2 ( size.x, size.y ), clr.hex ( ), rounding );
	}

	void rect_filled_multi_clr ( const vec2_t& pos, const vec2_t& size, const col_t& clr_upr_left, const col_t& clr_upr_right, const col_t& clr_bot_left, const col_t& clr_bot_right )
	{
		m_draw_list->AddRectFilledMultiColor ( *reinterpret_cast< const ImVec2* >( &pos ), ImVec2 ( pos.x + size.x, pos.y + size.y ), clr_upr_left.hex ( ), clr_upr_right.hex ( ), clr_bot_right.hex ( ), clr_bot_left.hex ( ) );
	}

	void draw_rect_filled ( float x, float y, float w, float h, col_t clr, float rounding, ImDrawCornerFlags rounding_corners )
	{
		m_draw_list->AddRectFilled ( ImVec2 ( x, y ), ImVec2 ( x + w, y + h ), clr.hex ( ), rounding, rounding_corners );
	}

	void draw_rect ( float x1, float y1, float x2, float y2, col_t color )
	{
		m_draw_list->AddRect ( ImVec2 ( x1, y1 ), ImVec2 ( x2, y2 ), color.hex ( ), 0.0f );
	}

	void polygon ( const std::vector<vec2_t>& points, const col_t& clr )
	{
		if ( clr.a ( ) <= 0 )
			return;

		m_draw_list->_Path.reserve ( m_draw_list->_Path.Size + points.size ( ) + 1 );

		for ( auto& point : points ) {
			m_draw_list->_Path.push_back ( *reinterpret_cast< const ImVec2* >( &point ) );
		}

		m_draw_list->PathStroke ( clr.hex ( ), true, 1.f );
	}
	void add_to_draw_list ( )
	{
		const auto lock = std::unique_lock<std::mutex> ( m_mutex, std::try_to_lock );
		if ( lock.owns_lock ( ) ) {
			*m_replace_draw_list = *m_data_draw_list;
		}

		*ImGui::GetBackgroundDrawList ( ) = *m_replace_draw_list;
	}
	void polygon_filled ( const std::vector<vec2_t>& points, const col_t& clr )
	{
		if ( clr.a ( ) <= 0 )
			return;

		m_draw_list->_Path.reserve ( m_draw_list->_Path.Size + points.size ( ) + 1 );

		for ( auto& point : points ) {
			m_draw_list->_Path.push_back ( *reinterpret_cast< const ImVec2* >( &point ) );
		}

		m_draw_list->PathFillConvex ( clr.hex ( ) );
	}

	void render_3d_circle ( const vec3_t& origin, float radius, col_t color )
	{

		static vec3_t previous_screen_pos = vec3_t ( 0, 0, 0 );
		static float_t step = 3.14159265358979323846f * 2.0f / 72.0f;

		vec3_t screen_position = vec3_t ( 0, 0, 0 );
		if ( !render::world_to_screen ( origin, screen_position ) )
			return;

		for ( float_t rotation = 0.0f; rotation <= 3.14159265358979323846f * 2.0f; rotation += step )
		{
			vec3_t world_position = vec3_t ( radius * cos ( rotation ) + origin.x, radius * sin ( rotation ) + origin.y, origin.z );
			if ( !render::world_to_screen ( world_position, screen_position ) )
				continue;

			draw_line ( previous_screen_pos.x, previous_screen_pos.y, screen_position.x, screen_position.y, color, 1.0f );
			triangle
			(
				screen_position.x, screen_position.y,
				screen_position.x, screen_position.y,
				previous_screen_pos.x, previous_screen_pos.y,
				col_t ( color.r ( ), color.g ( ), color.b ( ), color.a ( ) / 2 ), 1.f
			);

			previous_screen_pos = screen_position;
		}
	}

	void arc ( float x, float y, float radius, float min_angle, float max_angle, col_t col, float thickness )
	{
		m_draw_list->PathArcTo ( ImVec2 ( x, y ), radius, math::to_rad ( min_angle ), math::to_rad ( max_angle ), 32 );
		m_draw_list->PathStroke ( col.hex ( ), false, thickness );
	}

	void render_convex_poly_filled ( ImVec2* vec, int num_points, col_t col )
	{
		m_draw_list->AddConvexPolyFilled ( vec, num_points, col.hex ( ) );
	}
	void handle_drawings ( )
	{
		m_draw_list->Clear ( );
		m_draw_list->PushClipRectFullScreen ( );

		m_screen_size = *reinterpret_cast< vec2_t* >( &ImGui::GetIO ( ).DisplaySize );

		nem::hacks::g_player_esp->draw_data ( );

		nem::hacks::g_cached_renderer->render ( );

		nem::hacks::g_logs->draw_data ( );

		nem::hacks::g_hit_marker->handle_draw ( );
		nem::hacks::g_visuals->draw_auto_peek ( );

		if ( nem::valve::g_local_player && nem::valve::g_local_player->alive ( ) )
		{
			if ( nem::hacks::g_aim_bot->cfg ( ).m_bBulletTracers ) {

				if ( !nem::hacks::g_visuals->m_bullet_tracers.empty ( ) )
				{
					if ( nem::hacks::g_visuals->m_bullet_tracers.size ( ) >= 3 ) { 
						nem::hacks::g_visuals->m_bullet_tracers.pop_front ( );
					}

					auto it = nem::hacks::g_visuals->m_bullet_tracers.begin ( );

					while ( it != nem::hacks::g_visuals->m_bullet_tracers.end ( ) )
					{
						auto& cur = *it;
						vec3_t start {};
						vec3_t end {};
						auto next = ( it + 1 );
						auto life = valve___g_global_vars->m_cur_time - cur.m_spawn_time;

						if ( life > 2.f )
							cur.m_alpha_modifier -= 9.0f * valve___g_global_vars->m_frame_time;
						else
							cur.m_alpha_modifier = 1.f;

						cur.m_alpha_modifier = std::clamp ( cur.m_alpha_modifier, 0.f, 1.f );
				
						if ( next != nem::hacks::g_visuals->m_bullet_tracers.end ( ) && cur.m_spawn_time == ( *next ).m_spawn_time/* || current.time == csgo.m_globals()->curtime*/ ) {
							it = nem::hacks::g_visuals->m_bullet_tracers.erase ( it );
							continue;
						}

						if ( render::world_to_screen ( cur.m_start_pos, start ) && render::world_to_screen ( cur.m_end_pos, end ) ) {
							render::draw_line ( start.x, start.y, end.x, end.y, col_t ( 255, 255, 255, 255 * cur.m_alpha_modifier ) );
						}

						if ( life > 3.f )
						{
							it = nem::hacks::g_visuals->m_bullet_tracers.erase ( it );
						}
						else
							it++;
					}
					//}

					//nem::hacks::g_visuals->m_bullet_tracers.clear ( );
				}
			}
		}
		else
		{
			nem::hacks::g_visuals->m_bullet_tracers.clear ( );
		}
		

		int ent_id = valve___g_entity_list->highest_index ( );

		static auto last_server_tick = valve___g_client_state->m_server_tick;
		if ( !nem::hacks::g_aim_bot->cfg ( ).m_bGrenadeProximityWarning
			|| valve___g_client_state->m_server_tick != last_server_tick )
			nem::hacks::g_visuals->m_throwed_grenades.clear ( );

		last_server_tick = valve___g_client_state->m_server_tick;

		for ( int i {}; i <= ent_id; ++i ) {

			const auto entity = valve___g_entity_list->find_entity ( i );
			if ( !entity )
				continue;

			if ( entity->is_player ( ) )
			{
				// who 
			}
			else if ( const auto client_class = entity->client_class ( ) ) {
				if ( entity->dormant ( ) )
					continue;

				if ( entity->is_weapon ( ) )
				{
					nem::hacks::g_visuals->handle_weapon ( static_cast< nem::valve::c_weapon* const >( entity ) );
				}
				else {
					if ( client_class->m_class_id == nem::valve::e_class_id::planted_c4 )
					{
						// asked
					}

					if ( client_class->m_class_id == nem::valve::e_class_id::cascade_light )
						nem::hacks::g_visuals->change_shadows ( entity );

					if ( client_class->m_class_id == nem::valve::e_class_id::tonemap_controller )
						nem::hacks::g_visuals->bloom_modulation ( entity );

					nem::hacks::g_visuals->handle_projectile ( entity, client_class->m_class_id );
				}
			}
		}

		if ( nem::hacks::g_aim_bot->cfg ( ).m_bGrenadePrediction )
			nem::hacks::g_visuals->add_grenade_simulation ( nem::hacks::g_visuals->m_grenade_trajectory, false );

		static const auto& callbacks = nem::g_lua->events ( )[ xorstr_ ( "on_paint" ) ];
		for ( const auto& callback : callbacks )
			callback.m_fn ( );


		{
			const auto lock = std::unique_lock<std::mutex> ( m_mutex );

			*m_data_draw_list = *m_draw_list;
		}
	}

	std::mutex m_mutex;

	vec2_t m_screen_size;

	std::shared_ptr<ImDrawList> m_draw_list;
	std::shared_ptr<ImDrawList> m_data_draw_list;
	std::shared_ptr<ImDrawList> m_replace_draw_list;
}

namespace fonts {
	ImFont* m_tahoma14 = nullptr;
}