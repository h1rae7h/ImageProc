#ifndef BMPIMAGE_HPP
#define BMPIMAGE_HPP

#include <fstream>
#include "Image.hpp"


#pragma pack(push, 1)
struct BMPHeader {
	uint16_t type = 0x4D42;		// BM - BMP format magic numbers
	uint32_t size;				// file size in bytes
	uint16_t reserved1 = 0U;	// 4 reserved bytes, must be 0
	uint16_t reserved2 = 0U;
	uint32_t offset =			// offset to image data in bytes
		14U + 40U;				// header size + info size
};
#pragma pack(pop)


#pragma pack(push, 1)
struct BMPInfo {
	uint32_t size = 40U;			// info struct size in bytes
	int32_t width = defaultWidth;	// image width
	int32_t height = defaultHeight;	// image height
	uint16_t planes = 1U;			// number of color planes
	uint16_t bits = 24U;			// bits per pixel, min 1, max 24
	uint32_t compression = 0U;		// type of compression if any
	uint32_t imageSize = 0U;		// image size in bytes (only for compressed image, 0 for uncompressed)
	int32_t xRes = 0;				// pixels per meter vertical
	int32_t yRes = 0;				// pixels per meter horizontal
	uint32_t colorsNumberi = 0U;	// number of colors
	uint32_t importantColors = 0;	// important colors 
};
#pragma pack(pop)


class BMPImage : public Image {
public:
	BMPImage();
	BMPImage(int width, int height);

	uint16_t getFileSize() const;
	int writeFile(const char* filename) const;

private:
	BMPHeader header;
	BMPInfo info;

};

inline BMPImage::BMPImage() {}

inline BMPImage::BMPImage(int width, int height)
	: Image(width, height) {
	info.width = this->width;
	info.height = this->height;
	header.size = getFileSize();
}

inline uint16_t BMPImage::getFileSize() const {
	// rounding to upper
	uint16_t minSize = data.size() * info.bits / 8;
	float rest = data.size() * info.bits / 8.f - minSize;
	uint16_t dataSize = minSize + (rest > 0.f ? 1 : 0);
	return header.offset + dataSize;
}

#endif // BMPIMAGE_HPP
