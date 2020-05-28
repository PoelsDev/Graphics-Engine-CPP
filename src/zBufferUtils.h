//
// Created by thibaultpoels on 04/04/2020.
//

#ifndef ENGINE_ZBUFFERUTILS_H
#define ENGINE_ZBUFFERUTILS_H

#include "library.h"
#include "Lighting.h"

/**
 * @brief namespace used for all Z-Buffering methods/classes 
 */
 
namespace zBufferUtils {

    class ZBuffer: public std::vector<std::vector<double>>
    {
        /**
         * @brief Z-Buffer class
         */
        
        public:
            ZBuffer(const int width, const int height);
    };

    
    /**
     * @brief the following 5 function are supportive and calculate information for the Z-Buffering procedure
     */
    
    void calculateDZ(Vector3D& A, Vector3D& B, Vector3D& C, double d, double& o_dzdx, double& o_dzdy);

    void getCenterOfGravity(Vector3D& A, Vector3D& B, Vector3D& C, double d, double dx, double dy,double& o_xg, double& o_yg, double& o_oneOverZg);

    double getOneOverZValue(unsigned int i, int a, double z0, double z1);

    double getOneOverZValueForTriangle(int x, double xG, int y, double yG, double dzdx, double dzdy, double oneOverZG);

    bool bufferCheck(ZBuffer &zbuf, unsigned int x, unsigned int y, double zValue);
    
    
    /**
     * @brief The following 2 functions are used to draw either a line drawing or triangulated drawing with Z-Buffering
     */
    
    void draw_zbuf_line(ZBuffer &zbuf, img::EasyImage &img, unsigned int x0, unsigned int y0, double z0, unsigned int x1, unsigned int y1, double z1, const img::Color &color);

    void draw_zbuf_triag(ZBuffer &zbuf, img::EasyImage &img, Vector3D &A, Vector3D &B, Vector3D &C, double d, double dx, double dy, img::Color &color);

}

#endif //ENGINE_ZBUFFERUTILS_H
