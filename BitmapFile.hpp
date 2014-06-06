#ifndef BITMAPFILE_H_INCLUDED
#define BITMAPFILE_H_INCLUDED


#include "ImageBuffer.h"

#include <string>


namespace BitmapFile
{
	enum PixelFormat
	{
		RGB,
		RGBA
	};

	ImageBuffer * load( const std::string & fileName );
	bool write( const ImageBuffer & imageBuffer, const std::string & fileName, const PixelFormat & pixelFormat = RGB );
}


#endif
