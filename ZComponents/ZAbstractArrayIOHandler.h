//========================================================
#ifndef ZABSTRACTARRAYIOHANDLER_H
#define ZABSTRACTARRAYIOHANDLER_H
//========================================================
#include <QObject>
#include "ZSpectrumArray.h"
#include "QFile"
#include "ZRawArray.h"
//========================================================
class ZAbstractArrayIOHandler : public QObject
{
    Q_OBJECT
public:
    explicit ZAbstractArrayIOHandler(QObject *parent = 0);
    // virtual bool zp_readSpectrumArray(ZSpectrumArray& , QFile&) = 0;
    virtual bool zp_readSpectrumArray (QList<ZRawArray>&, QFile&) = 0;

signals:

    void zg_message(QString) const;

public slots:
};
//========================================================
#endif // ZABSTRACTARRAYIOHANDLER_H
