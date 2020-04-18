#include <chrono>
#include <iostream>
#include <string>

#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "renderer.hpp"
#include "shape.hpp"
#include "supersampling.hpp"
#include "utils.hpp"
#include "vectors.hpp"

int main() {
    Scene scene;
    const Lambertian wallWhite{Color(0.6f)};

    const Plane floor(Point3(0.0f), Vector3(0.0f, 1.0f, 0.0f), wallWhite);
    scene.addShape(floor);

    const Plane backWall(Point3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 0.0f, -1.0f),
                         wallWhite);
    scene.addShape(backWall);

    const Plane cameraWall(Point3(0.0f, 0.0f, 0.5f), Vector3(0.0f, 0.0f, 1.0f),
                           wallWhite);
    scene.addShape(cameraWall);

    const Plane ceiling(Point3(0.0f, 9.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f),
                        wallWhite);
    scene.addShape(ceiling);

    const Plane leftWall(Point3(-4.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f),
                         Lambertian(Color(0.7f, 0.4f, 0.4f)));
    scene.addShape(leftWall);

    const Plane rightWall(Point3(4.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f),
                          Lambertian(Color(0.4f, 0.7f, 0.4f)));
    scene.addShape(rightWall);

    const Sphere metalSphere{Point3(-2.0f, 1.0f, -6.0f), 1.0f,
                             Metal(Color(0.9f, 0.7f, 0.5f), 2.0f)};
    scene.addShape(metalSphere);

    const Sphere blueSphere{Point3(0.0f, 1.0f, -6.0f), 1.0f,
                            Lambertian(Color(0.35f, 0.45f, 0.55f))};
    scene.addShape(blueSphere);

    const Sphere mirrorSphere{Point3(2.0f, 1.0f, -6.0f), 1.0f, Metal(0.8f)};
    scene.addShape(mirrorSphere);

    LightRack lightRack;
    const AmbientLight ambient{Color(0.9f)};
    lightRack.addLight(ambient);
    const PointLight light{Point3(3.0f, 5.5f, 0.0f), Color(0.4f)};
    lightRack.addLight(light);
    const PointLight light2{Point3(-3.0f, 5.5f, 0.0f), Color(0.4f)};
    lightRack.addLight(light2);

    constexpr int width{720};
    constexpr int height{480};

    PerspectiveCamera camera{
        Point3(0.0f, 4.0f, 0.0f), Vector3(0.0f, 1.0f, -10.0f),
        Vector3(0.0f, 1.0f, 0.0f), Math::PI / 6,
        static_cast<float>(width) / static_cast<float>(height)};

    Renderer renderer{scene,
                      lightRack,
                      width,
                      height,
                      ReflectionGenerator(200),
                      DeterministicSupersampler(1)};

    auto render = renderer.rayTrace(camera, 2);
    renderer.saveRender(render, "scene.ppm");

    return 0;
}
