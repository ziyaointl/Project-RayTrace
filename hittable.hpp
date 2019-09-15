#ifndef HITTABLE_HPP
#define HITTABLE_HPP
#include "ray.hpp"
#include "vec3.hpp"

class Material;

struct HitRecord {
    float t;
    Vec3 p, normal;
    Material *mat;
};

class Hittable {
    public:
        virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const = 0;
};

#endif
