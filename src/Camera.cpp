#include "Camera.h"
#include "Image.h"

void Camera::render(const hittable &world) {
    initialize();
    Image image(image_width, image_height);

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            color pixel_color(0, 0, 0);
            for (int sample = 0; sample < samples_per_pixel; sample++) {
                ray r = get_ray(i, j);
                pixel_color += ray_color(r, max_depth, world);
            }
            image.set_pixel(i, j, pixel_samples_scale * pixel_color);
        }
    }
    std::clog << "\nDone                    \n";

    std::ofstream out("../image.ppm");
    image.write_ppm(out);
}

void Camera::initialize() {
    image_height            = static_cast<int>(image_width / aspect_ratio);
    image_height            = (image_height < 1) ? 1 : image_height;

    pixel_samples_scale     = 1.0f / samples_per_pixel;

    center                  = point3(0, 0, 0);

    float focal_length      = 1.0f;
    float viewport_height   = 2.0f;
    float viewport_width    = viewport_height * (static_cast<double>(image_width) / image_height);

    // Calculate vectors across horizontal and down the vertical viewport edges
    auto viewport_u         = vec3(viewport_width, 0, 0);
    auto viewport_v         = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    pixel_delta_u           = viewport_u / image_width;
    pixel_delta_v           = viewport_v / image_height;

    // Calculate location of the upper left pixel (0, 0)
    auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
}

ray Camera::get_ray(const int i, const int j) const {
    /* Construct a camera ray originating from the origin and directed at randomly sampled
     * point around the pixel location i, j
     */
    auto offset = sample_square();
    auto pixel_sample = pixel00_loc
                        + (i + offset.x()) * pixel_delta_u
                        + (j + offset.y()) * pixel_delta_v;
    auto ray_origin = center;
    auto ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
}

vec3 Camera::sample_square() const {
    /* Return vector to a random point in the [-.5, -.5] - [+.5, +.5] unit square */
    return vec3(random_real() - 0.5f, random_real() - 0.5f, 0);
}

color Camera::ray_color(const ray &r, int depth, const hittable &world) const {
    if (depth <=0) {
        return color(0, 0, 0);
    }

    const color white(1.0f, 1.0f, 1.0f);
    const color blue(0.5f, 0.7f, 1.0f);

    hit_record rec;
    if (world.hit(r, interval(0.001, infinity), rec)) {
        vec3 direction = rec.normal + random_unit_vector();
        return 0.5f * ray_color(ray(rec.p, direction), depth - 1, world);
    }

    const vec3 unit_direction = unit_vector(r.direction());
    const auto a = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - a) * white + a * blue;
}
