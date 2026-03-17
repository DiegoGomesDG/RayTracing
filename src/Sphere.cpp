#include "Sphere.h"

Sphere::Sphere(const point3 &center, float radius) {
    this->center = center;
    this->radius = std::fmax(0, radius);
}

bool Sphere::hit(const ray &ray, float ray_tmin, float ray_tmax, hit_record &rec) const {
    vec3 oc = center - ray.origin();
    auto a = ray.direction().length_squared();
    auto h = dot(ray.direction(), oc);
    auto c = oc.length_squared() - radius * radius;

    auto discr = h * h - a * c;

    if (discr < 0) {
        return false;
    }

    auto sqrtd = sqrt(discr);

    /* Find the nearest root that lies in the acceptable range */
    auto root = (h - sqrtd) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
        root = (h + sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root)
            return false;
    }

    rec.t = root;
    rec.p = ray.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(ray, outward_normal);

    return true;

}