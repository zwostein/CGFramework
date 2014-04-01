#ifndef PIXELATOR_H_INCLUDED
#define PIXELATOR_H_INCLUDED


#include <GLFW/glfw3.h>

#include <string>

#include "Texture.h"
#include "BitmapFile.hpp"


class Pixelator
{
public:
	Pixelator( unsigned int windowWidth, unsigned int windowHeight, unsigned int canvasWidth = 64, unsigned int canvasHeight = 64 );
	~Pixelator();

	void setPixel( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0xff );
	void clear( unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0xff );
	void wait();   ///< updates window and wait for key pressyPressed
	void update(); ///< updates window - draws canvas and processes window events
	void writeBMP( const std::string & fileName, BitmapFile::PixelFormat pixelFormat = BitmapFile::RGB );

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
