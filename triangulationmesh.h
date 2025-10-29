#ifndef TRIANGULATIONMESH_H
#define TRIANGULATIONMESH_H

#include "VisualObject.h"
#include "TriangleStruct.h"

class TriangulationMesh : public VisualObject
{
public:
    TriangulationMesh();
    std::vector<FTriangle> getTriangles() const;
};

#endif // TRIANGULATIONMESH_H
