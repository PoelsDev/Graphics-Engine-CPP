//
// Created by thibaultpoels on 04/03/2020.
//

#ifndef ENGINE_3DLINEUTILS_H
#define ENGINE_3DLINEUTILS_H

#include "library.h"
#include "lineUtils.h"
#include "Color.h"

class Face
{
public:
    // These indexes refer to the corresponding points in the Figure class
    Face(vector<int> indexes);
    std::vector<int> point_indexes;
};

class Figure
{
public:
    std::vector<Vector3D> points;
    std::vector<Face> faces;
    Color color;

    // Fractal Variables
    bool isFractal;
    double fractalScale;
    int nrIterations;
    
    // Lighting variables
    Color ambientReflection;
    Color diffuseReflection;
    Color specularReflection;
    
    double reflectionCoefficient;

    Figure();
};

typedef std::list<Figure> Figures3D;

namespace transform{
    Matrix scaleFigure(const double scale);

    Matrix rotateX(const double angle);

    Matrix rotateY(const double angle);

    Matrix rotateZ(const double angle);

    Matrix translate(const Vector3D &vector);

    void applyTransformation(Figure& f, const Matrix& m);

    void applyTransformation(Figures3D &f, const Matrix &m);

    void toPolar(const Vector3D vect, vector<double>& v_out);

    Matrix eyePointTrans(const Vector3D &eyepoint);

    Point2D doProjection(const Vector3D &point, const double d = 1);

    Lines2D doProjection(const Figures3D &, bool zBuffer=false);
}

class FiguresParser {
private:
    const ini::Configuration& config;
    Figures3D figures;
    int nrFig;
    Vector3D eyepoint = Vector3D::point(0,0,0);
public:
    explicit FiguresParser(const ini::Configuration& config);
    void generateFigures();
    Figure createFigure(int figureNumber);
    Figures3D getFigures3D();
};

class L3DReplacer{
private:
    LParser::LSystem3D l_system;
    Vector3D currentLocation;
    Vector3D vecH;
    Vector3D vecL;
    Vector3D vecU;

    stack<Vector3D> s_vecH;
    stack<Vector3D> s_vecL;
    stack<Vector3D> s_vecU;
    stack<Vector3D> s_location;
public:
    L3DReplacer(LParser::LSystem3D& l_sys);
    void drawRecursive(Figure& f, string init, int depth);
    void generateFigure(Figure& f);
};

#endif //ENGINE_3DLINEUTILS_H
