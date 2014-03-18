// GLEW documentation:
// http://www.glfw.org/docs/latest/modules.html
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdlib.h>

#include "Texture.h"


static GLFWwindow * window;
static Texture * texture;


static void drawTexture()
{
	if( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_1 ) == GLFW_PRESS )
	{
		// fetch cursor position on texture
		double cursorX = 0.0, cursorY = 0.0;
		glfwGetCursorPos( window, &cursorX, &cursorY );
		int width = 0, height = 0;
		glfwGetWindowSize( window, &width, &height );
		cursorY = (double)height - cursorY; // set origin to lower left corner instead of upper left window corner
		unsigned int cursorOnTextureX = ((double)cursorX/(double)width) * (double)texture->getWidth();
		unsigned int cursorOnTextureY = ((double)cursorY/(double)height) * (double)texture->getHeight();

		// set pixel at cursor position to a random color
		texture->setPixel( cursorOnTextureX, cursorOnTextureY, rand()%256, rand()%256, rand()%256 );
	}
	else if( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_2 ) == GLFW_PRESS )
	{
		texture->clear( 0, 0, 0 );
	}
}


static void draw()
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

	// draw something to the texture and upload it to the GPU
	drawTexture();
	texture->update();

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


static void GLFWErrorCallback( int error, const char * description )
{
	std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}


int main( int argc, char ** argv )
{
	// initialize GLFW
	glfwSetErrorCallback( GLFWErrorCallback );
	if( !glfwInit() )
	{
		std::cerr << "Failed to initialize GLFW\n";
		return EXIT_FAILURE;
	}

	// create a window
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
	window = glfwCreateWindow( 512, 512, "CGFramework", NULL, NULL );
	if( !window )
	{
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// switch to the window's GL context
	// after this call you can use GL functions until the window is destroyed
	glfwMakeContextCurrent( window );

	// enable vsync
	glfwSwapInterval( 1 );

	// create a new texture on the GPU
	texture = new Texture( 64, 64 );
	texture->clear( 0, 0, 0 );

	// the main loop
	while( !glfwWindowShouldClose( window ) )
	{
		// draw something to the window
		draw();
		glfwSwapBuffers( window );

		// report GL errors
		GLenum error = glGetError();
		if( error != GL_NO_ERROR )
			std::cerr << "GL Error " << error << ": " << gluErrorString( error ) << std::endl;

		// process window events
		glfwPollEvents();
	}

	// free resources and exit
	delete texture;
	glfwDestroyWindow( window );
	glfwTerminate();
	return EXIT_SUCCESS;
}
