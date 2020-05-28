//
// Created by thibaultpoels on 04/04/2020.
//

#include "zBufferUtils.h"
#include "lineUtils.h"
#include "3DLineUtils.h"
#include "utils.h"

using namespace utils;

namespace zBufferUtils {

    void calculateDZ(Vector3D& A, Vector3D& B, Vector3D& C, double d,double& o_dzdx, double& o_dzdy){
        // U & V
        Vector3D u = B - A;
        Vector3D v = C - A;

        // W1-3
        Vector3D w = Vector3D::vector((u.y*v.z)-(u.z*v.y),(u.z*v.x)-(u.x*v.z),(u.x*v.y)-(u.y*v.x));

        // K
        double k = (w.x*A.x)+(w.y*A.y)+(w.z*A.z);

        o_dzdx = w.x/(-d*k);
        o_dzdy = w.y/(-d*k);
    }

    void getCenterOfGravity(Vector3D& A, Vector3D& B, Vector3D& C, double d, double dx, double dy, double& o_xg, double& o_yg, double& o_oneOverZg){
        // Get point of gravity for A'B'C'
        Point2D projectedA = transform::doProjection(A,d);
        Point2D projectedB = transform::doProjection(B,d);
        Point2D projectedC = transform::doProjection(C,d);

        projectedA += {dx,dy};
        projectedB += {dx,dy};
        projectedC += {dx,dy};

        o_xg = (projectedA.x + projectedB.x + projectedC.x)/3;
        o_yg = (projectedA.y + projectedB.y + projectedC.y)/3;
        o_oneOverZg = ((1/(3*A.z))+(1/(3*B.z))+(1/(3*C.z)));
    }



    ZBuffer::ZBuffer(const int width, const int height) {
        for (int i = 0; i < width; i++) {
            vector<double> v;
            for (int j = 0; j < height; j++) {
                v.push_back(std::numeric_limits<double>::infinity());
            }
            this->push_back(v);
        }
    }

    bool bufferCheck(ZBuffer & zbuf, unsigned int x, unsigned int y, double zValue) {
        if(zValue <= zbuf[x][y]) {
            zbuf[x][y] =
                    zValue;
            return true;
        }
        return false;
    }

    double getOneOverZValue(unsigned int i, int a, double z0, double z1) {
        return ((i / (double) a) / z0) + ((1 - i / (double) a) / z1);
    }

    double getOneOverZValueForTriangle(int x, double xG, int y, double yG, double dzdx, double dzdy, double oneOverZG){
        return (1.0001*(oneOverZG)+(x-xG)*dzdx+(y-yG)*dzdy);
    }

    void draw_zbuf_line(ZBuffer &zbuf, img::EasyImage &img, unsigned int x0, unsigned int y0, double z0, unsigned int x1,
                        unsigned int y1, double z1, const img::Color &color) {
        assert(x0 < img.get_width() && y0 < img.get_height());
        assert(x1 < img.get_width() && y1 < img.get_height());

        if (x0 == x1) {
            //special case for x0 == x1
            if (y0 != min(y0, y1)) {
                std::swap(y0, y1);
                std::swap(z0, z1);
            }
            for (unsigned int i = std::min(y0, y1); i <= std::max(y0, y1); i++) {
                double zValue = getOneOverZValue((max(y0, y1) - min(y0, y1)) - (i - min(y0, y1)), max(y0, y1) - min(y0, y1), z0,
                                            z1);
                if (bufferCheck(zbuf, x0, i, zValue)) {
                    img(x0, i) = color;
                }
            }
        } else if (y0 == y1) {
            //special case for y0 == y1
            if (x0 != min(x0, x1)) {
                std::swap(x0, x1);
                std::swap(y0, y1);
                std::swap(z0, z1);
            }
            for (unsigned int i = std::min(x0, x1); i <= std::max(x0, x1); i++) {
                double zValue = getOneOverZValue((max(x0, x1) - min(x0, x1)) - (i - min(x0, x1)), max(x0, x1) - min(x0, x1), z0,
                                            z1);
                if (bufferCheck(zbuf, i, y0, zValue)) {
                    img(i, y0) = color;
                }
            }
        } else {
            if (x0 > x1) {
                //flip points if x1>x0: we want x0 to have the lowest value
                std::swap(x0, x1);
                std::swap(y0, y1);
                std::swap(z0, z1);
            }
            double m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0);
            if (-1.0 <= m && m <= 1.0) {
                for (unsigned int i = 0; i <= (x1 - x0); i++) {
                    double zValue = getOneOverZValue((x1 - x0) - i, (x1 - x0), z0, z1);
                    unsigned int y = (unsigned int) round(y0 + m * i);
                    if (bufferCheck(zbuf, x0 + i, y, zValue)) {
                        img(x0 + i, y) = color;
                    }
                }
            } else if (m > 1.0) {
                for (unsigned int i = 0; i <= (y1 - y0); i++) {
                    double zValue = getOneOverZValue((y1 - y0) - i, (y1 - y0), z0, z1);
                    unsigned int x = (unsigned int) round(x0 + (i / m));
                    if (bufferCheck(zbuf, x, y0 + i, zValue)) {
                        img(x, y0 + i) = color;
                    }
                }
            } else if (m < -1.0) {
                for (unsigned int i = 0; i <= (y0 - y1); i++) {
                    double zValue = getOneOverZValue((y0 - y1) - i, (y0 - y1), z0, z1);
                    unsigned int x = (unsigned int) round(x0 - (i / m));
                    if (bufferCheck(zbuf, x, y0 - i, zValue)) {
                        img(x, y0 - i) = color;
                    }
                }
            }
        }
    }

    void draw_zbuf_triag(ZBuffer &zbuf, img::EasyImage &img, Vector3D &A, Vector3D &B, Vector3D &C, double d, double dx, double dy, img::Color &color) {

        // get projected points & add offsets
        Point2D projectedA = transform::doProjection(A,d);
        Point2D projectedB = transform::doProjection(B,d);
        Point2D projectedC = transform::doProjection(C,d);

        projectedA += {dx,dy};
        projectedB += {dx,dy};
        projectedC += {dx,dy};

        int minY = roundToInt(min(min(projectedA.y,projectedB.y),projectedC.y)+0.5);
        int maxY = roundToInt(max(max(projectedA.y,projectedB.y),projectedC.y)-0.5);


        // Get y's
        vector<int> Y;

        for(int i = minY;i <= maxY; i++){
            Y.push_back(i);
        }

        double xG;
        double yG;
        double oneOverZg;
        getCenterOfGravity(A,B,C,d,dx,dy,xG,yG,oneOverZg);

        for(int y: Y){
            // xL/xR variables
            double xL1 = std::numeric_limits<double>::infinity();
            double xL2 = std::numeric_limits<double>::infinity();
            double xL3 = std::numeric_limits<double>::infinity();
            double xR1 = -std::numeric_limits<double>::infinity();
            double xR2 = -std::numeric_limits<double>::infinity();
            double xR3 = -std::numeric_limits<double>::infinity();

            // get intersections
            if((y-projectedA.y)*(y-projectedB.y)<=0 && projectedA.y != projectedB.y){
                xL1 = projectedB.x + (projectedA.x - projectedB.x)*((y-projectedB.y)/(projectedA.y-projectedB.y));
                xR1 = xL1;
            }
            if((y-projectedA.y)*(y-projectedC.y)<=0 && projectedA.y != projectedC.y){
                xL2 = projectedC.x + (projectedA.x - projectedC.x)*((y-projectedC.y)/(projectedA.y-projectedC.y));
                xR2 = xL2;
            }

            if((y-projectedB.y)*(y-projectedC.y)<=0 && projectedB.y != projectedC.y){
                xL3 = projectedC.x + (projectedB.x - projectedC.x)*((y-projectedC.y)/(projectedB.y-projectedC.y));
                xR3 = xL3;
            }

            int xL = roundToInt(min(min(xL1,xL2),xL3)+0.5);
            int xR = roundToInt(max(max(xR1,xR2),xR3)-0.5);

            // Actual draw & Z-Buffering check
            for(int x = xL; x <= xR; x++){
                double dzdx;
                double dzdy;

                calculateDZ(A,B,C,d,dzdx,dzdy);
                double zVal = getOneOverZValueForTriangle(x,xG,y,yG,dzdx,dzdy,oneOverZg);
                if(bufferCheck(zbuf,x,y,zVal)){
                    img(x, y) = color;
                }
            }
        }
    }
}

