#ifndef RAYTRACING_INTERVAL_H
#define RAYTRACING_INTERVAL_H
#include "utils.h"

struct interval {
public:
    float min, max;
    interval() : min(+infinity), max(-infinity) {}
    interval(const float min, const float max) : min(min), max(max) {}

    double size() const {
        return max - min;
    }

    bool contains(const float x) const {
        return min <= x && x <= max;
    }

    bool surrounds(const float x) const {
        return min < x && x < max;
    }

    float clamp(const float x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const interval empty, universe;
};

inline const interval interval::empty      = interval(+infinity, -infinity);
inline const interval interval::universe   = interval(-infinity, +infinity);

#endif //RAYTRACING_INTERVAL_H