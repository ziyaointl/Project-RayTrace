#include <iostream>
#include <cmath>
#include <limits>
#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"

#define WIDTH 400
#define HEIGHT 200

Vec3 color(const Ray &r, const Hittable &world) {
    HitRecord rec;
    if (world.hit(r, 0.0, std::numeric_limits<float>::max(), rec)) {
        return (rec.normal + Vec3(1, 1, 1)) * 0.5; // Scale colors
    }
    Vec3 unit_d = normalized(r.direction);
    float gradientParam = 0.5 * (unit_d.y() + 1.0);
    return gradientParam * Vec3(0.5, 0.7, 1.0) + (1 - gradientParam) * Vec3(1, 1, 1);
}

int main() {
    std::cout << "P3\n" << WIDTH << " " << HEIGHT << "\n255" << std::endl;
    // These vectors are in uv space
    const Vec3 origin = Vec3(0, 0, 0);
    const Vec3 horizontal = Vec3(4, 0, 0);
    const Vec3 vertical = Vec3(0, 2, 0);
    const Vec3 lowerLeft = Vec3(-2, -1, -1);

    // Initialize world
    HittableList hList;
    Sphere s1 = Sphere(Vec3(0, 0, -1), 0.5); // Sphere in the center
    Sphere s2 = Sphere(Vec3(0, -101, -1), 100);
    hList.hittables.push_back(&s1);
    hList.hittables.push_back(&s2);

    for (int y = HEIGHT - 1; y >= 0; --y) {
        for (int x = 0; x < WIDTH; ++x) {
            float u = float(x) / WIDTH;
            float v = float(y) / HEIGHT;
            Ray r = Ray(origin, lowerLeft + u * horizontal + v * vertical);
            Vec3 pixColor = color(r, hList) * 255.99;
            std::cout << int(pixColor.r()) << " " << int(pixColor.g()) << " " << int(pixColor.b()) << std::endl;
        }
    }
    return 0;
}

