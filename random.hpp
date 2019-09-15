#ifndef RANDOM_HPP
#define RANDOM_HPP
#include <random>
#include <functional>
#include "vec3.hpp"

float random_float() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<float()> rand_generator = std::bind(distribution, generator);
    return rand_generator();
}

// Returns a vector in the unit sphere
// By picking a random point within a cube and testing if the point is in the sphere
Vec3 random_in_unit_sphere() {
    Vec3 p;
    do {
        p = 2.0 * Vec3(random_float(), random_float(), random_float());
    } while (p.lengthSq() >= 1.0);
    return p;
}

#endif
