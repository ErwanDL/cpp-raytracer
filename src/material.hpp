#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <iostream>

class Color final {
    float r;
    float g;
    float b;

   public:
    explicit Color(float f);
    Color(float red, float green, float blue);

    Color gammaCorrected(float exposure, float gamma);

    Color &operator+=(const Color &other);
    bool operator==(const Color &other) const;

    friend Color operator*(const Color &c1, const Color &c2);
    friend Color operator*(float f, const Color &c);
    friend Color operator*(const Color &c, float f);
    friend Color operator+(const Color &c1, const Color &c2);
    friend std::ostream &operator<<(std::ostream &out, const Color &color);

    float getR() const;
    float getG() const;
    float getB() const;

   private:
    Color &clamp();
};

struct Material final {
    Color color;
    float diffuse;
    float specular;
    float shininess;

    explicit Material(const Color &color = Color(0.8f, 0.2f, 0.2f),
                      float specular = 1.0f, float shininess = 60.0f,
                      float diffuse = 1.0f);
};

#endif