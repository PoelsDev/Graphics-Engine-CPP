//
// Created by thibaultpoels on 04/04/2020.
//

#ifndef ENGINE_COLOR_H
#define ENGINE_COLOR_H

class Color{
public:
    double red{};
    double green{};
    double blue{};

    Color(double red, double green, double blue);
    Color()=default;

    void operator +=(const Color& c);
    Color operator * (const Color& c) const;
};

#endif //ENGINE_COLOR_H
