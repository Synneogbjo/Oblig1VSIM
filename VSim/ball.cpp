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

void Ball::CalculateAcceleration(RegularTriangulation* mesh, const float& deltaTime)
{
    if (!mesh) return;

    qDebug() << "Ball location: " << getPosition();

    QVector4D barycentricWithIndex = mesh->FindBarycentric(getPosition());

    if (barycentricWithIndex.x() < 0)
    {
        mAcceleration = mGravity;
    }
    else
    {
        QVector3D normal = mesh->mFaceNormals[barycentricWithIndex.x()].normalized();
        mAcceleration = mGravity - (QVector3D::dotProduct(mGravity, normal) * normal);
        //mAcceleration = mGravity * QVector3D(normal.x() * normal.z(), normal.y() * normal.z(), normal.z() * normal.z() - 1);
    }
}

void Ball::ApplyAcceleration(const float& deltaTime)
{
    mVelocity += mAcceleration * deltaTime;

    qDebug() << "Acceleration: " << mAcceleration << " | Velocity: " << mVelocity;

    move(mVelocity * deltaTime);
}
