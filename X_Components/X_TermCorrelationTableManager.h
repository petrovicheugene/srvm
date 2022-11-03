//==================================================================
#ifndef X_TERMTABLEMANAGER_H
#define X_TERMTABLEMANAGER_H
//==================================================================
#include <QObject>
#include <QModelIndex>
#include <QVector2D>

#include "X_CalibrationRepository.h"
#include "X_SpectrumArrayRepository.h"
//==================================================================
class X_JointSpectraDataManager;
class QTextEdit;
//==================================================================
class X_TermCorrelationTableManager : public QObject
{
    Q_OBJECT
public:
    explicit X_TermCorrelationTableManager(QObject *parent = 0);

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
                        TOT_HORIX_ONTAL_HEADER_CHANGED,
                        TOT_VERTICAL_HEADER_CHANGED
                       };

    // FUNCS
    void xp_connectToCalibrationRepository(X_CalibrationRepository*);
    void xp_connectToSpectrumArrayRepository(X_SpectrumArrayRepository*);
    void xp_connectToJointSpectraDataManager(X_JointSpectraDataManager*jointSpectraDataManager);

    int xp_rowCount() const;
    int xp_columnCount() const;

    bool xp_isRowEditable(int row) const;
    QVariant xp_data(QModelIndex index) const;
    bool xp_setData(QModelIndex index, QVariant vFactor);
    QVariant xp_cellColor(QModelIndex index) const;
    QString xp_horizontalColumnName(int) const;
    QString xp_verticalColumnName(int) const;

    QPixmap xp_termStateIcon(int) const;

    void xp_setNextUsersTermState(int);

signals:

    void xg_message(QString);
    void xg_currentOperation(OperationType, int, int) const;
    void xg_calculateCalibrationQualityData(bool saveToCalibration,
                                            qint64 calibrationId,
                                            int factorCount,
                                            qreal sumSquareAverageConcentration) const;

    void xg_currentTermChanged(int currentTermIndex, int previousTermIndex) const;
    void xg_termDoubleClicked(int row);

public slots:

private slots:

    void zh_currentSpectrumArrayChanged(qint64, int);
    void zh_onRepositoryChemElementOperation(X_SpectrumArrayRepository::ChemElementOperationType type,
                                             int arrayIndex, int first, int last);
    void zh_currentCalibrationChanged(qint64 calibrationId, int calibrationIndex);
    void zh_onRepositoryTermOperation(X_CalibrationRepository::TermOperationType, int, int, int);
    void zh_onCalibrationRepositoryOperation(X_CalibrationRepository::CalibrationOperationType, int, int);
    void zh_onSpectrumOperation(X_SpectrumArrayRepository::SpectrumOperationType, int, int, int);
    void zh_onCalibrationNormalizerChange(qint64 calibrationId);
    void zh_recalcCalibrationFactors();
    void zh_onCalibrationValuesChanged(qint64 calibrationId);

private:

    // VARS
    X_CalibrationRepository* xv_calibrationRepository;
    X_SpectrumArrayRepository* xv_spectrumArrayRepository;
    X_JointSpectraDataManager* xv_jointSpectraDataManager;
    qint64 xv_currentCalibrationId;
    int xv_currentCalibrationIndex;
    qint64 xv_currentArrayId;
    int xv_currentArrayIndex;

    // average values
    qreal xv_averageEquationIntercept;
    QList<qreal> xv_averageTermValueList;

    // dispersion data
    QList<QList<qreal>> xv_termDispersionMatrix;
    QList<qreal> xv_concentrationDispersionList;
    QList<qreal> xv_interceptDispersionList;
    qreal xv_sumSquareAverageConcentrationDispersion;

    // correlations
    QList<QList<QString> > xv_termIntercorrelationMatrix;
    QStringList xv_concentrationCorrelationList;
    QStringList xv_residualCorrelationList;

    // covariations
    QList<QList<qreal> > xv_termCovariationMatrix;
    QList<qreal> xv_freeTermCovariationList;

    const int xv_firstNonTermColumnCount = 3;
    const QString xv_defaultChemElementString = tr("No Element");
    int xv_columnCountCorrector;

    QColor xv_greenCell;
    QColor xv_yellowCell;
    QColor xv_blueCell;
    QColor xv_redCell;
    QColor xv_cyanCell;
    QColor xv_magentaCell;


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
#endif // X_TERMTABLEMANAGER_H
