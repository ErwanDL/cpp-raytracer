#include "material.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "../src/utils.hpp"

Color::Color(float f) : r(f), g(f), b(f) {}
Color::Color(float red, float green, float blue) {
    r = red;
    g = green;
    b = blue;
}

Color Color::gammaCorrected(float exposure, float gamma) const {
    float red = Math::unitClamp(::pow(r * exposure, gamma));
    float green = Math::unitClamp(std::pow(g * exposure, gamma));
    float blue = Math::unitClamp(std::pow(b * exposure, gamma));
    return Color(red, green, blue);
}

Color operator*(const Color &c1, const Color &c2) {
    return Color(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b);
}
Color operator*(float f, const Color &c) {
    return Color(c.r * f, c.g * f, c.b * f);
}
Color operator*(const Color &c, float f) {
    return Color(c.r * f, c.g * f, c.b * f);
}
Color operator+(const Color &c1, const Color &c2) {
    return Color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
}
std::ostream &operator<<(std::ostream &out, const Color &color) {
    out << "Color(R: " << color.r << ", G: " << color.g << ", B: " << color.b
        << ')';
    return out;
}

Color &Color::operator+=(const Color &other) {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}

bool Color::operator==(const Color &other) const {
    return Math::floatingPointEquality(r, other.r) &&
           Math::floatingPointEquality(g, other.g) &&
           Math::floatingPointEquality(b, other.b);
}

// STRUCT MATERIAL

Material::Material(const Color &color, float specular, float shininess,
                   float diffuse)
    : color(color),
      diffuse(diffuse),
      specular(specular),
      shininess(shininess) {}