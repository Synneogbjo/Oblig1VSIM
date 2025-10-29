#include "mymathlib.h"

MyMathLib::MyMathLib() {}

QVector3D MyMathLib::KileProdukt(const QVector2D& u, const QVector2D& v)
{
    return QVector3D(0,0,(u.x() * v.y()) - (u.y() * v.x()));
}
