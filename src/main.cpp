#include "camera.hpp"
#include "image.hpp"
#include "math_utils.hpp"
#include "shape.hpp"
#include "vectors.hpp"

int main() {
    constexpr int width{720};
    constexpr int height{480};

    Image img{width, height};

    PerspectiveCamera camera{
        Point3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, -1.0f),
        Vector3(0.0f, 1.0f, 0.0f), Math::PI / 6,
        static_cast<float>(width) / static_cast<float>(height)};

    ShapeSet scene{};

    Plane floor(Point3(0.0f, -10.0f, 0.0f), Vector3(0.0f, 1.0f, -0.1f),
                Color(0.5f, 1.0f, 0.5f));
    scene.addShape(&floor);

    Sphere sphere{Point3(0.0f, 2.5f, -5.0f), 1.0f, Color(0.5f, 0.5f, 1.0f)};
    scene.addShape(&sphere);

    img.rayTrace(camera, scene);

    img.saveImage("scene.ppm");

    return 0;
}