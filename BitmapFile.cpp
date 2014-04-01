#include "BitmapFile.hpp"

#include <iostream>
#include <fstream>

#include <string.h>


BitmapFile::BitmapFile( const std::string & fileName, PixelFormat pixelFormat ) : fileName(fileName)
{
	memset( &fileHeader, 0, sizeof(FileHeader) );
	fileHeader.type[0] = 'B';
	fileHeader.type[1] = 'M';
	fileHeader.dataOffset = sizeof(FileHeader) + sizeof(InfoHeader);

	memset( &infoHeader, 0, sizeof(InfoHeader) );
	infoHeader.size = sizeof(InfoHeader);

	infoHeader.planes = 1;
	if( pixelFormat == RGB )
	{
		infoHeader.bitsPerPixel = 24;
		infoHeader.compression = 0;
	}
	else
	{
		infoHeader.bitsPerPixel = 32;
		infoHeader.compression = 3;
	}

	infoHeader.pixelsPerMeterX = 2835;
	infoHeader.pixelsPerMeterY = 2835;
	infoHeader.redMask   = 0x000000FF;
	infoHeader.greenMask = 0x0000FF00;
	infoHeader.blueMask  = 0x00FF0000;
	infoHeader.alphaMask = 0xFF000000;
	infoHeader.colorSpaceType[0] = 'W';
	infoHeader.colorSpaceType[1] = 'i';
	infoHeader.colorSpaceType[2] = 'n';
	infoHeader.colorSpaceType[3] = ' ';
}


BitmapFile::~BitmapFile()
{
}


void BitmapFile::fromRGBAData( unsigned int width, unsigned int height, const std::vector<unsigned char> & data )
{
	infoHeader.width = width;
	infoHeader.height = height;

	infoHeader.sizeImage = width * height * (infoHeader.bitsPerPixel / 8);

	switch( infoHeader.bitsPerPixel )
	{
		case 24:
		{
			buffer.clear();
			for( unsigned int i=0; i<data.size(); i+=4 )
			{
				buffer.push_back( data[i] );
				buffer.push_back( data[i+1] );
				buffer.push_back( data[i+2] );
			}
		}
		break;
		case 32:
		{
			buffer = data;
		}
		break;
	}

	fileHeader.size = sizeof(FileHeader) + sizeof(InfoHeader) + infoHeader.sizeImage;
}


void BitmapFile::write()
{
	std::ofstream file( fileName.c_str(), std::ofstream::binary );
	file.write( reinterpret_cast<const char *>(&fileHeader), sizeof(FileHeader) );
	file.write( reinterpret_cast<const char *>(&infoHeader), sizeof(InfoHeader) );
	file.write( reinterpret_cast<const char *>(buffer.data()), buffer.size() );
	file.close();
}
