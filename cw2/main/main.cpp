#include <glad.h>
#include <GLFW/glfw3.h>


#include <typeinfo>
#include <stdexcept>

#include <cstdio>
#include <cstdlib>

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

	constexpr float kMovementPerSecond_ = 5.f; // units per second
	constexpr float kMouseSensitivity_ = 0.01f; // radians per pixel

	struct State_
	{
		ShaderProgram* terrain;// Shader program for the map
		ShaderProgram* pad; //Shader Program for the pad  

		struct CamCtrl_
		{

			//added variables for wasd 
			bool cameraActive;
			bool actionZoomIn, actionZoomOut;
			bool actionLeft, actionRight;
			bool up, down;
			bool shift = false; // state for Shift key
			bool control = false; // state for control key
			bool speedBoostApplied = false; // speed up
			bool slowDownApplied = false; // slow down 

			Vec3f position;
			// phi is rotation around the y-axis 
			// theta is rotation around the x-axis
			float phi, theta;
			float radius;
			float lastX, lastY;

			Vec3f cameraView; // Camera's front view so that the controls are relative to the way we're looking
		} camControl;
	};
	
	
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
	// Initialize GLFW
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

	//glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	glfwWindowHint( GLFW_DEPTH_BITS, 24 );

#	if !defined(NDEBUG)
	// When building in debug mode, request an OpenGL debug context. This
	// enables additional debugging features. However, this can carry extra
	// overheads. We therefore do not do this for release builds.
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#	endif // ~ !NDEBUG

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

	// Set up event handling
	// TODO: Additional event handling setup
	State_ state{};

	glfwSetWindowUserPointer( window, &state );

	glfwSetKeyCallback( window, &glfw_callback_key_ );
	glfwSetCursorPosCallback( window, &glfw_callback_motion_ );

	// Set up drawing stuff
	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 ); // V-Sync is on.

	// Initialize GLAD
	// This will load the OpenGL API. We mustn't make any OpenGL calls before this!
	if( !gladLoadGLLoader( (GLADloadproc)&glfwGetProcAddress ) )
		throw Error( "gladLoaDGLLoader() failed - cannot load GL API!" );

	std::printf( "RENDERER %s\n", glGetString( GL_RENDERER ) );
	std::printf( "VENDOR %s\n", glGetString( GL_VENDOR ) );
	std::printf( "VERSION %s\n", glGetString( GL_VERSION ) );
	std::printf( "SHADING_LANGUAGE_VERSION %s\n", glGetString( GL_SHADING_LANGUAGE_VERSION ) );

	// Ddebug output
#	if !defined(NDEBUG)
	setup_gl_debug_output();
#	endif // ~ !NDEBUG

	// Global GL state
	OGL_CHECKPOINT_ALWAYS();

	glEnable(GL_FRAMEBUFFER_SRGB);

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	// glEnable(GL_CULL_FACE);
	glEnable( GL_DEPTH_TEST );
	// TODO: global GL setup goes here

	OGL_CHECKPOINT_ALWAYS();

	// Get actual framebuffer size.
	// This can be different from the window size, as standard window
	// decorations (title bar, borders, ...) may be included in the window size
	// but not be part of the drawable surface area.
	int iwidth, iheight;
	glfwGetFramebufferSize( window, &iwidth, &iheight );

	glViewport( 0, 0, iwidth, iheight );
	// Load shader program
	ShaderProgram terrain( {
		{ GL_VERTEX_SHADER, "assets/default.vert" },
		{ GL_FRAGMENT_SHADER, "assets/default.frag" }
	} );
	state.terrain = &terrain;
	state.camControl.radius = 10.f;

	ShaderProgram pad( {
		{ GL_VERTEX_SHADER, "assets/pad.vert" },
		{ GL_FRAGMENT_SHADER, "assets/pad.frag" }
	} );
	state.pad = &pad;

	// Animation state
	auto last = Clock::now();
	float angle = 0.f;

	//Loading in map
	// ------------------
	SimpleMeshData map = load_wavefront_obj("assets/parlahti.obj");
	GLuint vao = create_vao( map );
	std::size_t VertexCount = map.positions.size();

	//Loading in the texture
	// ------------------
	GLuint textureMap = load_texture_world("assets/L4343A-4k.jpeg");
	
	//Loading in LandingPad
	// ------------------
	SimpleMeshDataNoTexture Launchpad = load_noTexture("assets/landingpad.obj");
	GLuint vaoPad = create_novaoTexture( Launchpad );
	std::size_t padVertexCount = Launchpad.positions.size();

	state.camControl.position = {0.0f, 0.0f, 0.0f}; // Set initial position
	float speed = kMovementPerSecond_; // Set initial speed
	float rotationAngleRadians = 3.1415926f / 2.0f; // kPi_ should be defined as 3.1415926f (π)
		
	// Make 3D Model of a space vehicle

	// ------------------ Cone on top
	SimpleMeshDataNoTexture xcone = make_cone(true, 16, {0.f, 0.f, 0.f}, 
					make_scaling(1.f, 0.3f, 0.3f) * make_translation({5.f, 0.f, 0.f}));

	// ------------------ Base Cylinder
	// Create the cylinder, apply a rotation to make it vertical, and then scale it.
	SimpleMeshDataNoTexture firstCyl = make_cylinder(
		true, 16, {1.f, 0.f, 0.f},  make_scaling(0.25f, 0.5f, 0.25f)* make_translation({0.f, 0.25f, 0.f}) * make_rotation_z(3.1415926f / 2) );
	
	// ------------------ Middle Cylinder
	SimpleMeshDataNoTexture midCyl = make_cylinder(
		true, 16, {1.f, 0.f, 0.f},  make_scaling(0.25f, 0.5f, 0.25f)* make_translation({0.f, 0.25f, 0.f}) * make_rotation_z(3.1415926f / 2) );
		
	// Make a cube model
	SimpleMeshDataNoTexture cube = make_cube(1, {0.f, 0.f, 0.f}, make_scaling(0.1f, 0.1f, 0.1f) * make_translation({0.f, 2.f, 0.f}));

	auto xarrow = concatenate(std::move(firstCyl), xcone);
	GLuint vaoShape = create_novaoTexture(xarrow); 
	std::size_t shapevertexCounts = xarrow.positions.size();

	GLuint vaoCube = create_novaoTexture(cube);
	std::size_t shapevertexCounts2 = cube.positions.size();

		


	// Other initialization & loading
	OGL_CHECKPOINT_ALWAYS();
	
	// TODO: global GL setup goes here

	OGL_CHECKPOINT_ALWAYS();

	// Main loop
	while( !glfwWindowShouldClose( window ) )
	{
		// Let GLFW process events
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Only once at the beginning of the render loop

		
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
				// This is a bit of a hack.
				do
				{
					glfwWaitEvents();
					glfwGetFramebufferSize( window, &nwidth, &nheight );
				} while( 0 == nwidth || 0 == nheight );
			}

			glViewport( 0, 0, nwidth, nheight );
		}

		// Update state
		//TODO: update state
		auto const now = Clock::now();
		float dt = std::chrono::duration_cast<Secondsf>(now-last).count();
		last = now;


		angle += dt * kPi_ * 0.3f;
		if( angle >= 2.f*kPi_ )
			angle -= 2.f*kPi_;

		// Update camera state
		// PART ADDED FOR WASD  
		// use the logic from the following [https://learnopengl.com/Getting-started/Camera]
		// replacing glm for the phi and theta values
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

		if (state.camControl.shift && state.camControl.speedBoostApplied)
		{
			speed = kMovementPerSecond_ * 2.0f; // Doubling the speed when shift is clicked
		}
		else if(state.camControl.control && state.camControl.slowDownApplied){
			speed = kMovementPerSecond_ * 0.5f; // Halfing the speed when control is clicked
		}
		else
		{
			speed = kMovementPerSecond_;
		}

		//A Keyboard input, moving left
		if(state.camControl.actionLeft) {			
			state.camControl.position -= state.camControl.cameraView * speed * dt;
		}

		//D Keyboard input, moving right 
		if(state.camControl.actionRight) {
			state.camControl.position += state.camControl.cameraView * speed * dt;
		}

		// S Keyboard input, moving backwards
		if(state.camControl.actionZoomOut) {			
			state.camControl.position -= camNormalised * speed * dt;
		}

		// W Keyboard input Forward movement
		if(state.camControl.actionZoomIn) {
			state.camControl.position += camNormalised * speed * dt;
		}

		// E Keyboard input, moving up 
		if(state.camControl.up) {
			state.camControl.position.y += speed * dt;
		}

		// Q Keyboard input, moving down
		if(state.camControl.down) {
			state.camControl.position.y -= speed * dt;
		}

		// Update: compute matrices
		//TODO: define and compute projCameraWorld matrix
		// Create cameraworld on map
		Mat33f normalMatrix = mat44_to_mat33(transpose(invert(kIdentity44f)));
		// rotation = 0 so that the terrain isn't rotating when the camera moves around
		Mat44f staticTerrain = make_rotation_y(0);
		Mat44f Rx = make_rotation_x( state.camControl.theta );
		Mat44f Ry = make_rotation_y( state.camControl.phi );
		Mat44f T = make_translation( -state.camControl.position );
		Mat44f world2camera = Ry * Rx * T;
		Mat44f projection = make_perspective_projection(
			60.f * 3.1415926f / 180.f, // Yes, a proper π would be useful. ( C++20: mathematical constants) 2
			fbwidth/float(fbheight),
			0.1f, 100.0f);
		Mat44f projCameraWorld = projection * world2camera * staticTerrain;

		// Create cameraworld on first pad
		// Only need to update the model2world to move the pad around 
		Mat44f model2worldPad = make_translation( {0.0f, -0.97300, 20.0f} );
		Mat44f projCameraWorldPad = projection * world2camera * model2worldPad;

		//Create cameraWorld on second pad 
		Mat44f model2worldPadsecond = make_translation( {10.0f, -0.969504, -23.0f} );
		Mat44f projCameraWorldPadsecond = projection * world2camera * model2worldPadsecond;

		// // ProjCameraWorld for shapes
		// Mat44f model2worldPadsecond = make_translation( {10.0f, -0.969504, -23.0f} );
		// Mat44f projCameraWorldPadsecond = projection * world2camera * model2worldPadsecond;
		
		// Draw scene
		OGL_CHECKPOINT_DEBUG();

		// Clear color buffer to specified clear color (glClearColor())
		glClear(GL_COLOR_BUFFER_BIT);

		
		OGL_CHECKPOINT_DEBUG();

		//Source input as defined in our VAO
		//Drawing the map
		// --------------
		//We want to draw with our scene
		glUseProgram(state.terrain->programId());

		glBindVertexArray(vao);	
		glUniformMatrix4fv(
			0,
			1, GL_TRUE, projCameraWorld.v);
		GLuint loc = glGetUniformLocation(state.terrain->programId(), "uNormalMatrix");
		glUniformMatrix3fv(
			loc, // make sure this matches the location = N in the vertex shader!
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
		
		//Drawing the Pad
		// -------------
		glUseProgram(state.pad->programId());

		glBindVertexArray(vaoPad);
		glUniformMatrix4fv(
			0,
			1, GL_TRUE, projCameraWorldPad.v);
			
		GLuint locPad = glGetUniformLocation(state.pad->programId(), "uNormalMatrix");
		glUniformMatrix3fv(
			locPad, // make sure this matches the location = N in the vertex shader!
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
			GLuint locPadSecond = glGetUniformLocation(state.pad->programId(), "uNormalMatrix");
		glUniformMatrix3fv(
			locPadSecond, // make sure this matches the location = N in the vertex shader!
			1, GL_TRUE, normalMatrix.v
		);
		//Lighting uniform values for second pad
		glUniform3fv( 2, 1, &lightDir.x );
		glUniform3f( 3, 0.9f, 0.9f, 0.6f );
		glUniform3f( 4, 0.05f, 0.05f, 0.05f );

		//Draw a single triangle starting at index 0
		glDrawArrays( GL_TRIANGLES, 0, padVertexCount); 

		//Reset state
		glBindVertexArray(0);
		glUseProgram(0);
		
		//Drawing the 3D shapes
		// -------------
		glUseProgram(state.pad->programId());

		glBindVertexArray(vaoShape);
		glUniformMatrix4fv(
			0,
			1, GL_TRUE, projCameraWorldPad.v);
			
		GLuint locBase = glGetUniformLocation(state.pad->programId(), "uNormalMatrix");
		glUniformMatrix3fv(
			locBase, // make sure this matches the location = N in the vertex shader!
			1, GL_TRUE, normalMatrix.v
		);
		glDrawArrays( GL_TRIANGLES, 0, shapevertexCounts); 

		// //Lighting uniform values for first pad
		// glUniform3fv( 2, 1, &lightDir.x );
		// glUniform3f( 3, 0.9f, 0.9f, 0.6f );
		// glUniform3f( 4, 0.05f, 0.05f, 0.05f );
		
		OGL_CHECKPOINT_DEBUG();

		// Display results
		glfwSwapBuffers( window );
		glBindVertexArray(0);
		glUseProgram(0);
		
	}

	// Cleanup.
	state.pad = nullptr;
	state.terrain = nullptr;

	

	//TODO: additional cleanup

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return 0;
}
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

	void glfw_callback_key_( GLFWwindow* aWindow, int aKey, int, int aAction, int )
	{
		if( GLFW_KEY_ESCAPE == aKey && GLFW_PRESS == aAction )
		{
			glfwSetWindowShouldClose( aWindow, GLFW_TRUE );
			return;
		}

		if( auto* state = static_cast<State_*>(glfwGetWindowUserPointer( aWindow )) )
		{
			// R-key reloads shaders.
			if( GLFW_KEY_R == aKey && GLFW_PRESS == aAction )
			{
				
					try
					{
						state->terrain->reload();
						state->pad->reload();


						std::fprintf( stderr, "Shaders reloaded and recompiled.\n" );
					}
					catch( std::exception const& eErr )
					{
						std::fprintf( stderr, "Error when reloading shader:\n" );
						std::fprintf( stderr, "%s\n", eErr.what() );
						std::fprintf( stderr, "Keeping old shader.\n" );
					}
				
			}

			switch (aKey) {
				case GLFW_KEY_A:
					state->camControl.actionLeft = (aAction != GLFW_RELEASE);
					break;
				case GLFW_KEY_D:
					state->camControl.actionRight = (aAction != GLFW_RELEASE);
					break;
				case GLFW_KEY_Q:
					state->camControl.up = (aAction != GLFW_RELEASE);
					break;
				case GLFW_KEY_E:
					state->camControl.down = (aAction != GLFW_RELEASE);
					break;

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
				else if (aKey == GLFW_KEY_LEFT_SHIFT || aKey == GLFW_KEY_RIGHT_SHIFT)
				{
					if (aAction == GLFW_PRESS && !state->camControl.speedBoostApplied)
					{
						state->camControl.speedBoostApplied = true;
						state->camControl.shift = true;
					}
					else if (aAction == GLFW_RELEASE)
					{
						state->camControl.speedBoostApplied = false;
						state->camControl.shift = false;
					}
				}

				else if (aKey == GLFW_KEY_LEFT_CONTROL || aKey == GLFW_KEY_RIGHT_CONTROL)
				{
					if (aAction == GLFW_PRESS && !state->camControl.slowDownApplied)
					{
						state->camControl.slowDownApplied = true;
						state->camControl.control = true;
					}
					else if (aAction == GLFW_RELEASE)
					{
						state->camControl.slowDownApplied = false;
						state->camControl.control = false;
					}
				}
			}
		}
	}

	void glfw_callback_motion_( GLFWwindow* aWindow, double aX, double aY )
	{
		if( auto* state = static_cast<State_*>(glfwGetWindowUserPointer( aWindow )) )
		{
			if( state->camControl.cameraActive )
			{
				auto const dx = float(aX-state->camControl.lastX);
				auto const dy = float(aY-state->camControl.lastY);

				state->camControl.phi += dx*kMouseSensitivity_;
				
				state->camControl.theta += dy*kMouseSensitivity_;
				if( state->camControl.theta > kPi_/2.f )
					state->camControl.theta = kPi_/2.f;
				else if( state->camControl.theta < -kPi_/2.f )
					state->camControl.theta = -kPi_/2.f;
			}

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

