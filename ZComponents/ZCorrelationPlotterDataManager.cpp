//=====================================================================
#include "ZCorrelationPlotterDataManager.h"
#include "ZPlotter.h"
#include "ZGeneral.h"
#include "ZGraphicsItemUserTypes.h"
#include "ZDefaultRectGraphicsItem.h"
#include "ZChartPointGraphicsItem.h"
#include "ZTermCorrelationTableWidget.h"

#include <QGraphicsView>
#include <QLabel>
#include <QMap>
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

//=====================================================================
ZCorrelationPlotterDataManager::ZCorrelationPlotterDataManager(QObject *parent) : QObject(parent)
{
    zv_plotter = 0;
    zv_chartDataKindComboBox = 0;
    zv_spectrumArrayRepository = 0;
    zv_calibrationRepository = 0;
    zv_defaultItem = 0;

    zv_currentArrayIndex = -1;
    zv_currentArrayId = -1;
    zv_currentCalibrationId = -1;
    zv_currentCalibrationIndex = -1;

    zv_concentrationRulerLabelBaseString = tr("Concentration");
    zv_termRulerLabelBaseString = tr("Term");
    zv_calibrationRulerLabelBaseString = tr("Calibration");
    zv_deviationRulerLabelBaseString = tr("Deviation %");

    zh_setUpChartPointOptions();
}
//=====================================================================
void ZCorrelationPlotterDataManager::zp_connectToPlotter(ZPlotter* plotter)
{
    zv_plotter = plotter;
    zv_plotter->zp_setAutoDefineVerticalAbsMax(false);
    zv_defaultItem = new ZDefaultRectGraphicsItem(zv_defaultSceneRect, true, true, true);
    zv_plotter->zp_addItem(zv_defaultItem);
    zv_plotter->zp_fitInBoundingRect();


    zv_plotter->zp_appendWidgetToDashboard(zh_createChartDataKindComboBoxWidget(),
                                           ZHorizontalDashBoard::AWP_LEFT_OF_BUTTONS,
                                           Qt::AlignLeft,
                                           10);

    //   QList<QAction*> actionList;
    //   actionList << zv_switchRuleMetrixAction;
    //   zv_plotter->zp_appendButtonsToDashboard(actionList, Qt::AlignLeft, 0);

    //   connect(zv_plotter, &ZPlotter::zg_cursorAreaImage,
    //           this, &ZPlotterDataManager::zh_findItemInCursorAreaImage);

    // rule settings // No label on right and top
    zv_plotter->zp_setLeftMarkRecalcFlag(false);
    zv_plotter->zp_setLeftRuleLabelVisible(true);

    zv_plotter->zp_setRightMarkRecalcFlag(false);
    zv_plotter->zp_setRightRuleLabelVisible(false);
    zv_plotter->zp_setRightRuleVisible(false);

    zv_plotter->zp_setTopMarkRecalcFlag(false);
    zv_plotter->zp_setTopRuleLabelVisible(false);
    zv_plotter->zp_setTopRuleVisible(false);

    zv_plotter->zp_setBottomMarkRecalcFlag(false);
    zv_plotter->zp_setBottomRuleLabelVisible(true);

    zv_plotter->zp_setDistortionSliderVisible(false);
}
//=====================================================================
void ZCorrelationPlotterDataManager::zp_connectToSpectrumArrayRepository(ZSpectrumArrayRepository* repository)
{
    zv_spectrumArrayRepository = repository;
    // array repository <-> array model
    connect(repository, &ZSpectrumArrayRepository::zg_spectrumOperation,
            this, &ZCorrelationPlotterDataManager::zh_onRepositoryArrayOperation);
    connect(repository, &ZSpectrumArrayRepository::zg_chemElementOperation,
            this, &ZCorrelationPlotterDataManager::zh_onRepositoryChemElementOperation);
    connect(repository, &ZSpectrumArrayRepository::zg_currentArrayIdChanged,
            this, &ZCorrelationPlotterDataManager::zh_currentSpectrumArrayChanged);
    connect(repository, &ZSpectrumArrayRepository::zg_currentSpectrumChanged,
            this, &ZCorrelationPlotterDataManager::zh_currentSpectrumChanged);

}
//=====================================================================
void ZCorrelationPlotterDataManager::zp_connectToCalibrationRepository(ZCalibrationRepository* repository)
{
    zv_calibrationRepository = repository;
    connect(repository, &ZCalibrationRepository::zg_calibrationOperation,
            this, &ZCorrelationPlotterDataManager::zh_onRepositoryCalibrationOperation);
    connect(repository, &ZCalibrationRepository::zg_termOperation,
            this, &ZCorrelationPlotterDataManager::zh_onRepositoryTermOperation);
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_currentCalibrationChanged,
            this, &ZCorrelationPlotterDataManager::zh_currentCalibrationChanged);
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_normalizerChanged,
            this, &ZCorrelationPlotterDataManager::zh_onNormalizerChange);

}
//=====================================================================
void ZCorrelationPlotterDataManager::zp_connectToTermCorrelationWidget(ZTermCorrelationTableWidget* termWidget)
{
    connect(this, &ZCorrelationPlotterDataManager::zg_requestCurrentTermIndex,
            termWidget, &ZTermCorrelationTableWidget::zp_currentTermIndex);
    connect(termWidget, &ZTermCorrelationTableWidget::zg_currentTermChanged,
            this, &ZCorrelationPlotterDataManager::zp_currentTermChanged);
}
//=====================================================================
void ZCorrelationPlotterDataManager::zp_setBottomRulerScaleMetrix(qreal scaleValue, const QString& rulerLabelString )
{





}
//=====================================================================
void ZCorrelationPlotterDataManager::zp_setCurrentSpectrum(qint64 id) const
{
    zv_spectrumArrayRepository->zp_setSpectrumCurrent(id);
}
//=====================================================================
bool ZCorrelationPlotterDataManager::zh_setRulerMetrixAndPrecisionToPlot(const ZChartPointOptions& options) const
{
    if(!zv_plotter)
    {
        return false;
    }


    zv_plotter->zp_setBottomMarkRecalcMetrix(options.zp_rulerLabelString(Qt::Horizontal),
                                             true, 0.0, options.zp_rulerScaleValue(Qt::Horizontal), 0.0);
    zv_plotter->zp_setBottomMarkPrecision(options.zp_rulerMarkPrecision(Qt::Horizontal));
    zv_plotter->zp_setBottomMarkRecalcFlag(true);

    zv_plotter->zp_setLeftMarkRecalcMetrix(options.zp_rulerLabelString(Qt::Vertical),
                                           true, 0.0, options.zp_rulerScaleValue(Qt::Vertical), 0.0);
    zv_plotter->zp_setLeftMarkPrecision(options.zp_rulerMarkPrecision(Qt::Vertical));
    zv_plotter->zp_setLeftMarkRecalcFlag(true);

    zv_plotter->zp_setLeftRuleLabel(options.zp_rulerLabelString(Qt::Vertical));
    return true;
}
//=====================================================================
QWidget* ZCorrelationPlotterDataManager::zh_createChartDataKindComboBoxWidget()
{
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout;
    widget->setLayout(layout);
    layout->setMargin(0);

    QLabel* label = new QLabel();
    label->setText(tr("Display:"));
    layout->addWidget(label);

    zv_chartDataKindComboBox = new QComboBox();
    label->setBuddy(zv_chartDataKindComboBox);
    connect(zv_chartDataKindComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(zh_onChartDataKindChange(int)));

    zv_chartDataKindComboBox->addItem(tr("Equation term"), CDK_TERM);
    zv_chartDataKindComboBox->addItem(tr("Calibration"), CDK_CALIBRATION);
    zv_chartDataKindComboBox->addItem(tr("Deviation"), CDK_DEVIATION);

    zv_chartDataKindComboBox->setCurrentIndex(0);

    layout->addWidget(zv_chartDataKindComboBox);

//    zv_rebuildPlotterButton = new QPushButton();
//    zv_rebuildPlotterButton->setText("Update");
//    connect(zv_rebuildPlotterButton, &QPushButton::clicked,
//            this, &ZCorrelationPlotterDataManager::zg_initPlotRebuild);
//    layout->addWidget(zv_rebuildPlotterButton);
    layout->addStretch();

    return widget;
}
//=====================================================================
void ZCorrelationPlotterDataManager::zh_setUpChartPointOptions()
{
    // concentration chart
    zv_calibrationChartPointOptions.zp_setPointType(ZChartPointOptions::PT_ROUND);
    zv_calibrationChartPointOptions.zp_setPointColor(QColor(Qt::cyan));
    zv_calibrationChartPointOptions.zp_setPointPixelSize(5);

    zv_calibrationChartPointOptions.zp_setRulerLabelString(Qt::Horizontal, zv_concentrationRulerLabelBaseString);
    zv_calibrationChartPointOptions.zp_setRulerLabelString(Qt::Vertical, zv_calibrationRulerLabelBaseString);

    zv_calibrationChartPointOptions.zp_setRulerScaleValue(Qt::Vertical, 0.01);
    zv_calibrationChartPointOptions.zp_setRulerScaleValue(Qt::Horizontal, 0.01);

    zv_calibrationChartPointOptions.zp_setRulerMarkPrecision(Qt::Vertical, 4);
    zv_calibrationChartPointOptions.zp_setRulerMarkPrecision(Qt::Horizontal, 4);

    // term correlation
    zv_termChartPointOptions.zp_setPointType(ZChartPointOptions::PT_ROUND);
    zv_termChartPointOptions.zp_setPointColor(QColor(Qt::green));
    zv_termChartPointOptions.zp_setPointPixelSize(5);

    zv_termChartPointOptions.zp_setRulerLabelString(Qt::Horizontal, zv_concentrationRulerLabelBaseString);
    zv_termChartPointOptions.zp_setRulerLabelString(Qt::Vertical, zv_termRulerLabelBaseString);

    zv_termChartPointOptions.zp_setRulerScaleValue(Qt::Vertical, 0.1);
    zv_termChartPointOptions.zp_setRulerScaleValue(Qt::Horizontal, 0.1);

    zv_termChartPointOptions.zp_setRulerMarkPrecision(Qt::Vertical, 4);
    zv_termChartPointOptions.zp_setRulerMarkPrecision(Qt::Horizontal, 4);

    // deviation

    zv_deviationChartPointOptions.zp_setPointType(ZChartPointOptions::PT_ROUND);
    zv_deviationChartPointOptions.zp_setPointColor(QColor(Qt::yellow));
    zv_deviationChartPointOptions.zp_setPointPixelSize(5);

    zv_deviationChartPointOptions.zp_setRulerLabelString(Qt::Horizontal, zv_concentrationRulerLabelBaseString);
    zv_deviationChartPointOptions.zp_setRulerLabelString(Qt::Vertical, zv_deviationRulerLabelBaseString);

    zv_deviationChartPointOptions.zp_setRulerScaleValue(Qt::Vertical, 0.001);
    zv_deviationChartPointOptions.zp_setRulerScaleValue(Qt::Horizontal, 0.001);

    zv_deviationChartPointOptions.zp_setRulerMarkPrecision(Qt::Vertical, 4);
    zv_deviationChartPointOptions.zp_setRulerMarkPrecision(Qt::Horizontal, 4);

}
//=====================================================================
void ZCorrelationPlotterDataManager::zh_rebuildChart()
{
    if(!zv_plotter || !zv_chartDataKindComboBox)
    {
        return;
    }

    // bool plotScaled = zv_plotter->zp_isPlotScaled();
    //    zv_plotter->zp_clearItemsExeptType(DefaultRectItemType);

    // chartPointItemList
    QList<QGraphicsItem*> chartPointItemList = zv_plotter->zp_itemListForType(ChartPointItemType);

    // get kind of chart
    QVariant vData = zv_chartDataKindComboBox->currentData();
    if(!vData.isValid() || vData.isNull() || !vData.canConvert<int>())
    {
        // TODO Error msg
        return;
    }

    ChartDataKind chartDataKind = static_cast<ChartDataKind>(vData.toInt());

    // getting chart data
    ZChartPointOptions* chartPointOptions = 0;
    QMap<qint64, ZVisibilityPointF> chartPointMap;
    qreal maxX = 0.0;
    qreal minX = 0.0;
    qreal maxY = 0.0;
    qreal minY = 0.0;

    if(chartDataKind == CDK_TERM)
    {
        zv_defaultItem->zp_setMedianaLineVisible(false);
        if(!zh_getTermToConcentrationData(chartPointMap, maxX, minX, maxY, minY))
        {
            // TODO ERROR msg
            // return;
        }
        zh_setRulerMetrixAndPrecisionToPlot(zv_termChartPointOptions);
         //zh_setRulerMetrixAndPrecisionToPlot(zv_deviationChartPointOptions);
        zh_recalcAndSetSceneRect(maxX, minX, maxY, minY, chartDataKind,
                                 &zv_termChartPointOptions);

        // assign chartPoinOptions
        chartPointOptions = &zv_termChartPointOptions;
    }
    else if(chartDataKind == CDK_CALIBRATION)
    {
        zv_defaultItem->zp_setMedianaLineVisible(true);
        if(!zh_getCalibrationToConcentrationData(chartPointMap, maxX, minX, maxY, minY))
        {
            // TODO ERROR msg
            // return;
        }
        zh_setRulerMetrixAndPrecisionToPlot(zv_calibrationChartPointOptions);
        zh_recalcAndSetSceneRect(maxX, minX, maxY, minY, chartDataKind,
                                 &zv_calibrationChartPointOptions);

        // assign chartPoinOptions
        chartPointOptions = &zv_calibrationChartPointOptions;
    }
    else if(chartDataKind == CDK_DEVIATION)
    {
        zv_defaultItem->zp_setMedianaLineVisible(false);
        if(!zh_getDeviationToConcentrationData(chartPointMap, maxX, minX, maxY, minY))
        {
            // TODO ERROR msg
            // return;
        }
        zh_setRulerMetrixAndPrecisionToPlot(zv_deviationChartPointOptions);
        zh_recalcAndSetSceneRect(maxX, minX, maxY, minY, chartDataKind,
                                 &zv_deviationChartPointOptions);

        // assign chartPoinOptions
        chartPointOptions = &zv_deviationChartPointOptions;
    }
    else
    {
        return;
    }

    QMap<qint64, ZVisibilityPointF>::const_iterator it;
    for(it = chartPointMap.begin(); it != chartPointMap.end(); it++)
    {
        ZChartPointGraphicsItem* pointItem = 0;
        bool itemExists = false;
        for(int i = 0; i < chartPointItemList.count(); i++ )
        {
            pointItem = qgraphicsitem_cast<ZChartPointGraphicsItem*>(chartPointItemList.value(i));
            if(!pointItem)
            {
                continue;
            }

            if(pointItem->zp_relatedObjectId() == it.key())
            {
                // remove pointer from list (in order to increse performance)
                chartPointItemList.removeAt(i);
                itemExists = true;
                break;
            }
        }

        // if item doesn't exist create it!
        // if item exists, change it's pos and visibility

        if(itemExists)
        {
            pointItem->zp_applyVisibilityAndPos(it.value(), chartPointOptions);
        }
        else
        {
            pointItem = new ZChartPointGraphicsItem(this, it.value(), chartPointOptions, it.key());
            zv_plotter->zp_addItem(pointItem);
        }
    }

    foreach(QGraphicsItem* item, chartPointItemList)
    {
        zv_plotter->zp_removeItem(item);
    }

    // if(!plotScaled)
    //{
        zv_plotter->zp_fitInBoundingRect();
        //        QMetaObject::invokeMethod(zv_plotter, "zp_fitInBoundingRect",
        //                                  Qt::QueuedConnection);

    //}

}
//=====================================================================
bool ZCorrelationPlotterDataManager::zh_getTermToConcentrationData(QMap<qint64, ZVisibilityPointF>& chartPointMap,
                                                                   qreal &maxX, qreal &minX, qreal &maxY, qreal& minY )
{
    if(!zv_calibrationRepository || !zv_spectrumArrayRepository || zv_currentArrayId < 0)
    {
        return false;
    }

    // check out hast current spectra array appropriate chem element
    QString calibrationChemElement = zv_calibrationRepository->zp_chemElementForCalibrationId(zv_currentCalibrationId);
    zv_termChartPointOptions.zp_setRulerLabelString(Qt::Horizontal, zv_concentrationRulerLabelBaseString + " " +calibrationChemElement);

    qint64 chemElementId = zv_spectrumArrayRepository->zp_chemElementIdForName(zv_currentArrayIndex, calibrationChemElement);

    // current term
    int currentTermIndex = -1;
    emit zg_requestCurrentTermIndex(currentTermIndex);

    QString termName = zv_calibrationRepository->zp_termName(zv_currentCalibrationId, currentTermIndex);
    zv_termChartPointOptions.zp_setRulerLabelString(Qt::Vertical, zv_termRulerLabelBaseString + " " +termName);

    //    bool zp_termVariablePart(qint64 calibrationId, int termIndex, const ZAbstractSpectrum *spectrum, qreal &value);
    const ZAbstractSpectrum* spectrum;
    qreal termValue;
    qreal concentration;
    bool ok;
    ZVisibilityPointF point;
    bool isDataOk;

    maxX = 0.0;
    minX = 0.0;
    maxY = 0.0;
    minY = 0.0;

    for(int s = 0; s < zv_spectrumArrayRepository->zp_spectrumCount(zv_currentArrayIndex); s++)
    {
        isDataOk = true;
        spectrum = zv_spectrumArrayRepository->zp_spectrum(zv_currentArrayIndex, s);
        // get term value for the spectrum
        if(!zv_calibrationRepository->zp_termVariablePart(zv_currentCalibrationId, currentTermIndex, spectrum, termValue))
        {
            isDataOk = false;
            termValue = 0.0;
        }

        concentration = spectrum->zp_concentrationString(chemElementId).toDouble(&ok);
        if(!ok)
        {
            isDataOk = false;
            concentration = 0.0;
        }

        if(maxX < concentration)
        {
            maxX = concentration;
        }

        if(minX > concentration)
        {
            minX = concentration;
        }

        if(maxY < termValue)
        {
            maxY = termValue;
        }

        if(minY > termValue)
        {
            minY = termValue;
        }

        point = ZVisibilityPointF(concentration, termValue, spectrum->zp_isSpectrumChecked() && isDataOk);
        chartPointMap.insert(spectrum->zp_spectrumId(), point);
    }

    return true;
}
//=====================================================================
bool ZCorrelationPlotterDataManager::zh_getCalibrationToConcentrationData(QMap<qint64, ZVisibilityPointF>& chartPointMap,
                                                                          qreal& maxX, qreal &minX, qreal& maxY, qreal& minY)
{
    if(!zv_calibrationRepository || !zv_spectrumArrayRepository || zv_currentArrayId < 0)
    {
        return false;
    }

    // check out has current spectra array appropriate chem element
    QString calibrationChemElement = zv_calibrationRepository->zp_chemElementForCalibrationId(zv_currentCalibrationId);
    zv_calibrationChartPointOptions.zp_setRulerLabelString(Qt::Horizontal, zv_concentrationRulerLabelBaseString + " " +calibrationChemElement);

    QString calibrationName = zv_calibrationRepository->zp_calibrationName(zv_currentCalibrationIndex);
    zv_calibrationChartPointOptions.zp_setRulerLabelString(Qt::Vertical, zv_calibrationRulerLabelBaseString + " " +calibrationName);

    qint64 chemElementId = zv_spectrumArrayRepository->zp_chemElementIdForName(zv_currentArrayIndex, calibrationChemElement);

    const ZAbstractSpectrum* spectrum;
    qreal calibrationValue;
    qreal concentration;
    bool ok;
    ZVisibilityPointF point;
    bool isDataOk;
    qreal maxConcentrationValue;

    maxX = 0.0;
    minX = 0.0;
    maxY = 0.0;
    minY = 0.0;
    for(int s = 0; s < zv_spectrumArrayRepository->zp_spectrumCount(zv_currentArrayIndex); s++)
    {
        isDataOk = true;
        spectrum = zv_spectrumArrayRepository->zp_spectrum(zv_currentArrayIndex, s);
        // get term value for the spectrum
        if(!zv_calibrationRepository->zp_calculateConcentrationForId(zv_currentCalibrationId, spectrum, calibrationValue))
        {
            isDataOk = false;
            calibrationValue = 0.0;
        }

        concentration = spectrum->zp_concentrationString(chemElementId).toDouble(&ok);
        if(!ok)
        {
            isDataOk = false;
            concentration = 0.0;
        }

        if(maxX < concentration)
        {
            maxX = concentration;
        }

        if(minX > concentration)
        {
            minX = concentration;
        }

        // in order to show green line
        maxConcentrationValue = qMax(calibrationValue, concentration);
        if(maxY < maxConcentrationValue)
        {
            maxY = maxConcentrationValue;
        }

        if(minY > calibrationValue)
        {
            minY = calibrationValue;
        }

        point = ZVisibilityPointF(concentration, calibrationValue, spectrum->zp_isSpectrumChecked() && isDataOk);
        chartPointMap.insert(spectrum->zp_spectrumId(), point);
    }

    if(qAbs(minY) > qAbs(maxY))
    {
        maxY = minY * -1;
    }

    //    if(maxY < maxX)
    //    {
    //        maxY = maxX;
    //    }

    return true;
}
//=====================================================================
bool ZCorrelationPlotterDataManager::zh_getDeviationToConcentrationData(QMap<qint64, ZVisibilityPointF> &chartPointMap,
                                                                        qreal &maxX, qreal &minX, qreal& maxY, qreal& minY )
{
    if(!zv_calibrationRepository || !zv_spectrumArrayRepository || zv_currentArrayId < 0)
    {
        return false;
    }

    // check out has current spectra array appropriate chem element
    QString calibrationChemElement = zv_calibrationRepository->zp_chemElementForCalibrationId(zv_currentCalibrationId);
    zv_calibrationChartPointOptions.zp_setRulerLabelString(Qt::Horizontal, zv_concentrationRulerLabelBaseString + " " +calibrationChemElement);

    QString calibrationName = zv_calibrationRepository->zp_calibrationName(zv_currentCalibrationIndex);
    zv_calibrationChartPointOptions.zp_setRulerLabelString(Qt::Vertical, zv_calibrationRulerLabelBaseString + " " +calibrationName);

    qint64 chemElementId = zv_spectrumArrayRepository->zp_chemElementIdForName(zv_currentArrayIndex, calibrationChemElement);

    const ZAbstractSpectrum* spectrum;
    qreal calibrationValue;
    qreal concentration;
    qreal deviationValue;
    bool ok;
    ZVisibilityPointF point;
    bool isDataOk;

    maxX = 0.0;
    minX = 0.0;
    maxY = 0.0;
    minY = 0.0;
    for(int s = 0; s < zv_spectrumArrayRepository->zp_spectrumCount(zv_currentArrayIndex); s++)
    {
        isDataOk = true;
        spectrum = zv_spectrumArrayRepository->zp_spectrum(zv_currentArrayIndex, s);
        // get term value for the spectrum
        if(!zv_calibrationRepository->zp_calculateConcentrationForId(zv_currentCalibrationId, spectrum, calibrationValue))
        {
            isDataOk = false;
            calibrationValue = 0.0;
            deviationValue = 0.0;
        }

        concentration = spectrum->zp_concentrationString(chemElementId).toDouble(&ok);
        if(!ok)
        {
            isDataOk = false;
            concentration = 0.0;
            deviationValue = 0.0;
        }

        if(concentration == 0.0)
        {
            deviationValue = 0.0;
        }
        else
        {
            deviationValue = ((calibrationValue - concentration) / concentration) * 100;
        }

//        if(deviationValue != 0.0)
//        {
//            deviationValue = ((calibrationValue - concentration) / concentration) * 100;
//        }

        if(maxX < concentration)
        {
            maxX = concentration;
        }

        if(minX > concentration)
        {
            minX = concentration;
        }

        if(maxY < deviationValue)
        {
            maxY = deviationValue;
        }

        if(minY > deviationValue)
        {
            minY = deviationValue;
        }

        point = ZVisibilityPointF(concentration, deviationValue, spectrum->zp_isSpectrumChecked() && isDataOk);
        chartPointMap.insert(spectrum->zp_spectrumId(), point);
    }

    if(qAbs(maxY) > qAbs(minY))
    {
        minY = maxY * -1;
    }
    else
    {
        maxY = minY * -1;
    }

    return true;
}
//=====================================================================
void ZCorrelationPlotterDataManager::zh_recalcAndSetSceneRect(const QMap<qint64, ZVisibilityPointF> &chartPointMap,
                                                              const ZChartPointOptions* chartPointOptions)
{
    if(!zv_plotter)
    {
        return;
    }

    if(!zv_defaultItem)
    {
        zv_plotter->zp_removeItemsForType(DefaultRectItemType);
        zv_defaultItem = new ZDefaultRectGraphicsItem(zv_defaultSceneRect, true, true, true);
        zv_plotter->zp_addItem(zv_defaultItem);
        zv_plotter->zp_fitInBoundingRect();
    }

    QList<ZVisibilityPointF> pointList = chartPointMap.values();

    qreal maxX = 0.0;
    qreal maxY = 0.0;
    qreal minX = 0.0;
    qreal minY = 0.0;

    bool visiblePointsPresented = false;
    foreach(ZVisibilityPointF point, pointList)
    {
        visiblePointsPresented = visiblePointsPresented || point.zp_isVisible();
        if(maxX < point.x())
        {
            maxX = point.x();
        }
        if(maxY < point.y())
        {
            maxY = point.y();
        }
        if(minX > point.x())
        {
            minX = point.x();
        }
        if(minY > point.y())
        {
            minY = point.y();
        }
    }

    if(visiblePointsPresented)
    {
        maxY /= chartPointOptions->zp_rulerScaleValue(Qt::Vertical);
        maxX /= chartPointOptions->zp_rulerScaleValue(Qt::Horizontal);
        minY /= chartPointOptions->zp_rulerScaleValue(Qt::Vertical);
        minX /= chartPointOptions->zp_rulerScaleValue(Qt::Horizontal);
    }
    else
    {
        maxY = 100.0;
        maxX = 100.0;
        minY = 0.0;
        minX = 0.0;
    }

    if(maxY == 0.0)
    {
        maxY = maxX;
    }

    qreal borderYWidth = qAbs(maxY - minY) * 0.03;
    qreal borderXWidth = qAbs(maxX - minX) * 0.03;

    maxY = maxY == 0.0? 0.0 : maxY * -1;
    minY = minY == 0.0? 0.0 : minY * -1;

    QRectF sceneRect = QRectF(minX, maxY, maxX, minY);
    sceneRect.adjust(-1 * borderXWidth, -1 * borderYWidth, borderXWidth, borderYWidth);
    zv_defaultItem->zp_fitItemInRect(sceneRect);
}
//=====================================================================
void ZCorrelationPlotterDataManager::zh_recalcAndSetSceneRect(qreal maxX, qreal minX,
                                                              qreal maxY, qreal minY, ChartDataKind chartDataKind,
                                                              const ZChartPointOptions* chartPointOptions)
{
    // increase rect if necessary
    if(qAbs(maxX - minX) == 0.0 && maxX == 0.0)
    {
        maxX = zv_defaultSceneRect.width();
        minX = 0.0;
    }

    if(qAbs(maxY - minY) == 0.0 && maxY == 0.0)
    {
        if(chartDataKind == CDK_DEVIATION)
        {
            qreal halfHeight = zv_defaultSceneRect.height() / 2;
            maxY = maxY + halfHeight;
            minY = minY - halfHeight;
        }
        else
        {
            if(chartDataKind == CDK_CALIBRATION)
            {
                maxY = maxX;
            }
            else
            {
                maxY = zv_defaultSceneRect.height();
            }
            minY = 0.0;
        }
    }

    // make one of rect corner as coordinate origin
    // x - axis
    if(maxX > 0.0 && minX > 0.0)
    {
        minX = 0.0;
    }
    else if(maxX < 0.0 && minX < 0.0)
    {
        maxX = 0.0;
    }

    // y - axis
    if(maxY > 0.0 && minY > 0.0)
    {
        minY = 0.0;
    }
    else if(maxY < 0.0 && minY < 0.0)
    {
        maxY = 0.0;
    }

    maxY /= chartPointOptions->zp_rulerScaleValue(Qt::Vertical);
    minY /= chartPointOptions->zp_rulerScaleValue(Qt::Vertical);
    maxX /= chartPointOptions->zp_rulerScaleValue(Qt::Horizontal);
    minX /= chartPointOptions->zp_rulerScaleValue(Qt::Horizontal);

    QRectF sceneRect;
    // sceneRect = QRectF(0.0, maxY * -1, maxX, maxY);
    // sceneRect = QRectF(0.0, maxY * -1, maxX, minY * -1);
    maxY = maxY == 0.0 ? 0.0 : maxY * -1;
    minY = minY == 0.0 ? 0.0 : minY * -1;
    sceneRect = QRectF(QPointF(minX, maxY), QPointF(maxX, minY));
    qreal borderYWidth = qAbs((maxY - minY) * 0.03);
    qreal borderXWidth = qAbs((maxX - minX) * 0.03);
    sceneRect.adjust(-1 * borderXWidth, -1 * borderYWidth, borderXWidth, borderYWidth);

    zv_defaultItem->zp_fitItemInRect(sceneRect.normalized());
}
//=====================================================================
void ZCorrelationPlotterDataManager::zh_createAndPlaceChartPointItems(const QMap<qint64,
                                                                      ZVisibilityPointF>& chartPointMap,
                                                                      ZChartPointOptions* chartPointOptions)
{
    ZChartPointGraphicsItem* pointItem;
    QMap<qint64, ZVisibilityPointF>::const_iterator it;
    for(it = chartPointMap.begin(); it != chartPointMap.end(); it++)
    {
        pointItem = new ZChartPointGraphicsItem(this, it.value(), chartPointOptions, it.key());
        zv_plotter->zp_addItem(pointItem);
    }
}
//=====================================================================
void ZCorrelationPlotterDataManager::zp_currentTermChanged(int , int)
{
    zh_rebuildChart();
}
//=====================================================================
void ZCorrelationPlotterDataManager::zh_currentSpectrumChanged(qint64 currentSpectrumId,
                                                               int currentSpectrumIndex,
                                                               qint64 previousSpectrumId,
                                                               int previousSpectrumIndex)
{
    ZChartPointGraphicsItem::zp_setCurrentSpectrumId(currentSpectrumId);

    QList<QGraphicsItem*> itemList = zv_plotter->zp_itemListForType(ChartPointItemType);
    ZChartPointGraphicsItem* spectrumItem;
    for(int i = 0; i < itemList.count(); i++)
    {
        spectrumItem = qgraphicsitem_cast<ZChartPointGraphicsItem*>(itemList.value(i));
        if(!spectrumItem)
        {
            continue;
        }
        spectrumItem->zp_updateCurrentItem();
    }
    zv_plotter->zp_updatePlot();
}
//=====================================================================
void ZCorrelationPlotterDataManager::zh_onChartDataKindChange(int)
{
    zh_rebuildChart();
}
//=====================================================================
void ZCorrelationPlotterDataManager::zh_currentSpectrumArrayChanged(qint64 currentArrayId, int currentArrayIndex)
{
    if(zv_currentArrayIndex == currentArrayIndex && zv_currentArrayId == currentArrayId)
    {
        return;
    }

    //emit zg_currentOperation(OT_BEGIN_RESET, -1, -1);
    zv_currentArrayIndex = currentArrayIndex;
    zv_currentArrayId = currentArrayId;
    zh_rebuildChart();
    //    zh_defineColumnCounts();
    //    zh_calculateCalibrationConcentrations();
    //    emit zg_currentOperation(OT_END_RESET, -1, -1);
}
//=====================================================================
void ZCorrelationPlotterDataManager::zh_onRepositoryArrayOperation(ZSpectrumArrayRepository::SpectrumOperationType type,
                                                                   int arrayIndex, int first, int last)
{
    if(zv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    zh_rebuildChart();
}
//=====================================================================
void ZCorrelationPlotterDataManager::zh_onRepositoryChemElementOperation(ZSpectrumArrayRepository::ChemElementOperationType type,
                                                                         int arrayIndex, int first, int last)
{
    if(zv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    zh_rebuildChart();
}
//=====================================================================
void ZCorrelationPlotterDataManager::zh_onRepositoryCalibrationOperation(ZCalibrationRepository::CalibrationOperationType type, int first, int last)
{
    if(zv_currentCalibrationIndex >= first && zv_currentCalibrationIndex <= last)
    {
        // get kind of chart
        QVariant vData = zv_chartDataKindComboBox->currentData();
        if(!vData.isValid() || vData.isNull() || !vData.canConvert<int>())
        {
            return;
        }

        ChartDataKind chartDataKind = static_cast<ChartDataKind>(vData.toInt());
        if(chartDataKind == CDK_CALIBRATION || chartDataKind == CDK_DEVIATION || chartDataKind == CDK_TERM)
        {
            if(type == ZCalibrationRepository::COT_CALIBRATION_NAME_CHANGED ||
                    type == ZCalibrationRepository::COT_CALIBRATION_CHEM_ELEMENT_CHANGED ||
                    type == ZCalibrationRepository::COT_CALIBRATION_FREE_MEMBER_CHANGED ||
                    type == ZCalibrationRepository::COT_CALIBRATION_EQUATION_TYPE_CHANGED ||
                    type == ZCalibrationRepository::COT_CALIBRATION_EQUATION_BASE_TERM_CHANGED ||
                    type == ZCalibrationRepository::COT_CALIBRATION_NORMA_CHANGED ||
                    type == ZCalibrationRepository::COT_END_REMOVE_CALIBRATIONS ||
                    type == ZCalibrationRepository::COT_END_INSERT_CALIBRATIONS)
            {
                zh_rebuildChart();
            }
        }
    }
}
//=====================================================================
void ZCorrelationPlotterDataManager::zh_onRepositoryTermOperation(ZCalibrationRepository::TermOperationType type, int calibrationIndex, int first, int last)
{
    if(calibrationIndex == zv_currentCalibrationIndex)
    {
        // get kind of chart
        QVariant vData = zv_chartDataKindComboBox->currentData();
        if(!vData.isValid() || vData.isNull() || !vData.canConvert<int>())
        {
            return;
        }

        ChartDataKind chartDataKind = static_cast<ChartDataKind>(vData.toInt());
        if(chartDataKind == CDK_CALIBRATION || chartDataKind == CDK_DEVIATION)
        {
            if(type == ZCalibrationRepository::TOT_TERM_STATE_CHANGED ||
                    type == ZCalibrationRepository::TOT_TERM_WINDOW_MARGIN_CHANGED ||
                    type == ZCalibrationRepository::TOT_TERM_FACTOR_CHANGED )
            {
                zh_rebuildChart();
            }
        }
        else if(chartDataKind == CDK_TERM)
        {
            if(type == ZCalibrationRepository::TOT_TERM_NAME_CHANGED ||
                    type == ZCalibrationRepository::TOT_TERM_WINDOW_MARGIN_CHANGED)
            {
                zh_rebuildChart();
            }
        }
    }
}
//=====================================================================
void ZCorrelationPlotterDataManager::zh_currentCalibrationChanged(qint64 calibrationId, int calibrationIndex)
{
    if(zv_currentCalibrationId == calibrationId && zv_currentCalibrationIndex == calibrationIndex)
    {
        return;
    }

    zv_currentCalibrationId = calibrationId;
    zv_currentCalibrationIndex = calibrationIndex;
    zh_rebuildChart();
}
//=====================================================================
void ZCorrelationPlotterDataManager::zh_onNormalizerChange(qint64 calibrationId)
{
    if(zv_currentCalibrationId == calibrationId)
    {
        zh_rebuildChart();
    }
}
//=====================================================================
