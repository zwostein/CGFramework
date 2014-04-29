#include "Texture.h"

#include <GLFW/glfw3.h>


Texture::Texture() : id(0), width(0), height(0)
{
}


Texture::Texture( const ImageBuffer & imageBuffer ) : id(0), width(0), height(0)
{
	this->update( imageBuffer );
}


Texture::~Texture()
{
	if( this->id )
		glDeleteTextures( 1, &this->id );
}


void Texture::update( const ImageBuffer & imageBuffer )
{
	if( this->id && this->width == imageBuffer.getWidth() && this->height == imageBuffer.getHeight()
		&& imageBuffer.getWidth() != 0 && imageBuffer.getHeight() != 0 )
	{
		glBindTexture( GL_TEXTURE_2D, this->id );
		glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, this->width, this->height, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer.getRGBAData().data() );
	}
	else
	{
		if( this->id )
		{
			glDeleteTextures( 1, &this->id );
			this->id = 0;
		}

		this->width = imageBuffer.getWidth();
		this->height = imageBuffer.getHeight();

		glGenTextures( 1, &this->id );
		glBindTexture( GL_TEXTURE_2D, this->id );
		glTexImage2D( GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer.getRGBAData().data() );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	}
}
