#include "traceline.h"

TraceLine::TraceLine()
{
    drawType = 1;
}

void TraceLine::AddVertex(QVector3D newPoint)
{
    int index = mVertices.size();

    mVertices.push_back({newPoint.x(), newPoint.y(), newPoint.z(), 1.f,1.f,1.f,0.f,0.f});

    if (index > 0) mIndices.push_back(index - 1);
    mIndices.push_back(index + 1);
}
