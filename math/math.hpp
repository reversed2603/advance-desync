#pragma once

using vec2_t = ImVec2;

using vec3_t = ImVec3;

using qangle_t = ImVec3;

struct mat3x4_t {
	__forceinline float* operator []( const int i ) { return m_matrix[ i ]; }

	__forceinline const float* operator []( const int i ) const { return m_matrix[ i ]; }

	vec3_t get_origin ( ) const { return at ( 3 ); }

	vec3_t at ( int i ) const { return vec3_t { m_matrix[ 0 ][ i ], m_matrix[ 1 ][ i ], m_matrix[ 2 ][ i ] }; }
	float m_matrix[ 3u ][ 4u ]{};

    inline void set_origin ( vec3_t const& p )
	{
		m_matrix[ 0 ][ 3 ] = p.x;
		m_matrix[ 1 ][ 3 ] = p.y;
		m_matrix[ 2 ][ 3 ] = p.z;
	}
};

template <typename T>
struct bit_flag_t {
	bit_flag_t ( ) = default;
	bit_flag_t ( const T& value ) { m_value = value; }

	__forceinline bool has ( const T& value ) const { return m_value & value; }

	__forceinline void add ( const T& value ) { m_value |= value; }

	__forceinline void remove ( const T& value ) { m_value &= ~value; }

	__forceinline void clear ( ) { m_value = {}; }

	__forceinline bool empty ( ) const { return m_value == std::numeric_limits<T>::quiet_NaN ( ); }

	__forceinline operator T( ) const { return m_value; }

	__forceinline bit_flag_t<T>& operator=( const bit_flag_t<T>& value )
	{
		m_value = value.m_value;

		return *this;
	}

	__forceinline T& operator=( const T& value )
	{
		m_value = value;

		return m_value;
	}

	T m_value = {};
};

struct vec4_t : public vec3_t {
	vec4_t ( ) = default;
	vec4_t ( float ix, float iy, float iz, float iw )
	{
		x = ix;
		y = iy;
		z = iz;
		w = iw;
	}
	vec4_t ( const vec3_t& value )
	{
		x = value.x;
		y = value.y;
		z = value.z;
		w = 0.f;
	}
	vec4_t ( const vec2_t& value0, const vec2_t& value1 )
	{
		x = value0.x;
		y = value0.y;
		z = value1.x;
		w = value1.y;
	}

	__forceinline vec4_t& operator=( const vec3_t& value )
	{
		x = value.x;
		y = value.y;
		z = value.z;
		w = 0.f;

		return *this;
	}

	__forceinline vec4_t& operator=( const vec4_t& value )
	{
		x = value.x;
		y = value.y;
		z = value.z;
		w = value.w;

		return *this;
	}

	float w = 0.f;
};

struct col_t {
	col_t ( ) = default;
	col_t ( int r, int g, int b ) { set ( r, g, b, 255 ); }
	col_t ( int r, int g, int b, int a ) { set ( r, g, b, a ); }
	col_t ( const col_t& col, int a ) { set ( col.r ( ), col.g ( ), col.b ( ), a ); }

	std::array<uint8_t, 4> m_value = {};

	__forceinline void set ( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) { m_value = { r, g, b, a }; }

	__forceinline int r ( ) const { return m_value.at ( 0u ); }

	__forceinline int g ( ) const { return m_value.at ( 1u ); }

	__forceinline int b ( ) const { return m_value.at ( 2u ); }

	__forceinline int a ( ) const { return m_value.at ( 3u ); }

	__forceinline uint32_t hex ( bool rgba = false ) const
	{
		return rgba
			? ( ( r ( ) & 0xFF ) << 24 ) + ( ( g ( ) & 0xFF ) << 16 ) + ( ( b ( ) & 0xFF ) << 8 ) + ( a ( ) & 0xFF )
			: ( ( a ( ) & 0xFF ) << 24 ) + ( ( b ( ) & 0xFF ) << 16 ) + ( ( g ( ) & 0xFF ) << 8 ) + ( r ( ) & 0xFF );
	}

	__forceinline col_t alpha ( int value ) const { return col_t ( r ( ), g ( ), b ( ), value ); }

	__forceinline col_t& operator=( const col_t& value )
	{
		set ( value.r ( ), value.g ( ), value.b ( ), value.a ( ) );

		return *this;
	}

	__forceinline col_t& operator-=( uint8_t value )
	{
		set ( r ( ) - value, g ( ) - value, b ( ) - value, a ( ) - value );

		return *this;
	}

	__forceinline col_t& operator+=( uint8_t value )
	{
		set ( r ( ) + value, g ( ) + value, b ( ) + value, a ( ) + value );

		return *this;
	}

	__forceinline col_t& operator/=( uint8_t value )
	{
		set ( r ( ) / value, g ( ) / value, b ( ) / value, a ( ) / value );

		return *this;
	}

	__forceinline col_t& operator*=( uint8_t value )
	{
		set ( r ( ) * value, g ( ) * value, b ( ) * value, a ( ) * value );

		return *this;
	}

	__forceinline col_t& operator-=( const col_t& value )
	{
		set ( r ( ) - value.r ( ), g ( ) - value.g ( ), b ( ) - value.b ( ), a ( ) - value.a ( ) );

		return *this;
	}

	__forceinline col_t& operator+=( const col_t& value )
	{
		set ( r ( ) + value.r ( ), g ( ) + value.g ( ), b ( ) + value.b ( ), a ( ) + value.a ( ) );

		return *this;
	}

	__forceinline col_t operator-( uint8_t value ) const { return col_t ( r ( ) - value, g ( ) - value, b ( ) - value, a ( ) - value ); }

	__forceinline col_t operator+( uint8_t value ) const { return col_t ( r ( ) + value, g ( ) + value, b ( ) + value, a ( ) + value ); }

	__forceinline col_t operator/( uint8_t value ) const { return col_t ( r ( ) / value, g ( ) / value, b ( ) / value, a ( ) / value ); }

	__forceinline col_t operator*( uint8_t value ) const { return col_t ( r ( ) * value, g ( ) * value, b ( ) * value, a ( ) * value ); }

	__forceinline col_t operator-( const col_t& value ) const { return col_t ( r ( ) - value.r ( ), g ( ) - value.g ( ), b ( ) - value.b ( ), a ( ) - value.a ( ) ); }

	__forceinline col_t operator+( const col_t& value ) const { return col_t ( r ( ) + value.r ( ), g ( ) + value.g ( ), b ( ) + value.b ( ), a ( ) + value.a ( ) ); }

	__forceinline bool operator==( const col_t& value ) const { return *const_cast< col_t* >( this ) == *const_cast< col_t* >( &value ); }

	__forceinline bool operator!=( const col_t& value ) const { return !( operator==( value ) ); }

	__forceinline float hue ( ) const
	{
		const auto red = r ( ) / 255.f;
		const auto green = g ( ) / 255.f;
		const auto blue = b ( ) / 255.f;

		const auto max = std::max<float> ( { red, green, blue } );
		const auto min = std::min<float> ( { red, green, blue } );

		if ( max == min )
			return 0.f;

		const auto delta = max - min;

		auto hue = 0.f;

		if ( max == red ) {
			hue = ( green - blue ) / delta;
		}
		else if ( max == green ) {
			hue = 2.f + ( blue - red ) / delta;
		}
		else {
			hue = 4.f + ( red - green ) / delta;
		}

		hue *= 60.f;

		if ( hue < 0.f ) {
			hue += 360.f;
		}

		return hue / 360.f;
	}

	__forceinline float saturation ( ) const
	{
		const auto red = r ( ) / 255.f;
		const auto green = g ( ) / 255.f;
		const auto blue = b ( ) / 255.f;

		const auto max = std::max<float> ( { red, green, blue } );
		const auto min = std::min<float> ( { red, green, blue } );

		const auto delta = max - min;

		if ( max == 0.f )
			return delta;

		return delta / max;
	}

	__forceinline float brightness ( ) const { return std::max<float> ( { r ( ) / 255.f, g ( ) / 255.f, b ( ) / 255.f } ); }

	__forceinline static col_t from_hsb ( float hue, float saturation, float brightness )
	{
		const auto h = hue == 1.f ? 0 : hue * 6.f;
		const auto f = h - static_cast< int >( h );
		const auto p = brightness * ( 1.f - saturation );
		const auto q = brightness * ( 1.f - saturation * f );
		const auto t = brightness * ( 1.f - ( saturation * ( 1.f - f ) ) );

		if ( h < 1.f ) {
			return col_t (
				static_cast< uint8_t >( brightness * 255.f ),
				static_cast< uint8_t >( t * 255.f ),
				static_cast< uint8_t >( p * 255.f )
			);
		}

		if ( h < 2.f ) {
			return col_t (
				static_cast< uint8_t >( q * 255 ),
				static_cast< uint8_t >( brightness * 255 ),
				static_cast< uint8_t >( p * 255 )
			);
		}

		if ( h < 3.f ) {
			return col_t (
				static_cast< uint8_t >( p * 255.f ),
				static_cast< uint8_t >( brightness * 255.f ),
				static_cast< uint8_t >( t * 255.f )
			);
		}

		if ( h < 4.f ) {
			return col_t (
				static_cast< uint8_t >( p * 255.f ),
				static_cast< uint8_t >( q * 255.f ),
				static_cast< uint8_t >( brightness * 255.f )
			);
		}

		if ( h < 5.f ) {
			return col_t (
				static_cast< uint8_t >( t * 255.f ),
				static_cast< uint8_t >( p * 255.f ),
				static_cast< uint8_t >( brightness * 255.f )
			);
		}

		return col_t (
			static_cast< uint8_t >( brightness * 255.f ),
			static_cast< uint8_t >( p * 255.f ),
			static_cast< uint8_t >( q * 255.f )
		);
	}

	struct palette_t {
		static col_t red ( int alpha = 255 ) { return col_t ( 255, 0, 0, alpha ); }
		static col_t green ( int alpha = 255 ) { return col_t ( 0, 255, 0, alpha ); }
		static col_t blue ( int alpha = 255 ) { return col_t ( 0, 0, 255, alpha ); }

		static col_t white ( int alpha = 255 ) { return col_t ( 255, 255, 255, alpha ); }
		static col_t black ( int alpha = 255 ) { return col_t ( 13, 13, 13, alpha ); }
		static col_t grey ( int alpha = 255 ) { return col_t ( 60, 60, 60, alpha ); }
		static col_t orange ( int alpha = 255 ) { return col_t ( 245, 171, 53, alpha ); }

		static col_t light_black ( int alpha = 255 ) { return col_t ( 32, 32, 32, alpha ); }
		static col_t light_grey ( int alpha = 255 ) { return col_t ( 77, 77, 77, alpha ); }
		static col_t light_green ( int alpha = 255 ) { return col_t ( 127, 255, 0, alpha ); }
		static col_t light_red ( int alpha = 255 ) { return col_t ( 236, 98, 95, alpha ); }
		static col_t light_blue ( int alpha = 255 ) { return col_t ( 42, 87, 233, alpha ); }
		static col_t light_violet ( int alpha = 255 ) { return col_t ( 153, 153, 255, alpha ); }
		static col_t light_purple ( int alpha = 255 ) { return col_t ( 127, 73, 208, alpha ); }

		static col_t dark_grey ( int alpha = 255 ) { return col_t ( 44, 44, 44, alpha ); }
		static col_t dark_blue ( int alpha = 255 ) { return col_t ( 14, 18, 134, alpha ); }

		static col_t purple ( int alpha = 255 ) { return col_t ( 220, 0, 220, alpha ); }
		static col_t pink ( int alpha = 255 ) { return col_t ( 255, 105, 180, alpha ); }
	};
};


struct mat4x4_t {
	__forceinline float* operator []( const int i ) { return m_matrix[ i ]; }

	__forceinline const float* operator []( const int i ) const { return m_matrix[ i ]; }

	float m_matrix[ 4u ][ 4u ]{};
};

struct v_matrix {
	__forceinline vec4_t& operator[]( int i ) { return m_value.at ( i ); }

	__forceinline const vec4_t& operator[]( int i ) const { return m_value.at ( i ); }

	std::array<vec4_t, 4u> m_value = {};
};


namespace math {
	constexpr auto k_pi = 3.14159265358979323846f;

	constexpr auto k_pi2 = k_pi * 2.f;

	constexpr auto k_rad_pi = 180.f / k_pi;

	constexpr auto k_deg_pi = k_pi / 180.f;

	__forceinline void sin_cos( const float rad, float& sin, float& cos );

	__forceinline float to_deg( const float rad );

	__forceinline float to_rad( const float deg );

	__forceinline float angle_diff( float src, float dst );

	__forceinline void concat_transforms( const mat3x4_t& in0, const mat3x4_t& in1, mat3x4_t& out );

	__forceinline void angle_vectors(
		const qangle_t& angle, vec3_t* const fwd, vec3_t* const right = nullptr, vec3_t* const up = nullptr
	);

	__forceinline vec3_t calc_ang ( const vec3_t& src, const vec3_t& dst )
	{
		vec3_t angles;

		vec3_t delta = src - dst;
		float hyp = delta.length_2d ( );

		angles.y = std::atanf ( delta.y / delta.x ) * k_rad_pi;
		angles.x = std::atanf ( -delta.z / hyp ) * -k_rad_pi;
		angles.z = 0.0f;

		if ( delta.x >= 0.0f )
			angles.y += 180.0f;

		return angles;
	}

	__forceinline void vector_angles( const vec3_t& in, qangle_t& out );

	__forceinline float calc_fov( const qangle_t& view_angles, const vec3_t& src, const vec3_t& dst );

	__forceinline void vector_transform( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out );

	__forceinline void vector_rotate( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out );

	__forceinline void vector_i_transform( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out );

	__forceinline void vector_i_rotate( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out );

	__forceinline mat3x4_t vector_matrix( const vec3_t& in );

	__forceinline float segment_to_segment( const vec3_t& s1, const vec3_t& s2, const vec3_t& k1, const vec3_t& k2 );

	__forceinline bool line_vs_bb( const vec3_t& src, const vec3_t& dst, const vec3_t& min, const vec3_t& max );

	__forceinline bool to_screen( const vec3_t& world, const vec2_t& screen_size, const mat4x4_t& matrix, vec2_t& out );
}

#include "impl/math.inl"