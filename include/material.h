#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H

#include "hittable.h"

class material {
public:
    virtual ~material() = default;
    virtual bool scatter(
        const ray           &r_in,
        const hit_record    &rec,
        color               &attenuation,
        ray                 &scattered) const {
        return false;
    }
};

class lambertian : public material {
public:
    lambertian(const color &albedo) : albedo(albedo) {}
    ~lambertian() override {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {
        auto scatter_direction = rec.normal + random_unit_vector();

        if (scatter_direction.near_zero()) {
            scatter_direction = rec.normal;
        }

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class metal : public material {
public:
    metal(const color &albedo, const real fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
    ~metal() override {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = unit_vector(reflected) + fuzz * random_unit_vector();
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }
private:
    color albedo;
    real fuzz;
};

class dielectric : public material {
public:
    dielectric(real refraction_index) : refraction_index(refraction_index) {}
    ~dielectric() override {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {
        attenuation = color(1.0, 1.0, 1.0);
        const double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

        const vec3 unit_direction = unit_vector(r_in.direction());
        const vec3 refracted = refract(unit_direction, rec.normal, ri);

        scattered = ray(rec.p, refracted);
        return true;
    }

private:
    real refraction_index;
};
#endif //RAYTRACING_MATERIAL_H