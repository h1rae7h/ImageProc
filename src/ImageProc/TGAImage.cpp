#include <ImageProc/TGAImage.hpp>


void TGAImage::toggleCompression() {
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

std::vector<uint8_t> TGAImage::getEncodedData() const {
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

int TGAImage::writeFile(const char* filename) const {
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
