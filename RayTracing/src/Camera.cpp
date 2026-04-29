#include "Camera.h"
#include "Image.h"
#include "material.h"

void Camera::set_resolution(const int width, const int height) {
    image_width = width;
    image_height = std::max(1, height);
    aspect_ratio = static_cast<double>(width) / static_cast<double>(height);
}

void Camera::render(const hittable &world) {
    initialize();
    Image image(image_width, image_height);

    auto start = std::chrono::steady_clock::now();
    double smoothed_avg = 0.0;

    for (int j = 0; j < image_height; j++) {
        print_progress(j, image_height, start, smoothed_avg);
        for (int i = 0; i < image_width; i++) {
            color pixel_color(0, 0, 0);
            for (int sample = 0; sample < samples_per_pixel; sample++) {
                ray r = get_ray(i, j);
                pixel_color += ray_color(r, max_depth, world);
            }
            image.set_pixel(i, j, pixel_samples_scale * pixel_color);
        }
    }

    std::ofstream out("../image.ppm");
    image.write_ppm(out);
}

void Camera::render_to_buffer(const hittable &world, std::vector<uint32_t> &framebuffer, std::atomic<bool> &cancel_flag) {
    initialize();
    framebuffer.resize(image_width * image_height);

    auto start = std::chrono::steady_clock::now();
    double smoothed_avg = 0.0;

    for (int j = 0; j < image_height; j++) {
        float progress = (float)j / (float)image_height;
        if (progress_callback) {
            progress_callback(progress);
        }

        if (cancel_flag) return;
        for (int i = 0; i < image_width; i++) {
            if (cancel_flag) return;
            color pixel_color(0, 0, 0);

            for (int sample = 0; sample < samples_per_pixel; sample++) {
                if (cancel_flag) return;
                ray r = get_ray(i, j);
                pixel_color += ray_color(r, max_depth, world);
            }

            pixel_color *= pixel_samples_scale;

            // Write directly into GPU-ready format
            int flipped_j = image_height - 1 - j;
            framebuffer[flipped_j * image_width + i] = to_rgba(pixel_color);
        }
    }
}


void Camera::print_progress(int current_line,
                            int total_lines,
                            const std::chrono::steady_clock::time_point& start,
                            double &smoothed_avg) {
    const int bar_width = 30;

    auto now = std::chrono::steady_clock::now();
    auto elapsed = now - start;

    if (current_line == 0) return;

    int remaining_lines = total_lines - current_line;
    double progress = (double)current_line / total_lines;

    double avg_time =
        std::chrono::duration<double>(elapsed).count() / current_line;

    // smoothing
    if (smoothed_avg == 0.0)
        smoothed_avg = avg_time;
    else
        smoothed_avg = 0.9 * smoothed_avg + 0.1 * avg_time;

    double eta_seconds = smoothed_avg * remaining_lines;

    int total_sec = static_cast<int>(eta_seconds);
    int minutes = total_sec / 60;
    int seconds = total_sec % 60;

    int pos = bar_width * progress;

    std::clog << "\r[";
    for (int i = 0; i < bar_width; i++) {
        if (i < pos) std::clog << "█";
        else std::clog << "-";
    }

    std::clog << "] "
              << int(progress * 100.0) << "% "
              << "| ETA: "
              << minutes << " min "
              << seconds << " sec   "
              << std::flush;
}

void Camera::initialize() {
    // image_height            = static_cast<int>(image_width / aspect_ratio);
    // image_height            = (image_height < 1) ? 1 : image_height;

    pixel_samples_scale     = 1.0 / samples_per_pixel;

    center = lookfrom;

    // Determine viewport dimensions
    // real    focal_length      = (lookfrom - lookat).length();
    auto    theta             = degrees_to_radians(vfov);
    auto    h                 = std::tan(theta / 2.0);
    real    viewport_height   = 2.0 * h * focus_dist;
    real    viewport_width    = viewport_height * (static_cast<double>(image_width) / image_height);

    // Calculate the u, v, w unit basis vectors for the camera coordinate frame
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    // Calculate vectors across horizontal and down the vertical viewport edges
    auto viewport_u = viewport_width * u;
    auto viewport_v = viewport_height * -v;

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    pixel_delta_u   = viewport_u / image_width;
    pixel_delta_v   = viewport_v / image_height;

    // Calculate location of the upper left pixel (0, 0)
    auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
    pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

    // Calculate the camera defocus disk basis vectors
    auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
}

ray Camera::get_ray(const int i, const int j) const {
    // Construct a camera ray originating from the defocus disk and directed at a randomly
    // sampled point around the pixel location i, j.
    auto offset = sample_square();
    auto pixel_sample = pixel00_loc
                        + (i + offset.x()) * pixel_delta_u
                        + (j + offset.y()) * pixel_delta_v;
    auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
}

vec3 Camera::sample_square() const {
    /* Return vector to a random point in the [-.5, -.5] - [+.5, +.5] unit square */
    return vec3(random_real() - 0.5f, random_real() - 0.5f, 0);
}

point3 Camera::defocus_disk_sample() const {
    auto p = random_in_unit_disk();
    return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}

color Camera::ray_color(const ray &r, int depth, const hittable &world) const {
    if (depth <=0) {
        return color(0, 0, 0);
    }

    const color white(1.0f, 1.0f, 1.0f);
    const color blue(0.5f, 0.7f, 1.0f);

    hit_record rec;
    if (world.hit(r, interval(0.001, infinity), rec)) {
        if (render_mode == RenderMode::Normals) {
            vec3 N = rec.normal;
            return 0.5f * color(N.x() + 1, N.y() + 1, N.z() + 1);
        }

        ray scattered;
        color attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, depth - 1, world);
        }
        return white;
    }

    const vec3 unit_direction = unit_vector(r.direction());
    const auto a = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - a) * white + a * blue;
}
