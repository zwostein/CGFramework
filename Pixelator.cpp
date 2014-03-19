#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "Pixelator.h"
#include "Texture.h"

#include <iostream>
#include <stdexcept>
#include <stdlib.h>


#ifdef _WIN32
#include <windows.h>
void usleep( unsigned int usec )
{
	Sleep( usec/1000 );
}
#endif


bool Pixelator::anyKeyPressed = false;


void Pixelator::GLFWErrorCallback( int error, const char * description )
{
	std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}


void Pixelator::GLFWKeyCallback( GLFWwindow * window, int key, int scancode, int action, int mods )
{
	if( action == GLFW_PRESS )
		anyKeyPressed = true;
}


Pixelator::Pixelator( unsigned int windowWidth, unsigned int windowHeight, unsigned int canvasWidth, unsigned int canvasHeight )
{
	// initialize GLFW
	glfwSetErrorCallback( GLFWErrorCallback );
	if( !glfwInit() )
	{
		throw std::runtime_error( "Failed to initialize GLFW" );
	}

	// create a window
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
	window = glfwCreateWindow( windowWidth, windowHeight, "CGFramework", NULL, NULL );
	if( !window )
	{
		glfwTerminate();
		throw std::runtime_error( "Failed to create GLFW window" );
	}

	// switch to the window's GL context
	// after this call you can use GL functions until the window is destroyed
	glfwMakeContextCurrent( window );

	// enable vsync
	glfwSwapInterval( 1 );

	// create a new texture on the GPU
	texture = new Texture( canvasWidth, canvasHeight );
	texture->clear( 0, 0, 0 );
}


Pixelator::~Pixelator()
{
	delete texture;
	glfwDestroyWindow( window );
	glfwTerminate();
}


void Pixelator::draw()
{
	// set viewport to current framebuffer size
	int width = 0, height = 0;
	glfwGetFramebufferSize( window, &width, &height );
	glViewport( 0, 0, width, height );

	// clear background
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );

	// setup projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( 0.0, 1.0, 0.0, 1.0 );

	// setup modelview matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// draw a fullscreen quad using the texture
	glEnable( GL_TEXTURE_2D );
	texture->bind();
	glBegin( GL_QUADS );
		glTexCoord2f( 0.0f, 0.0f ); glVertex2f( 0.0f, 0.0f );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2f( 1.0f, 0.0f );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2f( 1.0f, 1.0f );
		glTexCoord2f( 0.0f, 1.0f ); glVertex2f( 0.0f, 1.0f );
	glEnd();
}


void Pixelator::clear( unsigned char r, unsigned char g, unsigned char b, unsigned char a )
{
	texture->clear( r, g, b, a );
}


void Pixelator::setPixel( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a )
{
	texture->setPixel( x, y, r, g, b, a );
}


void Pixelator::wait()
{
	// setup key callback to detect any key press
	anyKeyPressed = false;
	GLFWkeyfun oldKeyCallback = glfwSetKeyCallback( window, GLFWKeyCallback );

	// update window and keep updating while the window should not close and no key is pressed
	do {
		update();
		usleep( 1000 );
	} while( !anyKeyPressed && !glfwWindowShouldClose( window ) );

	// restore previous key callback
	glfwSetKeyCallback( window, oldKeyCallback );
	anyKeyPressed = false;
}


void Pixelator::update()
{
	// upload texture to GPU
	texture->update();

	// draw texture to window
	draw();
	glfwSwapBuffers( window );

	// report GL errors
	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
		std::cerr << "GL Error " << error << ": " << gluErrorString( error ) << std::endl;

	// process window events
	glfwPollEvents();
}
