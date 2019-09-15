#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <cmath>
#include "catch.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"

TEST_CASE("Vector Creation") {
    Vec3 v = Vec3(1, 2, 3);
    CHECK( v.x() == 1.0 );
    CHECK( v.y() == 2.0 );
    CHECK( v.z() == 3.0 );
}

TEST_CASE("Vector Addition") {
    Vec3 v1 = Vec3(2, 3, 4);
    v1 += Vec3(5, 6, 7);
    CHECK(v1.r() == 7.0);
    CHECK(v1.g() == 9.0);
    CHECK(v1.b() == 11.0);
    
    Vec3 v2 = v1 + Vec3(-10, -20, 0);
    CHECK (v2.x() == -3);
    CHECK (v2.y() == -11);
    CHECK (v2.z() == 11);

    // Check v1 has not been modified 
    CHECK(v1.r() == 7.0);
    CHECK(v1.g() == 9.0);
    CHECK(v1.b() == 11.0);
}

TEST_CASE("Vector Negation") {
    Vec3 v = Vec3(1, 2, 3);
    v = -v;
    CHECK(v.x() == -1);
    CHECK(v.y() == -2);
    CHECK(v.z() == -3);
}

TEST_CASE("Comparison") {
    Vec3 v1 = Vec3(1, 2, 3);
    Vec3 v2 = Vec3(4, 5, 6);
    Vec3 v3 = Vec3(1, 2, 3) + Vec3(3, 3, 3);
    CHECK(v3 == v2);
    CHECK(v3 != v1);
    CHECK(v1 != v2);
}

TEST_CASE("Euclidean Distance") {
    Vec3 v1 = Vec3(3, 4, 0);
    CHECK(v1.length() == Approx(5));
    
    Vec3 v2 = Vec3(sqrt(2), 0, sqrt(2));
    CHECK(v2.length() == Approx(2));

    Vec3 v3 = Vec3(0, 0, 0);
    CHECK(v3.length() == 0);

    Vec3 v4 = Vec3(1, 1, 2);
    CHECK(v4.length() == Approx(sqrt(6)));
}

TEST_CASE("Normalization") {
    Vec3 v1 = Vec3(1, 1, 0);
    Vec3 v2 = normalized(v1);
    CHECK(v2.x() == Approx(1.0/sqrt(2.0)));
    CHECK(v2.y() == Approx(1.0/sqrt(2.0)));
    CHECK(v2.z() == 0.0);
    
    Vec3 v3 = Vec3(0, 0, 3);
    Vec3 v4 = normalized(v3);
    CHECK(v4.r() == 0.0);
    CHECK(v4.g() == 0.0);
    CHECK(v4.b() == 1.0);
    CHECK(v3 == Vec3(0, 0, 3));
}

TEST_CASE("Division") {
    Vec3 v1 = Vec3(1, 2, 3);
    Vec3 v2 = v1 / 10;
    CHECK(v2 == Vec3(0.1, 0.2, 0.3));
}

TEST_CASE("Ray Creation") {
    Ray r = Ray(Vec3(0, 0, 0), Vec3(0, 0, 1));
    CHECK(r.origin == Vec3(0, 0, 0));
    CHECK(r.direction == Vec3(0, 0, 1));
}

TEST_CASE("Vector Multiplication") {
    Vec3 v = Vec3(4, 5, 6);
    v *= 2;
    CHECK(v.x() == 8);
    CHECK(v.y() == 10);
    CHECK(v.z() == 12);
    Vec3 v2 = Vec3(0, 2, 4);
    Vec3 v3 = v2 * 4;
    Vec3 v4 = 4 * v2;
    CHECK(v3 == v4);
}

TEST_CASE("Dot Product") {
    Vec3 v1 = Vec3(1, 2, 3);
    Vec3 v2 = Vec3(4, 5, 6);
    CHECK(v1 * v2 == 32);
}

TEST_CASE("Sphere Normal") {
    Sphere s = Sphere(Vec3(0, 0, 0), 1);
    Vec3 pt = Vec3(0, 0, 1);
    CHECK(s.normalAt(pt) == pt);    
}


