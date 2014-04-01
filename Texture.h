#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED


#include <GLFW/glfw3.h>

#include <vector>


class Texture
{
public:
	Texture( unsigned int width, unsigned int height );
	~Texture();

	void resize( unsigned int width, unsigned int height );
	void setPixel( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0xff );
	void clear( unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0xff );
	void update();

	void bind() { glBindTexture( GL_TEXTURE_2D, this->id ); }

	GLuint getID() { return this->id; }
	const std::vector<unsigned char> & getRGBAData() { return buffer; }
	unsigned int getWidth() { return this->width; }
	unsigned int getHeight() { return this->height; }

private:
	GLuint id;
	std::vector<unsigned char> buffer;
	unsigned int width;
	unsigned int height;

	void free();
	void create( unsigned int width, unsigned int height );
};


#endif
