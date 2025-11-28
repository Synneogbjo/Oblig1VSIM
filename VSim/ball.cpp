#include "ball.h"
#include "mymathlib.h"
#include <cmath>

///
/// @brief Ball::Ball - Constructor
/// @param radius - float
/// @param gravity - QVector3D
/// @param mass - float
/// @param filename - string
/// @param spawnPosition - QVector3D, default {0,0,0}
///
Ball::Ball(const float& radius, const QVector3D& gravity, const float& mass, const std::string& filename, const QVector3D spawnPosition) : ObjMesh(filename)
{
    mRadius = radius;
    mMass = mass;
    mGravity = gravity;

    setPosition(spawnPosition.x(), spawnPosition.y(), spawnPosition.z());
}

///
/// @brief Ball::CalculateMovement - Updates acceleration, velocity and position.
/// @param mesh - The Regular Triangulation mesh the ball is rolling down.
///
void Ball::CalculateMovement(RegularTriangulation* mesh, const float& deltaTime)
{
    if (!mesh) return;

    QVector4D barycentricWithIndex = mesh->FindBarycentric(this->getPosition());
    QVector3D normal = mesh->mFaceNormals[barycentricWithIndex.x()];

    mAcceleration = mGravity * QVector3D(normal.x() * normal.z(), normal.y() * normal.z(), normal.z() * normal.z() - 1);
    mVelocity += mAcceleration;
    move(mVelocity * deltaTime);
}
