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

/**
 * @brief namespace for utilities
 */

namespace utils {
    
    /**
     * @brief function to round a double to an integer in a particular way, which is used in the whole project
     * @param d double
     * @return rounded integer
     */
    
    int roundToInt(double d);
    
    /**
     * @brief converts a list of possibly color doubles to a list of corresponding color integers
     * @param initialValues double values
     * @return integer values
     */
    
    std::vector<int> convertColorValues(std::vector<double> initialValues);
    
    /**
     * @brief function which converts a Color to a img::Color
     * @param c Color to be converted
     * @return img::Color
     */
    
    img::Color convertColorValue(Color& c);
    
    /**
     * @brief function which converts an angle in degrees to an angle in radians
     * @param angle Degrees
     * @return angle in radians
     */
    
    double getRad(double angle);
    
    /**
     * @brief function which triangulates a Face and returns a new list of triangle faces
     * @param face Face to be triangulated
     * @return list of triangle faces
     */
    
    std::vector<Face> triangulate(const Face& face);
    
    /**
     * @brief function which calculates information used in several files in the project
     * @param lines 2D Lines in the image
     * @param size image size
     * @param imageX width
     * @param imageY height
     * @param DCx 
     * @param DCy 
     * @param dx 
     * @param dy 
     * @param d 
     */
    
    void getImageInfo(Lines2D& lines, int size, double& imageX, double& imageY, double& DCx, double& DCy, double& dx, double& dy, double& d);
    
    /**
     * @brief generates fractals from the figure given
     * @param fig Figure in question
     * @param fractal List of Fractal Figures
     * @param nrIterations 
     * @param scale 
     */
    
    void generateFractal(Figure& fig, Figures3D& fractal, const double nrIterations, const double scale);
    
    /**
     * @brief function which creates 1 Figure object from multiple Figure objects
     * @param figs Figure list
     * @return Single figure created from the given figures
     */
    
    Figure FiguresToFigure(Figures3D& figs);
}

#endif //ENGINE_UTILS_H
