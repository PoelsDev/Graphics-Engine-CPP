//
// Created by thibaultpoels on 19/03/2020.
//

#ifndef ENGINE_SOLIDS_H
#define ENGINE_SOLIDS_H

#include "3DLineUtils.h"
#include "lineUtils.h"
#include "utils.h"

/**
 * @brief namespace used to create platonic solids and return them in a Figure class
 * The function names describe which solid is created.
 */

namespace pSolids{
    Figure createTetrahedron();
    Figure createCube();
    Figure createOctahedron();
    Figure createIcosahedron();
    Figure createDodecahedron();
}


/**
 * @brief namespace used to create other solids and return them in a Figure class
 * The function names describe which solid is created.
 */

namespace solids {
    Figure createCylinder(const int n, const double h);
    Figure createCone(const int n, const double h);
    Figure createSphere(const double radius, const int n);
    Figure createTorus(const double r, const double R, const int n, const int m);
    Figure createBuckyBall();
    void createMengerSponge(Figure& fig, Figures3D& fractal, const double nrIterations, const double scale);
}


#endif //ENGINE_SOLIDS_H
