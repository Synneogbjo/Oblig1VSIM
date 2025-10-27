#ifndef BALL_H
#define BALL_H

#include "ObjMesh.h"

class Ball : public ObjMesh
{
    float mRadius;
    float mGravity;

public:
    Ball(const float& radius, const float& gravity, const std::string& filename);
};

#endif // BALL_H
