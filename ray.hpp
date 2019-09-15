#ifndef RAY_HPP
#define RAY_HPP
#include "vec3.hpp"

struct Ray {
    Vec3 origin, direction;
    Ray() = default;
    Ray(const Vec3 &origin, const Vec3 &direction) {
        this->origin = origin;
        this->direction = normalized(direction);
    }
    Vec3 pointAt(const float t) const {
        return origin + t * direction;
    }
};

#endif
