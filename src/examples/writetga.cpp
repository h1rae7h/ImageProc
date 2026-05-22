#include <ImageProc/TGAImage.hpp>
#include <iostream>


int main() {
    TGAImage img(20, 20);

    img.setPoint(3, 15, RGBA(255, 255, 255, 255));
    img.setPoint(4, 15, RGBA(56, 32, 11, 255));
    img.setPoint(5, 15, RGBA(56, 32, 11, 255));
    img.setPoint(6, 15, RGBA(56, 32, 11, 255));
    img.setPoint(7, 15, RGBA(56, 32, 11, 255));
    img.setPoint(8, 15, RGBA(255, 255, 255, 255));

/*
    img.setPoint(17, 5, RGBA(255, 255, 255, 255));
    img.setPoint(18, 5, RGBA(211, 43, 123, 255));
    img.setPoint(19, 5, RGBA(211, 43, 123, 255));
    img.setPoint(0, 6, RGBA(211, 43, 123, 255));
    img.setPoint(1, 6, RGBA(211, 43, 123, 255));
    img.setPoint(2, 6, RGBA(200, 200, 200, 255));
*/
    img.writeFile("img.tga");
}
