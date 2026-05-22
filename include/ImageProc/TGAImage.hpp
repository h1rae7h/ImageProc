#ifndef TGAIMAGE_HPP
#define TGAIMAGE_HPP

#include <fstream>
#include "Image.hpp"


enum class TGAImageType : uint8_t {
	NoData,
	UncompressedColorMapped,
	UncompressedTrueColor,
	UncompressedGrayscale,
	CompressedColorMapped = 9U,
	CompressedTrueColor,
	CompressedGrayscale
};

#pragma pack(push, 1)	// save current padding (push), disable padding by setting max padding to 1 byte
struct TGAHeader {
	uint8_t idLength = 0;

	uint8_t colorMapType = 0;

	TGAImageType imageType = TGAImageType::CompressedTrueColor;

	// Color map specification
	uint16_t firstEntryIndex = 0;
	uint16_t colorMapLength = 0;
	uint8_t colorMapEntrySize = 0;

	// Image specification
	uint16_t xOrigin = 0;
	uint16_t yOrigin = 0;
	uint16_t width = defaultWidth;
	uint16_t height = defaultHeight;
	uint8_t pixelDepth = 32;
	uint8_t imageDescriptor;
};
#pragma pack(pop)		// return padding



class TGAImage : public Image {
public:
	TGAImage();
	TGAImage(int width, int height);

	bool isCompressed() const;
	void toggleCompression();
	int writeFile(const char* filename) const;

private:
	std::vector<uint8_t> getEncodedData() const;

	TGAHeader header;

	uint8_t alphaChannelBits  = 0b00001000;	// 8 bit alpha channel (first 4 bits)
	uint8_t dataToScreenOrder = 0b00000000;	// first pixel pos is bottom (bit 6), so bottom-to-top vertical order, and left (bit 5), so left-to-right horizontal order

};

inline TGAImage::TGAImage() {
	header.imageDescriptor = alphaChannelBits | dataToScreenOrder;
}

inline TGAImage::TGAImage(int width, int height)
	: Image(width, height) {
	header.width = this->width;
	header.height = this->height;
	header.imageDescriptor = alphaChannelBits | dataToScreenOrder;
}

inline bool TGAImage::isCompressed() const {
	return header.imageType > TGAImageType::UncompressedGrayscale;
}

#endif // TGAIMAGE_HPP
