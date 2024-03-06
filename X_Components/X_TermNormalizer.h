//======================================================================
#ifndef X_TERMNORMALIX_ATOR_H
#define X_TERMNORMALIX_ATOR_H
//======================================================================
#include <QObject>
#include "X_CalibrationWindow.h"
#include "X_MathExpressionHandler.h"
//======================================================================
class X_AbstractSpectrum;
class X_Calibration;
class X_SpeSpectrum;
//======================================================================
class X_TermNormalizer : public QObject
{
    Q_OBJECT
public:

    enum NormaType {NT_NONE,
                    NT_COHERENT,
                    NT_INCOHERENT,
                    NT_COHERENT_INCOHERENT,
                    NT_INCOHERENT_COHERENT,
                    NT_CUSTOM
                   };

    static void xp_initStaticVariables();
    explicit X_TermNormalizer(X_Calibration *parent);

    // Static
    // static QStringList xp_normaTypeList();
    static QString xp_displayNormalizerTypeName(NormaType);
    static QString xp_normalizerTypeName(NormaType);
    static X_TermNormalizer::NormaType xp_normalizerTypeFromString(const QString&typestring);
    // End Static

    void xp_connectToWindow(X_CalibrationWindow* window);

    bool xp_normalizeValue(const X_AbstractSpectrum *spectrum, qreal &value);
    bool xp_normalizeValue(qreal &value);
    bool xp_calcAndSetNormaValue(const X_AbstractSpectrum *spectrum);
    void xp_resetNormaValue();

    X_TermNormalizer::NormaType xp_normaType() const;
    bool xp_setNormaType(NormaType);

    bool xp_setCustomNormaString(const QString &customString);
    QString xp_normaCustomString() const;
    bool xp_setNormalizerParameters(X_TermNormalizer::NormaType type,
                                  const QString& customString);
signals:

    void xg_normalizerChanged() const;
    void xg_requestCoherentIntensity(qint64&) const;
    void xg_requestIncoherentIntensity(qint64&) const;

    void xg_calcWindowIntensity(
        const QString& windowName, const QObject*, qreal&, bool keepBufferClean, bool* ok);
    void xg_errorReport(const QString& errorMsg) const;
    void xg_windowIsExist(const QString& name, bool& res) const;


public slots:



private slots:

    void xh_onWindowTypeChange(X_CalibrationWindow::WindowType previousType,
                               X_CalibrationWindow::WindowType currentType) const;
    void xh_onWindowDestroy(QObject* obj) const;
    void xh_onWindowMarginsChange() const;

    void xh_handleWindowIntensityRequest(const QString& varName,
                                         double& value,
                                         bool& bRes);
    void xh_handleErrorReport(const QString& errorString,
                              int errorTokenStartPosition,
                              int errorTokenEndPosition) const;

private:

    // VARS
    NormaType xv_normaType;
    X_Calibration* xv_calibration;
    QString xv_customNormaString;
    X_MathExpressionHandler xv_mathExpressionHandler;
    const X_SpeSpectrum* xv_spectrum = nullptr;

    // buffer
    qint64 xv_spectrumIdBuffer;
    qreal xv_normaValueBuffer;

    // FUNCS
    void xh_createConnections();
    bool xh_getWindowsValue(X_CalibrationWindow::WindowType type, const X_AbstractSpectrum* spectrum, qint64& value) const;
    bool xh_calcNormaValue(const X_AbstractSpectrum *spectrum, qreal& normaValue);


    // STATIC
    // VARS
    static QMap<X_TermNormalizer::NormaType, QPair<QString, QString>> xv_normaTypeStringMap;

    // FUNCS
    static QMap<X_TermNormalizer::NormaType, QPair<QString, QString>> xh_initNormaTypeStringMap();

};
//======================================================================
#endif // X_TERMNORMALIX_ATOR_H
