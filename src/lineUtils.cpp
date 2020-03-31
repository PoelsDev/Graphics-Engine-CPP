//
// Created by thibaultpoels on 19/02/2020.
//

#include "lineUtils.h"

using namespace std;
using namespace utils;


Point2D::Point2D(double x, double y) : x(x), y(y) {};

void Point2D::operator *=(double factor){
    x *= factor;
    y *= factor;
}

void Point2D::operator +=(pair<double, double> factors){
    x += factors.first;
    y += factors.second;
}

Line2D::Line2D (Point2D p1, Point2D p2, Color color) : p1(p1), p2(p2), color(color){};

ostream operator <<(ostream& os, Point2D p){
    os << "(" << p.x << "," << p.y << ")";
}

ostream operator <<(ostream& os, Line2D line){
    os << "(" << line.p1.x << "," << line.p1.y << "), (" << line.p2.x << "," << line.p2.y << ")";
}


img::EasyImage draw2DLines(Lines2D &lines, const int size, vector<double>& backgroundColor, bool zBuffer) {
    double maxX = -std::numeric_limits<double>::infinity();
    double maxY = -std::numeric_limits<double>::infinity();
    double minX = std::numeric_limits<double>::infinity();
    double minY = std::numeric_limits<double>::infinity();

    double rangeX;
    double rangeY;




    for (Line2D &line:lines) {
        maxX = max(max(line.p1.x, line.p2.x), maxX);
        maxY = max(max(line.p1.y, line.p2.y), maxY);
        minX = min(min(line.p1.x, line.p2.x), minX);
        minY = min(min(line.p1.y, line.p2.y), minY);
    }

    rangeX = fabs(maxX - minX);
    rangeY = fabs(maxY - minY);

    double imageX = size * (rangeX / max(rangeX, rangeY));
    double imageY = size * (rangeY / max(rangeX, rangeY));

    // Create ZBuffer on ImageX & ImageY
    ZBuffer zbuf = ZBuffer(roundToInt(imageX),roundToInt(imageY));

    double d = 0.95 * (imageX / rangeX);

    for (Line2D &line:lines) {
        line.p1 *= d;
        line.p2 *= d;
    }

    double DCx = d * ((minX + maxX) / 2);
    double DCy = d * ((minY + maxY) / 2);
    double dx = (imageX / 2) - DCx;
    double dy = (imageY / 2) - DCy;

    for (Line2D &line: lines) {
        line.p1 += {dx, dy};
        line.p2 += {dx, dy};
    }


    Color bc;
    bc.red = backgroundColor[0];
    bc.green = backgroundColor[1];
    bc.blue = backgroundColor[2];
    img::EasyImage img(roundToInt(imageX), roundToInt(imageY), convertColorValue(bc));

    for(Line2D& line: lines){
        if(!zBuffer){
            img.draw_line(roundToInt(line.p1.x),roundToInt(line.p1.y), roundToInt(line.p2.x), roundToInt(line.p2.y), convertColorValue(line.color));
        } else {
            draw_zbuf_line(zbuf,img,roundToInt(line.p1.x),roundToInt(line.p1.y),line.z1,roundToInt(line.p2.x),roundToInt(line.p2.y),line.z2, convertColorValue(line.color));
        }
    }

    return img;

}

void L2DReplacer::drawRecursive(Lines2D &lines, string text, int depth) {
    for(char c: text){
        if(depth > 0){
            if(l_system.get_alphabet().find(c) != l_system.get_alphabet().end()){
                // Recursive call
                string s = l_system.get_replacement(c);
                L2DReplacer::drawRecursive(lines, s, depth-1);
            } else if(c == '+'){
                // Add to angle
                current_angle += l_system.get_angle();
            } else if(c == '-'){
                // Substract from angle
                current_angle = current_angle - l_system.get_angle();
            } else if(c == '('){
                // Push current location & angle
                s_location.push(current_location);
                s_angle.push(current_angle);
            } else if(c == ')'){
                // Return to last saved location / angle
                current_location = s_location.top();
                current_angle = s_angle.top();
                s_location.pop();
                s_angle.pop();
            }
        } else {
            // Add line to Lines2D object if(draw) is true
            if(c == '+') {
                // Add to angle
                current_angle += l_system.get_angle();
            } else if(c == '-'){
                // Substract from angle
                current_angle = current_angle - l_system.get_angle();
            } else if(c == '('){
                // Push current location & angle
                s_location.push(current_location);
                s_angle.push(current_angle);
            } else if(c == ')'){
                // Return to last saved location / angle
                current_location = s_location.top();
                current_angle = s_angle.top();
                s_location.pop();
                s_angle.pop();
            }

            if(l_system.get_alphabet().find(c) != l_system.get_alphabet().end()){
                Point2D new_loc = Point2D(current_location.x + cos(getRad(current_angle)),current_location.y + sin(getRad(current_angle)));
                if(l_system.draw(c)){
                    lines.push_back(Line2D(current_location,new_loc,color));
                }
                current_location = new_loc;
            }
        }
    }
}

L2DReplacer::L2DReplacer(LParser::LSystem2D &l_sys, Color color) : l_system(l_sys), color(color){
    current_angle = l_sys.get_starting_angle();
}
