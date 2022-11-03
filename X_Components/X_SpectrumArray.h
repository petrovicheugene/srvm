//===============================================
#ifndef X_SPECTRUMARRAY_H
#define X_SPECTRUMARRAY_H
//===============================================
#include <QObject>
#include <QList>
#include <QString>
#include "X_AbstractSpectrum.h"
#include "X_RawSpectrumArray.h"
#include "X_ChemElementList.h"
//===============================================
class X_SpeSpectrum;
//===============================================
class X_SpectrumArray : public QObject
{
    Q_OBJECT
public:
    explicit X_SpectrumArray(QObject* parent = nullptr);
    explicit X_SpectrumArray(const QString&, QObject* parent);
    ~X_SpectrumArray();
    enum OperationType {OT_INSERT_SPECTRA,
                        OT_END_INSERT_SPECTRA,
                        OT_REMOVE_SPECTRA,
                        OT_END_REMOVE_SPECTRA,
                        OT_CHANGED,
                        OT_CONCENTRATION_CHANGED,
                        OT_SPECTRUM_ENERGY_CALIBRATION_CHANGED
                       };

    QString xp_arrayName() const;
    void xp_setArrayName(const QString&);
    X_RawSpectrumArray xp_createRawSpectrumArray();
    int xp_spectrumCount() const;

    int xp_chemElementCount() const;
    int xp_chemElementIndexForName(const QString& chemElement) const;
    int xp_chemElementIndexForId(qint64 chemElementId) const;
    int xp_numberVisibleChemElementsBeforeIndex(int index) const;
    int xp_visibleChemElementCount() const;
    QStringList xp_chemElementList() const;
    QString xp_visibleChemElementName(int visibleIndex) const;
    QString xp_chemElementName(int index) const;
    qint64 xp_visibleChemElementId(int visibleIndex) const;
    qint64 xp_chemElementId(int index) const;
    bool xp_containsChemElementId(qint64) const;

    bool xp_chemElementIsVisible(int index) const;
    bool xp_setChemElementVisible(int row, bool visible);
    bool xp_setChemElementName(int row, const QString&);

    QString xp_chemConcentration(qint64 chemElementId,
                                 int spectrumIndex) const;
    qreal xp_chemConcentrationValue(qint64 chemElementId,
                                    int spectrumIndex) const;
    bool xp_setChemConcentration(qint64 chemElementId,
                                 int spectrumIndex, const QString& concentration);
    bool xp_energyCalibration(int spectrumIndex,
                              QList<double> &energyCalibration,
                              QString& energyUnit) const;
    int xp_gainFactor() const;
    bool xp_setGainFactor(int gainFactor);
    int xp_exposition() const;

    QString xp_spectrumFileName(int index) const;
    qint64 xp_spectrumId(int index) const;
    QColor xp_spectrumColor(int index) const;
    QList<quint32> xp_spectrumData(int index) const;
    bool xp_isSpectrumVisible(int index) const;
    bool xp_setSpectrumVisible(int index, bool visible);

    bool xp_isSpectrumChecked(int index) const;
    bool xp_setSpectrumChecked(int index, bool checked);

    bool xp_removeSpectrum(int);
    void xp_clearArray();
    void xp_clearChemElements();

    bool xp_appendSpectrum(const X_RawSpectrum& rawSpectrum, bool last, bool& dontAsk, bool &continueAppend);
    bool xp_appendNewChemElement(QString chemElement = QString());
    bool xp_removeChemElement(int);

    const X_AbstractSpectrum* xp_spectrum(int index) const;
    QList<X_AbstractSpectrum*> xp_spectrumList() const;

    int xp_maxIntensity() const;
    int xp_maxChannelCount() const;
    bool xp_isEnergyCalibrationValid() const;

    qint64 xp_arrayId() const;
    //    bool xp_averageChemConcentrationForChemElementId(qint64 chemElementId, qreal&) const;
    //    bool xp_averageChemConcentration(const QString&, qreal&) const;
    //    bool xp_averageChemConcentration(int, qreal&) const;

signals:

    void xg_message(QString) const;
    void xg_chemElementOperation(X_ChemElementList::OperationType, int, int) const;
    void xg_spectrumOperation(X_SpectrumArray::OperationType, int first, int last) const;
    void xg_arrayMaxParametersChanged(qint64 arrayId, int intensity, int channels);
    void xg_averageConcentrationChanged(qint64 chemElementId);

private:

    // VARS
    QString xv_arrayName;
    qint64 xv_arrayId;
    static qint64 xv_lastArrayId;
    QList<X_AbstractSpectrum*> xv_spectrumList;
    X_ChemElementList xv_chemElementList;
    // max parameters
    int xv_maxArrayChannelCount;
    int xv_maxArrayIntensity;

    // energy calibration
    qreal xv_energyK0;
    qreal xv_energyK1;
    qreal xv_energyK2;
    QString xv_energyUnit;
    int xv_gainFactor;
    int xv_exposition;

    // spectrum color
    int xv_lastColorIndex;
    static QList<QColor> xv_colorList;


    // FUNCS
    void xh_createConnections();
    void xh_recalcArrayMaxParameters();

    void xh_saveSpectrumToFile(X_SpeSpectrum* spectrum) const;
    int xh_indexForSpectrum(X_SpeSpectrum* spectrum, bool *ok) const;

    //    bool xh_calcAverageChemConcentration(qint64 chemElementId);
    //    void xh_calcAverageChemConcentrations();

    static QList<QColor> xp_createColorList();
    static bool checkColor(QColor color);


private slots:

    void xh_onSpectrumEnergyCalibrationChange() const;
    void xh_onSpectrumGainFactorChange() const;

};
//===============================================
#endif // X_SPECTRUMARRAY_H
