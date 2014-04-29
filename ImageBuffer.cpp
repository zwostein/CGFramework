#include "ImageBuffer.h"


ImageBuffer::ImageBuffer( unsigned int width, unsigned int height ) : width(width), height(height)
{
	this->buffer.resize( this->width * this->height * 4 );
}


ImageBuffer::ImageBuffer( unsigned int width, unsigned int height, const std::vector< unsigned char > & rgbaData ) : width(width), height(height), buffer(rgbaData)
{
	this->buffer.resize( this->width * this->height * 4 );
}


ImageBuffer::~ImageBuffer()
{

}


ImageBuffer & ImageBuffer::operator=( const ImageBuffer & other )
{
	if( this == &other )
		return * this;

	this->width = other.width;
	this->height = other.height;
	this->buffer = other.buffer;

	return *this;
}


bool ImageBuffer::getPixel( unsigned int x, unsigned int y, unsigned char & r, unsigned char & g, unsigned char & b ) const
{
	// clipping
	if( x >= this->width || y >= this->height )
		return false;

	// get pixel at position x, y
	unsigned int index = 4 * ( x + this->width * y );
	r = this->buffer[ index ];
	g = this->buffer[ index + 1 ];
	b = this->buffer[ index + 2 ];
	return true;
}


bool ImageBuffer::getPixel( unsigned int x, unsigned int y, unsigned char & r, unsigned char & g, unsigned char & b, unsigned char & a ) const
{
	// clipping
	if( x >= this->width || y >= this->height )
		return false;

	// get pixel at position x, y
	unsigned int index = 4 * ( x + this->width * y );
	r = this->buffer[ index ];
	g = this->buffer[ index + 1 ];
	b = this->buffer[ index + 2 ];
	a = this->buffer[ index + 3 ];
	return true;
}


bool ImageBuffer::setPixel( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a )
{
	// clipping
	if( x >= this->width || y >= this->height )
		return false;

	// set pixel at position x, y
	unsigned int index = 4 * ( x + this->width * y );
	this->buffer[ index ] = r;
	this->buffer[ index + 1 ] = g;
	this->buffer[ index + 2 ] = b;
	this->buffer[ index + 3 ] = a;
	return true;
}


void ImageBuffer::setRGBAData( unsigned int width, unsigned int height, const std::vector< unsigned char > & data )
{
	this->width = width;
	this->height = height;
	this->buffer = data;
	this->buffer.resize( this->width * this->height * 4 );
}


void ImageBuffer::resize( unsigned int width, unsigned int height )
{
	this->width = width;
	this->height = height;
	this->buffer.resize( this->width * this->height * 4 );
}


void ImageBuffer::clear( unsigned char r, unsigned char g, unsigned char b, unsigned char a )
{
	for( unsigned int y = 0; y < this->height; y++ )
	{
		for( unsigned int x = 0; x < this->width; x++ )
		{
			this->setPixel( x, y, r, g, b, a );
		}
	}
}
