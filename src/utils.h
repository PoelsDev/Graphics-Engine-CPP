//
// Created by thibaultpoels on 19/02/2020.
//

#ifndef ENGINE_UTILS_H
#define ENGINE_UTILS_H

#include "library.h"
#include "lineUtils.h"
#include "Color.h"
#include "3DLineUtils.h"

class Face;

using namespace std;

namespace utils {
    int roundToInt(double d);
    std::vector<int> convertColorValues(std::vector<double> initialValues);
    img::Color convertColorValue(Color& c);
    double getRad(double angle);
    std::vector<Face> triangulate(const Face& face);
    void getImageInfo(Lines2D& lines, int size, double& imageX, double& imageY, double& DCx, double& DCy, double& dx, double& dy, double& d);
    void generateFractal(Figure& fig, Figures3D& fractal, const double nrIterations, const double scale);
    Figure FiguresToFigure(Figures3D& figs);
}

#endif //ENGINE_UTILS_H
