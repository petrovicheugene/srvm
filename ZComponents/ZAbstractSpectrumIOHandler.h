//=============================================================
#ifndef ZABSTRACTSPECTRUMIOHANDLER_H
#define ZABSTRACTSPECTRUMIOHANDLER_H
//=============================================================
#include <QObject>
#include <QFile>
#include <QSharedPointer>
#include "ZAbstractSpectrum.h"
#include "ZSpectrumArray.h"
//=============================================================


//=============================================================
class ZAbstractSpectrumIOHandler : public QObject
{
    Q_OBJECT
public:
    explicit ZAbstractSpectrumIOHandler(QObject* spectrumParent, QObject *parent = 0);
    virtual bool zp_getSpectrumFromFile(const QString& path, ZAbstractSpectrum*&) const = 0;
    virtual bool zp_getSpectrumFromFile(QFile& file, ZAbstractSpectrum*&) const = 0;
    // virtual bool zp_getSpectrumFromFile(QFile& file, ZSpectrumArray&) const = 0;

signals:
    void zg_message(QString) const;

public slots:

protected:

    // VARS
    QObject* zv_spectrumParent;

};
//=============================================================
#endif // ZABSTRACTSPECTRUMIOHANDLER_H
