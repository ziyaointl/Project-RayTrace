#ifndef VEC3_HPP
#define VEC3_HPP
#include <cmath>

class Vec3 {
    public:
        Vec3() = default;
        Vec3(float i, float j, float k) {
            elements[0] = i;
            elements[1] = j;
            elements[2] = k;
        }
        float x() const {
            return elements[0];
        }
        float y() const {
            return elements[1];
        }
        float z() const {
            return elements[2];
        }
        float r() const {
            return elements[0];
        }
        float g() const {
            return elements[1];
        }
        float b() const {
            return elements[2];
        }

        Vec3& operator+=(const Vec3 &rhs) {
            elements[0] += rhs.x();
            elements[1] += rhs.y();
            elements[2] += rhs.z();
            return *this;
        }

        Vec3& operator*=(const float &t) {
            for (int i = 0; i < 3; ++i) {
                elements[i] *= t;
            }
            return *this;
        }
        
        float operator*(const Vec3 &rhs) const {
            return x()*rhs.x() + y()*rhs.y() + z()*rhs.z();
        }

        Vec3 operator+(const Vec3 &rhs) const {
            Vec3 sum = *this;
            sum += rhs;
            return sum;
        }

        Vec3 operator-(const Vec3 &rhs) const {
            return *this + (-rhs);
        }

        Vec3 operator-() const {
            return Vec3(-elements[0], -elements[1], -elements[2]);
        }

        Vec3 operator/(const float &rhs) const {
            return Vec3(elements[0] / rhs, elements[1] / rhs, elements[2] / rhs);
        }

        bool operator==(const Vec3 &rhs) const {
            return elements[0] == rhs.x()
                && elements[1] == rhs.y()
                && elements[2] == rhs.z();
        }

        bool operator!=(const Vec3 &rhs) const {
            return !(*this == rhs);
        }
        
        float lengthSq() const {
            return x() * x() + y() * y() + z() * z();
        }

        float length() const {
            return sqrt(lengthSq());
        }
    private:
        float elements[3] = {0.0, 0.0, 0.0};
};

Vec3 normalized(const Vec3 v) {
    float l = v.length();
    return v / l;
}

Vec3 operator* (const Vec3 &lhs, const float &rhs) {
    return Vec3(lhs.x() * rhs, lhs.y() * rhs, lhs.z() * rhs);
}

Vec3 operator* (const float &lhs, const Vec3 &rhs) {
    return rhs * lhs;
}


#endif
