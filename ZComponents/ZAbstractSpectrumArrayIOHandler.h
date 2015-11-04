//========================================================
#ifndef ZABSTRACTARRAYIOHANDLER_H
#define ZABSTRACTARRAYIOHANDLER_H
//========================================================
#include <QObject>
#include <QFile>
#include "ZRawSpectrumArray.h"
//========================================================
class ZAbstractSpectrumArrayIOHandler : public QObject
{
    Q_OBJECT
public:
    explicit ZAbstractSpectrumArrayIOHandler(QObject *parent = 0);
    virtual ~ZAbstractSpectrumArrayIOHandler();
    virtual bool zp_readSpectrumArray (QFile&, QList<ZRawSpectrumArray>&) const = 0;
    virtual bool zp_writeSpectrumArray (QFile&, const QList<ZRawSpectrumArray>&) const = 0;

signals:

    void zg_message(QString) const;

public slots:
};
//========================================================
#endif // ZABSTRACTARRAYIOHANDLER_H
