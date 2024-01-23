//=========================================-----------------------------
#ifndef ZSERIALIZER_H
#define ZSERIALIZER_H
//=========================================-----------------------------
#include <QObject>
#include <QPointF>
#include <QVector>
#include <QVariant>
#include "ZMargins.h"
//=========================================-----------------------------


//=========================================-----------------------------
class ZSerializer //: public QObject
{
    //Q_OBJECT
public:
    //explicit ZSpectrSerializer(QObject *parent = 0);
    
    static QVector<QPointF>deserializeToZVectorPointF(const QVariant data);
    static ZSimpleMargins deserializeToZMargins(const QVariant data);
    static QList<int>deserializeToZListInt(const QVariant data);

    static QVariant serialize(const QVector<QPointF>& );
    static QVariant serialize(const ZSimpleMargins &margins);
    static QVariant serialize(const QList<int> &);

protected:

};
//=========================================-----------------------------
#endif // ZSERIALIZER_H
