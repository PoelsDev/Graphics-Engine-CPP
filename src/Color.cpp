//
// Created by thibaultpoels on 04/04/2020.
//

#include "Color.h"

Color::Color(double red, double green, double blue):red(red), green(green), blue(blue) {}

void Color::operator+=(const Color &c) {
    this->red = (this->red + c.red);
    this->green = (this->green + c.green);
    this->blue = (this->blue + c.blue);
}

Color Color::operator*(const Color &c) const {
    return Color(this->red*c.red,this->green*c.green,this->blue*c.blue);
}

