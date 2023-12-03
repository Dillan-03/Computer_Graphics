// // You will need to define your own tests. Refer to CW1 or Exercise G.3 for
// // examples.

// #include <catch2/catch_amalgamated.hpp>
// #include "../vmlib/mat44.hpp"

// // Test Cases for Mat44f operator*
// // Checking the operations are computed correctly
// TEST_CASE("Mat44f operator*") {

// 	//Identity matrix
// 	Mat44f identity = kIdentity44f;

// 	//Multiplying two matrices to check if they are calculated correctly
// 	SECTION("Mat44f * Mat44f") {

// 		Mat44f matrixLeft = {
// 			{1.0f, 2.0f, 3.0f, 4.0f},
// 			{5.0f, 6.0f, 7.0f, 8.0f},
// 			{9.0f, 10.0f, 11.0f, 12.0f},
// 			{13.0f, 14.0f, 15.0f, 16.0f}
// 		};

// 		Mat44f matrixRight = {
// 			{17.0f, 18.0f, 19.0f, 20.0f},
// 			{21.0f, 22.0f, 23.0f, 24.0f},
// 			{25.0f, 26.0f, 27.0f, 28.0f},
// 			{29.0f, 30.0f, 31.0f, 32.0f}
// 		};

// 		Mat44f result = {
// 			{250.0f, 260.0f, 270.0f, 280.0f},
// 			{618.0f, 644.0f, 670.0f, 696.0f},
// 			{986.0f, 1028.0f, 1070.0f, 1112.0f},
// 			{1354.0f, 1412.0f, 1470.0f, 1528.0f}
// 		};

// 		Mat44f calculated_matrix = matrixLeft * matrixRight;

// 		//Looping through and comparing each value to see if they match
// 		for (int row = 0; row < 4; row++) {
// 			for (int column = 0; column < 4; column++) {
// 				REQUIRE(result(row, column) == calculated_matrix(row, column));
// 			}
// 		}

// 	}

// 	//Matrix with the Identity
// 	SECTION("Mat44f * Mat44f Identity") {
// 		Mat44f matrixLeft = {
// 			{1.0f, 2.0f, 3.0f, 4.0f},
// 			{5.0f, 6.0f, 7.0f, 8.0f},
// 			{9.0f, 10.0f, 11.0f, 12.0f},
// 			{13.0f, 14.0f, 15.0f, 16.0f}
// 		};

// 		Mat44f calculated_matrix = matrixLeft * identity;

// 		//Looping through and comparing each value to see if they match
// 		for (int row = 0; row < 4; row++) {
// 			for (int column = 0; column < 4; column++) {
// 				REQUIRE(identity(row, column) == calculated_matrix(row, column));
// 			}
// 		}
// 	}	

// 	//Symmetric matrix is not the case 
// 	//MatrixA = MatrixB, MatrixB != MatrixA
// 	SECTION("Symmetric Test"){
// 		Mat44f matrixLeft = {
// 			{1.0f, 2.0f, 3.0f, 4.0f},
// 			{5.0f, 6.0f, 7.0f, 8.0f},
// 			{9.0f, 10.0f, 11.0f, 12.0f},
// 			{13.0f, 14.0f, 15.0f, 16.0f}
// 		};

// 		Mat44f matrixRight = {
// 			{17.0f, 18.0f, 19.0f, 20.0f},
// 			{21.0f, 22.0f, 23.0f, 24.0f},
// 			{25.0f, 26.0f, 27.0f, 28.0f},
// 			{29.0f, 30.0f, 31.0f, 32.0f}
// 		};

// 		Mat44f LeftRight = matrixLeft * matrixRight;
// 		Mat44f RightLeft = matrixRight * matrixLeft;

// 		//Check to see if both matrices are the same
// 		REQUIRE_FALSE(LeftRight == RightLeft);
// 	}

// }


// // Test Case for Vec4f Operator*
// // Matrix * Vector
// TEST_CASE("Mat44f * Vec4f") {

// 	Mat44f identity = kIdentity44f;

// 	// Identity Matrix multiplied by a vector
// 	SECTION("Mat44f Identity * Vec4f") {

// 		Vec4f vector = {1.0f, 2.0f, 3.0f, 4.0f};

// 		Vec4f result = {1.0f, 2.0f, 3.0f, 4.0f};

// 		Vec4f calculated_vector = identity * vector;

// 		//Looping through and comparing each value to see if they match
// 		for (int row = 0; row < 4; row++) {
// 			REQUIRE(result(row) == calculated_vector(row));
// 		}
// 	}

// 	// Matrix multiplied by a vector
// 	SECTION("Mat44 * Vec4f"){
// 		Mat44f aLeft = {
// 			{1.0f, 2.0f, 3.0f, 4.0f},
// 			{5.0f, 6.0f, 7.0f, 8.0f},
// 			{9.0f, 10.0f, 11.0f, 12.0f},
// 			{13.0f, 14.0f, 15.0f, 16.0f}
// 		};

// 		// Vector 
// 		Vec4f aRight = {17.0f, 18.0f, 19.0f, 20.0f};

// 		// Expected result
// 		Vec4f result = {250.0f, 260.0f, 270.0f, 280.0f};

// 		// Calculated result
// 		Vec4f calculated_vector = aLeft * aRight;

// 		// Looping through and comparing each value to see if they match
// 		for (int row = 0; row < 4; row++) {
// 			REQUIRE(result(row) == calculated_vector(row));
// 		}
// 	}

// 	// Vector multiplied by a translation matrix
// 	SECTION("Vec4 * Mat44f Translation"){
// 		Mat44f matrix = {
// 			(1.0f, 0.0f, 0.0f, 10.0f),
// 			{0.0f, 1.0f, 0.0f, 9.0f},
// 			{0.0f, 0.0f, 1.0f, 8.0f},
// 			{0.0f, 0.0f, 0.0f, 1.0f}
// 		};

// 		// Vector 
// 		Vec4f vector = {17.0f, 18.0f, 19.0f, 1.0f};

// 		// Expected result. Translating (17,18,19) by (10,9,8)
// 		Vec4f result = {17.0f + 10.0f, 18.0f + 9.0f, 19.0f + 8.0f, 20.0f};

// 		// Calculated result
// 		Vec4f calculated_vector = matrix * vector;

// 		// Check to see if they match
// 		REQUIRE_THAT(result(0), WithinAbs(calculated_vector(0), kEps_));
// 		REQUIRE_THAT(result(1), WithinAbs(calculated_vector(1), kEps_));
// 		REQUIRE_THAT(result(2), WithinAbs(calculated_vector(2), kEps_));
// 		REQUIRE_THAT(result(3), WithinAbs(calculated_vector(3), kEps_));

// 	}
// }

// // Test Cases for make_rotation(x,y,z)
// // Check that rotation around the z-axis is correct and computes the correct value
// TEST_CASE( "4x4 rotation around Z axis", "[rotation][mat44]" )
// {
// 	static constexpr float kEps_ = 1e-6f;

// 	using namespace Catch::Matchers;

// 	// Simple check: rotating zero degrees should yield an identity matrix
// 	SECTION( "Identity" )
// 	{
// 		auto const identity = make_rotation_z( 0.f );

//         //To compare each value in the matrix with the expected value, give or take the tolerance value of kEps_

// 		REQUIRE_THAT( identity(0,0), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( identity(0,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(0,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(0,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( identity(1,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(1,1), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( identity(1,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(1,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( identity(2,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(2,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(2,2), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( identity(2,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( identity(3,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(3,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(3,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(3,3), WithinAbs( 1.f, kEps_ ) );
// 	}

// 	// Rotating 90 degrees = pi/2 radians.
// 	SECTION( "90 degrees" )
// 	{

// 		// Expected known-good values for 90-degree rotation around Z-axis
// 		// | 0  -1  0  0 |
// 		// | 1  0  0  0 |
// 		// | 0  0  1  0 |
// 		// | 0  0  0  1 |
// 		auto const zPositiveRotation = make_rotation_z( 3.1415926f/2.f );

// 		REQUIRE_THAT( zPositiveRotation(0,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zPositiveRotation(0,1), WithinAbs( -1.f, kEps_ ) );
// 		REQUIRE_THAT( zPositiveRotation(0,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zPositiveRotation(0,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( zPositiveRotation(1,0), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( zPositiveRotation(1,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zPositiveRotation(1,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zPositiveRotation(1,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( zPositiveRotation(2,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zPositiveRotation(2,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zPositiveRotation(2,2), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( zPositiveRotation(2,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( zPositiveRotation(3,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zPositiveRotation(3,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zPositiveRotation(3,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zPositiveRotation(3,3), WithinAbs( 1.f, kEps_ ) );
// 	}

// 	// Rotating -90 degrees = -pi/2 radians.
// 	SECTION( "-90 degrees" )
// 	{
// 		// Expected known-good values for -90-degree rotation around Z-axis
// 		// | 0  1  0  0 |
// 		// | -1  0  0  0 |
// 		// | 0 0  1  0 |
// 		// | 0  0  0  1 |
// 		auto const zNegativeRotation = make_rotation_z( -3.1415926f/2.f );

// 		REQUIRE_THAT( zNegativeRotation(0,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zNegativeRotation(0,1), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( zNegativeRotation(0,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zNegativeRotation(0,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( zNegativeRotation(1,0), WithinAbs( -1.f, kEps_ ) );
// 		REQUIRE_THAT( zNegativeRotation(1,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zNegativeRotation(1,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zNegativeRotation(1,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( zNegativeRotation(2,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zNegativeRotation(2,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zNegativeRotation(2,2), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( zNegativeRotation(2,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( zNegativeRotation(3,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zNegativeRotation(3,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zNegativeRotation(3,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zNegativeRotation(3,3), WithinAbs( 1.f, kEps_ ) );
// 	}

// 	// Rotating 180 degrees = pi radians.
// 	SECTION("180 degrees"){

// 		// Expected known-good values for 180-degree rotation around Z-axis
// 		// | -1  0  0  0 |
// 		// | 0  -1  0  0 |
// 		// | 0  0  1  0 |
// 		// | 0  0  0  1 |
// 		auto const zFlipRotation = make_rotation_z( 3.1415926f );

//         REQUIRE_THAT( zFlipRotation(0,0), WithinAbs( -1.f, kEps_ ) );
//         REQUIRE_THAT( zFlipRotation(0,1), WithinAbs( 0.f, kEps_ ) );
//         REQUIRE_THAT( zFlipRotation(0,2), WithinAbs( 0.f, kEps_ ) );
//         REQUIRE_THAT( zFlipRotation(0,3), WithinAbs( 0.f, kEps_ ) );

//         REQUIRE_THAT( zFlipRotation(1,0), WithinAbs( 0.f, kEps_ ) );
//         REQUIRE_THAT( zFlipRotation(1,1), WithinAbs( -1.f, kEps_ ) );
// 		REQUIRE_THAT( zFlipRotation(1,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zFlipRotation(1,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( zFlipRotation(2,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zFlipRotation(2,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zFlipRotation(2,2), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( zFlipRotation(2,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( zFlipRotation(3,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zFlipRotation(3,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zFlipRotation(3,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( zFlipRotation(3,3), WithinAbs( 1.f, kEps_ ) );

// 	}
// }

// TEST_CASE( "4x4 rotation around Y axis", "[rotation][mat44]" )
// {
//     // The known-good value of the rotation matrix is the identity matrix
//     static constexpr float kEps_ = 1e-6f;

//     using namespace Catch::Matchers;

//     SECTION( "Identity" ){

//         auto const identity = make_rotation_y( 0.f );

// 		REQUIRE_THAT( identity(0,0), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( identity(0,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(0,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(0,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( identity(1,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(1,1), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( identity(1,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(1,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( identity(2,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(2,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(2,2), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( identity(2,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( identity(3,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(3,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(3,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(3,3), WithinAbs( 1.f, kEps_ ) );
//     }

// 	// Rotating 90 degrees = pi/2 radians.
// 	SECTION( "90 degrees" ) 
// 	{
// 		// Expected known-good values for 90-degree rotation around Y-axis
// 		// | 1  0  0  0 |
// 		// | 0  0  1  0 |
// 		// | 0 -1  0  0 |
// 		// | 0  0  0  1 |
// 		auto const yPositiveRotation = make_rotation_y( 3.1415926f/2.f );

// 		REQUIRE_THAT( yPositiveRotation(0,0), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(0,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(0,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(0,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( yPositiveRotation(1,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(1,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(1,2), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(1,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( yPositiveRotation(2,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(2,1), WithinAbs( -1.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(2,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(2,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( yPositiveRotation(3,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(3,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(3,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(3,3), WithinAbs( 1.f, kEps_ ) );
// 	}

// 	// Rotating -90 degrees = -pi/2 radians.
// 	SECTION( "-90 degrees" )
// 	{
// 		// Expected known-good values for -90-degree rotation around Y-axis
// 		// | 0  0  -1  0 |
// 		// | 0  1  0  0 |
// 		// | 1  0  0  0 |
// 		// | 0  0  0  1 |
// 		auto const yNegativeRotation = make_rotation_y( -3.1415926f/2.f );

// 		REQUIRE_THAT( yNegativeRotation(0,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(0,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(0,2), WithinAbs( -1.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(0,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( yNegativeRotation(1,0), WithinAbs(0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(1,1), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(1,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(1,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( yNegativeRotation(2,0), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(2,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(2,2), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(2,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( yNegativeRotation(3,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(3,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(3,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(3,3), WithinAbs( 1.f, kEps_ ) );
// 	}

// 	// Rotating 180 degrees = pi radians.
// 	SECTION("180 degrees"){

// 		// Expected known-good values for 180-degree rotation around Y-axis
// 		// | -1  0  0  0 |
// 		// | 0  1  0  0 |
// 		// | 0  0  -1  0 |
// 		// | 0  0  0  1 |
// 		auto const yFlipRotation = make_rotation_y( 3.1415926f );

// 		REQUIRE_THAT( yFlipRotation(0,0), WithinAbs( -1.f, kEps_ ) );
// 		REQUIRE_THAT( yFlipRotation(0,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yFlipRotation(0,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yFlipRotation(0,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( yFlipRotation(1,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yFlipRotation(1,1), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( yFlipRotation(1,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yFlipRotation(1,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( yFlipRotation(2,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yFlipRotation(2,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yFlipRotation(2,2), WithinAbs( -1.f, kEps_ ) );
// 		REQUIRE_THAT( yFlipRotation(2,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( yFlipRotation(3,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yFlipRotation(3,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yFlipRotation(3,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yFlipRotation(3,3), WithinAbs( 1.f, kEps_ ) );
// 	}

// }

// TEST_CASE( "4x4 rotation around X axis", "[rotation][mat44]" )
// {
//     // The known-good value of the rotation matrix is the identity matrix
//     static constexpr float kEps_ = 1e-6f;

//     using namespace Catch::Matchers;

//     SECTION( "Identity" ){

//         auto const identity = make_rotation_x( 0.f );

// 		REQUIRE_THAT( identity(0,0), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( identity(0,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(0,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(0,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( identity(1,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(1,1), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( identity(1,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(1,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( identity(2,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(2,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(2,2), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( identity(2,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( identity(3,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(3,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(3,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( identity(3,3), WithinAbs( 1.f, kEps_ ) );
//     }

// 	// Rotating 90 degrees = pi/2 radians.
// 	SECTION( "90 degrees" ) 
// 	{
// 		// Expected known-good values for 90-degree rotation around Y-axis
// 		// | 1  0  0  0 |
// 		// | 0  0  -1  0 |
// 		// | 0 1  0  0 |
// 		// | 0  0  0  1 |
// 		auto const yPositiveRotation = make_rotation_x( 3.1415926f/2.f );

// 		REQUIRE_THAT( yPositiveRotation(0,0), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(0,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(0,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(0,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( yPositiveRotation(1,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(1,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(1,2), WithinAbs( -1.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(1,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( yPositiveRotation(2,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(2,1), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(2,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(2,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( yPositiveRotation(3,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(3,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(3,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yPositiveRotation(3,3), WithinAbs( 1.f, kEps_ ) );
// 	}

// 	// Rotating -90 degrees = -pi/2 radians.
// 	SECTION( "-90 degrees" )
// 	{
		
// 		// Expected known-good values for -90-degree rotation around x-axis
// 		// | 1  0  0  0 |
// 		// | 0  0  1  0 |
// 		// | 0  -1  0  0 |
// 		// | 0  0  0  1 |
// 		auto const yNegativeRotation = make_rotation_y( -3.1415926f/2.f );

// 		REQUIRE_THAT( yNegativeRotation(0,0), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(0,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(0,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(0,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( yNegativeRotation(1,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(1,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(1,2), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(1,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( yNegativeRotation(2,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(2,1), WithinAbs( -1.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(2,2), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(2,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( yNegativeRotation(3,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(3,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(3,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( yNegativeRotation(3,3), WithinAbs( 1.f, kEps_ ) );
// 	}

// 	// Rotating 180 degrees = pi radians.
// 	SECTION( "180 degrees" )
// 	{
// 		// Expected known-good values for 180-degree rotation around x-axis
// 		// | 1  0  0  0 |
// 		// | 0  -1  0  0 |
// 		// | 0  0  -1  0 |
// 		// | 0  0  0  1 |
// 		auto const xFlipRotation = make_rotation_x( 3.1415926f );

// 		REQUIRE_THAT( xFlipRotation(0,0), WithinAbs( 1.f, kEps_ ) );
// 		REQUIRE_THAT( xFlipRotation(0,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( xFlipRotation(0,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( xFlipRotation(0,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( xFlipRotation(1,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( xFlipRotation(1,1), WithinAbs( -1.f, kEps_ ) );
// 		REQUIRE_THAT( xFlipRotation(1,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( xFlipRotation(1,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( xFlipRotation(2,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( xFlipRotation(2,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( xFlipRotation(2,2), WithinAbs( -1.f, kEps_ ) );
// 		REQUIRE_THAT( xFlipRotation(2,3), WithinAbs( 0.f, kEps_ ) );

// 		REQUIRE_THAT( xFlipRotation(3,0), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( xFlipRotation(3,1), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( xFlipRotation(3,2), WithinAbs( 0.f, kEps_ ) );
// 		REQUIRE_THAT( xFlipRotation(3,3), WithinAbs( 1.f, kEps_ ) );

// 	}


// }


// // Test Case for make_translation(Vec3f)
// // To check that the translation on a object works as intended
// TEST_CASE( "make_translation(Vec3f)", "[translation][mat44]" ){

//  	static constexpr float kEps_ = 1e-6f;

//     using namespace Catch::Matchers;

// 	Mat44f identity = kIdentity44f;

// 	//Zero Translation being done
// 	SECTION("Zero Translation"){
// 		Vec3f vector = {0.f, 0.f, 0.f};
// 		Mat44f translation = make_translation(vector);

// 		//Check to see if the translation matrix is the same as the identity matrix
// 		REQUIRE(translation == identity);
// 	}

// 	//Translation being done
// 	SECTION("Translation Taking Place")
// 	{
// 		Vec3f vector = {1.f, 2.f, 3.f};
// 		Mat44f translation = make_translation(vector);

// 		//Check each value in the matrix to see if it has been correctly translated
// 		REQUIRE_THAT(translation(0,0), WithinAbs(1.f, kEps_));
// 		REQUIRE_THAT(translation(0,1), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(translation(0,2), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(translation(0,3), WithinAbs(2.f, kEps_));

// 		REQUIRE_THAT(translation(1,0), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(translation(1,1), WithinAbs(1.f, kEps_));
// 		REQUIRE_THAT(translation(1,2), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(translation(1,3), WithinAbs(3.f, kEps_));

// 		REQUIRE_THAT(translation(2,0), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(translation(2,1), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(translation(2,2), WithinAbs(1.f, kEps_));
// 		REQUIRE_THAT(translation(2,3), WithinAbs(4.f, kEps_));

// 		REQUIRE_THAT(translation(3,0), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(translation(3,1), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(translation(3,2), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(translation(3,3), WithinAbs(1.f, kEps_));
// 	}

// }


// // Test Case for make_perspective_projection(float, float, float, float)
// // To check that the perspective projection matrix is correct
// TEST_CASE("Perspective Projection", "[perspective][mat44]"){

// 	constexpr float kPi_ = 3.1415926f;

//  	static constexpr float kEps_ = 1e-6f;

//     using namespace Catch::Matchers;

// 	// Checking the function works with a simple case
// 	SECTION("Normal Perspective Projection"){
// 		float FOV = kPi_/4.f; //45 degrees
//         float aAspect = 16.f/9.f; //Aspect ratio for normal screens with a resolution of 1920x1080 
//         float aNear = 0.1f;
//         float aFar = 50.f; //objects more that 50 units away from the camera will not be rendered

//         Mat44f perspective = make_perspective_projection(FOV,aAspect,aNear,aFar);

// 		const float s = 1.0f/std::tan(FOV / 2.0f);

// 		float sx = s/aAspect;
// 		float sy = s;
// 		float a = -((aFar + aNear) / (aFar - aNear));
//    		float b = -2.0f * ((aFar * aNear) / (aFar - aNear));
    
//         //Check each value in the matrix to see if it has been correctly translated
// 		REQUIRE_THAT(perspective(0,0), WithinAbs(sx, kEps_));
// 		REQUIRE_THAT(perspective(0,1), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(0,2), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(0,3), WithinAbs(0.f, kEps_));

// 		REQUIRE_THAT(perspective(1,0), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(1,1), WithinAbs(sy, kEps_));
// 		REQUIRE_THAT(perspective(1,2), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(1,3), WithinAbs(0.f, kEps_));

// 		REQUIRE_THAT(perspective(2,0), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(2,1), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(2,2), WithinAbs(a, kEps_));
// 		REQUIRE_THAT(perspective(2,3), WithinAbs(b, kEps_));

// 		REQUIRE_THAT(perspective(3,0), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(3,1), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(3,2), WithinAbs(-1.f, kEps_));
// 		REQUIRE_THAT(perspective(3,3), WithinAbs(0.f, kEps_));
// 	}

// 	// Check for edge cases
// 	// To see if the function can handle extreme values too 
// 	SECTION("Edge Cases"){
// 		float FOV = kPi_/180.f; //2 degrees. Narrow field of view
//         float aAspect = 16.f/9.f; //Aspect ratio for normal screens with a resolution of 1920x1080 
//         float aNear = 0.1f;
//         float aFar = 7000.f; //objects more that 7000 units away from the camera will not be rendered. A larger further vision

//         Mat44f perspective = make_perspective_projection(FOV,aAspect,aNear,aFar);

// 		const float s = 1.0f/std::tan(FOV / 2.0f);

// 		float sx = s/aAspect;
// 		float sy = s;
// 		float a = -((aFar + aNear) / (aFar - aNear));
//    		float b = -2.0f * ((aFar * aNear) / (aFar - aNear));
    
//         //Check each value in the matrix to see if it has been correctly translated
// 		REQUIRE_THAT(perspective(0,0), WithinAbs(sx, kEps_));
// 		REQUIRE_THAT(perspective(0,1), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(0,2), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(0,3), WithinAbs(0.f, kEps_));

// 		REQUIRE_THAT(perspective(1,0), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(1,1), WithinAbs(sy, kEps_));
// 		REQUIRE_THAT(perspective(1,2), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(1,3), WithinAbs(0.f, kEps_));

// 		REQUIRE_THAT(perspective(2,0), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(2,1), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(2,2), WithinAbs(a, kEps_));
// 		REQUIRE_THAT(perspective(2,3), WithinAbs(b, kEps_));

// 		REQUIRE_THAT(perspective(3,0), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(3,1), WithinAbs(0.f, kEps_));
// 		REQUIRE_THAT(perspective(3,2), WithinAbs(-1.f, kEps_));
// 		REQUIRE_THAT(perspective(3,3), WithinAbs(0.f, kEps_));
// 	}


// }
