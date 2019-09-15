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
#include "material.hpp"

#define WIDTH 400
#define HEIGHT 200
#define SAMPLES_PP 100
#define MAX_BOUNCES 50

float gamma_encode(float x) {
    return pow(x, 1.0/2.2);
}

Vec3 color(const Ray &r, const Hittable &world, unsigned bounces) {
    HitRecord rec;
    if (world.hit(r, 0.0001, std::numeric_limits<float>::max(), rec)) {
        Ray scattered;
        Vec3 attenuation;
        if (bounces < MAX_BOUNCES && rec.mat->scatter(r, rec, attenuation, scattered)) {
            return attenuation.elementWiseMul(color(scattered, world, bounces + 1));
        } else {
            return Vec3(0, 0, 0);
        }
    }
    Vec3 unit_d = normalized(r.direction);
    float gradientParam = 0.5 * (unit_d.y() + 1.0);
    return gradientParam * Vec3(0.5, 0.7, 1.0) + (1 - gradientParam) * Vec3(1, 1, 1);
}

int main() {
    // Initialize world
    Lambertian gray = Lambertian(Vec3(0.5, 0.5, 0.5));
    HittableList hList;
    Sphere s1 = Sphere(Vec3(0, 0, -1), 0.5, &gray); // Sphere in the center
    Sphere s2 = Sphere(Vec3(0, -100.5, -1), 100, &gray);
    hList.hittables.push_back(&s1);
    hList.hittables.push_back(&s2);

    // Initialize camera
    Camera cam;

    // Initialize image
    Vec3 image[HEIGHT*WIDTH];

    // Main render loop
    #pragma omp parallel for schedule(dynamic)
    for (int y = HEIGHT - 1; y >= 0; --y) {
        std::cerr << "Rendering row " << HEIGHT - y << std::endl;
        for (int x = 0; x < WIDTH; ++x) {
            Vec3 pixColor = Vec3(0, 0, 0);
            for (int i = 0; i < SAMPLES_PP; ++i) {    
                float u = float(x + random_float()) / float(WIDTH);
                float v = float(y + random_float()) / float(HEIGHT);
                Ray r = cam.getRay(u, v);
                pixColor += color(r, hList, 0);
            }
            pixColor *= (1.0 / float(SAMPLES_PP)); // Average by samples per pixel
            pixColor = Vec3(gamma_encode(pixColor.r()), gamma_encode(pixColor.g()), gamma_encode(pixColor.b()));
            pixColor *= 255.99; // Scale to 256 bit RGB
            image[y * WIDTH + x] = pixColor;
        }
    }

    // Write image to stdout
    std::cout << "P3\n" << WIDTH << " " << HEIGHT << "\n255" << std::endl;
    for (int y = HEIGHT - 1; y >= 0; --y) {
        for (int x = 0; x < WIDTH; ++x) {
            Vec3 pixColor = image[y * WIDTH + x]; 
            std::cout << int(pixColor.r()) << " " << int(pixColor.g()) << " " << int(pixColor.b()) << std::endl;
        }
    }
    return 0;
}

