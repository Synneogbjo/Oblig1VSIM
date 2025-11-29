#include "regulartriangulation.h"
#include "mymathlib.h"

RegularTriangulation::RegularTriangulation(const PointCloud& points, const float& resolution)
{
    drawType = 0;

    if (points.lineCount <= 0)
    {
        qDebug() << "Warning: Inserted PointCloud when initializing a new RegularTriangulation has a line count <= 0!";
        return;
    }

    int gridWidth = floor((points.maxPos.x() - points.minPos.x()) / resolution) + 1;
    int gridHeight = floor((points.maxPos.z() - points.minPos.z()) / resolution) + 1;

    std::vector<QVector4D> squarePoints; //First three values are x, y, z. The last value is used to count the points from the PointCloud that are within this square.
    float halfResolution = resolution * 0.5f;

    for (float z = points.minPos.z(); z <= points.maxPos.z(); z += resolution)
    {
        for (float x = points.minPos.x(); x <= points.maxPos.x(); x += resolution)
        {
            squarePoints.push_back({x, 0, z, 0});
            //qDebug() << "x: " << (x) << " z: " << (z);
        }
    }

    float resolutionInverted = 1.0f / resolution;
    int ix = 0;
    int iz = 0;
    int index = -1;

    for (const Vertex& point : points.getVertices())
    {
        ix = static_cast<int>((point.x - points.minPos.x()) * resolutionInverted);
        iz = static_cast<int>((point.z - points.minPos.z()) * resolutionInverted);

        if (ix < 0 || ix >= gridWidth || iz < 0 || iz >= gridHeight) continue;
        index = (iz * gridWidth) + ix;

        squarePoints[index].setY(squarePoints[index].y() + point.y);
        squarePoints[index].setW(squarePoints[index].w() + 1);
    }

    QVector3D colorScalar(1 / (points.maxPos.x() - points.minPos.x()), 1 / (points.maxPos.y() - points.minPos.y()), 1 / (points.maxPos.z() - points.minPos.z()));

    for (auto& point : squarePoints)
    {
        if (point.w() > 0) point.setY(point.y() / point.w());

        mVertices.push_back({point.x(), point.y(), point.z(),
                             point.x() * colorScalar.x(), point.y() * colorScalar.y(), point.z() * colorScalar.z(),
                             0, 0});
        //qDebug() << "x " << point.x() << " y " << point.y() << " z " << point.z();
    }

    qDebug() << "gridHeight: " << gridHeight << "gridWidth: " << gridWidth;

    int squaresMade = 0;

    for (int z = 0; z < gridHeight - 1; ++z)
    {
        for (int x = 0; x < gridWidth - 1; ++x)
        {
            int topLeft = (z * gridWidth) + x;
            int topRight = topLeft + 1;
            int bottomLeft = ((z + 1) * gridWidth) + x;
            int bottomRight = bottomLeft + 1;

            mIndices.push_back(topLeft);
            mIndices.push_back(bottomLeft);
            mIndices.push_back(topRight);
            mFaceNormals.push_back(MyMathLib::CalculateNormal(mVertices[topLeft], mVertices[bottomLeft], mVertices[topRight]));

            mIndices.push_back(topRight);
            mIndices.push_back(bottomLeft);
            mIndices.push_back(bottomRight);
            mFaceNormals.push_back(MyMathLib::CalculateNormal(mVertices[topRight], mVertices[bottomLeft], mVertices[bottomRight]));

            //qDebug() << "Made tri {" << topLeft << "," << topRight <<"," << bottomLeft << "} & {" << topRight << "," << bottomRight << "," << bottomLeft << "}";
            //qDebug() << "Has normals {" << MyMathLib::CalculateNormal(mVertices[topLeft], mVertices[topRight], mVertices[bottomLeft]) << "," <<MyMathLib::CalculateNormal(mVertices[topRight], mVertices[bottomRight], mVertices[bottomLeft]) << "}";

            squaresMade++;
        }
    }

    qDebug() << "Creation of Regular Triangulation Mesh is complete! Created " << squaresMade << " squares!";
}

///
/// @brief RegularTriangulation::FindBarycentric - Iterates through all the triangles in the mest to find which triangle the given location is at.
/// @param location - Location to find the barycentric coordinates for.
/// @return Barycentric coordinates and index of triangle - {index of triangle, u, v, w}
///
QVector4D RegularTriangulation::FindBarycentric(const QVector3D& location)
{
    QVector2D location2D = QVector2D(location.x(), location.z());
    QVector3D barycentric;
    QVector4D barycentricWithIndex;

    for (int tri = 0; (tri * 3) + 2 < mIndices.size(); tri++)
    {
        barycentric = MyMathLib::BarycentricCoordinates(mVertices[mIndices[tri*3]].getQVector2D(), mVertices[mIndices[(tri*3)+1]].getQVector2D(), mVertices[mIndices[(tri*3)+2]].getQVector2D(), location2D);

        if (barycentric.x() < 0 || barycentric.x() > 1 || barycentric.y() < 0 || barycentric.y() > 1 || barycentric.z() < 0 || barycentric.z() > 1) continue;

        QVector3D v1 = mVertices[mIndices[tri*3]].getQVector3D();
        QVector3D v2 = mVertices[mIndices[(tri*3)+1]].getQVector3D();
        QVector3D v3 = mVertices[mIndices[(tri*3)+2]].getQVector3D();

        return QVector4D(tri, barycentric.x(), barycentric.y(), barycentric.z());
    }

    return QVector4D(-1,0,0,0); // Default if the correct triangle could not be found
}

float RegularTriangulation::FindTriangleHeight(const int& triangleIndex, const QVector3D& position)
{
    if (triangleIndex < 0 || triangleIndex >= mIndices.size()) return -INFINITY;

    return MyMathLib::CalculateBarycentricHeight(mVertices[mIndices[(triangleIndex*3)]].getQVector3D(),
                                                     mVertices[mIndices[(triangleIndex*3)+1]].getQVector3D(),
                                                     mVertices[mIndices[(triangleIndex*3)+2]].getQVector3D(),
                                                     position);
}
