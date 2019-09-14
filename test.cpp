#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "vec3.hpp"

TEST_CASE( "Vector Creation ") {
    Vec3 v = Vec3(1, 2, 3);
    CHECK( v.x() == 1.0 );
    CHECK( v.y() == 2.0 );
    CHECK( v.z() == 3.0 );
}

TEST_CASE(" Vector Addtion ") {
    Vec3 v1 = Vec3(2, 3, 4);
    v1 += Vec3(5, 6, 7);
    CHECK( v1.r() == 7.0 );
    CHECK( v1.g() == 9.0 );
    CHECK( v1.b() == 11.0 );
}

