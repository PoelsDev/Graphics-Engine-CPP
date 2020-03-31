//
// Created by thibaultpoels on 19/03/2020.
//

#ifndef ENGINE_SOLIDS_H
#define ENGINE_SOLIDS_H

#include "3DLineUtils.h"
#include "lineUtils.h"
#include "utils.h"

namespace pSolids{
    Figure createTetrahedron();
    Figure createCube();
    Figure createOctahedron();
    Figure createIcosahedron();
    Figure createDodecahedron();
}

namespace solids {
    Figure createCylinder(const int n, const double h);
    Figure createCone(const int n, const double h);
    Figure createSphere(const double radius, const int n);
    Figure createTorus(const double r, const double R, const int n, const int m);
}


#endif //ENGINE_SOLIDS_H
