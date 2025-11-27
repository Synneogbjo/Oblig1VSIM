#include "ball.h"
#include "mymathlib.h"
#include <cmath>

Ball::Ball(const float& radius, const QVector3D& gravity, const float& mass, const std::string& filename) : ObjMesh(filename)
{
    mRadius = radius;
    mGravity = gravity;
    mMass = mass;
}

QVector3D Ball::CalculateBarycentric(const QVector2D& p1, const QVector2D& p2, const QVector2D& p3)
{
    QVector2D p12 = p2 - p1;
    QVector2D p13 = p3 - p1;

    QVector3D n = MyMathLib::KileProdukt(p12, p13);

    qDebug() << "p12: " << p12 << " p13: " << p13 << " n: " << n;

    float area = n.length();

    qDebug() << "area: " << area;

    QVector3D barycentric;
    QVector2D myPos = QVector2D(this->getPosition().x(), this->getPosition().z());

    //Calculates X
    QVector2D p = p2 - myPos;
    QVector2D q = p3 - myPos;
    barycentric.setX(MyMathLib::KileProdukt(p, q).z() / area);
    //Calculates Y
    p = p3 - myPos;
    q = p1 - myPos;
    barycentric.setY(MyMathLib::KileProdukt(p, q).z() / area);
    //Calculates Z
    p = p1 - myPos;
    q = p2 - myPos;
    barycentric.setZ(MyMathLib::KileProdukt(p, q).z() / area);

    qDebug() << "bary: " << barycentric;

    return barycentric;
}

float Ball::CalculateBarycentricHeight(const QVector3D& p1, const QVector3D& p2, const QVector3D& p3)
{
    QVector3D barycentric = CalculateBarycentric(QVector2D(p1.x(),p1.z()), QVector2D(p2.x(),p2.z()),QVector2D(p3.x(),p3.z()));

    return (barycentric.x() * p1.z()) + (barycentric.y() * p2.z()) + (barycentric.z() * p3.z());
}

FTriangle Ball::FindTriangle(const TriangulationMesh& triangulationMesh)
{
    for (auto triangle : triangulationMesh.getTriangles())
    {
        auto barycentric = CalculateBarycentric(QVector2D(triangle.v1.x(), triangle.v1.z()), QVector2D(triangle.v2.x(), triangle.v2.z()),QVector2D(triangle.v3.x(), triangle.v3.z()));

        qDebug() << "barycentric:" << barycentric;

        if (barycentric.x() >= 0 && barycentric.x() <= 1 && barycentric.y() >= 0 && barycentric.y() <= 1 && barycentric.z() >= 0 && barycentric.z() <= 1)
        {
            qDebug() << "This one is valid!";
            triangle.isValid = true;
            return triangle;
        }
    }

    return {};
}

void Ball::CalculateAccelerationAlongPlane(const TriangulationMesh& triangulationMesh)
{
    FTriangle tri = FindTriangle(triangulationMesh);

    if (!tri.isValid)
    {
        mAcceleration = {0,0,0};
        mVelocity = {0,0,0};
        qDebug() << "Ball could not find a valid triangle!";

        return;
    }

    QVector3D edge1 = tri.v2 - tri.v1;
    QVector3D edge2 = tri.v3 - tri.v1;
    QVector3D normal = QVector3D::crossProduct(edge1, edge2).normalized();

    QVector3D gravityNormal = normal * QVector3D::dotProduct(mGravity, normal);

    mAcceleration = mGravity - gravityNormal;
}
