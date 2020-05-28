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
    /**
     * @brief Figure's Face class
     */
public:
    // These indexes refer to the corresponding points in the Figure class
    Face(vector<int> indexes);
    std::vector<int> point_indexes;
};

class Figure
{
    /**
     * @brief Figure class
     */
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


/**
 * @brief namespace used for transforming/scaling/rotating/projections/translations
 * The function names describe what they do.
 */
 
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
    
    /**
     * @brief A class used to parse Figures from the ini-files 
     */
     
private:
    
    const ini::Configuration& config;
    
    Figures3D figures;
    
    int nrFig;
    
    Vector3D eyepoint = Vector3D::point(0,0,0);
    
public:
    
    /**
     * @brief simple constructor for FiguresParser
     * @param config: ini::Configuration which contains the information from the ini-files
     */
    
    explicit FiguresParser(const ini::Configuration& config);
    
    /**
     * @brief generateFigures creates the Figures described in the ini-file and adds them to the Figures3D "figures" list
     */
    
    void generateFigures();
    
    /**
     * @brief createFigure creates 1 figure and returns it
     * @param figureNumber Number of the corresponding ini-file figure
     * @return generated Figure
     */
    
    Figure createFigure(int figureNumber);
    
    /**
     * @brief simple getter for figures
     * @return figures
     */
    
    Figures3D getFigures3D();
    
};

class L3DReplacer{
    
    /**
     * @brief class used to create 3D L-systems
     */
    
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
    
    /**
     * @brief simple constructor for L3DReplacer
     * @param l_sys LSystem3D object which contains the information from the L3D-files
     */
    
    L3DReplacer(LParser::LSystem3D& l_sys);
    
    /**
     * @brief Recursively draws the L-System Figure
     * @param f the figure to be generated
     * @param init the initial L-System string
     * @param depth the recursive iteration
     */
    
    void drawRecursive(Figure& f, string init, int depth);
    
    /**
     * @brief generates a figure by calling drawRecursive()
     * @param f Figure to be generated
     */
    
    void generateFigure(Figure& f);
    
};

#endif //ENGINE_3DLINEUTILS_H
