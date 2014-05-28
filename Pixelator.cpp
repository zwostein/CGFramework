#ifdef _WIN32
	#include <windows.h>
	void usleep( unsigned int usec )
	{
		Sleep( usec/1000 );
	}
#else
	#include <unistd.h>
#endif

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "Pixelator.h"
#include "Texture.h"
#include "BitmapFile.hpp"

#include <string>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>


bool Pixelator::anyKeyPressed = false;


void Pixelator::GLFWErrorCallback( int error, const char * description )
{
	std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}


void Pixelator::GLFWKeyCallback( GLFWwindow * window, int key, int scancode, int action, int mods )
{
	(void)window;
	(void)key;
	(void)scancode;
	(void)mods;
	if( action == GLFW_PRESS )
		anyKeyPressed = true;
}


Pixelator::Pixelator( unsigned int windowWidth, unsigned int windowHeight, unsigned int canvasWidth, unsigned int canvasHeight ) : ImageBuffer( canvasWidth, canvasHeight )
{
#ifndef HEADLESS
	// initialize GLFW
	glfwSetErrorCallback( GLFWErrorCallback );
	if( !glfwInit() )
	{
		throw std::runtime_error( "Failed to initialize GLFW" );
	}

	// create a window
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
	this->window = glfwCreateWindow( windowWidth, windowHeight, "CGFramework", NULL, NULL );
	if( !this->window )
	{
		glfwTerminate();
		throw std::runtime_error( "Failed to create GLFW window" );
	}

	// switch to the window's GL context
	// after this call you can use GL functions until the window is destroyed
	glfwMakeContextCurrent( this->window );

	// enable vsync
	glfwSwapInterval( 1 );
#endif

	// set image to black initially
	this->clear( 0, 0, 0 );

	// create a new texture on the GPU
	this->texture = new Texture( *this );
}


Pixelator::~Pixelator()
{
	delete this->texture;
#ifndef HEADLESS
	glfwDestroyWindow( this->window );
	glfwTerminate();
#endif
}


void Pixelator::draw()
{
#ifndef HEADLESS
	// set viewport to current framebuffer size
	int width = 0, height = 0;
	glfwGetFramebufferSize( this->window, &width, &height );
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
#endif
}


void Pixelator::wait()
{
#ifndef HEADLESS
	// setup key callback to detect any key press
	this->anyKeyPressed = false;
	GLFWkeyfun oldKeyCallback = glfwSetKeyCallback( this->window, GLFWKeyCallback );

	// update window and keep updating while the window should not close and no key is pressed
	do {
		update();
		usleep( 1000 );
	} while( !anyKeyPressed && !glfwWindowShouldClose( this->window ) );

	// restore previous key callback
	glfwSetKeyCallback( this->window, oldKeyCallback );
	this->anyKeyPressed = false;
#endif
}


void Pixelator::update()
{
#ifndef HEADLESS
	// upload texture to GPU
	this->texture->update( *this );

	// draw texture to window
	this->draw();
	glfwSwapBuffers( this->window );

	// report GL errors
	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
		std::cerr << "GL Error " << error << ": " << gluErrorString( error ) << std::endl;

	// process window events
	glfwPollEvents();
#endif
}


void Pixelator::writeBMP( const std::string & fileName, BitmapFile::PixelFormat pixelFormat )
{
	BitmapFile::write( *this, fileName, pixelFormat );
}


void Pixelator::readBMP( const std::string & fileName )
{
	ImageBuffer * newImageBuffer = BitmapFile::load( fileName );
	if( !newImageBuffer )
		return;
	*this = *newImageBuffer;
	delete newImageBuffer;
}
