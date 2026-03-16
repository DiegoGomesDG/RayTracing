#include "PPMImage.h"

#include <iostream>
#include <ostream>

PPMImage::PPMImage(int width, int height) : imageWidth(width), imageHeight(height) {}

PPMImage::~PPMImage() {}

void PPMImage::setPixel(int x, int y) {
}

void PPMImage::save() {
}

void PPMImage::sample() {
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; j++) {
        std::clog << "\rScanlines Remaining: " << (imageHeight - j) << ' ' << std::flush;
        for (int i = 0; i < imageWidth; i++) {
            auto r = double(i) / (imageWidth - 1);
            auto g = double(j) / (imageHeight - 1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    std::clog << "\rDone.       \n";
}
