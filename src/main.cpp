#include "utils.h"

#include "Camera.h"
#include "hittable_list.h"
#include "Sphere.h"

#include <chrono>
#include <iostream>

#include "material.h"

int main(int argc, char *argv[]) {
    const auto start = std::chrono::high_resolution_clock::now();

    hittable_list world;

    auto material_ground    = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center    = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left      = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right     = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<Sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(point3( 0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(point3( 1.0, 0.0, -1.0), 0.5, material_right));

    Camera cam;

    cam.aspect_ratio        = 16.0f / 9.0f;
    cam.image_width         = 1080;
    cam.samples_per_pixel   = 100;
    cam.max_depth           = 50;

    cam.render(world);

    const auto end = std::chrono::high_resolution_clock::now();

    /* Compute duration in seconds */
    const std::chrono::duration<double> elapsed = end - start;
    std::cout << "Total Render Time: " << elapsed.count() << " seconds\n";

    return 0;
}
