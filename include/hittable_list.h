#ifndef RAYTRACING_HITTABLE_LIST_H
#define RAYTRACING_HITTABLE_LIST_H

#include "utils.h"
#include "hittable.h"
#include <vector>

struct hittable_list : hittable {
    std::vector<std::shared_ptr<hittable>> objects;

    hittable_list() = default;
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    bool hit(const ray &ray, interval ray_t, hit_record &rec) const override {
        hit_record temp_rec;
        bool hit_anything    = false;
        auto closest_so_far  = ray_t.max;

        for (const auto &object : objects) {
            if (object->hit(ray, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything    = true;
                closest_so_far  = temp_rec.t;
                rec             = temp_rec;
            }
        }
        return hit_anything;
    }
};

#endif //RAYTRACING_HITTABLE_LIST_H