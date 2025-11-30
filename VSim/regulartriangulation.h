#ifndef REGULARTRIANGULATION_H
#define REGULARTRIANGULATION_H

#include "../VisualObject.h"
#include "PointCloud.h"

class RegularTriangulation : public VisualObject
{
public:
    RegularTriangulation(const PointCloud& points, const float& resolution = 1.0f, const QVector2D& friction = {0.1f,0.05f});

    QVector4D FindBarycentric(const QVector3D& location);
    float FindTriangleHeight(const int& triangleIndex, const QVector3D& position);
    bool RayIntersectsTriangle(const QVector3D& rayOrigin, const QVector3D& rayDir,const QVector3D& v0,const QVector3D& v1,const QVector3D& v2, float& outT, QVector3D& outHitPoint);

    QVector2D GetTriangleFriction(const int& triangleIndex);
    void SetTriangleFriction(const int& triangleIndex, const QVector2D& friction);

    std::vector<QVector3D> mFaceNormals;
    std::vector<QVector2D> mFaceFrictions;
};

#endif // REGULARTRIANGULATION_H
