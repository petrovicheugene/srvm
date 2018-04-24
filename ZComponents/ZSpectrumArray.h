//===============================================
#ifndef ZSPECTRUMARRAY_H
#define ZSPECTRUMARRAY_H
//===============================================
#include <QObject>
#include <QList>
#include <QString>
#include "ZAbstractSpectrum.h"
#include "ZRawSpectrumArray.h"
#include "ZChemElementList.h"

//===============================================
class ZSpectrumArray : public QObject
{
    Q_OBJECT
public:
    explicit ZSpectrumArray(QObject* parent = 0);
    explicit ZSpectrumArray(const QString&, QObject* parent);
    ~ZSpectrumArray();
    enum OperationType {OT_INSERT_SPECTRA,
                        OT_END_INSERT_SPECTRA,
                        OT_REMOVE_SPECTRA,
                        OT_END_REMOVE_SPECTRA,
                        OT_CHANGED,
                        OT_CONCENTRATION_CHANGED
                       };



    QString zp_arrayName() const;
    void zp_setArrayName(const QString&);
    ZRawSpectrumArray zp_createRawSpectrumArray();
    int zp_spectrumCount() const;

    int zp_chemElementCount() const;
    int zp_chemElementIndexForName(const QString& chemElement) const;
    int zp_chemElementIndexForId(qint64 chemElementId) const;
    int zp_numberVisibleChemElementsBeforeIndex(int index) const;
    int zp_visibleChemElementCount() const;
    QStringList zp_chemElementList() const;
    QString zp_visibleChemElementName(int visibleIndex) const;
    QString zp_chemElementName(int index) const;
    qint64 zp_visibleChemElementId(int visibleIndex) const;
    qint64 zp_chemElementId(int index) const;
    bool zp_containsChemElementId(qint64) const;

    bool zp_chemElementIsVisible(int index) const;
    bool zp_setChemElementVisible(int row, bool visible);
    bool zp_setChemElementName(int row, const QString&);

    QString zp_chemConcentration(qint64 chemElementId,
                                 int spectrumIndex) const;
    qreal zp_chemConcentrationValue(qint64 chemElementId,
                               int spectrumIndex) const;
    bool zp_setChemConcentration(qint64 chemElementId,
                                 int spectrumIndex, const QString& concentration);
    bool zp_energyCalibration(qreal& K0, qreal& K1, qreal& K2, QString& energyUnit);
    int zp_gainFactor() const;
    bool zp_setGainFactor(int gainFactor);
    int zp_exposition() const;

    QString zp_spectrumFileName(int index) const;
    qint64 zp_spectrumId(int index) const;
    QColor zp_spectrumColor(int index) const;
    QList<quint32> zp_spectrumData(int index) const;
    bool zp_isSpectrumVisible(int index) const;
    bool zp_setSpectrumVisible(int index, bool visible);

    bool zp_isSpectrumChecked(int index) const;
    bool zp_setSpectrumChecked(int index, bool checked);

    bool zp_removeSpectrum(int);
    void zp_clearArray();
    void zp_clearChemElements();

    bool zp_appendSpectrum(const ZRawSpectrum& rawSpectrum, bool last, bool& dontAsk);
    bool zp_appendNewChemElement(QString chemElement = QString());
    bool zp_removeChemElement(int);

    const ZAbstractSpectrum* zp_spectrum(int index) const;
    QList<ZAbstractSpectrum*> zp_spectrumList() const;

    int zp_maxIntensity() const;
    int zp_maxChannelCount() const;
    bool zp_isEnergyCalibrationValid() const;

    qint64 zp_arrayId() const;
//    bool zp_averageChemConcentrationForChemElementId(qint64 chemElementId, qreal&) const;
//    bool zp_averageChemConcentration(const QString&, qreal&) const;
//    bool zp_averageChemConcentration(int, qreal&) const;

signals:

    void zg_message(QString) const;
    void zg_chemElementOperation(ZChemElementList::OperationType, int, int) const;
    void zg_spectrumOperation(ZSpectrumArray::OperationType, int first, int last) const;
    void zg_energyCalibrationChanged(qint64 arrayId);
    void zg_arrayMaxParametersChanged(qint64 arrayId, int intensity, int channels);
    void zg_averageConcentrationChanged(qint64 chemElementId);

private:

    // VARS
    QString zv_arrayName;
    qint64 zv_arrayId;
    static qint64 zv_lastArrayId;
    QList<ZAbstractSpectrum*> zv_spectrumList;
    ZChemElementList zv_chemElementList;
    // max parameters
    int zv_maxArrayChannelCount;
    int zv_maxArrayIntensity;

    // energy calibration
    qreal zv_energyK0;
    qreal zv_energyK1;
    qreal zv_energyK2;
    QString zv_energyUnit;
    int zv_gainFactor;
    int zv_exposition;

    // spectrum color
    int zv_lastColorIndex;
    static QList<QColor> zv_colorList;


    // FUNCS
    void zh_createConnections();
    void zh_recalcArrayMaxParameters();
//    bool zh_calcAverageChemConcentration(qint64 chemElementId);
//    void zh_calcAverageChemConcentrations();

    static QList<QColor> zp_createColorList();
    static bool checkColor(QColor color);


private slots:


};
//===============================================
#endif // ZSPECTRUMARRAY_H
