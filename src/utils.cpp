//
// Created by thibaultpoels on 19/02/2020.
//

#include "utils.h"

namespace utils{
    int roundToInt(double d)
    {
        return static_cast<int>(std::round(d));
    }

    std::vector<int> convertColorValues(std::vector<double> initialValues){
        std::vector<int> result;
        for(double i: initialValues){
            int value = roundToInt(i*255);
            result.push_back(value);
        }
        return result;
    }

    img::Color convertColorValue(Color& c){
        img::Color out;
        out.red = roundToInt(c.red*255);
        out.green = roundToInt(c.green*255);
        out.blue = roundToInt(c.blue*255);

        return out;
    }

    double getRad(double angle){
        return (angle * M_PI)/180;
    }
}

Color::Color(double red, double green, double blue):red(red), green(green), blue(blue) {}

ZBuffer::ZBuffer(const int width, const int height) {
    for(int i=0;i<width;i++){
        vector<double> v;
        for(int j=0;j<height;j++){
            v.push_back(std::numeric_limits<double>::infinity());
        }
        this->push_back(v);
    }
}

bool bufferCheck(ZBuffer& zbuf, unsigned int x, unsigned int y, double zValue){
    if(zValue <= zbuf[x][y]) {
        zbuf[x][y] = zValue;
        return true;
    }
    return false;
}

double get1_ZValue(unsigned int i, int a, double z0, double z1){
    return ((i/(double)a)/z0)+((1-i/(double)a)/z1);
}

void draw_zbuf_line(ZBuffer& zbuf, img::EasyImage& img, unsigned int x0, unsigned int y0, double z0, unsigned int x1,
        unsigned int y1, double z1, const img::Color &color){
    assert(x0 < img.get_width() && y0 < img.get_height());
    assert(x1 < img.get_width() && y1 < img.get_height());

    if (x0 == x1)
    {
        //special case for x0 == x1
        if(y0 != min(y0,y1)){
            std::swap(y0, y1);
            std::swap(z0,z1);
        }
        for (unsigned int i = std::min(y0, y1); i <= std::max(y0, y1); i++)
        {
            double zValue = get1_ZValue((max(y0,y1)-min(y0,y1))-(i-min(y0,y1)),max(y0,y1)-min(y0,y1),z0,z1);
            if(bufferCheck(zbuf,x0,i,zValue)){
                img(x0, i) = color;
            }
        }
    }
    else if (y0 == y1)
    {
        //special case for y0 == y1
        if(x0 != min(x0,x1)) {
            std::swap(x0, x1);
            std::swap(y0, y1);
            std::swap(z0,z1);
        }
        for (unsigned int i = std::min(x0, x1); i <= std::max(x0, x1); i++)
        {
            double zValue = get1_ZValue((max(x0,x1)-min(x0,x1))-(i-min(x0,x1)),max(x0,x1)-min(x0,x1),z0,z1);
            if(bufferCheck(zbuf,i,y0,zValue)) {
                img(i, y0) = color;
            }
        }
    }
    else
    {
        if (x0 > x1)
        {
            //flip points if x1>x0: we want x0 to have the lowest value
            std::swap(x0, x1);
            std::swap(y0, y1);
            std::swap(z0,z1);
        }
        double m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0);
        if (-1.0 <= m && m <= 1.0)
        {
            for (unsigned int i = 0; i <= (x1 - x0); i++)
            {
                double zValue = get1_ZValue((x1-x0)-i,(x1-x0),z0,z1);
                unsigned int y = (unsigned int) round(y0 + m * i);
                if(bufferCheck(zbuf,x0+i,y,zValue)){
                    img(x0 + i, y) = color;
                }
            }
        }
        else if (m > 1.0)
        {
            for (unsigned int i = 0; i <= (y1 - y0); i++)
            {
                double zValue = get1_ZValue((y1-y0)-i,(y1-y0),z0,z1);
                unsigned int x = (unsigned int) round(x0 + (i / m));
                if(bufferCheck(zbuf,x,y0+i,zValue)){
                    img(x, y0 + i) = color;
                }
            }
        }
        else if (m < -1.0)
        {
            for (unsigned int i = 0; i <= (y0 - y1); i++)
            {
                double zValue = get1_ZValue((y0-y1)-i,(y0-y1),z0,z1);
                unsigned int x = (unsigned int) round(x0 - (i / m));
                if(bufferCheck(zbuf,x,y0-i,zValue)) {
                    img(x, y0 - i) = color;
                }
            }
        }
    }
}
