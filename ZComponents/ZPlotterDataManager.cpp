//===========================================================
#include "ZPlotterDataManager.h"
#include "ZPlotter.h"
#include "ZGraphicsItemUserTypes.h"
#include "ZSpectrumGraphicsItem.h"
#include "ZWindowGraphicsItem.h"
#include "ZDefaultRectGraphicsItem.h"
#include "globalVariables.h"
//===========================================================
ZPlotterDataManager::ZPlotterDataManager(QObject *parent) : QObject(parent)
{
    zv_spectrumArrayRepositiry = 0;
    zv_calibrationRepository = 0;
    zv_plotter = 0;
    zv_currentArrayIndex = -1;
    zv_switchRuleMetrixAction = 0;
    zv_currentArrayId = -1;
    zv_boundingRectTopFactor = 1.05;

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
    connect(repository, &ZSpectrumArrayRepository::zg_energyCalibrationChanged,
            this, &ZPlotterDataManager::zh_changeEnergyCalibrationOnRule);
    connect(repository, &ZSpectrumArrayRepository::zg_arrayMaxParametersChanged,
            this, &ZPlotterDataManager::zh_onArrayMaxParametersChanged);
    connect(repository, &ZSpectrumArrayRepository::zg_requestIsPlotScaled,
            this, &ZPlotterDataManager::zh_definePlotScaling);
    connect(repository, &ZSpectrumArrayRepository::zg_currentArrayIdChanged,
            this, &ZPlotterDataManager::zp_currentArrayChanged);
    connect(repository, &ZSpectrumArrayRepository::zg_currentSpectrumChanged,
            this, &ZPlotterDataManager::zp_currentSpectrumChanged);


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
    zv_plotter->zp_appendButtonsToDashboard(actionList, Qt::AlignLeft, 0);

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

    if(zv_plotter->zp_itemCount() <= 0)
    {
        ZDefaultRectGraphicsItem* defaultItem = new ZDefaultRectGraphicsItem(zv_defaultSceneRect);
        zv_plotter->zp_addItem(defaultItem);
    }
}
//===========================================================
void ZPlotterDataManager::zp_currentCalibrationChanged(qint64 currentCalibrationId,
                                                       int currentCalibrationIndex)
{
    if((zv_currentCalibrationId == currentCalibrationId && zv_currentCalibrationIndex == currentCalibrationIndex)
            || zv_plotter == 0 || zv_calibrationRepository == 0)
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
        if(windowItem == 0)
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
            || zv_plotter == 0 || zv_spectrumArrayRepositiry == 0)
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

    zh_changeEnergyCalibrationOnRule(zv_currentArrayId);
    if(!isPlotScaled)
    {
        zv_plotter->zp_fitInBoundingRect();
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
void ZPlotterDataManager::zh_createComponents()
{
    zv_switchRuleMetrixAction = new QAction(this);
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
    if(zv_currentArrayIndex != arrayIndex || zv_plotter == 0)
    {
        return;
    }

    if(type == ZSpectrumArrayRepository::SOT_INSERT_SPECTRA)
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
        }
    }
    else if(type == ZSpectrumArrayRepository::SOT_REMOVE_SPECTRA)
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
    else if(type == ZSpectrumArrayRepository::SOT_DATA_CHANGED)
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
    if(zv_plotter == 0)
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
            if(windowItem == 0)
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
    else if(type == ZCalibrationRepository::WOT_WINDOW_CHANGED)
    {
        qint64 calibrationId;
        calibrationId = zv_calibrationRepository->zp_calibrationIdForCalibrationIndex(calibrationIndex);
        if(calibrationId < 0)
        {
            return;
        }

        ZWindowGraphicsItem* windowItem;
        QList<QGraphicsItem*>windowList = zv_plotter->zp_itemListForType(WindowItemType);

        for(int w = first; w <= last; w++ )
        {
            const ZCalibrationWindow* window = zv_calibrationRepository->zp_calibrationWindow(calibrationId, w);
            if(window == 0)
            {
                continue;
            }
            for(int i = 0; i < windowList.count(); i++)
            {
                windowItem = qgraphicsitem_cast<ZWindowGraphicsItem*>(windowList.value(i));
                if(windowItem == 0)
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

            zv_plotter->zp_addItem(windowItem);
        }
    }
}
//===========================================================
void ZPlotterDataManager::zh_switchRuleMetrix(bool toggled)
{
    if(zv_currentArrayIndex < 0 || !zv_spectrumArrayRepositiry || zv_plotter == 0 || !toggled)
    {
        zv_plotter->zp_setBottomMarkRecalcFlag(false);
        zv_plotter->zp_setTopMarkRecalcFlag(false);
        zv_plotter->zp_setHorizontalMarkRecalcFactors(zv_horizontalRuleLabel,
                                                      0, 1, 0);
        return;
    }

    qreal K0;
    qreal K1;
    qreal K2;
    QString energyUnit;
    if(!zv_spectrumArrayRepositiry->zp_energyCalibrationForArrayId(zv_currentArrayId,
                                                                   K0, K1, K2, energyUnit))
    {
        zv_plotter->zp_setBottomMarkRecalcFlag(false);
        zv_plotter->zp_setTopMarkRecalcFlag(false);
        zv_plotter->zp_setHorizontalMarkRecalcFactors(zv_horizontalRuleLabel,
                                                      0, 1, 0);
    }
    else
    {
        QString labelString = zv_horizontalRecalcedRuleLabel + " " + energyUnit;
        zv_plotter->zp_setBottomMarkRecalcFlag(true);
        zv_plotter->zp_setTopMarkRecalcFlag(true);
        zv_plotter->zp_setHorizontalMarkRecalcFactors(labelString,
                                                      K0, K1, K2);
    }
}
//===========================================================
void ZPlotterDataManager::zh_changeEnergyCalibrationOnRule(qint64 arrayId)
{
    if(!zv_switchRuleMetrixAction->isChecked() || zv_currentArrayId != arrayId)
    {
        return;
    }

    zh_switchRuleMetrix(zv_switchRuleMetrixAction->isChecked());
}
//===========================================================
void ZPlotterDataManager::zh_onArrayMaxParametersChanged(int arrayId, int maxIntensity, int channelCount)
{
    if(arrayId != zv_currentArrayId || zv_plotter == 0)
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
        if(windowItem == 0)
        {
            continue;
        }
        windowItem->zp_updateWindowHeight();
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

    defaultItem->zp_setSceneRect(rect);
}
//===========================================================
void ZPlotterDataManager::zh_definePlotScaling(bool& plotIsScaled)
{
    plotIsScaled = zv_plotter->zp_isPlotScaled();
}
//===========================================================
void ZPlotterDataManager::zh_findItemInCursorAreaImage(QImage cursorAreaImage)
{

    // TODO CURRENT SPECTRUM BY CLICK ON PLOT

#ifdef DBG
    qDebug() << "zh_findItemInCursorAreaImage";
#endif

    // central pixel
    //   QPoint centralPixel = QPoint(cursorAreaImage.width()/2, cursorAreaImage.height()/2);
    //   QColor centralPixelColor = cursorAreaImage.pixel(centralPixel);

    //QList<QGraphicsItem*>itemList = zv_plotter->zp_itemListForType();
    //   ZSpectrumGraphicsItem* spectrumItem;
    //   qint64 spectrumId = spectrumItem;
    //   QColor spectrumColor;

    //   for(int i = 0; i < spectrumItemList.count(); i++)
    //   {
    //      spectrumItem = qgraphicsitem_cast<ZSpectrumGraphicsItem*>(spectrumItemList.at(i));
    //      if(spectrumItem == 0)
    //      {
    //         continue;
    //      }
    //      spectrumId = spectrumItem->zp_spectrumId();
    //      spectrumColor = zv_;
    //      if()
    //      {

    //      }
    //   }


    //   //qDebug() << "central pixel" << QColor(cursorAreaImage->pixel(zv_colorPickUpAuxCoverageSize, zv_colorPickUpAuxCoverageSize));

    //   for(int row = 0; row < cursorAreaImage->size().height(); row++)
    //   {
    //      for(int col = 0; col < cursorAreaImage->size().width(); col++)
    //      {
    //         QColor currentColor(cursorAreaImage->pixel(row, col));
    //         if(currentColor != backgroundColor && currentColor != gridColor)
    //         {
    //            qDebug() << "pixel" << currentColor << "Row" << row << "Col" << col;
    //         }
    //      }
    //   }
}
//===========================================================
