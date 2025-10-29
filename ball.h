#ifndef BALL_H
#define BALL_H

#include "ObjMesh.h"
#include "triangulationmesh.h"

class Ball : public ObjMesh
{
    float mRadius;
    QVector3D mGravity;
    float mMass;

    float mFrictionCoeffisient = 0.5;

public:
    Ball(const float& radius, const QVector3D& gravity, const float& mass, const std::string& filename);

    QVector3D mVelocity{};
    QVector3D mAcceleration{};

    QVector3D CalculateBarycentric(const QVector2D& p1, const QVector2D& p2, const QVector2D& p3);
    float CalculateBarycentricHeight(const QVector3D& p1, const QVector3D& p2, const QVector3D& p3);
    FTriangle FindTriangle(const TriangulationMesh& mesh);
    void CalculateAccelerationAlongPlane(const TriangulationMesh& triangulationMesh);
};

#endif // BALL_H
