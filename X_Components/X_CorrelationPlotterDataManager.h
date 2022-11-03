//=====================================================================
#ifndef X_CORRELATIONPLOTTERMANAGER_H
#define X_CORRELATIONPLOTTERMANAGER_H
//=====================================================================
#include <QObject>
#include <QAction>
#include "X_ChartPointOptions.h"
#include "X_VisibilityPointF.h"
#include "X_CalibrationRepository.h"
#include "X_SpectrumArrayRepository.h"
//=====================================================================
class X_Plotter;
class X_DefaultRectGraphicsItem;
class X_TermCorrelationTableWidget;
class QComboBox;
class QPushButton;
//=====================================================================

//=====================================================================
class X_CorrelationPlotterDataManager : public QObject
{
    Q_OBJECT
public:
    explicit X_CorrelationPlotterDataManager(QObject *parent = 0);

    enum ChartDataKind {CDK_TERM,
                        CDK_CALIBRATION,
                        CDK_DEVIATION};

    // FUNCS
    void xp_connectToPlotter(X_Plotter* plotter);
    void xp_connectToSpectrumArrayRepository(X_SpectrumArrayRepository*);
    void xp_connectToCalibrationRepository(X_CalibrationRepository*);
    void xp_connectToTermCorrelationWidget(X_TermCorrelationTableWidget*);

    void xp_setBottomRulerScaleMetrix(qreal scaleValue, const QString& rulerLabelString);
    void xp_setCurrentSpectrum(qint64 id) const;
    void zh_rebuildChart();

signals:

    void xg_requestCurrentTermIndex(int&) const;
    void xg_initPlotRebuild();

public slots:

    void xp_currentTermChanged(int , int);

private slots:

    //    void zh_showTermCorrelationChart();
    //    void zh_showConcentrationChart();

    void zh_currentSpectrumChanged(qint64 currentSpectrumId, int currentSpectrumIndex,
                                   qint64 previousSpectrumId, int previousSpectrumIndex);

    void zh_onChartDataKindChange(int);
    void zh_currentSpectrumArrayChanged(qint64, int);
    void zh_onRepositoryArrayOperation(X_SpectrumArrayRepository::SpectrumOperationType type,
                                       int arrayIndex, int first, int last);
    void zh_onRepositoryChemElementOperation(X_SpectrumArrayRepository::ChemElementOperationType,
                                             int arrayIndex, int first, int last);

    void zh_onRepositoryCalibrationOperation(X_CalibrationRepository::CalibrationOperationType type, int first, int last);
    void zh_onRepositoryTermOperation(X_CalibrationRepository::TermOperationType type, int calibrationIndex, int first, int last);
    void zh_currentCalibrationChanged(qint64 calibrationId, int calibrationIndex);
    void zh_onNormalizerChange(qint64 calibrationId);


private:

    // VARS
    X_Plotter* xv_plotter;
    QComboBox* xv_chartDataKindComboBox;
    // QPushButton* xv_rebuildPlotterButton;
    X_ChartPointOptions xv_calibrationChartPointOptions;
    X_ChartPointOptions xv_termChartPointOptions;
    X_ChartPointOptions xv_deviationChartPointOptions;

    QRectF xv_defaultSceneRect = QRectF(QPointF(0.0,-100.0), QSizeF(100.0, 100.0));
    X_DefaultRectGraphicsItem* xv_defaultItem;

    X_SpectrumArrayRepository* xv_spectrumArrayRepository;
    X_CalibrationRepository* xv_calibrationRepository;

    int xv_currentArrayIndex;
    qint64 xv_currentArrayId;
    int xv_currentCalibrationIndex;
    qint64 xv_currentCalibrationId;

    QString xv_concentrationRulerLabelBaseString;
    QString xv_termRulerLabelBaseString;
    QString xv_calibrationRulerLabelBaseString;
    QString xv_deviationRulerLabelBaseString;

    // FUNCS
    QWidget* zh_createChartDataKindComboBoxWidget();
    void zh_setUpChartPointOptions();

    // void zh_rebuildChart();
    bool zh_getTermToConcentrationData(QMap<qint64, X_VisibilityPointF> &chartPointMap,
                                       qreal& maxX, qreal &minX, qreal& maxY, qreal &minY);
    bool zh_getCalibrationToConcentrationData(QMap<qint64, X_VisibilityPointF> &chartPointMap,
                                              qreal &maxX, qreal &minX, qreal& maxY, qreal &minY);
    bool zh_getDeviationToConcentrationData(QMap<qint64, X_VisibilityPointF> &chartPointMap,
                                            qreal &maxX, qreal &minX, qreal& maxY, qreal &minY);

    void zh_recalcAndSetSceneRect(const QMap<qint64, X_VisibilityPointF>& chartPointMap,
                                  const X_ChartPointOptions* chartPointOptions);

    void zh_recalcAndSetSceneRect(qreal maxX, qreal minX, qreal maxY, qreal minY, ChartDataKind chartDataKind,
                                  const X_ChartPointOptions* chartPointOptions);
    void zh_createAndPlaceChartPointItems(const QMap<qint64, X_VisibilityPointF> &chartPointMap,
                                          X_ChartPointOptions* chartPointOptions);
    bool zh_setRulerMetrixAndPrecisionToPlot(const X_ChartPointOptions &) const;

};
//=====================================================================
#endif // X_CORRELATIONPLOTTERMANAGER_H
