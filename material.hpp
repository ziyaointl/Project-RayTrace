#ifndef MATERIAL_HPP
#define MATERIAL_HPP
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

#endif
