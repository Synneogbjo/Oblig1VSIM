#include "ball.h"

Ball::Ball(const float& radius, const float& gravity, const std::string& filename) : ObjMesh(filename)
{
    mRadius = radius;
    mGravity = gravity;

    scale(radius);
}
