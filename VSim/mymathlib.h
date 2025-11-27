#ifndef MYMATHLIB_H
#define MYMATHLIB_H

#include "QVector2D"

class MyMathLib
{
public:
    MyMathLib();

    static QVector3D KileProdukt(const QVector2D& u, const QVector2D& v);
};

#endif // MYMATHLIB_H
