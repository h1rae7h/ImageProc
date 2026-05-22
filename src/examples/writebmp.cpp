#include <ImageProc/BMPImage.hpp>
#include <iostream>


int main() {
	BMPImage img(20, 20);
	img.writeFile("img.bmp");

	return 0;
}
