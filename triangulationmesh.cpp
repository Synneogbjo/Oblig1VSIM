#include "triangulationmesh.h"

TriangulationMesh::TriangulationMesh()
{
    drawType = 0;

    mVertices.push_back({0.f,1.f,0.f,1.f,0.f,0.f,0.f,0.f}); //Bottom Left
    mVertices.push_back({1.f,0.6f,0.f,0.5f,0.f,0.f,0.f,0.f}); //Bottom Middle
    mVertices.push_back({0.f,1.f,1.f,1.f,0.f,1.f,0.f,0.f}); //Top Left
    mVertices.push_back({1.f,0.8f,1.f,0.5f,0.f,1.f,0.f,0.f}); //Top Middle
    mVertices.push_back({2.f,0.4f,1.f,0.f,0.f,1.f,0.f,0.f}); //Top Right
    mVertices.push_back({2.f,0.3f,0.f,0.f,0.f,0.f,0.f,0.f}); //Bottom Right

    //Bottom Left Triangle
    mIndices.push_back(0);
    mIndices.push_back(1);
    mIndices.push_back(2);

    //Top Left Triangle
    mIndices.push_back(2);
    mIndices.push_back(1);
    mIndices.push_back(3);

    //Top Right Triangle
    mIndices.push_back(1);
    mIndices.push_back(4);
    mIndices.push_back(3);

    //Bottom Right Triangle
    mIndices.push_back(1);
    mIndices.push_back(5);
    mIndices.push_back(4);
}

std::vector<FTriangle> TriangulationMesh::getTriangles() const
{
    std::vector<FTriangle> triangles;

    for (int i = 0; i + 2 < mIndices.size(); i += 2)
    {
        FTriangle tri = {true, QVector3D(mVertices[mIndices[i]].x,mVertices[mIndices[i]].y,mVertices[mIndices[i]].z),
                        QVector3D(mVertices[mIndices[i+1]].x,mVertices[mIndices[i+1]].y,mVertices[mIndices[i+1]].z),
                        QVector3D(mVertices[mIndices[i+2]].x,mVertices[mIndices[i+2]].y,mVertices[mIndices[i+2]].z)};

        triangles.push_back(tri);

        qDebug() << "tri: " << tri.v1 << tri.v2 << tri.v3;
    }

    return triangles;
}
