#include "color.hpp"

Color Color::WHITE = Color(1.0f);
Color Color::BLACK = Color(0.0f);

Color operator*(const Color& c1, const Color& c2) {
    return Color(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b);
}
Color operator*(float f, const Color& c) { return Color(c.r * f, c.g * f, c.b * f); }
Color operator*(const Color& c, float f) { return Color(c.r * f, c.g * f, c.b * f); }
Color operator+(const Color& c1, const Color& c2) {
    return Color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
}
Color operator/(const Color& c, float f) { return Color(c.r / f, c.g / f, c.b / f); }

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