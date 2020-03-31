//
// Created by thibaultpoels on 19/02/2020.
//

#ifndef ENGINE_UTILS_H
#define ENGINE_UTILS_H

#include "easy_image.h"
#include <cmath>
#include <limits>
#include <cassert>

using namespace std;

class Color{
public:
    double red;
    double green;
    double blue;

    Color(double red, double green, double blue);
    Color(){}
};

class ZBuffer: public std::vector<std::vector<double>>
{
public:
    ZBuffer(const int width, const int height);
};

namespace utils{
    int roundToInt(double d);
    std::vector<int> convertColorValues(std::vector<double> initialValues);
    img::Color convertColorValue(Color& c);
    double getRad(double angle);
}

bool bufferCheck(ZBuffer& zbuf, unsigned int x, unsigned int y, double zValue);

double get1_ZValue(unsigned int i, int a, double z0, double z1);

void draw_zbuf_line(ZBuffer& zbuf, img::EasyImage& img, unsigned int x0, unsigned int y0, double z0, unsigned int x1,
        unsigned int y1, double z1, const img::Color &color);

#endif //ENGINE_UTILS_H
