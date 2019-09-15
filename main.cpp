#include <iostream>
#include <cmath>
#include <limits>
#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "camera.hpp"
#include "random.hpp"

#define WIDTH 400
#define HEIGHT 200
#define SAMPLES_PP 100

Vec3 color(const Ray &r, const Hittable &world) {
    HitRecord rec;
    if (world.hit(r, 0.0, std::numeric_limits<float>::max(), rec)) {
        Vec3 nextTarget = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5 * color(Ray(rec.p, nextTarget - rec.p), world); // Absorbs 50% light
    }
    Vec3 unit_d = normalized(r.direction);
    float gradientParam = 0.5 * (unit_d.y() + 1.0);
    return gradientParam * Vec3(0.5, 0.7, 1.0) + (1 - gradientParam) * Vec3(1, 1, 1);
}

int main() {
    std::cout << "P3\n" << WIDTH << " " << HEIGHT << "\n255" << std::endl;

    // Initialize world
    HittableList hList;
    Sphere s1 = Sphere(Vec3(0, 0, -1), 0.5); // Sphere in the center
    Sphere s2 = Sphere(Vec3(0, -101, -1), 100);
    hList.hittables.push_back(&s1);
    hList.hittables.push_back(&s2);

    // Initialize camera
    Camera cam;

    for (int y = HEIGHT - 1; y >= 0; --y) {
        for (int x = 0; x < WIDTH; ++x) {
            Vec3 pixColor = Vec3(0, 0, 0);
            for (int i = 0; i < SAMPLES_PP; ++i) {    
                float u = float(x + random_float()) / float(WIDTH);
                float v = float(y + random_float()) / float(HEIGHT);
                Ray r = cam.getRay(u, v);
                pixColor += color(r, hList);
            }
            //std::cout << pixColor.x() << std::endl;
            pixColor *= (1.0 / float(SAMPLES_PP)); // Average by samples per pixel
            pixColor *= 255.99; // Scale to 256 bit RGB 
            std::cout << int(pixColor.r()) << " " << int(pixColor.g()) << " " << int(pixColor.b()) << std::endl;
        }
    }
    return 0;
}

