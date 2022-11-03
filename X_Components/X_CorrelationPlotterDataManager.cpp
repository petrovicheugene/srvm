//=====================================================================
#include "X_CorrelationPlotterDataManager.h"
#include "X_Plotter.h"
#include "X_General.h"
#include "X_GraphicsItemUserTypes.h"
#include "X_DefaultRectGraphicsItem.h"
#include "X_ChartPointGraphicsItem.h"
#include "X_TermCorrelationTableWidget.h"

#include <QGraphicsView>
#include <QLabel>
#include <QMap>
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

//=====================================================================
X_CorrelationPlotterDataManager::X_CorrelationPlotterDataManager(QObject *parent) : QObject(parent)
{
    xv_plotter = 0;
    xv_chartDataKindComboBox = 0;
    xv_spectrumArrayRepository = 0;
    xv_calibrationRepository = 0;
    xv_defaultItem = 0;

    xv_currentArrayIndex = -1;
    xv_currentArrayId = -1;
    xv_currentCalibrationId = -1;
    xv_currentCalibrationIndex = -1;

    xv_concentrationRulerLabelBaseString = tr("Concentration");
    xv_termRulerLabelBaseString = tr("Term");
    xv_calibrationRulerLabelBaseString = tr("Calibration");
    xv_deviationRulerLabelBaseString = tr("Deviation %");

    zh_setUpChartPointOptions();
}
//=====================================================================
void X_CorrelationPlotterDataManager::xp_connectToPlotter(X_Plotter* plotter)
{
    xv_plotter = plotter;
    xv_plotter->xp_setAutoDefineVerticalAbsMax(false);
    xv_defaultItem = new X_DefaultRectGraphicsItem(xv_defaultSceneRect, true, true, true);
    xv_plotter->xp_addItem(xv_defaultItem);
    xv_plotter->xp_fitInBoundingRect();


    xv_plotter->xp_appendWidgetToDashboard(zh_createChartDataKindComboBoxWidget(),
                                           X_HorizontalDashBoard::AWP_LEFT_OF_BUTTONS,
                                           Qt::AlignLeft,
                                           10);

    //   QList<QAction*> actionList;
    //   actionList << xv_switchRuleMetrixAction;
    //   xv_plotter->xp_appendButtonsToDashboard(actionList, Qt::AlignLeft, 0);

    //   connect(xv_plotter, &X_Plotter::xg_cursorAreaImage,
    //           this, &X_PlotterDataManager::zh_findItemInCursorAreaImage);

    // rule settings // No label on right and top
    xv_plotter->xp_setLeftMarkRecalcFlag(false);
    xv_plotter->xp_setLeftRuleLabelVisible(true);

    xv_plotter->xp_setRightMarkRecalcFlag(false);
    xv_plotter->xp_setRightRuleLabelVisible(false);
    xv_plotter->xp_setRightRuleVisible(false);

    xv_plotter->xp_setTopMarkRecalcFlag(false);
    xv_plotter->xp_setTopRuleLabelVisible(false);
    xv_plotter->xp_setTopRuleVisible(false);

    xv_plotter->xp_setBottomMarkRecalcFlag(false);
    xv_plotter->xp_setBottomRuleLabelVisible(true);

    xv_plotter->xp_setDistortionSliderVisible(false);
}
//=====================================================================
void X_CorrelationPlotterDataManager::xp_connectToSpectrumArrayRepository(X_SpectrumArrayRepository* repository)
{
    xv_spectrumArrayRepository = repository;
    // array repository <-> array model
    connect(repository, &X_SpectrumArrayRepository::xg_spectrumOperation,
            this, &X_CorrelationPlotterDataManager::zh_onRepositoryArrayOperation);
    connect(repository, &X_SpectrumArrayRepository::xg_chemElementOperation,
            this, &X_CorrelationPlotterDataManager::zh_onRepositoryChemElementOperation);
    connect(repository, &X_SpectrumArrayRepository::xg_currentArrayIdChanged,
            this, &X_CorrelationPlotterDataManager::zh_currentSpectrumArrayChanged);
    connect(repository, &X_SpectrumArrayRepository::xg_currentSpectrumChanged,
            this, &X_CorrelationPlotterDataManager::zh_currentSpectrumChanged);

}
//=====================================================================
void X_CorrelationPlotterDataManager::xp_connectToCalibrationRepository(X_CalibrationRepository* repository)
{
    xv_calibrationRepository = repository;
    connect(repository, &X_CalibrationRepository::xg_calibrationOperation,
            this, &X_CorrelationPlotterDataManager::zh_onRepositoryCalibrationOperation);
    connect(repository, &X_CalibrationRepository::xg_termOperation,
            this, &X_CorrelationPlotterDataManager::zh_onRepositoryTermOperation);
    connect(xv_calibrationRepository, &X_CalibrationRepository::xg_currentCalibrationChanged,
            this, &X_CorrelationPlotterDataManager::zh_currentCalibrationChanged);
    connect(xv_calibrationRepository, &X_CalibrationRepository::xg_normalizerChanged,
            this, &X_CorrelationPlotterDataManager::zh_onNormalizerChange);

}
//=====================================================================
void X_CorrelationPlotterDataManager::xp_connectToTermCorrelationWidget(X_TermCorrelationTableWidget* termWidget)
{
    connect(this, &X_CorrelationPlotterDataManager::xg_requestCurrentTermIndex,
            termWidget, &X_TermCorrelationTableWidget::xp_currentTermIndex);
    connect(termWidget, &X_TermCorrelationTableWidget::xg_currentTermChanged,
            this, &X_CorrelationPlotterDataManager::xp_currentTermChanged);
}
//=====================================================================
void X_CorrelationPlotterDataManager::xp_setBottomRulerScaleMetrix(qreal scaleValue, const QString& rulerLabelString )
{





}
//=====================================================================
void X_CorrelationPlotterDataManager::xp_setCurrentSpectrum(qint64 id) const
{
    xv_spectrumArrayRepository->xp_setSpectrumCurrent(id);
}
//=====================================================================
bool X_CorrelationPlotterDataManager::zh_setRulerMetrixAndPrecisionToPlot(const X_ChartPointOptions& options) const
{
    if(!xv_plotter)
    {
        return false;
    }


    xv_plotter->xp_setBottomMarkRecalcMetrix(options.xp_rulerLabelString(Qt::Horizontal),
                                             true, 0.0, options.xp_rulerScaleValue(Qt::Horizontal), 0.0);
    xv_plotter->xp_setBottomMarkPrecision(options.xp_rulerMarkPrecision(Qt::Horizontal));
    xv_plotter->xp_setBottomMarkRecalcFlag(true);

    xv_plotter->xp_setLeftMarkRecalcMetrix(options.xp_rulerLabelString(Qt::Vertical),
                                           true, 0.0, options.xp_rulerScaleValue(Qt::Vertical), 0.0);
    xv_plotter->xp_setLeftMarkPrecision(options.xp_rulerMarkPrecision(Qt::Vertical));
    xv_plotter->xp_setLeftMarkRecalcFlag(true);

    xv_plotter->xp_setLeftRuleLabel(options.xp_rulerLabelString(Qt::Vertical));
    return true;
}
//=====================================================================
QWidget* X_CorrelationPlotterDataManager::zh_createChartDataKindComboBoxWidget()
{
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout;
    widget->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);

    QLabel* label = new QLabel();
    label->setText(tr("Display:"));
    layout->addWidget(label);

    xv_chartDataKindComboBox = new QComboBox();
    label->setBuddy(xv_chartDataKindComboBox);
    connect(xv_chartDataKindComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(zh_onChartDataKindChange(int)));

    xv_chartDataKindComboBox->addItem(tr("Equation term"), CDK_TERM);
    xv_chartDataKindComboBox->addItem(tr("Calibration"), CDK_CALIBRATION);
    xv_chartDataKindComboBox->addItem(tr("Deviation"), CDK_DEVIATION);

    xv_chartDataKindComboBox->setCurrentIndex(0);

    layout->addWidget(xv_chartDataKindComboBox);

//    xv_rebuildPlotterButton = new QPushButton();
//    xv_rebuildPlotterButton->setText("Update");
//    connect(xv_rebuildPlotterButton, &QPushButton::clicked,
//            this, &X_CorrelationPlotterDataManager::xg_initPlotRebuild);
//    layout->addWidget(xv_rebuildPlotterButton);
    layout->addStretch();

    return widget;
}
//=====================================================================
void X_CorrelationPlotterDataManager::zh_setUpChartPointOptions()
{
    // concentration chart
    xv_calibrationChartPointOptions.xp_setPointType(X_ChartPointOptions::PT_ROUND);
    xv_calibrationChartPointOptions.xp_setPointColor(QColor(Qt::cyan));
    xv_calibrationChartPointOptions.xp_setPointPixelSize(5);

    xv_calibrationChartPointOptions.xp_setRulerLabelString(Qt::Horizontal, xv_concentrationRulerLabelBaseString);
    xv_calibrationChartPointOptions.xp_setRulerLabelString(Qt::Vertical, xv_calibrationRulerLabelBaseString);

    xv_calibrationChartPointOptions.xp_setRulerScaleValue(Qt::Vertical, 0.01);
    xv_calibrationChartPointOptions.xp_setRulerScaleValue(Qt::Horizontal, 0.01);

    xv_calibrationChartPointOptions.xp_setRulerMarkPrecision(Qt::Vertical, 4);
    xv_calibrationChartPointOptions.xp_setRulerMarkPrecision(Qt::Horizontal, 4);

    // term correlation
    xv_termChartPointOptions.xp_setPointType(X_ChartPointOptions::PT_ROUND);
    xv_termChartPointOptions.xp_setPointColor(QColor(Qt::green));
    xv_termChartPointOptions.xp_setPointPixelSize(5);

    xv_termChartPointOptions.xp_setRulerLabelString(Qt::Horizontal, xv_concentrationRulerLabelBaseString);
    xv_termChartPointOptions.xp_setRulerLabelString(Qt::Vertical, xv_termRulerLabelBaseString);

    xv_termChartPointOptions.xp_setRulerScaleValue(Qt::Vertical, 0.1);
    xv_termChartPointOptions.xp_setRulerScaleValue(Qt::Horizontal, 0.1);

    xv_termChartPointOptions.xp_setRulerMarkPrecision(Qt::Vertical, 4);
    xv_termChartPointOptions.xp_setRulerMarkPrecision(Qt::Horizontal, 4);

    // deviation

    xv_deviationChartPointOptions.xp_setPointType(X_ChartPointOptions::PT_ROUND);
    xv_deviationChartPointOptions.xp_setPointColor(QColor(Qt::yellow));
    xv_deviationChartPointOptions.xp_setPointPixelSize(5);

    xv_deviationChartPointOptions.xp_setRulerLabelString(Qt::Horizontal, xv_concentrationRulerLabelBaseString);
    xv_deviationChartPointOptions.xp_setRulerLabelString(Qt::Vertical, xv_deviationRulerLabelBaseString);

    xv_deviationChartPointOptions.xp_setRulerScaleValue(Qt::Vertical, 0.001);
    xv_deviationChartPointOptions.xp_setRulerScaleValue(Qt::Horizontal, 0.001);

    xv_deviationChartPointOptions.xp_setRulerMarkPrecision(Qt::Vertical, 4);
    xv_deviationChartPointOptions.xp_setRulerMarkPrecision(Qt::Horizontal, 4);

}
//=====================================================================
void X_CorrelationPlotterDataManager::zh_rebuildChart()
{
    if(!xv_plotter || !xv_chartDataKindComboBox)
    {
        return;
    }

    // bool plotScaled = xv_plotter->xp_isPlotScaled();
    //    xv_plotter->xp_clearItemsExeptType(DefaultRectItemType);

    // chartPointItemList
    QList<QGraphicsItem*> chartPointItemList = xv_plotter->xp_itemListForType(ChartPointItemType);

    // get kind of chart
    QVariant vData = xv_chartDataKindComboBox->currentData();
    if(!vData.isValid() || vData.isNull() || !vData.canConvert<int>())
    {
        // TODO Error msg
        return;
    }

    ChartDataKind chartDataKind = static_cast<ChartDataKind>(vData.toInt());

    // getting chart data
    X_ChartPointOptions* chartPointOptions = 0;
    QMap<qint64, X_VisibilityPointF> chartPointMap;
    qreal maxX = 0.0;
    qreal minX = 0.0;
    qreal maxY = 0.0;
    qreal minY = 0.0;

    if(chartDataKind == CDK_TERM)
    {
        xv_defaultItem->xp_setMedianaLineVisible(false);
        if(!zh_getTermToConcentrationData(chartPointMap, maxX, minX, maxY, minY))
        {
            // TODO ERROR msg
            // return;
        }
        zh_setRulerMetrixAndPrecisionToPlot(xv_termChartPointOptions);
         //zh_setRulerMetrixAndPrecisionToPlot(xv_deviationChartPointOptions);
        zh_recalcAndSetSceneRect(maxX, minX, maxY, minY, chartDataKind,
                                 &xv_termChartPointOptions);

        // assign chartPoinOptions
        chartPointOptions = &xv_termChartPointOptions;
    }
    else if(chartDataKind == CDK_CALIBRATION)
    {
        xv_defaultItem->xp_setMedianaLineVisible(true);
        if(!zh_getCalibrationToConcentrationData(chartPointMap, maxX, minX, maxY, minY))
        {
            // TODO ERROR msg
            // return;
        }
        zh_setRulerMetrixAndPrecisionToPlot(xv_calibrationChartPointOptions);
        zh_recalcAndSetSceneRect(maxX, minX, maxY, minY, chartDataKind,
                                 &xv_calibrationChartPointOptions);

        // assign chartPoinOptions
        chartPointOptions = &xv_calibrationChartPointOptions;
    }
    else if(chartDataKind == CDK_DEVIATION)
    {
        xv_defaultItem->xp_setMedianaLineVisible(false);
        if(!zh_getDeviationToConcentrationData(chartPointMap, maxX, minX, maxY, minY))
        {
            // TODO ERROR msg
            // return;
        }
        zh_setRulerMetrixAndPrecisionToPlot(xv_deviationChartPointOptions);
        zh_recalcAndSetSceneRect(maxX, minX, maxY, minY, chartDataKind,
                                 &xv_deviationChartPointOptions);

        // assign chartPoinOptions
        chartPointOptions = &xv_deviationChartPointOptions;
    }
    else
    {
        return;
    }

    QMap<qint64, X_VisibilityPointF>::const_iterator it;
    for(it = chartPointMap.begin(); it != chartPointMap.end(); it++)
    {
        X_ChartPointGraphicsItem* pointItem = 0;
        bool itemExists = false;
        for(int i = 0; i < chartPointItemList.count(); i++ )
        {
            pointItem = qgraphicsitem_cast<X_ChartPointGraphicsItem*>(chartPointItemList.value(i));
            if(!pointItem)
            {
                continue;
            }

            if(pointItem->xp_relatedObjectId() == it.key())
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
            pointItem->xp_applyVisibilityAndPos(it.value(), chartPointOptions);
        }
        else
        {
            pointItem = new X_ChartPointGraphicsItem(this, it.value(), chartPointOptions, it.key());
            xv_plotter->xp_addItem(pointItem);
        }
    }

    foreach(QGraphicsItem* item, chartPointItemList)
    {
        xv_plotter->xp_removeItem(item);
    }

    // if(!plotScaled)
    //{
        xv_plotter->xp_fitInBoundingRect();
        //        QMetaObject::invokeMethod(xv_plotter, "xp_fitInBoundingRect",
        //                                  Qt::QueuedConnection);

    //}

}
//=====================================================================
bool X_CorrelationPlotterDataManager::zh_getTermToConcentrationData(QMap<qint64, X_VisibilityPointF>& chartPointMap,
                                                                   qreal &maxX, qreal &minX, qreal &maxY, qreal& minY )
{
    if(!xv_calibrationRepository || !xv_spectrumArrayRepository || xv_currentArrayId < 0)
    {
        return false;
    }

    // check out hast current spectra array appropriate chem element
    QString calibrationChemElement = xv_calibrationRepository->xp_chemElementForCalibrationId(xv_currentCalibrationId);
    xv_termChartPointOptions.xp_setRulerLabelString(Qt::Horizontal, xv_concentrationRulerLabelBaseString + " " +calibrationChemElement);

    qint64 chemElementId = xv_spectrumArrayRepository->xp_chemElementIdForName(xv_currentArrayIndex, calibrationChemElement);

    // current term
    int currentTermIndex = -1;
    emit xg_requestCurrentTermIndex(currentTermIndex);

    QString termName = xv_calibrationRepository->xp_termName(xv_currentCalibrationId, currentTermIndex);
    xv_termChartPointOptions.xp_setRulerLabelString(Qt::Vertical, xv_termRulerLabelBaseString + " " +termName);

    //    bool xp_termVariablePart(qint64 calibrationId, int termIndex, const X_AbstractSpectrum *spectrum, qreal &value);
    const X_AbstractSpectrum* spectrum;
    qreal termValue;
    qreal concentration;
    bool ok;
    X_VisibilityPointF point;
    bool isDataOk;

    maxX = 0.0;
    minX = 0.0;
    maxY = 0.0;
    minY = 0.0;

    for(int s = 0; s < xv_spectrumArrayRepository->xp_spectrumCount(xv_currentArrayIndex); s++)
    {
        isDataOk = true;
        spectrum = xv_spectrumArrayRepository->xp_spectrum(xv_currentArrayIndex, s);
        // get term value for the spectrum
        if(!xv_calibrationRepository->xp_termVariablePart(xv_currentCalibrationId, currentTermIndex, spectrum, termValue))
        {
            isDataOk = false;
            termValue = 0.0;
        }

        concentration = spectrum->xp_concentrationString(chemElementId).toDouble(&ok);
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

        point = X_VisibilityPointF(concentration, termValue, spectrum->xp_isSpectrumChecked() && isDataOk);
        chartPointMap.insert(spectrum->xp_spectrumId(), point);
    }

    return true;
}
//=====================================================================
bool X_CorrelationPlotterDataManager::zh_getCalibrationToConcentrationData(QMap<qint64, X_VisibilityPointF>& chartPointMap,
                                                                          qreal& maxX, qreal &minX, qreal& maxY, qreal& minY)
{
    if(!xv_calibrationRepository || !xv_spectrumArrayRepository || xv_currentArrayId < 0)
    {
        return false;
    }

    // check out has current spectra array appropriate chem element
    QString calibrationChemElement = xv_calibrationRepository->xp_chemElementForCalibrationId(xv_currentCalibrationId);
    xv_calibrationChartPointOptions.xp_setRulerLabelString(Qt::Horizontal, xv_concentrationRulerLabelBaseString + " " +calibrationChemElement);

    QString calibrationName = xv_calibrationRepository->xp_calibrationName(xv_currentCalibrationIndex);
    xv_calibrationChartPointOptions.xp_setRulerLabelString(Qt::Vertical, xv_calibrationRulerLabelBaseString + " " +calibrationName);

    qint64 chemElementId = xv_spectrumArrayRepository->xp_chemElementIdForName(xv_currentArrayIndex, calibrationChemElement);

    const X_AbstractSpectrum* spectrum;
    qreal calibrationValue;
    qreal concentration;
    bool ok;
    X_VisibilityPointF point;
    bool isDataOk;
    qreal maxConcentrationValue;

    maxX = 0.0;
    minX = 0.0;
    maxY = 0.0;
    minY = 0.0;
    for(int s = 0; s < xv_spectrumArrayRepository->xp_spectrumCount(xv_currentArrayIndex); s++)
    {
        isDataOk = true;
        spectrum = xv_spectrumArrayRepository->xp_spectrum(xv_currentArrayIndex, s);
        // get term value for the spectrum
        if(!xv_calibrationRepository->xp_calculateConcentrationForId(xv_currentCalibrationId, spectrum, calibrationValue))
        {
            isDataOk = false;
            calibrationValue = 0.0;
        }

        concentration = spectrum->xp_concentrationString(chemElementId).toDouble(&ok);
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

        point = X_VisibilityPointF(concentration, calibrationValue, spectrum->xp_isSpectrumChecked() && isDataOk);
        chartPointMap.insert(spectrum->xp_spectrumId(), point);
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
bool X_CorrelationPlotterDataManager::zh_getDeviationToConcentrationData(QMap<qint64, X_VisibilityPointF> &chartPointMap,
                                                                        qreal &maxX, qreal &minX, qreal& maxY, qreal& minY )
{
    if(!xv_calibrationRepository || !xv_spectrumArrayRepository || xv_currentArrayId < 0)
    {
        return false;
    }

    // check out has current spectra array appropriate chem element
    QString calibrationChemElement = xv_calibrationRepository->xp_chemElementForCalibrationId(xv_currentCalibrationId);
    xv_calibrationChartPointOptions.xp_setRulerLabelString(Qt::Horizontal, xv_concentrationRulerLabelBaseString + " " +calibrationChemElement);

    QString calibrationName = xv_calibrationRepository->xp_calibrationName(xv_currentCalibrationIndex);
    xv_calibrationChartPointOptions.xp_setRulerLabelString(Qt::Vertical, xv_calibrationRulerLabelBaseString + " " +calibrationName);

    qint64 chemElementId = xv_spectrumArrayRepository->xp_chemElementIdForName(xv_currentArrayIndex, calibrationChemElement);

    const X_AbstractSpectrum* spectrum;
    qreal calibrationValue;
    qreal concentration;
    qreal deviationValue;
    bool ok;
    X_VisibilityPointF point;
    bool isDataOk;

    maxX = 0.0;
    minX = 0.0;
    maxY = 0.0;
    minY = 0.0;
    for(int s = 0; s < xv_spectrumArrayRepository->xp_spectrumCount(xv_currentArrayIndex); s++)
    {
        isDataOk = true;
        spectrum = xv_spectrumArrayRepository->xp_spectrum(xv_currentArrayIndex, s);
        // get term value for the spectrum
        if(!xv_calibrationRepository->xp_calculateConcentrationForId(xv_currentCalibrationId, spectrum, calibrationValue))
        {
            isDataOk = false;
            calibrationValue = 0.0;
            deviationValue = 0.0;
        }

        concentration = spectrum->xp_concentrationString(chemElementId).toDouble(&ok);
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

        point = X_VisibilityPointF(concentration, deviationValue, spectrum->xp_isSpectrumChecked() && isDataOk);
        chartPointMap.insert(spectrum->xp_spectrumId(), point);
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
void X_CorrelationPlotterDataManager::zh_recalcAndSetSceneRect(const QMap<qint64, X_VisibilityPointF> &chartPointMap,
                                                              const X_ChartPointOptions* chartPointOptions)
{
    if(!xv_plotter)
    {
        return;
    }

    if(!xv_defaultItem)
    {
        xv_plotter->xp_removeItemsForType(DefaultRectItemType);
        xv_defaultItem = new X_DefaultRectGraphicsItem(xv_defaultSceneRect, true, true, true);
        xv_plotter->xp_addItem(xv_defaultItem);
        xv_plotter->xp_fitInBoundingRect();
    }

    QList<X_VisibilityPointF> pointList = chartPointMap.values();

    qreal maxX = 0.0;
    qreal maxY = 0.0;
    qreal minX = 0.0;
    qreal minY = 0.0;

    bool visiblePointsPresented = false;
    foreach(X_VisibilityPointF point, pointList)
    {
        visiblePointsPresented = visiblePointsPresented || point.xp_isVisible();
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
        maxY /= chartPointOptions->xp_rulerScaleValue(Qt::Vertical);
        maxX /= chartPointOptions->xp_rulerScaleValue(Qt::Horizontal);
        minY /= chartPointOptions->xp_rulerScaleValue(Qt::Vertical);
        minX /= chartPointOptions->xp_rulerScaleValue(Qt::Horizontal);
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
    xv_defaultItem->xp_fitItemInRect(sceneRect);
}
//=====================================================================
void X_CorrelationPlotterDataManager::zh_recalcAndSetSceneRect(qreal maxX, qreal minX,
                                                              qreal maxY, qreal minY, ChartDataKind chartDataKind,
                                                              const X_ChartPointOptions* chartPointOptions)
{
    // increase rect if necessary
    if(qAbs(maxX - minX) == 0.0 && maxX == 0.0)
    {
        maxX = xv_defaultSceneRect.width();
        minX = 0.0;
    }

    if(qAbs(maxY - minY) == 0.0 && maxY == 0.0)
    {
        if(chartDataKind == CDK_DEVIATION)
        {
            qreal halfHeight = xv_defaultSceneRect.height() / 2;
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
                maxY = xv_defaultSceneRect.height();
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

    maxY /= chartPointOptions->xp_rulerScaleValue(Qt::Vertical);
    minY /= chartPointOptions->xp_rulerScaleValue(Qt::Vertical);
    maxX /= chartPointOptions->xp_rulerScaleValue(Qt::Horizontal);
    minX /= chartPointOptions->xp_rulerScaleValue(Qt::Horizontal);

    QRectF sceneRect;
    // sceneRect = QRectF(0.0, maxY * -1, maxX, maxY);
    // sceneRect = QRectF(0.0, maxY * -1, maxX, minY * -1);
    maxY = maxY == 0.0 ? 0.0 : maxY * -1;
    minY = minY == 0.0 ? 0.0 : minY * -1;
    sceneRect = QRectF(QPointF(minX, maxY), QPointF(maxX, minY));
    qreal borderYWidth = qAbs((maxY - minY) * 0.03);
    qreal borderXWidth = qAbs((maxX - minX) * 0.03);
    sceneRect.adjust(-1 * borderXWidth, -1 * borderYWidth, borderXWidth, borderYWidth);

    xv_defaultItem->xp_fitItemInRect(sceneRect.normalized());
}
//=====================================================================
void X_CorrelationPlotterDataManager::zh_createAndPlaceChartPointItems(const QMap<qint64,
                                                                      X_VisibilityPointF>& chartPointMap,
                                                                      X_ChartPointOptions* chartPointOptions)
{
    X_ChartPointGraphicsItem* pointItem;
    QMap<qint64, X_VisibilityPointF>::const_iterator it;
    for(it = chartPointMap.begin(); it != chartPointMap.end(); it++)
    {
        pointItem = new X_ChartPointGraphicsItem(this, it.value(), chartPointOptions, it.key());
        xv_plotter->xp_addItem(pointItem);
    }
}
//=====================================================================
void X_CorrelationPlotterDataManager::xp_currentTermChanged(int , int)
{
    zh_rebuildChart();
}
//=====================================================================
void X_CorrelationPlotterDataManager::zh_currentSpectrumChanged(qint64 currentSpectrumId,
                                                               int currentSpectrumIndex,
                                                               qint64 previousSpectrumId,
                                                               int previousSpectrumIndex)
{
    X_ChartPointGraphicsItem::xp_setCurrentSpectrumId(currentSpectrumId);

    QList<QGraphicsItem*> itemList = xv_plotter->xp_itemListForType(ChartPointItemType);
    X_ChartPointGraphicsItem* spectrumItem;
    for(int i = 0; i < itemList.count(); i++)
    {
        spectrumItem = qgraphicsitem_cast<X_ChartPointGraphicsItem*>(itemList.value(i));
        if(!spectrumItem)
        {
            continue;
        }
        spectrumItem->xp_updateCurrentItem();
    }
    xv_plotter->xp_updatePlot();
}
//=====================================================================
void X_CorrelationPlotterDataManager::zh_onChartDataKindChange(int)
{
    zh_rebuildChart();
}
//=====================================================================
void X_CorrelationPlotterDataManager::zh_currentSpectrumArrayChanged(qint64 currentArrayId, int currentArrayIndex)
{
    if(xv_currentArrayIndex == currentArrayIndex && xv_currentArrayId == currentArrayId)
    {
        return;
    }

    //emit xg_currentOperation(OT_BEGIN_RESET, -1, -1);
    xv_currentArrayIndex = currentArrayIndex;
    xv_currentArrayId = currentArrayId;
    zh_rebuildChart();
    //    zh_defineColumnCounts();
    //    zh_calculateCalibrationConcentrations();
    //    emit xg_currentOperation(OT_END_RESET, -1, -1);
}
//=====================================================================
void X_CorrelationPlotterDataManager::zh_onRepositoryArrayOperation(X_SpectrumArrayRepository::SpectrumOperationType type,
                                                                   int arrayIndex, int first, int last)
{
    if(xv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    zh_rebuildChart();
}
//=====================================================================
void X_CorrelationPlotterDataManager::zh_onRepositoryChemElementOperation(X_SpectrumArrayRepository::ChemElementOperationType type,
                                                                         int arrayIndex, int first, int last)
{
    if(xv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    zh_rebuildChart();
}
//=====================================================================
void X_CorrelationPlotterDataManager::zh_onRepositoryCalibrationOperation(X_CalibrationRepository::CalibrationOperationType type, int first, int last)
{
    if(xv_currentCalibrationIndex >= first && xv_currentCalibrationIndex <= last)
    {
        // get kind of chart
        QVariant vData = xv_chartDataKindComboBox->currentData();
        if(!vData.isValid() || vData.isNull() || !vData.canConvert<int>())
        {
            return;
        }

        ChartDataKind chartDataKind = static_cast<ChartDataKind>(vData.toInt());
        if(chartDataKind == CDK_CALIBRATION || chartDataKind == CDK_DEVIATION || chartDataKind == CDK_TERM)
        {
            if(type == X_CalibrationRepository::COT_CALIBRATION_NAME_CHANGED ||
                    type == X_CalibrationRepository::COT_CALIBRATION_CHEM_ELEMENT_CHANGED ||
                    type == X_CalibrationRepository::COT_CALIBRATION_FREE_MEMBER_CHANGED ||
                    type == X_CalibrationRepository::COT_CALIBRATION_EQUATION_TYPE_CHANGED ||
                    type == X_CalibrationRepository::COT_CALIBRATION_EQUATION_BASE_TERM_CHANGED ||
                    type == X_CalibrationRepository::COT_CALIBRATION_NORMA_CHANGED ||
                    type == X_CalibrationRepository::COT_END_REMOVE_CALIBRATIONS ||
                    type == X_CalibrationRepository::COT_END_INSERT_CALIBRATIONS)
            {
                zh_rebuildChart();
            }
        }
    }
}
//=====================================================================
void X_CorrelationPlotterDataManager::zh_onRepositoryTermOperation(X_CalibrationRepository::TermOperationType type, int calibrationIndex, int first, int last)
{
    if(calibrationIndex == xv_currentCalibrationIndex)
    {
        // get kind of chart
        QVariant vData = xv_chartDataKindComboBox->currentData();
        if(!vData.isValid() || vData.isNull() || !vData.canConvert<int>())
        {
            return;
        }

        ChartDataKind chartDataKind = static_cast<ChartDataKind>(vData.toInt());
        if(chartDataKind == CDK_CALIBRATION || chartDataKind == CDK_DEVIATION)
        {
            if(type == X_CalibrationRepository::TOT_TERM_STATE_CHANGED ||
                    type == X_CalibrationRepository::TOT_TERM_WINDOW_MARGIN_CHANGED ||
                    type == X_CalibrationRepository::TOT_TERM_FACTOR_CHANGED )
            {
                zh_rebuildChart();
            }
        }
        else if(chartDataKind == CDK_TERM)
        {
            if(type == X_CalibrationRepository::TOT_TERM_NAME_CHANGED ||
                    type == X_CalibrationRepository::TOT_TERM_WINDOW_MARGIN_CHANGED)
            {
                zh_rebuildChart();
            }
        }
    }
}
//=====================================================================
void X_CorrelationPlotterDataManager::zh_currentCalibrationChanged(qint64 calibrationId, int calibrationIndex)
{
    if(xv_currentCalibrationId == calibrationId && xv_currentCalibrationIndex == calibrationIndex)
    {
        return;
    }

    xv_currentCalibrationId = calibrationId;
    xv_currentCalibrationIndex = calibrationIndex;
    zh_rebuildChart();
}
//=====================================================================
void X_CorrelationPlotterDataManager::zh_onNormalizerChange(qint64 calibrationId)
{
    if(xv_currentCalibrationId == calibrationId)
    {
        zh_rebuildChart();
    }
}
//=====================================================================
