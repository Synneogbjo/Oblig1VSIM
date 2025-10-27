#include "triangulationmesh.h"

TriangulationMesh::TriangulationMesh()
{
    mVertices.push_back({0.f,1.f,0.f,1.f,0.f,0.f,0.f,0.f}); //Bottom Left
    mVertices.push_back({1.f,0.5f,0.f,0.5f,0.f,0.f,0.f,0.f}); //Bottom Middle
    mVertices.push_back({0.f,1.f,1.f,1.f,0.f,1.f,0.f,0.f}); //Top Left
    mVertices.push_back({1.f,0.6f,1.f,0.5f,0.f,1.f,0.f,0.f}); //Top Middle
    mVertices.push_back({2.f,0.8f,1.f,0.f,0.f,1.f,0.f,0.f}); //Top Right
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
