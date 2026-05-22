#ifndef TGAIMAGE_HPP
#define TGAIMAGE_HPP

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

	bool isCompressed();
	void toggleCompression();
	int writeFile(const char* filename);

private:
	std::vector<uint8_t> getEncodedData();

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

inline bool TGAImage::isCompressed() {
	return header.imageType > TGAImageType::UncompressedGrayscale;
}

inline void TGAImage::toggleCompression() {
	switch (header.imageType) {
		case TGAImageType::UncompressedColorMapped:
			header.imageType = TGAImageType::CompressedColorMapped;
			break;
		case TGAImageType::UncompressedTrueColor:
			header.imageType = TGAImageType::CompressedTrueColor;
			break;
		case TGAImageType::UncompressedGrayscale:
			header.imageType = TGAImageType::CompressedGrayscale;
			break;
		case TGAImageType::CompressedColorMapped:
			header.imageType = TGAImageType::UncompressedColorMapped;
			break;
		case TGAImageType::CompressedTrueColor:
			header.imageType = TGAImageType::UncompressedTrueColor;
			break;
		case TGAImageType::CompressedGrayscale:
			header.imageType = TGAImageType::UncompressedGrayscale;
			break;
		default:
			break;
	}
}

inline std::vector<uint8_t> TGAImage::getEncodedData() {
	std::vector<uint8_t> ans;
	int x;
	bool lineEnd;	
	for (int y = 0; y < height; ++y) {
		x = 0;
		lineEnd = false;
		while (!lineEnd) {
			uint8_t size = 0;
			if (data[y * width + x] == data[y * width + x + 1]) {
				RGBA col = data[y * width + x];
				while (x < width && size < 127 && data[y * width + x] == col) {
					++x;
					++size;
				}
				ans.push_back(0b10000000 | (size - 1));
				for (int i = 0; i < sizeof(RGBA); ++i)
					ans.push_back(*(&col.r + i));
				if (x == width)
					lineEnd = true;
			}
			else {
				std::vector<RGBA> colBuff;
				colBuff.push_back(data[y * width + x]);
				while (x < width - 1 && size < 127 && data[y * width + x] != data[y * width + x + 1]) {
					++x;
					colBuff.push_back(data[y * width + x]);
					++size;
				}
				if (x < width - 1 && size < 128) {
					size--;
					colBuff.pop_back();
				}
				ans.push_back(size);
				for (const RGBA& col : colBuff) {
					for (int j = 0; j < sizeof(RGBA); ++j)
						ans.push_back(*(&col.r + j));
				}
				if (x == width - 1)
					lineEnd = true;
			}
		}
	}
	return ans;
}

inline int TGAImage::writeFile(const char* filename) {
	std::ofstream file(filename, std::ios::binary);
	file.write(reinterpret_cast<const char*>(&header), sizeof(header));
	if (isCompressed()) {
		std::vector<uint8_t> encodedData = getEncodedData();
		file.write(reinterpret_cast<const char*>(encodedData.data()), encodedData.size());
	} else
		file.write(reinterpret_cast<const char*>(data.data()), width * height * sizeof(RGBA));
	file.close();
	return 0;
}

#endif // TGAIMAGE_HPP
