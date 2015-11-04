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
    explicit ZSpeIOHandler(QObject* spectrumParent, QObject* parent = 0);
    virtual ~ZSpeIOHandler();

    virtual bool zp_getSpectrumFromFile(const QString& path, ZAbstractSpectrum*&) const override;
    virtual bool zp_getSpectrumFromFile(QFile& file, ZAbstractSpectrum *&) const override;
    //virtual bool zp_getSpectrumFromFile(QFile& file, ZSpectrumArray&) const override;

signals:
    void zg_message(QString) const;
private:

    // VARS
    const int zv_intensityStartLine = 20;

    // FUNCS

};
//===========================================================
#endif // ZSPEIOHANDLER_H
