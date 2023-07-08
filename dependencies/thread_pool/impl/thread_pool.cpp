

#include /*"../../../sdk.hpp"*/"../nem.hpp"

namespace sdk {
	void c_thread_pool::thread_t::work_loop( std::stop_token token ) {
		auto& queue = g_thread_pool->queue( );

		for ( ; !token.stop_requested( ); ) {
			std::function< void( ) > task{};

			/* wait til get the task */
			queue.m_watcher.acquire( );

			{
				const std::unique_lock lock{ queue.m_mutex };

				if ( queue.m_elements.empty( ) )
					continue;

				/* start from the front, since we are pushing everything back */
				task = std::move( queue.m_elements.front( ) );

				queue.m_elements.pop_front( );
			}

			task( );

			--queue.m_tasks_count;
		}
	}
}