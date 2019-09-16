#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include <cassert>
#include <iostream>
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
// Returns true if not total internal reflection
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

// Schlick's approximation
// https://en.wikipedia.org/wiki/Schlick%27s_approximation#cite_note-1
// Assuming v is incident ray, n is normal, and both are unit vectors
float reflectionProb(float cosine, float refIndex) {
    float r0 = (1- refIndex) / (1 + refIndex);
    r0 *= r0;
    //std::cout << r0 + (1 - r0) * pow(1 - cos, 5) << std::endl;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
}

class Dielectric : public Material {
    public:
        Dielectric(float refractionIndex) : refractionIndex(refractionIndex) {}
        virtual bool scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override {
            Vec3 normal = rec.normal;
            Vec3 refracted;
            float n1, n2, cosine;

            if (normal * in.direction <= 0) { // Light is shining in the material
                n1 = 1.0;
                n2 = refractionIndex;
                cosine = -in.direction * normal;
            } else { // Light is shining out of the material
                normal = -normal;
                n1 = refractionIndex;
                n2 = 1.0;
                cosine = refractionIndex * in.direction * rec.normal;
            }
            bool refractionSuccess = refract(in.direction, normal, n1, n2, refracted);
            // Randomly choose to refract or reflect
            if (refractionSuccess && random_float() > reflectionProb(cosine, refractionIndex)) {
                scattered = Ray(rec.p, refracted);
            } else {
                scattered = Ray(rec.p, reflect(in.direction, normal));
            }

            attenuation = Vec3(1.0, 1.0, 1.0); // Does not attenuate
            return true;
        }
    float refractionIndex;
};
#endif
