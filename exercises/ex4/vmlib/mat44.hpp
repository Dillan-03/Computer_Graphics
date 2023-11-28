#ifndef MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA
#define MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA

#include <cmath>
#include <cassert>
#include <cstdlib>

#include "vec3.hpp"
#include "vec4.hpp"

/** Mat44f: 4x4 matrix with floats
 *
 * See vec2f.hpp for discussion. Similar to the implementation, the Mat44f is
 * intentionally kept simple and somewhat bare bones.
 *
 * The matrix is stored in row-major order (careful when passing it to OpenGL).
 *
 * The overloaded operator () allows access to individual elements. Example:
 *    Mat44f m = ...;
 *    float m12 = m(1,2);
 *    m(0,3) = 3.f;
 *
 * The matrix is arranged as:
 *
 *   ⎛ 0,0  0,1  0,2  0,3 ⎞
 *   ⎜ 1,0  1,1  1,2  1,3 ⎟
 *   ⎜ 2,0  2,1  2,2  2,3 ⎟
 *   ⎝ 3,0  3,1  3,2  3,3 ⎠
 */
struct Mat44f
{
	float v[16];

	constexpr
	float& operator() (std::size_t aI, std::size_t aJ) noexcept
	{
		assert( aI < 4 && aJ < 4 );
		return v[aI*4 + aJ];
	}
	constexpr
	float const& operator() (std::size_t aI, std::size_t aJ) const noexcept
	{
		assert( aI < 4 && aJ < 4 );
		return v[aI*4 + aJ];
	}
};

// Identity matrix
constexpr Mat44f kIdentity44f = { {
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
} };

// Common operators for Mat44f.
// Note that you will need to implement these yourself.

constexpr
Mat44f operator*( Mat44f const& aLeft, Mat44f const& aRight ) noexcept
{
	// Matrix Multiplication 
	Mat44f result = kIdentity44f;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result(i,j) = 0;
            for (int k = 0; k < 4; k++) {
                result(i, j) += aLeft(i, k) * aRight(k, j);
            }
        }
    }

	// (void)aLeft;   // Avoid warnings about unused arguments until the function
	// (void)aRight;  // is properly implemented.
	return result;
}

constexpr
Vec4f operator*( Mat44f const& aLeft, Vec4f const& aRight ) noexcept
{
	//TODO: your implementation goes here
	//TODO: remove the following when you start your implementation
    Vec4f result = {};

    for (int i = 0; i < 4; ++i) {
        result[i] = aLeft(i, 0) * aRight.x +
                    aLeft(i, 1) * aRight.y +
                    aLeft(i, 2) * aRight.z +
                    aLeft(i, 3) * aRight.w;
    }

	// (void)aLeft;   // Avoid warnings about unused arguments until the function
	// (void)aRight;  // is properly implemented.
	return result;
}

// Functions:

inline
Mat44f make_rotation_x( float aAngle ) noexcept
{
	//TODO: your implementation goes here
	//TODO: remove the following when you start your implementation
	// (void)aAngle; // Avoid warnings about unused arguments until the function
	              // is properly implemented.
    
    Mat44f result = kIdentity44f;
    float c = std::cos(aAngle);
    float s = std::sin(aAngle);

    result(1, 1) = c;
    result(1, 2) = -s;
    result(2, 1) = s;
    result(2, 2) = c;

    return result;
}

inline Mat44f make_rotation_y( float aAngle ) noexcept
{
	//TODO: your implementation goes here
	//TODO: remove the following when you start your implementation
	
	// (void)aAngle; // Avoid warnings about unused arguments until the function
	              // is properly implemented.

     Mat44f result = kIdentity44f;
    float c = std::cos(aAngle);
    float s = std::sin(aAngle);

    result(0, 0) = c;
    result(0, 2) = s;
    result(2, 0) = -s;
    result(2, 2) = c;

    return result;



	
}

inline
Mat44f make_rotation_z( float aAngle ) noexcept
{
	//TODO: your implementation goes here
	//TODO: remove the following when you start your implementation
	Mat44f result = kIdentity44f;
    float c = std::cos(aAngle);
    float s = std::sin(aAngle);

    result(0, 0) = c;
    result(0, 1) = -s;
    result(1, 0) = s;
    result(1, 1) = c;

    return result;

}

inline
Mat44f make_translation( Vec3f aTranslation ) noexcept
{
	//TODO: your implementation goes here
	//TODO: remove the following when you start your implementation
	// (void)aTranslation; // Avoid warnings about unused arguments until the function
	//                     // is properly implemented.
	// return kIdentity44f;  
	Mat44f result = kIdentity44f;

    result(0, 3) = aTranslation.x;
    result(1, 3) = aTranslation.y;
    result(2, 3) = aTranslation.z;

    return result;
}

inline
Mat44f make_perspective_projection( float aFovInRadians, float aAspect, float aNear, float aFar ) noexcept
{
	Mat44f result = kIdentity44f;
    const float tanHalfFOV = std::tan(aFovInRadians / 2.0f);

    result(0, 0) = 1.0f / (aAspect * tanHalfFOV);
    result(1, 1) = 1.0f / tanHalfFOV;
    result(2, 2) = -(aFar + aNear) / (aFar - aNear);
    result(2, 3) = -(2.0f * aFar * aNear) / (aFar - aNear);
    result(3, 2) = -1.0f;
    result(3, 3) = 0.0f;

    return result;
}



#endif // MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA
