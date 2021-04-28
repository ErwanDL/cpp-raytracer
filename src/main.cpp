#include <iostream>
#include <memory>

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
        std::make_shared<Plane>(Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0, 0.0f), // floor
                                wallMat),
        // std::make_shared<Plane>(Point3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 0.0f, 1.0f), // back
        //                         wallMat),
        // std::make_shared<Plane>(Point3(0.0f, 4.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), // ceiling
        //                         wallMat),
        // std::make_shared<Plane>(Point3(-2.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), // left
        //                         Material::Diffuse(Color(0.9f, 0.3f, 0.3f))),
        // std::make_shared<Plane>(Point3(2.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), // right
        //                         Material::Diffuse(Color(0.3f, 0.9f, 0.3f))),
        std::make_shared<Sphere>(Point3(-0.7f, 0.5f, -6.0f), 0.5f, // glossy sphere
                                 Material::Metal(Color(0.9f, 0.6f, 0.4f), 25.0f)),
        std::make_shared<Sphere>(Point3(-0.9f, 1.0f, -8.5f), 1.0f, // diffuse sphere
                                 Material::Diffuse(Color(0.3, 0.4f, 0.9f), 10.0f)),
        std::make_shared<Sphere>(Point3(0.8f, 1.0f, -7.0f), 1.0f, // mirror sphere
                                 Material::Metal(Color(0.7f), 10'000.0f)),
    };

    std::vector<std::shared_ptr<Intersectable>> lights{
        std::make_shared<Sphere>(Point3(4.0f, 5.0f, -8.0f), 1.0f,
                                 Material::Emissive(Color(1.0f, 1.0f, 0.9f), 3.0f)),
        std::make_shared<Sphere>(Point3(0.8f, 0.5f, -5.7f), 0.4f,
                                 Material::Emissive(Color(1.0f, 0.5f, 0.3f), 1.0f)),
    };

    RenderParams params{720, 405, 2, 10, true};

    Scene scene{shapes, lights, params, Color(0.5f)};

    PerspectiveCamera camera{Point3(0.0f, 2.0f, -2.0f), Point3(0.0f, 1.5f, -7.0f), Utils::PI / 4};

    auto render = rayTrace(camera, scene, params);
    saveRenderToPNG(render, "scene.png");

    return 0;
}
