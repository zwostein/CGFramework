#ifndef BITMAPFILE_H_INCLUDED
#define BITMAPFILE_H_INCLUDED


#include <string>
#include <vector>

#include <stdint.h>


class BitmapFile
{
public:
	enum PixelFormat
	{
		RGB,
		RGBA
	};

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

	BitmapFile( const std::string & fileName, PixelFormat pixelFormat = RGB );
	~BitmapFile();

	void fromRGBAData( unsigned int width, unsigned int height, const std::vector<unsigned char> & data );
	void write();

private:
	std::string fileName;
	FileHeader fileHeader;
	InfoHeader infoHeader;
	std::vector<unsigned char> buffer;
};


#endif
