#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED


#include "ImageBuffer.h"

#include <GL/gl.h>


class Texture
{
public:
	Texture();
	Texture( const ImageBuffer & imageBuffer );
	~Texture();

	GLuint getID() { return this->id; }
	unsigned int getWidth() { return this->width; }
	unsigned int getHeight() { return this->height; }

	void update( const ImageBuffer & imageBuffer );
	void bind() { glBindTexture( GL_TEXTURE_2D, this->id ); }

private:
	GLuint id;
	unsigned int width;
	unsigned int height;
};


#endif
