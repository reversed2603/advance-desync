#pragma once

#include "../anim_sync.hpp"

namespace nem::hacks {
	__forceinline c_anim_sync::local_data_t& c_anim_sync::local_data( ) {
		return m_local_data;
	}
}