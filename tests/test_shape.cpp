#include <catch2/catch.hpp>

#include "../src/ray.hpp"
#include "../src/shape.hpp"
#include "../src/vectors.hpp"

TEST_CASE("Plane") {
    SECTION("intersect returns false if plane is parallel to ray") {
        const Plane plane{Point3(1.0f, 0.5f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = plane.intersect(ray);

        REQUIRE(!intersection);
    }
    SECTION(
        "intersect returns false if plane isnt parallel but is behind ray "
        "origin") {
        const Plane plane{Point3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f)};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = plane.intersect(ray);

        REQUIRE(!intersection);
    }

    SECTION(
        "intersect returns true and updates intersection if plane isnt "
        "parallel and is in front of ray") {
        const Plane plane{Point3(0.0f, 2.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f)};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = plane.intersect(ray);

        REQUIRE(intersection);
        REQUIRE(intersection->location == Point3(0.0f, 2.0f, 0.0f));
    }
}

TEST_CASE("Sphere") {
    SECTION("intersect returns false if sphere is not on the ray's path") {
        const Sphere sphere{Point3(-1.5f, 3.0f, 0.0f), 1.0f};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = sphere.intersect(ray);

        REQUIRE(!intersection);
    }

    SECTION("intersect returns false if sphere is fully behind ray origin") {
        const Sphere sphere{Point3(-1.5f, -2.0f, 0.0f), 1.0f};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = sphere.intersect(ray);

        REQUIRE(!intersection);
    }

    SECTION(
        "intersect returns true and updates intersection if sphere is on the "
        "ray's path and in front of ray origin") {
        const Sphere sphere{Point3(-0.5f, 2.0f, 0.0f), 1.0f};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = sphere.intersect(ray);

        float expectedIntersectionY = 2.0f - std::sqrt(3.0f) / 2.0f;

        REQUIRE(intersection);
        REQUIRE(intersection->location ==
                Point3(0.0f, expectedIntersectionY, 0.0f));
    }

    SECTION(
        "intersect returns true and updates intersection if ray origin is "
        "inside the sphere") {
        const Sphere sphere{Point3(0.0f, -0.5f, 0.0f), 1.0f};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = sphere.intersect(ray);

        REQUIRE(intersection);
        REQUIRE(intersection->location == Point3(0.0f, 0.5f, 0.0f));
    }
}

TEST_CASE("Scene") {
    SECTION("intersect returns false if nothing in scene") {
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};
        Scene scene;

        auto intersection = scene.intersect(ray);

        REQUIRE(!intersection);
    }

    SECTION("intersect returns false if no shape intersects ray") {
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};
        Scene scene;
        const Sphere sphere{Point3(3.0f, 5.0f, 0.0f), 1.0f};
        const Plane plane{Point3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f)};
        scene.addShape(&sphere);
        scene.addShape(&plane);

        auto intersection = scene.intersect(ray);

        REQUIRE(!intersection);
    }

    SECTION("intersect returns correct intersection if one shape intersects") {
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};
        Scene scene;
        const Sphere sphere{Point3(3.0f, 5.0f, 0.0f), 1.0f};
        const Plane plane{Point3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f)};
        scene.addShape(&sphere);
        scene.addShape(&plane);

        auto intersection = scene.intersect(ray);

        REQUIRE(intersection);
        REQUIRE(intersection->location == Point3(0.0f, 1.0f, 0.0f));
    }

    SECTION(
        "intersect returns closest intersection if several shapes intersect") {
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};
        Scene scene;
        const Sphere sphere{Point3(0.0f, 5.0f, 0.0f), 1.0f};
        const Plane plane{Point3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f)};
        scene.addShape(&sphere);
        scene.addShape(&plane);

        auto intersection = scene.intersect(ray);

        REQUIRE(sphere.intersect(ray));
        REQUIRE(intersection);
        REQUIRE(intersection->location == Point3(0.0f, 1.0f, 0.0f));
    }
}