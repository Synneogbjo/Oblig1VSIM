#ifndef REGULARTRIANGULATION_H
#define REGULARTRIANGULATION_H

#include "../VisualObject.h"
#include "PointCloud.h"

class RegularTriangulation : public VisualObject
{
public:
    RegularTriangulation(const PointCloud& points, const float& resolution = 1.0f);
};

#endif // REGULARTRIANGULATION_H
