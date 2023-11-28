#include <glad.h>
#include <GLFW/glfw3.h>

#include <typeinfo>
#include <stdexcept>

#include <cstdio>
#include <cstdlib>

#include "defaults.hpp"

#include "../support/error.hpp"
#include "../support/program.hpp"
#include "../support/checkpoint.hpp"
#include "../support/debug_output.hpp"

#include "../vmlib/vec2.hpp"
#include "../vmlib/mat22.hpp"

namespace
{
	constexpr char const* kWindowTitle = "COMP3811 - GL Triangle";

	struct State_
	{
		ShaderProgram* prog;
	};

	void glfw_callback_error_( int, char const* );

	void glfw_callback_key_( GLFWwindow*, int, int, int, int );

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

#	if !defined(__APPLE__)
	// Most platforms will support OpenGL 4.3
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
#	else // defined(__APPLE__)
	// Apple has at most OpenGL 4.1, so don't ask for something newer.
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
#	endif // ~ __APPLE__
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

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
	State_ state{};

	glfwSetWindowUserPointer( window, &state );

	glfwSetKeyCallback( window, &glfw_callback_key_ );

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

	// TODO: global GL setup goes here
	glEnable( GL_FRAMEBUFFER_SRGB );
	glEnable( GL_CULL_FACE ); 
	glClearColor( 0.2f, 0.2f, 0.2f, 0.0f );

	OGL_CHECKPOINT_ALWAYS();

	// Get actual framebuffer size.
	// This can be different from the window size, as standard window
	// decorations (title bar, borders, ...) may be included in the window size
	// but not be part of the drawable surface area.
	int iwidth, iheight;
	glfwGetFramebufferSize( window, &iwidth, &iheight );

	glViewport( 0, 0, iwidth, iheight );

	// Load shader program
	ShaderProgram prog( {
		{ GL_VERTEX_SHADER, "assets/passthrough.vert" },
		{ GL_FRAGMENT_SHADER, "assets/modulate.frag" }
	} );

	state.prog = &prog;

	// Create vertex buffers and VAO

	static float const kPositions[] = { 
		0.f, 0.8f, // vertex 0 position 2
		-0.7f, -0.8f, // vertex 1 position 3
		+0.7f, -0.8f // vertex 2 position 4
	}; 

	GLuint positionVBO = 0; 
	glGenBuffers( 1, &positionVBO ); 
	glBindBuffer( GL_ARRAY_BUFFER, positionVBO ); 
	glBufferData( GL_ARRAY_BUFFER, sizeof(kPositions), kPositions, GL_STATIC_DRAW ); 

// Note: we can use sizeof(kPositions) because kPositions is defined as a C/C++ array. Never use sizeof() on pointers or on classes such as std::vector! 

	static float const kColors[] = { 
	1.f, 1.f, 0.f,// vertex 0 color 2
	1.f, 0.f, 1.f,// vertex 1 color 3
	0.f, 1.f, 1.f
	};

	//TODO: create VBOs and VAO
	GLuint colorVBO = 0; 
	glGenBuffers( 1, &colorVBO); 
	glBindBuffer( GL_ARRAY_BUFFER, colorVBO ); 
	glBufferData( GL_ARRAY_BUFFER, sizeof(kColors), kColors, GL_STATIC_DRAW ); 

	GLuint vao = 0; 
	glGenVertexArrays( 1, &vao ); 
	glBindVertexArray( vao ); 
	glBindBuffer( GL_ARRAY_BUFFER, positionVBO ); 
	glVertexAttribPointer( 
	0, // location = 0 in vertex shader 
	2, GL_FLOAT, GL_FALSE, // 2 floats, not normalized to [0..1] (GL FALSE) 
	0, // stride = 0 indicates that there is no padding between inputs 
	0 // data starts at offset 0 in the VBO. 
	); 
	glEnableVertexAttribArray( 0 ); 

	glBindBuffer( GL_ARRAY_BUFFER, colorVBO ); 
	glVertexAttribPointer( 
	1, // location = 1 in vertex shader 
	3, GL_FLOAT, GL_FALSE, // 3 floats, not normalized to [0..1] (GL FALSE) 
	0, // see above 
	0 // see above 
	);
	glEnableVertexAttribArray( 1 ); 

	// Reset state
	glBindVertexArray( 0 ); 
	glBindBuffer( GL_ARRAY_BUFFER, 0 ); 

	// Clean up buffers 
	// Note: these are not deleted fully, as the VAO holds a reference to them. 
	glDeleteBuffers( 1, &colorVBO ); 
	glDeleteBuffers( 1, &positionVBO ); 

	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	
	// Animation state
	auto last = Clock::now();

	float angle = 0.f;

	// Main loop
	while( !glfwWindowShouldClose( window ) )
	{
		// Let GLFW process events
		glfwPollEvents();
		
		// Check if window was resized.
		{
			int nwidth, nheight;
			glfwGetFramebufferSize( window, &nwidth, &nheight );


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
		auto const now = Clock::now();
		float dt = std::chrono::duration_cast<Secondsf>(now-last).count();
		last = now;

		static constexpr float kPi_ = 3.1415926f; // this is me being lazy.
		angle += dt * kPi_ * 0.3f;
		if( angle >= 2.f*kPi_ )
			angle -= 2.f*kPi_;

		// Draw scene
		OGL_CHECKPOINT_DEBUG();

		//TODO: draw frame
		// Clear color buffer to specified clear color (glClearColor())
		glClear(GL_COLOR_BUFFER_BIT);
		//We want to draw with our program
		glUseProgram(prog.programId());

		// Specify the base color (uBaseColor in location 0 in the fragment shader) 
		static float const baseColor[] = { 0.2f, 1.f, 1.f }; 
		glUniform3fv( 0,
		//∗ l o c a t i o n in s h a de r s ∗/, 
		1, baseColor );

		//Source input as defined in our VAO
		glBindVertexArray(vao);

		//Draw a single triangle starting at index 0
		glDrawArrays( GL_TRIANGLES, 0, 3 );

		//Reset state
		glBindVertexArray(0);
		glUseProgram(0);


		OGL_CHECKPOINT_DEBUG();

		// Display results
		glfwSwapBuffers( window );
	}

	// Cleanup.
	state.prog = nullptr;

	//TODO: additional cleanup
	
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
				if( state->prog )
				{
					try
					{
						state->prog->reload();
						std::fprintf( stderr, "Shaders reloaded and recompiled.\n" );
					}
					catch( std::exception const& eErr )
					{
						std::fprintf( stderr, "Error when reloading shader:\n" );
						std::fprintf( stderr, "%s\n", eErr.what() );
						std::fprintf( stderr, "Keeping old shader.\n" );
					}
				}
			}
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

