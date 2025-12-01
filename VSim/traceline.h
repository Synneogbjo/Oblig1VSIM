#ifndef TRACELINE_H
#define TRACELINE_H

#include <QVector3D>
#include "../VisualObject.h"

class TraceLine : public VisualObject
{
public:
    TraceLine();

    void AddVertex(QVector3D newPoint);
};

#endif // TRACELINE_H
