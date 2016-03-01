//======================================================================
#ifndef ZTERMNORMALIZATOR_H
#define ZTERMNORMALIZATOR_H
//======================================================================
#include <QObject>
#include "ZCalibrationWindow.h"
//======================================================================
class ZAbstractSpectrum;
class ZCalibration;
//======================================================================
class ZTermNormalizer : public QObject
{
    Q_OBJECT
public:

    enum NormaType {NT_NONE = 0,
                    NT_COHERENT = 1,
                    NT_INCOHERENT = 2,
                    NT_COHERENT_INCOHERENT = 3,
                    NT_INCOHERENT_COHERENT = 4,
                    NT_CUSTOM = 5
                   };

    explicit ZTermNormalizer(ZCalibration *parent);

    // Static
    static QStringList zp_normaTypeList();
    static QString zp_normaTypeString(NormaType);
    static ZTermNormalizer::NormaType zp_normaTypeForString(const QString&);
    // End Static

    void zp_connectToWindow(ZCalibrationWindow* window);

    bool zp_isValid() const;
    qreal zp_value() const;
    bool zp_calcNormalizedValue(const ZAbstractSpectrum *spectrum, qreal &value);

    ZTermNormalizer::NormaType zp_normaType() const;
    bool zp_setNormaType(NormaType);

signals:

    void zg_normalizerChanged() const;
    void zg_requestCoherentIntensity(qint64&) const;
    void zg_requestIncoherentIntensity(qint64&) const;

public slots:

    void zp_isValid(bool&) const;
    void zp_value(qreal&) const;

private slots:

    void zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType,
                               ZCalibrationWindow::WindowType currentType) const;
    void zh_onWindowDestroy(QObject* obj) const;
    void zh_onWindowMarginsChange() const;

private:

    // VARS
    NormaType zv_normaType;
    ZCalibration* zv_calibration;
    static QMap<ZTermNormalizer::NormaType, QString> zv_normaTypeStringMap;

    // buffer
    qint64 zv_spectrumId;
    qreal zv_normaValue;

    // FUNCS
    void zh_createConnections();
    bool zh_getWindowsValue(ZCalibrationWindow::WindowType type, const ZAbstractSpectrum* spectrum, qint64& value);

    // STATIC
    static QMap<ZTermNormalizer::NormaType, QString> zh_initNormaTypeStringMap();

};
//======================================================================
#endif // ZTERMNORMALIZATOR_H
