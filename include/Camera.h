#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include "hittable.h"
#include "utils.h"

class Camera {
public:
    float   aspect_ratio        = 1.0;
    int     image_width         = 1000;
    int     samples_per_pixel   = 100;

    void    render(const hittable &world);

private:
    int     image_height;
    float   pixel_samples_scale;
    point3  center;
    point3  pixel00_loc;
    vec3    pixel_delta_u;
    vec3    pixel_delta_v;

    void    initialize();
    ray     get_ray(const int i, const int j) const;
    vec3    sample_square() const;
    color   ray_color(const ray &r, const hittable &world) const;
};
#endif //RAYTRACING_CAMERA_H