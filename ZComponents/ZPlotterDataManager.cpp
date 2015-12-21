//===========================================================
#include "ZPlotterDataManager.h"
#include "ZPlotter.h"
#include "ZGraphicsItemUserTypes.h"
#include "ZSpectrumGraphicsItem.h"
#include "ZWindowGraphicsItem.h"

#include "globalVariables.h"
//===========================================================
ZPlotterDataManager::ZPlotterDataManager(QObject *parent) : QObject(parent)
{
    zv_spectrumArrayRepositiry = 0;
    zv_plotter = 0;
    zv_currentArrayIndex = -1;
    zv_switchRuleMetrixAction = 0;
    zv_currentArrayId = -1;
    zv_boundingRectTopFactor = 1.05;

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
    connect(repository, &ZSpectrumArrayRepository::zg_currentSpectrumOperation,
            this, &ZPlotterDataManager::zh_onRepositoryArrayOperation);
    connect(repository, &ZSpectrumArrayRepository::zg_energyCalibrationChanged,
            this, &ZPlotterDataManager::zh_changeEnergyCalibrationOnRule);
    connect(repository, &ZSpectrumArrayRepository::zg_arrayMaxParametersChanged,
            this, &ZPlotterDataManager::zh_onArrayMaxParametersChanged);
    connect(repository, &ZSpectrumArrayRepository::zg_requestIsPlotScaled,
            this, &ZPlotterDataManager::zh_definePlotScaling);

}
//===========================================================
void ZPlotterDataManager::zp_connectToPlotter(ZPlotter* plotter)
{
    zv_plotter = plotter;
    zv_plotter->zp_setAutoDefineVerticalAbsMax(false);
    QList<QAction*> actionList;
    actionList << zv_switchRuleMetrixAction;
    zv_plotter->zp_appendButtonsToDashboard(actionList, Qt::AlignLeft, 0);

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
}
//===========================================================
void ZPlotterDataManager::zp_currentArrayChanged(qint64 currentArrayId, int currentArrayIndex)
{
    if((zv_currentArrayIndex == currentArrayIndex && zv_currentArrayId == currentArrayId)
            || zv_plotter == 0 || zv_spectrumArrayRepositiry == 0)
    {
        return;
    }

    zv_currentArrayId = currentArrayId;
    zv_currentArrayIndex = currentArrayIndex;
    zv_plotter->zp_clearItemsForType(SpectrumItemType);

    qreal verticalMax = (qreal)zv_spectrumArrayRepositiry->zp_arrayMaxIntensity(zv_currentArrayIndex);
    zv_plotter->zp_setVerticalAbsMax(verticalMax * zv_boundingRectTopFactor);

    QList<ZAbstractSpectrum*> spectrumList = zv_spectrumArrayRepositiry->zp_spectrumListForArray(zv_currentArrayIndex);
    ZSpectrumGraphicsItem* spectrumItem;
    qreal distortionFactor;
    qreal distortionCorrectionFactor;
    zv_plotter->zp_verticalDistortionFactors(distortionFactor, distortionCorrectionFactor);
    for(int i = 0; i < spectrumList.count(); i++)
    {
        spectrumItem = new ZSpectrumGraphicsItem(spectrumList.at(i),
                                                 zv_boundingRectTopFactor,
                                                 distortionFactor,
                                                 distortionCorrectionFactor);
        zv_plotter->zp_addItem(spectrumItem);
    }

    zh_changeEnergyCalibrationOnRule(zv_currentArrayId);
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
#ifdef DBG
    qDebug() << "zh_onArrayMaxParametersChanged";
#endif
    if(arrayId != zv_currentArrayId || zv_plotter == 0)
    {
        return;
    }

    zv_plotter->zp_setVerticalAbsMax((qreal)maxIntensity);
}
//===========================================================
void ZPlotterDataManager::zh_definePlotScaling(bool& plotIsScaled)
{
    plotIsScaled = zv_plotter->zp_isPlotScaled();
}
//===========================================================
