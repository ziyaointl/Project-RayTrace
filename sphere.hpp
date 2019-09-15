#ifndef SPHERE_HPP
#define SPHERE_HPP
#include "vec3.hpp"

struct Sphere {
    Sphere(const Vec3 &center, float radius) {
        this->center = center;
        this->radius = radius;
    }
    Vec3 center;
    float radius;

    Vec3 normalAt(const Vec3 &pt) {
        return normalized(pt - center);
    }
};

#endif
