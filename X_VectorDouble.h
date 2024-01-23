#ifndef ZVECTORDOUBLE_H
#define ZVECTORDOUBLE_H
//-------------------------------------------------------------------------
#include <QMetaType>
#include <QVector>
#include <QPointF>

typedef QVector<double> ZVectorDouble;
Q_DECLARE_METATYPE(ZVectorDouble)

typedef QVector<QPointF> ZVectorPointF;
Q_DECLARE_METATYPE(ZVectorPointF)


#endif // ZVECTORDOUBLE_H
