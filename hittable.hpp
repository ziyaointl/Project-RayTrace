#ifndef HITTABLE_HPP
#define HITTABLE_HPP
#include "ray.hpp"
#include "vec3.hpp"

struct HitRecord {
    float t;
    Vec3 p, normal;
}

class Hittable {
    public:
        virtual bool hit(const Ray &r, float minT, float maxT, HitRecord &rec) const = 0;
}

#endif
