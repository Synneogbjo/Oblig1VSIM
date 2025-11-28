#ifndef REGULARTRIANGULATION_H
#define REGULARTRIANGULATION_H

#include "../VisualObject.h"
#include "PointCloud.h"

class RegularTriangulation : public VisualObject
{
public:
    RegularTriangulation(const PointCloud& points, const float& resolution = 1.0f);

    std::vector<QVector3D> mFaceNormals;
};

#endif // REGULARTRIANGULATION_H
