//
// Created by thibaultpoels on 19/02/2020.
//

#ifndef ENGINE_LINEUTILS_H
#define ENGINE_LINEUTILS_H

#include "Color.h"
#include "library.h"

class Point2D {
    
    /**
     * @brief class used to represent a 2D Point in the Engine
     */
    
public:
    double x;
    double y;

    
    /**
     * @brief simple constructor
     * @param x double
     * @param y double
     */
    
    Point2D(double x, double y);
    
    
    /**
     * @brief operators used for Point calculations
     * @param factor 
     */
    
    void operator *=(double factor);

    void operator +=(pair<double, double> factors);
};

class Line2D {
    
    /**
     * @brief class used to represent a Line in the Engine
     */
    
public:
    
    Point2D p1;
    
    Point2D p2;
    
    Color color;
    
    
    /**
     * @brief simple constructor
     * @param p1 First point
     * @param p2 Second point
     * @param color 
     */
    
    Line2D (Point2D p1, Point2D p2, Color color);

    // Z-Buffering
    double z1;
    double z2;
};

using Lines2D = std::list<Line2D>;

class L2DReplacer {
    
    /**
     * @brief class used for 2D L-Systems
     */
    
private:
    
    double current_angle;
    
    Point2D current_location = Point2D(0,0);
    
    LParser::LSystem2D l_system;
    
    Color color;
    
    stack<Point2D> s_location;
    
    stack<double> s_angle;
    
public:
    
    /**
     * for documentation of these functions go to class L3DReplacer which works the same for 3D objects 
     */
    
    L2DReplacer(LParser::LSystem2D& l_sys, Color c);
    
    void drawRecursive(Lines2D& lines, string text, int depth);
    
};

/**
 * @brief operators made to print objects from class Point2D and Line2D 
 */

ostream operator <<(ostream& os, Point2D p);

ostream operator <<(ostream& os, Line2D line);

/**
 * @brief function which draws the given lines on an EasyImage and returns it
 * @param lines Lines to be drawn
 * @param size Size of the image
 * @param backgroundcolor 
 * @param zBuffer Boolean which represents the use of a Z-Buffer
 * @return EasyImage
 */

img::EasyImage draw2DLines(Lines2D &lines, const int size, vector<double>& backgroundcolor, bool zBuffer=false);

#endif //ENGINE_LINEUTILS_H
