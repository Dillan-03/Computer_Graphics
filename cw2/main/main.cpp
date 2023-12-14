#include <glad.h>
#include <GLFW/glfw3.h>

#include <typeinfo>
#include <stdexcept>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <tuple>

#include "../support/error.hpp"
#include "../support/program.hpp"
#include "../support/checkpoint.hpp"
#include "../support/debug_output.hpp"

#include "../vmlib/vec4.hpp"
#include "../vmlib/mat44.hpp"
#include "../vmlib/mat33.hpp"
#include "../vmlib/vec3.hpp"

#include "defaults.hpp"
#include "loadcustom.hpp"
#include "cone.hpp"
#include "cylinder.hpp"
#include "cube.hpp"
#include "simple_mesh.hpp"


namespace
{
	constexpr char const* kWindowTitle = "COMP3811 - CW2";
	constexpr float kPi_ = 3.1415926f;
	constexpr float kMovementPerSecond_ = 5.f; // Units per second
	constexpr float kMouseSensitivity_ = 0.01f; // Radians per pixel

	// Hardcode the phi and theta values so that the spaceship look at the same angle each time 
	const float fixedCameraPhi = 18.0299; // More to the left
	const float fixedCameraTheta = 0.240; // More upward

	// Define camera modes
	enum CameraMode {
		DEFAULT_CAMERA,
		FIXED_DISTANCE_CAMERA,
		GROUND_CAMERA
	};

	struct State_
	{
		// Shader program for the map
		ShaderProgram* terrain;
		// Shader Program for the pad 
		ShaderProgram* pad;  
		// Shader program for space vehicle 
		ShaderProgram* spaceVehicle;
		// Initial hardcoded position
		Vec3f spaceVehiclePosition = Vec3f{0.f, -0.969504f, -2.5f}; 

	  
		 bool thrust = false;
	    CameraMode cameraMode = DEFAULT_CAMERA; // Add camera mode variable
		
		bool splitScreen = false;

		// Bezier curve positions
		Vec3f starting;
		Vec3f control1;
		Vec3f control2;
		Vec3f ending;
		float fRotation = 0.0f;
		float rotateSpaceship = 0.0f;
		bool stopped = false; 

		float bezier = 0.0f;

		struct CamCtrl_
		{

			// Added variables for wasd 
			bool cameraActive;
			bool actionZoomIn, actionZoomOut; // W, S
			bool actionLeft, actionRight; // A, D
			bool up, down; // Q, E
			bool shift = false; // state for Shift key
			bool control = false; // state for control key
			bool speedBoostApplied = false; // speed up
			bool slowDownApplied = false; // slow down 
			
			bool isFixedCameraInitialized= false;

			float spaceVehicleThrust = 0.0f;  

			Vec3f position;

			// phi is rotation around the y-axis 
			// theta is rotation around the x-axis
			float phi, theta;
			float radius;
			float lastX, lastY;

			// TO decide which camera is being used
			bool LeftCamera = false;
			bool RightCamera = false;

			Vec3f cameraView; // Camera's front view so that the controls are relative to the way we're looking
		} camControl, camControlRight;
	};

	

	//Using the logic from the following [https://www.jasondavies.com/animated-bezier/]
	//Using the logic from the following [https://en.wikipedia.org/wiki/B%C3%A9zier_curve]
	Vec3f curveBezier(Vec3f startingPoint, Vec3f factor, Vec3f endingPoint, float parameter) {

		//Default parameter is 1
		//0 <= Parameter <= 1
		// Curve will start at point0, and where point2 is ending point where the spaceship is horizontal
		float tParameter = 1 - parameter;

		float tSquared = tParameter * tParameter; //To find each starting point as the parameter changes 
		float middleParameter = parameter * parameter; //Used to find the ending point of the curve 

		Vec3f curvePoint = tSquared * startingPoint; //Finds the starting point of the curve and how the curve will go from there
		curvePoint += 2 * tParameter * parameter * factor; // Ensuring the curve is mre naturally curvy in the middle section
		curvePoint += middleParameter * endingPoint; //Returns a vector which represents a specific point in the curve that the spaceship can follow through

		return curvePoint;
	}	
	
	void glfw_callback_error_( int, char const* );

	void glfw_callback_key_( GLFWwindow*, int, int, int, int );
	void glfw_callback_motion_( GLFWwindow*, double, double );
	struct GLFWCleanupHelper
	{
		~GLFWCleanupHelper();
	};
	struct GLFWWindowDeleter
	{
		~GLFWWindowDeleter();
		GLFWwindow* window;
	};

}


int main() try
{
	// ----------INITIALISE GLFW----------
	
	if( GLFW_TRUE != glfwInit() )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwInit() failed with '%s' (%d)", msg, ecode );
	}

	// Ensure that we call glfwTerminate() at the end of the program.
	GLFWCleanupHelper cleanupHelper;

	// Configure GLFW and create window
	glfwSetErrorCallback( &glfw_callback_error_ );
	glfwWindowHint( GLFW_SRGB_CAPABLE, GLFW_TRUE );
	glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_DEPTH_BITS, 24 );

	#if !defined(NDEBUG)

	/* When building in debug mode, request an OpenGL debug context. This
	enables additional debugging features. However, this can carry extra
	overheads. We therefore do not do this for release builds. */

	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
	#endif // ~ !NDEBUG

	GLFWwindow* window = glfwCreateWindow(
		1280,
		720,
		kWindowTitle,
		nullptr, nullptr
	);

	if( !window )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwCreateWindow() failed with '%s' (%d)", msg, ecode );
	}

	GLFWWindowDeleter windowDeleter{ window };

	//----------STATE AND BEZIER CURVE INITIALISATION----------
	
	State_ state{};

	// Bezier curve points
	state.starting = Vec3f{0.f, -0.969504f, -2.5f}; // Starting point of the curve 
	state.control1 = Vec3f{0.f, 11.92000, -2.5f};  // Control point to bind the curve to that point so that the curve doesn't exceed the y-coordinate
	state.ending = Vec3f{7.8f, 11.92000, -2.5f}; // Ending point when it reaches the horizontal state
	
	state.bezier = 0.0f;

	//----------SETTING UP CALLBACKS AND CONTEXT PROPERTIES----------

	glfwSetWindowUserPointer( window, &state );
	glfwSetKeyCallback( window, &glfw_callback_key_ );
	glfwSetCursorPosCallback( window, &glfw_callback_motion_ );
	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 ); // V-Sync is on.

	// ----------INITIALISE GLAD---------- 
	// This will load the OpenGL API. We mustn't make any OpenGL calls before this!
	if( !gladLoadGLLoader( (GLADloadproc)&glfwGetProcAddress ) )
		throw Error( "gladLoaDGLLoader() failed - cannot load GL API!" );

	std::printf( "RENDERER %s\n", glGetString( GL_RENDERER ) );
	std::printf( "VENDOR %s\n", glGetString( GL_VENDOR ) );
	std::printf( "VERSION %s\n", glGetString( GL_VERSION ) );
	std::printf( "SHADING_LANGUAGE_VERSION %s\n", glGetString( GL_SHADING_LANGUAGE_VERSION ) );

	// Ddebug output
	#if !defined(NDEBUG)
	setup_gl_debug_output();
	#endif // ~ !NDEBUG

    // ----------INITIALISE SHADER PROGRAMS----------
	// Global GL state
	OGL_CHECKPOINT_ALWAYS();
	glEnable(GL_FRAMEBUFFER_SRGB);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glEnable( GL_DEPTH_TEST );
	OGL_CHECKPOINT_ALWAYS();

	// Get actual framebuffer size.
	// This can be different from the window size, as standard window
	// decorations (title bar, borders, ...) may be included in the window size
	// but not be part of the drawable surface area.
	int iwidth, iheight;
	glfwGetFramebufferSize( window, &iwidth, &iheight );
	glViewport( 0, 0, iwidth, iheight );
	
	// Terrain shader program
	ShaderProgram terrain( {
		{ GL_VERTEX_SHADER, "assets/default.vert" },
		{ GL_FRAGMENT_SHADER, "assets/default.frag" }
	} );
	state.terrain = &terrain;
	state.camControl.radius = 10.f;
	
	// Pad shader program
	ShaderProgram pad( {
		{ GL_VERTEX_SHADER, "assets/spaceVehicle.vert" },
		{ GL_FRAGMENT_SHADER, "assets/spaceVehicle.frag" }
	} );
	state.pad = &pad;

	// Space vehicle shader program
	ShaderProgram spaceVehicle( {
			{ GL_VERTEX_SHADER, "assets/spaceVehicle.vert" },
			{ GL_FRAGMENT_SHADER, "assets/spaceVehicle.frag" }
		} );
    state.spaceVehicle = &spaceVehicle;

	// Animation state
	auto last = Clock::now();
	float angle = 0.f;

	//Loading in map
	SimpleMeshData map = load_wavefront_obj("assets/parlahti.obj");
	GLuint vao = create_vao( map );
	std::size_t VertexCount = map.positions.size();

	//Loading in the texture
	GLuint textureMap = load_texture_world("assets/L4343A-4k.jpeg");
	
	//Loading in LandingPad
	SimpleMeshDataNoTexture Launchpad = load_noTexture("assets/landingpad.obj");
	GLuint vaoPad = create_novaoTexture( Launchpad );
	std::size_t padVertexCount = Launchpad.positions.size();

	// Set initial position
	state.camControl.position = {0.0f, 0.0f, 0.0f};
	// Set initial speed
	float speed = kMovementPerSecond_; 

	//----------3D SPACE VEHICLE CODE----------

	/* The code below creates the 3D space vehicle by creating 28 shapes including cylinders, cones, cubes and cuboids.
	Each shape is made with its coordinates relative to the second launchpad. Colours are also specified here in addition
	to rotations and translations*/

	// BIG CYLINDERS AND BIG CONE
	// Cone on top
	SimpleMeshDataNoTexture topCone = make_cone(true, 16, {0.58823529411f, 0.63529411764f, 0.67058823529f}, 
					make_scaling(0.25f, 0.5f, 0.25f) * make_translation({0.f, 3.52f, 0.f}) * make_rotation_z(3.1415926f / 2));

	// Base Cylinder
	// Create the cylinder, apply a rotation to make it vertical, and then scale it.
	SimpleMeshDataNoTexture firstCyl = make_cylinder(
		true, 16, {0.58823529411f, 0.63529411764f, 0.67058823529f},  make_scaling(0.25f, 0.5f, 0.25f)* make_translation({0.f, 0.52f, 0.f}) * make_rotation_z(3.1415926f / 2) );
	
	// Middle Cylinder
	SimpleMeshDataNoTexture midCyl = make_cylinder(
		true, 16, {0.13725490196f, 0.14117647058f, 0.14117647058f},  make_scaling(0.25, 0.5f, 0.25f)* make_translation({0.f, 1.52f, 0.f}) * make_rotation_z(3.1415926f / 2) );

	// Top Cylinder
	SimpleMeshDataNoTexture topCyl = make_cylinder(
		true, 16, {0.58823529411f, 0.63529411764f, 0.67058823529f},  make_scaling(0.25, 0.5f, 0.25f)* make_translation({0.f, 2.52f, 0.f}) * make_rotation_z(3.1415926f / 2) );

	// MINI ROCKETS FOR BOTTOM CYLINDER 
	// Mini Cylinder 1
	SimpleMeshDataNoTexture miniCyl1 = make_cylinder(
		true, 16, {0.13725490196f, 0.14117647058f, 0.14117647058f},  make_scaling(0.08, 0.1667f, 0.08f)* make_translation({3.5f, 2.31f, 0.f}) * make_rotation_z(3.1415926f / 2) );

	// Mini cone on top 1
	SimpleMeshDataNoTexture miniCone1 = make_cone(true, 16, {0.7294117647f, 0.12549019607f, 0.12549019607f}, 
					make_scaling(0.08f, 0.1667f, 0.08f) * make_translation({3.5f, 3.31f, 0.f}) * make_rotation_z(3.1415926f / 2));

	// Mini Cylinder 2
	SimpleMeshDataNoTexture miniCyl2 = make_cylinder(
			true, 16, {0.13725490196f, 0.14117647058f, 0.14117647058f},  make_scaling(0.08, 0.1667f, 0.08f)* make_translation({-3.5f, 2.31f, 0.f}) * make_rotation_z(3.1415926f / 2) );

	// Mini cone on top 2
	SimpleMeshDataNoTexture miniCone2 = make_cone(true, 16, {0.7294117647f, 0.12549019607f, 0.12549019607f}, 
					make_scaling(0.08f, 0.1667f, 0.08f) * make_translation({-3.5f, 3.31f, 0.f}) * make_rotation_z(3.1415926f / 2));

	// Mini Cylinder 3 
	SimpleMeshDataNoTexture miniCyl3 = make_cylinder(
			true, 16, {0.13725490196f, 0.14117647058f, 0.14117647058f},  make_scaling(0.08, 0.1667f, 0.08f)* make_translation({0.f, 2.31f, 3.5f}) * make_rotation_z(3.1415926f / 2) );

	// Mini cone on top 3
	SimpleMeshDataNoTexture miniCone3 = make_cone(true, 16, {0.7294117647f, 0.12549019607f, 0.12549019607f}, 
					make_scaling(0.08f, 0.1667f, 0.08f) * make_translation({0.f, 3.31f, 3.5f}) * make_rotation_z(3.1415926f / 2));

	// Mini Cylinder 4
	SimpleMeshDataNoTexture miniCyl4 = make_cylinder(
			true, 16, {0.13725490196f, 0.14117647058f, 0.14117647058f},  make_scaling(0.08, 0.1667f, 0.08f)* make_translation({0.f, 2.31f, -3.5f}) * make_rotation_z(3.1415926f / 2) );

	// Mini cone on top 4
	SimpleMeshDataNoTexture miniCone4 = make_cone(true, 16, {0.7294117647f, 0.12549019607f, 0.12549019607f}, 
					make_scaling(0.08f, 0.1667f, 0.08f) * make_translation({0.f, 3.31f, -3.5f}) * make_rotation_z(3.1415926f / 2));

	
	// MINI ROCKETS FOR TOP CYLINDER  
	// Mini Cylinder 5
	SimpleMeshDataNoTexture miniCyl5 = make_cylinder(
		true, 16, {0.13725490196f, 0.14117647058f, 0.14117647058f},  make_scaling(0.08, 0.1667f, 0.08f)* make_translation({3.5f, 8.31f, 0.f}) * make_rotation_z(3.1415926f / 2) );

	// Mini cone on top 5
	SimpleMeshDataNoTexture miniCone5 = make_cone(true, 16, {0.7294117647f, 0.12549019607f, 0.12549019607f}, 
					make_scaling(0.08f, 0.1667f, 0.08f) * make_translation({3.5f, 9.31f, 0.f}) * make_rotation_z(3.1415926f / 2));

	// Mini Cylinder 6
	SimpleMeshDataNoTexture miniCyl6 = make_cylinder(
			true, 16, {0.13725490196f, 0.14117647058f, 0.14117647058f},  make_scaling(0.08, 0.1667f, 0.08f)* make_translation({-3.5f, 8.31f, 0.f}) * make_rotation_z(3.1415926f / 2) );

	// Mini cone on top 6
	SimpleMeshDataNoTexture miniCone6 = make_cone(true, 16, {0.7294117647f, 0.12549019607f, 0.12549019607f}, 
					make_scaling(0.08f, 0.1667f, 0.08f) * make_translation({-3.5f, 9.31f, 0.f}) * make_rotation_z(3.1415926f / 2));

	// Mini Cylinder 7 
	SimpleMeshDataNoTexture miniCyl7 = make_cylinder(
			true, 16, {0.13725490196f, 0.14117647058f, 0.14117647058f},  make_scaling(0.08, 0.1667f, 0.08f)* make_translation({0.f, 8.31f, 3.5f}) * make_rotation_z(3.1415926f / 2) );

	// Mini cone on top 7
	SimpleMeshDataNoTexture miniCone7 = make_cone(true, 16, {0.7294117647f, 0.12549019607f, 0.12549019607f}, 
					make_scaling(0.08f, 0.1667f, 0.08f) * make_translation({0.f, 9.31f, 3.5f}) * make_rotation_z(3.1415926f / 2));

	// Mini Cylinder 8
	SimpleMeshDataNoTexture miniCyl8 = make_cylinder(
			true, 16, {0.13725490196f, 0.14117647058f, 0.14117647058f},  make_scaling(0.08, 0.1667f, 0.08f)* make_translation({0.f, 8.31f, -3.5f}) * make_rotation_z(3.1415926f / 2) );

	// Mini cone on top 8
	SimpleMeshDataNoTexture miniCone8 = make_cone(true, 16, {0.7294117647f, 0.12549019607f, 0.12549019607f}, 
					make_scaling(0.08f, 0.1667f, 0.08f) * make_translation({0.f, 9.31f, -3.5f}) * make_rotation_z(3.1415926f / 2));
	
	// BASE  
    // Cuboid 
    SimpleMeshDataNoTexture cuboid = make_cube(1, {0.27450980392f, 0.30196078431f, 0.32156862745f}, make_scaling(0.25f, 0.004f, 0.25f) * make_translation({0.f, 65.f, 0.f}));

    // 4 Small Cuboids
    SimpleMeshDataNoTexture smallCuboidOne = make_cube(1, {0.13333333333f, 0.16078431372f, 0.14901960784f}, make_scaling(0.029f, 0.06f, 0.029f) * make_translation({-7.5f, 3.26f , -7.5f}));
    SimpleMeshDataNoTexture smallCuboidTwo = make_cube(1, {0.13333333333f, 0.16078431372f, 0.14901960784f}, make_scaling(0.029f, 0.06f, 0.029f) * make_translation({7.5f, 3.26f , 7.5f}));
    SimpleMeshDataNoTexture smallCuboidThree = make_cube(1, {0.13333333333f, 0.16078431372f, 0.14901960784f}, make_scaling(0.029f, 0.06f, 0.029f) * make_translation({7.5f, 3.26f , -7.5f}));
    SimpleMeshDataNoTexture smallCuboidFour = make_cube(1, {0.13333333333f, 0.16078431372f, 0.14901960784f}, make_scaling(0.029f, 0.06f, 0.029f) * make_translation({-7.5f, 3.26f , 7.5f}));
    auto smallCuboids = concatenate(concatenate(concatenate(concatenate(std::move(smallCuboidOne),smallCuboidTwo),smallCuboidThree),smallCuboidFour),cuboid);

	// 4 cones for the thrusts 
    SimpleMeshDataNoTexture smallConeOne = make_cone(true, 16, {0.13333333333f, 0.16078431372f, 0.14901960784f}, 
                    make_scaling(0.09f, 0.15f, 0.09f) * make_translation({-2.4f, 0.2f, 2.4f}) * make_rotation_z(3.1415926f / 2));
    SimpleMeshDataNoTexture smallConeTwo = make_cone(true, 16, {0.13333333333f, 0.16078431372f, 0.14901960784f}, 
                    make_scaling(0.09f, 0.15f, 0.09f) * make_translation({2.4f, 0.2f, -2.4f}) * make_rotation_z(3.1415926f / 2));
    SimpleMeshDataNoTexture smallConeThree = make_cone(true, 16, {0.13333333333f, 0.16078431372f, 0.14901960784f}, 
                    make_scaling(0.09f, 0.15f, 0.09f) * make_translation({2.4f, 0.2f, 2.4f}) * make_rotation_z(3.1415926f / 2));
    SimpleMeshDataNoTexture smallConeFour = make_cone(true, 16, {0.13333333333f, 0.16078431372f, 0.14901960784f}, 
                    make_scaling(0.09f, 0.15f, 0.09f) * make_translation({-2.4f, 0.2f, -2.4f}) * make_rotation_z(3.1415926f / 2));
    auto smallCones = concatenate(concatenate(concatenate(std::move(smallConeOne),smallConeTwo),smallConeThree),smallConeFour);

	// Concatenating the base
    auto base = concatenate(std::move(smallCuboids),smallCones);

	// Concatenating the three big cylinders and the top cone
	auto cylindersCombined = concatenate(concatenate(concatenate(std::move(firstCyl), midCyl), topCyl), topCone);

	// Concatenating the four mini spaceships on the bottom cylinder
	auto miniCylbottom = concatenate(concatenate(concatenate(concatenate(concatenate(concatenate(concatenate
	(std::move(miniCyl1), miniCone1), miniCyl2), miniCone2), miniCyl3), miniCone3), miniCyl4), miniCone4);

	// Concatenating the four mini spaceships on the top cylinder
	auto miniCyltop = concatenate(concatenate(concatenate(concatenate(concatenate(concatenate(concatenate
	(std::move(miniCyl5), miniCone5), miniCyl6), miniCone6), miniCyl7), miniCone7), miniCyl8), miniCone8);

	// Combining all to spaceship 
	auto spaceShip = concatenate(concatenate(concatenate(std::move(cylindersCombined), miniCylbottom), miniCyltop), base);

	GLuint vaoShape = create_novaoTexture(spaceShip); 
	std::size_t shapevertexCounts = spaceShip.positions.size();

	//----------OTHER INITIALIASATION AND LOADING----------
	OGL_CHECKPOINT_ALWAYS();
	OGL_CHECKPOINT_ALWAYS();
	GLuint loc = glGetUniformLocation(state.terrain->programId(), "uNormalMatrix");
	GLuint locPad = glGetUniformLocation(state.pad->programId(), "uNormalMatrix");
	GLuint locPadSecond = glGetUniformLocation(state.pad->programId(), "uNormalMatrix");
	GLuint veh = glGetUniformLocation(state.spaceVehicle->programId(), "uNormalMatrix");
	GLuint locRight = glGetUniformLocation(state.terrain->programId(), "uNormalMatrix");
	GLuint locPadRight = glGetUniformLocation(state.pad->programId(), "uNormalMatrix");
	GLuint locPadSecondRight = glGetUniformLocation(state.pad->programId(), "uNormalMatrix");
	GLuint vehRight = glGetUniformLocation(state.spaceVehicle->programId(), "uNormalMatrix");

	//----------MAIN LOOP----------
	while( !glfwWindowShouldClose( window ) )
	{
		// Let GLFW process events
		glfwPollEvents();
		// Only once at the beginning of the render loop
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		
		// Check if window was resized.
		float fbwidth, fbheight;
		{
			int nwidth, nheight;
			glfwGetFramebufferSize( window, &nwidth, &nheight );

			fbwidth = float(nwidth);
			fbheight = float(nheight);

			if( 0 == nwidth || 0 == nheight )
			{
				// Window minimized? Pause until it is unminimized.
				do
				{
					glfwWaitEvents();
					glfwGetFramebufferSize( window, &nwidth, &nheight );
				} while( 0 == nwidth || 0 == nheight );
			}

			glViewport( 0, 0, nwidth, nheight );
		}

		//----------CAMERA AND PROJECTION MATRICES FOR RENDERING 3D SCENES----------
		// Update state
		// Get current time
		auto const now = Clock::now();
		// Get time since last frame
		float dt = std::chrono::duration_cast<Secondsf>(now-last).count();
		last = now;

		// Update angle for the camera
		angle += dt * kPi_ * 0.3f;
		if( angle >= 2.f*kPi_ )
			angle -= 2.f*kPi_;

		// Define up vector in the world space
		Vec3f upVector = {0.0f, 1.0f, 0.0f};
		// Create the view matrix for the camera
   	 	Mat44f viewMatrix = createViewMatrix(state.camControl.position, state.camControl.cameraView, upVector);
    	Mat44f viewMatrixRight = createViewMatrix(state.camControlRight.position, state.camControlRight.cameraView, upVector);
		Mat33f normalMatrix = mat44_to_mat33(transpose(invert(kIdentity44f)));
		// Rotation = 0 so that the terrain isn't rotating when the camera moves around
		Mat44f staticTerrain = make_rotation_y(0);

		// Create rotation matrices for the camera
		Mat44f Rx = make_rotation_x( state.camControl.theta );
		Mat44f Ry = make_rotation_y( state.camControl.phi );
		Mat44f T = make_translation( -state.camControl.position );
		Mat44f world2camera = Rx * Ry * T;

		// Set up the projection matrix for the camera
		Mat44f projection = make_perspective_projection(
			60.f * 3.1415926f / 180.f, 
			fbwidth/float(fbheight),
			0.1f, 100.0f);
		Mat44f projCameraWorld = projection * world2camera * staticTerrain;

		// To handle the right side of the screen when splitted
		Mat44f RxRight = make_rotation_x( state.camControlRight.theta );
		Mat44f RyRight = make_rotation_y( state.camControlRight.phi );
		Mat44f TRight = make_translation( -state.camControlRight.position );
		Mat44f world2cameraRight = RxRight * RyRight * TRight;
		Mat44f projectionRight = make_perspective_projection(
			60.f * 3.1415926f / 180.f, 
			fbwidth/float(fbheight),
			0.1f, 100.0f);
		Mat44f projCameraWorldRight = projectionRight * world2cameraRight * staticTerrain;

		// Create cameraworld on first pad
		// Only need to update the model2world to move the pad around 
		Mat44f model2worldPad = make_translation( {0.0f, -0.97300, 20.0f} );
		Mat44f projCameraWorldPad = projection * world2camera * model2worldPad;

		// Create cameraWorld on second pad 
		Mat44f model2worldPadsecond = make_translation( {0.f, -0.97300, -2.5f} );
		Mat44f projCameraWorldPadsecond = projection * world2camera* model2worldPadsecond;

		// ProjCameraWorld for spaceship
        Mat44f model2worldVehicle = make_translation( {state.spaceVehiclePosition} );
        Mat44f projCameraWorldVehicle = projection * world2camera * model2worldVehicle;
	
		// Right Screen
		// Only need to update the model2world to move the pad around 
		Mat44f model2worldPadRight = make_translation( {0.0f, -0.97300, 20.0f} );
		Mat44f projCameraWorldPadRight = projectionRight * world2cameraRight * model2worldPadRight;

		// Create cameraWorld on second pad 
		Mat44f model2worldPadsecondRight = make_translation( {0.f, -0.97300, -2.5f} );
		Mat44f projCameraWorldPadsecondRight = projectionRight * world2cameraRight * model2worldPadsecondRight;

		// ProjCameraWorld for spaceship
        Mat44f model2worldVehicleRight = make_translation( {state.spaceVehiclePosition} );
        Mat44f projCameraWorldVehicleRight = projectionRight * world2cameraRight * model2worldVehicleRight;

		
		//----------CAMERA MODES AND ANIMATION LOGIC----------
		// Camera control variables
		float height = 2.361225f; // Before the spaceship starts to curve
		constexpr float ROTATION_SPEED = 0.4f; // Rotation speed
		float counter_speed = 0.64f; // Slowly moving
		float start_speed = 0.0f;

		// Logic for animating the space vehicle
		if (state.thrust) {
			// Before the spaceship starts to curve
			if (state.spaceVehiclePosition.y < height) 
			{
				// Code to move spaceship vertically up to the height
				start_speed += speed * counter_speed;
				state.spaceVehiclePosition.y += start_speed * dt;
			} 
			else 
			{
				// Begin the bezier curve
				if (state.bezier < 0.8f) 
				{ // Only animate until 80% of the curve
					state.bezier += dt * 0.4; 
					state.bezier = std::min(state.bezier, 0.8f); 
					state.spaceVehiclePosition = curveBezier(Vec3f{0.f, height, -2.5f}, state.control1, state.ending, state.bezier);

					// Update rotation until the spaceship has stopped
					state.rotateSpaceship += dt * ROTATION_SPEED;
					if (state.rotateSpaceship >= kPi_ / 2.f) 
					{
						state.rotateSpaceship = kPi_ / 2.f;
					}
					// Get the final rotation
					state.fRotation = state.rotateSpaceship; 
				}

				// Apply the  final rotation when the space vehicle has reached 80% of the bezier curve
				Mat44f rotationMatrix = make_rotation_z(-state.fRotation);
				model2worldVehicle = make_translation(state.spaceVehiclePosition) * rotationMatrix;
				projCameraWorldVehicle = projection * world2camera * model2worldVehicle;
				model2worldVehicleRight = make_translation(state.spaceVehiclePosition) * rotationMatrix;
				projCameraWorldVehicleRight = projection * world2camera * model2worldVehicle;
			}
		}

		// Logic for the default camera mode
		if (state.cameraMode == DEFAULT_CAMERA) 
		{
			// Reset the flag if we switch to a different camera mode
			state.camControl.isFixedCameraInitialized = false;

			// Update camera state
			// PART ADDED FOR WASD   
			// Used the logic from the following [https://learnopengl.com/Getting-started/Camera]
			// Replacing glm for the phi and theta values
			// Calculate camera cameraView vector

			// Find the x coordinatees of the camera 
			state.camControl.cameraView.x = cos(state.camControl.phi) * cos(state.camControl.theta);

			// Find the y coordinatees of the camera 
			state.camControl.cameraView.y = sin(state.camControl.theta);

			// Find the z coordinatees of the camera 
			state.camControl.cameraView.z = sin(state.camControl.phi) * cos(state.camControl.theta);

			// We need to normalise so that the calculations are consistent 
			state.camControl.cameraView = normalize(state.camControl.cameraView);
			Vec3f camNormalised = normalize(cross({0.0f, 1.0f, 0.0f}, state.camControl.cameraView));
			// Set vectors for the camera movement
			Vec3f backwardMovement = camNormalised;
			Vec3f forwardMovement = camNormalised; 
			// The horizontal vector is used for right and left movement, it ignores the vertical movement (y axis)
			Vec3f horizontalForward = state.camControl.cameraView;
			horizontalForward.y = 0.0f; 
			horizontalForward = normalize(horizontalForward); 

			if (state.camControl.shift && state.camControl.speedBoostApplied)
			{
				// Multiplying the speed by 2.5 when shift is clicked
				speed = kMovementPerSecond_ * 2.5f; 
			}
			else if(state.camControl.control && state.camControl.slowDownApplied){
				// Halfing the speed when control is clicked
				speed = kMovementPerSecond_ * 0.5f; 
			}
			else
			{
				speed = kMovementPerSecond_;
			}

			//A Keyboard input, moving left
			if(state.camControl.actionLeft) {			
				state.camControl.position -= horizontalForward* speed * dt;
			}

			//D Keyboard input, moving right 
			if(state.camControl.actionRight) {
				state.camControl.position += horizontalForward * speed * dt;
			}

			
			/* The logic below for the W and S keys works by being relative to the camera angle.
			W and S can move along the x, y, and z axis. If the camera is looking straight up or down
			(90 degrees) then it only moves down the y. This is done by checking the theta angle, if it is within 
			an epsilon range of 90 degrees then that means the camera is looking straight down or up. In any other 
			orientation, the camera moves forward in the direction it is facing */

			// W Keyboard input, moving forward relative to angle
			if(state.camControl.actionZoomIn) {
					// Check if theta is within an epsilon range of M_PI / 2
					// We check if its looking up or down, if up the go up the y axis, if down then go down the y axis
				 if (std::abs(state.camControl.theta - M_PI / 2) < 0.01) {
					state.camControl.position.y -= speed * dt;
				} else if (std::abs(state.camControl.theta + M_PI / 2) < 0.01) {
					state.camControl.position.y += speed * dt;
				} else {
					// Normal forward movement
					forwardMovement.y -= state.camControl.cameraView.y;
					forwardMovement = normalize(forwardMovement);
					state.camControl.position += forwardMovement * speed * dt;
				}
			}

			// S Keyboard input, moving backward relative to angle (same logic as W)
			if(state.camControl.actionZoomOut) {
				// We check if its looking up or down, if up the go down the y axis, if down then go up the y axis
				if (std::abs(state.camControl.theta - M_PI / 2) < 0.01) {
					state.camControl.position.y += speed * dt;
				} else if (std::abs(state.camControl.theta + M_PI / 2) < 0.01) {
					state.camControl.position.y -= speed * dt;
				} else {
					//Normal backward movement
					backwardMovement.y = -state.camControl.cameraView.y;
					backwardMovement = normalize(backwardMovement);
					state.camControl.position -= backwardMovement * speed * dt;
				}
			}

			/* The logic for up and down (Q and E) is similiar here, to make it relative to the camera angle, if we are looking down 
			or up (90 degrees) then the Q and E will move the down horizontal axis otherwise it takes us up and down  */

			if(state.camControl.up){
				if (std::abs(state.camControl.theta - M_PI / 2) < 0.01 ) {
					state.camControl.position += camNormalised * speed * dt;
				}
				else if(std::abs(state.camControl.theta + M_PI / 2) < 0.01 ) {
					state.camControl.position -= camNormalised * speed * dt;
				}
				else {
					state.camControl.position.y += speed * dt;
				}
			}

			if(state.camControl.down){
				if (std::abs(state.camControl.theta - M_PI / 2) < 0.01 ) {
					state.camControl.position -= camNormalised * speed * dt;
				}
				else if(std::abs(state.camControl.theta + M_PI / 2) < 0.01 ) {
					state.camControl.position += camNormalised * speed * dt;
				}
				else {
					state.camControl.position.y -= speed * dt;
				}
			}
		
		}

		// Logic for the fixed distance camera mode
		if (state.cameraMode == FIXED_DISTANCE_CAMERA) 
		{
			// Set an distanceFromVehicle from the space vehicle
			Vec3f distanceFromVehicle = Vec3f{6.0f, 3.0f, 5.0f};
			
			// Update camera position based on the space vehicle position and the distanceFromVehicle
			state.camControl.position = state.spaceVehiclePosition + distanceFromVehicle;
			state.camControlRight.position = state.spaceVehiclePosition + distanceFromVehicle;

			// Update camera view to always look at the space vehicle
			state.camControl.cameraView = normalize(state.spaceVehiclePosition - state.camControl.position);
			state.camControlRight.position = state.spaceVehiclePosition + distanceFromVehicle;

			// Create the view matrix for the fixed distance camera
			viewMatrix = createViewMatrix(state.camControl.position, state.camControl.cameraView, upVector);
			viewMatrixRight = createViewMatrix(state.camControlRight.position, state.camControlRight.cameraView, upVector);
		}

		// Logic for the ground camera mode
		if (state.cameraMode == GROUND_CAMERA) 
		{
			// Set ground camera position
			state.camControl.position = Vec3f{9.f, 0.f, -3.f};

			// Update camera view to always look at the space vehicle
			Vec3f cameraDirection = normalize(state.spaceVehiclePosition);
			state.camControl.cameraView = cameraDirection;
			
			// Calculate phi and theta values for the camera
			float phi = atan2(cameraDirection.z, cameraDirection.x);
			float theta = asin(-cameraDirection.y);

			// Update ground camera orientation
			state.camControl.phi = phi;
			state.camControl.theta = theta;

			// Set the camera's view direction to point at the spaceship
			// Create the view matrix for the fixed distance camera
			viewMatrix = createViewMatrix(state.camControl.position, state.camControl.cameraView, upVector);
		}

		OGL_CHECKPOINT_DEBUG();

		//----------SPLITSCREEN LOGIC AND LIGHTING IMPLEMENTATION----------
		// Check if splitscreen has been activated
		if (state.splitScreen == true){

			// Left Side of the Screen
			glViewport(0, 0, fbwidth / 2, fbheight);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			// DrawScene(state, projCameraWorld, projCameraWorldPad, projCameraWorldPadsecond, projCameraWorldVehicle);
			OGL_CHECKPOINT_DEBUG();
			// Source input as defined in our VAO
			// Drawing the map
			// We want to draw with our scene
			glUseProgram(state.terrain->programId());
			glBindVertexArray(vao);	

			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorld.v);
			
			glUniformMatrix3fv(
				loc, 
				1, GL_TRUE, normalMatrix.v
			);

			// Textures to match to the map texture file 
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureMap);

			// Lighting uniform values 
			Vec3f lightDir = normalize( Vec3f{ 0.f, 1.f, -1.f } );
			glUniform3fv( 2, 1, &lightDir.x );
			glUniform3f( 3, 0.9f, 0.9f, 0.6f );
			glUniform3f( 4, 0.05f, 0.05f, 0.05f );

			glDrawArrays( GL_TRIANGLES, 0, VertexCount);

			// Reset state
			glBindVertexArray(0);
			glUseProgram(0);
			
			// Drawing the Pad
			glUseProgram(state.pad->programId());

			glBindVertexArray(vaoPad);
			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorldPad.v);
				
			glUniformMatrix3fv(
				locPad, 
				1, GL_TRUE, normalMatrix.v
			);
			glDrawArrays( GL_TRIANGLES, 0, padVertexCount); 

			//Lighting uniform values for first pad
			glUniform3fv( 2, 1, &lightDir.x );
			glUniform3f( 3, 0.9f, 0.9f, 0.6f );
			glUniform3f( 4, 0.05f, 0.05f, 0.05f );

			// Drawing the second Pad
			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorldPadsecond.v);
				
			glUniformMatrix3fv(
				locPadSecond, 
				1, GL_TRUE, normalMatrix.v
			);
			
			//Lighting uniform values for second pad
			Vec3f light1Pad = Vec3f{ 0.f, 1.f, -1.f } ;
			Vec3f light2Pad = Vec3f{-1.0f, 1.0f, 1.0f}; // Light coming from the left and slightly above
			Vec3f light3Pad = Vec3f{1.0f, 0.0f, 1.0f};  // Light coming from the right and slightly in front

			Vec3f lightColor1Pad = {1.0f, 0.95f, 0.9f};  
			Vec3f lightColor2Pad = {1.0f, 0.95f, 0.9f};  
			Vec3f lightColor3Pad = {1.0f, 0.95f, 0.9f}; 

			Vec3f cameraPos1Pad = {0.0f, 0.0f, 10.0f};
			Vec3f cameraPos2Pad  = {10.0f, 0.0f, 0.0f};
			Vec3f cameraPos3Pad  = {1.0f, 0.0f, -10.0f};

			// Positional Light 1
			glUniform3fv( 2, 1, &light1Pad.x); // Light Position 1
			glUniform3f( 3, lightColor1Pad.x,lightColor1Pad.y,lightColor1Pad.z  ); // Light Colour 1
			glUniform3f( 4, cameraPos1Pad.x,cameraPos1Pad.y,cameraPos1Pad.z  ); // Camera Position 1

			// Positional Light 2
			glUniform3fv( 5, 1, &light2Pad.x); // Light Position 2
			glUniform3f( 6, lightColor2Pad.x,lightColor2Pad.y,lightColor2Pad.z  ); // Light Colour 2
			glUniform3f( 7, cameraPos2Pad.x,cameraPos2Pad.y,cameraPos2Pad.z  ); // Camera Position 2

			// Positional Light 3
			glUniform3fv( 8, 1, &light3Pad.x); //Light Position 3 
			glUniform3f( 9, lightColor3Pad.x,lightColor3Pad.y,lightColor3Pad.z  ); //Light Colour 3 
			glUniform3f( 10, cameraPos3Pad.x,cameraPos3Pad.y,cameraPos3Pad.z ); // Camera Position 3
			glUniform3f( 11, 0.2, 0.19, 0.12); // Uscene ambient light

			// Draw a single triangle starting at index 0
			glDrawArrays( GL_TRIANGLES, 0, padVertexCount); 

			// Reset state
			glBindVertexArray(0);
			glUseProgram(0);

			
			//----------DRAWING THE SPACE VEHICLE----------
			glUseProgram(state.spaceVehicle->programId());
			glBindVertexArray(vaoShape);

			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorldVehicle.v);

			glUniformMatrix3fv(
				veh, 
				1, GL_TRUE, normalMatrix.v
			);

			glDrawArrays( GL_TRIANGLES, 0, shapevertexCounts); 

			// Lighting uniform values for space vehicle
			Vec3f light1 = Vec3f{ 0.f, 1.f, -1.f } ;
			Vec3f light2 = Vec3f{-1.0f, 1.0f, 1.0f}; // Light coming from the left and slightly above
			Vec3f light3 = Vec3f{1.0f, 0.0f, 1.0f};  // Light coming from the right and slightly in front

			Vec3f lightColor1 = {0.0f, 3.0f, 0.0f};  
			Vec3f lightColor2 = {3.0f, 0.0f, 0.0f};  
			Vec3f lightColor3 = {0.0f, 0.0f, 3.0f}; 

			Vec3f cameraPos1 = {0.0f, -1.0f, 1.0f};
			Vec3f cameraPos2  = {1.0f, 0.0f, 0.0f};
			Vec3f cameraPos3  = {0.0f, 1.0f, -1.0f};

			// Positional Light 1
			glUniform3fv( 2, 1, &light1.x); // Light Position 1
			glUniform3f( 3, lightColor1.x,lightColor1.y,lightColor1.z  ); // Light Colour 1
			glUniform3f( 4, cameraPos1.x,cameraPos1.y,cameraPos1.z  ); // Camera Position 1

			// Positional Light 2
			glUniform3fv( 5, 1, &light2.x); // Light Position 2
			glUniform3f( 6, lightColor2.x,lightColor2.y,lightColor2.z  ); // Light Colour 2
			glUniform3f( 7, cameraPos2.x,cameraPos2.y,cameraPos2.z  ); // Camera Position 2
 
			// Positional Light 3
			glUniform3fv( 8, 1, &light3.x); // Light Position 3
			glUniform3f( 9, lightColor3.x,lightColor3.y,lightColor3.z  ); // Light Colour 3
			glUniform3f( 10, cameraPos3.x,cameraPos3.y,cameraPos3.z ); // Camera Position 3
			glUniform3f( 11, 0.2, 0.19, 0.18); // Uscene ambient light

			OGL_CHECKPOINT_DEBUG();

			// Display results
			glBindVertexArray(0);
			glUseProgram(0);
			
			// Right Side of the Screen
			glViewport(fbwidth / 2, 0, fbwidth / 2, fbheight);
			
			// DrawScene(state, projCameraWorld, projCameraWorldPad, projCameraWorldPadsecond, projCameraWorldVehicle);
			OGL_CHECKPOINT_DEBUG();

			//--------------------
			//Source input as defined in our VAO
			//Drawing the map

			//We want to draw with our scene
			glUseProgram(state.terrain->programId());

			glBindVertexArray(vao);	
			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorldRight.v);
			
			glUniformMatrix3fv(
				locRight, 
				1, GL_TRUE, normalMatrix.v
			);

			// Textures to match to the map texture file 
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureMap);

			// Lighting uniform values 
			Vec3f lightDirRight = normalize( Vec3f{ 0.f, 1.f, -1.f } );
			glUniform3fv( 2, 1, &lightDirRight.x );
			glUniform3f( 3, 0.9f, 0.9f, 0.6f );
			glUniform3f( 4, 0.05f, 0.05f, 0.05f );

			glDrawArrays( GL_TRIANGLES, 0, VertexCount);

			// Reset state
			glBindVertexArray(0);
			glUseProgram(0);
			
			//----------DRAWING THE PAD----------
			glUseProgram(state.pad->programId());

			glBindVertexArray(vaoPad);
			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorldPadRight.v);
				
			glUniformMatrix3fv(
				locPadRight,
				1, GL_TRUE, normalMatrix.v
			);
			glDrawArrays( GL_TRIANGLES, 0, padVertexCount); 

			// Lighting uniform values for first pad
			glUniform3fv( 2, 1, &lightDirRight.x );
			glUniform3f( 3, 0.9f, 0.9f, 0.6f );
			glUniform3f( 4, 0.05f, 0.05f, 0.05f );

			//----------DRAWING THE SECOND PAD----------
			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorldPadsecondRight.v);
				
			glUniformMatrix3fv(
				locPadSecondRight,
				1, GL_TRUE, normalMatrix.v
			);
			
			// Lighting uniform values for second pad
			Vec3f light1PadRight = Vec3f{ 0.f, 1.f, -1.f } ;
			Vec3f light2PadRight = Vec3f{-1.0f, 1.0f, 1.0f}; // Light coming from the left and slightly above
			Vec3f light3PadRight = Vec3f{1.0f, 0.0f, 1.0f};  // Light coming from the right and slightly in front

			Vec3f lightColor1PadRight = {1.0f, 0.95f, 0.9f};  
			Vec3f lightColor2PadRight = {1.0f, 0.95f, 0.9f};  
			Vec3f lightColor3PadRight = {1.0f, 0.95f, 0.9f}; 

			Vec3f cameraPos1PadRight = {0.0f, 0.0f, 10.0f};
			Vec3f cameraPos2PadRight  = {10.0f, 0.0f, 0.0f};
			Vec3f cameraPos3PadRight  = {1.0f, 0.0f, -10.0f};

			// Positional Light 1
			glUniform3fv( 2, 1, &light1PadRight.x); // Light Position 1
			glUniform3f( 3, lightColor1PadRight.x,lightColor1PadRight.y,lightColor1PadRight.z  ); // Light Colour 1
			glUniform3f( 4, cameraPos1PadRight.x,cameraPos1PadRight.y,cameraPos1PadRight.z  ); // Camera Position 1

			// Positional Light 2
			glUniform3fv( 5, 1, &light2PadRight.x); // Light Position 2
			glUniform3f( 6, lightColor2PadRight.x,lightColor2PadRight.y,lightColor2PadRight.z  ); // Light Colour 2
			glUniform3f( 7, cameraPos2PadRight.x,cameraPos2PadRight.y,cameraPos2PadRight.z  ); // Camera Position 2

			// Positional Light 3
			glUniform3fv( 8, 1, &light3PadRight.x); // Light Position 3
			glUniform3f( 9, lightColor3PadRight.x,lightColor3PadRight.y,lightColor3PadRight.z  ); // Light Colour 3
			glUniform3f( 10, cameraPos3PadRight.x,cameraPos3PadRight.y,cameraPos3PadRight.z ); // Camera Position 3
			glUniform3f( 11, 0.2, 0.19, 0.12); // Uscene ambient light

			//Draw a single triangle starting at index 0
			glDrawArrays( GL_TRIANGLES, 0, padVertexCount); 

			//Reset state
			glBindVertexArray(0);
			glUseProgram(0);
			
			//----------DRAWING THE SPACE VEHICLE----------
			glUseProgram(state.spaceVehicle->programId());

			glBindVertexArray(vaoShape);

			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorldVehicleRight.v);

			glUniformMatrix3fv(
				vehRight, 
				1, GL_TRUE, normalMatrix.v
			);

			glDrawArrays( GL_TRIANGLES, 0, shapevertexCounts); 

			// Lighting uniform values for space vehicle
			Vec3f light1Right = Vec3f{ 0.f, 1.f, -1.f } ;
			Vec3f light2Right = Vec3f{-1.0f, 1.0f, 1.0f}; // Light coming from the left and slightly above
			Vec3f light3Right = Vec3f{1.0f, 0.0f, 1.0f};  // Light coming from the right and slightly in front

			Vec3f lightColor1Right = {0.0f, 3.0f, 0.0f};  
			Vec3f lightColor2Right = {3.0f, 0.0f, 0.0f};  
			Vec3f lightColor3Right = {0.0f, 0.0f, 3.0f}; 

			Vec3f cameraPos1Right = {0.0f, -1.0f, 1.0f};
			Vec3f cameraPos2Right = {1.0f, 0.0f, 0.0f};
			Vec3f cameraPos3Right  = {0.0f, 1.0f, -1.0f};

			// Positional Light 1
			glUniform3fv( 2, 1, &light1Right.x); // Light Position 1
			glUniform3f( 3, lightColor1Right.x,lightColor1Right.y,lightColor1Right.z  ); // Light Colour 1
			glUniform3f( 4, cameraPos1Right.x,cameraPos1Right.y,cameraPos1Right.z  ); // Camera Position 1

			// Positional Light 2
			glUniform3fv( 5, 1, &light2Right.x); // Light Position 2
			glUniform3f( 6, lightColor2Right.x,lightColor2Right.y,lightColor2Right.z  ); // Light Colour 2
			glUniform3f( 7, cameraPos2Right.x,cameraPos2Right.y,cameraPos2Right.z  ); // Camera Position 2

			// Positional Light 3
			glUniform3fv( 8, 1, &light3Right.x); // Light Position 3
			glUniform3f( 9, lightColor3Right.x,lightColor3Right.y,lightColor3Right.z  ); // Light Colour 3
			glUniform3f( 10, cameraPos3Right.x,cameraPos3Right.y,cameraPos3Right.z ); // Camera Position 3
			glUniform3f( 11, 0.2, 0.19, 0.18); // Uscene ambient light

			OGL_CHECKPOINT_DEBUG();

			// Display results
			glBindVertexArray(0);
			glUseProgram(0);
			
		
		}else
		{
			OGL_CHECKPOINT_DEBUG();

			// Source input as defined in our VAO
			// Drawing the map
			// We want to draw with our scene
			glUseProgram(state.terrain->programId());

			glBindVertexArray(vao);	
			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorld.v);
			
			glUniformMatrix3fv(
				loc, 
				1, GL_TRUE, normalMatrix.v
			);

			// Textures to match to the map texture file 
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureMap);

			//Lighting uniform values 
			Vec3f lightDir = normalize( Vec3f{ 0.f, 1.f, -1.f } );
			glUniform3fv( 2, 1, &lightDir.x );
			glUniform3f( 3, 0.9f, 0.9f, 0.6f );
			glUniform3f( 4, 0.05f, 0.05f, 0.05f );

			glDrawArrays( GL_TRIANGLES, 0, VertexCount);

			//Reset state
			glBindVertexArray(0);
			glUseProgram(0);
			
			//----------DRAWING THE PAD----------
			glUseProgram(state.pad->programId());

			glBindVertexArray(vaoPad);
			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorldPad.v);
				
			glUniformMatrix3fv(
				locPad, 
				1, GL_TRUE, normalMatrix.v
			);
			glDrawArrays( GL_TRIANGLES, 0, padVertexCount); 

			//Lighting uniform values for first pad
			glUniform3fv( 2, 1, &lightDir.x );
			glUniform3f( 3, 0.9f, 0.9f, 0.6f );
			glUniform3f( 4, 0.05f, 0.05f, 0.05f );

			//----------DRAWING THE SECOND PAD----------
			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorldPadsecond.v);
				
			glUniformMatrix3fv(
				locPadSecond, 
				1, GL_TRUE, normalMatrix.v
			);
			
			// Lighting uniform values for second pad
			Vec3f light1Pad = Vec3f{ 0.f, 1.f, -1.f } ;
			Vec3f light2Pad = Vec3f{-1.0f, 1.0f, 1.0f}; // Light coming from the left and slightly above
			Vec3f light3Pad = Vec3f{1.0f, 0.0f, 1.0f};  // Light coming from the right and slightly in front

			Vec3f lightColor1Pad = {1.0f, 0.95f, 0.9f};  
			Vec3f lightColor2Pad = {1.0f, 0.95f, 0.9f};  
			Vec3f lightColor3Pad = {1.0f, 0.95f, 0.9f}; 

			Vec3f cameraPos1Pad = {0.0f, 0.0f, 10.0f};
			Vec3f cameraPos2Pad  = {10.0f, 0.0f, 0.0f};
			Vec3f cameraPos3Pad  = {1.0f, 0.0f, -10.0f};

			// Positional Light 1
			glUniform3fv( 2, 1, &light1Pad.x); // Light Position 1
			glUniform3f( 3, lightColor1Pad.x,lightColor1Pad.y,lightColor1Pad.z  ); // Light Colour 1
			glUniform3f( 4, cameraPos1Pad.x,cameraPos1Pad.y,cameraPos1Pad.z  ); // Camera Position 1

			// Positional Light 2
			glUniform3fv( 5, 1, &light2Pad.x); // Light Position 2
			glUniform3f( 6, lightColor2Pad.x,lightColor2Pad.y,lightColor2Pad.z  ); // Light Colour 2
			glUniform3f( 7, cameraPos2Pad.x,cameraPos2Pad.y,cameraPos2Pad.z  ); // Camera Position 2

			// Positional Light 3
			glUniform3fv( 8, 1, &light3Pad.x); // Light Position 3
			glUniform3f( 9, lightColor3Pad.x,lightColor3Pad.y,lightColor3Pad.z  ); // Light Colour 3
			glUniform3f( 10, cameraPos3Pad.x,cameraPos3Pad.y,cameraPos3Pad.z ); // Camera Position 3
			glUniform3f( 11, 0.2, 0.19, 0.12); // Uscene ambient light

			//Draw a single triangle starting at index 0
			glDrawArrays( GL_TRIANGLES, 0, padVertexCount); 

			//Reset state
			glBindVertexArray(0);
			glUseProgram(0);

			//----------DRAWING THE SPACE VEHICLE----------
			glUseProgram(state.spaceVehicle->programId());

			glBindVertexArray(vaoShape);

			glUniformMatrix4fv(
				0,
				1, GL_TRUE, projCameraWorldVehicle.v);

			glUniformMatrix3fv(
				veh, 
				1, GL_TRUE, normalMatrix.v
			);

			glDrawArrays( GL_TRIANGLES, 0, shapevertexCounts); 

			// Lighting uniform values for space vehicle
			Vec3f light1 = Vec3f{ 0.f, 1.f, -1.f } ;
			Vec3f light2 = Vec3f{-1.0f, 1.0f, 1.0f}; // Light coming from the left and slightly above
			Vec3f light3 = Vec3f{1.0f, 0.0f, 1.0f};  // Light coming from the right and slightly in front

			Vec3f lightColor1 = {0.0f, 3.0f, 0.0f};  
			Vec3f lightColor2 = {3.0f, 0.0f, 0.0f};  
			Vec3f lightColor3 = {0.0f, 0.0f, 3.0f}; 

			Vec3f cameraPos1 = {0.0f, -1.0f, 1.0f};
			Vec3f cameraPos2  = {1.0f, 0.0f, 0.0f};
			Vec3f cameraPos3  = {0.0f, 1.0f, -1.0f};

			// Positional Light 1
			glUniform3fv( 2, 1, &light1.x); // Light Position 1
			glUniform3f( 3, lightColor1.x,lightColor1.y,lightColor1.z  ); // Light Colour 1
			glUniform3f( 4, cameraPos1.x,cameraPos1.y,cameraPos1.z  ); // Camera Position 1

			// Positional Light 2
			glUniform3fv( 5, 1, &light2.x); // Light Position 2
			glUniform3f( 6, lightColor2.x,lightColor2.y,lightColor2.z  ); // Light Colour 2
			glUniform3f( 7, cameraPos2.x,cameraPos2.y,cameraPos2.z  ); // Camera Position 2

			// Positional Light 3
			glUniform3fv( 8, 1, &light3.x); // Light Position 3
			glUniform3f( 9, lightColor3.x,lightColor3.y,lightColor3.z  ); // Light Colour 3
			glUniform3f( 10, cameraPos3.x,cameraPos3.y,cameraPos3.z ); // Camera Position 3
			glUniform3f( 11, 0.2, 0.19, 0.18); // Uscene ambient light

			OGL_CHECKPOINT_DEBUG();

			// Display results
			glBindVertexArray(0);
			glUseProgram(0);

		}	
		glfwSwapBuffers(window);
	}
		
	//----------CLEANUP----------
	state.pad = nullptr;
	state.terrain = nullptr;
	state.spaceVehicle = nullptr;
	
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return 0;
}

// Catch any errors
catch( std::exception const& eErr )
{
	std::fprintf( stderr, "Top-level Exception (%s):\n", typeid(eErr).name() ); 
	std::fprintf( stderr, "%s\n", eErr.what() );
	std::fprintf( stderr, "Bye.\n" );
	return 1;
}

namespace
{
	void glfw_callback_error_( int aErrNum, char const* aErrDesc )
	{
		std::fprintf( stderr, "GLFW error: %s (%d)\n", aErrDesc, aErrNum );
	}

	// The following callback function is called whenever a key is pressed, repeated or released
	void glfw_callback_key_( GLFWwindow* aWindow, int aKey, int, int aAction, int )
	{
		// To close the window
		if( GLFW_KEY_ESCAPE == aKey && GLFW_PRESS == aAction )
		{
			glfwSetWindowShouldClose( aWindow, GLFW_TRUE );
			return;
		}
		if( auto* state = static_cast<State_*>(glfwGetWindowUserPointer( aWindow )) )
		{
			// R-key reloads shaders
			// R reloads the terrain, pad, and space vehicle
			// It also reloads the animations 
			if( GLFW_KEY_R == aKey && GLFW_PRESS == aAction )
			{
					try
					{
						state->terrain->reload();
						state->pad->reload();
						state->spaceVehicle->reload();
						state->spaceVehiclePosition = Vec3f{0.f, -0.969504f, -2.5f};
						state->thrust = false;
						state->bezier = 0.0f;
						state->rotateSpaceship = 0.0f;
						state->stopped = false;
						state->fRotation = 0.0f;
						std::fprintf( stderr, "Shaders reloaded and recompiled.\n" );
					}
					catch( std::exception const& eErr )
					{
						std::fprintf( stderr, "Error when reloading shader:\n" );
						std::fprintf( stderr, "%s\n", eErr.what() );
						std::fprintf( stderr, "Keeping old shader.\n" );
					}
			}

			// F is to animate the spaceship
			if (aKey == GLFW_KEY_F && aAction == GLFW_PRESS) {
					state->thrust = true;
			}

			// To split the screen
			if (aKey == GLFW_KEY_V && aAction == GLFW_PRESS){
				if (state->splitScreen){
					state->splitScreen = false;
				}else{
				
					state->splitScreen = true;
				}
			}

			// Changing camera modes when split screen is on
			// This is when the user presses the C key while holding the left shift key
			if (aKey == GLFW_KEY_C && aKey && aAction == GLFW_PRESS && state->splitScreen && glfwGetKey(aWindow,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
				// Control the right side of the screen
				// If in default camera mode then switch to fixed distance camera 
				if (state->cameraMode == DEFAULT_CAMERA) 
					{
					state->camControlRight.phi = fixedCameraPhi;
					state->camControlRight.theta = fixedCameraTheta;
					state->cameraMode = FIXED_DISTANCE_CAMERA;
					} 

					// If in fixed distance camera mode then switch to ground camera
					else if(state->cameraMode == FIXED_DISTANCE_CAMERA)  
					{
						state->cameraMode = GROUND_CAMERA;
					}

					// If in ground camera mode then switch to default mode
					else
					state->cameraMode = DEFAULT_CAMERA;			
				
			}
			
			// For the C key without the left shift key, so just C
			// This changes between the ground and the fixed distance camera and the default camera
			if (aKey != GLFW_KEY_LEFT_SHIFT && aKey == GLFW_KEY_C && aAction == GLFW_PRESS && glfwGetKey(aWindow,GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS) 
			{
				// If in default camera mode then switch to fixed distance camera
				if (state->cameraMode == DEFAULT_CAMERA) 
				{
				state->camControl.phi = fixedCameraPhi;
				state->camControl.theta = fixedCameraTheta;
				state->cameraMode = FIXED_DISTANCE_CAMERA;
				} 

				// If in fixed distance camera mode then switch to ground camera
				else if(state->cameraMode == FIXED_DISTANCE_CAMERA)  
				{
					state->cameraMode = GROUND_CAMERA;
				}

				// If in ground camera mode then switch to fixed distance camera
				else
				state->cameraMode = DEFAULT_CAMERA;
			}

			// Space toggles camera
			if( GLFW_KEY_SPACE == aKey && GLFW_PRESS == aAction )
			{
				state->camControl.cameraActive = !state->camControl.cameraActive;

				if( state->camControl.cameraActive )
					glfwSetInputMode( aWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
				else
					glfwSetInputMode( aWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
			}

			// Camera controls if camera is active
			if( state->camControl.cameraActive )
			{
				// WASDQE Controls
				if( GLFW_KEY_W == aKey )
				{
					if( GLFW_PRESS == aAction )
						state->camControl.actionZoomIn = true;
					else if( GLFW_RELEASE == aAction )
						state->camControl.actionZoomIn = false;
				}

				else if( GLFW_KEY_S == aKey )
				{
					if( GLFW_PRESS == aAction )
						state->camControl.actionZoomOut = true;
					else if( GLFW_RELEASE == aAction )
						state->camControl.actionZoomOut = false;
				}

				else if (GLFW_KEY_A == aKey)
				{
					if( GLFW_PRESS == aAction )
						state->camControl.actionLeft = true;
					else if( GLFW_RELEASE == aAction )
						state->camControl.actionLeft = false;
				}

				else if (GLFW_KEY_D == aKey)
				{
					if( GLFW_PRESS == aAction )
						state->camControl.actionRight = true;
					else if( GLFW_RELEASE == aAction )
						state->camControl.actionRight = false;
				}

				else if (GLFW_KEY_Q == aKey)
				{
					if( GLFW_PRESS == aAction )
						state->camControl.up = true;
					else if( GLFW_RELEASE == aAction )
						state->camControl.up = false;
				}

				else if (GLFW_KEY_E == aKey)
				{
					if( GLFW_PRESS == aAction )
						state->camControl.down = true;
					else if( GLFW_RELEASE == aAction )
						state->camControl.down = false;
				}

				// Shift and Control keys for speeding up and slowing down
				else if (aKey == GLFW_KEY_LEFT_SHIFT || aKey == GLFW_KEY_RIGHT_SHIFT)
				{
					// Only applied as it is held down
					if (aAction == GLFW_PRESS && !state->camControl.speedBoostApplied)
					{
						// Speed boost is true now, logic is in main
						state->camControl.speedBoostApplied = true;
						state->camControl.shift = true;
					}
					else if (aAction == GLFW_RELEASE)
					{
						// Once released, back to normal
						state->camControl.speedBoostApplied = false;
						state->camControl.shift = false;
					}
				}

				else if (aKey == GLFW_KEY_LEFT_CONTROL || aKey == GLFW_KEY_RIGHT_CONTROL)
				{
					// Only applied as it is held down
					if (aAction == GLFW_PRESS && !state->camControl.slowDownApplied)
					{
						// Slow down is true now, logic is in main
						state->camControl.slowDownApplied = true;
						state->camControl.control = true;
					}
					else if (aAction == GLFW_RELEASE)
					{
						// Once released, back to normal
						state->camControl.slowDownApplied = false;
						state->camControl.control = false;
					}
				}
			}
		}
	}

	// Callback function for mouse movement
	void glfw_callback_motion_(GLFWwindow* aWindow, double aX, double aY) {
		if(auto* state = static_cast<State_*>(glfwGetWindowUserPointer(aWindow))) {
			// Check if camera is active and not in fixed mode or ground mode
			// Only in this case the camera controls are applied
			if(state->camControl.cameraActive && (state->cameraMode != FIXED_DISTANCE_CAMERA) && (state->cameraMode != GROUND_CAMERA)) {
				
				// These represent the change in the x and y coordinates of the mouse
				auto const dx = float(aX - state->camControl.lastX);
				auto const dy = float(aY - state->camControl.lastY);

				// Updated based on the mouse movement
				state->camControl.phi += dx * kMouseSensitivity_;
				state->camControl.theta += dy * kMouseSensitivity_;

				// Ensure that the camera doesn't go out of the screen by not exceeding pi/2
				if(state->camControl.theta > kPi_ / 2.f){
					state->camControl.theta = kPi_ / 2.f;
				}
				else if(state->camControl.theta < -kPi_ / 2.f){
					state->camControl.theta = -kPi_ / 2.f;
				}
			}
			// Store the last x and y coordinates
			state->camControl.lastX = float(aX);
			state->camControl.lastY = float(aY);
		}
	}
}

namespace
{
	GLFWCleanupHelper::~GLFWCleanupHelper()
	{
		glfwTerminate();
	}

	GLFWWindowDeleter::~GLFWWindowDeleter()
	{

		if( window )
			glfwDestroyWindow( window );
	}
}

