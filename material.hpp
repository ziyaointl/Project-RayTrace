#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include <cassert>
#include "ray.hpp"
#include "random.hpp"
#include "hittable.hpp"

class Material {
    public:
        // Returns true if a scattered ray is produced
        // If scatters, updates hit record, attenuation, and the scattered ray
        virtual bool scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material {
    public:
        Lambertian(const Vec3 &albedo): albedo(albedo) { }
        // Always scatter
        // Each bounce attenuates the color
        virtual bool scatter(const Ray&, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override {
            Vec3 nextTarget = rec.p + rec.normal + random_in_unit_sphere();
            scattered = Ray(rec.p, nextTarget - rec.p);
            attenuation = albedo;
            return true;
        }
        Vec3 albedo; // RGB Vector with values [0.0, 1.0]
};

Vec3 reflect(const Vec3 &v, const Vec3 &n) {
    return v - 2 * (v * n) * n;
}

class Metal : public Material {
    public:
        Metal(const Vec3 &albedo, float fuzziness = 0) : albedo(albedo) {
            assert(fuzziness <= 1);
            this->fuzziness = fuzziness;
        }
        virtual bool scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override {
            Vec3 reflectedDirection = reflect(in.direction, rec.normal);
            scattered = Ray(rec.p, reflectedDirection + fuzziness*random_in_unit_sphere());
            attenuation = albedo;
            return scattered.direction * rec.normal > 0; // Why this check tho
        }
        Vec3 albedo; // RGB Vector with values [0.0, 1.0]
        float fuzziness;
};

// Vector form of Snell's law
// https://en.wikipedia.org/wiki/Snell%27s_law#Vector_form
// Assuming v is incident ray, n is normal, and both are unit vectors
// Returns the refracted direction
bool refract(const Vec3 &v, const Vec3 &n, float n1, float n2, Vec3 &refracted) {
    float r = n1 / n2;
    float c = -n * v;
    float discriminant = 1 - r*r*(1 - c*c);
    if (discriminant > 0) {
        refracted = r*v + (r*c - sqrt(discriminant)) * n;
        return true;
    }
    return false;
}


#endif
