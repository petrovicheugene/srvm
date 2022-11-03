//=============================================================
#ifndef X_ABSTRACTSPECTRUMIOHANDLER_H
#define X_ABSTRACTSPECTRUMIOHANDLER_H
//=============================================================
#include <QObject>
#include <QFile>
#include <QSharedPointer>
#include "X_AbstractSpectrum.h"
#include "X_SpectrumArray.h"
//=============================================================


//=============================================================
class X_AbstractSpectrumIOHandler : public QObject
{
    Q_OBJECT
public:
    explicit X_AbstractSpectrumIOHandler(QObject* spectrumParent, QObject *parent = 0);
    virtual ~X_AbstractSpectrumIOHandler();

    virtual bool xp_getSpectrumFromFile(const QString& path, QColor, X_AbstractSpectrum*&) const = 0;
    virtual bool xp_getSpectrumFromFile(QFile& file, QColor, X_AbstractSpectrum*&) const = 0;
    // virtual bool xp_getSpectrumFromFile(QFile& file, X_SpectrumArray&) const = 0;

signals:
    void xg_message(QString) const;

public slots:

protected:

    // VARS
    QObject* xv_spectrumParent;

};
//=============================================================
#endif // X_ABSTRACTSPECTRUMIOHANDLER_H
