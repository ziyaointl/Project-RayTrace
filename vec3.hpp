#ifndef VEC3_HPP
#define VEC3_HPP
class Vec3 {
    public:
        float elements[3] = {0.0, 0.0, 0.0};
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

};

static Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs) {
    Vec3 sum = lhs;
    sum += rhs;
    return sum;
}
#endif
