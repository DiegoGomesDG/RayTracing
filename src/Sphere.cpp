#include "Sphere.h"

Sphere::Sphere(const point3 &center, float radius, shared_ptr<material> mat) {
    this->center = center;
    this->radius = std::fmax(0, radius);
    this->mat = mat;
}

bool Sphere::hit(const ray &ray, interval ray_t, hit_record &rec) const {
    vec3 oc     = center - ray.origin();
    auto a      = ray.direction().length_squared();
    auto h      = dot(ray.direction(), oc);
    auto c      = oc.length_squared() - radius * radius;

    auto discr  = h * h - a * c;

    if (discr < 0) {
        return false;
    }

    auto sqrtd  = sqrt(discr);

    /* Find the nearest root that lies in the acceptable range */
    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root))
            return false;
    }

    rec.t               = root;
    rec.p               = ray.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(ray, outward_normal);
    rec.mat             = mat;

    return true;

}