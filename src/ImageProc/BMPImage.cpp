#include <ImageProc/BMPImage.hpp>


int BMPImage::writeFile(const char* filename) const {
	// header.size = getFileSize();
	std::ofstream file(filename, std::ios::binary);
	file.write(reinterpret_cast<const char*>(&header), sizeof(BMPHeader));
	file.write(reinterpret_cast<const char*>(&info), sizeof(BMPInfo));
	for (const RGBA& color : data) {
		file.write(reinterpret_cast<const char*>(&color), 3);
	}
	file.close();
	return 0;
}
