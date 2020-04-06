#include <iostream>
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

    const Sphere metalSphere{Point3(-1.0f, 1.0f, -6.0f), 1.0f,
                             Metal(Color(0.9f, 0.7f, 0.5f), 100.0f)};
    scene.addShape(metalSphere);

    const Sphere blueSphere{Point3(1.0f, 1.0f, -6.0f), 1.0f,
                            Lambertian(Color(0.4f, 0.4f, 0.7f), 0.05f)};
    scene.addShape(blueSphere);

    LightRack lightRack;
    const AmbientLight ambient{Color(0.7f)};
    lightRack.addLight(ambient);
    const PointLight light{Point3(3.0f, 5.5f, 0.0f), Color(0.5f)};
    lightRack.addLight(light);
    const PointLight light2{Point3(-3.0f, 5.5f, 0.0f), Color(0.5f)};
    lightRack.addLight(light2);

    constexpr int width{1280};
    constexpr int height{720};

    PerspectiveCamera camera{
        Point3(0.0f, 4.0f, 0.0f), Vector3(0.0f, 1.0f, -10.0f),
        Vector3(0.0f, 1.0f, 0.0f), Math::PI / 6,
        static_cast<float>(width) / static_cast<float>(height)};

    Renderer renderer{scene, lightRack, width, height};

    auto render = renderer.rayTrace(camera, 4);
    renderer.saveRenderer(render, "scene.ppm");

    return 0;
}
