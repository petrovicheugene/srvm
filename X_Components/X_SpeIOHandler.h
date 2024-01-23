//===========================================================
#ifndef X_SPEIOHANDLER_H
#define X_SPEIOHANDLER_H
//===========================================================
#include "X_AbstractSpectrumIOHandler.h"
#include "X_SpeSpectrum.h"

//===========================================================

//===========================================================
class X_SpeIOHandler : public X_AbstractSpectrumIOHandler
{
    Q_OBJECT
public:
    explicit X_SpeIOHandler(QObject* spectrumParent, QObject* parent = 0);
    virtual ~X_SpeIOHandler();

    bool xp_getSpectrumFromFile(const QString& path, QColor, X_AbstractSpectrum*&) const;
    bool xp_getSpectrumFromFile(QFile& file, QColor, X_AbstractSpectrum *&) const;
    //virtual bool xp_getSpectrumFromFile(QFile& file, X_SpectrumArray&) const override;

    bool xp_saveSpectrumToFile(const QString& path,
                               const QString& fileName,
                               X_SpeSpectrum *spectrum,
                               bool dontAsk = true);

    bool xp_saveSpectrumToFile(QFile& file, X_SpeSpectrum *spectrum);

signals:

    void xg_message(QString) const;

private:

    // VARS
    const int xv_intensityStartLine = 20;
    int xv_whatToDoAnswer;
    // FUNCS

};
//===========================================================
#endif // X_SPEIOHANDLER_H
