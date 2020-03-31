//
// Created by thibaultpoels on 19/02/2020.
//

#ifndef ENGINE_LINEUTILS_H
#define ENGINE_LINEUTILS_H

using namespace std;
#include "easy_image.h"
#include <list>
#include <cmath>
#include <limits>
#include <iostream>
#include "utils.h"
#include "l_parser.h"
#include <stack>

class Point2D {
public:
    double x;
    double y;

    Point2D(double x, double y);

    void operator *=(double factor);

    void operator +=(pair<double, double> factors);
};

class Line2D {
public:
    Point2D p1;
    Point2D p2;
    Color color;

    Line2D (Point2D p1, Point2D p2, Color color);

    // Z-Buffering
    double z1;
    double z2;
};

using Lines2D = std::list<Line2D>;

class L2DReplacer {
private:
    double current_angle;
    Point2D current_location = Point2D(0,0);
    LParser::LSystem2D l_system;
    Color color;
    stack<Point2D> s_location;
    stack<double> s_angle;
public:
    L2DReplacer(LParser::LSystem2D& l_sys, Color c);
    void drawRecursive(Lines2D& lines, string text, int depth);
};


ostream operator <<(ostream& os, Point2D p);

ostream operator <<(ostream& os, Line2D line);

img::EasyImage draw2DLines(Lines2D &lines, const int size, vector<double>& backgroundcolor, bool zBuffer=false);

#endif //ENGINE_LINEUTILS_H
