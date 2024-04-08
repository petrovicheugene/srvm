//===========================================================
#include "X_AbstractPlotterDataManager.h"
#include "X_Plotter.h"
#include "X_GraphicsItemUserTypes.h"
#include "X_SpectrumGraphicsItem.h"
#include "X_WindowGraphicsItem.h"
#include "X_DefaultRectGraphicsItem.h"
#include "X_RulerToolGraphicsItem.h"
#include "X_SpectrumGraphicsItem.h"
#include "X_EnergyLineGraphicsItem.h"

#include "X_General.h"
#include <math.h>
#include <QSet>
#include <math.h>

//===========================================================
X_AbstractPlotterDataManager::X_AbstractPlotterDataManager(QObject *parent) : QObject(parent)
{
    xv_spectrumArrayRepositiry = nullptr;
    xv_calibrationRepository = nullptr;
    xv_plotter = nullptr;
    xv_currentArrayIndex = -1;
    xv_switchRuleMetrixAction = nullptr;
    xv_currentArrayId = -1;
    xv_boundingRectTopFactor = 1.05;
    xv_defaultItem = nullptr;
    xv_currentCalibrationId = -1;
    xv_currentCalibrationIndex = -1;

    xv_verticalRuleLabel = tr("Intensity");
    xv_horizontalRuleLabel = tr("Channel");
    xv_horizontalRecalcedRuleLabel = tr("Energy");

    xh_createComponents();
    xh_createConnections();
}
//===========================================================
void X_AbstractPlotterDataManager::xp_connectToSpectraArrayRepository(X_SpectrumArrayRepository* repository)
{
    xv_spectrumArrayRepositiry = repository;
    // array repository <->  model
    connect(repository, &X_SpectrumArrayRepository::xg_spectrumOperation,
            this, &X_AbstractPlotterDataManager::xh_onRepositoryArrayOperation);
//    connect(repository, &X_SpectrumArrayRepository::xg_energyCalibrationChanged,
//            this, &X_PlotterDataManager::xh_updateEnergyCalibrationOnRule);
    connect(repository, &X_SpectrumArrayRepository::xg_arrayMaxParametersChanged,
            this, &X_AbstractPlotterDataManager::xh_onArrayMaxParametersChanged);
    connect(repository, &X_SpectrumArrayRepository::xg_requestIsPlotScaled,
            this, &X_AbstractPlotterDataManager::xh_definePlotScaling);
    connect(repository, &X_SpectrumArrayRepository::xg_currentArrayIdChanged,
            this, &X_AbstractPlotterDataManager::xp_currentArrayChanged);
    connect(repository, &X_SpectrumArrayRepository::xg_currentSpectrumChanged,
            this, &X_AbstractPlotterDataManager::xp_currentSpectrumChanged);
    connect(repository, &X_SpectrumArrayRepository::xg_energyCalibrationChanged,
            this, &X_AbstractPlotterDataManager::xh_onCurrentEnergyCalibrationChange);


}
//===========================================================
void X_AbstractPlotterDataManager::xp_connectToCalibrationRepository(X_CalibrationRepository* repository)
{
    xv_calibrationRepository = repository;
    connect(repository, &X_CalibrationRepository::xg_calibrationWindowOperation,
            this, &X_AbstractPlotterDataManager::xh_onRepositoryCalibrationWindowOperation);
    connect(repository, &X_CalibrationRepository::xg_currentCalibrationChanged,
            this, &X_AbstractPlotterDataManager::xp_currentCalibrationChanged);
    connect(repository, &X_CalibrationRepository::xg_currentCalibrationWindowChanged,
            this, &X_AbstractPlotterDataManager::xp_currentCalibrationWindowChanged);
}
//===========================================================
void X_AbstractPlotterDataManager::xp_connectToPlotter(X_Plotter* plotter)
{
    xv_plotter = plotter;
    xv_plotter->xp_setAutoDefineVerticalAbsMax(false);
    QList<QAction*> actionList;
    actionList << xv_switchRuleMetrixAction;
    xv_plotter->xp_appendButtonsToDashboard(actionList,
                                            X_HorizontalDashBoard::AWP_LEFT_OF_BUTTONS,
                                            Qt::AlignLeft, 0);

    connect(xv_plotter, &X_Plotter::xg_cursorAreaImage,
            this, &X_AbstractPlotterDataManager::xh_findItemInCursorAreaImage);

    // rule settings // No label on right and top
    xv_plotter->xp_setLeftMarkRecalcFlag(false);
    xv_plotter->xp_setLeftRuleLabel(xv_verticalRuleLabel);
    xv_plotter->xp_setLeftRuleLabelVisible(true);

    xv_plotter->xp_setRightMarkRecalcFlag(false);
    xv_plotter->xp_setRightRuleLabelVisible(false);

    xv_plotter->xp_setTopMarkRecalcFlag(false);
    xv_plotter->xp_setTopRuleLabelVisible(false);

    xv_plotter->xp_setBottomMarkRecalcFlag(false);
    xv_plotter->xp_setBottomRuleLabel(xv_horizontalRuleLabel);
    xv_plotter->xp_setBottomRuleLabelVisible(true);

    xh_setPlotterVerticalAbsMax(100.0);

    if(xv_plotter->xp_itemCount() <= 0)
    {
        xv_defaultItem = new X_DefaultRectGraphicsItem(xv_defaultSceneRect, false, false, false);
        xv_plotter->xp_addItem(xv_defaultItem);
    }

    connect(xv_plotter, &X_Plotter::xg_viewportRectChanged,
            this, &X_AbstractPlotterDataManager::xp_onPlotterViewPortRectChange);
    connect(xv_plotter, &X_Plotter::xg_rulerToolChanged,
            this, &X_AbstractPlotterDataManager::xh_updateRulerTool);
}
//===========================================================
void X_AbstractPlotterDataManager::xp_onEnergyLineOperation(QString elementSymbol, QString lineName,
                                                   EnergyLineOperationType operationType)
{
    if(!xv_plotter)
    {
        return;
    }
    // find item
    X_EnergyLineGraphicsItem* energyLineItem = nullptr;
    QList<QGraphicsItem*> energyLineList = xv_plotter->xp_itemListForType(EnergyLineItemType);
    for(int i = 0; i < energyLineList.count(); i++)
    {
        energyLineItem = qgraphicsitem_cast<X_EnergyLineGraphicsItem*>(energyLineList.at(i));
        if(energyLineItem != nullptr && energyLineItem->xp_chemicalElementSymbol() == elementSymbol
                && energyLineItem->xp_lineName() == lineName)
        {
            // item found
            break;
        }

        // reset item pointer and go on
        energyLineItem = nullptr;
        continue;
    }

    if(operationType == 0)
    {
        //"EL_REMOVED";
        if(energyLineItem)
        {
            xv_plotter->xp_removeItem(energyLineItem);
        }
    }
    else if(operationType == 1)
    {
        //"EL_INSERTED";

        if(energyLineItem)
        {
            xv_plotter->xp_removeItem(energyLineItem);
        }

        // request energy line properties
        double energyVal;
        emit xg_requestEnergyLineEnergyValue(elementSymbol, lineName, energyVal);

        bool visibility;
        emit xg_requestEnergyLineVisibility(elementSymbol, lineName, visibility);

        QColor color;
        emit xg_requestEnergyLineColor(elementSymbol, lineName, color);

        int relativeIntensity = 100;
        emit xg_requestEnergyLineRelativeIntensity(elementSymbol, lineName, relativeIntensity);

        energyLineItem = new X_EnergyLineGraphicsItem(elementSymbol, lineName, relativeIntensity, energyVal);
        xv_plotter->xp_addItem(energyLineItem);

        double channel;
        if(xh_convertEnergyToChannel(energyVal, channel))
        {
            energyLineItem->xp_setXPosition(channel);
            energyLineItem->setVisible(visibility);
        }
        else
        {
            energyLineItem->xp_setXPosition(0.0);
            energyLineItem->setVisible(false);
        }

        energyLineItem->xp_setColor(color);
    }
    else if(operationType == 2)
    {
        // "EL_VISIBILITY_CHANGED";
        if(energyLineItem)
        {
            bool visibility;
            emit xg_requestEnergyLineVisibility(elementSymbol, lineName, visibility);
            energyLineItem->setVisible(visibility);
        }
    }
    else if(operationType == 3)
    {
        // "EL_COLOR_CHANGED";
        if(energyLineItem)
        {
            QColor color;
            emit xg_requestEnergyLineColor(elementSymbol, lineName, color);
            energyLineItem->xp_setColor(color);
        }
    }
    else if(operationType == 4)
    {
        // "EL_CHANGED";
        if(energyLineItem)
        {
            int relativeIntensity;
            emit xg_requestEnergyLineRelativeIntensity(elementSymbol, lineName, relativeIntensity);
            energyLineItem->xp_setHeightPercent(relativeIntensity);
        }
    }
}
//===========================================================
void X_AbstractPlotterDataManager::xp_onPlotterViewPortRectChange(QRectF rect)
{
    X_EnergyLineGraphicsItem::xp_setTopAndButtonMargins(rect.top(), rect.bottom());
    X_EnergyLineGraphicsItem* energyLineItem = nullptr;
    QList<QGraphicsItem*> energyLineList = xv_plotter->xp_itemListForType(EnergyLineItemType);
    for(int i = 0; i < energyLineList.count(); i++)
    {
        energyLineItem = qgraphicsitem_cast<X_EnergyLineGraphicsItem*>(energyLineList.at(i));
        if(energyLineItem != nullptr)
        {
            energyLineItem->xp_updateItem();

        }
    }
}
//===========================================================
void X_AbstractPlotterDataManager::xh_updateRulerTool(QPointF startPoint, QPointF endPoint, bool visibility)
{
    QRectF plotterRect = xv_plotter->xp_viewportSceneRect();
    QSize plotterPixelSize = xv_plotter->xp_viewportPixelSize();

    if(!visibility || !plotterRect.isValid() || plotterRect.isNull())
    {
        xv_plotter->xp_removeItemsForType(RulerToolItemType);
        return;
    }

    X_RulerToolGraphicsItem* rulerItem = nullptr;
    QList<QGraphicsItem*> itemList = xv_plotter->xp_itemListForType(RulerToolItemType);

    for(int i = 0; i < itemList.count(); i++)
    {
        rulerItem = qgraphicsitem_cast<X_RulerToolGraphicsItem*>(itemList.at(i));
        if(rulerItem)
        {
            break;
        }
    }

    if(!rulerItem)
    {
        rulerItem = new X_RulerToolGraphicsItem;
        xv_plotter->xp_addItem(rulerItem);
    }

    // first channel
    QString infoString = tr("Start: %1<br>End: %2").arg(QString::number(static_cast<int>(startPoint.x()) + 1),
                                                                     QString::number(static_cast<int>(endPoint.x()) + 1));

    QList<QGraphicsItem*> spectrumList = xv_plotter->xp_itemListForType(SpectrumItemType);
    if(!spectrumList.isEmpty())
    {
        qint64 id = X_SpectrumGraphicsItem::xp_currentSpectrumId();
        const X_AbstractSpectrum* currentSpectrum = xv_spectrumArrayRepositiry->xp_spectrumForId(id);
        if(currentSpectrum)
        {
            double intensity;
            if(currentSpectrum->xp_intensityInWindow(static_cast<int>(startPoint.x()),
                                                     static_cast<int>(endPoint.x()),
                                                     intensity))
            {
                infoString += tr("<br>Integral: %1").arg(QString::number(intensity, 'f', 0));
            }
        }
    }

    rulerItem->xp_setProperties(startPoint,
                                endPoint,
                                plotterRect,
                                plotterPixelSize,
                                infoString);
}
//======================================================
void X_AbstractPlotterDataManager::xh_onCurrentEnergyCalibrationChange(QList<double> calibrationFactors)
{
    xv_calibrationFactors = calibrationFactors;
    xv_plotter->xp_setEnergyCalibration(xv_calibrationFactors);
    xh_updateEnergyLines();
    xh_updateRuleMetrix();
}
//======================================================
void X_AbstractPlotterDataManager::xh_updateEnergyLines()
{
    X_EnergyLineGraphicsItem* energyLineItem = nullptr;
    QList<QGraphicsItem*> energyLineList = xv_plotter->xp_itemListForType(EnergyLineItemType);
    for(int i = 0; i < energyLineList.count(); i++)
    {
        energyLineItem = qgraphicsitem_cast<X_EnergyLineGraphicsItem*>(energyLineList.at(i));
        if(energyLineItem == nullptr)
        {
            continue;
        }

        double channel = 0.0;
        if(!xh_convertEnergyToChannel(energyLineItem->xp_energyValue(), channel))
        {
            channel = 0.0;
        }

        energyLineItem->xp_setXPosition(channel);
    }
}
//===========================================================
void X_AbstractPlotterDataManager::xp_currentCalibrationChanged(qint64 currentCalibrationId,
                                                       int currentCalibrationIndex)
{
    if((xv_currentCalibrationId == currentCalibrationId && xv_currentCalibrationIndex == currentCalibrationIndex)
            || xv_plotter == nullptr || xv_calibrationRepository == nullptr)
    {
        return;
    }

    xh_setCurrentWindowId(-1);
}
//===========================================================
void X_AbstractPlotterDataManager::xp_currentCalibrationWindowChanged(qint64 currentWindowId, int currentWindowIndex,
                                                             qint64 previousWindowId, int previousWindowIndex)
{
    xh_setCurrentWindowId(currentWindowId);
}
//===========================================================
void X_AbstractPlotterDataManager::xh_setCurrentWindowId(qint64 windowId)
{
    X_WindowGraphicsItem::xp_setCurrentWindowId(windowId);

    QList<QGraphicsItem*> windowList = xv_plotter->xp_itemListForType(WindowItemType);
    X_WindowGraphicsItem* windowItem;
    bool visible;
    for(int w = 0; w < windowList.count(); w++)
    {
        windowItem = qgraphicsitem_cast<X_WindowGraphicsItem*>(windowList.at(w));
        if(windowItem == nullptr)
        {
            continue;
        }
        visible = xv_calibrationRepository->xp_isCalibrationWindowVisible(windowItem->xp_windowId());
        windowItem->xp_updateCurrentWindow(visible);
    }
    xv_plotter->xp_updatePlot();
}
//===========================================================
void X_AbstractPlotterDataManager::xp_currentArrayChanged(qint64 currentArrayId,
                                                 int currentArrayIndex)
{
    if((xv_currentArrayIndex == currentArrayIndex && xv_currentArrayId == currentArrayId)
            || xv_plotter == nullptr || xv_spectrumArrayRepositiry == nullptr)
    {
        return;
    }

    // Current spectrum reset
    X_SpectrumGraphicsItem::xp_setCurrentSpectrumId(-1);

    xv_currentArrayId = currentArrayId;
    xv_currentArrayIndex = currentArrayIndex;
    xv_plotter->xp_clearItemsForType(SpectrumItemType);

    qreal verticalMax = (qreal)xv_spectrumArrayRepositiry->xp_arrayMaxIntensity(xv_currentArrayIndex);
    //qreal horizontalMax = (qreal)xv_spectrumArrayRepositiry->xp_arrayChannelCount(xv_currentArrayIndex);
    //xv_plotter->xp_setVerticalAbsMax(verticalMax);

    xh_setPlotterVerticalAbsMax(verticalMax);

    //xh_setMaxParametersToDefaultItem(verticalMax, horizontalMax);

    QList<X_AbstractSpectrum*> spectrumList = xv_spectrumArrayRepositiry->xp_spectrumListForArray(xv_currentArrayIndex);
    X_SpectrumGraphicsItem* spectrumItem;
    qreal distortionFactor = 0.0;
    qreal distortionCorrectionFactor = 0.0;
    xv_plotter->xp_verticalDistortionFactors(distortionFactor, distortionCorrectionFactor);
    bool isPlotScaled = xv_plotter->xp_isPlotScaled();
    for(int i = 0; i < spectrumList.count(); i++)
    {
        spectrumItem = new X_SpectrumGraphicsItem(spectrumList.at(i),
                                                 xv_boundingRectTopFactor,
                                                 distortionFactor,
                                                 distortionCorrectionFactor);
        xv_plotter->xp_addItem(spectrumItem);
    }

    xh_updateRuleMetrix();
    xh_updateEnergyLines();

    if(!isPlotScaled)
    {
        xv_plotter->xp_fitInBoundingRect();
        //        QMetaObject::invokeMethod(xv_plotter, "xp_fitInBoundingRect",
        //                                  Qt::QueuedConnection);

    }
}
//===========================================================
void X_AbstractPlotterDataManager::xp_currentSpectrumChanged(qint64 currentSpectrumId,
                                                    int currentSpectrumIndex,
                                                    qint64 previousSpectrumId,
                                                    int previousSpectrumIndex)
{
    X_SpectrumGraphicsItem::xp_setCurrentSpectrumId(currentSpectrumId);

    QList<QGraphicsItem*> itemList = xv_plotter->xp_itemListForType(SpectrumItemType);
    X_SpectrumGraphicsItem* spectrumItem;
    bool visible;
    for(int i = 0; i < itemList.count(); i++)
    {
        spectrumItem = qgraphicsitem_cast<X_SpectrumGraphicsItem*>(itemList.value(i));
        if(!spectrumItem)
        {
            continue;
        }
        visible = xv_spectrumArrayRepositiry->xp_isSpectrumVisible(xv_currentArrayId, spectrumItem->xp_spectrumId());
        spectrumItem->xp_updateCurrentSpectrum(visible);
    }
    xv_plotter->xp_updatePlot();
}
//===========================================================
bool X_AbstractPlotterDataManager::xh_convertEnergyToChannel(double energyValue, double& channel)
{
    if(xv_calibrationFactors.count() < 2)
    {
        return false;
    }

    if(xv_calibrationFactors.count() == 2 || (xv_calibrationFactors.count() > 2 && xv_calibrationFactors.at(2) == 0.0))
    {
        // linear equation
        if(xv_calibrationFactors.at(1) == 0.0)
        {
            return false;
        }

        channel = (energyValue - xv_calibrationFactors.at(0)) / xv_calibrationFactors.at(1);
        return true;
    }

    // square equation
    // D = b^2 - 4*(ac)
    double D = pow(xv_calibrationFactors.at(1), 2.0) - (4*xv_calibrationFactors.at(2) * (xv_calibrationFactors.at(0) - energyValue));

    if(D < 0.0)
    {
        return false;
    }

    if(D == 0.0)
    {
        channel = (-xv_calibrationFactors.at(1)) / (2.0 * xv_calibrationFactors.at(2));
    }
    else // D > 0
    {
        double ch1 = (-xv_calibrationFactors.at(1) + sqrt(D)) / (2.0 * xv_calibrationFactors.at(2));
        double ch2 = (-xv_calibrationFactors.at(1) - sqrt(D)) / (2.0 * xv_calibrationFactors.at(2));

        channel = qMax(ch1, ch2);
    }

    return true;
}
//===========================================================
void X_AbstractPlotterDataManager::xh_createComponents()
{
    xv_switchRuleMetrixAction = new QAction(this);
    xv_switchRuleMetrixAction->setIcon(QIcon(NS_Icons::glIconAxisToEnergy));
    xv_switchRuleMetrixAction->setCheckable(true);
}
//===========================================================
void X_AbstractPlotterDataManager::xh_createConnections()
{
    connect(xv_switchRuleMetrixAction, &QAction::toggled,
            this, &X_AbstractPlotterDataManager::xh_switchRuleMetrix);
}
//===========================================================
void X_AbstractPlotterDataManager::xh_onRepositoryArrayOperation(X_SpectrumArrayRepository::SpectrumOperationType type,
                                                        int arrayIndex, int first, int last)
{
    if(xv_currentArrayIndex != arrayIndex || xv_plotter == nullptr)
    {
        return;
    }

    if(type == X_SpectrumArrayRepository::SOT_BEGIN_INSERT_SPECTRA)
    {

    }
    else if(type == X_SpectrumArrayRepository::SOT_END_INSERT_SPECTRA)
    {
        qreal distortionFactor = 0.0;
        qreal distortionCorrectionFactor = 0.0;
        xv_plotter->xp_verticalDistortionFactors(distortionFactor, distortionCorrectionFactor);
        bool isPlotScaled = xv_plotter->xp_isPlotScaled();

        for(int s = first; s <= last; s++ )
        {
            const X_AbstractSpectrum* spectrum = xv_spectrumArrayRepositiry->xp_spectrum(xv_currentArrayIndex, s);
            if(spectrum)
            {
                X_SpectrumGraphicsItem* spectrumItem = new X_SpectrumGraphicsItem(spectrum,
                                                                                xv_boundingRectTopFactor,
                                                                                distortionFactor,
                                                                                distortionCorrectionFactor);
                xv_plotter->xp_addItem(spectrumItem);
            }
        }

        if(!isPlotScaled)
        {
            xv_plotter->xp_fitInBoundingRect();
            //            QMetaObject::invokeMethod(xv_plotter, "xp_fitInBoundingRect",
            //                                      Qt::QueuedConnection);
        }
    }
    else if(type == X_SpectrumArrayRepository::SOT_BEGIN_REMOVE_SPECTRA)
    {
        // spectrum is searching by id
        for(int i = first; i <= last; i++ )
        {
            qint64 spectrumId = xv_spectrumArrayRepositiry->xp_spectrumId(xv_currentArrayIndex, i);
            QList<QGraphicsItem*> spectrumItemList = xv_plotter->xp_itemListForType(SpectrumItemType);
            for(int s = spectrumItemList.count() - 1; s >= 0; s--)
            {
                X_SpectrumGraphicsItem* spectrumItem = qgraphicsitem_cast<X_SpectrumGraphicsItem*>(spectrumItemList.at(s));
                if(!spectrumItem)
                {
                    continue;
                }
                if(spectrumItem->xp_spectrumId() == spectrumId)
                {
                    xv_plotter->xp_removeItem(spectrumItemList.at(s));
                    break;
                }
            }
        }
    }
    else if(type == X_SpectrumArrayRepository::SOT_END_REMOVE_SPECTRA)
    {

    }
    else if(type == X_SpectrumArrayRepository::SOT_VISIBLE_CHANGED)
    {
        for(int i = first; i <= last; i++ )
        {
            const X_AbstractSpectrum* spectrum = xv_spectrumArrayRepositiry->xp_spectrum(xv_currentArrayIndex, i);
            if(!spectrum)
            {
                return;
            }
            qint64 spectrumId = spectrum->xp_spectrumId();
            QList<QGraphicsItem*> spectrumItemList = xv_plotter->xp_itemListForType(SpectrumItemType);
            for(int i = 0; i < spectrumItemList.count(); i++)
            {
                X_SpectrumGraphicsItem* spectrumItem = qgraphicsitem_cast<X_SpectrumGraphicsItem*>(spectrumItemList.at(i));
                if(!spectrumItem)
                {
                    continue;
                }
                if(spectrumItem->xp_spectrumId() == spectrumId)
                {
                    spectrumItem->xp_updateSpectrumData(spectrum);
                    break;
                }
            }
        }
    }
}
//===========================================================
void X_AbstractPlotterDataManager::xh_onRepositoryCalibrationWindowOperation(X_CalibrationRepository::WindowOperationType type,
                                                                    int calibrationIndex, int first, int last)
{
    if(xv_plotter == nullptr)
    {
        return;
    }

    if(type == X_CalibrationRepository::WOT_BEGIN_RESET)
    {
        xv_plotter->xp_removeItemsForType(WindowItemType);
    }
    else if(type == X_CalibrationRepository::WOT_END_RESET)
    {
        // calibration count
        int calibrationCount = xv_calibrationRepository->xp_calibrationCount();
        int windowCount;
        qint64 calibrationId;
        for(int c = 0; c < calibrationCount; c++)
        {
            calibrationId = xv_calibrationRepository->xp_calibrationIdForCalibrationIndex(c);
            windowCount = xv_calibrationRepository->xp_calibrationWindowCount(calibrationId);
            for(int w = 0; w < windowCount; w++)
            {
                QColor windowColor = xv_calibrationRepository->xp_calibrationWindowColor(calibrationId, w);
                const X_CalibrationWindow* window = xv_calibrationRepository->xp_calibrationWindow(calibrationId, w);
                X_WindowGraphicsItem* windowItem = new X_WindowGraphicsItem(window, windowColor);
                xv_plotter->xp_addItem(windowItem);
            }
        }
    }
    else if(type == X_CalibrationRepository::WOT_INSERT_WINDOWS)
    {

    }
    else if(type == X_CalibrationRepository::WOT_END_INSERT_WINDOWS)
    {
        qint64 calibrationId;
        calibrationId = xv_calibrationRepository->xp_calibrationIdForCalibrationIndex(calibrationIndex);
        if(calibrationId < 0)
        {
            return;
        }
        for(int w = first; w <= last; w++ )
        {
            QColor windowColor = xv_calibrationRepository->xp_calibrationWindowColor(calibrationId, w);
            const X_CalibrationWindow* window = xv_calibrationRepository->xp_calibrationWindow(calibrationId, w);
            X_WindowGraphicsItem* windowItem = new X_WindowGraphicsItem(window, windowColor);
            xv_plotter->xp_addItem(windowItem);
        }
    }
    else if(type == X_CalibrationRepository::WOT_REMOVE_WINDOWS)
    {
        qint64 calibrationId;
        calibrationId = xv_calibrationRepository->xp_calibrationIdForCalibrationIndex(calibrationIndex);
        if(calibrationId < 0)
        {
            return;
        }
        qint64 windowId;
        QSet<qint64> windowIdSet;
        for(int w = first; w <= last; w++ )
        {
            windowId = xv_calibrationRepository->xp_calibrationWindowId(calibrationId, w);
            if(windowId < 0)
            {
                continue;
            }
            windowIdSet << windowId;
        }

        X_WindowGraphicsItem* windowItem;
        QList<QGraphicsItem*>windowList = xv_plotter->xp_itemListForType(WindowItemType);
        for(int i = windowList.count()-1; i >= 0; i--)
        {
            windowItem = qgraphicsitem_cast<X_WindowGraphicsItem*>(windowList.value(i));
            if(windowItem == nullptr)
            {
                continue;
            }
            if(windowIdSet.contains(windowItem->xp_windowId()))
            {
                xv_plotter->xp_removeItem(windowItem);
            }
        }
    }
    else if(type == X_CalibrationRepository::WOT_END_REMOVE_WINDOWS)
    {

    }
    else if(type == X_CalibrationRepository::WOT_WINDOW_CHANGED ||
            type == X_CalibrationRepository::WOT_WINDOW_VISIBLE_CHANGED)
    {
        qint64 calibrationId;
        calibrationId = xv_calibrationRepository->xp_calibrationIdForCalibrationIndex(calibrationIndex);
        if(calibrationId < 0)
        {
            return;
        }

        X_WindowGraphicsItem* windowItem = nullptr;
        QList<QGraphicsItem*>windowList = xv_plotter->xp_itemListForType(WindowItemType);

        for(int w = first; w <= last; w++ )
        {
            const X_CalibrationWindow* window = xv_calibrationRepository->xp_calibrationWindow(calibrationId, w);
            if(window == nullptr)
            {
                continue;
            }
            for(int i = 0; i < windowList.count(); i++)
            {
                windowItem = qgraphicsitem_cast<X_WindowGraphicsItem*>(windowList.value(i));
                if(windowItem == nullptr)
                {
                    continue;
                }

                if(window->xp_windowId() == windowItem->xp_windowId())
                {
                    qreal firstChannel = (qreal)window->xp_firstChannel();
                    qreal lastChannel = (qreal)window->xp_lastChannel();
                    windowItem->xp_setLeftRightMargins(firstChannel, lastChannel);
                    windowItem->xp_setVisible(window->xp_isWindowVisible());
                    break;
                }
            }

            //            if(windowItem)
            //            {
            //                xv_plotter->xp_addItem(windowItem);
            //            }
        }
    }
}

//===========================================================
void X_AbstractPlotterDataManager::xh_switchRuleMetrix(bool toggled)
{
    xv_energyRuleMetrixFlag = toggled;
    xh_updateRuleMetrix();
}
//======================================================
void X_AbstractPlotterDataManager::xh_updateRuleMetrix()
{
    if(!xv_energyRuleMetrixFlag || xv_calibrationFactors.isEmpty() ||
            (!(xv_calibrationFactors.value(1, 0.0) == 0.0 ||
               xv_calibrationFactors.value(2, 0.0) == 0.0 )))
    {
        xv_plotter->xp_setBottomMarkRecalcFlag(false);
        xv_plotter->xp_setTopMarkRecalcFlag(false);
        xv_plotter->xp_setHorizontalMarkRecalcFactors(xv_horizontalRuleLabel,
                                                      0, 1, 0);
        return;
    }

    qreal K0 = xv_calibrationFactors.value(0, 0.0);
    qreal K1 = xv_calibrationFactors.value(1, 1.0);
    qreal K2 = xv_calibrationFactors.value(2, 0.0);
    QString energyUnit = tr("kEv");

    QString labelString = xv_horizontalRecalcedRuleLabel + " " + energyUnit;
    xv_plotter->xp_setBottomMarkRecalcFlag(true);
    xv_plotter->xp_setTopMarkRecalcFlag(true);
    xv_plotter->xp_setHorizontalMarkRecalcFactors(labelString,
                                                  K0, K1, K2);

}
////===========================================================
//void X_PlotterDataManager::xh_updateEnergyCalibrationOnRule(qint64 arrayId)
//{
//    if(!xv_switchRuleMetrixAction->isChecked() || xv_currentArrayId != arrayId)
//    {
//        return;
//    }

//    xh_switchRuleMetrix(xv_switchRuleMetrixAction->isChecked());
//}
//===========================================================
void X_AbstractPlotterDataManager::xh_onArrayMaxParametersChanged(int arrayId, int maxIntensity, int channelCount)
{
    if(arrayId != xv_currentArrayId || xv_plotter == nullptr)
    {
        return;
    }

    xh_setPlotterVerticalAbsMax((qreal)maxIntensity);
    //   xv_plotter->xp_setVerticalAbsMax((qreal)maxIntensity);

    //   // windows height recalc
    //   X_WindowGraphicsItem::xp_setTopAndButtonMargins(qAbs((qreal)maxIntensity)*-1.0, 0.0);
    //   QList<QGraphicsItem*> windowList = xv_plotter->xp_itemListForType(WindowItemType);
    //   X_WindowGraphicsItem* windowItem;
    //   for(int w = 0; w < windowList.count(); w++)
    //   {
    //      windowItem = qgraphicsitem_cast<X_WindowGraphicsItem*>(windowList.at(w));
    //      if(windowItem == 0)
    //      {
    //         continue;
    //      }
    //      windowItem->xp_updateWindowHeight();
    //   }
    //   xv_plotter->xp_updatePlot();

    //   //xh_setMaxParametersToDefaultItem((qreal)maxIntensity, (qreal)channelCount);
}
//===========================================================
void X_AbstractPlotterDataManager::xh_setPlotterVerticalAbsMax(qreal maxIntensity)
{
    xv_plotter->xp_setVerticalAbsMax((qreal)maxIntensity);

    // windows height recalc
    X_WindowGraphicsItem::xp_setTopAndButtonMargins(qAbs((qreal)maxIntensity)*-1.0, 0.0);
    QList<QGraphicsItem*> windowList = xv_plotter->xp_itemListForType(WindowItemType);
    X_WindowGraphicsItem* windowItem;
    for(int w = 0; w < windowList.count(); w++)
    {
        windowItem = qgraphicsitem_cast<X_WindowGraphicsItem*>(windowList.at(w));
        if(windowItem == nullptr)
        {
            continue;
        }
        windowItem->xp_updateItem();
    }
    xv_plotter->xp_updatePlot();
}
//===========================================================
void X_AbstractPlotterDataManager::xh_setMaxParametersToDefaultItem(qreal maxY, qreal maxX)
{
    QList<QGraphicsItem*>defaultItemList = xv_plotter->xp_itemListForType(DefaultRectItemType);
    if(defaultItemList.isEmpty())
    {
        return;
    }

    X_DefaultRectGraphicsItem* defaultItem = qgraphicsitem_cast<X_DefaultRectGraphicsItem*>(defaultItemList[0]);
    if(!defaultItem)
    {
        return;
    }

    QRectF rect = QRectF(QPointF(0.0, qAbs(maxY)*-1.0), QPointF(maxX, 0.0));
    if(rect.isEmpty())
    {
        return;
    }

    defaultItem->xp_fitItemInRect(rect);
}
//===========================================================
void X_AbstractPlotterDataManager::xh_definePlotScaling(bool& plotIsScaled)
{
    plotIsScaled = xv_plotter->xp_isPlotScaled();
}
//===========================================================
void X_AbstractPlotterDataManager::xh_findItemInCursorAreaImage(QImage cursorAreaImage)
{
    // central pixel
    int centralPointRow = cursorAreaImage.height()/2;
    int centralPointColumn = cursorAreaImage.width()/2;

    QPoint centralPixel = QPoint(centralPointColumn, centralPointRow);
    QColor centralPixelColor = cursorAreaImage.pixel(centralPixel);

    QList<QGraphicsItem*>itemList = xv_plotter->xp_itemListForType(SpectrumItemType);
    X_SpectrumGraphicsItem* spectrumItem;
    qint64 spectrumId = -1;// = spectrumItem;
    QColor spectrumColor;
    bool spectrumFound = false;
    // buffer map for futher search in peripheal points of image
    QMap<qint64, QColor> itemColorIdBufferMap;

    for(int i = 0; i < itemList.count(); i++)
    {
        spectrumItem = qgraphicsitem_cast<X_SpectrumGraphicsItem*>(itemList.at(i));
        if(spectrumItem == nullptr)
        {
            continue;
        }
        spectrumColor = spectrumItem->xp_spectrumColor();
        if(spectrumColor != centralPixelColor)
        {
            // set spe color and spe id in buffer map and continue
            itemColorIdBufferMap.insert( spectrumItem->xp_spectrumId(), spectrumColor);
            continue;
        }

        spectrumId = spectrumItem->xp_spectrumId();
        xv_spectrumArrayRepositiry->xp_setSpectrumCurrent(spectrumId);
        return;
    }

    if(!spectrumFound)
    {
        QList<QPoint> checkedPointList;
        checkedPointList.append(centralPixel);

        QPoint currentPoint = QPoint();
        int periphialCircleStartRow = centralPointRow;
        int periphialCircleStartColumn = centralPointColumn;
        int periphialCircleLastRow = centralPointRow;
        int periphialCircleLastCol = centralPointColumn;
        // circle on peripheral points
        for(;;)
        {
            periphialCircleStartRow = periphialCircleStartRow <= 0 ? 0 : periphialCircleStartRow -1;
            periphialCircleStartColumn = periphialCircleStartColumn <= 0 ? 0 : periphialCircleStartColumn -1;

            periphialCircleLastRow = (periphialCircleLastRow >= cursorAreaImage.height() - 1)? cursorAreaImage.height() - 1 : periphialCircleLastRow + 1;
            periphialCircleLastCol = (periphialCircleLastCol >= cursorAreaImage.width() - 1)? cursorAreaImage.width() - 1 : periphialCircleLastCol + 1;
            // spe color corcle
            QMap<qint64, QColor>::iterator it;
            for(it = itemColorIdBufferMap.begin(); it != itemColorIdBufferMap.end(); it++)
            {
                spectrumColor = it.value();
                // periphial circle
                // rows
                for(int row = periphialCircleStartRow; row <= periphialCircleLastRow; row++ )
                {
                    // columns
                    for(int col = periphialCircleStartColumn; col <= periphialCircleLastCol; col++)
                    {
                        // current point
                        currentPoint = QPoint(col, row);
                        if(checkedPointList.contains(currentPoint) || cursorAreaImage.width() <= col
                                || cursorAreaImage.height() <= row)
                        {
                            continue;
                        }

                        if(spectrumColor == QColor(cursorAreaImage.pixel(currentPoint)))
                        {
                            spectrumId = it.key();
                            xv_spectrumArrayRepositiry->xp_setSpectrumCurrent(spectrumId);
                            return;
                        }

                        // if(it + 1 == itemColorIdBufferMap.end())
                        if(it.key() == itemColorIdBufferMap.lastKey())
                        {
                            // last spe. Insert point into checkedPointSet
                            checkedPointList.append(currentPoint);
                        }
                    }
                }
            }

            //
            if(periphialCircleStartRow == 0 && periphialCircleStartColumn == 0)
            {
                // last circle
                break;
            }
        }
    }
}
//===========================================================
