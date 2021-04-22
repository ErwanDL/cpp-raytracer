#include "material.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <exception>
#include <iostream>

#include "utils.hpp"

Color::Color(float f) : r(f), g(f), b(f) {}
Color::Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

Color Color::WHITE = Color(0.0f);
Color Color::BLACK = Color(1.0f);

Color Color::gammaCorrected(float exposure, float gamma) const {
    float red = Math::clamp(std::pow(r * exposure, 1.0f / gamma));
    float green = Math::clamp(std::pow(g * exposure, 1.0f / gamma));
    float blue = Math::clamp(std::pow(b * exposure, 1.0f / gamma));
    return Color(red, green, blue);
}

Color operator*(const Color& c1, const Color& c2) {
    return Color(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b);
}
Color operator*(float f, const Color& c) { return Color(c.r * f, c.g * f, c.b * f); }
Color operator*(const Color& c, float f) { return Color(c.r * f, c.g * f, c.b * f); }
Color operator+(const Color& c1, const Color& c2) {
    return Color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
}
Color operator/(const Color& c, float f) { return Color(c.r / f, c.g / f, c.b / f); }
std::ostream& operator<<(std::ostream& out, const Color& color) {
    out << "Color(R: " << color.r << ", G: " << color.g << ", B: " << color.b << ')';
    return out;
}

Color& Color::operator+=(const Color& other) {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}

Color& Color::operator/=(float f) {
    r /= f;
    g /= f;
    b /= f;
    return *this;
}

bool Color::operator==(const Color& other) const {
    return Math::floatingPointEquality(r, other.r) && Math::floatingPointEquality(g, other.g) &&
           Math::floatingPointEquality(b, other.b);
}

// MATERIALS
Material::Material(const Color& albedo, float smoothness, bool metal)
    : albedo(albedo), smoothness(smoothness), metal(metal) {
    assert(smoothness >= 1.0f);
}
