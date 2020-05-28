//
// Created by thibaultpoels on 19/02/2020.
//

#include "utils.h"
#include "3DLineUtils.h"

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

    std::vector<Face> triangulate(const Face& face){
        vector<Face> result;
        for(int i = 1;i<=face.point_indexes.size()-2;i++){
            result.push_back(Face({face.point_indexes[0],face.point_indexes[i],face.point_indexes[i+1]}));
        }

        return result;
    }

    void getImageInfo(Lines2D& lines, int size, double& imageX, double& imageY, double& DCx, double& DCy, double& dx, double& dy, double& d){
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

        imageX = size * (rangeX / max(rangeX, rangeY));
        imageY = size * (rangeY / max(rangeX, rangeY));

        d = 0.95 * (imageX / rangeX);

        DCx = d * ((minX + maxX) / 2);
        DCy = d * ((minY + maxY) / 2);
        dx = (imageX / 2) - DCx;
        dy = (imageY / 2) - DCy;
    }

    void generateFractal(Figure& fig, Figures3D& fractal, const double nrIterations, const double scale){
        if(nrIterations > 0){
            for(Vector3D p: fig.points){
                // Create new figure and scale
                Figure frac = fig;
                Matrix mScale = transform::scaleFigure(1/scale);
                transform::applyTransformation(frac, mScale);

                // Create translate matrix and move new figure
                Vector3D scaledP = p*mScale;
                Matrix mTransform = transform::translate(p-scaledP);
                transform::applyTransformation(frac,mTransform);

                // Recursive call
                generateFractal(frac,fractal,nrIterations-1,scale);
            }
        } else if(nrIterations == 0){
            fractal.push_back(fig);
        }
    }

    Figure FiguresToFigure(Figures3D& figs){
        Figure resultFig = Figure();
        
        for(Figure& f: figs){
            int index = resultFig.points.size();
            resultFig.points.insert(resultFig.points.end(),f.points.begin(),f.points.end());
            for(Face& fc:f.faces){
                for(int& i:fc.point_indexes){
                    i+=index;
                }
                resultFig.faces.push_back(fc);
            }
        }
        
        return resultFig;
    }
}



