#include "Camera.h"

void Camera::render(const hittable &world) {
    initialize();

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - center;
            ray r(center, ray_direction);
            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\nDone.                               \n";
}

void Camera::initialize() {
    image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    center = point3(0, 0, 0);

    float focal_length      = 1.0f;
    float viewport_height   = 2.0f;
    float viewport_width    = viewport_height * (static_cast<double>(image_width) / image_height);

    // Calculate vectors across horizontal and down the vertical viewport edges
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calculate location of the upper left pixel (0, 0)
    auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
}

color Camera::ray_color(const ray &r, const hittable &world) const {
    const color white(1.0f, 1.0f, 1.0f);
    const color blue(0.5f, 0.7f, 1.0f);

    hit_record rec;
    if (world.hit(r, interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + white);
    }

    const vec3 unit_direction = unit_vector(r.direction());
    const auto a = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - a) * white + a * blue;
}
