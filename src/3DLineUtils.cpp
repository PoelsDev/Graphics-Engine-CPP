//
// Created by thibaultpoels on 04/03/2020.
//

#include "3DLineUtils.h"
#include "solids.h"
#include "utils.h"

using namespace utils;

Figure::Figure() {
    Figure::isFractal = false;
}

namespace transform{
    Matrix scaleFigure(const double scale){
        Matrix s;
        // Fill matrix with correct values;
        s(1,1) = scale;
        s(2,2) = scale;
        s(3,3) = scale;

        return s;
    }

    Matrix rotateX(const double angle){
        double rad = getRad(angle);
        Matrix m_angle;
        // Fill matrix with correct values;
        m_angle(2,2) = cos(rad);
        m_angle(2,3) = sin(rad);
        m_angle(3,2) = -sin(rad);
        m_angle(3,3) = cos(rad);

        return m_angle;
    }

    Matrix rotateY(const double angle){
        double rad = getRad(angle);
        Matrix m_angle;
        // Fill matrix with correct values;
        m_angle(1,1) = cos(rad);
        m_angle(1,3) = -sin(rad);
        m_angle(3,1) = sin(rad);
        m_angle(3,3) = cos(rad);

        return m_angle;
    }

    Matrix rotateZ(const double angle){
        double rad = getRad(angle);
        Matrix m_angle;
        // Fill matrix with correct values;
        m_angle(1,1) = cos(rad);
        m_angle(2,2) = cos(rad);
        m_angle(2,1) = -sin(rad);
        m_angle(1,2) = sin(rad);

        return m_angle;
    }

    Matrix translate(const Vector3D &vector){
        Matrix t;
        // Fill matrix with correct values;
        t(4,1) = vector.x;
        t(4,2) = vector.y;
        t(4,3) = vector.z;

        return t;
    }

    void applyTransformation(Figure& f, const Matrix &m){
        for(Vector3D &p: f.points){
            p = p*m;
        }
    }

    void applyTransformation(Figures3D &f, const Matrix &m){
        for(Figure &fig: f){
            applyTransformation(fig,m);
        }
    }

    void toPolar(const Vector3D vect, vector<double>& v_out){
        v_out.push_back(sqrt(pow(vect.x,2)+pow(vect.y,2)+pow(vect.z,2)));
        v_out.push_back(atan2(vect.y,vect.x));
        v_out.push_back(acos(vect.z/v_out[0]));
    }

    Matrix eyePointTrans(const Vector3D &eyepoint){
        Matrix t;

        // Get spherical coordinates
        vector<double> polar;
        toPolar(eyepoint, polar);

        // Variables
        double sinTheta = sin(polar[1]);
        double sinPhi = sin(polar[2]);
        double cosTheta = cos(polar[1]);
        double cosPhi = cos(polar[2]);

        t(1,1) = -sinTheta;
        t(1,2) = -cosTheta*cosPhi;
        t(1,3) = cosTheta*sinPhi;
        t(2,1) = cosTheta;
        t(2,2) = -sinTheta*cosPhi;
        t(2,3) = sinTheta*sinPhi;
        t(3,2) = sinPhi;
        t(3,3) = cosPhi;
        t(4,3) = -polar[0];

        return t;

    }

    Point2D doProjection(const Vector3D &point, const double d){
        Point2D p = Point2D((d*point.x)/(-point.z),(d*point.y)/(-point.z));
        return p;
    }

    Lines2D doProjection(const Figures3D &figs, bool zBuffer){
        Lines2D result;
        for(Figure f: figs){
            const Color& lineC = f.color;
            for(Face fa: f.faces){
                if(fa.point_indexes.size()>2){
                    for(int i = 0; i<fa.point_indexes.size();i++){
                        if(i<fa.point_indexes.size()-1){
                            Line2D l = Line2D(doProjection(f.points[fa.point_indexes[i]]),doProjection(f.points[fa.point_indexes[i+1]]),lineC);
                            if(zBuffer) {l.z1 = f.points[fa.point_indexes[i]].z; l.z2 = f.points[fa.point_indexes[i+1]].z;}
                            result.push_back(l);
                        } else {
                            Line2D l = Line2D(doProjection(f.points[fa.point_indexes[i]]),doProjection(f.points[fa.point_indexes[0]]),lineC);
                            if(zBuffer) {l.z1 = f.points[fa.point_indexes[i]].z; l.z2 = f.points[fa.point_indexes[0]].z; }
                            result.push_back(l);
                        }
                    }
                } else {
                    Line2D l = Line2D(doProjection(f.points[fa.point_indexes[0]]),doProjection(f.points[fa.point_indexes[1]]),lineC);
                    if(zBuffer) {l.z1 = f.points[fa.point_indexes[0]].z; l.z2 = f.points[fa.point_indexes[1]].z;}
                    result.push_back(l);
                }
            }
        }

        return result;
    }
}

FiguresParser::FiguresParser(const ini::Configuration &config) : config(config){
    nrFig = config["General"]["nrFigures"].as_int_or_die();
    vector<double> tempEyepoint = config["General"]["eye"].as_double_tuple_or_die();
    eyepoint.x = tempEyepoint[0];
    eyepoint.y = tempEyepoint[1];
    eyepoint.z = tempEyepoint[2];
}

void FiguresParser::generateFigures() {
    // Loop over amount of figures
    for(int i = 0;i<nrFig;i++){
        figures.push_back(createFigure(i));
    }
}

Figure FiguresParser::createFigure(int figureNumber) {
    const vector<string> solids = {"Cube", "Cone", "Cylinder", "Torus", "Sphere", "Tetrahedron", "Octahedron", "Icosahedron", "Dodecahedron", "BuckyBall", "MengerSponge"};
    const vector<string> fractals = {"FractalCube", "FractalDodecahedron", "FractalIcosahedron", "FractalOctahedron", "FractalTetrahedron", "FractalBuckyBall"};
    string currentFig = "Figure"+std::to_string(figureNumber);
    Figure fig = Figure();

    if(config[currentFig]["type"].as_string_or_die() == "LineDrawing"){
        // All used matrices
        Matrix rotateX = transform::rotateX(config[currentFig]["rotateX"].as_double_or_die());
        Matrix rotateY = transform::rotateY(config[currentFig]["rotateY"].as_double_or_die());
        Matrix rotateZ = transform::rotateZ(config[currentFig]["rotateZ"].as_double_or_die());
        Matrix scale = transform::scaleFigure(config[currentFig]["scale"].as_double_or_die());
        Matrix eye = transform::eyePointTrans(eyepoint);
        vector<double> transVDouble = config[currentFig]["center"].as_double_tuple_or_die();
        Vector3D transVect = Vector3D::point(transVDouble[0],transVDouble[1],transVDouble[2]);
        Matrix translate = transform::translate(transVect);

        // Combined
        Matrix combinedMatrix = scale * rotateX * rotateY * rotateZ * translate * eye;

        int nrOfPoints = config[currentFig]["nrPoints"].as_int_or_die();
        int nrOfLines = config[currentFig]["nrLines"].as_int_or_die();

        // Create points
        for(int i = 0; i<nrOfPoints;i++){
            vector<double> point = config[currentFig]["point"+std::to_string(i)].as_double_tuple_or_die();
            Vector3D pointVector3D = Vector3D::point(0,0,0);
            pointVector3D.x = point[0];
            pointVector3D.y = point[1];
            pointVector3D.z = point[2];
            fig.points.push_back(pointVector3D);
        }

        // Apply transformation to the created points
        transform::applyTransformation(fig,combinedMatrix);

        // Create lines
        for(int i = 0;i<nrOfLines;i++){
            vector<int> indexes = config[currentFig]["line"+std::to_string(i)].as_int_tuple_or_die();
            Face f = Face(indexes);
            fig.faces.push_back(f);
        }

        vector<double> lineRGB = config[currentFig]["color"].as_double_tuple_or_die();
        fig.color = Color(lineRGB[0],lineRGB[1],lineRGB[2]);
    } else if(std::find(solids.begin(), solids.end(), config[currentFig]["type"].as_string_or_die()) != solids.end()){
        // All used matrices
        Matrix rotateX = transform::rotateX(config[currentFig]["rotateX"].as_double_or_die());
        Matrix rotateY = transform::rotateY(config[currentFig]["rotateY"].as_double_or_die());
        Matrix rotateZ = transform::rotateZ(config[currentFig]["rotateZ"].as_double_or_die());
        Matrix scale = transform::scaleFigure(config[currentFig]["scale"].as_double_or_die());
        Matrix eye = transform::eyePointTrans(eyepoint);
        vector<double> transVDouble = config[currentFig]["center"].as_double_tuple_or_die();
        Vector3D transVect = Vector3D::point(transVDouble[0],transVDouble[1],transVDouble[2]);
        Matrix translate = transform::translate(transVect);

        // Combined
        Matrix combinedMatrix = scale * rotateX * rotateY * rotateZ * translate * eye;

        // Get figure
        string type = config[currentFig]["type"].as_string_or_die();
        if(type == "Cube"){
            fig = pSolids::createCube();
        } else if(type == "Cylinder"){
            fig = solids::createCylinder(config[currentFig]["n"].as_int_or_die(), config[currentFig]["height"].as_double_or_die());
        } else if(type == "Cone"){
            fig = solids::createCone(config[currentFig]["n"].as_int_or_die(), config[currentFig]["height"].as_double_or_die());
        } else if(type == "Torus"){
            fig = solids::createTorus(config[currentFig]["r"].as_double_or_die(), config[currentFig]["R"].as_double_or_die(),config[currentFig]["n"].as_int_or_die(), config[currentFig]["m"].as_int_or_die());
        } else if(type == "Sphere"){
            fig = solids::createSphere(1,config[currentFig]["n"].as_int_or_die());
        } else if(type == "Tetrahedron"){
            fig = pSolids::createTetrahedron();
        } else if(type == "Octahedron"){
            fig = pSolids::createOctahedron();
        } else if(type == "Dodecahedron"){
            fig = pSolids::createDodecahedron();
        } else if(type == "Icosahedron"){
            fig = pSolids::createIcosahedron();
        } else if(type == "MengerSponge"){
            fig = pSolids::createCube();
            Figures3D figs;
            solids::createMengerSponge(fig,figs,config[currentFig]["nrIterations"].as_int_or_die(),3);
            fig = utils::FiguresToFigure(figs);
        } else if(type == "BuckyBall"){
            fig = solids::createBuckyBall();
        }

        // Apply Transformation with the combined matrix
        transform::applyTransformation(fig,combinedMatrix);
        
        
        if(config["General"]["type"].as_string_or_die() == "LightedZBuffering") {
            // Lighting
            vector<double> ambientReflection = config[currentFig]["ambientReflection"].as_double_tuple_or_die();
            fig.ambientReflection = Color(ambientReflection[0], ambientReflection[1], ambientReflection[2]);
        } else {
            // Color
            vector<double> lineRGB = config[currentFig]["color"].as_double_tuple_or_die();
            fig.color = Color(lineRGB[0],lineRGB[1],lineRGB[2]);
        }
        
    } else if(config[currentFig]["type"].as_string_or_die() == "3DLSystem"){
        // All used matrices
        Matrix rotateX = transform::rotateX(config[currentFig]["rotateX"].as_double_or_die());
        Matrix rotateY = transform::rotateY(config[currentFig]["rotateY"].as_double_or_die());
        Matrix rotateZ = transform::rotateZ(config[currentFig]["rotateZ"].as_double_or_die());
        Matrix scale = transform::scaleFigure(config[currentFig]["scale"].as_double_or_die());
        Matrix eye = transform::eyePointTrans(eyepoint);
        vector<double> transVDouble = config[currentFig]["center"].as_double_tuple_or_die();
        Vector3D transVect = Vector3D::point(transVDouble[0],transVDouble[1],transVDouble[2]);
        Matrix translate = transform::translate(transVect);

        // Combined
        Matrix combinedMatrix = scale * rotateX * rotateY * rotateZ * translate * eye;

        // Input stream
        LParser::LSystem3D l_sys = LParser::LSystem3D();
        std::ifstream input(config[currentFig]["inputfile"].as_string_or_die());
        input >> l_sys;
        input.close();
        L3DReplacer replacer = L3DReplacer(l_sys);
        replacer.generateFigure(fig);

        // Apply Transformation with the combined matrix
        transform::applyTransformation(fig,combinedMatrix);

        // Color
        vector<double> lineRGB = config[currentFig]["color"].as_double_tuple_or_die();
        fig.color = Color(lineRGB[0],lineRGB[1],lineRGB[2]);
    } else if(find(fractals.begin(),fractals.end(),config[currentFig]["type"].as_string_or_die()) != fractals.end()){
        // All used matrices
        Matrix rotateX = transform::rotateX(config[currentFig]["rotateX"].as_double_or_die());
        Matrix rotateY = transform::rotateY(config[currentFig]["rotateY"].as_double_or_die());
        Matrix rotateZ = transform::rotateZ(config[currentFig]["rotateZ"].as_double_or_die());
        Matrix scale = transform::scaleFigure(config[currentFig]["scale"].as_double_or_die());
        Matrix eye = transform::eyePointTrans(eyepoint);
        vector<double> transVDouble = config[currentFig]["center"].as_double_tuple_or_die();
        Vector3D transVect = Vector3D::point(transVDouble[0],transVDouble[1],transVDouble[2]);
        Matrix translate = transform::translate(transVect);

        // Combined
        Matrix combinedMatrix = scale * rotateX * rotateY * rotateZ * translate * eye;

        // Get figure
        string type = config[currentFig]["type"].as_string_or_die();
        if(type == "FractalCube"){
            fig = pSolids::createCube();
        } else if(type == "FractalTetrahedron"){
            fig = pSolids::createTetrahedron();
        } else if(type == "FractalOctahedron"){
            fig = pSolids::createOctahedron();
        } else if(type == "FractalDodecahedron"){
            fig = pSolids::createDodecahedron();
        } else if(type == "FractalIcosahedron"){
            fig = pSolids::createIcosahedron();
        } else if(type == "FractalBuckyBall"){
            fig = solids::createBuckyBall();
        }

        // Apply Transformation with the combined matrix
        transform::applyTransformation(fig,combinedMatrix);

        // Get fractal variables for figure
        fig.isFractal = true;
        fig.fractalScale = config[currentFig]["fractalScale"].as_double_or_die();
        fig.nrIterations = config[currentFig]["nrIterations"].as_int_or_die();

        if(config["General"]["type"].as_string_or_die() == "LightedZBuffering") {
            // Lighting
            vector<double> ambientReflection = config[currentFig]["ambientReflection"].as_double_tuple_or_die();
            fig.ambientReflection = Color(ambientReflection[0], ambientReflection[1], ambientReflection[2]);
        } else {
            // Color
            vector<double> lineRGB = config[currentFig]["color"].as_double_tuple_or_die();
            fig.color = Color(lineRGB[0],lineRGB[1],lineRGB[2]);
        }
    }
    return fig;
}

Figures3D FiguresParser::getFigures3D() {
    return figures;
}

Face::Face(vector<int> indexes) {
    this->point_indexes = indexes;
}

L3DReplacer::L3DReplacer(LParser::LSystem3D &l_sys) : l_system(l_sys){
    currentLocation = Vector3D::point(0,0,0);
    vecH = Vector3D::vector(1,0,0);
    vecL = Vector3D::vector(0,1,0);
    vecU = Vector3D::vector(0,0,1);
}

void L3DReplacer::drawRecursive(Figure &f, string init, int depth) {
    for(char c: init){
        if(depth > 0){
            if(l_system.get_alphabet().find(c) != l_system.get_alphabet().end()){
                // Recursive call
                string s = l_system.get_replacement(c);
                L3DReplacer::drawRecursive(f, s, depth-1);
            } else if(c == '+'){
                // Update vectors H & L with angle
                Vector3D tempH = vecH*cos(getRad(l_system.get_angle()))+vecL*sin(getRad(l_system.get_angle()));
                Vector3D tempL = -vecH*sin(getRad(l_system.get_angle())) + vecL*cos(getRad(l_system.get_angle()));
                vecH = tempH;
                vecL = tempL;
            } else if(c == '-'){
                // Update vectors H & L with -angle
                Vector3D tempH = vecH*cos(getRad(-l_system.get_angle()))+vecL*sin(getRad(-l_system.get_angle()));
                Vector3D tempL = -vecH*sin(getRad(-l_system.get_angle())) + vecL*cos(getRad(-l_system.get_angle()));
                vecH = tempH;
                vecL = tempL;
            } else if(c == '('){
                // Push current location / angle
                s_location.push(currentLocation);
                s_vecH.push(vecH);
                s_vecL.push(vecL);
                s_vecU.push(vecU);
            } else if(c == ')'){
                // Return to last saved location / angle
                currentLocation = s_location.top();
                vecH = s_vecH.top();
                vecL = s_vecL.top();
                vecU = s_vecU.top();
                s_location.pop();
                s_vecH.pop();
                s_vecL.pop();
                s_vecU.pop();
            } else if(c == '^') {
                Vector3D tempH = vecH*cos(getRad(l_system.get_angle()))+vecU*sin(getRad(l_system.get_angle()));
                Vector3D tempU = -vecH*sin(getRad(l_system.get_angle())) + vecU*cos(getRad(l_system.get_angle()));
                vecH = tempH;
                vecU = tempU;
            } else if(c == '&') {
                Vector3D tempH = vecH*cos(getRad(-l_system.get_angle()))+vecU*sin(getRad(-l_system.get_angle()));
                Vector3D tempU = -vecH*sin(getRad(-l_system.get_angle())) + vecU*cos(getRad(-l_system.get_angle()));
                vecH = tempH;
                vecU = tempU;
            } else if(c == '\\') {
                Vector3D tempL = vecL*cos(getRad(l_system.get_angle()))-vecU*sin(getRad(l_system.get_angle()));
                Vector3D tempU = vecL*sin(getRad(l_system.get_angle())) + vecU*cos(getRad(l_system.get_angle()));
                vecL = tempL;
                vecU = tempU;
            } else if(c == '/'){
                Vector3D tempL = vecL*cos(getRad(-l_system.get_angle()))-vecU*sin(getRad(-l_system.get_angle()));
                Vector3D tempU = vecL*sin(getRad(-l_system.get_angle())) + vecU*cos(getRad(-l_system.get_angle()));
                vecL = tempL;
                vecU = tempU;
            } else if(c == '|') {
                Vector3D tempH = -vecH;
                Vector3D tempL = -vecL;
                vecH = tempH;
                vecL = tempL;
            }
        } else {
            if(c == '+'){
                // Update vectors H & L with angle
                Vector3D tempH = vecH*cos(getRad(l_system.get_angle()))+vecL*sin(getRad(l_system.get_angle()));
                Vector3D tempL =  -vecH*sin(getRad(l_system.get_angle())) + vecL*cos(getRad(l_system.get_angle()));
                vecH = tempH;
                vecL = tempL;
            } else if(c == '-'){
                // Update vectors H & L with -angle
                Vector3D tempH = vecH*cos(getRad(-l_system.get_angle()))+vecL*sin(getRad(-l_system.get_angle()));
                Vector3D tempL = -vecH*sin(getRad(-l_system.get_angle())) + vecL*cos(getRad(-l_system.get_angle()));
                vecH = tempH;
                vecL = tempL;
            } else if(c == '('){
                // Push current location / angle
                s_location.push(currentLocation);
                s_vecH.push(vecH);
                s_vecL.push(vecL);
                s_vecU.push(vecU);
            } else if(c == ')'){
                // Return to last saved location / angle
                currentLocation = s_location.top();
                vecH = s_vecH.top();
                vecL = s_vecL.top();
                vecU = s_vecU.top();
                s_location.pop();
                s_vecH.pop();
                s_vecL.pop();
                s_vecU.pop();
            } else if(c == '^') {
                Vector3D tempH = vecH*cos(getRad(l_system.get_angle()))+vecU*sin(getRad(l_system.get_angle()));
                Vector3D tempU = -vecH*sin(getRad(l_system.get_angle())) + vecU*cos(getRad(l_system.get_angle()));
                vecH = tempH;
                vecU = tempU;
            } else if(c == '&') {
                Vector3D tempH = vecH*cos(getRad(-l_system.get_angle()))+vecU*sin(getRad(-l_system.get_angle()));
                Vector3D tempU = -vecH*sin(getRad(-l_system.get_angle())) + vecU*cos(getRad(-l_system.get_angle()));
                vecH = tempH;
                vecU = tempU;
            } else if(c == '\\') {
                Vector3D tempL = vecL*cos(getRad(l_system.get_angle()))-vecU*sin(getRad(l_system.get_angle()));
                Vector3D tempU = vecL*sin(getRad(l_system.get_angle())) + vecU*cos(getRad(l_system.get_angle()));
                vecL = tempL;
                vecU = tempU;
            } else if(c == '/'){
                Vector3D tempL = vecL*cos(getRad(-l_system.get_angle()))-vecU*sin(getRad(-l_system.get_angle()));
                Vector3D tempU = vecL*sin(getRad(-l_system.get_angle())) + vecU*cos(getRad(-l_system.get_angle()));
                vecL = tempL;
                vecU = tempU;
            } else if(c == '|') {
                Vector3D tempH = -vecH;
                Vector3D tempL = -vecL;
                vecH = tempH;
                vecL = tempL;
            }

            if(l_system.get_alphabet().find(c) != l_system.get_alphabet().end()){
                Vector3D newLoc = Vector3D::point(currentLocation.x+vecH.x,currentLocation.y+vecH.y,currentLocation.z+vecH.z);
                f.points.push_back(currentLocation);
                f.points.push_back(newLoc);
                if(l_system.draw(c)){
                    f.faces.push_back(Face({int(f.points.size()-2),int(f.points.size()-1)}));
                }
                currentLocation = newLoc;
            }
        }
    }
}

void L3DReplacer::generateFigure(Figure &f) {
    f.points.push_back(currentLocation);
    drawRecursive(f,l_system.get_initiator(),l_system.get_nr_iterations());
}
