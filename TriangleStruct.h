#ifndef TRIANGLESTRUCT_H
#define TRIANGLESTRUCT_H

#include "QVector3D"

struct FTriangle
{
    bool isValid = false;
    QVector3D v1;
    QVector3D v2;
    QVector3D v3;
};

#endif // TRIANGLESTRUCT_H
