#include "Texture.h"

#include <GLFW/glfw3.h>
#include <stdlib.h>


Texture::Texture( unsigned int width, unsigned int height ) : id(0), buffer(NULL), width(0), height(0)
{
	this->create( width, height );
}


Texture::~Texture()
{
	this->free();
}


void Texture::free()
{
	if( this->buffer )
		delete[] this->buffer;
	if( this->id )
		glDeleteTextures( 1, &this->id );
	this->id = 0;
	this->buffer = NULL;
	width = 0;
	height = 0;
}


void Texture::create( unsigned int width, unsigned int height )
{
	this->buffer = new unsigned char[ width * height * 4 * sizeof( unsigned char ) ];
	this->height = height;
	this->width = width;

	glGenTextures( 1, &this->id );
	glBindTexture( GL_TEXTURE_2D, this->id );
	glTexImage2D( GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->buffer );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
}


void Texture::resize( unsigned int width, unsigned int height )
{
	this->free();
	this->create( width, height );
}


void Texture::update()
{
	glBindTexture( GL_TEXTURE_2D, this->id );
	glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, this->width, this->height, GL_RGBA, GL_UNSIGNED_BYTE, this->buffer );
}


void Texture::setPixel( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a )
{
	// clipping
	if( x >= this->width || y >= this->height )
		return;

	// set pixel at position x, y
	unsigned int index = 4 * ( x + this->width * y );
	this->buffer[ index ] = r;
	this->buffer[ index + 1 ] = g;
	this->buffer[ index + 2 ] = b;
	this->buffer[ index + 3 ] = a;
}


void Texture::clear( unsigned char r, unsigned char g, unsigned char b, unsigned char a )
{
	for( unsigned int y = 0; y < height; y++ )
	{
		for( unsigned int x = 0; x < width; x++ )
		{
			setPixel( x, y, r, g, b, a );
		}
	}
}
