//==================================================================
#ifndef ZTERMTABLEMANAGER_H
#define ZTERMTABLEMANAGER_H
//==================================================================
#include <QObject>
#include <QModelIndex>
#include <QVector2D>

#include "ZCalibrationRepository.h"
#include "ZSpectrumArrayRepository.h"
//==================================================================
class ZJointSpectraDataManager;
//==================================================================
class ZTermCorrelationTableManager : public QObject
{
    Q_OBJECT
public:
    explicit ZTermCorrelationTableManager(QObject *parent = 0);

    enum OperationType {TOT_BEGIN_RESET,
                        TOT_END_RESET,
                        TOT_BEGIN_INSERT_ROW,
                        TOT_END_INSERT_ROW,
                        TOT_BEGIN_REMOVE_ROW,
                        TOT_END_REMOVE_ROW,
                        TOT_BEGIN_INSERT_COLUMN,
                        TOT_END_INSERT_COLUMN,
                        TOT_BEGIN_REMOVE_COLUMN,
                        TOT_END_REMOVE_COLUMN,
                        TOT_DATA_CHANGED,
                        TOT_HORIZONTAL_HEADER_CHANGED,
                        TOT_VERTICAL_HEADER_CHANGED
                       };

    // FUNCS
    void zp_connectToCalibrationRepository(ZCalibrationRepository*);
    void zp_connectToSpectrumArrayRepository(ZSpectrumArrayRepository*);
    void zp_connectToJointSpectraDataManager(ZJointSpectraDataManager*jointSpectraDataManager);

    int zp_rowCount() const;
    int zp_columnCount() const;

    bool zp_isRowEditable(int row) const;
    QVariant zp_data(QModelIndex index) const;
    bool zp_setData(QModelIndex index, QVariant vFactor);
    QVariant zp_cellColor(QModelIndex index) const;
    QString zp_horizontalColumnName(int) const;
    QString zp_verticalColumnName(int) const;

    QPixmap zp_termStateIcon(int) const;

    void zp_setNextUsersTermState(int);

signals:

    void zg_message(QString);
    void zg_currentOperation(OperationType, int, int) const;
    void zg_calculateCalibrationQualityData(bool saveToCalibration,
                                            qint64 calibrationId,
                                            int factorCount,
                                            qreal sumSquareAverageConcentration) const;

    void zg_currentTermChanged(int currentTermIndex, int previousTermIndex) const;
    void zg_termDoubleClicked(int row);

public slots:

private slots:

    void zh_currentSpectrumArrayChanged(qint64, int);
    void zh_onRepositoryChemElementOperation(ZSpectrumArrayRepository::ChemElementOperationType type,
                                             int arrayIndex, int first, int last);
    void zh_currentCalibrationChanged(qint64 calibrationId, int calibrationIndex);
    void zh_onRepositoryTermOperation(ZCalibrationRepository::TermOperationType, int, int, int);
    void zh_onCalibrationRepositoryOperation(ZCalibrationRepository::CalibrationOperationType, int, int);
    void zh_onSpectrumOperation(ZSpectrumArrayRepository::SpectrumOperationType, int, int, int);
    void zh_onCalibrationNormalizerChange(qint64 calibrationId);
    void zh_recalcCalibrationFactors();
    void zh_onCalibrationValuesChanged(qint64 calibrationId);

private:

    // VARS
    ZCalibrationRepository* zv_calibrationRepository;
    ZSpectrumArrayRepository* zv_spectrumArrayRepository;
    ZJointSpectraDataManager* zv_jointSpectraDataManager;
    qint64 zv_currentCalibrationId;
    int zv_currentCalibrationIndex;
    qint64 zv_currentArrayId;
    int zv_currentArrayIndex;

    // average values
    qreal zv_averageEquationIntercept;
    QList<qreal> zv_averageTermValueList;

    // dispersion data
    QList<QList<qreal>> zv_termDispersionMatrix;
    QList<qreal> zv_concentrationDispersionList;
    QList<qreal> zv_interceptDispersionList;
    qreal zv_sumSquareAverageConcentrationDispersion;

    // correlations
    QList<QList<QString> > zv_termIntercorrelationMatrix;
    QStringList zv_concentrationCorrelationList;
    QStringList zv_residualCorrelationList;

    // covariations
    QList<QList<qreal> > zv_termCovariationMatrix;
    QList<qreal> zv_freeTermCovariationList;

    const int zv_firstNonTermColumnCount = 3;
    const QString zv_defaultChemElementString = tr("No Element");
    int zv_columnCountCorrector;

    QColor zv_greenCell;
    QColor zv_yellowCell;
    QColor zv_blueCell;
    QColor zv_redCell;
    QColor zv_cyanCell;
    QColor zv_magentaCell;

    // FUNCS

    bool zh_convertColRowForInterCorrelationMatrix(int& row, int& col) const;
    bool zh_convertColRowForCovariationMatrix(int& row, int& col) const;

    void zh_startCalculationCorrelationsAndCovariations();
    void zh_calcTermDispersions();
    void zh_calcConcentrationAndFreeTermDispersions();
    void zh_calcIntercorrelationsAndCovariations();
    void zh_calcConcentrationCorrelationsAndCavariations();
    void zh_calcResidualTermCorrelation();

};
//==================================================================
#endif // ZTERMTABLEMANAGER_H
