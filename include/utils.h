#ifndef RAYTRACING_UTILS_H
#define RAYTRACING_UTILS_H

#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

using std::make_shared;
using std::shared_ptr;

using real = double;

constexpr double infinity   = std::numeric_limits<double>::infinity();
constexpr double pi         = 3.1415926535897932385;

inline real degrees_to_radians(real degrees) {
    return static_cast<real>(degrees * pi / 180.0);
}

inline real random_real() {
    static std::uniform_real_distribution<real> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline real random_real(const real min, const real max) {
    return min + (max - min) * random_real();
}

/* Common Headers */
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif //RAYTRACING_UTILS_H