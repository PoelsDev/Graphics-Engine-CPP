//
// Created by thibaultpoels on 19/03/2020.
//
#include <algorithm>
#include "solids.h"
#include <cmath>


namespace pSolids{
    Figure createTetrahedron(){
        Figure f = Figure();

        f.points.push_back(Vector3D::point(1,-1,-1));
        f.points.push_back(Vector3D::point(-1,1,-1));
        f.points.push_back(Vector3D::point(1,1,1));
        f.points.push_back(Vector3D::point(-1,-1,1));

        f.faces.push_back(Face({0,1,2}));
        f.faces.push_back(Face({1,3,2}));
        f.faces.push_back(Face({0,3,1}));
        f.faces.push_back(Face({0,2,3}));

        return f;
    }

    Figure createCube(){
        Figure f = Figure();

        f.points.push_back(Vector3D::point(1,-1,-1));
        f.points.push_back(Vector3D::point(-1,1,-1));
        f.points.push_back(Vector3D::point(1,1,1));
        f.points.push_back(Vector3D::point(-1,-1,1));
        f.points.push_back(Vector3D::point(1,1,-1));
        f.points.push_back(Vector3D::point(-1,-1,-1));
        f.points.push_back(Vector3D::point(1,-1,1));
        f.points.push_back(Vector3D::point(-1,1,1));

        f.faces.push_back(Face({0,4,2,6}));
        f.faces.push_back(Face({4,1,7,2}));
        f.faces.push_back(Face({1,5,3,7}));
        f.faces.push_back(Face({5,0,6,3}));
        f.faces.push_back(Face({6,2,7,3}));
        f.faces.push_back(Face({0,5,1,4}));

        return f;
    }

    Figure createOctahedron(){
        Figure f = Figure();

        f.points.push_back(Vector3D::point(1,0,0));
        f.points.push_back(Vector3D::point(0,1,0));
        f.points.push_back(Vector3D::point(-1,0,0));
        f.points.push_back(Vector3D::point(0,-1,0));
        f.points.push_back(Vector3D::point(0,0,-1));
        f.points.push_back(Vector3D::point(0,0,1));

        f.faces.push_back(Face({0,1,5}));
        f.faces.push_back(Face({1,2,5}));
        f.faces.push_back(Face({2,3,5}));
        f.faces.push_back(Face({3,0,5}));
        f.faces.push_back(Face({1,0,4}));
        f.faces.push_back(Face({2,1,4}));
        f.faces.push_back(Face({3,2,4}));
        f.faces.push_back(Face({0,3,4}));

        return f;
    }

    Figure createIcosahedron(){
        Figure f = Figure();

        f.points.push_back(Vector3D::point(0,0,sqrt(5)/2));
        for(int i = 2; i < 7; i++){
            f.points.push_back(Vector3D::point(cos((i-2)*2*M_PI/5),sin((i-2)*2*M_PI/5),0.5));
        }
        for(int i = 7; i < 12; i++){
            f.points.push_back(Vector3D::point(cos(M_PI/5+(i-7)*2*M_PI/5),sin(M_PI/5+(i-7)*2*M_PI/5),-0.5));
        }
        f.points.push_back(Vector3D::point(0,0,-sqrt(5)/2));

        f.faces.push_back(Face({0,1,2}));
        f.faces.push_back(Face({0,2,3}));
        f.faces.push_back(Face({0,3,4}));
        f.faces.push_back(Face({0,4,5}));
        f.faces.push_back(Face({0,5,1}));
        f.faces.push_back(Face({1,6,2}));
        f.faces.push_back(Face({2,6,7}));
        f.faces.push_back(Face({2,7,3}));
        f.faces.push_back(Face({3,7,8}));
        f.faces.push_back(Face({3,8,4}));
        f.faces.push_back(Face({4,8,9}));
        f.faces.push_back(Face({4,9,5}));
        f.faces.push_back(Face({5,9,10}));
        f.faces.push_back(Face({5,10,1}));
        f.faces.push_back(Face({1,10,6}));
        f.faces.push_back(Face({11,7,6}));
        f.faces.push_back(Face({11,8,7}));
        f.faces.push_back(Face({11,9,8}));
        f.faces.push_back(Face({11,10,9}));
        f.faces.push_back(Face({11,6,10}));

        return f;
    }

    Figure createDodecahedron(){
        Figure f = Figure();
        Figure ico = createIcosahedron();

        for(Face face: ico.faces){
            double x = (ico.points[face.point_indexes[0]].x+ico.points[face.point_indexes[1]].x+ico.points[face.point_indexes[2]].x)/3;
            double y = (ico.points[face.point_indexes[0]].y+ico.points[face.point_indexes[1]].y+ico.points[face.point_indexes[2]].y)/3;
            double z = (ico.points[face.point_indexes[0]].z+ico.points[face.point_indexes[1]].z+ico.points[face.point_indexes[2]].z)/3;
            f.points.push_back(Vector3D::point(x,y,z));
        }

        f.faces.push_back(Face({0,1,2,3,4}));
        f.faces.push_back(Face({0,5,6,7,1}));
        f.faces.push_back(Face({1,7,8,9,2}));
        f.faces.push_back(Face({2,9,10,11,3}));
        f.faces.push_back(Face({3,11,12,13,4}));
        f.faces.push_back(Face({4,13,14,5,0}));
        f.faces.push_back(Face({19,18,17,16,15}));
        f.faces.push_back(Face({19,14,13,12,18}));
        f.faces.push_back(Face({18,12,11,10,17}));
        f.faces.push_back(Face({17,10,9,8,16}));
        f.faces.push_back(Face({16,8,7,6,15}));
        f.faces.push_back(Face({15,6,5,14,19}));

        return f;
    }
}

namespace solids{
    Figure createCylinder(const int n, const double h){
        Figure f = Figure();

        vector<int> indexes_down;
        vector<int> indexes_up;

        for(int i = 0;i<n;i++){
            f.points.push_back(Vector3D::point(cos((2*i*M_PI)/n),sin((2*i*M_PI)/n), 0));
        }

        for(int i = 0; i<n;i++){
            f.points.push_back(Vector3D::point(cos((2*i*M_PI)/n),sin((2*i*M_PI)/n), h));
        }

        for(int i = 0;i<2*n;i++){
            if(i<n){
                indexes_down.push_back(i);
            } else {
                indexes_up.push_back(i);
            }
        }

        for(int i = 0; i<n;i++){
            f.faces.push_back(Face({i,(i+1)%n,(((i+1)%n)+n),(n+i)}));
        }

        f.faces.emplace_back(Face(indexes_down));
        f.faces.emplace_back(Face(indexes_up));

        return f;
    }

    Figure createCone(const int n, const double h){
        Figure f = Figure();
        vector<int> indexes;
        for(int i = 0;i<=n;i++){
            indexes.push_back(i);
            f.points.push_back(Vector3D::point(cos((2*i*M_PI)/n),sin((2*i*M_PI)/n), 0));
        }

        f.points.push_back(Vector3D::point(0,0,h));

        for(int i = 0;i<=n;i++){
            f.faces.push_back(Face({i,(i+1)%n,n+1}));
        }
        f.faces.emplace_back(Face(indexes));

        return f;
    }

    Figure createSphere(const double radius, const int n){
        Figure f = pSolids::createIcosahedron();

        for(int i = 0; i<n;i++){
            vector<Face> newFaces;
            for(Face face:f.faces){
                // Current points list size for indexing later on
                int vectorFaceIndex = f.points.size();

                Vector3D A = f.points.at(face.point_indexes.at(0));
                Vector3D B = f.points.at(face.point_indexes.at(1));
                Vector3D C = f.points.at(face.point_indexes.at(2));

                Vector3D D = (A + B)/2;
                Vector3D E = (A + C)/2;
                Vector3D F = (B + C)/2;

                // Add new points
                f.points.push_back(D);
                f.points.push_back(F);
                f.points.push_back(E);

                // Create new faces and add to new faces list
                newFaces.push_back(Face({face.point_indexes.at(0),vectorFaceIndex+0,vectorFaceIndex+2}));
                newFaces.push_back(Face({face.point_indexes.at(1),vectorFaceIndex+1,vectorFaceIndex+0}));
                newFaces.push_back(Face({face.point_indexes.at(2),vectorFaceIndex+2,vectorFaceIndex+1}));
                newFaces.push_back(Face({vectorFaceIndex+0,vectorFaceIndex+1,vectorFaceIndex+2}));

            }
            // Add new faces to faces list
            f.faces = newFaces;
        }

        for(Vector3D& point: f.points){
            if(point.length() != radius){
                point.normalise();
                point*=radius;
            }

        }

        return f;
    }

    Figure createTorus(const double r, const double R, const int n, const int m){
        Figure f = Figure();

        for(int i = 0;i<n;i++){
            double u = (2*i*M_PI)/n;
            for(int j = 0;j<m;j++){
                double v = (2*j*M_PI)/m;
                f.points.push_back(Vector3D::point(((R + r*cos(v))*cos(u)),((R+r*cos(v))*sin(u)),r*sin(v)));
            }
        }

        for(int i = 0;i<n;i++){
            int initialIndex = i*m;
            for(int j = 0;j<m;j++){
                f.faces.push_back(Face({initialIndex+j,((initialIndex+m)%(n*m))+j,(((initialIndex+m)%(n*m))+((j+1)%m)),initialIndex+((j+1)%m)}));
            }
        }

        return f;
    }

    Figure createBuckyBall(){
        Figure ico = pSolids::createIcosahedron();
        Figure buckyBall = Figure();

        // All new faces
        for(Face f: ico.faces){

            int index = buckyBall.points.size()-1;

            // Original points
            Vector3D A = ico.points.at(f.point_indexes.at(0));
            Vector3D B = ico.points.at(f.point_indexes.at(1));
            Vector3D C = ico.points.at(f.point_indexes.at(2));

            // new points
            Vector3D D = A + (B-A)/3;
            Vector3D E = A/3+2*B/3;
            Vector3D F = B + (C-B)/3;
            Vector3D G = B/3+2*C/3;
            Vector3D H = A/3+2*C/3;
            Vector3D I = A + (C-A)/3;

            buckyBall.points.insert(buckyBall.points.end(), {D,E,F,G,H,I});

            // New face
            buckyBall.faces.push_back(Face({index+1,index+2,index+3,index+4,index+5,index+6}));
        }

        // 12 Pentagons
        buckyBall.faces.push_back(Face({0,5,12,17,24}));
        buckyBall.faces.push_back(Face({1,2,89,84,27}));
        buckyBall.faces.push_back(Face({3,4,8,41,33}));
        buckyBall.faces.push_back(Face({9,10,14,53,48}));
        buckyBall.faces.push_back(Face({39,40,44,99,100}));
        buckyBall.faces.push_back(Face({37,38,94,116,88}));
        buckyBall.faces.push_back(Face({15,16,71,66,57}));
        buckyBall.faces.push_back(Face({64,104,103,111,68}));
        buckyBall.faces.push_back(Face({51,52,56,105,106}));
        buckyBall.faces.push_back(Face({75,76,80,86,109}));
        buckyBall.faces.push_back(Face({107,102,108,114,90}));
        buckyBall.faces.push_back(Face({69,70,22,83,77}));

        return buckyBall;
    }

    void createMengerSponge(Figure& fig, Figures3D& fractal, const double nrIterations, const double scale){
        if(nrIterations > 0){
            for(Vector3D p0: fig.points){
                // Create new figure and scale
                Figure frac = fig;
                Matrix mScale = transform::scaleFigure(1/scale);
                transform::applyTransformation(frac, mScale);

                // Create translate matrix and move new figure
                Vector3D scaledP = p0*mScale;
                Matrix mTransform = transform::translate(p0-scaledP);
                transform::applyTransformation(frac,mTransform);

                for(Vector3D p1: fig.points){
                    if((p0.x == p1.x && p0.y == p1.y && p0.z != p1.z) || (p0.x != p1.x && p0.y == p1.y && p0.z == p1.z) || (p0.x == p1.x && p0.y != p1.y && p0.z == p1.z)){
                        Vector3D middlePoint = Vector3D::point((p0.x+p1.x)/2,(p0.y+p1.y)/2,(p0.z+p1.z)/2);
                        Figure middleFrac = fig;
                        Vector3D middleScaledP = middlePoint*mScale;
                        Matrix middleTransform = transform::translate(middlePoint-middleScaledP);
                        transform::applyTransformation(middleFrac,mScale);
                        transform::applyTransformation(middleFrac,middleTransform);
                        createMengerSponge(middleFrac,fractal,nrIterations-1,scale);
                    }
                }

                // Recursive call
                createMengerSponge(frac,fractal,nrIterations-1,scale);
            }
        } else if(nrIterations == 0){
            fractal.push_back(fig);
        }
    }
}

