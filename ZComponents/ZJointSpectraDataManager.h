//==================================================================
#ifndef ZSPECTRAJOINTDATAMANAGER_H
#define ZSPECTRAJOINTDATAMANAGER_H
//==================================================================
#include <QObject>
#include <QModelIndex>
#include <QMap>
#include <QList>
#include "ZSpectrumArrayRepository.h"
#include "ZCalibrationRepository.h"
#include "ZCalibrationQualityData.h"
//==================================================================
class ZJointSpectraDataManager : public QObject
{
    Q_OBJECT
public:
    explicit ZJointSpectraDataManager(QObject *parent = 0);

    enum OperationType {OT_BEGIN_RESET,
                        OT_END_RESET,
                        OT_BEGIN_INSERT_ROW,
                        OT_END_INSERT_ROW,
                        OT_BEGIN_REMOVE_ROW,
                        OT_END_REMOVE_ROW,
                        OT_BEGIN_INSERT_COLUMN,
                        OT_END_INSERT_COLUMN,
                        OT_BEGIN_REMOVE_COLUMN,
                        OT_END_REMOVE_COLUMN,
                        OT_COLUMN_HEADER_CHANGED,
                        OT_COLUMN_DATA_CHANGED,
                        OT_SEPECTRUM_VISIBLE_CHANGED};
    // FUNCS
    void zp_connectToSpectrumArrayRepository(ZSpectrumArrayRepository*);
    void zp_connectToCalibrationRepository(ZCalibrationRepository*);

    int zp_rowCount() const;
    int zp_columnCount() const;
    int zp_visibleChemElementCount() const;
    bool zp_isColumnChemElement(int column) const;
    QVariant zp_data(QModelIndex index) const;
    QString zp_columnName(int) const;
    QColor zp_spectrumColor(int) const;
    bool zp_isSpectrumVisible(int) const;
    bool zp_setSpectrumVisible(int, bool);
    bool zp_isSpectrumChecked(int) const;
    bool zp_setSpectrumChecked(int, bool);

    bool zp_setChemConcentration(int row, int column, const QString& consentration);
    int zp_spectrumDataColumnCount();
    bool zp_calculateConcentrationResidualList(qint64 calibrationId,
                                               QList<qreal>& residualDispersionList) const;

signals:

    void zg_currentOperation(OperationType, int, int) const;
    void zg_calibrationQualityData(qint64 calibrationId, ZCalibrationQualityData) const;
    void zg_calibrationValuesChanged(qint64 calibrationId);
    void zg_requestCurrentIndex(QModelIndex& index) const;

public slots:

    void zp_calculateCalibrationQualityData(qint64 calibrationId,
                                            int factorCount,
                                            qreal summSquareAverageConcentrationDispersion) const;
private slots:

    void zh_onRepositoryArrayOperation(ZSpectrumArrayRepository::SpectrumOperationType,
                                       int arrayIndex, int first, int last);

    void zh_onRepositoryChemElementOperation(ZSpectrumArrayRepository::ChemElementOperationType,
                                             int arrayIndex, int first, int last);
    void zh_onRepositoryTermOperation(ZCalibrationRepository::TermOperationType, int calibrationIndex, int first, int last);
    void zh_onRepositoryCalibrationOperation(ZCalibrationRepository::CalibrationOperationType, int, int);
    void zh_currentSpectrumArrayChanged(qint64, int);
    void zh_currentChemConcentrationCellIndex(int& row, int& chemConcColumn);

private:

    // VARS
    const int zv_spectrumDataColumnCount = 2;
    char zv_concentrationFormat;
    int zv_concentrationPrecision;

    int zv_currentArrayIndex;
    qint64 zv_currentArrayId;
    ZSpectrumArrayRepository* zv_spectrumArrayRepository;
    ZCalibrationRepository* zv_calibrationRepository;
    QString zv_currentChemElement;

    int zv_visibleChemElementCount;
    int zv_visibleCalibrationCount;
    QMap<qint64, QStringList> zv_calibrationConcentrationMap; // QMap<CalibName, QPair<specIndex, Concentration>>
   // QMap<qint64, QList<qreal> > zv_concentrationResidualMap; //

    // FUNCS
    bool zh_getVisibleIndexesForInsert(int first, int last, int& visibleFirst, int& visibleLast) const;
    bool zh_getVisibleIndexesForOperation(int first, int last, int& visibleFirst, int& visibleLast) const;

    int zh_convertChemElementIndexToVisibleChemElementIndex(int arrayIndex, int originalIndex);
    int zh_convertVisibleIndexToCalibrationIndex(int visibleIndex);
    void zh_defineColumnCounts();
    void zh_calculateCalibrationConcentrations();
    bool zh_calculateCalibrationConcentrationForCalibration(int calibrationIndex);
    bool zh_calculateCalibrationConcentrationForCalibrationId(qint64 calibrationId);

};
//==================================================================
#endif // ZSPECTRAJOINTDATAMANAGER_H
