//==================================================================
#ifndef X_SPECTRAJOINTDATAMANAGER_H
#define X_SPECTRAJOINTDATAMANAGER_H
//==================================================================
#include <QObject>
#include <QModelIndex>
#include <QMap>
#include <QList>
#include <QItemSelection>
#include "X_SpectrumArrayRepository.h"
#include "X_CalibrationRepository.h"
#include "X_CalibrationQualityData.h"
//==================================================================
class X_JointSpectraDataManager : public QObject
{
    Q_OBJECT
    friend class X_JointSpectraModel;
public:
    explicit X_JointSpectraDataManager(QObject *parent = 0);

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
                        OT_SEPECTRUM_VISIBLE_CHANGED,
                        OT_SEPECTRUM_CONCENTRATION_CHANGED};
    // FUNCS
    void xp_connectToSpectrumArrayRepository(X_SpectrumArrayRepository*);
    void xp_connectToCalibrationRepository(X_CalibrationRepository*);

    int xp_rowCount() const;
    int xp_columnCount() const;
    int xp_visibleChemElementCount() const;
    bool xp_isColumnChemElement(int column) const;
    QVariant xp_data(QModelIndex index) const;
    QString xp_columnName(int) const;
    QColor xp_spectrumColor(int) const;
    bool xp_isSpectrumVisible(int) const;
    bool xp_setSpectrumVisible(int, bool);
    bool xp_isSpectrumChecked(int) const;
    bool xp_setSpectrumChecked(int, bool);

    bool xp_setChemConcentration(int row, int column, const QString& consentration);
    int xp_spectrumDataColumnCount();
    bool xp_calculateConcentrationResidualList(qint64 calibrationId,
                                               QList<qreal>& residualDispersionList) const;

signals:

    void xg_currentOperation(OperationType, int, int) const;
    void xg_calibrationQualityData(bool saveToCalibration, qint64 calibrationId, X_CalibrationQualityData) const;
    void xg_calibrationValuesChanged(qint64 calibrationId);
    void xg_requestCurrentIndex(QModelIndex& index) const;
    void xg_onSelectionChange(bool selectionEnabled, bool concentrationSelected, bool spectrumSelected) const;
    void xg_requestClearSelected() const;
    void xg_requestSelectedString(QString&) const;

public slots:

    void xp_calculateCalibrationQualityData(bool saveToCalibration, qint64 calibrationId,
                                            int factorCount,
                                            qreal summSquareAverageConcentrationDispersion) const;

private slots:

    void zh_onRepositoryArrayOperation(X_SpectrumArrayRepository::SpectrumOperationType,
                                       int arrayIndex, int first, int last);

    void zh_onRepositoryChemElementOperation(X_SpectrumArrayRepository::ChemElementOperationType,
                                             int arrayIndex, int first, int last);
    void zh_onRepositoryTermOperation(X_CalibrationRepository::TermOperationType, int calibrationIndex, int first, int last);
    void zh_onRepositoryCalibrationOperation(X_CalibrationRepository::CalibrationOperationType, int, int);
    void zh_currentSpectrumArrayChanged(qint64, int);
    void zh_currentChemConcentrationCellIndex(int& row, int& chemConcColumn);

private:

    // VARS
    const int xv_spectrumDataColumnCount = 2;
    char xv_concentrationFormat;
    int xv_concentrationPrecision;

    int xv_currentArrayIndex;
    qint64 xv_currentArrayId;
    X_SpectrumArrayRepository* xv_spectrumArrayRepository;
    X_CalibrationRepository* xv_calibrationRepository;
    QString xv_currentChemElement;

    int xv_visibleChemElementCount;
    int xv_visibleCalibrationCount;
    QMap<qint64, QStringList> xv_calibrationConcentrationMap; // QMap<CalibName, QPair<specIndex, Concentration>>
   // QMap<qint64, QList<qreal> > xv_concentrationResidualMap; //

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
#endif // X_SPECTRAJOINTDATAMANAGER_H
