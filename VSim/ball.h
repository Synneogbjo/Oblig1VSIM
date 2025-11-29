#ifndef BALL_H
#define BALL_H

#include "../ObjMesh.h"
#include "../triangulationmesh.h"
#include "regulartriangulation.h"

class Ball : public ObjMesh
{
    float mRadius;
    float mMass;
    QVector3D mGravity;

public:
    Ball(const float& radius, const QVector3D& gravity, const float& mass, const std::string& filename, const QVector3D spawnPosition = {0,0,0});

    QVector3D mVelocity{};
    QVector3D mAcceleration{};

    void CalculateAcceleration(RegularTriangulation* mesh, const float& deltaTime);
    void ApplyAcceleration(const float& deltaTime);
};

#endif // BALL_H
