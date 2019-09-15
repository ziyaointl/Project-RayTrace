#ifndef RANDOM_HPP
#define RANDOM_HPP
#include <random>
#include <functional>

float random_float() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<float()> rand_generator = std::bind(distribution, generator);
    return rand_generator();
}

#endif
