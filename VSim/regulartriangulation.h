#ifndef REGULARTRIANGULATION_H
#define REGULARTRIANGULATION_H

#include "../VisualObject.h"
#include "PointCloud.h"

class RegularTriangulation : public VisualObject
{
public:
    RegularTriangulation(const PointCloud& points, const float& resolution = 1.0f);
    QVector4D FindBarycentric(const QVector3D& location);
    float FindTriangleHeight(const int& triangleIndex, const QVector3D& position);

    std::vector<QVector3D> mFaceNormals;
};

#endif // REGULARTRIANGULATION_H
