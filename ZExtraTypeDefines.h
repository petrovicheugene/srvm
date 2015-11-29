#ifndef ZEXTRATYPEDEFINES_H
#define ZEXTRATYPEDEFINES_H
//=========================================--------------------
#include <QMetaType>
#include <QVector>
#include <QPointF>
#include <QList>

typedef QVector<double> ZVectorDouble;
Q_DECLARE_METATYPE(ZVectorDouble)

typedef QVector<QPointF> ZVectorPointF;
Q_DECLARE_METATYPE(ZVectorPointF)

typedef QList<int> ZListInt;
Q_DECLARE_METATYPE(ZListInt)

#endif // ZEXTRATYPEDEFINES_H
