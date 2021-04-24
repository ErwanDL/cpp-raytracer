#ifndef COLOR_HPP
#define COLOR_HPP

#include "utils.hpp"

struct Color {
    float r;
    float g;
    float b;

    static Color WHITE;
    static Color BLACK;

    Color() : r(0.0f), g(0.0f), b(0.0f) {}
    Color(float f) : r(f), g(f), b(f) {}
    Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

    Color& operator+=(const Color& other);
    Color& operator/=(float f);
    bool operator==(const Color& other) const;
};

Color operator*(const Color& c1, const Color& c2);
Color operator*(float f, const Color& c);
Color operator*(const Color& c, float f);
Color operator+(const Color& c1, const Color& c2);
Color operator/(const Color& c, float f);

#endif