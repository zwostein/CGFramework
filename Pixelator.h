#ifndef PIXELATOR_H_INCLUDED
#define PIXELATOR_H_INCLUDED


#include <GLFW/glfw3.h>

#include <string>

#include "ImageBuffer.h"
#include "Texture.h"
#include "BitmapFile.hpp"


class Pixelator : public ImageBuffer
{
public:
	Pixelator( unsigned int windowWidth, unsigned int windowHeight, unsigned int canvasWidth, unsigned int canvasHeight );
	~Pixelator();

	Pixelator & operator=( const ImageBuffer & other ) { ImageBuffer::operator=(other); return *this; }

	void wait();   ///< updates window and wait for key pressyPressed
	void update(); ///< updates window - draws canvas and processes window events
	void writeBMP( const std::string & fileName, BitmapFile::PixelFormat pixelFormat = BitmapFile::RGB );
	void readBMP( const std::string & fileName );

	unsigned int getCanvasWidth() const { return texture->getWidth(); }
	unsigned int getCanvasHeight() const { return texture->getHeight(); }

	GLFWwindow * getWindow() { return window; }

private:
	static void GLFWErrorCallback( int error, const char * description );
	static void GLFWKeyCallback( GLFWwindow * window, int key, int scancode, int action, int mods );

	static bool anyKeyPressed;

	void draw();
	bool waitForKeyboard();

	GLFWwindow * window;
	Texture * texture;
};


#endif
