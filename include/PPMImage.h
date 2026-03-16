#ifndef RAYTRACING_PPMIMAGE_H
#define RAYTRACING_PPMIMAGE_H

class PPMImage {
private:
    // TODO: Add 2D array to store the pixels
    int imageWidth;
    int imageHeight;
public:
    PPMImage(int width, int height);
    ~PPMImage();

    void setPixel(int x, int y);
    void save();
    void sample();
};

#endif