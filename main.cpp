#include <iostream>
#include <cmath>
#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"

#define WIDTH 400
#define HEIGHT 200

float hit(const Sphere &s, const Ray &r) {
    Vec3 oc = r.origin - s.center;
    float a = r.direction * r.direction;
    float b = 2.0 * r.direction * oc;
    float c = oc * oc - s.radius * s.radius;
    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return -1;
    }
    return (-b - sqrt(discriminant)) / (2.0*a);
}

Vec3 color(const Ray &r) {
    Sphere s = Sphere(Vec3(0, 0, -1), 0.5);
    float t = hit(s, r);
    if (t > 0.0) {
        Vec3 normal = s.normalAt(r.pointAt(t));
        return (normal + Vec3(1, 1, 1)) * 0.5; // Scale colors
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

    for (int y = HEIGHT - 1; y >= 0; --y) {
        for (int x = 0; x < WIDTH; ++x) {
            float u = float(x) / WIDTH;
            float v = float(y) / HEIGHT;
            Ray r = Ray(origin, lowerLeft + u * horizontal + v * vertical);
            Vec3 pixColor = color(r) * 255.99;
            std::cout << int(pixColor.r()) << " " << int(pixColor.g()) << " " << int(pixColor.b()) << std::endl;
        }
    }
    return 0;
}

