//===========================================================
#include "ZPlotterDataManager.h"
#include "ZPlotter.h"
#include "ZGraphicsItemUserTypes.h"
#include "ZSpectrumGraphicsItem.h"
#include "ZWindowGraphicsItem.h"
#include "ZDefaultRectGraphicsItem.h"
#include "ZRulerToolGraphicsItem.h"
#include "ZSpectrumGraphicsItem.h"
#include "ZEnergyLineGraphicsItem.h"
#include "ZSpeSpectrum.h"

#include "ZGeneral.h"
#include <math.h>
#include <QSet>
#include <math.h>

//===========================================================
ZPlotterDataManager::ZPlotterDataManager(QObject *parent) : QObject(parent)
{
    zv_spectrumArrayRepositiry = nullptr;
    zv_calibrationRepository = nullptr;
    zv_plotter = nullptr;
    zv_currentArrayIndex = -1;
    zv_switchRuleMetrixAction = nullptr;
    zv_currentArrayId = -1;
    zv_boundingRectTopFactor = 1.05;
    zv_defaultItem = nullptr;
    zv_currentCalibrationId = -1;
    zv_currentCalibrationIndex = -1;

    zv_verticalRuleLabel = tr("Intensity");
    zv_horizontalRuleLabel = tr("Channels");
    zv_horizontalRecalcedRuleLabel = tr("Energy");

    zh_createComponents();
    zh_createConnections();
}
//===========================================================
void ZPlotterDataManager::zp_connectToSpectraArrayRepository(ZSpectrumArrayRepository* repository)
{
    zv_spectrumArrayRepositiry = repository;
    // array repository <->  model
    connect(repository, &ZSpectrumArrayRepository::zg_spectrumOperation,
            this, &ZPlotterDataManager::zh_onRepositoryArrayOperation);
//    connect(repository, &ZSpectrumArrayRepository::zg_energyCalibrationChanged,
//            this, &ZPlotterDataManager::zh_updateEnergyCalibrationOnRule);
    connect(repository, &ZSpectrumArrayRepository::zg_arrayMaxParametersChanged,
            this, &ZPlotterDataManager::zh_onArrayMaxParametersChanged);
    connect(repository, &ZSpectrumArrayRepository::zg_requestIsPlotScaled,
            this, &ZPlotterDataManager::zh_definePlotScaling);
    connect(repository, &ZSpectrumArrayRepository::zg_currentArrayIdChanged,
            this, &ZPlotterDataManager::zp_currentArrayChanged);
    connect(repository, &ZSpectrumArrayRepository::zg_currentSpectrumChanged,
            this, &ZPlotterDataManager::zp_currentSpectrumChanged);
    connect(repository, &ZSpectrumArrayRepository::zg_energyCalibrationChanged,
            this, &ZPlotterDataManager::zh_onCurrentEnergyCalibrationChange);


}
//===========================================================
void ZPlotterDataManager::zp_connectToCalibrationRepository(ZCalibrationRepository* repository)
{
    zv_calibrationRepository = repository;
    connect(repository, &ZCalibrationRepository::zg_calibrationWindowOperation,
            this, &ZPlotterDataManager::zh_onRepositoryCalibrationWindowOperation);
    connect(repository, &ZCalibrationRepository::zg_currentCalibrationChanged,
            this, &ZPlotterDataManager::zp_currentCalibrationChanged);
    connect(repository, &ZCalibrationRepository::zg_currentCalibrationWindowChanged,
            this, &ZPlotterDataManager::zp_currentCalibrationWindowChanged);
}
//===========================================================
void ZPlotterDataManager::zp_connectToPlotter(ZPlotter* plotter)
{
    zv_plotter = plotter;
    zv_plotter->zp_setAutoDefineVerticalAbsMax(false);
    QList<QAction*> actionList;
    actionList << zv_switchRuleMetrixAction;
    zv_plotter->zp_appendButtonsToDashboard(actionList,
                                            ZHorizontalDashBoard::AWP_LEFT_OF_BUTTONS,
                                            Qt::AlignLeft, 0);

    connect(zv_plotter, &ZPlotter::zg_cursorAreaImage,
            this, &ZPlotterDataManager::zh_findItemInCursorAreaImage);

    // rule settings // No label on right and top
    zv_plotter->zp_setLeftMarkRecalcFlag(false);
    zv_plotter->zp_setLeftRuleLabel(zv_verticalRuleLabel);
    zv_plotter->zp_setLeftRuleLabelVisible(true);

    zv_plotter->zp_setRightMarkRecalcFlag(false);
    zv_plotter->zp_setRightRuleLabelVisible(false);

    zv_plotter->zp_setTopMarkRecalcFlag(false);
    zv_plotter->zp_setTopRuleLabelVisible(false);

    zv_plotter->zp_setBottomMarkRecalcFlag(false);
    zv_plotter->zp_setBottomRuleLabel(zv_horizontalRuleLabel);
    zv_plotter->zp_setBottomRuleLabelVisible(true);

    zh_setPlotterVerticalAbsMax(100.0);

    if(zv_plotter->zp_itemCount() <= 0)
    {
        zv_defaultItem = new ZDefaultRectGraphicsItem(zv_defaultSceneRect, false, false, false);
        zv_plotter->zp_addItem(zv_defaultItem);
    }

    connect(zv_plotter, &ZPlotter::zg_viewportRectChanged,
            this, &ZPlotterDataManager::zp_onPlotterViewPortRectChange);
    connect(zv_plotter, &ZPlotter::zg_rulerToolChanged,
            this, &ZPlotterDataManager::zh_updateRulerTool);
}
//===========================================================
void ZPlotterDataManager::zp_onEnergyLineOperation(QString elementSymbol, QString lineName,
                                                   EnergyLineOperationType operationType)
{
    if(!zv_plotter)
    {
        return;
    }
    // find item
    ZEnergyLineGraphicsItem* energyLineItem = nullptr;
    QList<QGraphicsItem*> energyLineList = zv_plotter->zp_itemListForType(EnergyLineItemType);
    for(int i = 0; i < energyLineList.count(); i++)
    {
        energyLineItem = qgraphicsitem_cast<ZEnergyLineGraphicsItem*>(energyLineList.at(i));
        if(energyLineItem != nullptr && energyLineItem->zp_chemicalElementSymbol() == elementSymbol
                && energyLineItem->zp_lineName() == lineName)
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
            zv_plotter->zp_removeItem(energyLineItem);
        }
    }
    else if(operationType == 1)
    {
        //"EL_INSERTED";

        if(energyLineItem)
        {
            zv_plotter->zp_removeItem(energyLineItem);
        }

        // request energy line properties
        double energyVal;
        emit zg_requestEnergyLineEnergyValue(elementSymbol, lineName, energyVal);

        bool visibility;
        emit zg_requestEnergyLineVisibility(elementSymbol, lineName, visibility);

        QColor color;
        emit zg_requestEnergyLineColor(elementSymbol, lineName, color);

        int relativeIntensity = 100;
        emit zg_requestEnergyLineRelativeIntensity(elementSymbol, lineName, relativeIntensity);

        energyLineItem = new ZEnergyLineGraphicsItem(elementSymbol, lineName, relativeIntensity, energyVal);
        zv_plotter->zp_addItem(energyLineItem);

        double channel;
        if(zh_convertEnergyToChannel(energyVal, channel))
        {
            energyLineItem->zp_setXPosition(channel);
            energyLineItem->setVisible(visibility);
        }
        else
        {
            energyLineItem->zp_setXPosition(0.0);
            energyLineItem->setVisible(false);
        }

        energyLineItem->zp_setColor(color);
    }
    else if(operationType == 2)
    {
        // "EL_VISIBILITY_CHANGED";
        if(energyLineItem)
        {
            bool visibility;
            emit zg_requestEnergyLineVisibility(elementSymbol, lineName, visibility);
            energyLineItem->setVisible(visibility);
        }
    }
    else if(operationType == 3)
    {
        // "EL_COLOR_CHANGED";
        if(energyLineItem)
        {
            QColor color;
            emit zg_requestEnergyLineColor(elementSymbol, lineName, color);
            energyLineItem->zp_setColor(color);
        }
    }
    else if(operationType == 4)
    {
        // "EL_CHANGED";
        if(energyLineItem)
        {
            int relativeIntensity;
            emit zg_requestEnergyLineRelativeIntensity(elementSymbol, lineName, relativeIntensity);
            energyLineItem->zp_setHeightPercent(relativeIntensity);
        }
    }
}
//===========================================================
void ZPlotterDataManager::zp_onPlotterViewPortRectChange(QRectF rect)
{
    ZEnergyLineGraphicsItem::zp_setTopAndButtonMargins(rect.top(), rect.bottom());
    ZEnergyLineGraphicsItem* energyLineItem = nullptr;
    QList<QGraphicsItem*> energyLineList = zv_plotter->zp_itemListForType(EnergyLineItemType);
    for(int i = 0; i < energyLineList.count(); i++)
    {
        energyLineItem = qgraphicsitem_cast<ZEnergyLineGraphicsItem*>(energyLineList.at(i));
        if(energyLineItem != nullptr)
        {
            energyLineItem->zp_updateItem();

        }
    }
}
//===========================================================
void ZPlotterDataManager::zh_updateRulerTool(QPointF startPoint, QPointF endPoint, bool visibility)
{
    // qDebug() << "UPDATE RT" << startPoint.x() << endPoint.x() << visibility;
    QRectF plotterRect = zv_plotter->zp_viewportSceneRect();
    QSize plotterPixelSize = zv_plotter->zp_viewportPixelSize();

    if(!visibility || !plotterRect.isValid() || plotterRect.isNull())
    {
        zv_plotter->zp_removeItemsForType(RulerToolItemType);
        return;
    }

    ZRulerToolGraphicsItem* rulerItem = nullptr;
    QList<QGraphicsItem*> itemList = zv_plotter->zp_itemListForType(RulerToolItemType);

    for(int i = 0; i < itemList.count(); i++)
    {
        rulerItem = qgraphicsitem_cast<ZRulerToolGraphicsItem*>(itemList.at(i));
        if(rulerItem)
        {
            break;
        }
    }

    if(!rulerItem)
    {
        rulerItem = new ZRulerToolGraphicsItem;
        zv_plotter->zp_addItem(rulerItem);
    }

    // first channel
    QString infoString = tr("Start: %1<br>End: %2").arg(QString::number(static_cast<int>(startPoint.x()) + 1),
                                                                     QString::number(static_cast<int>(endPoint.x()) + 1));

    QList<QGraphicsItem*> spectrumList = zv_plotter->zp_itemListForType(SpectrumItemType);
    if(!spectrumList.isEmpty())
    {
        qint64 id = ZSpectrumGraphicsItem::zp_currentSpectrumId();
        const ZAbstractSpectrum* currentSpectrum = zv_spectrumArrayRepositiry->zp_spectrumForId(id);
        if(currentSpectrum)
        {
            double intensity;
            if(currentSpectrum->zp_intensityInWindow(static_cast<int>(startPoint.x()),
                                                     static_cast<int>(endPoint.x()),
                                                     intensity))
            {
                infoString += tr("<br>Integral: %1").arg(QString::number(intensity, 'f', 0));
            }
        }
    }

    rulerItem->zp_setProperties(startPoint,
                                endPoint,
                                plotterRect,
                                plotterPixelSize,
                                infoString);
}
//======================================================
void ZPlotterDataManager::zh_onCurrentEnergyCalibrationChange(QList<double> calibrationFactors)
{
    zv_calibrationFactors = calibrationFactors;
    zv_plotter->zp_setEnergyCalibration(zv_calibrationFactors);
    zh_updateEnergyLines();
    zh_updateRuleMetrix();
}
//======================================================
void ZPlotterDataManager::zh_updateEnergyLines()
{
    ZEnergyLineGraphicsItem* energyLineItem = nullptr;
    QList<QGraphicsItem*> energyLineList = zv_plotter->zp_itemListForType(EnergyLineItemType);
    for(int i = 0; i < energyLineList.count(); i++)
    {
        energyLineItem = qgraphicsitem_cast<ZEnergyLineGraphicsItem*>(energyLineList.at(i));
        if(energyLineItem == nullptr)
        {
            continue;
        }

        double channel = 0.0;
        if(!zh_convertEnergyToChannel(energyLineItem->zp_energyValue(), channel))
        {
            channel = 0.0;
        }

        energyLineItem->zp_setXPosition(channel);
    }
}
//===========================================================
void ZPlotterDataManager::zp_currentCalibrationChanged(qint64 currentCalibrationId,
                                                       int currentCalibrationIndex)
{
    if((zv_currentCalibrationId == currentCalibrationId && zv_currentCalibrationIndex == currentCalibrationIndex)
            || zv_plotter == nullptr || zv_calibrationRepository == nullptr)
    {
        return;
    }

    zh_setCurrentWindowId(-1);
}
//===========================================================
void ZPlotterDataManager::zp_currentCalibrationWindowChanged(qint64 currentWindowId, int currentWindowIndex,
                                                             qint64 previousWindowId, int previousWindowIndex)
{
    zh_setCurrentWindowId(currentWindowId);
}
//===========================================================
void ZPlotterDataManager::zh_setCurrentWindowId(qint64 windowId)
{
    ZWindowGraphicsItem::zp_setCurrentWindowId(windowId);

    QList<QGraphicsItem*> windowList = zv_plotter->zp_itemListForType(WindowItemType);
    ZWindowGraphicsItem* windowItem;
    bool visible;
    for(int w = 0; w < windowList.count(); w++)
    {
        windowItem = qgraphicsitem_cast<ZWindowGraphicsItem*>(windowList.at(w));
        if(windowItem == nullptr)
        {
            continue;
        }
        visible = zv_calibrationRepository->zp_isCalibrationWindowVisible(windowItem->zp_windowId());
        windowItem->zp_updateCurrentWindow(visible);
    }
    zv_plotter->zp_updatePlot();
}
//===========================================================
void ZPlotterDataManager::zp_currentArrayChanged(qint64 currentArrayId,
                                                 int currentArrayIndex)
{
    if((zv_currentArrayIndex == currentArrayIndex && zv_currentArrayId == currentArrayId)
            || zv_plotter == nullptr || zv_spectrumArrayRepositiry == nullptr)
    {
        return;
    }

    // Current spectrum reset
    ZSpectrumGraphicsItem::zp_setCurrentSpectrumId(-1);

    zv_currentArrayId = currentArrayId;
    zv_currentArrayIndex = currentArrayIndex;
    zv_plotter->zp_clearItemsForType(SpectrumItemType);

    qreal verticalMax = (qreal)zv_spectrumArrayRepositiry->zp_arrayMaxIntensity(zv_currentArrayIndex);
    //qreal horizontalMax = (qreal)zv_spectrumArrayRepositiry->zp_arrayChannelCount(zv_currentArrayIndex);
    //zv_plotter->zp_setVerticalAbsMax(verticalMax);

    zh_setPlotterVerticalAbsMax(verticalMax);

    //zh_setMaxParametersToDefaultItem(verticalMax, horizontalMax);

    QList<ZAbstractSpectrum*> spectrumList = zv_spectrumArrayRepositiry->zp_spectrumListForArray(zv_currentArrayIndex);
    ZSpectrumGraphicsItem* spectrumItem;
    qreal distortionFactor;
    qreal distortionCorrectionFactor;
    zv_plotter->zp_verticalDistortionFactors(distortionFactor, distortionCorrectionFactor);
    bool isPlotScaled = zv_plotter->zp_isPlotScaled();
    for(int i = 0; i < spectrumList.count(); i++)
    {
        spectrumItem = new ZSpectrumGraphicsItem(spectrumList.at(i),
                                                 zv_boundingRectTopFactor,
                                                 distortionFactor,
                                                 distortionCorrectionFactor);
        zv_plotter->zp_addItem(spectrumItem);
    }

    zh_updateRuleMetrix();
    zh_updateEnergyLines();

    if(!isPlotScaled)
    {
        zv_plotter->zp_fitInBoundingRect();
        //        QMetaObject::invokeMethod(zv_plotter, "zp_fitInBoundingRect",
        //                                  Qt::QueuedConnection);

    }
}
//===========================================================
void ZPlotterDataManager::zp_currentSpectrumChanged(qint64 currentSpectrumId,
                                                    int currentSpectrumIndex,
                                                    qint64 previousSpectrumId,
                                                    int previousSpectrumIndex)
{
    ZSpectrumGraphicsItem::zp_setCurrentSpectrumId(currentSpectrumId);

    QList<QGraphicsItem*> itemList = zv_plotter->zp_itemListForType(SpectrumItemType);
    ZSpectrumGraphicsItem* spectrumItem;
    bool visible;
    for(int i = 0; i < itemList.count(); i++)
    {
        spectrumItem = qgraphicsitem_cast<ZSpectrumGraphicsItem*>(itemList.value(i));
        if(!spectrumItem)
        {
            continue;
        }
        visible = zv_spectrumArrayRepositiry->zp_isSpectrumVisible(zv_currentArrayId, spectrumItem->zp_spectrumId());
        spectrumItem->zp_updateCurrentSpectrum(visible);
    }
    zv_plotter->zp_updatePlot();
}
//===========================================================
bool ZPlotterDataManager::zh_convertEnergyToChannel(double energyValue, double& channel)
{
    if(zv_calibrationFactors.count() < 2)
    {
        return false;
    }

    if(zv_calibrationFactors.count() == 2 || (zv_calibrationFactors.count() > 2 && zv_calibrationFactors.at(2) == 0.0))
    {
        // linear equation
        if(zv_calibrationFactors.at(1) == 0.0)
        {
            return false;
        }

        channel = (energyValue - zv_calibrationFactors.at(0)) / zv_calibrationFactors.at(1);
        return true;
    }

    // square equation
    // D = b^2 - 4*(ac)
    double D = pow(zv_calibrationFactors.at(1), 2.0) - (4*zv_calibrationFactors.at(2) * (zv_calibrationFactors.at(0) - energyValue));

    if(D < 0.0)
    {
        return false;
    }

    if(D == 0.0)
    {
        channel = (-zv_calibrationFactors.at(1)) / (2.0 * zv_calibrationFactors.at(2));
    }
    else // D > 0
    {
        double ch1 = (-zv_calibrationFactors.at(1) + sqrt(D)) / (2.0 * zv_calibrationFactors.at(2));
        double ch2 = (-zv_calibrationFactors.at(1) - sqrt(D)) / (2.0 * zv_calibrationFactors.at(2));

        channel = qMax(ch1, ch2);
    }

    return true;
}
//===========================================================
void ZPlotterDataManager::zh_createComponents()
{
    zv_switchRuleMetrixAction = new QAction(this);
    zv_switchRuleMetrixAction->setIcon(QIcon(NS_Icons::glIconAxisToEnergy));
    zv_switchRuleMetrixAction->setCheckable(true);
}
//===========================================================
void ZPlotterDataManager::zh_createConnections()
{
    connect(zv_switchRuleMetrixAction, &QAction::toggled,
            this, &ZPlotterDataManager::zh_switchRuleMetrix);
}
//===========================================================
void ZPlotterDataManager::zh_onRepositoryArrayOperation(ZSpectrumArrayRepository::SpectrumOperationType type,
                                                        int arrayIndex, int first, int last)
{
    if(zv_currentArrayIndex != arrayIndex || zv_plotter == nullptr)
    {
        return;
    }

    if(type == ZSpectrumArrayRepository::SOT_BEGIN_INSERT_SPECTRA)
    {

    }
    else if(type == ZSpectrumArrayRepository::SOT_END_INSERT_SPECTRA)
    {
        qreal distortionFactor;
        qreal distortionCorrectionFactor;
        zv_plotter->zp_verticalDistortionFactors(distortionFactor, distortionCorrectionFactor);
        bool isPlotScaled = zv_plotter->zp_isPlotScaled();

        for(int s = first; s <= last; s++ )
        {
            const ZAbstractSpectrum* spectrum = zv_spectrumArrayRepositiry->zp_spectrum(zv_currentArrayIndex, s);
            if(spectrum)
            {
                ZSpectrumGraphicsItem* spectrumItem = new ZSpectrumGraphicsItem(spectrum,
                                                                                zv_boundingRectTopFactor,
                                                                                distortionFactor,
                                                                                distortionCorrectionFactor);
                zv_plotter->zp_addItem(spectrumItem);
            }
        }

        if(!isPlotScaled)
        {
            zv_plotter->zp_fitInBoundingRect();
            //            QMetaObject::invokeMethod(zv_plotter, "zp_fitInBoundingRect",
            //                                      Qt::QueuedConnection);
        }
    }
    else if(type == ZSpectrumArrayRepository::SOT_BEGIN_REMOVE_SPECTRA)
    {
        // spectrum is searching by id
        for(int i = first; i <= last; i++ )
        {
            qint64 spectrumId = zv_spectrumArrayRepositiry->zp_spectrumId(zv_currentArrayIndex, i);
            QList<QGraphicsItem*> spectrumItemList = zv_plotter->zp_itemListForType(SpectrumItemType);
            for(int s = spectrumItemList.count() - 1; s >= 0; s--)
            {
                ZSpectrumGraphicsItem* spectrumItem = qgraphicsitem_cast<ZSpectrumGraphicsItem*>(spectrumItemList.at(s));
                if(!spectrumItem)
                {
                    continue;
                }
                if(spectrumItem->zp_spectrumId() == spectrumId)
                {
                    zv_plotter->zp_removeItem(spectrumItemList.at(s));
                    break;
                }
            }
        }
    }
    else if(type == ZSpectrumArrayRepository::SOT_END_REMOVE_SPECTRA)
    {

    }
    else if(type == ZSpectrumArrayRepository::SOT_VISIBLE_CHANGED)
    {
        for(int i = first; i <= last; i++ )
        {
            const ZAbstractSpectrum* spectrum = zv_spectrumArrayRepositiry->zp_spectrum(zv_currentArrayIndex, i);
            if(!spectrum)
            {
                return;
            }
            qint64 spectrumId = spectrum->zp_spectrumId();
            QList<QGraphicsItem*> spectrumItemList = zv_plotter->zp_itemListForType(SpectrumItemType);
            for(int i = 0; i < spectrumItemList.count(); i++)
            {
                ZSpectrumGraphicsItem* spectrumItem = qgraphicsitem_cast<ZSpectrumGraphicsItem*>(spectrumItemList.at(i));
                if(!spectrumItem)
                {
                    continue;
                }
                if(spectrumItem->zp_spectrumId() == spectrumId)
                {
                    spectrumItem->zp_updateSpectrumData(spectrum);
                    break;
                }
            }
        }
    }
}
//===========================================================
void ZPlotterDataManager::zh_onRepositoryCalibrationWindowOperation(ZCalibrationRepository::WindowOperationType type,
                                                                    int calibrationIndex, int first, int last)
{
    if(zv_plotter == nullptr)
    {
        return;
    }

    if(type == ZCalibrationRepository::WOT_BEGIN_RESET)
    {
        zv_plotter->zp_removeItemsForType(WindowItemType);
    }
    else if(type == ZCalibrationRepository::WOT_END_RESET)
    {
        // calibration count
        int calibrationCount = zv_calibrationRepository->zp_calibrationCount();
        int windowCount;
        qint64 calibrationId;
        for(int c = 0; c < calibrationCount; c++)
        {
            calibrationId = zv_calibrationRepository->zp_calibrationIdForCalibrationIndex(c);
            windowCount = zv_calibrationRepository->zp_calibrationWindowCount(calibrationId);
            for(int w = 0; w < windowCount; w++)
            {
                QColor windowColor = zv_calibrationRepository->zp_calibrationWindowColor(calibrationId, w);
                const ZCalibrationWindow* window = zv_calibrationRepository->zp_calibrationWindow(calibrationId, w);
                ZWindowGraphicsItem* windowItem = new ZWindowGraphicsItem(window, windowColor);
                zv_plotter->zp_addItem(windowItem);
            }
        }
    }
    else if(type == ZCalibrationRepository::WOT_INSERT_WINDOWS)
    {

    }
    else if(type == ZCalibrationRepository::WOT_END_INSERT_WINDOWS)
    {
        qint64 calibrationId;
        calibrationId = zv_calibrationRepository->zp_calibrationIdForCalibrationIndex(calibrationIndex);
        if(calibrationId < 0)
        {
            return;
        }
        for(int w = first; w <= last; w++ )
        {
            QColor windowColor = zv_calibrationRepository->zp_calibrationWindowColor(calibrationId, w);
            const ZCalibrationWindow* window = zv_calibrationRepository->zp_calibrationWindow(calibrationId, w);
            ZWindowGraphicsItem* windowItem = new ZWindowGraphicsItem(window, windowColor);
            zv_plotter->zp_addItem(windowItem);
        }
    }
    else if(type == ZCalibrationRepository::WOT_REMOVE_WINDOWS)
    {
        qint64 calibrationId;
        calibrationId = zv_calibrationRepository->zp_calibrationIdForCalibrationIndex(calibrationIndex);
        if(calibrationId < 0)
        {
            return;
        }
        qint64 windowId;
        QSet<qint64> windowIdSet;
        for(int w = first; w <= last; w++ )
        {
            windowId = zv_calibrationRepository->zp_calibrationWindowId(calibrationId, w);
            if(windowId < 0)
            {
                continue;
            }
            windowIdSet << windowId;
        }

        ZWindowGraphicsItem* windowItem;
        QList<QGraphicsItem*>windowList = zv_plotter->zp_itemListForType(WindowItemType);
        for(int i = windowList.count()-1; i >= 0; i--)
        {
            windowItem = qgraphicsitem_cast<ZWindowGraphicsItem*>(windowList.value(i));
            if(windowItem == nullptr)
            {
                continue;
            }
            if(windowIdSet.contains(windowItem->zp_windowId()))
            {
                zv_plotter->zp_removeItem(windowItem);
            }
        }
    }
    else if(type == ZCalibrationRepository::WOT_END_REMOVE_WINDOWS)
    {

    }
    else if(type == ZCalibrationRepository::WOT_WINDOW_CHANGED ||
            type == ZCalibrationRepository::WOT_WINDOW_VISIBLE_CHANGED)
    {
        qint64 calibrationId;
        calibrationId = zv_calibrationRepository->zp_calibrationIdForCalibrationIndex(calibrationIndex);
        if(calibrationId < 0)
        {
            return;
        }

        ZWindowGraphicsItem* windowItem = nullptr;
        QList<QGraphicsItem*>windowList = zv_plotter->zp_itemListForType(WindowItemType);

        for(int w = first; w <= last; w++ )
        {
            const ZCalibrationWindow* window = zv_calibrationRepository->zp_calibrationWindow(calibrationId, w);
            if(window == nullptr)
            {
                continue;
            }
            for(int i = 0; i < windowList.count(); i++)
            {
                windowItem = qgraphicsitem_cast<ZWindowGraphicsItem*>(windowList.value(i));
                if(windowItem == nullptr)
                {
                    continue;
                }

                if(window->zp_windowId() == windowItem->zp_windowId())
                {
                    qreal firstChannel = (qreal)window->zp_firstChannel();
                    qreal lastChannel = (qreal)window->zp_lastChannel();
                    windowItem->zp_setLeftRightMargins(firstChannel, lastChannel);
                    windowItem->zp_setVisible(window->zp_isWindowVisible());
                    break;
                }
            }

            //            if(windowItem)
            //            {
            //                zv_plotter->zp_addItem(windowItem);
            //            }
        }
    }
}

//===========================================================
void ZPlotterDataManager::zh_switchRuleMetrix(bool toggled)
{
    zv_energyRuleMetrixFlag = toggled;
    zh_updateRuleMetrix();
}
//======================================================
void ZPlotterDataManager::zh_updateRuleMetrix()
{
    if(!zv_energyRuleMetrixFlag || zv_calibrationFactors.isEmpty() ||
            (!(zv_calibrationFactors.value(1, 0.0) == 0.0 ||
               zv_calibrationFactors.value(2, 0.0) == 0.0 )))
    {
        zv_plotter->zp_setBottomMarkRecalcFlag(false);
        zv_plotter->zp_setTopMarkRecalcFlag(false);
        zv_plotter->zp_setHorizontalMarkRecalcFactors(zv_horizontalRuleLabel,
                                                      0, 1, 0);
        return;
    }

    qreal K0 = zv_calibrationFactors.value(0, 0.0);
    qreal K1 = zv_calibrationFactors.value(1, 1.0);
    qreal K2 = zv_calibrationFactors.value(2, 0.0);
    QString energyUnit = tr("kEv");

    QString labelString = zv_horizontalRecalcedRuleLabel + " " + energyUnit;
    zv_plotter->zp_setBottomMarkRecalcFlag(true);
    zv_plotter->zp_setTopMarkRecalcFlag(true);
    zv_plotter->zp_setHorizontalMarkRecalcFactors(labelString,
                                                  K0, K1, K2);

}
////===========================================================
//void ZPlotterDataManager::zh_updateEnergyCalibrationOnRule(qint64 arrayId)
//{
//    if(!zv_switchRuleMetrixAction->isChecked() || zv_currentArrayId != arrayId)
//    {
//        return;
//    }

//    zh_switchRuleMetrix(zv_switchRuleMetrixAction->isChecked());
//}
//===========================================================
void ZPlotterDataManager::zh_onArrayMaxParametersChanged(int arrayId, int maxIntensity, int channelCount)
{
    if(arrayId != zv_currentArrayId || zv_plotter == nullptr)
    {
        return;
    }

    zh_setPlotterVerticalAbsMax((qreal)maxIntensity);
    //   zv_plotter->zp_setVerticalAbsMax((qreal)maxIntensity);

    //   // windows height recalc
    //   ZWindowGraphicsItem::zp_setTopAndButtonMargins(qAbs((qreal)maxIntensity)*-1.0, 0.0);
    //   QList<QGraphicsItem*> windowList = zv_plotter->zp_itemListForType(WindowItemType);
    //   ZWindowGraphicsItem* windowItem;
    //   for(int w = 0; w < windowList.count(); w++)
    //   {
    //      windowItem = qgraphicsitem_cast<ZWindowGraphicsItem*>(windowList.at(w));
    //      if(windowItem == 0)
    //      {
    //         continue;
    //      }
    //      windowItem->zp_updateWindowHeight();
    //   }
    //   zv_plotter->zp_updatePlot();

    //   //zh_setMaxParametersToDefaultItem((qreal)maxIntensity, (qreal)channelCount);
}
//===========================================================
void ZPlotterDataManager::zh_setPlotterVerticalAbsMax(qreal maxIntensity)
{
    zv_plotter->zp_setVerticalAbsMax((qreal)maxIntensity);

    // windows height recalc
    ZWindowGraphicsItem::zp_setTopAndButtonMargins(qAbs((qreal)maxIntensity)*-1.0, 0.0);
    QList<QGraphicsItem*> windowList = zv_plotter->zp_itemListForType(WindowItemType);
    ZWindowGraphicsItem* windowItem;
    for(int w = 0; w < windowList.count(); w++)
    {
        windowItem = qgraphicsitem_cast<ZWindowGraphicsItem*>(windowList.at(w));
        if(windowItem == nullptr)
        {
            continue;
        }
        windowItem->zp_updateItem();
    }
    zv_plotter->zp_updatePlot();
}
//===========================================================
void ZPlotterDataManager::zh_setMaxParametersToDefaultItem(qreal maxY, qreal maxX)
{
    QList<QGraphicsItem*>defaultItemList = zv_plotter->zp_itemListForType(DefaultRectItemType);
    if(defaultItemList.isEmpty())
    {
        return;
    }

    ZDefaultRectGraphicsItem* defaultItem = qgraphicsitem_cast<ZDefaultRectGraphicsItem*>(defaultItemList[0]);
    if(!defaultItem)
    {
        return;
    }

    QRectF rect = QRectF(QPointF(0.0, qAbs(maxY)*-1.0), QPointF(maxX, 0.0));
    if(rect.isEmpty())
    {
        return;
    }

    defaultItem->zp_fitItemInRect(rect);
}
//===========================================================
void ZPlotterDataManager::zh_definePlotScaling(bool& plotIsScaled)
{
    plotIsScaled = zv_plotter->zp_isPlotScaled();
}
//===========================================================
void ZPlotterDataManager::zh_findItemInCursorAreaImage(QImage cursorAreaImage)
{
    // central pixel
    int centralPointRow = cursorAreaImage.height()/2;
    int centralPointColumn = cursorAreaImage.width()/2;

    QPoint centralPixel = QPoint(centralPointColumn, centralPointRow);
    QColor centralPixelColor = cursorAreaImage.pixel(centralPixel);

    QList<QGraphicsItem*>itemList = zv_plotter->zp_itemListForType(SpectrumItemType);
    ZSpectrumGraphicsItem* spectrumItem;
    qint64 spectrumId = -1;// = spectrumItem;
    QColor spectrumColor;
    bool spectrumFound = false;
    // buffer map for futher search in peripheal points of image
    QMap<qint64, QColor> itemColorIdBufferMap;

    for(int i = 0; i < itemList.count(); i++)
    {
        spectrumItem = qgraphicsitem_cast<ZSpectrumGraphicsItem*>(itemList.at(i));
        if(spectrumItem == nullptr)
        {
            continue;
        }
        spectrumColor = spectrumItem->zp_spectrumColor();
        if(spectrumColor != centralPixelColor)
        {
            // set spe color and spe id in buffer map and continue
            itemColorIdBufferMap.insert( spectrumItem->zp_spectrumId(), spectrumColor);
            continue;
        }

        spectrumId = spectrumItem->zp_spectrumId();
        zv_spectrumArrayRepositiry->zp_setSpectrumCurrent(spectrumId);
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
                            zv_spectrumArrayRepositiry->zp_setSpectrumCurrent(spectrumId);
                            return;
                        }

                        if(it + 1 == itemColorIdBufferMap.end())
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
