#include <iostream>
#include "camera.hpp"
#include "image.hpp"
#include "light.hpp"
#include "material.hpp"
#include "math_utils.hpp"
#include "shape.hpp"
#include "vectors.hpp"

int main() {
    constexpr int width{1920};
    constexpr int height{1080};

    Image img{width, height, 1.0f, 1.8f};

    PerspectiveCamera camera{
        Point3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, -1.0f),
        Vector3(0.0f, 1.0f, 0.0f), Math::PI / 6,
        static_cast<float>(width) / static_cast<float>(height)};

    Scene scene{};

    Plane floor(Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f),
                Color(0.3f, 0.6f, 0.3f));
    scene.addShape(&floor);

    Plane backwall(Point3(0.0f, 0.0f, -15.0f), Vector3(0.0f, 0.0f, 1.0f),
                   Color(0.6f, 0.3f, 0.3f));
    scene.addShape(&backwall);

    Sphere sphere{Point3(1.0f, 2.5f, -5.0f), 0.7f, Color(0.5f, 0.5f, 1.0f)};
    scene.addShape(&sphere);
    Sphere sphere2{Point3(-1.5f, 0.5f, -2.5f), 0.5f, Color(0.7f, 0.7f, 0.0f)};
    scene.addShape(&sphere2);

    LightRack lightRack{0.6f};

    Spotlight light{Point3(2.0f, 4.0f, 0.0f), Color(0.6f)};
    lightRack.addLight(&light);

    img.rayTrace(camera, scene, lightRack);

    img.saveImage("scene.ppm");

    return 0;
}