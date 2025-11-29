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

void Ball::Update(RegularTriangulation* mesh, const float& deltaTime)
{
    //Runs all acceleration and movement logic
    UpdateBarycentricCoordinates(mesh);
    CalculateAcceleration(mesh, deltaTime);
    ApplyAcceleration(deltaTime);

    //Runs all collision checks, automatically fixing any incorrect movement after the fact
    CheckForSurfaceCollision(mesh, deltaTime);
}

void Ball::UpdateBarycentricCoordinates(RegularTriangulation* mesh)
{
    if (!mesh) return;

    mBarycentricWithIndex = mesh->FindBarycentric(getPosition());
}

void Ball::CalculateAcceleration(RegularTriangulation* mesh, const float& deltaTime)
{
    if (!mesh) return;

    if (mBarycentricWithIndex.x() < 0)
    {
        mAcceleration = mGravity;
    }
    else
    {
        float distanceToSurface = MyMathLib::Abs(getPosition().y() - mesh->FindTriangleHeight(mBarycentricWithIndex.x(), getPosition()));
        QVector3D normal = mesh->mFaceNormals[mBarycentricWithIndex.x()].normalized();

        //Checks if ball is close enough to the surface to start rolling. Is not fully accurate yet. TODO: Make calculation of when to start rolling more accurate
        if (distanceToSurface >= (mGravity.length() * deltaTime) + mRadius) mAcceleration = mGravity;
        else mAcceleration = mGravity - (QVector3D::dotProduct(mGravity, normal) * normal);
    }
}

void Ball::ApplyAcceleration(const float& deltaTime)
{
    mVelocity += mAcceleration * deltaTime;

    move(mVelocity * deltaTime);
}

void Ball::CheckForSurfaceCollision(RegularTriangulation* mesh, const float& deltaTime)
{
    float distanceToSurface = MyMathLib::Abs(getPosition().y() - mesh->FindTriangleHeight(mBarycentricWithIndex.x(), getPosition()));

    qDebug() << distanceToSurface;

    //Checks if ball is currently colliding with surface
    if (distanceToSurface < mRadius)
    {
        QVector3D surfaceNormal = mesh->mFaceNormals[mBarycentricWithIndex.x()].normalized();
        QVector3D newVelocity = mVelocity - 2 * (mVelocity * surfaceNormal) * surfaceNormal; // Equation 9.18

        QVector3D heightCorrection = (((mRadius - distanceToSurface)/mRadius) * mVelocity) + (distanceToSurface/mRadius) * newVelocity; //Equation on page 139

        //Applying changes to velocity and position
        mVelocity = newVelocity;
        move(heightCorrection * deltaTime);
    }
}
