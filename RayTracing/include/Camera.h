#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include <fstream>

#include "hittable.h"
#include "utils.h"

class Camera {
public:
    real    aspect_ratio;                   // Ratio of image width over height
    int     image_width;                    // Rendered image width in pixel count
    int     image_height;                   // Rendered image height
    int     samples_per_pixel   = 100;      // Count of random samples for each pixel
    int     max_depth           = 10;       // Maximum number of ray bounces into scene

    real    vfov        = 90.0;             // Vertical view angle (FoV)
    point3  lookfrom    = point3(0, 0, 0);  // Point camera is looking from
    point3  lookat      = point3(0, 0, -1); // Point camera is looking at
    vec3    vup         = vec3(0, 1, 0);    // Camera-relative "up" direction

    real    defocus_angle   = 0;
    real    focus_dist      = 10;

    void    set_resolution(const int width, const int height);

    void    render(const hittable &world);
    void    print_progress(
        int     current_line,
        int     total_lines,
        const   std::chrono::steady_clock::time_point& start,
        double  &smoothed_avg
    );

    void    render_to_buffer(
        const hittable& world,
        std::vector<uint32_t>& framebuffer,
        std::atomic<bool>& cancel_flag
    );

private:
    real    pixel_samples_scale;            // Color scale factor for a sum of pixel samples
    point3  center;                         // Camera center
    point3  pixel00_loc;                    // Location of pixel 0, 0
    vec3    pixel_delta_u;                  // Offset to pixel to the right
    vec3    pixel_delta_v;                  // Offset to pixel below
    vec3    u, v, w;                        // Camera frame basis vectors
    vec3    defocus_disk_u;
    vec3    defocus_disk_v;

    void    initialize();
    ray     get_ray(const int i, const int j) const;
    vec3    sample_square() const;
    point3  defocus_disk_sample() const;
    color   ray_color(const ray &r, int depth, const hittable &world) const;
};
#endif //RAYTRACING_CAMERA_H