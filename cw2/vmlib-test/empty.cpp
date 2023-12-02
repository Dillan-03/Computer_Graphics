// You will need to define your own tests. Refer to CW1 or Exercise G.3 for
// examples.


// Test Cases for making_rotation(x,y,z)
// Check that rotation around the z-axis is correct and computes the correct value
TEST_CASE( "4x4 rotation around Z axis", "[rotation][mat44]" )
{
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	// Simple check: rotating zero degrees should yield an idenity matrix
	SECTION( "Identity" )
	{
		auto const identity = make_rotation_z( 0.f );

        //To compare each value in the matrix with the expected value, give or take the tolerance value of kEps_

		REQUIRE_THAT( identity(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 90 degrees = pi/2 radians.
	SECTION( "90 degrees" )
	{
		auto const right = make_rotation_z( 3.1415926f/2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating -90 degrees = -pi/2 radians.
	SECTION( "-90 degrees" )
	{
		auto const right = make_rotation_z( -3.1415926f/2.f );

		REQUIRE_THAT( right(0,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(1,0), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( right(1,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( right(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( right(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( right(3,3), WithinAbs( 1.f, kEps_ ) );
	}
}


TEST_CASE( "4x4 rotation around Y axis", "[rotation][mat44]" )
{
    // The known-good value of the rotation matrix is the identity matrix
    static constexpr float kEps_ = 1e-6f;

    using namespace Catch::Matchers;

    SECTION( "Identity" ){

        auto const identity = make_rotation_z( 0.f );

		REQUIRE_THAT( identity(0,0), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(0,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(0,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(1,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,1), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(1,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(1,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(2,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(2,2), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( identity(2,3), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( identity(3,0), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,1), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,2), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( identity(3,3), WithinAbs( 1.f, kEps_ ) );
    }
}