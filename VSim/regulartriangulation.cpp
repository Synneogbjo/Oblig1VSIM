#include "regulartriangulation.h"
#include "mymathlib.h"

RegularTriangulation::RegularTriangulation(const PointCloud& points, const float& resolution, const QVector2D& friction)
{
    drawType = 0;

    if (points.lineCount <= 0)
    {
        qDebug() << "Warning: Inserted PointCloud when initializing a new RegularTriangulation has a line count <= 0!";
        return;
    }

    // Logic for creating the Regular Triangulation Surface is inspired by chapter 11.4 of Dag's Lecture Notes. Citation is found in report.

    int gridWidth = floor((points.maxPos.x() - points.minPos.x()) / resolution) + 1;
    int gridHeight = floor((points.maxPos.z() - points.minPos.z()) / resolution) + 1;

    std::vector<QVector4D> squarePoints; //First three values are x, y, z. The last value is used to count the points from the PointCloud that are within this square.
    float halfResolution = resolution * 0.5f;

    //Loop creating all the vertices we want for the surface
    for (float z = points.minPos.z(); z <= points.maxPos.z(); z += resolution)
    {
        for (float x = points.minPos.x(); x <= points.maxPos.x(); x += resolution)
        {
            squarePoints.push_back({x, 0, z, 0});
            //qDebug() << "x: " << (x) << " z: " << (z);
        }
    }

    float resolutionInverted = 1.0f / resolution;
    int indexX = 0;
    int indexZ = 0;
    int index = -1;

    //Adds the height of each point in our PointCloud to the matching vertex in our surface mesh
    for (const Vertex& point : points.getVertices())
    {
        indexX = static_cast<int>((point.x - points.minPos.x()) * resolutionInverted);
        indexZ = static_cast<int>((point.z - points.minPos.z()) * resolutionInverted);

        if (indexX < 0 || indexX >= gridWidth || indexZ < 0 || indexZ >= gridHeight) continue;
        index = (indexZ * gridWidth) + indexX;

        squarePoints[index].setY(squarePoints[index].y() + point.y); //Adds point height to matching vertex in surface mesh
        squarePoints[index].setW(squarePoints[index].w() + 1); //Increments the amount of points sampled for this vertex to be used later
    }

    //Scales the color values to be between 0 and 1
    QVector3D colorScalar(1 / (points.maxPos.x() - points.minPos.x()), 1 / (points.maxPos.y() - points.minPos.y()), 1 / (points.maxPos.z() - points.minPos.z()));

    for (auto& point : squarePoints)
    {
        if (point.w() > 0) point.setY(point.y() / point.w()); //Divides the height of the vertex by the amount of points sampled to said vertex

        mVertices.push_back({point.x(), point.y(), point.z(),
                             point.x() * colorScalar.x(), point.y() * colorScalar.y(), point.z() * colorScalar.z(),
                             0, 0});
    }

    int squaresMade = 0;

    //Adds indices for each triangle in the surface mesh
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
            mFaceNormals.push_back(MyMathLib::CalculateNormal(mVertices[topLeft], mVertices[bottomLeft], mVertices[topRight])); //Lecture notes chapter 6.2.1
            mFaceFrictions.push_back(friction);

            mIndices.push_back(topRight);
            mIndices.push_back(bottomLeft);
            mIndices.push_back(bottomRight);
            mFaceNormals.push_back(MyMathLib::CalculateNormal(mVertices[topRight], mVertices[bottomLeft], mVertices[bottomRight])); //Lecture notes chapter 6.2.1
            mFaceFrictions.push_back(friction);

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

bool RegularTriangulation::RayIntersectsTriangle(const QVector3D& rayOrigin, const QVector3D& rayDir,const QVector3D& v1,const QVector3D& v2,const QVector3D& v3, float& outT, QVector3D& outHitPoint)
{
    //Uses the Möller-Trumbore algorithm to calculate raytrace collision with the surface.
    const float roundingError = 1e-6f;
    QVector3D edge1 = v2-v1;
    QVector3D edge2 = v3-v1;
    QVector3D h = QVector3D::crossProduct(rayDir, edge2);
    float determinant = QVector3D::dotProduct(edge1, h);

    if (determinant > -roundingError && determinant < roundingError)
    {
        return false; //Ray is parallel to triangle, and will not hit
    }

    //Calculates for first barycentric coordinate
    float f = 1.0f / determinant; // Used to scale the barycentric coordinates
    QVector3D s = rayOrigin - v1;
    float u = f * QVector3D::dotProduct(s,h);
    if (u < 0.0f || u > 1.0f)
    {
        return false; // Intersection is outside of the triangle
    }

    //Calculates for second barycentric coordinate
    QVector3D q = QVector3D::crossProduct(s, edge1);
    float v = f * QVector3D::dotProduct(rayDir, q);

    if (v < 0.0f || u + v > 1.0f)
    {
        return false;
    }

    //Calculates the distance along the ray where the intersection occurs
    float intersectDistance = f * QVector3D::dotProduct(edge2, q);

    if (intersectDistance > 0.0f) //Intersection point exists at rayOrigin + rayDirection * intersectDistance
    {
        outT = intersectDistance;
        outHitPoint = rayOrigin + rayDir * intersectDistance;

        return true;
    }
    return false;
}

QVector2D RegularTriangulation::GetTriangleFriction(const int& triangleIndex)
{
    if (triangleIndex < 0 || triangleIndex >= mFaceFrictions.size()) return {0.f,0.f};

    return mFaceFrictions[triangleIndex];
}

void RegularTriangulation::SetTriangleFriction(const int& triangleIndex, const QVector2D& friction)
{
    if (triangleIndex < 0 || triangleIndex >= mFaceFrictions.size()) return;

    mFaceFrictions[triangleIndex] = friction;
}
