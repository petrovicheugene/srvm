//===========================================================
#ifndef ZSPEIOHANDLER_H
#define ZSPEIOHANDLER_H
//===========================================================
#include "ZAbstractSpectrumIOHandler.h"
#include "ZAbstractSpectrum.h"
//===========================================================


//===========================================================
class ZSpeIOHandler : public ZAbstractSpectrumIOHandler
{
    Q_OBJECT
public:
    explicit ZSpeIOHandler(QObject* parent = 0);

    virtual bool zp_getSpectrumFromFile(const QString& path, ZAbstractSpectrum*&) const override;
    virtual bool zp_getSpectrumFromFile(QFile& file, ZAbstractSpectrum *&) const override;
    //virtual bool zp_getSpectrumFromFile(QFile& file, ZSpectrumArray&) const override;

signals:
    void zg_message(QString) const;
private:

    // FUNCS

};
//===========================================================
#endif // ZSPEIOHANDLER_H
