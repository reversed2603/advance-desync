#pragma once

namespace nem::hacks {

	class c_knife_bot 
	{
	public:
		void handle_knife_bot ( valve::user_cmd_t& cmd );
		bool select_target ( );

	protected:

		__forceinline int get_min_dmg ( )
		{
			if ( !valve::g_local_player || !valve::g_local_player->weapon ( ) )
				return -1;

			if ( valve::to_time ( valve::g_local_player->tick_base ( ) ) > ( valve::g_local_player->weapon ( )->next_primary_attack ( ) + 0.4f ) )
				return 34;

			return 21;
		}

		__forceinline vec3_t get_hitbox_pos ( int hitbox_id, valve::c_player* player )
		{
			if ( !this )
				return vec3_t ( );

			auto hdr = player->mdl_data ( )->m_studio_hdr;

			if ( !hdr )
				return vec3_t ( );

			auto hitbox_set = hdr->hitbox_set ( player->hitbox_set_index ( ) );

			if ( !hitbox_set )
				return vec3_t ( );

			auto hitbox = hitbox_set->hitbox ( hitbox_id );

			if ( !hitbox )
				return vec3_t ( );

			vec3_t min, max;

			math::vector_transform ( hitbox->m_min, player->bone_cache ( ).m_elements[ hitbox->m_bone ], min );
			math::vector_transform ( hitbox->m_max, player->bone_cache ( ).m_elements[ hitbox->m_bone ], max );

			return ( min + max ) * 0.5f;
		}


		__forceinline bool is_visible ( const vec3_t& start, const vec3_t& end, valve::c_player* player, valve::c_player* local )
		{
			valve::trace_t trace;

			valve::ray_t ray { start, end };

			valve::trace_filter_simple_t filter { player, 0 };
			filter.m_ignore_entity = local;

			valve::g_engine_trace->trace_ray ( ray, valve::e_mask::shot_player, reinterpret_cast< valve::trace_filter_t* >( &filter ), &trace );

			return trace.m_hit_entity == player || trace.m_fraction == 1.0f;
		}

		valve::c_player* m_best_player;
		int m_best_distance;
		int m_best_index;
	};

	inline const std::unique_ptr < c_knife_bot > g_knife_bot = std::make_unique < c_knife_bot > ( );

}