#include <catch2/catch.hpp>
#include <cmath>

#include "../src/vectors.hpp"

TEST_CASE("Vector2") {
    SECTION("computes correct length squared") {
        const Vector2 v{2.0f};
        REQUIRE(v.lengthSquared() == Approx(8.0f));
    }

    SECTION("computes correct length") {
        const Vector2 v{2.0f, 0.0f};
        REQUIRE(v.length() == Approx(2.0f));
    }

    SECTION("normalize results in length 1") {
        Vector2 v{2.0f, -9.5f};

        v.normalize();

        REQUIRE(v.length() == Approx(1.0f));
    }

    SECTION("normalized returns vector of length 1") {
        const Vector2 v{3.0f};

        const auto vNorm = v.normalized();

        REQUIRE(vNorm.length() == Approx(1.0f));
    }
}

TEST_CASE("Vector3") {
    SECTION("computes correct length squared") {
        const Vector3 v{2.0f};
        REQUIRE(v.lengthSquared() == Approx(12.0f));
    }

    SECTION("computes correct length") {
        const Vector3 v{2.0f, 0.0f, -1.f};
        REQUIRE(v.length() == Approx(std::sqrt(5.0f)));
    }

    SECTION("normalize results in length 1") {
        Vector3 v{2.0f, -9.5f, 1.0f};

        v.normalize();

        REQUIRE(v.length() == Approx(1.0f));
    }

    SECTION("normalized returns vector of length 1") {
        const Vector3 v{3.0f};

        const auto vNorm = v.normalized();

        REQUIRE(vNorm.length() == Approx(1.0f));
    }

    SECTION("dot computes correct dot product") {
        const Vector3 v1{1.0f, 2.0f, -2.5f};
        const Vector3 v2{5.0f};

        const float dotProduct = v1.dot(v2);

        REQUIRE(dotProduct == Approx(2.5f));
    }

    SECTION("dot computes 0 if orthogonal") {
        const Vector3 v1{1.0f, 2.0f, 0.0f};
        const Vector3 v2{0.0f, 0.0f, -4.5f};

        const float dotProduct = v1.dot(v2);

        REQUIRE(dotProduct == Approx(0.0f));
    }

    SECTION("cross computes correct cross product") {
        const Vector3 v1{1.0f, 1.0f, 0.0f};
        const Vector3 v2{0.0f, 1.0f, 0.0f};

        const auto v3 = v1.cross(v2);

        REQUIRE(v3 == Vector3(0.0f, 0.0f, 1.0f));
    }

    SECTION("reflected produces correct reflection") {
        const Vector3 v{1.0f};
        const Vector3 normal{0.0f, 0.0f, 2.0f};

        const auto reflected = v.reflected(normal);

        REQUIRE(reflected == Vector3(-1.0f, -1.0f, 1.0f));
    }
}