#include "Pixelator.h"

#include <stdlib.h>


int main( int argc, char ** argv )
{
	Pixelator p( 512, 512, 64, 64 );

	// set some random pixels to a random color
	for( int i = 0; i < 256; i++ )
		p.setPixel( rand()%p.getWidth(), rand()%p.getHeight(), rand()%256, rand()%256, rand()%256 );

	p.writeBMP( "pixelator.bmp", BitmapFile::RGB );
	p.writeBMP( "pixelator2.bmp", BitmapFile::RGBA );
	p.wait();
	p.readBMP( "test.bmp" );
	p.wait();
	p.readBMP( "test2.bmp" );
	p.wait();
	p.readBMP( "pixelator.bmp" );
	p.wait();
}
