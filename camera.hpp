#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "ray.hpp"
#include "vec3.hpp"

class Camera {
    public:
        Camera() = default;
        Ray getRay(float u, float v) {
            return Ray(origin, lowerLeft + u*horizontal + v*vertical);
        }
    private:
        const Vec3 origin = Vec3(0, 0, 0);
        const Vec3 horizontal = Vec3(4, 0, 0);
        const Vec3 vertical = Vec3(0, 2, 0);
        const Vec3 lowerLeft = Vec3(-2, -1, -1); 
};

#endif
