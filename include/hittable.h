#ifndef RAYTRACING_HITTABLE_H
#define RAYTRACING_HITTABLE_H

#include "utils.h"

struct hit_record {
    point3 p;
    vec3 normal;
    float t;
    bool front_face;

    void set_face_normal(const ray &ray, const vec3 &outward_normal) {
        front_face = dot(ray.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

struct hittable {
    virtual ~hittable() = default;
    virtual bool hit(const ray &ray,
                    float ray_tmin,
                    float ray_tmax,
                    hit_record &rec) const = 0;
};

#endif //RAYTRACING_HITTABLE_H