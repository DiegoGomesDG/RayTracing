#include "utils.h"

#include "Camera.h"
#include "hittable_list.h"
#include "Sphere.h"

int main(int argc, char *argv[]) {

    hittable_list world;

    world.add(make_shared<Sphere>(point3(0,0, -1), 0.5));
    world.add(make_shared<Sphere>(point3(0,-100.5,-1), 100));

    Camera cam;

    cam.aspect_ratio    = 16.0f / 9.0f;
    cam.image_width     = 400;

    cam.render(world);

    return 0;
}
