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
    explicit ZSpectrumArray(const QString&, QObject* parent);
    ~ZSpectrumArray();
    enum OperationType {OT_INSERT_SPECTRA,
                        OT_END_INSERT_SPECTRA,
                        OT_REMOVE_SPECTRA,
                        OT_END_REMOVE_SPECTRA,
                        OT_CHANGED};



    QString zp_arrayName() const;
    void zp_setArrayName(const QString&);
    ZRawSpectrumArray zp_createRawSpectrumArray();
    int zp_spectrumCount() const;

    int zp_chemElementCount() const;
    int zp_chemElementIndex(const QString& chemElement) const;
    int zp_numberVisibleChemElementsBeforeIndex(int index) const;
    int zp_visibleChemElementCount() const;
    QStringList zp_chemElementList() const;
    QString zp_visibleChemElementName(int visibleIndex) const;
    QString zp_chemElementName(int index) const;
    bool zp_chemElementIsVisible(int index) const;
    bool zp_setChemElementVisible(int row, bool visible);
    bool zp_setChemElementName(int row, const QString&);

    QString zp_chemConcentration(const QString& chemElement,
                                 int spectrumIndex) const;
    bool zp_setChemConcentration(const QString& chemElement,
                                 int spectrumIndex, const QString& concentration);
    bool zp_energyCalibration(qreal& K0, qreal& K1, qreal& K2, QString& energyUnit);

    QString zp_spectrumFileName(int index) const;
    qint64 zp_spectrumId(int index) const;
    QColor zp_spectrumColor(int index) const;
    QList<int> zp_spectrumData(int index) const;
    bool zp_isSpectrumVisible(int index) const;
    bool zp_setSpectrumVisible(int index, bool visible);

    bool zp_removeSpectrum(int);
    void zp_clearArray();
    void zp_clearChemElements();

    bool zp_appendSpectrum(const ZRawSpectrum& rawSpectrum, bool last, bool& dontAsk);
    bool zp_appendNewChemElement();
    bool zp_removeChemElement(int);

    const ZAbstractSpectrum* zp_spectrum(int index) const;
    QList<ZAbstractSpectrum*> zp_spectrumList() const;

    int zp_maxIntensity();
    int zp_maxChannelCount();
    bool zp_isEnergyCalibrationValid();

    qint64 zp_arrayId();

signals:

    void zg_message(QString) const;
    void zg_chemElementOperation(ZChemElementList::OperationType, int, int) const;
    void zg_spectrumOperation(ZSpectrumArray::OperationType, int first, int last) const;
    void zg_energyCalibrationChanged(qint64 arrayId);
    void zg_arrayMaxParametersChanged(qint64 arrayId, int intensity, int channels);

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

    // spectrum color
    int zv_lastColorIndex;
    static QList<QColor> zv_colorList;


    // FUNCS
    void zh_createConnections();
    void zh_recalcArrayMaxParameters();

    static QList<QColor> zp_createColorList();
    static bool checkColor(QColor color);


private slots:


};
//===============================================
#endif // ZSPECTRUMARRAY_H
