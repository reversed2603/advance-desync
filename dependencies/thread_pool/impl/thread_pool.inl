
#pragma once

#include "../thread_pool.hpp"

namespace sdk {
	template < typename _fn_t, typename... _args_t > requires std::is_invocable_v< _fn_t, _args_t... >
	ALWAYS_INLINE std::future< detail::invoke_t< _fn_t, _args_t... > > c_thread_pool::enqueue(
		_fn_t&& fn, _args_t&&... args
	) {
		const auto task = std::make_shared< std::packaged_task< detail::invoke_t< _fn_t, _args_t... >( ) > >(
			std::bind( std::forward< _fn_t >( fn ), std::forward< _args_t >( args )... )
		);

		auto ret = task->get_future( );

		{
			const std::unique_lock lock{ m_queue.m_mutex };

			/* @todo impl container for a move only objects */
			m_queue.m_elements.emplace_back( [ = ]( ) { ( *task )( ); } );
		}

		++m_queue.m_tasks_count;

		/* notify that we have a new task */
		m_queue.m_watcher.release( );

		return ret;
	}

	ALWAYS_INLINE void c_thread_pool::wait( ) const {
		for ( ; m_queue.m_tasks_count; )
			std::this_thread::yield( );
	}

	ALWAYS_INLINE c_thread_pool::queue_t& c_thread_pool::queue( ) { return m_queue; }
}