#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cstdint>
#include <vector>
#include <fstream>


inline constexpr uint16_t defaultWidth = 32;
inline constexpr uint16_t defaultHeight = 32;

using Grayscale = uint8_t;

struct RGB {
	RGB()
		: r(0), g(0), b(0) {}
	RGB(uint8_t r, uint8_t g, uint8_t b)
		: r(r), g(g), b(b) {}
	
	RGB &operator=(Grayscale right);
	bool operator==(RGB right);
	bool operator!=(RGB right);

	Grayscale getGrayscale();

	uint8_t r, g, b;
};

inline RGB &RGB::operator=(Grayscale right) {
	r = g = b = right;
	return *this;
}

inline bool RGB::operator==(RGB right) {
	return r == right.r && g == right.g && b == right.b;
}

inline bool RGB::operator!=(RGB right) {
	return r != right.r || g != right.g || b != right.b;
}

inline Grayscale RGB::getGrayscale() {
	return (r + g + b) / 3;
}


struct RGBA : public RGB {
	RGBA()
		: a(255) {}
	RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: RGB(r, g, b), a(a) {}
	RGBA(uint8_t r, uint8_t g, uint8_t b)
		: RGB(r, g, b), a(255) {}
	
	RGBA &operator=(Grayscale right);
	RGBA &operator=(RGB right);
	bool operator==(RGBA right);
	bool operator!=(RGBA right);

	uint8_t a;
};

inline RGBA &RGBA::operator=(Grayscale right) {
	r = g = b = right;
	a = 255;
	return *this;
}

inline RGBA &RGBA::operator=(RGB right) {
	r = right.r;
	g = right.g;
	b = right.b;
	a = 255;
	return *this;
}

inline bool RGBA::operator==(RGBA right) {
	return r == right.r && g == right.g && b == right.b && a == right.a;
}

inline bool RGBA::operator!=(RGBA right) {
	return r != right.r || g != right.g || b != right.b || a != right.a;
}


class Image {
public:
	Image();
	Image(uint16_t width, uint16_t height);

	RGBA& operator[](int i);
	const RGBA& operator[](int i) const;

	int getWidth();
	int getHeight();


	RGBA getPoint(int y, int x) const;
	void setPoint(int y, int x, RGBA color);

protected:
	int width, height;
	// data is stored as 'height' scanlines of lenght 'width' one after another
	std::vector<RGBA> data;

};

inline Image::Image()
	: width(defaultWidth), height(defaultHeight), data(defaultWidth * defaultWidth) {}

inline Image::Image(uint16_t width, uint16_t height)
	: width(width < 1 ? 32 : width), height(height < 1 ? 32 : height) {
	data.resize(width * height);
}

inline RGBA& Image::operator[](int i) {
	return data[i];
}

inline const RGBA& Image::operator[](int i) const {
	return data[i];
}

inline int Image::getWidth() {
	return width;
}

inline int Image::getHeight() {
	return height;
}

inline RGBA Image::getPoint(int x, int y) const {
	return data[y * width + x];
}

inline void Image::setPoint(int x, int y, RGBA color) {
	data[y * width + x] = color;
}

#endif // IMAGE_HPP
