#pragma once

#include "../math.hpp"

namespace math {
	static const float invtwopi = 0.1591549f;
	static const float twopi = 6.283185f;
	static const float threehalfpi = 4.7123889f;
	static const float pi = 3.141593f;
	static const float halfpi = 1.570796f;
	static const __m128 signmask = _mm_castsi128_ps ( _mm_set1_epi32 ( 0x80000000 ) );

	static const __declspec( align( 16 ) ) float null[ 4 ] = { 0.f, 0.f, 0.f, 0.f };
	static const __declspec( align( 16 ) ) float _pi2[ 4 ] = { 1.5707963267948966192f, 1.5707963267948966192f, 1.5707963267948966192f, 1.5707963267948966192f };
	static const __declspec( align( 16 ) ) float _pi[ 4 ] = { 3.141592653589793238f, 3.141592653589793238f, 3.141592653589793238f, 3.141592653589793238f };

	typedef __declspec( align( 16 ) ) union {
		float f[ 4 ];
		__m128 v;
	} m128;

	__forceinline __m128 sqrt_ps ( const __m128 squared )
	{
		return _mm_sqrt_ps ( squared );
	}

	__forceinline __m128 cos_52s_ps ( const __m128 x )
	{
		const auto c1 = _mm_set1_ps ( 0.9999932946f );
		const auto c2 = _mm_set1_ps ( -0.4999124376f );
		const auto c3 = _mm_set1_ps ( 0.0414877472f );
		const auto c4 = _mm_set1_ps ( -0.0012712095f );
		const auto x2 = _mm_mul_ps ( x, x );
		return _mm_add_ps ( c1, _mm_mul_ps ( x2, _mm_add_ps ( c2, _mm_mul_ps ( x2, _mm_add_ps ( c3, _mm_mul_ps ( c4, x2 ) ) ) ) ) );
	}

	__forceinline __m128 cos_ps ( __m128 angle )
	{
		angle = _mm_andnot_ps ( signmask, angle );
		angle = _mm_sub_ps ( angle, _mm_mul_ps ( _mm_cvtepi32_ps ( _mm_cvttps_epi32 ( _mm_mul_ps ( angle, _mm_set1_ps ( invtwopi ) ) ) ), _mm_set1_ps ( twopi ) ) );

		auto cosangle = angle;
		cosangle = _mm_xor_ps ( cosangle, _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( halfpi ) ), _mm_xor_ps ( cosangle, _mm_sub_ps ( _mm_set1_ps ( pi ), angle ) ) ) );
		cosangle = _mm_xor_ps ( cosangle, _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( pi ) ), signmask ) );
		cosangle = _mm_xor_ps ( cosangle, _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( threehalfpi ) ), _mm_xor_ps ( cosangle, _mm_sub_ps ( _mm_set1_ps ( twopi ), angle ) ) ) );

		auto result = cos_52s_ps ( cosangle );
		result = _mm_xor_ps ( result, _mm_and_ps ( _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( halfpi ) ), _mm_cmplt_ps ( angle, _mm_set1_ps ( threehalfpi ) ) ), signmask ) );
		return result;
	}

	__forceinline __m128 sin_ps ( const __m128 angle )
	{
		return cos_ps ( _mm_sub_ps ( _mm_set1_ps ( halfpi ), angle ) );
	}

	__forceinline void sincos_ps ( __m128 angle, __m128* sin, __m128* cos )
	{
		const auto anglesign = _mm_or_ps ( _mm_set1_ps ( 1.f ), _mm_and_ps ( signmask, angle ) );
		angle = _mm_andnot_ps ( signmask, angle );
		angle = _mm_sub_ps ( angle, _mm_mul_ps ( _mm_cvtepi32_ps ( _mm_cvttps_epi32 ( _mm_mul_ps ( angle, _mm_set1_ps ( invtwopi ) ) ) ), _mm_set1_ps ( twopi ) ) );

		auto cosangle = angle;
		cosangle = _mm_xor_ps ( cosangle, _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( halfpi ) ), _mm_xor_ps ( cosangle, _mm_sub_ps ( _mm_set1_ps ( pi ), angle ) ) ) );
		cosangle = _mm_xor_ps ( cosangle, _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( pi ) ), signmask ) );
		cosangle = _mm_xor_ps ( cosangle, _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( threehalfpi ) ), _mm_xor_ps ( cosangle, _mm_sub_ps ( _mm_set1_ps ( twopi ), angle ) ) ) );

		auto result = cos_52s_ps ( cosangle );
		result = _mm_xor_ps ( result, _mm_and_ps ( _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( halfpi ) ), _mm_cmplt_ps ( angle, _mm_set1_ps ( threehalfpi ) ) ), signmask ) );
		*cos = result;

		const auto sinmultiplier = _mm_mul_ps ( anglesign, _mm_or_ps ( _mm_set1_ps ( 1.f ), _mm_and_ps ( _mm_cmpgt_ps ( angle, _mm_set1_ps ( pi ) ), signmask ) ) );
		*sin = _mm_mul_ps ( sinmultiplier, sqrt_ps ( _mm_sub_ps ( _mm_set1_ps ( 1.f ), _mm_mul_ps ( result, result ) ) ) );
	}

	__forceinline void sin_cos( const float rad, float& sin, float& cos ) {
		sin = std::sin( rad );
		cos = std::cos( rad );
	}

	__forceinline float to_deg( const float rad ) { return rad * k_rad_pi; }

	__forceinline float to_rad( const float deg ) { return deg * k_deg_pi; }

	__forceinline float angle_diff( float src, float dst ) {
		return std::remainder(
			std::remainder( dst, 360.f ) - std::remainder( src, 360.f ),
			360.f
		);
	}

	__forceinline void concat_transforms( const mat3x4_t& in0, const mat3x4_t& in1, mat3x4_t& out ) {
		out[ 0 ][ 0 ] = in0[ 0 ][ 0 ] * in1[ 0 ][ 0 ] + in0[ 0 ][ 1 ] * in1[ 1 ][ 0 ] + in0[ 0 ][ 2 ] * in1[ 2 ][ 0 ];
		out[ 0 ][ 1 ] = in0[ 0 ][ 0 ] * in1[ 0 ][ 1 ] + in0[ 0 ][ 1 ] * in1[ 1 ][ 1 ] + in0[ 0 ][ 2 ] * in1[ 2 ][ 1 ];
		out[ 0 ][ 2 ] = in0[ 0 ][ 0 ] * in1[ 0 ][ 2 ] + in0[ 0 ][ 1 ] * in1[ 1 ][ 2 ] + in0[ 0 ][ 2 ] * in1[ 2 ][ 2 ];
		out[ 0 ][ 3 ] = in0[ 0 ][ 0 ] * in1[ 0 ][ 3 ] + in0[ 0 ][ 1 ] * in1[ 1 ][ 3 ] + in0[ 0 ][ 2 ] * in1[ 2 ][ 3 ] + in0[ 0 ][ 3 ];

		out[ 1 ][ 0 ] = in0[ 1 ][ 0 ] * in1[ 0 ][ 0 ] + in0[ 1 ][ 1 ] * in1[ 1 ][ 0 ] + in0[ 1 ][ 2 ] * in1[ 2 ][ 0 ];
		out[ 1 ][ 1 ] = in0[ 1 ][ 0 ] * in1[ 0 ][ 1 ] + in0[ 1 ][ 1 ] * in1[ 1 ][ 1 ] + in0[ 1 ][ 2 ] * in1[ 2 ][ 1 ];
		out[ 1 ][ 2 ] = in0[ 1 ][ 0 ] * in1[ 0 ][ 2 ] + in0[ 1 ][ 1 ] * in1[ 1 ][ 2 ] + in0[ 1 ][ 2 ] * in1[ 2 ][ 2 ];
		out[ 1 ][ 3 ] = in0[ 1 ][ 0 ] * in1[ 0 ][ 3 ] + in0[ 1 ][ 1 ] * in1[ 1 ][ 3 ] + in0[ 1 ][ 2 ] * in1[ 2 ][ 3 ] + in0[ 1 ][ 3 ];

		out[ 2 ][ 0 ] = in0[ 2 ][ 0 ] * in1[ 0 ][ 0 ] + in0[ 2 ][ 1 ] * in1[ 1 ][ 0 ] + in0[ 2 ][ 2 ] * in1[ 2 ][ 0 ];
		out[ 2 ][ 1 ] = in0[ 2 ][ 0 ] * in1[ 0 ][ 1 ] + in0[ 2 ][ 1 ] * in1[ 1 ][ 1 ] + in0[ 2 ][ 2 ] * in1[ 2 ][ 1 ];
		out[ 2 ][ 2 ] = in0[ 2 ][ 0 ] * in1[ 0 ][ 2 ] + in0[ 2 ][ 1 ] * in1[ 1 ][ 2 ] + in0[ 2 ][ 2 ] * in1[ 2 ][ 2 ];
		out[ 2 ][ 3 ] = in0[ 2 ][ 0 ] * in1[ 0 ][ 3 ] + in0[ 2 ][ 1 ] * in1[ 1 ][ 3 ] + in0[ 2 ][ 2 ] * in1[ 2 ][ 3 ] + in0[ 2 ][ 3 ];
	}

	__forceinline void angle_vectors( const qangle_t& angle, vec3_t* const fwd, vec3_t* const right, vec3_t* const up ) {
		float cp{}, sp{}, cy{}, sy{}, cr{}, sr{};

		sin_cos( to_rad( angle.x ), sp, cp );
		sin_cos( to_rad( angle.y ), sy, cy );
		sin_cos( to_rad( angle.z ), sr, cr );

		if ( fwd ) {
			fwd->x = cp * cy;
			fwd->y = cp * sy;
			fwd->z = -sp;
		}

		if ( right ) {
			right->x = -1.f * sr * sp * cy + -1.f * cr * -sy;
			right->y = -1.f * sr * sp * sy + -1.f * cr * cy;
			right->z = -1.f * sr * cp;
		}

		if ( up ) {
			up->x = cr * sp * cy + -sr * -sy;
			up->y = cr * sp * sy + -sr * cy;
			up->z = cr * cp;
		}
	}

	template <typename t>
	__forceinline static t lerp2(float progress, const t& t1, const t& t2)
	{
		return t1 + (t2 - t1) * progress;
	}

	__forceinline float simple_spline_remap_val_clamped(float value, float a, float b, float c, float d)
	{
		if (a == b)
			return value >= b ? d : c;
		float c_val = (value - a) / (b - a);
		c_val = std::clamp(c_val, 0.0f, 1.0f);

		return c + (d - c) * c_val;
	}

	__forceinline void vector_angles( const vec3_t& in, qangle_t& out ) {
		if ( in.x == 0.f
			&& in.y == 0.f ) {
			out.y = 0.f;
			out.x = in.z > 0.f ? 270.f : 90.f;
		}
		else {
			out.y = to_deg( std::atan2( in.y, in.x ) );
			if ( out.y < 0.f )
				out.y += 360.f;

			out.x = to_deg( std::atan2( -in.z, in.length_2d( ) ) );
			if ( out.x < 0.f )
				out.x += 360.f;
		}

		out.x = std::remainder( out.x, 360.f );
		out.y = std::remainder( out.y, 360.f );
		out.z = std::remainder( out.z, 360.f );
	}

	__forceinline float calc_fov( const qangle_t& view_angles, const vec3_t& src, const vec3_t& dst ) {
		const auto dir = ( dst - src ).normalized( );

		vec3_t fwd{};

		angle_vectors( view_angles, &fwd );

		return std::max( to_deg( std::acos( fwd.dot( dir ) ) ), 0.f );
	}

	__forceinline mat3x4_t angle_matrix ( const qangle_t& angles )
	{
		mat3x4_t result;

		m128 angle, sin, cos;
		angle.f[ 0 ] = math::to_rad ( angles.x );
		angle.f[ 1 ] = math::to_rad ( angles.y );
		angle.f[ 2 ] = math::to_rad ( angles.z );
		sincos_ps ( angle.v, &sin.v, &cos.v );

		result[ 0 ][ 0 ] = cos.f[ 0 ] * cos.f[ 1 ];
		result[ 1 ][ 0 ] = cos.f[ 0 ] * sin.f[ 1 ];
		result[ 2 ][ 0 ] = -sin.f[ 0 ];

		const auto crcy = cos.f[ 2 ] * cos.f[ 1 ];
		const auto crsy = cos.f[ 2 ] * sin.f[ 1 ];
		const auto srcy = sin.f[ 2 ] * cos.f[ 1 ];
		const auto srsy = sin.f[ 2 ] * sin.f[ 1 ];

		result[ 0 ][ 1 ] = sin.f[ 0 ] * srcy - crsy;
		result[ 1 ][ 1 ] = sin.f[ 0 ] * srsy + crcy;
		result[ 2 ][ 1 ] = sin.f[ 2 ] * cos.f[ 0 ];

		result[ 0 ][ 2 ] = sin.f[ 0 ] * crcy + srsy;
		result[ 1 ][ 2 ] = sin.f[ 0 ] * crsy - srcy;
		result[ 2 ][ 2 ] = cos.f[ 2 ] * cos.f[ 0 ];

		return result;
	}


	__forceinline void vector_transform( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out ) {
		out = {
			in.dot( { matrix[ 0 ][ 0 ], matrix[ 0 ][ 1 ], matrix[ 0 ][ 2 ] } ) + matrix[ 0 ][ 3 ],
			in.dot( { matrix[ 1 ][ 0 ], matrix[ 1 ][ 1 ], matrix[ 1 ][ 2 ] } ) + matrix[ 1 ][ 3 ],
			in.dot( { matrix[ 2 ][ 0 ], matrix[ 2 ][ 1 ], matrix[ 2 ][ 2 ] } ) + matrix[ 2 ][ 3 ]
		};
	}

	__forceinline void vector_rotate( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out ) {
		out = { in.dot( matrix[ 0 ] ), in.dot( matrix[ 1 ] ), in.dot( matrix[ 2 ] ) };
	}
	__forceinline vec3_t vector_rotate ( const vec3_t& in1, const mat3x4_t& in2 )
	{
		return vec3_t ( in1.dot ( in2[ 0 ] ), in1.dot ( in2[ 1 ] ), in1.dot ( in2[ 2 ] ) );
	}
	__forceinline vec3_t vector_rotate ( const vec3_t& in1, const qangle_t& in2 )
	{
		const auto matrix = angle_matrix ( in2 );
		return vector_rotate ( in1, matrix );
	}

	__forceinline void vector_i_transform( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out ) {
		const vec3_t diff{
			in.x - matrix[ 0 ][ 3 ],
			in.y - matrix[ 1 ][ 3 ],
			in.z - matrix[ 2 ][ 3 ]
		};

		out = {
			diff.x * matrix[ 0 ][ 0 ] + diff.y * matrix[ 1 ][ 0 ] + diff.z * matrix[ 2 ][ 0 ],
			diff.x * matrix[ 0 ][ 1 ] + diff.y * matrix[ 1 ][ 1 ] + diff.z * matrix[ 2 ][ 1 ],
			diff.x * matrix[ 0 ][ 2 ] + diff.y * matrix[ 1 ][ 2 ] + diff.z * matrix[ 2 ][ 2 ]
		};
	}

	__forceinline void vector_i_rotate( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out ) {
		out = {
			in.x * matrix[ 0 ][ 0 ] + in.y * matrix[ 1 ][ 0 ] + in.z * matrix[ 2 ][ 0 ],
			in.x * matrix[ 0 ][ 1 ] + in.y * matrix[ 1 ][ 1 ] + in.z * matrix[ 2 ][ 1 ],
			in.x * matrix[ 0 ][ 2 ] + in.y * matrix[ 1 ][ 2 ] + in.z * matrix[ 2 ][ 2 ]
		};
	}

	__forceinline mat3x4_t vector_matrix( const vec3_t& in ) {
		vec3_t right{}, up{};

		if ( in.x == 0.f
			&& in.y == 0.f ) {
			right = { 0.f, -1.f, 0.f };
			up = { -in.z, 0.f, 0.f };
		}
		else {
			right = in.cross( { 0.f, 0.f, 1.f } ).normalized( );
			up = right.cross( in ).normalized( );
		}

		mat3x4_t ret{};

		ret[ 0 ][ 0 ] = in.x;
		ret[ 1 ][ 0 ] = in.y;
		ret[ 2 ][ 0 ] = in.z;

		ret[ 0 ][ 1 ] = -right.x;
		ret[ 1 ][ 1 ] = -right.y;
		ret[ 2 ][ 1 ] = -right.z;

		ret[ 0 ][ 2 ] = up.x;
		ret[ 1 ][ 2 ] = up.y;
		ret[ 2 ][ 2 ] = up.z;

		return ret;
	}
	__forceinline float normalize_yaw ( float f )
	{
		while ( f < -180.0f )
			f += 360.0f;

		while ( f > 180.0f )
			f -= 360.0f;

		return f;
	}

	__forceinline vec3_t normalize_angle ( vec3_t vec ) {

		while ( vec.y < -180.f )
			vec.y += 360.f;

		while ( vec.y > 180.f )
			vec.y -= 360.f;

		if ( vec.x > 89.f )
			vec.x = 89.f;
		
		if ( vec.x < -89.f )
			vec.x = -89.f;

		vec.z = std::clamp ( vec.z, -50.f, 50.f );

		return vec;
	}

	__forceinline float segment_to_segment( const vec3_t& s1, const vec3_t& s2, const vec3_t& k1, const vec3_t& k2 ) {
		const auto u = s2 - s1, v = k2 - k1, w = s1 - k1;

		const auto a = u.dot( u );
		const auto b = u.dot( v );
		const auto c = v.dot( v );
		const auto d = u.dot( w );
		const auto e = v.dot( w );
		const auto unk = a * c - b * b;

		auto sn = 0.f, tn = 0.f, sd = unk, td = unk;

		if ( unk < 0.00000001f ) {
			sn = 0.f;
			sd = 1.f;
			tn = e;
			td = c;
		}
		else {
			sn = b * e - c * d;
			tn = a * e - b * d;

			if ( sn < 0.f ) {
				sn = 0.f;
				tn = e;
				td = c;
			}
			else if ( sn > sd ) {
				sn = sd;
				tn = e + b;
				td = c;
			}
		}

		if ( tn < 0.f ) {
			tn = 0.f;

			if ( -d < 0.f ) {
				sn = 0.f;
			}
			else if ( -d > a ) {
				sn = sd;
			}
			else {
				sn = -d;
				sd = a;
			}
		}
		else if ( tn > td ) {
			tn = td;

			if ( -d + b < 0.f ) {
				sn = 0.f;
			}
			else if ( -d + b > a ) {
				sn = sd;
			}
			else {
				sn = -d + b;
				sd = a;
			}
		}

		const auto sc = std::abs( sn ) < 0.00000001f ? 0.f : sn / sd;
		const auto tc = std::abs( tn ) < 0.00000001f ? 0.f : tn / td;

		return ( w + u * sc - v * tc ).length( );
	}


	__forceinline bool intersect_bb ( vec3_t& start, vec3_t& delta, vec3_t& min, vec3_t& max )
	{
		float d1, d2, f;
		auto start_solid = true;
		auto t1 = -1.0, t2 = 1.0;

		const float _start[ 3 ] = { start.x, start.y, start.z };
		const float _delta[ 3 ] = { delta.x, delta.y, delta.z };
		const float mins[ 3 ] = { min.x, min.y, min.z };
		const float maxs[ 3 ] = { max.x, max.y, max.z };

		for ( auto i = 0; i < 6; ++i ) {
			if ( i >= 3 ) {
				const auto j = ( i - 3 );

				d1 = _start[ j ] - maxs[ j ];
				d2 = d1 + _delta[ j ];
			}
			else {
				d1 = -_start[ i ] + mins[ i ];
				d2 = d1 - _delta[ i ];
			}

			if ( d1 > 0 && d2 > 0 ) {
				start_solid = false;
				return false;
			}

			if ( d1 <= 0 && d2 <= 0 )
				continue;

			if ( d1 > 0 )
				start_solid = false;

			if ( d1 > d2 ) {
				f = d1;
				if ( f < 0 )
					f = 0;

				f /= d1 - d2;
				if ( f > t1 )
					t1 = f;
			}
			else {
				f = d1 / ( d1 - d2 );
				if ( f < t2 )
					t2 = f;
			}
		}

		return start_solid || ( t1 < t2&& t1 >= 0.0f );
	}

	__forceinline bool intersect ( vec3_t start, vec3_t end, vec3_t a, vec3_t b, float radius )
	{
		const auto dist = segment_to_segment ( start, end, a, b );
		return ( dist < radius );
	}


	__forceinline bool line_vs_bb( const vec3_t& src, const vec3_t& dst, const vec3_t& min, const vec3_t& max ) {
		float d1{}, d2{}, f{};
		auto t1 = -1.f, t2 = 1.f;

		auto start_solid = true;

		for ( std::size_t i{}; i < 6u; ++i ) {
			if ( i >= 3 ) {
				const auto j = i - 3u;

				d1 = src[ j ] - max[ j ];
				d2 = d1 + dst[ j ];
			}
			else {
				d1 = -src[ i ] + min[ i ];
				d2 = d1 - dst[ i ];
			}

			if ( d1 > 0.0f
				&& d2 > 0.0f )
				return false;

			if ( d1 <= 0.0f
				&& d2 <= 0.0f )
				continue;

			if ( d1 > 0.f )
				start_solid = false;

			if ( d1 > d2 ) {
				f = d1;

				if ( f < 0.f )
					f = 0.f;

				f /= d1 - d2;

				if ( f > t1 )
					t1 = f;
			}
			else {
				f = d1 / ( d1 - d2 );

				if ( f < t2 )
					t2 = f;
			}
		}

		return start_solid || ( t1 < t2 && t1 >= 0.f );
	}

	__forceinline bool to_screen( const vec3_t& world, const vec2_t& screen_size, const mat4x4_t& matrix, vec2_t& out ) {
		out.x = matrix[ 0 ][ 0 ] * world.x + matrix[ 0 ][ 1 ] * world.y + matrix[ 0 ][ 2 ] * world.z + matrix[ 0 ][ 3 ];
		out.y = matrix[ 1 ][ 0 ] * world.x + matrix[ 1 ][ 1 ] * world.y + matrix[ 1 ][ 2 ] * world.z + matrix[ 1 ][ 3 ];

		const auto w = matrix[ 3 ][ 0 ] * world.x + matrix[ 3 ][ 1 ] * world.y + matrix[ 3 ][ 2 ] * world.z + matrix[ 3 ][ 3 ];
		if ( w < 0.001f ) {
			out *= 100000.f;

			return false;
		}

		out /= w;

		out.x = ( screen_size.x / 2.f ) + ( out.x * screen_size.x ) / 2.f;
		out.y = ( screen_size.y / 2.f ) - ( out.y * screen_size.y ) / 2.f;

		return true;
	}
}