#include "utils.h"

#include "Camera.h"
#include "hittable_list.h"
#include "Sphere.h"

#include <chrono>
#include <iostream>

int main(int argc, char *argv[]) {
    const auto start = std::chrono::high_resolution_clock::now();

    hittable_list world;

    world.add(make_shared<Sphere>(point3(0.0f, 0.0f, -1.0f), 0.5));
    world.add(make_shared<Sphere>(point3(0.0f, -100.5f, -1.0f), 100));

    Camera cam;

    cam.aspect_ratio        = 16.0f / 9.0f;
    cam.image_width         = 600;
    cam.samples_per_pixel   = 100;
    cam.max_depth           = 50;

    cam.render(world);

    const auto end = std::chrono::high_resolution_clock::now();

    /* Compute duration in seconds */
    const std::chrono::duration<double> elapsed = end - start;
    std::cout << "Total Render Time: " << elapsed.count() << " seconds\n";

    return 0;
}
