//=========================================-----------------------------
#include "ZSerializer.h"
#include <QByteArray>
#include "ZExtraTypeDefines.h"
#include <QDebug>
#include <QDataStream>

//=========================================-----------------------------
//ZSpectrSerializer::ZSpectrSerializer(QObject *parent) :
//    QObject(parent)
//{
//}
//=========================================-----------------------------
QVector<QPointF> ZSerializer::deserializeToZVectorPointF(const QVariant vData)
{
    //return vData.value<ZVectorPointF>();

    QByteArray byteArray = vData.toByteArray();
    QVector<QPointF> vector;
    if(byteArray.count() > 0)
    {

        QDataStream dataStream(&byteArray, QIODevice::ReadOnly);
        dataStream.setVersion(QDataStream::Qt_4_8);
        dataStream >> vector;
    }
    return vector;

}

//=========================================-----------------------------
ZSimpleMargins ZSerializer::deserializeToZMargins(const QVariant vData)
{
    QByteArray byteArray = vData.toByteArray();
    ZSimpleMargins margins;
    if(byteArray.count() > 0)
    {

        QDataStream dataStream(&byteArray, QIODevice::ReadOnly);
        dataStream.setVersion(QDataStream::Qt_4_8);
        dataStream.readRawData((char*)(&margins), sizeof(ZSimpleMargins));
    }
    return margins;

}

//=========================================-----------------------------
QList<int> ZSerializer::deserializeToZListInt(const QVariant vData)
{
    QByteArray byteArray = vData.toByteArray();
    QList<int> list;
    if(byteArray.count() > 0)
    {

        QDataStream dataStream(&byteArray, QIODevice::ReadOnly);
        dataStream.setVersion(QDataStream::Qt_4_8);
        dataStream >> list;
    }
    return list;


}

//=========================================-----------------------------
QVariant ZSerializer::serialize(const QVector<QPointF>& vector)
{

    QByteArray byteArray;
    QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_4_8);

    dataStream << vector;
    return QVariant(byteArray);

}
//=========================================-----------------------------
QVariant ZSerializer::serialize(const ZSimpleMargins& margins)
{

    QByteArray byteArray;
    QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_4_8);

    dataStream.writeRawData((const char*)(&margins), sizeof(ZSimpleMargins));
    return QVariant(byteArray);

}
//=========================================-----------------------------
QVariant ZSerializer::serialize(const QList<int> & list)
{
    QByteArray byteArray;
    QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_4_8);

    dataStream << list;
    return QVariant(byteArray);

}
//=========================================-----------------------------
