#include "BitmapFile.hpp"

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdint.h>


#pragma pack(push,1)

struct FileHeader
{
	uint8_t type[2];
	uint32_t size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t dataOffset;
};

struct InfoHeader
{
	uint32_t size;
	int32_t width;
	int32_t height;
	uint16_t planes;
	uint16_t bitsPerPixel;
	uint32_t compression;
	uint32_t sizeImage;
	uint32_t pixelsPerMeterX;
	uint32_t pixelsPerMeterY;
	uint32_t colorsUsed;
	uint32_t colorsImportant;
	uint32_t redMask;
	uint32_t greenMask;
	uint32_t blueMask;
	uint32_t alphaMask;
	uint8_t colorSpaceType[4];
	uint32_t endpoints[9];
	uint32_t gammaRed;
	uint32_t gammaGreen;
	uint32_t gammaBlue;
	uint32_t intent;
	uint32_t profileData;
	uint32_t profileSize;
	uint32_t reserved;
};

#pragma pack(pop)


static uint8_t filterBitmask( uint32_t value, uint32_t mask )
{
	if( !mask )
		return 0;
	value &= mask;
	while( !(mask & 1) )
	{
		value >>= 1;
		mask >>= 1;
	}
	return value;
}


ImageBuffer * BitmapFile::load( const std::string & fileName )
{
	////////////////////////////////
	// open image or return NULL on error
	std::ifstream file( fileName.c_str(), std::ifstream::binary );
	if( !file.is_open() )
	{
		return NULL;
	}

	////////////////////////////////
	// read file header and check file type
	FileHeader fileHeader;
	memset( &fileHeader, 0, sizeof(FileHeader) );
	file.read( reinterpret_cast<char*>(&fileHeader), sizeof(FileHeader) );
	if( fileHeader.type[0] != 'B' || fileHeader.type[1] != 'M' )
	{
		std::cerr << "File \"" << fileName << "\" is not a supported Bitmap." << std::endl;
		return NULL;
	}

	////////////////////////////////
	// read info header
	InfoHeader infoHeader;
	memset( &infoHeader, 0, sizeof(InfoHeader) );
	file.read( reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader.size) );
	file.read( reinterpret_cast<char*>(&infoHeader) + sizeof(infoHeader.size), infoHeader.size - sizeof(infoHeader.size) );

	////////////////////////////////
	// read image data
	file.seekg( fileHeader.dataOffset, file.beg );
	std::vector<unsigned char> fileData( infoHeader.sizeImage );
	file.read( reinterpret_cast<char*>(fileData.data()), fileData.size() );

	file.close();

	////////////////////////////////
	// convert image data to RGBA format
	std::vector<unsigned char> rgbaData( infoHeader.width * infoHeader.height * 4 );
	if( infoHeader.bitsPerPixel == 24 && infoHeader.compression == 0 )
	{
		for( unsigned int src=0,dst=0; src<infoHeader.sizeImage; src+=3,dst+=4 )
		{
			rgbaData[dst] = fileData[src+2];
			rgbaData[dst+1] = fileData[src+1];
			rgbaData[dst+2] = fileData[src];
			rgbaData[dst+3] = 0xff;
		}
	}
	else if( infoHeader.bitsPerPixel == 32 && infoHeader.compression == 3 )
	{
		for( unsigned int i = 0; i < infoHeader.sizeImage; i += 4 )
		{
			uint32_t color = *reinterpret_cast<const uint32_t *>(&fileData[i]);
			rgbaData[i] = filterBitmask( color, infoHeader.redMask );
			rgbaData[i+1] = filterBitmask( color, infoHeader.greenMask );
			rgbaData[i+2] = filterBitmask( color, infoHeader.blueMask );
			rgbaData[i+3] = filterBitmask( color, infoHeader.alphaMask );
		}
	}
	else
	{
		std::cerr << "File \"" << fileName << "\" uses supported format." << std::endl;
		return NULL;
	}

	////////////////////////////////
	// create a new image buffer
	ImageBuffer * imageBuffer = new ImageBuffer( infoHeader.width, infoHeader.height, rgbaData );

	return imageBuffer;
}


bool BitmapFile::write( const ImageBuffer & imageBuffer, const std::string & fileName, const PixelFormat & pixelFormat )
{
	InfoHeader infoHeader;
	memset( &infoHeader, 0, sizeof(InfoHeader) );
	infoHeader.size = sizeof(InfoHeader);
	infoHeader.planes = 1;
	switch( pixelFormat )
	{
		case RGB:
			infoHeader.bitsPerPixel = 24;
			infoHeader.compression = 0;
			break;
		case RGBA:
			infoHeader.bitsPerPixel = 32;
			infoHeader.compression = 3;
			break;
	}
	infoHeader.width = imageBuffer.getWidth();
	infoHeader.height = imageBuffer.getHeight();
	infoHeader.sizeImage = infoHeader.width * infoHeader.height * (infoHeader.bitsPerPixel / 8);
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

	FileHeader fileHeader;
	memset( &fileHeader, 0, sizeof(FileHeader) );
	fileHeader.type[0] = 'B';
	fileHeader.type[1] = 'M';
	fileHeader.dataOffset = sizeof(FileHeader) + sizeof(InfoHeader);
	fileHeader.size = sizeof(FileHeader) + sizeof(InfoHeader) + infoHeader.sizeImage;

	std::ofstream file( fileName.c_str(), std::ofstream::binary );
	file.write( reinterpret_cast<const char *>(&fileHeader), sizeof(FileHeader) );
	file.write( reinterpret_cast<const char *>(&infoHeader), sizeof(InfoHeader) );
	std::vector<unsigned char> fileData( infoHeader.sizeImage );
	switch( pixelFormat )
	{
		case RGB:
			for( unsigned int src=0,dst=0; dst<infoHeader.sizeImage; src+=4,dst+=3 )
			{
				fileData[dst] = imageBuffer.getRGBAData()[src+2];
				fileData[dst+1] = imageBuffer.getRGBAData()[src+1];
				fileData[dst+2] = imageBuffer.getRGBAData()[src];
			}
			break;
		case RGBA:
			fileData = imageBuffer.getRGBAData();
			break;
	}
	file.write( reinterpret_cast<const char *>(fileData.data()), fileData.size() );
	file.close();

	return true;
}
