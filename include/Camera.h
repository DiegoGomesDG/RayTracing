#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include "hittable.h"

class Camera {
public:
    float   aspect_ratio    = 1.0;
    int     image_width     = 1000;

    void render(const hittable &world);

private:
    int     image_height;
    point3  center;
    point3  pixel00_loc;
    vec3    pixel_delta_u;
    vec3    pixel_delta_v;

    void initialize();
    color ray_color(const ray &r, const hittable &world) const;
};
#endif //RAYTRACING_CAMERA_H