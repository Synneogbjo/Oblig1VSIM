#include "PointCloud.h"

#include <fstream>
#include <sstream>
#include <QDebug>

PointCloud::PointCloud() : VisualObject()
{
    drawType = 2;
}

PointCloud::PointCloud(const std::string &filename)
{
    drawType = 2;

    std::ifstream inn(filename);
    if (!inn.is_open())
        return;
    // read input from math part of compulsory
    float minX = INFINITY, minY = INFINITY, minZ = INFINITY, maxX = -INFINITY, maxY = -INFINITY, maxZ = -INFINITY;
    Vertex v{0.f,0.f,0.f,0.7f,0.7f,0.9f,0.f,0.f};

    std::string line;
    float x, y, z;

    if (std::getline(inn, line)) {
        std::stringstream ss(line);
        ss >> lineCount;
    }

    // Read min position
    if (std::getline(inn, line)) {
        std::stringstream ss(line);
        ss >> x >> y >> z;
        minPos = QVector3D(x, z, y);
    }

    // Read max position
    if (std::getline(inn, line)) {
        std::stringstream ss(line);
        ss >> x >> y >> z;
        maxPos = QVector3D(x, z, y);
    }

    while (std::getline(inn, line))
    {
        std::stringstream ss(line);
        std::string token;

        // Read x
        if (std::getline(ss, token, ' ')) {
            v.x = std::stof(token);
            //v.r = v.x;
        }
        // Read z
        if (std::getline(ss, token, ' ')) {
            v.z = std::stof(token);
            //v.g = v.z;
        }
        // Read ys
        if (std::getline(ss, token, '\n')) {
            v.y = std::stof(token);
            //v.b = v.y;
        }

        mVertices.push_back(v);
    }
    inn.close();
}

int PointCloud::getVertexCount()
{
    return mVertices.size();
}
