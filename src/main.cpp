#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "renderer.hpp"
#include "shape.hpp"
#include "utils.hpp"
#include "vectors.hpp"

int main() {
    Scene scene;
    const Material wallWhite{Color(0.8f)};

    auto floor{
        std::make_shared<Plane>(Point3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 1.0f, 0.0f), wallWhite)};
    scene.addShape(floor);

    auto backWall{
        std::make_shared<Plane>(Point3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 0.0f, -1.0f), wallWhite)};
    scene.addShape(backWall);

    auto cameraWall{
        std::make_shared<Plane>(Point3(0.0f, 0.0f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), wallWhite)};
    scene.addShape(cameraWall);

    auto ceiling{
        std::make_shared<Plane>(Point3(0.0f, 9.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), wallWhite)};
    scene.addShape(ceiling);

    auto leftWall{std::make_shared<Plane>(Point3(-4.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f),
                                          Material(Color(0.8f, 0.4f, 0.4f)))};
    scene.addShape(leftWall);

    auto rightWall{std::make_shared<Plane>(Point3(4.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f),
                                           Material(Color(0.4f, 0.8f, 0.4f)))};
    scene.addShape(rightWall);

    auto metalSphere{std::make_shared<Sphere>(Point3(-2.0f, 1.0f, -6.0f), 1.0f,
                                              Material(Color(0.8f, 0.6f, 0.4f), 10.0f, true))};
    scene.addShape(metalSphere);

    auto blueSphere{std::make_shared<Sphere>(Point3(0.0f, 1.0f, -6.0f), 1.0f,
                                             Material(Color(0.35f, 0.45f, 0.65f)))};
    scene.addShape(blueSphere);

    auto mirrorSphere{std::make_shared<Sphere>(Point3(2.0f, 1.0f, -6.0f), 1.0f,
                                               Material(Color::WHITE, 1000.0f, true))};
    scene.addShape(mirrorSphere);

    LightRack lightRack;
    const AmbientLight ambient{Color(0.5f)};
    lightRack.addLight(ambient);
    const PointLight light{Point3(3.0f, 5.5f, 0.0f), Color(0.4f)};
    lightRack.addLight(light);
    const PointLight light2{Point3(-3.0f, 5.5f, 0.0f), Color(0.4f)};
    lightRack.addLight(light2);

    constexpr int width{1280};
    constexpr int height{720};

    PerspectiveCamera camera{Point3(0.0f, 4.0f, 0.0f), Vector3(0.0f, 0.0f, -10.0f),
                             Vector3(0.0f, 1.0f, 0.0f), Math::PI / 8,
                             static_cast<float>(width) / static_cast<float>(height)};

    Renderer renderer{scene, lightRack, width, height};

    auto render = renderer.rayTrace(camera, 2, 200);
    renderer.saveRender(render, "scene.ppm");

    return 0;
}
