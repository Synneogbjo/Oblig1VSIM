#include "mymathlib.h"
#include <QDebug>

MyMathLib::MyMathLib() {}

QVector3D MyMathLib::BarycentricCoordinates(const QVector2D& triangleP1, const QVector2D& triangleP2, const QVector2D& triangleP3, const QVector2D& point)
{
    //Inspired by Dag's lecture notes chapter 6.2.3

    QVector3D barycentric;

    float areaABC = KileProdukt(triangleP2 - triangleP1, triangleP3 - triangleP1).z(); // signed area

    float areaPBC = KileProdukt(triangleP2 - point, triangleP3 - point).z();
    float areaPCA = KileProdukt(triangleP3 - point, triangleP1 - point).z();
    float areaPAB = KileProdukt(triangleP1 - point, triangleP2 - point).z();

    barycentric.setX(areaPBC / areaABC); // u
    barycentric.setY(areaPCA / areaABC); // v
    barycentric.setZ(areaPAB / areaABC); // w

    return barycentric;
}

float MyMathLib::CalculateBarycentricHeight(const QVector3D& triangleP1, const QVector3D& triangleP2, const QVector3D& triangleP3, const QVector3D& point)
{
    //Inspired by Dag's lecture notes chapter 6.2.4

    QVector2D point2D(point.x(), point.z());
    QVector3D barycentric = BarycentricCoordinates({triangleP1.x(), triangleP1.z()}, {triangleP2.x(), triangleP2.z()}, {triangleP3.x(), triangleP3.z()}, point2D);

    float height = (barycentric.x() * triangleP1.y()) + (barycentric.y() * triangleP2.y()) + (barycentric.z() * triangleP3.y());

    return height;
}

QVector3D MyMathLib::KileProdukt(const QVector2D& u, const QVector2D& v)
{
    //Taken from Dag's lecture notes chapter 6.2.2

    return QVector3D(0,0,(u.x() * v.y()) - (u.y() * v.x()));
}

///
/// @brief MyMathLib::CalculateNormal - Returns the normal for the given u and v vectors.
/// @param u - First vector
/// @param v - Second vector
/// @return - Normal vector
///
QVector3D MyMathLib::CalculateNormal(const QVector3D& u, const QVector3D& v)
{
    //Inspired by Dag's lecture notes chapter 6.2.1
    return QVector3D(u.y() * v.z() - u.z() * v.y(),
            u.z() * v.x() - u.x() * v.z(),
            u.x() * v.y() - u.y() * v.x()).normalized();
}

///
/// @brief MyMathLib::CalculateNormal - Returns the normal for the given points p1, p2, and p3.
/// @param p1 - First point
/// @param p2 - Second point
/// @param p3 - Third point
/// @return - Normal vector
///
QVector3D MyMathLib::CalculateNormal(const QVector3D& p1, const QVector3D& p2, const QVector3D& p3)
{
    return CalculateNormal(p2 - p1, p3 - p1);
}

///
/// @brief MyMathLib::CalculateNormal - Returns the normal for the given vertices v1, v2, and v3
/// @param v1 - First vertex
/// @param v2 - Second vertex
/// @param v3 - Third vertex
/// @return - Normal vector
///
QVector3D MyMathLib::CalculateNormal(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
    return CalculateNormal(QVector3D(v1.x,v1.y,v1.z) - QVector3D(v2.x,v2.y,v2.z), QVector3D(v1.x,v1.y,v1.z) - QVector3D(v3.x,v3.y,v3.z));
}

float MyMathLib::Abs(const float& value)
{
    return (value < 0) ? -value : value;
}
