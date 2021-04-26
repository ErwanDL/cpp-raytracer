#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include "camera.hpp"
#include "intersectable.hpp"
#include "material.hpp"
#include "save_render.hpp"
#include "scene.hpp"
#include "trace.hpp"
#include "utils.hpp"
#include "vector3.hpp"

int main() {
    Material wallMat{Material::Diffuse(0.9f)};
    std::vector<std::shared_ptr<Intersectable>> shapes{
        std::make_shared<Plane>(Point3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 1.0f, 0.0f), // floor
                                Material::Diffuse(Color(0.35f, 0.55f, 0.3f))),
        // std::make_shared<Plane>(Point3(0.0f, 0.0f, -10.0f),
        //                         Vector3(0.0f, 0.0f, -1.0f), // front wall
        //                         wallMat),
        // std::make_shared<Plane>(Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), // back
        // wall
        //                         wallMat),
        // std::make_shared<Plane>(Point3(0.0f, 9.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), // ceiling
        //                         wallMat),
        // std::make_shared<Plane>(Point3(-4.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), // left
        // wall
        //                         Material(Color(0.9f, 0.3f, 0.3f))),
        // std::make_shared<Plane>(Point3(4.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), // right
        // wall
        //                         Material(Color(0.3f, 0.9f, 0.3f))),
        std::make_shared<Sphere>(Point3(-2.0f, 1.0f, -6.0f), 1.0f, // glossy sphere
                                 Material::Metal(Color(0.9f, 0.6f, 0.4f), 25.0f)),
        std::make_shared<Sphere>(Point3(0.0f, 1.0f, -7.0f), 1.0f, // diffuse sphere
                                 Material::Diffuse(Color(0.3, 0.4f, 0.9f), 10.0f)),
        std::make_shared<Sphere>(Point3(2.0f, 1.0f, -6.0f), 1.0f, // mirror sphere
                                 Material::Metal(Color(0.7f), 10'000.0f)),
    };

    std::vector<std::shared_ptr<Intersectable>> lights{
        std::make_shared<Sphere>(Point3(1.0f, 7.0f, -10.0f), 1.0f,
                                 Material::Emissive(Color(1.0f, 1.0f, 0.9f), 5.0f)),
        std::make_shared<Sphere>(Point3(0.0f, 0.5f, -5.0f), 0.5f,
                                 Material::Emissive(Color::WHITE, 3.0f)),
    };

    RenderParams params{720, 405, 2, 20, true};

    Scene scene{shapes, lights, params, Color(0.02f, 0.05f, 0.1f)};

    PerspectiveCamera camera{Point3(0.0f, 3.0f, 0.0f), Point3(0.0f, 0.5f, -7.0f), Utils::PI / 5};

    auto render = rayTrace(camera, scene, params);
    saveRenderToPNG(render, "scene.png");

    return 0;
}
