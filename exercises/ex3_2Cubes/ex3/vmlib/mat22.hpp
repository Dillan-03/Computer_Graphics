#ifndef MAT22_HPP_1F974C02_D0D1_4FBD_B5EE_A69C88112088
#define MAT22_HPP_1F974C02_D0D1_4FBD_B5EE_A69C88112088

#include <cmath>

#include "vec2.hpp"

/** Mat22f : 2x2 matrix with floats
 *
 * See comments for Vec2f for some discussion.
 *
 * The matrix is stored in row-major order.
 *
 * Example:
 *   Mat22f identity{ 
 *     1.f, 0.f,
 *     0.f, 1.f
 *   };
 */
struct Mat22f
{
	float _00, _01;
	float _10, _11;
};

// Common operators for Mat22f.
// Note that you will need to implement these yourself.

constexpr
Mat22f operator*( Mat22f const& aLeft, Mat22f const& aRight ) noexcept
{
	 return Mat22f
	{
        aLeft._00 * aRight._00 + aLeft._01 * aRight._10,  
        aLeft._00 * aRight._01 + aLeft._01 * aRight._11,  
        aLeft._10 * aRight._00 + aLeft._11 * aRight._10,  
        aLeft._10 * aRight._01 + aLeft._11 * aRight._11  
    };
}

constexpr
Vec2f operator*( Mat22f const& aLeft, Vec2f const& aRight ) noexcept
{
	return Vec2f
	{
        aLeft._00 * aRight.x + aLeft._01 * aRight.y,  
        aLeft._10 * aRight.x + aLeft._11 * aRight.y  
    };
}

// Functions:

inline
Mat22f make_rotation_2d( float aAngle ) noexcept
{
	float cosA = std::cos(aAngle);
    float sinA = std::sin(aAngle);

    return Mat22f
	{
        cosA, -sinA,
        sinA, cosA
    };
}

#endif // MAT22_HPP_1F974C02_D0D1_4FBD_B5EE_A69C88112088

