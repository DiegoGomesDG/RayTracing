#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include "hittable.h"

class Sphere : public hittable {
public:
    Sphere(const point3 &center, float radius);
    bool hit(const ray &ray, float ray_tmin, float ray_tmax, hit_record &rec) const override;

private:
    point3 center;
    float radius;
};

#endif //RAYTRACING_SPHERE_H