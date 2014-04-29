#ifndef IMAGEBUFFER_H_INCLUDED
#define IMAGEBUFFER_H_INCLUDED


#include <vector>


class ImageBuffer
{
public:
	ImageBuffer( unsigned int width, unsigned int height );
	ImageBuffer( unsigned int width, unsigned int height, const std::vector< unsigned char > & rgbaData );
	~ImageBuffer();

	ImageBuffer & operator=( const ImageBuffer & other );

	unsigned int getWidth() const { return this->width; }
	unsigned int getHeight() const { return this->height; }
	const std::vector<unsigned char> & getRGBAData() const { return buffer; }
	bool getPixel( unsigned int x, unsigned int y, unsigned char & r, unsigned char & g, unsigned char & b ) const;
	bool getPixel( unsigned int x, unsigned int y, unsigned char & r, unsigned char & g, unsigned char & b, unsigned char & a ) const;

	void setRGBAData( unsigned int width, unsigned int height, const std::vector<unsigned char> & data );
	bool setPixel( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0xff );

	void resize( unsigned int width, unsigned int height );
	void clear( unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0xff );

private:
	unsigned int width;
	unsigned int height;
	std::vector<unsigned char> buffer;
};


#endif
