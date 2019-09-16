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

#define WIDTH 200
#define HEIGHT 100
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
    Metal m = Metal(Vec3(0.8, 0.8, 0.8), 0);
    Dielectric d = Dielectric(1.8);
    HittableList hList;
    Sphere *s1 = new Sphere(Vec3(-1, 0, -1), 0.5, &m);
    Sphere *s2 = new Sphere(Vec3(1, 0, -1), 0.3, &d);
    Sphere *s3 = new Sphere(Vec3(0, -1000.3, -1), 1000, &gray);
    hList.hittables.push_back(s1);
    hList.hittables.push_back(s2);
    hList.hittables.push_back(s3);
    for (int i = 0; i < 100; ++i) {
        float mat = random_float();
        float radius = random_float() * 0.5;
        Vec3 location = Vec3((random_float() - 0.5) * 10 - 0.5, 0, -5 * (random_float()) - 1.0);
        if (mat < 0.3) {
            hList.hittables.push_back(new Sphere(location, radius, &gray));
        } else if (mat < 0.6) {
            hList.hittables.push_back(new Sphere(location, radius, &m));
        } else {
            hList.hittables.push_back(new Sphere(location, radius, &d));
        }
    }

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

    // Free memory
    for (Hittable *h : hList.hittables) {
        delete h;
    }
    return 0;
}

