//=====================================================================
#ifndef ZCORRELATIONPLOTTERMANAGER_H
#define ZCORRELATIONPLOTTERMANAGER_H
//=====================================================================
#include <QObject>
#include <QAction>
#include "ZChartPointOptions.h"
#include "ZVisibilityPointF.h"
#include "ZCalibrationRepository.h"
#include "ZSpectrumArrayRepository.h"
//=====================================================================
class ZPlotter;
class ZDefaultRectGraphicsItem;
class ZTermCorrelationTableWidget;
class QComboBox;
//=====================================================================

//=====================================================================
class ZCorrelationPlotterDataManager : public QObject
{
    Q_OBJECT
public:
    explicit ZCorrelationPlotterDataManager(QObject *parent = 0);

    enum ChartDataKind {CDK_TERM,
                        CDK_CALIBRATION};

    // FUNCS
    void zp_connectToPlotter(ZPlotter* plotter);
    void zp_connectToSpectrumArrayRepository(ZSpectrumArrayRepository*);
    void zp_connectToCalibrationRepository(ZCalibrationRepository*);
    void zp_connectToTermCorrelationWidget(ZTermCorrelationTableWidget*);

    void zp_setBottomRulerScaleMetrix(qreal scaleValue, const QString& rulerLabelString);
    void zp_setCurrentSpectrum(qint64 id) const;
signals:

    void zg_requestCurrentTermIndex(int&) const;

public slots:

    void zp_currentTermChanged(int , int);

private slots:

    //    void zh_showTermCorrelationChart();
    //    void zh_showConcentrationChart();

    void zh_currentSpectrumChanged(qint64 currentSpectrumId, int currentSpectrumIndex,
                                   qint64 previousSpectrumId, int previousSpectrumIndex);

    void zh_onChartDataKindChange(int);
    void zh_currentSpectrumArrayChanged(qint64, int);
    void zh_onRepositoryArrayOperation(ZSpectrumArrayRepository::SpectrumOperationType type,
                                       int arrayIndex, int first, int last);
    void zh_onRepositoryChemElementOperation(ZSpectrumArrayRepository::ChemElementOperationType,
                                             int arrayIndex, int first, int last);

    void zh_onRepositoryCalibrationOperation(ZCalibrationRepository::CalibrationOperationType type, int first, int last);
    void zh_onRepositoryTermOperation(ZCalibrationRepository::TermOperationType type, int calibrationIndex, int first, int last);
    void zh_currentCalibrationChanged(qint64 calibrationId, int calibrationIndex);
    void zh_onNormalizerChange(qint64 calibrationId);

private:

    // VARS
    ZPlotter* zv_plotter;
    QComboBox* zv_chartDataKindComboBox;
    ZChartPointOptions zv_calibrationChartPointOptions;
    ZChartPointOptions zv_termChartPointOptions;

    QRectF zv_defaultSceneRect = QRectF(QPointF(0.0,-100.0), QSizeF(100.0, 100.0));
    ZDefaultRectGraphicsItem* zv_defaultItem;

    ZSpectrumArrayRepository* zv_spectrumArrayRepository;
    ZCalibrationRepository* zv_calibrationRepository;

    int zv_currentArrayIndex;
    qint64 zv_currentArrayId;
    int zv_currentCalibrationIndex;
    qint64 zv_currentCalibrationId;

    QString zv_concentrationRulerLabelBaseString;
    QString zv_termRulerLabelBaseString;
    QString zv_calibrationRulerLabelBaseString;

    // FUNCS
    QWidget* zh_createChartDataKindComboBoxWidget();
    void zh_setUpChartPointOptions();

    void zh_rebuildChart();
    bool zh_getTermToConcentrationData(QMap<qint64, ZVisibilityPointF> &chartPointMap, qreal& maxX, qreal& maxY);
    bool zh_getCalibrationToConcentrationData(QMap<qint64, ZVisibilityPointF> &chartPointMap, qreal &maxX, qreal& maxY);
    void zh_recalcAndSetSceneRect(const QMap<qint64, ZVisibilityPointF>& chartPointMap,
                                  const ZChartPointOptions* chartPointOptions);

    void zh_recalcAndSetSceneRect(qreal maxX, qreal maxY, const ZChartPointOptions* chartPointOptions);
    void zh_createAndPlaceChartPointItems(const QMap<qint64, ZVisibilityPointF> &chartPointMap,
                                          ZChartPointOptions* chartPointOptions);
    bool zh_setRulerMetrixAndPrecisionToPlot(const ZChartPointOptions &) const;

};
//=====================================================================
#endif // ZCORRELATIONPLOTTERMANAGER_H
