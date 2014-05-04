#include "Pixelator.h"

#include <iostream>
#include <stdlib.h>


int main( int argc, char ** argv )
{
	(void)argc;
	(void)argv;

	Pixelator p( 512, 512, 128, 128 );

	////////////////////////////////
	// set some random pixels to a random color and write a bmp file
	for( unsigned int i = 0; i < 128; i++ )
		p.setPixel( rand()%p.getWidth(), rand()%p.getHeight(), rand()%256, rand()%256, rand()%256 );
	p.writeBMP( "pixelator.bmp", BitmapFile::RGB );
	p.wait();

	////////////////////////////////
	// clear and set some more random pixels to a random color and write a bmp file (this time with alpha channel)
	p.clear( 0, 0, 0, 0 );
	for( unsigned int i = 0; i < 1024; i++ )
		p.setPixel( rand()%p.getWidth(), rand()%p.getHeight(), rand()%256, rand()%256, rand()%256, rand()%256 );
	p.writeBMP( "pixelator2.bmp", BitmapFile::RGBA );
	p.wait();

	////////////////////////////////
	// mirror on x axis
	for( unsigned int y = 0; y < p.getHeight() / 2; y++ )
	{
		for( unsigned int x = 0; x < p.getWidth(); x++ )
		{
			unsigned char r=0,g=0,b=0,a=0;
			p.getPixel( x, y, r, g, b, a );
			p.setPixel( x, p.getHeight() - y - 1, r, g, b, a );
		}
	}
	p.wait();

	////////////////////////////////
	// mirror on y axis
	for( unsigned int y = 0; y < p.getHeight(); y++ )
	{
		for( unsigned int x = 0; x < p.getWidth() / 2; x++ )
		{
			unsigned char r=0,g=0,b=0,a=0;
			p.getPixel( x, y, r, g, b, a );
			p.setPixel( p.getWidth() - x - 1, y, r, g, b, a );
		}
	}
	p.wait();

	////////////////////////////////
	// blend the pixels from second image with the current
	ImageBuffer * tmp = BitmapFile::load( "pixelator2.bmp" );
	for( unsigned int y = 0; y < p.getHeight(); y++ )
	{
		for( unsigned int x = 0; x < p.getWidth(); x++ )
		{
			unsigned char r1=0,g1=0,b1=0,a1=0;
			unsigned char r2=0,g2=0,b2=0,a2=0;
			tmp->getPixel( x, y, r1, g1, b1, a1 );
			p.getPixel( x, y, r2, g2, b2, a2 );
			p.setPixel( x, y, ((int)r1+(int)r2)/2, ((int)g1+(int)g2)/2, ((int)b1+(int)b2)/2, ((int)a1+(int)a2)/2 );
		}
	}
	delete tmp;
	p.writeBMP( "pixelator3.bmp", BitmapFile::RGBA );
	p.wait();

	////////////////////////////////
	// load first image
	p.readBMP( "pixelator.bmp" );
	p.wait();

	////////////////////////////////
	// load second image
	p.readBMP( "pixelator2.bmp" );
	p.wait();

	////////////////////////////////
	// load third image
	p.readBMP( "pixelator3.bmp" );
	p.wait();

	return 0;
}
