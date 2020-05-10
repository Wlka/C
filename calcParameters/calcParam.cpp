#include <iostream>
#include <vector>
#include <cmath>
#include "matrix.h"

using namespace std;

const double PI = 3.141592;

class Point2D
{
public:
    Point2D(double x, double y) : x(x), y(y) {}
    double X() const
    {
        return x;
    }
    double Y() const
    {
        return y;
    }

private:
    double x;
    double y;
};

class Point3D
{
public:
    Point3D(double x, double y, double z) : x(x), y(y), z(z) {}
    double X() const
    {
        return x;
    }
    double Y() const
    {
        return y;
    }
    double Z() const
    {
        return z;
    }

private:
    double x;
    double y;
    double z;
};

class fourPara
{
public:
    fourPara(double r, double s, double x, double y) : rotate(r), scale(s), dx(x), dy(y) {}
    double Rotate() const
    {
        return rotate;
    }
    double Scale() const
    {
        return scale;
    }
    double Dx() const
    {
        return dx;
    }
    double Dy() const
    {
        return dy;
    }

private:
    double rotate;
    double scale;
    double dx;
    double dy;
};

class sevenPara
{
public:
    sevenPara(double rx, double ry, double rz, double scale, double x, double y, double z) : rotateX(rx), rotateY(ry), rotateZ(rz), scale(scale), dx(x), dy(y), dz(z) {}
    double RotateX() const
    {
        return rotateX;
    }
    double RotateY() const
    {
        return rotateY;
    }
    double RotateZ() const
    {
        return rotateZ;
    }
    double Scale() const
    {
        return scale;
    }
    double Dx() const
    {
        return dx;
    }
    double Dy() const
    {
        return dy;
    }
    double Dz() const
    {
        return dz;
    }

private:
    double rotateX;
    double rotateY;
    double rotateZ;
    double scale;
    double dx;
    double dy;
    double dz;
};

double coordinateAzimuth(const Point2D &p1, const Point2D &p2)
{
    /*坐标方位角        坐标增量情况
     *  0-90     ->     dx>0,dy>0
     * 90-180    ->     dx<0,dy>0
     * 180-270   ->     dx<0,dy<0
     * 270-360   ->     dx>0,dy<0
    */
    double dx = p2.X() - p1.X();
    double dy = p2.Y() - p1.Y();
    double res = atan(fabs(dx) / fabs(dy));
    if (dx < 0 && dy > 0)
        res += 90 * PI / 180;
    else if (dx < 0 && dy < 0)
        res += 180 * PI / 180;
    else if (dx > 0 && dy < 0)
        res += 270 * PI / 180;
    return res;
}

double dist(const Point2D &p1, const Point2D &p2)
{
    return sqrt(pow((p2.X() - p1.X()), 2) + pow((p2.Y() - p1.Y()), 2));
}

fourPara calcFourParameters(vector<Point2D> &p1, vector<Point2D> &p2)
{
    /*   四参数计算(XY坐标位移量{米}、旋转角度{弧度}、缩放尺度{无单位})
     * |x2|     |dx|              |cos(rotate)  -sin(rotate)|  |x1|
     *       =       + (1+scale)*                            *
     * |y2|     |dy|              |sin(rotate)  cos(rotate)|   |y2|
    */
    if (p1.size() != p2.size())
    {
        throw "两个坐标系下的点数量不同，无法计算";
    }
    int pointCount = p1.size();
    if (pointCount < 2)
    {
        throw "点数量太少，无法完成计算";
    }
    //平差求解
    double u = 1, v = 0, Dx = 0, Dy = 0;
    Matrix corrections(4, 1);
    Matrix B(2 * pointCount, 4);
    Matrix Bt(4, 2 * pointCount);
    Matrix W(2 * pointCount, 1);
    Matrix N(4, 4);
    Matrix invN(4, 4);
    Matrix BtW(4, 1);
    for (int i = 0; i < pointCount; ++i)
    {
        //计算误差方程系数
        B(2 * i, 0) = 1;
        B(2 * i, 1) = 0;
        B(2 * i, 2) = p1[i].X();
        B(2 * i, 3) = -p1[i].Y();

        B(2 * i + 1, 0) = 0;
        B(2 * i + 1, 1) = 1;
        B(2 * i + 1, 2) = p1[i].Y();
        B(2 * i + 1, 3) = p1[i].X();
    }
    for (int i = 0; i < pointCount; ++i)
    {
        //计算误差方程常数
        W(2 * i, 0) = p2[i].X() - u * p1[i].X() + v * p1[i].Y() - Dx;
        W(2 * i + 1, 0) = p2[i].Y() - u * p1[i].Y() - v * p1[i].X() - Dy;
    }
    //最小二乘求解
    Bt = B.transpose();
    N = Bt * B;
    invN = N.inversion();
    BtW = Bt * W;
    corrections = invN * BtW;
    Dx += corrections(0, 0);
    Dy += corrections(1, 0);
    u += corrections(2, 0);
    v += corrections(3, 0);
    double rotate = atan(v / u);
    double scale = u / cos(rotate);
    double dx = Dx;
    double dy = Dy;
    return fourPara(rotate, scale, dx, dy);
}

//Result in dx/dy/dz/scale are error
sevenPara calcSevenParameters(vector<Point3D> &p1, vector<Point3D> &p2)
{
    //七参数计算(XYZ坐标位移量{米}、XYZ旋转角度{弧度}、缩放尺度{无单位})
    if (p1.size() != p2.size())
    {
        throw "两个坐标系下的点数量不同，无法计算";
    }
    int pointCount = p1.size();
    Matrix B(pointCount * 3, 7);
    Matrix corrections(7, 1);
    Matrix L(pointCount * 3, 1);
    Matrix Bt(7, pointCount * 3);
    Matrix N(7, 7);
    Matrix invN(7, 7);
    Matrix BtL(7, 1);

    //初始化L矩阵
    for (int i = 0; i < pointCount * 3; ++i)
    {
        if (i % 3 == 0)
        {
            L(i, 0) = p2[i / 3].X();
        }
        else if (i % 3 == 1)
        {
            L(i, 0) = p2[i / 3].Y();
        }
        else if (i % 3 == 2)
        {
            L(i, 0) = p2[i / 3].Z();
        }
    }
    //初始化B矩阵
    for (int i = 0; i < pointCount * 3; ++i)
    {
        if (i % 3 == 0)
        {
            B(i, 0) = 1;
            B(i, 1) = 0;
            B(i, 2) = 0;
            B(i, 3) = p1[i / 3].X();
            B(i, 4) = 0;
            B(i, 5) = -p1[i / 3].Z();
            B(i, 6) = p1[i / 3].Y();
        }
        else if (i % 3 == 1)
        {
            B(i, 0) = 0;
            B(i, 1) = 1;
            B(i, 2) = 0;
            B(i, 3) = p1[i / 3].Y();
            B(i, 4) = p1[i / 3].Z();
            B(i, 5) = 0;
            B(i, 6) = -p1[i / 3].X();
        }
        else if (i % 3 == 2)
        {
            B(i, 0) = 0;
            B(i, 1) = 0;
            B(i, 2) = 1;
            B(i, 3) = p1[i / 3].Z();
            B(i, 4) = -p1[i / 3].Y();
            B(i, 5) = p1[i / 3].X();
            B(i, 6) = 0;
        }
    }
    Bt = B.transpose();
    N = Bt * B;
    invN = N.inversion();
    BtL = Bt * L;
    corrections = invN * BtL;

    double dx = corrections(0, 0);
    double dy = corrections(1, 0);
    double dz = corrections(2, 0);
    double scale = corrections(3, 0);
    double rotateX = corrections(4, 0)/corrections(3,0);
    double rotateY = corrections(5, 0)/corrections(3,0);
    double rotateZ = corrections(6, 0)/corrections(3,0);
    return sevenPara(rotateX, rotateY, rotateZ, scale, dx, dy, dz);
}

int main()
{
    vector<Point2D> p1;
    p1.push_back(Point2D(10, 20));
    p1.push_back(Point2D(20, 10));
    vector<Point2D> p2;
    p2.push_back(Point2D(15, 24));
    p2.push_back(Point2D(23, 17));
    fourPara res = calcFourParameters(p1, p2);
    cout << res.Dx() << " " << res.Dy() << " " << " " << res.Rotate()  << " " << res.Scale() << endl;
    // vector<Point3D> p1;
    // p1.push_back(Point3D(10, 20, 5));
    // p1.push_back(Point3D(20, 10, 7));
    // p1.push_back(Point3D(15, 40, 6));
    // vector<Point3D> p2;
    // p2.push_back(Point3D(15, 24, 7));
    // p2.push_back(Point3D(23, 17, 4));
    // p2.push_back(Point3D(27, 35, 9));
    // sevenPara res = calcSevenParameters(p1, p2);
    // cout << res.Dx() << " " << res.Dy() << " " << res.Dz() << " " 
    //     << res.RotateX() << " " << res.RotateY() << " " << res.RotateZ() << " " << res.Scale() << endl;
    return 0;
}
