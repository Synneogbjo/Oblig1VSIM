#ifndef MYMATHLIB_H
#define MYMATHLIB_H

#include "QVector2D"
#include "../Vertex.h"

class MyMathLib
{
public:
    MyMathLib();

    static QVector3D KileProdukt(const QVector2D& u, const QVector2D& v);
    static QVector3D CalculateNormal(const QVector3D& u, const QVector3D& v);
    static QVector3D CalculateNormal(const QVector3D& p1, const QVector3D& p2, const QVector3D& p3);
    static QVector3D CalculateNormal(const Vertex& v1, const Vertex& v2, const Vertex& v3);
};

#endif // MYMATHLIB_H
