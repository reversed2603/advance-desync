#pragma once
#include <array>

template<typename T, T value>
struct constant_holder_t_h {
    enum class e_value_holder_h : T {
        m_value = value
    };
};

#define CONSTANT_h(value) (static_cast<decltype(value)>(constant_holder_t_h<decltype(value), value>::e_value_holder_h::m_value))

namespace hash_h {
    namespace fnv1a_h {
        constexpr auto m_seed = 0x45C3370D;
        constexpr auto m_prime = 0x1000193;

        __forceinline uint32_t rt( const char* txt ) {
            auto hash = m_seed;

            for ( auto i = 0u; i < strlen( txt ); i++ ) {
                hash ^= txt[i];
                hash *= m_prime;
            }

            return ~hash;
        }

        constexpr uint32_t ct( const char* txt, uint32_t value = m_seed ) noexcept {
            return !*txt ? value : ct( txt + 1, static_cast< unsigned >( 1ull * ( value ^ static_cast< uint8_t >( *txt ) ) * m_prime ) );
        }
    }
}

#define _h(txt) ~CONSTANT_h(hash_h::fnv1a_h::ct(txt))
#define _h_RT(txt) hash_h::fnv1a_h::rt(txt)
