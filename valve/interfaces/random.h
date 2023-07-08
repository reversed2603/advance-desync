#pragma once

#include <cstdint>

// all numbers are generated randomly at compile time. the internal state is pseudo
// remembered using the counter macro. the seed is based on time using the timestamp
// and time macro. additionally a custom random seed can be specified to fully rely

#ifndef DYNLEC_CUSTOM_RANDOM_SEED
#define DYNLEC_CUSTOM_RANDOM_SEED 0xbdacf99b3f7a1bb4ULL
#endif

// just iterating over the macros will always result in same
// number because the internal state is only updated for each occurance
// of the following macros

// generates a random number seeded with time and the custom seed
#define DYC_RAND_NEXT (::Dynlec::CTRandomGeneratorValueSeeded<__COUNTER__>)
// generates a random number seeded with time and the custom seed between min and max ( [min, max[ )
#define DYC_RAND_NEXT_BETWEEN(min, max) (min + (::Dynlec::CTRandomGeneratorValueSeeded<__COUNTER__> % (max - min)))
// generates a random number seeded with time and the custom seed with a limit ( [0, limit[ )
#define DYC_RAND_NEXT_LIMIT(limit) DYC_RAND_NEXT_BETWEEN(0, limit)
// helper macro for non repetetive indexed values
#define DYC_RAND_INDEXED(index) (::Dynlec::CTRandomGeneratorValue<index, ::Dynlec::CTRandomSeed ^ DYC_RAND_NEXT>)
// helper macro for non repetetive random streams
#define DYC_RAND_STREAM(n, callback) (::Dynlec::CTRandomStream<n, ::Dynlec::CTRandomSeed ^ DYC_RAND_NEXT>)

namespace Dynlec
{
    // the random generator internal state is represented by
    // the CTRandomGeneratorRaw type with each of its values
    // x, y, z and c
    template <
        uint64_t x,
        uint64_t y,
        uint64_t z,
        uint64_t c>
        class CTRandomGeneratorRaw
    {
        static_assert( y != 0,
            "CompileTimeRandom can not be used with 'y' equals 0" );
        static_assert( z != 0 || c != 0,
            "CompileTimeRandom can not be used with 'z' and 'c' equals 0" );
    public:
        typedef CTRandomGeneratorRaw<
            6906969069ULL * x + 1234567ULL,
            ( ( y ^ ( y << 13 ) ) ^ ( ( y ^ ( y << 13 ) ) >> 17 ) ) ^ ( ( ( y ^ ( y << 13 ) ) ^ ( ( y ^ ( y << 13 ) ) >> 17 ) ) << 43 ),
            z + ( ( z << 58 ) + c ),
            ( ( z + ( ( z << 58 ) + c ) ) >> 6 ) + ( z + ( ( z << 58 ) + c ) < ( ( z << 58 ) + c ) )> Next;

        constexpr static uint64_t Value = x + y + z;
    };

    // to prevent any accidental selection of invalid parameters
    // these values are omitted
    template <
        uint64_t x,
        uint64_t y,
        uint64_t z,
        uint64_t c>
        class CTRandomGeneratorRawSafe
        :
        public CTRandomGeneratorRaw<
        x, ( y == 0 ) ? 1 : y, ( z == 0 && c == 0 ) ? 1 : z, c>
    {
    };

    // CTRandomGenerator is used to quickly compute the nth iteration
    // of CTRandomGeneratorSafeRaw based on a single uint64_t seed
    template <uint64_t iterations, uint64_t seed>
    class CTRandomGenerator
    {
        friend CTRandomGenerator<iterations + 1, seed>;
        typedef typename CTRandomGenerator<iterations - 1, seed>::Current::Next Current;

    public:
        constexpr static uint64_t Value = Current::Value;
    };

    template <uint64_t seed>
    class CTRandomGenerator<0ULL, seed>
    {
        friend CTRandomGenerator<1ULL, seed>;

        typedef typename CTRandomGeneratorRawSafe<
            seed ^ 1066149217761810ULL,
            seed ^ 362436362436362436ULL,
            seed ^ 1234567890987654321ULL,
            seed ^ 123456123456123456ULL>::Next Current;

    public:
        constexpr static uint64_t Value = Current::Value;
    };

    template <uint64_t iteration, uint64_t seed>
    constexpr static uint64_t CTRandomGeneratorValue = CTRandomGenerator<iteration, seed>::Value;

    const uint64_t CTRandomTimeSeed =
        CTRandomGeneratorValue<0, ( __TIME__[0] ) ^
        CTRandomGeneratorValue<0, ( __TIME__[1] ) ^
        CTRandomGeneratorValue<0, ( __TIME__[3] ) ^
        CTRandomGeneratorValue<0, ( __TIME__[4] ) ^
        CTRandomGeneratorValue<0, ( __TIME__[6] ) ^
        CTRandomGeneratorValue<0, ( __TIME__[7] )>>>>>> ^
        CTRandomGeneratorValue<0, ( __TIMESTAMP__[0] ) ^
        CTRandomGeneratorValue<0, ( __TIMESTAMP__[1] ) ^
        CTRandomGeneratorValue<0, ( __TIMESTAMP__[2] ) ^
        CTRandomGeneratorValue<0, ( __TIMESTAMP__[4] ) ^
        CTRandomGeneratorValue<0, ( __TIMESTAMP__[5] ) ^
        CTRandomGeneratorValue<0, ( __TIMESTAMP__[6] )>>>>>> ^
        CTRandomGeneratorValue<0, ( __TIMESTAMP__[8] ) ^
        CTRandomGeneratorValue<0, ( __TIMESTAMP__[9] ) ^
        CTRandomGeneratorValue<0, ( __TIMESTAMP__[20] ) ^
        CTRandomGeneratorValue<0, ( __TIMESTAMP__[21] ) ^
        CTRandomGeneratorValue<0, ( __TIMESTAMP__[22] ) ^
        CTRandomGeneratorValue<0, ( __TIMESTAMP__[23] )>>>>>>;

    const uint64_t CTRandomSeed = ( DYNLEC_CUSTOM_RANDOM_SEED ^ CTRandomTimeSeed );

    template <uint64_t iteration>
    constexpr static uint64_t CTRandomGeneratorValueSeeded = CTRandomGeneratorValue<iteration, CTRandomSeed>;

    template <uint64_t n, uint64_t seed = ::Dynlec::CTRandomSeed>
    struct CTRandomStream
    {
        // callback(uint64_t index [0;n[, uint64_t random_number)
        template <typename T>
        static void Call( T callback )
        {
            CTRandomStream<n - 1, seed>::Call( callback );
            callback( n - 1, CTRandomGeneratorValue<n, seed> );
        }
    };

    template <uint64_t seed>
    struct CTRandomStream<0, seed>
    {
        template <typename T>
        static void Call( T callback ) { }
    };
}


__forceinline const int crypt_int_ ( const int i, const int rand_val )
{
    int crypted = __builtin_bswap32 ( _rotl ( i, rand_val % 31 ) - rand_val );

    volatile int decrypted = crypted;
    decrypted = __builtin_bswap32 ( decrypted );
    decrypted += rand_val;
    decrypted = _rotr ( decrypted, rand_val % 31 );

    return decrypted;
}

#define crypt_int(val) crypt_int_( val, DYC_RAND_NEXT )

__forceinline const float crypt_float_ ( const float f )
{
    auto decrypted_int = crypt_int ( ( int ) ( f * 10000.f ) );

    return ( ( float ) decrypted_int ) / ( float ) crypt_int ( 10000 );
}

#define crypt_float(val) crypt_float_( val )

__forceinline const double crypt_double_ ( const double f )
{
    auto decrypted_int = crypt_int ( ( int ) ( f * 10000.0 ) );

    return ( ( double ) decrypted_int ) / ( double ) crypt_int ( 10000 );
}

#define crypt_double(val) crypt_double_( val )

template < typename T >
class c_crypt_container
{
private:
    volatile unsigned long m_crypted = 0;
    volatile unsigned long m_rand_val = 0;
    volatile unsigned long m_rol_val = 0;

public:
    __forceinline c_crypt_container ( T val )
    {
        set ( val );
    }

    __forceinline void set ( T val )
    {
        m_rand_val = ( unsigned long ) ( this );
        m_rol_val = ( unsigned long ) ( this ) % 31;
        m_crypted = __builtin_bswap32 ( _rotl ( ( unsigned long ) val, ( unsigned long ) ( this ) % 31 ) - ( unsigned long ) ( this ) );
    }

    __forceinline T get ( )
    {
        return ( T ) ( _rotr ( __builtin_bswap32 ( m_crypted ) + m_rand_val, m_rol_val ) );
    }
};

#define cc_init(val) c_crypt_container < decltype(val) > ( val )
#define cc_def(type) c_crypt_container < type >