#include "../../../nem.hpp"

namespace nem::valve {
	void anim_state_t::set_layer_seq ( valve::anim_layer_t* layer, int act )
	{
		int32_t sequence = select_sequence_from_acitivty_modifier ( act );
		if ( sequence < 2 )
			return;

		layer->m_cycle = 0.0f;
		layer->m_weight = 0.0f;
		layer->m_sequence = sequence;
		layer->m_playback_rate = valve::g_local_player->get_layer_seq_cycle_rate ( layer, sequence );
	}
}