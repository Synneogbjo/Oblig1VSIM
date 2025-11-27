#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <string>
#include "../VisualObject.h"
#include <QVector3D>

class PointCloud : public VisualObject
{
public:
    PointCloud();
    PointCloud(const std::string& filename);

    int lineCount;
    QVector3D minPos;
    QVector3D maxPos;

    int getVertexCount();
};

#endif // POINTCLOUD_H
