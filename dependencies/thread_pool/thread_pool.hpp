

#pragma once

#include "detail/detail.hpp"

namespace sdk {
	class c_thread_pool {
	private:
		struct queue_t {
			std::mutex								m_mutex{};
			std::counting_semaphore<>				m_watcher{ 0u };

			std::deque< std::function< void( ) > >	m_elements{};
			std::atomic< std::size_t >				m_tasks_count{};
		}						m_queue{};

		struct thread_t {
			ALWAYS_INLINE thread_t( )
				: m_handle{ work_loop } {}

			static void work_loop( std::stop_token token );

			std::jthread m_handle{};
		};

		std::vector< thread_t >		m_threads{ std::jthread::hardware_concurrency( ) };
	public:
		virtual ~c_thread_pool( ) = default;

		ALWAYS_INLINE void wait( ) const;

		/* works like std::async... */
		template < typename _fn_t, typename... _args_t > requires std::is_invocable_v< _fn_t, _args_t... >
		ALWAYS_INLINE std::future< detail::invoke_t< _fn_t, _args_t... > > enqueue( _fn_t&& fn, _args_t&&... args );

		ALWAYS_INLINE queue_t& queue( );
	};

	inline const auto g_thread_pool = std::make_unique< c_thread_pool >( );
}

#include "impl/thread_pool.inl"