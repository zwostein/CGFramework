#include "Pixelator.h"

#include <stdlib.h>

int main( int argc, char ** argv )
{
	Pixelator p( 512, 512, 64, 64 );

	// set some random pixels to a random color
	for( int i = 0; i < 100; i++ )
		p.setPixel( rand()%p.getCanvasWidth(), rand()%p.getCanvasHeight(), rand()%256, rand()%256, rand()%256 );

	p.wait();
}
