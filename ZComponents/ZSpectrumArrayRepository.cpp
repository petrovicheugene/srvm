//==================================================================
#include "ZSpectrumArrayRepository.h"
#include "ZGeneral.h"
#include "ZFileActionManager.h"
#include "ZEnergyCalibrationDialogV2.h"

#include <math.h>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QtAlgorithms>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QMessageBox>
#include <QStringList>
#include <math.h>
//==================================================================
ZSpectrumArrayRepository::ZSpectrumArrayRepository(QObject *parent) : QObject(parent)
{
    zv_arrayFilePath = QString();
    zv_dirty = false;
    zv_defaultArrayBaseName = tr("Array #");
    zh_createActions();
    zh_createConnections();
}
//==================================================================
void ZSpectrumArrayRepository::zp_appendActionsToMenu(QMenu* menu) const
{
    if(menu->objectName() == "Edit")
    {
        //        QMenu* arrayMenu = new QMenu(tr("Spectrum arrays"));
        //        arrayMenu->setIcon(QIcon(glArrayIconString));
        //        arrayMenu->addAction(zv_appendArrayAction);
        //        arrayMenu->addAction(zv_removeArrayAction);
        //        menu->addMenu(arrayMenu);

        //        QMenu* spectraMenu = new QMenu(tr("Spectra"));
        //        spectraMenu->setIcon(QIcon(glSpectrumIconString));
        //        spectraMenu->addAction(zv_appendSpectrumToArrayAction);
        //        spectraMenu->addAction(zv_removeSpectrumFromArrayAction);
        //        menu->addMenu(spectraMenu);

        //        QMenu* elementMenu = new QMenu(tr("Chemical elements"));
        //        elementMenu->setIcon(QIcon(glElementIconString));
        //        elementMenu->addAction(zv_appendChemElementAction);
        //        elementMenu->addAction(zv_removeChemElementAction);
        //        menu->addMenu(elementMenu);

        // spectra array
        menu->addAction(zv_appendArrayAction);
        menu->addAction(zv_removeArrayAction);
        menu->addSeparator();

        // spectra
        menu->addAction(zv_appendSpectrumToArrayAction);
        menu->addAction(zv_removeSpectrumFromArrayAction);
        menu->addSeparator();
        menu->addAction(zv_copyConcentrationDataAction);
        menu->addAction(zv_pasteConcentrationDataAction);
        menu->addSeparator();
        menu->addAction(zv_clearConcentrationDataAction);
        menu->addSeparator();

        // chemical
        menu->addAction(zv_appendChemElementAction);
        menu->addAction(zv_removeChemElementAction);
    }
    else if(menu->objectName() == "Actions")
    {
        menu->addAction(zv_energyCalibrationAction);
        menu->addSeparator();
    }

}
//==================================================================
QList<QAction*> ZSpectrumArrayRepository::zp_arrayActions() const
{
    QList<QAction*> actionList;
    actionList << zv_appendArrayAction;
    actionList << zv_removeArrayAction;
    return actionList;
}
//==================================================================
QList<QAction*> ZSpectrumArrayRepository::zp_arrayContextMenuActions() const
{
    QList<QAction*> actionList;
    actionList << zv_appendArrayAction;
    actionList << zv_openArrayAction;
    actionList << zv_removeArrayAction;
    actionList << nullptr;
    actionList << zv_saveArrayAction;
    actionList << zv_saveAsArrayAction;

    return actionList;
}
//==================================================================
QList<QAction*> ZSpectrumArrayRepository::zp_spectrumActions() const
{
    QList<QAction*> actionList;
    actionList << zv_appendSpectrumToArrayAction;
    actionList << zv_removeSpectrumFromArrayAction;
    //    actionList << 0; // separator
    //    actionList << zv_copyConcentrationDataAction;
    //    actionList << zv_pasteConcentrationDataAction;
    //    actionList << 0; // separator
    //    actionList << zv_clearConcentrationDataAction;

    return actionList;
}
//==================================================================
QList<QAction*> ZSpectrumArrayRepository::zp_spectrumContextMenuActions() const
{
    QList<QAction*> actionList;
    actionList << zv_appendSpectrumToArrayAction;
    actionList << zv_removeSpectrumFromArrayAction;
    actionList << nullptr; // separator
    actionList << zv_copyConcentrationDataAction;
    actionList << zv_pasteConcentrationDataAction;
    actionList << nullptr; // separator
    actionList << zv_clearConcentrationDataAction;
    actionList << nullptr; // separator
    actionList << zv_setSpectraVisibleAction;
    actionList << zv_setSpectraInvisibleAction;
    actionList << zv_invertSpectraVisibilityAction;
    actionList << nullptr; // separator
    actionList << zv_setChemElementsVisibleAction;
    actionList << zv_setChemElementsInvisibleAction;
    actionList << zv_invertChemElementsVisibilityAction;
    actionList << nullptr; // separator
    actionList << zv_energyCalibrationAction;
    return actionList;
}
//==================================================================
QList<QAction*> ZSpectrumArrayRepository::zp_chemElementActions() const
{
    QList<QAction*> actionList;
    actionList << zv_appendChemElementAction;
    actionList << zv_removeChemElementAction;
    return actionList;
}
//==================================================================
QList<QAction*> ZSpectrumArrayRepository::zp_chemElementContextMenuActions() const
{
    QList<QAction*> actionList;
    actionList << zv_appendChemElementAction;
    actionList << zv_removeChemElementAction;
    actionList << nullptr; // separator
    actionList << zv_setChemElementsVisibleAction;
    actionList << zv_setChemElementsInvisibleAction;
    actionList << zv_invertChemElementsVisibilityAction;

    return actionList;
}
//==================================================================
void ZSpectrumArrayRepository::zp_connectToFileActionManager(ZFileActionManager* manager)
{
    manager->zp_initOpenSpectrumArrayAction(zv_openArrayAction);
    manager->zp_initSaveSpectrumArrayAction(zv_saveArrayAction);
    manager->zp_initSaveAsSpectrumArrayAction(zv_saveAsArrayAction);

    connect(manager, &ZFileActionManager::zg_spectrumRawArrayList,
            this, &ZSpectrumArrayRepository::zp_appendArrays);
    connect(manager, &ZFileActionManager::zg_spectrumFileListToOpen,
            this, &ZSpectrumArrayRepository::zp_appendSpectraToArray);
    connect(manager, &ZFileActionManager::zg_spectrumArraySaved,
            this, &ZSpectrumArrayRepository::zh_onSpectrumArraySaving);

    connect(this, &ZSpectrumArrayRepository::zg_initSpectraAppending,
            manager, &ZFileActionManager::zp_selectSpeFilesAndInitAppending);
    connect(manager, &ZFileActionManager::zg_requestRawArrayListAndInitSaving,
            this, &ZSpectrumArrayRepository::zh_createRawArrayListAndStartSaving);
    connect(this, &ZSpectrumArrayRepository::zg_saveSpectraArrayList,
            manager, &ZFileActionManager::zp_saveSpectraArrayListToFile);

    connect(this, &ZSpectrumArrayRepository::zg_arrayListDirtyChanged,
            manager, &ZFileActionManager::zp_onArrayListDirtyChange);

}
//==================================================================
bool ZSpectrumArrayRepository::zp_isEmpty() const
{
    return zv_arrayList.isEmpty();
}
//==================================================================
void ZSpectrumArrayRepository::zp_clear()
{
    if(!zv_arrayList.isEmpty())
    {
        emit zg_spectrumArrayOperation(AOT_REMOVE_ARRAYS, 0, zv_arrayList.count() - 1);
        for(int i = zv_arrayList.count()-1; i >= 0; i-- )
        {
            delete zv_arrayList.takeAt(i);
        }
        emit zg_spectrumArrayOperation(AOT_END_REMOVE_ARRAYS, 0, zv_arrayList.count() - 1);
    }

    zv_arrayFilePath.clear();
    zh_setDirty(false);
}
//==================================================================
bool ZSpectrumArrayRepository::zp_isDirty() const
{
    return zv_dirty;
}
//==================================================================
const ZSpectrumArray* ZSpectrumArrayRepository::zp_array(int arrayIndex)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return nullptr;
    }

    return zv_arrayList.at(arrayIndex);
}
//==================================================================
int ZSpectrumArrayRepository::zp_arrayCount() const
{
    return zv_arrayList.count();
}
//==================================================================
int ZSpectrumArrayRepository::zp_spectrumCount(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return 0;
    }

    return zv_arrayList.value(arrayIndex)->zp_spectrumCount();
}
//==================================================================
int ZSpectrumArrayRepository::zp_numberVisibleChemElementsBeforeIndex(int arrayIndex, int index) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return -1;
    }

    return zv_arrayList.value(arrayIndex)->zp_numberVisibleChemElementsBeforeIndex(index);
}
//==================================================================
int ZSpectrumArrayRepository::zp_chemElementCount(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return 0;
    }

    return zv_arrayList.value(arrayIndex)->zp_chemElementCount();
}
//==================================================================
int ZSpectrumArrayRepository::zp_visibleChemElementCount(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return 0;
    }

    return zv_arrayList.value(arrayIndex)->zp_visibleChemElementCount();
}
//==================================================================
QStringList ZSpectrumArrayRepository::zp_chemElementList(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return QStringList();
    }

    return zv_arrayList.value(arrayIndex)->zp_chemElementList();
}
//==================================================================
QString ZSpectrumArrayRepository::zp_visibleChemElementName(int arrayIndex, int visibleIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count()
            || visibleIndex < 0)
    {
        return QString();
    }

    return zv_arrayList.value(arrayIndex)->zp_visibleChemElementName(visibleIndex);
}
//==================================================================
QString ZSpectrumArrayRepository::zp_chemElementName(int arrayIndex, int row) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count()
            || row < 0)
    {
        return QString();
    }

    return zv_arrayList.value(arrayIndex)->zp_chemElementName(row);
}
//==================================================================
qint64 ZSpectrumArrayRepository::zp_visibleChemElementId(int arrayIndex, int visibleIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count()
            || visibleIndex < 0)
    {
        return -1;
    }

    return zv_arrayList.value(arrayIndex)->zp_visibleChemElementId(visibleIndex);
}
//==================================================================
qint64 ZSpectrumArrayRepository::zp_chemElementId(int arrayIndex, int row) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count()
            || row < 0)
    {
        return -1;
    }

    return zv_arrayList.value(arrayIndex)->zp_chemElementId(row);
}
//==================================================================
qint64 ZSpectrumArrayRepository::zp_chemElementIdForName(int arrayIndex, const QString& name) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() || name.isEmpty())
    {
        return -1;
    }

    //    for(int a = 0; a < zv_arrayList.count(); a++)
    //    {
    for(int c = 0; c < zv_arrayList.at(arrayIndex)->zp_chemElementCount(); c++)
    {
        if(zv_arrayList.at(arrayIndex)->zp_chemElementName(c) == name)
        {
            return zv_arrayList.at(arrayIndex)->zp_chemElementId(c);
        }
    }
    //    }

    return -1;
}
//==================================================================
QString ZSpectrumArrayRepository::zp_chemConcentration(int arrayIndex,
                                                       int spectrumIndex, int visibleChemElementIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return QString();
    }

    if(spectrumIndex < 0 || spectrumIndex >= zp_spectrumCount(arrayIndex))
    {
        return QString();
    }

    if(visibleChemElementIndex < 0 || visibleChemElementIndex >= zp_visibleChemElementCount(arrayIndex))
    {
        return QString();
    }

    qint64 chemElementId = zp_visibleChemElementId(arrayIndex, visibleChemElementIndex);

    return zv_arrayList.value(arrayIndex)->zp_chemConcentration(chemElementId, spectrumIndex);
}
//==================================================================
bool ZSpectrumArrayRepository::zp_setChemConcentration(int arrayIndex, int spectrumIndex,
                                                       int visibleChemElementIndex, const QString& concentration)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return false;
    }

    if(spectrumIndex < 0 || spectrumIndex >= zp_spectrumCount(arrayIndex))
    {
        return false;
    }

    if(visibleChemElementIndex < 0 || visibleChemElementIndex >= zp_visibleChemElementCount(arrayIndex))
    {
        return false;
    }

    qint64 chemElementId = zp_visibleChemElementId(arrayIndex, visibleChemElementIndex);
    bool res = zv_arrayList.value(arrayIndex)->zp_setChemConcentration(chemElementId, spectrumIndex, concentration);
    if(res)
    {
        //        int chemElementIndex = zh_chemElementIndexForId(arrayIndex, chemElementId);
        //        emit zg_chemElementOperation(CEOT_CHEM_ELEMENT_VALUE_CHANGED,
        //                                     arrayIndex, chemElementIndex, chemElementIndex);


        zh_setDirty(true);
    }

    return res;
}
//==================================================================
bool ZSpectrumArrayRepository::zp_setChemElementName(int arrayIndex, int chemElementIndex, const QString& chemElementName)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return false;
    }

    return zv_arrayList.at(arrayIndex)->zp_setChemElementName(chemElementIndex, chemElementName);
}
//==================================================================
//bool ZSpectrumArrayRepository::zp_energyCalibration(int arrayIndex, qreal& K0, qreal& K1, qreal& K2, QString& energyUnit)
//{
//    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
//    {
//        return false;
//    }

//    return zv_arrayList.at(arrayIndex)->zp_energyCalibration(K0, K1, K2, energyUnit);
//}
////==================================================================
//bool ZSpectrumArrayRepository::zp_energyCalibrationForArrayId(qint64 arrayId, qreal& K0, qreal& K1, qreal& K2, QString& energyUnit)
//{
//    for(int i = 0; i < zv_arrayList.count(); i++)
//    {
//        if(zv_arrayList.at(i)->zp_arrayId() == arrayId)
//        {
//            return zp_energyCalibration(i, K0, K1, K2, energyUnit);
//        }
//    }

//    return false;
//}
//==================================================================
int ZSpectrumArrayRepository::zp_gainFactor(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return -1;
    }

    return zv_arrayList.at(arrayIndex)->zp_gainFactor();
}
//==================================================================
bool ZSpectrumArrayRepository::zp_setGainFactor(int arrayIndex, int gainFactor)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return false;
    }

    bool res = zv_arrayList.at(arrayIndex)->zp_setGainFactor(gainFactor);
    if(res)
    {
        emit zg_spectrumArrayOperation(AOT_DATA_CHANGED, arrayIndex, arrayIndex);
        zh_setDirty(true);
    }

    return res;
}
//==================================================================
int ZSpectrumArrayRepository::zp_exposition(int arrayIndex)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return -1;
    }

    return zv_arrayList.at(arrayIndex)->zp_exposition();
}
//==================================================================
bool ZSpectrumArrayRepository::zp_expositionForArrayId(qint64 arrayId, int& exposition)
{
    for(int i = 0; i < zv_arrayList.count(); i++)
    {
        if(zv_arrayList.at(i)->zp_arrayId() == arrayId)
        {
            exposition = zp_exposition(i);
            return true;
        }
    }

    return false;
}
//==================================================================
qint64 ZSpectrumArrayRepository::zp_arrayIdForArrayIndex(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return -1;
    }

    return zv_arrayList.at(arrayIndex)->zp_arrayId();
}
//==================================================================
const ZAbstractSpectrum* ZSpectrumArrayRepository::zp_spectrumForId(qint64 id) const
{
    for(int a = 0; a < zv_arrayList.count(); a++)
    {
        for(int s = 0; s < zv_arrayList.at(a)->zp_spectrumCount(); s++)
        {
            if(zv_arrayList.at(a)->zp_spectrumId(s) == id)
            {
                return zv_arrayList.at(a)->zp_spectrum(s);
            }
        }
    }

    return nullptr;
}
//==================================================================
//bool ZSpectrumArrayRepository::zp_averageChemConcentration(int arrayIndex, const QString& chemElement, qreal& averageValue ) const
//{
//    return zv_arrayList.at(arrayIndex)->zp_averageChemConcentration(chemElement, averageValue);
//}
////==================================================================
//bool ZSpectrumArrayRepository::zp_averageChemConcentration(int arrayIndex, int chemElementindex, qreal& averageValue ) const
//{
//    return zv_arrayList.at(arrayIndex)->zp_averageChemConcentration(chemElementindex, averageValue);
//}
////==================================================================
//bool ZSpectrumArrayRepository::zp_averageChemConcentrationForChemElementId(int arrayIndex, qint64 chemElementId , qreal& averageValue ) const
//{
//    return zv_arrayList.at(arrayIndex)->zp_averageChemConcentrationForChemElementId(chemElementId, averageValue);
//}
//==================================================================
bool ZSpectrumArrayRepository::zp_chemElementIsVisible(int arrayIndex, int row) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count()
            || row < 0)
    {
        return false;
    }

    return zv_arrayList.value(arrayIndex)->zp_chemElementIsVisible(row);
}
//==================================================================
bool ZSpectrumArrayRepository::zp_setChemElementVisible(int arrayIndex, int row, bool visible) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count()
            || row < 0)
    {
        return false;
    }

    return zv_arrayList.value(arrayIndex)->zp_setChemElementVisible(row, visible);

}
//==================================================================
QString ZSpectrumArrayRepository::zp_arrayName(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return QString();
    }

    return zv_arrayList.value(arrayIndex)->zp_arrayName();
}
//==================================================================
bool ZSpectrumArrayRepository::zp_setSpectrumArrayName(int arrayIndex, const QString& name)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() || name.isEmpty())
    {
        return false;
    }

    zv_arrayList[arrayIndex]->zp_setArrayName(name);
    emit zg_spectrumArrayOperation(AOT_DATA_CHANGED, arrayIndex, arrayIndex);

    zh_setDirty(true);

    return true;
}
//==================================================================
QString ZSpectrumArrayRepository::zp_spectrumName(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return QString();
    }

    return zv_arrayList.value(arrayIndex)->zp_spectrumFileName(spectrumIndex);
}
//======================================================
qint64 ZSpectrumArrayRepository::zp_spectrumId(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return 0;
    }

    return zv_arrayList.value(arrayIndex)->zp_spectrumId(spectrumIndex);
}
//======================================================
QColor ZSpectrumArrayRepository::zp_spectrumColor(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return QColor();
    }

    return zv_arrayList.value(arrayIndex)->zp_spectrumColor(spectrumIndex);
}
//======================================================
bool ZSpectrumArrayRepository::zp_isSpectrumVisible(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return false;
    }

    return zv_arrayList.value(arrayIndex)->zp_isSpectrumVisible(spectrumIndex);
}
//======================================================
bool ZSpectrumArrayRepository::zp_setSpectrumVisible(int arrayIndex, int spectrumIndex, bool visible)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return false;
    }

    bool res = zv_arrayList.value(arrayIndex)->zp_setSpectrumVisible(spectrumIndex, visible);
    if(res)
    {
        emit zg_spectrumOperation(SOT_VISIBLE_CHANGED, arrayIndex, spectrumIndex, spectrumIndex);
    }
    return res;
}
//==================================================================
bool ZSpectrumArrayRepository::zp_isSpectrumVisible(qint64 arrayId, qint64 spectrumId) const
{
    for(int i = 0; i < zv_arrayList.count(); i++)
    {
        if(zv_arrayList.at(i)->zp_arrayId() != arrayId)
        {
            continue;
        }

        for(int s = 0; s < zv_arrayList.at(i)->zp_spectrumCount(); s++)
        {
            if(zv_arrayList.at(i)->zp_spectrumId(s) != spectrumId)
            {
                continue;
            }

            return zv_arrayList.at(i)->zp_isSpectrumVisible(s);
        }
        return false;
    }
    return false;
}
//==================================================================
bool ZSpectrumArrayRepository::zp_setSpectrumVisible(qint64 arrayId, qint64 spectrumId, bool visible)
{
    for(int i = 0; i < zv_arrayList.count(); i++)
    {
        if(zv_arrayList.at(i)->zp_arrayId() != arrayId)
        {
            continue;
        }

        for(int s = 0; s < zv_arrayList.at(i)->zp_spectrumCount(); s++)
        {
            if(zv_arrayList.at(i)->zp_spectrumId(s) != spectrumId)
            {
                continue;
            }

            return zv_arrayList.at(i)->zp_setSpectrumVisible(s, visible);
        }
        return false;
    }
    return false;
}
//==================================================================
bool ZSpectrumArrayRepository::zp_isSpectrumCheckedForId(qint64 arrayId, qint64 spectrumId) const
{
    for(int i = 0; i < zv_arrayList.count(); i++)
    {
        if(zv_arrayList.at(i)->zp_arrayId() != arrayId)
        {
            continue;
        }

        for(int s = 0; s < zv_arrayList.at(i)->zp_spectrumCount(); s++)
        {
            if(zv_arrayList.at(i)->zp_spectrumId(s) != spectrumId)
            {
                continue;
            }

            return zv_arrayList.at(i)->zp_isSpectrumChecked(s);
        }
        return false;
    }
    return false;
}
//==================================================================
bool ZSpectrumArrayRepository::zp_isSpectrumChecked(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return false;
    }
    return zv_arrayList.at(arrayIndex)->zp_isSpectrumChecked(spectrumIndex);
}
//==================================================================
bool ZSpectrumArrayRepository::zp_setSpectrumChecked(int arrayIndex, int spectrumIndex, bool checked)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return false;
    }

    bool res = zv_arrayList.at(arrayIndex)->zp_setSpectrumChecked(spectrumIndex, checked);
    if(res)
    {
        emit zg_spectrumOperation(SOT_CHECKED_CHANGED,  arrayIndex, spectrumIndex, spectrumIndex);
    }
    return res;
}
//==================================================================
bool ZSpectrumArrayRepository::zp_setSpectrumCheckedForId(qint64 arrayId, qint64 spectrumId, bool checked)
{
    for(int i = 0; i < zv_arrayList.count(); i++)
    {
        if(zv_arrayList.at(i)->zp_arrayId() != arrayId)
        {
            continue;
        }

        for(int s = 0; s < zv_arrayList.at(i)->zp_spectrumCount(); s++)
        {
            if(zv_arrayList.at(i)->zp_spectrumId(s) != spectrumId)
            {
                continue;
            }

            bool res = zv_arrayList.at(i)->zp_setSpectrumChecked(s, checked);
            if(res)
            {
                emit zg_spectrumOperation(SOT_CHECKED_CHANGED,  i, s, s);
            }

            return res;
        }
    }
    return false;
}
//==================================================================
void ZSpectrumArrayRepository::zp_setSpectrumCurrent(qint64 spectrumId) const
{
    if(spectrumId < 0)
    {
        return;
    }

    int currentArrayIndex = -1;
    emit zg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= zv_arrayList.count())
    {
        return;
    }

    for(int s = 0; s < zv_arrayList.at(currentArrayIndex)->zp_spectrumCount(); s++)
    {
        if(zv_arrayList.at(currentArrayIndex)->zp_spectrumId(s) == spectrumId)
        {
            emit zg_setCurrentSpectrumIndex(s);
            break;
        }
    }
}
//==================================================================
const ZAbstractSpectrum* ZSpectrumArrayRepository::zp_spectrum(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return nullptr;
    }

    return zv_arrayList.value(arrayIndex)->zp_spectrum(spectrumIndex);
}
//==================================================================
QList<ZAbstractSpectrum*> ZSpectrumArrayRepository::zp_spectrumListForArray(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return QList<ZAbstractSpectrum*>();
    }

    return zv_arrayList.value(arrayIndex)->zp_spectrumList();
}
//==================================================================
QList<quint32> ZSpectrumArrayRepository::zp_spectrumData(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return QList<quint32>();
    }

    return zv_arrayList.at(arrayIndex)->zp_spectrumData(spectrumIndex);
}
//==================================================================
int ZSpectrumArrayRepository::zp_arrayChannelCount(int arrayIndex)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return 0;
    }

    return zv_arrayList.value(arrayIndex)->zp_maxChannelCount();
}
//==================================================================
int ZSpectrumArrayRepository::zp_arrayMaxIntensity(int arrayIndex)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return 0;
    }

    return zv_arrayList.value(arrayIndex)->zp_maxIntensity();
}
//==================================================================
void ZSpectrumArrayRepository::zp_appendArrays(QString path, QList<ZRawSpectrumArray> rawArrayList)
{
    bool changePath = true;
    if(path == zv_arrayFilePath || (zv_arrayFilePath.isEmpty() && !zv_dirty))
    {
        zp_clear();
    }
    else
    {
        if(zv_dirty)
        {
            QString question;
            if(!zv_arrayFilePath.isEmpty())
            {
                question = tr("File \"%1\" has been changed. ").arg(zv_arrayFilePath);
            }
            question += tr("Do you want to save current array changes?");

            if(QMessageBox::question(nullptr, tr("Array appending"), question, QMessageBox::Yes, QMessageBox::No)
                    == QMessageBox::Yes)
            {
                // TODO SAVING ARRAY LIST TO FILE
                // zp_saveArrayListToFile();
            }
        }

        if(zv_arrayList.count() > 0)
        {
            QString question = tr("Clear current array list?");
            if(QMessageBox::question(nullptr, tr("Array appending"), question, QMessageBox::Yes, QMessageBox::No)
                    == QMessageBox::Yes)
            {
                zp_clear();
            }
            else
            {
                changePath = false;
                zh_setDirty(true);
            }
        }
    }

    foreach(ZRawSpectrumArray rawArray, rawArrayList)
    {
        zh_createArray(rawArray);
    }

    if(changePath)
    {
        zv_arrayFilePath = path;

    }


    emit zg_currentFile(zv_dirty, zv_arrayFilePath);
    emit zg_fitPlotInBoundingRect();
    zh_actionEnablingControl();
}
//==================================================================
void ZSpectrumArrayRepository::zp_appendSpectraToArray(int arrayIndex, QStringList fileNameList)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return;
    }

    int spectraStartCount = zv_arrayList.at(arrayIndex)->zp_spectrumCount();
    bool dontAsk = false;
    bool continueAppend = true;
    for(int i = 0; i < fileNameList.count(); i++)
    {
        QFileInfo fileInfo(fileNameList.at(i));
        if(!fileInfo.exists())
        {
            QString msg = tr("File \"%1\" does not exist!").arg(zv_arrayList.at(arrayIndex)->zp_arrayName());
            emit zg_message(msg);
            qCritical().noquote() <<  msg;
            continue;
        }

        if(fileInfo.isDir())
        {
            QString msg = tr("\"%1\" is a folder!").arg(zv_arrayList.at(arrayIndex)->zp_arrayName());
            emit zg_message(msg);
            qCritical().noquote() <<  msg;
            continue;
        }

        ZRawSpectrum rawSpectrum;
        rawSpectrum.path = fileNameList.at(i);

        // int spectrumIndex = zv_arrayList[arrayIndex]->zp_spectrumCount();
        // emit zg_currentSpectrumOperation(SOT_INSERT_SPECTRA, arrayIndex, spectrumIndex, spectrumIndex);
        if(!zv_arrayList[arrayIndex]->zp_appendSpectrum(rawSpectrum, fileNameList.count()-1 == i, dontAsk, continueAppend))
        {

        }

        if(!continueAppend)
        {
            break;
        }

        // emit zg_currentSpectrumOperation(SOT_END_INSERT_SPECTRA, arrayIndex, spectrumIndex, spectrumIndex);
    }

    int loaded = zv_arrayList.at(arrayIndex)->zp_spectrumCount() - spectraStartCount;
    if(loaded > 0)
    {
        QString spectrumString = loaded > 1? tr("spectra") : tr("spectrum");
        QString msg = tr("%1 %2 loaded", "", 3).arg(QString::number(loaded), spectrumString);
        emit zg_message(msg);
        qInfo().noquote() <<  msg;

        if(spectraStartCount < 1)
        {
            emit zg_fitPlotInBoundingRect();
        }
        else
        {
            bool plotIsScaled;
            emit zg_requestIsPlotScaled(plotIsScaled);
            if(!plotIsScaled)
            {
                emit zg_fitPlotInBoundingRect();
            }
        }

        zh_setDirty(true);

    }
    zh_actionEnablingControl();

    int currentArrayIndex;
    emit zg_requestCurrentArrayIndex(currentArrayIndex);
    if(arrayIndex == currentArrayIndex)
    {

    }
}
//==================================================================
void ZSpectrumArrayRepository::zh_createArray(const ZRawSpectrumArray& rawArray)
{
    int nextArrayIndex = zv_arrayList.count();
    emit zg_spectrumArrayOperation(AOT_INSERT_ARRAYS, nextArrayIndex, nextArrayIndex);

    ZSpectrumArray* array = new ZSpectrumArray(rawArray.name, this);
    array->zp_setGainFactor(rawArray.gainFactor);
    zv_arrayList << array;

    connect(array, &ZSpectrumArray::zg_message,
            this, &ZSpectrumArrayRepository::zg_message);
    connect(array, &ZSpectrumArray::zg_chemElementOperation,
            this, &ZSpectrumArrayRepository::zh_onChemElementOperation);
    connect(array, &ZSpectrumArray::zg_spectrumOperation,
            this, &ZSpectrumArrayRepository::zh_onSpectrumOperation);
    connect(array, &ZSpectrumArray::zg_arrayMaxParametersChanged,
            this, &ZSpectrumArrayRepository::zg_arrayMaxParametersChanged);

    bool dontAsk = false;
    bool continueAppend = true;
    for(int s = 0; s < rawArray.spectrumList.count(); s++)
    {
        if(!array->zp_appendSpectrum(rawArray.spectrumList.value(s), rawArray.spectrumList.count()-1 == s, dontAsk, continueAppend))
        {

        }

        if(!continueAppend)
        {
            break;
        }
    }

    emit zg_fitPlotInBoundingRect();
    emit zg_spectrumArrayOperation(AOT_END_INSERT_ARRAYS, nextArrayIndex, nextArrayIndex);
}
//==================================================================
bool ZSpectrumArrayRepository::zh_removeArray(int arrayIndex)
{
    int nextCurrentIndex;
    if(arrayIndex == zv_arrayList.count() - 1 && zv_arrayList.count() > 1)
    {
        nextCurrentIndex = zv_arrayList.count() - 2;
    }
    else if(arrayIndex == zv_arrayList.count() - 1 && zv_arrayList.count() == 1)
    {
        nextCurrentIndex = -1;
    }
    else
    {
        nextCurrentIndex = arrayIndex;
    }

    emit zg_spectrumArrayOperation(AOT_REMOVE_ARRAYS, arrayIndex, arrayIndex);

    delete zv_arrayList.at(arrayIndex);
    zv_arrayList.removeAt(arrayIndex);

    emit zg_spectrumArrayOperation(AOT_END_REMOVE_ARRAYS, arrayIndex, arrayIndex);
    emit zg_setCurrentArrayIndex(nextCurrentIndex);
    return true;
}
//==================================================================
bool ZSpectrumArrayRepository::zh_removeSpectrum(int arrayIndex, int spectrumIndex)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return false;
    }

    if(spectrumIndex < 0 || spectrumIndex >= zv_arrayList.at(arrayIndex)->zp_spectrumCount())
    {
        return false;
    }

    return zv_arrayList[arrayIndex]->zp_removeSpectrum(spectrumIndex);
}
//==================================================================
bool ZSpectrumArrayRepository::zh_removeChemicalElement(int arrayIndex, int elementIndex)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return false;
    }

    if(elementIndex < 0 || elementIndex >= zv_arrayList.at(arrayIndex)->zp_chemElementCount())
    {
        return false;
    }

    bool res = zv_arrayList[arrayIndex]->zp_removeChemElement(elementIndex);
    if(res)
    {
        zh_setDirty(true);
    }
    return  res;
}
//==================================================================
void ZSpectrumArrayRepository::zp_getArrayCount(int& arrayCount) const
{
    arrayCount = zv_arrayList.count();
}
//==================================================================
void ZSpectrumArrayRepository::zp_getSpectrumCount(int arrayIndex, int& spectrumCount) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        spectrumCount = -1;
        return;
    }

    spectrumCount = zv_arrayList.value(arrayIndex)->zp_spectrumCount();
}
//==================================================================
void ZSpectrumArrayRepository::zp_getArrayName(int arrayIndex, QString& arrayName) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        arrayName = QString();
        return;
    }

    arrayName = zv_arrayList.value(arrayIndex)->zp_arrayName();
}
//==================================================================
void ZSpectrumArrayRepository::zp_getSpectrumName(int arrayIndex, int spectrumIndex, QString& spectrumName) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        spectrumName = QString();
        return;
    }

    if(spectrumIndex < 0 || spectrumIndex >= zv_arrayList.value(arrayIndex)->zp_spectrumCount())
    {
        spectrumName = QString();
        return;
    }

    spectrumName = zv_arrayList.value(arrayIndex)->zp_spectrumFileName(spectrumIndex);
}
//==================================================================
void ZSpectrumArrayRepository::zp_currentArrayChanged(int current, int previous)
{
    qint64 arrayId;
    if(current < 0 || current >= zv_arrayList.count() )
    {
        arrayId = -1;
    }
    else
    {
        arrayId = zv_arrayList.at(current)->zp_arrayId();
    }

    zv_pasteConcentrationDataAction->setEnabled(current >= 0 && zv_pasteData.zp_isValid());

    emit zg_currentArrayIsAboutChange(arrayId, current);
    emit zg_currentArrayIdChanged(arrayId, current);

    zh_actionEnablingControl();
}
//==================================================================
void ZSpectrumArrayRepository::zp_currentSpectrumChanged(int currentSpectrumIndex, int previousSpectrumIndex)
{
    int currentArrayIndex;
    qint64 currentSpectrumId;
    qint64 previousSpectrumId;
    QList<double> energyCalibration;
    QString energyUnit;

    emit zg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= zv_arrayList.count())
    {
        currentSpectrumId = -1;
        previousSpectrumId = -1;
    }
    else
    {
        if(currentSpectrumIndex < 0 || currentSpectrumIndex >= zv_arrayList.at(currentArrayIndex)->zp_spectrumCount())
        {
            currentSpectrumId = -1;
        }
        else
        {
            currentSpectrumId = zv_arrayList.at(currentArrayIndex)->zp_spectrumId(currentSpectrumIndex);
            zv_arrayList.at(currentArrayIndex)->zp_energyCalibration(currentSpectrumIndex, energyCalibration, energyUnit);
        }

        if(previousSpectrumIndex < 0 || previousSpectrumIndex >= zv_arrayList.at(currentArrayIndex)->zp_spectrumCount())
        {
            previousSpectrumId = -1;
        }
        else
        {
            previousSpectrumId = zv_arrayList.at(currentArrayIndex)->zp_spectrumId(previousSpectrumIndex);
        }
    }

    emit zg_currentSpectrumChanged(currentSpectrumId, currentSpectrumIndex, previousSpectrumId, previousSpectrumIndex);
    emit zg_energyCalibrationChanged(energyCalibration);
}
//==================================================================
void ZSpectrumArrayRepository::zp_chemElementListForCurrentArray(QStringList& chemElementList)
{
    int currentArray;
    emit zg_requestCurrentArrayIndex(currentArray);
    chemElementList = zp_chemElementList(currentArray);
}
//==================================================================
void ZSpectrumArrayRepository::zp_onSelectionSpectraChange(bool selectionEnabled,
                                                           bool concentrationSelected,
                                                           bool spectrumSelected)
{
    // zh_actionEnablingControl();
    zv_removeSpectrumFromArrayAction->setEnabled(selectionEnabled);
    zv_energyCalibrationAction->setEnabled(spectrumSelected);

    zv_copyConcentrationDataAction->setEnabled(selectionEnabled);
    zv_clearConcentrationDataAction->setEnabled(concentrationSelected);
}
//==================================================================
void ZSpectrumArrayRepository::zp_currentSpectrumWindowIntensity(int firstChannel,
                                                                 int lastChennel, qreal& intensity) const
{
    intensity = sqrt(-1); // NAN value

    // get current spectrum array
    int currentArrayIndex = -1;
    emit zg_requestCurrentArrayIndex(currentArrayIndex);

    if(currentArrayIndex < 0 || currentArrayIndex >= zv_arrayList.count())
    {
        // RETURN ERROR
        return;
    }

    int currentSpectrumIndex = -1;
    bool ok = false;
    emit zg_requestCurrentSpectrumRow(currentSpectrumIndex, &ok);

    if(!ok || currentSpectrumIndex < 0 || currentSpectrumIndex >= zv_arrayList.at(currentArrayIndex)->zp_spectrumCount())
    {
        // RETURN ERROR
        return;
    }


    const ZAbstractSpectrum* spectrum =  zv_arrayList.at(currentArrayIndex)->zp_spectrum(currentSpectrumIndex);
    if(!spectrum)
    {
        // RETURN ERROR
        return;
    }

    spectrum->zp_intensityInWindow(firstChannel, lastChennel, intensity);
}
//==================================================================
void ZSpectrumArrayRepository::zh_onSpectrumArraySaving(QString filePath)
{
    zv_arrayFilePath = filePath;
    zh_setDirty(false);
}
//==================================================================
void ZSpectrumArrayRepository::zh_onAppendArrayAction()
{
    ZRawSpectrumArray rawArray;
    // new array name
    int maxArrayNumber = 1;
    for(int i = 0; i < zv_arrayList.count(); i++)
    {
        QString name = zv_arrayList.at(i)->zp_arrayName();
        if(name.startsWith(zv_defaultArrayBaseName))
        {
            QString digitalPart = name.mid(zv_defaultArrayBaseName.count());
            bool ok;
            int number = digitalPart.toInt(&ok);
            if(!ok)
            {
                continue;
            }
            if(number >= maxArrayNumber)
            {
                maxArrayNumber = ++number;
            }
        }
    }

    rawArray.name = zv_defaultArrayBaseName + QString::number(maxArrayNumber);

    zh_createArray(rawArray);
    zh_setDirty(true);

    emit zg_setCurrentArrayIndex(zv_arrayList.count() - 1);
    emit zg_startCurrentArrayEdition();
    zh_actionEnablingControl();

}
//==================================================================
void ZSpectrumArrayRepository::zh_onRemoveArrayAction()
{
    int currentArrayIndex = -1;
    emit zg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= zv_arrayList.count())
    {
        return;
    }

    QString question = tr("Do you want to remove array \"%1\" from the list?").arg(zv_arrayList.value(currentArrayIndex)->zp_arrayName());
    if(QMessageBox::question(nullptr, tr("Array removing"), question, QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::Yes)
    {
        bool res = zh_removeArray(currentArrayIndex);
        if(res)
        {
            if(zv_arrayList.isEmpty())
            {
                zh_setDirty(false);
                zv_arrayFilePath = QString();
            }
            else
            {
                zh_setDirty(true);
            }
        }
    }
    zh_actionEnablingControl();

}
//==================================================================
void ZSpectrumArrayRepository::zh_onAppendSpectraToArrayAction()
{
    if(zv_arrayList.count() < 1)
    {
        // a new array creation
        zv_appendArrayAction->trigger();
    }

    int currentArrayIndex;
    emit zg_requestCurrentArrayIndex(currentArrayIndex);

    if(currentArrayIndex < 0 || currentArrayIndex >= zv_arrayList.count())
    {
        QString string = tr("Select an array you want to append spectra to!");
        QMessageBox::critical(nullptr, tr("Spectra appending"), string, QMessageBox::Ok);
        return;
    }

    emit zg_initSpectraAppending(currentArrayIndex);
    zh_actionEnablingControl();

}
//==================================================================
void ZSpectrumArrayRepository::zh_onRemoveSpectrumFromArrayAction()
{
    QList<int> selectedSpectrumList;
    emit zg_requestSelectedSpectrumIndexList(selectedSpectrumList);

    if(selectedSpectrumList.isEmpty())
    {
        QString string = tr("There is no spectra to remove!");
        QMessageBox::critical(nullptr, tr("Spectra removing"), string, QMessageBox::Ok);
        return;
    }

    int currentArrayIndex;
    emit zg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= zv_arrayList.count())
    {
        return;
    }

    QString question = tr("Do you want to remove the selected spectra from the list?");
    if(QMessageBox::question(nullptr, tr("Spectra removing"), question, QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::No)
    {
        return;
    }

    //qSort(selectedSpectrumList);
    std::sort(selectedSpectrumList.begin(), selectedSpectrumList.end());

    bool res = false;
    for(int i = selectedSpectrumList.count() - 1; i >= 0; i--)
    {
        res = zh_removeSpectrum(currentArrayIndex, selectedSpectrumList.value(i)) || res;
    }

    if(res)
    {
        bool plotIsScaled;
        emit zg_requestIsPlotScaled(plotIsScaled);
        if(!plotIsScaled)
        {
            emit zg_fitPlotInBoundingRect();
        }

        if(zv_arrayList.isEmpty())
        {
            zh_setDirty(false);

            zv_arrayFilePath = QString();
        }
        else
        {
            zh_setDirty(true);
        }
    }
    zh_actionEnablingControl();

}
//==================================================================
void ZSpectrumArrayRepository::zh_onAppendChemElementAction()
{
    int currentArrayIndex = -1;
    emit zg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= zv_arrayList.count())
    {
        return;
    }

    if(zv_arrayList[currentArrayIndex]->zp_appendNewChemElement())
    {
        zh_setDirty(true);

        emit zg_setCurrentChemElementIndex(zv_arrayList.at(currentArrayIndex)->zp_chemElementCount() - 1);
        emit zg_startCurrentChemElementEdition();
    }
    zh_actionEnablingControl();

}
//==================================================================
void ZSpectrumArrayRepository::zh_onRemoveChemElementAction()
{
    QList<int> selectedChemElementList;
    emit zg_requestSelectedChemElementIndexList(selectedChemElementList);

    if(selectedChemElementList.isEmpty())
    {
        QString string = tr("There is no chemical elements to remove!");
        QMessageBox::critical(nullptr, tr("Chemical element removing"), string, QMessageBox::Ok);
        return;
    }

    int currentArrayIndex;
    emit zg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= zv_arrayList.count())
    {
        return;
    }

    QString question = tr("Do you want to remove the selected chemical elemnts from the list?");
    if(QMessageBox::question(nullptr, tr("Chemical elements removing"), question, QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::No)
    {
        return;
    }

    //qSort(selectedChemElementList);
    std::sort(selectedChemElementList.begin(), selectedChemElementList.end());
    bool res = false;

    for(int i = selectedChemElementList.count() - 1; i >= 0; i--)
    {
        res = zh_removeChemicalElement(currentArrayIndex, selectedChemElementList.value(i)) || res;
    }

    zh_actionEnablingControl();
}
//==================================================================
void ZSpectrumArrayRepository::zh_onChemElementOperation(ZChemElementList::OperationType type, int first, int last)
{
    QObject* signalSender = sender();
    if(signalSender == nullptr)
    {
        return;
    }

    for(int i = 0; i < zv_arrayList.count(); i++)
    {
        if(zv_arrayList.value(i) == signalSender)
        {
            ChemElementOperationType chemElementOperationType;
            if(type == ZChemElementList::OT_INSERT_CHEM_ELEMENT)
            {
                chemElementOperationType = CEOT_INSERT_CHEM_ELEMENT;
            }
            else if(type == ZChemElementList::OT_END_INSERT_CHEM_ELEMENT)
            {
                chemElementOperationType = CEOT_END_INSERT_CHEM_ELEMENT;
            }
            else if(type == ZChemElementList::OT_REMOVE_CHEM_ELEMENT)
            {
                chemElementOperationType = CEOT_REMOVE_CHEM_ELEMENT;
            }
            else if(type == ZChemElementList::OT_END_REMOVE_CHEM_ELEMENT)
            {
                chemElementOperationType = CEOT_END_REMOVE_CHEM_ELEMENT;
            }
            else if(type == ZChemElementList::OT_CHANGED)
            {
                chemElementOperationType = CEOT_CHEM_ELEMENT_CHANGED;
            }
            else if(type == ZChemElementList::OT_VISIBLE_CHANGE)
            {
                chemElementOperationType = CEOT_CHEM_ELEMENT_VISIBILITY_CHANGE;
            }
            else if(type == ZChemElementList::OT_END_VISIBLE_CHANGE)
            {
                chemElementOperationType = CEOT_END_CHEM_ELEMENT_VISIBILITY_CHANGE;
            }
            else
            {
                return;
            }

            emit zg_chemElementOperation(chemElementOperationType, i, first, last);
            return;
        }
    }
}
//==================================================================
void ZSpectrumArrayRepository::zh_createRawArrayListAndStartSaving(QString& filePath, bool& res) const
{
    if(filePath.isEmpty())
    {
        filePath = zv_arrayFilePath;
    }

    QList<ZRawSpectrumArray> rawSpectrumArrayList = zh_createRawArrayList();
    emit zg_saveSpectraArrayList(filePath, rawSpectrumArrayList, res);
    // TODO after saving FileActionManager must send back a signal for reset dirty flag
    // TODO signal from fileActionManager with saved filePath. zv_path have to be canged
}
//==================================================================
void ZSpectrumArrayRepository::zh_onSpectrumOperation(ZSpectrumArray::OperationType type, int first, int last) const
{
    if(!sender())
    {
        return;
    }
    ZSpectrumArray* array = static_cast<ZSpectrumArray*>(sender());
    for(int arrayIndex = 0; arrayIndex < zv_arrayList.count(); arrayIndex++)
    {
        if(zv_arrayList.at(arrayIndex) != array)
        {
            continue;
        }

        if(type == ZSpectrumArray::OT_INSERT_SPECTRA)
        {
            emit zg_spectrumOperation(SOT_BEGIN_INSERT_SPECTRA, arrayIndex, first, last);
        }
        else if(type == ZSpectrumArray::OT_END_INSERT_SPECTRA)
        {
            emit zg_spectrumOperation(SOT_END_INSERT_SPECTRA, arrayIndex, first, last);
        }
        else if(type == ZSpectrumArray::OT_REMOVE_SPECTRA)
        {
            emit zg_spectrumOperation(SOT_BEGIN_REMOVE_SPECTRA, arrayIndex, first, last);
        }
        else if(type == ZSpectrumArray::OT_END_REMOVE_SPECTRA)
        {
            emit zg_spectrumOperation(SOT_END_REMOVE_SPECTRA, arrayIndex, first, last);
        }
        else if(type == ZSpectrumArray::OT_CONCENTRATION_CHANGED)
        {
            emit zg_spectrumOperation(SOT_CONCENTRATION_CHANGED, arrayIndex, first, last);
        }
        else if(type == ZSpectrumArray::OT_SPECTRUM_ENERGY_CALIBRATION_CHANGED)
        {
            // if calling spectrum is current then get energy calibration and emit signal

            // define current spectrum
            int currentArrayIndex = -1;
            emit zg_requestCurrentArrayIndex(currentArrayIndex);

            if(currentArrayIndex >= 0 && currentArrayIndex < zv_arrayList.count() &&
                    currentArrayIndex == arrayIndex)
            {
                // get current spectrum
                int currentRow = -1;
                bool ok = false;
                emit zg_requestCurrentSpectrumRow(currentRow, &ok);

                if(ok && currentRow >= first && currentRow <= last)
                {
                    QList<double> energyCalibration;
                    QString energyUnit;
                    array->zp_energyCalibration(currentRow, energyCalibration, energyUnit);
                    emit zg_energyCalibrationChanged(energyCalibration);
                }
            }
        }

        break;
    }
}
//==================================================================
void ZSpectrumArrayRepository::zh_onClipboardContentChange()
{
    zv_pasteData.zp_reset();

    QClipboard* clipboard = qApp->clipboard();
    if(!clipboard->mimeData()->hasText())
    {
        zv_pasteConcentrationDataAction->setEnabled(false);
        return;
    }

    QString sourceString = clipboard->text();

    int currentArrayIndex = -1;
    emit zg_requestCurrentArrayIndex(currentArrayIndex);
    ZSpectrumArray* currentArray = zv_arrayList.value(currentArrayIndex);
    QStringList chemElementList;
    if(currentArray)
    {
        chemElementList = currentArray->zp_chemElementList();
    }

    if(!zv_pasteData.zp_loadData(sourceString, chemElementList))
    {
        zv_pasteConcentrationDataAction->setEnabled(false);
        return;
    }

    zv_pasteConcentrationDataAction->setEnabled(currentArrayIndex >= 0 && currentArrayIndex < zv_arrayList.count());
    return;
}
//==================================================================
void ZSpectrumArrayRepository::zh_onPasteConcentrationDataAction()
{
    if(!zv_pasteData.zp_isValid())
    {
        zv_pasteConcentrationDataAction->setEnabled(false);
        return;
    }

    int currentArrayIndex = -1;
    emit zg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= zv_arrayList.count())
    {
        QString warning = tr("Select a spectrum array for data pasting.");
        QMessageBox::warning(nullptr, zv_messageBoxPasteTitle, warning, QMessageBox::Ok);
        return;
    }

    ZSpectrumArray* currentArray = zv_arrayList.value(currentArrayIndex);
    if(!currentArray)
    {
        return;
    }

    if(zv_pasteData.zp_isHeaderListValid())
    {
        // Headers exist. fill every chemElement from first spectrum
        // check chemElements in pastingData
        QStringList chemElementList = currentArray->zp_chemElementList();
        QStringList absentChemElementList = zv_pasteData.zp_absentHeaderList(chemElementList);

        if(!absentChemElementList.isEmpty())
        {
            QString absentChemElementString;
            for(int i = 0; i < absentChemElementList.count(); i++)
            {
                absentChemElementString += absentChemElementList.at(i);
                if(i < absentChemElementList.count() - 1)
                {
                    absentChemElementString += ", ";
                }
            }
            absentChemElementString += ".";
            QString question = tr("In the pasting data are folowing chemical elements not presented in current array:\n"
                                  "%1\n"
                                  "Do you want to create new chemical elements?").arg(absentChemElementString);
            if(QMessageBox::question(nullptr, zv_messageBoxPasteTitle, question, QMessageBox::Yes | QMessageBox::No)
                    == QMessageBox::Yes)
            {
                // create new chem elements
                foreach(QString str, absentChemElementList)
                {
                    currentArray->zp_appendNewChemElement(str);
                }
            }
        } // end if(!absentChemElementList.isEmpty())

        // check dimension
        QString question;
        if(zv_pasteData.zp_dataRowCount() > currentArray->zp_spectrumCount())
        {
            question = tr("The number of pasting data row exceeds the number of spectrum in the array.");
        }
        else if(zv_pasteData.zp_dataRowCount() < currentArray->zp_spectrumCount())
        {
            question = tr("The number of spectrum in the array exceeds the number of pasting data.");
        }

        if(!question.isEmpty())
        {
            question += "\nContinue anyway?";
            if(QMessageBox::question(nullptr, zv_messageBoxPasteTitle, question, QMessageBox::Yes | QMessageBox::No)
                    == QMessageBox::No)
            {
                return;
            }
        }

        // paste data
        qint64 chemElementId;
        QStringList concentrationColumn;
        QString header;
        int chemElementIndex;
        enum AnswerResult {AR_NOT_DEFINED, AR_YES, AR_NO} answerResult = AR_NOT_DEFINED;
        for(int col = 0; col < zv_pasteData.zp_columnCount(); col++)
        {
            header = zv_pasteData.zp_columnHeader(col);
            chemElementIndex = currentArray->zp_chemElementIndexForName(header);
            chemElementId = currentArray->zp_chemElementId(chemElementIndex);

            if(chemElementId < 0 || chemElementIndex < 0 || chemElementIndex >= currentArray->zp_chemElementCount())
            {
                continue;
            }

            if(!currentArray->zp_chemElementIsVisible(chemElementIndex) && answerResult != AR_YES)
            {
                if(answerResult == AR_NOT_DEFINED)
                {
                    QString question = tr("There are hidden chemical element columns.\n"
                                          "Do you want to paste data to them?");
                    int res = QMessageBox::question(nullptr, zv_messageBoxPasteTitle, question,
                                                    QMessageBox::Yes | QMessageBox::No);
                    switch(res)
                    {
                    case QMessageBox::Yes:
                        answerResult = AR_YES;
                        break;
                    case QMessageBox::No:
                        answerResult = AR_NO;
                        continue;
                    }
                }
                else if(answerResult == AR_NO)
                {
                    continue;
                }
            }

            concentrationColumn = zv_pasteData.zp_columnData(col);
            for(int s = 0; (s < currentArray->zp_spectrumCount() && s < concentrationColumn.count()); s++)
            {
                currentArray->zp_setChemConcentration(chemElementId, s, concentrationColumn.at(s));
            }
        }
    }
    else
    {
        // data only. fill from current cell

        // get start cell to paste
        int row = -1;
        int column = -1;
        emit zg_requestCurrentChemConcentrationCellIndex(row, column);
        if(row < 0 || row >= currentArray->zp_spectrumCount()
                || column < 0 || column >= currentArray->zp_visibleChemElementCount())
        {
            QString warning = tr("Select a top left cell in a chemical concentration column for data pasting.");
            QMessageBox::warning(nullptr, zv_messageBoxPasteTitle, warning, QMessageBox::Ok);
            return;
        }

        // check dimension
        QString question;
        if(zv_pasteData.zp_dataRowCount() > currentArray->zp_spectrumCount() - row)
        {
            question = tr("The number of inserted rows exceeds the number of spectra.");
        }

        if(zv_pasteData.zp_columnCount() > currentArray->zp_visibleChemElementCount() - column)
        {
            if(question.isEmpty())
            {
                question = tr("The number of inserted columns exceeds the number of visible chemical elements.");
            }
            else
            {
                question = tr("The dimension of inserted data exceeds the numbers of spectra and visible chemical elements.");
            }
        }

        if(!question.isEmpty())
        {
            question += "\nContinue anyway?";
            if(QMessageBox::question(nullptr, zv_messageBoxPasteTitle, question, QMessageBox::Yes | QMessageBox::No)
                    == QMessageBox::No)
            {
                return;
            }
        }

        // create chemElementIdList that corresponding with pastingData columns
        QList<qint64> chemElementIdList;
        for(int col = column; col < currentArray->zp_visibleChemElementCount(); col++)
        {
            chemElementIdList.append(currentArray->zp_visibleChemElementId(col));
        }

        // paste data
        qint64 chemElementId;
        QStringList concentrationColumn;
        for(int col = 0; col < zv_pasteData.zp_columnCount(); col++)
        {
            if(col >= chemElementIdList.count())
            {
                return;
            }
            chemElementId = chemElementIdList.at(col);
            concentrationColumn = zv_pasteData.zp_columnData(col);

            for(int s = row, cr = 0; (s < currentArray->zp_spectrumCount() && cr < concentrationColumn.count()); s++, cr++)
            {
                currentArray->zp_setChemConcentration(chemElementId, s, concentrationColumn.at(cr));
            }
        } // end pasting
    }
}
//==================================================================
void ZSpectrumArrayRepository::zh_onCopyConcentrationDataAction()
{
    QString selectedString;
    emit zg_requestSelectedString(selectedString);

    if(selectedString.isEmpty())
    {
        return;
    }

    QClipboard* clipboard = qApp->clipboard();
    clipboard->setText(selectedString);
}
//==================================================================
void ZSpectrumArrayRepository::zh_onClearConcentrationDataAction()
{
    emit zg_requestClearSelected();
}
//==================================================================
void ZSpectrumArrayRepository::zh_onSetSpectraVisibleAction()
{
    int arrayIndex = -1;
    emit zg_requestCurrentArrayIndex(arrayIndex);

    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return;
    }

    ZSpectrumArray* array = zv_arrayList.at(arrayIndex);
    bool res;
    for(int s = 0; s < array->zp_spectrumCount(); s++)
    {
        res = array->zp_setSpectrumVisible(s, true);
        if(res)
        {
            emit zg_spectrumOperation(SOT_VISIBLE_CHANGED, arrayIndex, s, s);
        }
    }
}
//==================================================================
void ZSpectrumArrayRepository::zh_onSetSpectraInvisibleAction()
{
    int arrayIndex = -1;
    emit zg_requestCurrentArrayIndex(arrayIndex);

    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return;
    }

    ZSpectrumArray* array = zv_arrayList.at(arrayIndex);
    bool res;
    for(int s = 0; s < array->zp_spectrumCount(); s++)
    {
        res = array->zp_setSpectrumVisible(s, false);
        if(res)
        {
            emit zg_spectrumOperation(SOT_VISIBLE_CHANGED, arrayIndex, s, s);
        }
    }
}
//==================================================================
void ZSpectrumArrayRepository::zh_onInvertSpectraVisibilityAction()
{
    int arrayIndex = -1;
    emit zg_requestCurrentArrayIndex(arrayIndex);

    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return;
    }

    ZSpectrumArray* array = zv_arrayList.at(arrayIndex);
    bool res;
    for(int s = 0; s < array->zp_spectrumCount(); s++)
    {
        res = array->zp_setSpectrumVisible(s, !array->zp_isSpectrumVisible(s));
        if(res)
        {
            emit zg_spectrumOperation(SOT_VISIBLE_CHANGED, arrayIndex, s, s);
        }
    }
}
//==================================================================
void ZSpectrumArrayRepository::zh_onSetChemElementsVisibleAction()
{
    int arrayIndex = -1;
    emit zg_requestCurrentArrayIndex(arrayIndex);

    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return;
    }

    ZSpectrumArray* array = zv_arrayList.at(arrayIndex);
    for(int c = 0; c < array->zp_chemElementCount(); c++)
    {
        array->zp_setChemElementVisible(c, true);
    }
}
//==================================================================
void ZSpectrumArrayRepository::zh_onSetChemElementsInvisibleAction()
{
    int arrayIndex = -1;
    emit zg_requestCurrentArrayIndex(arrayIndex);

    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return;
    }

    ZSpectrumArray* array = zv_arrayList.at(arrayIndex);
    for(int c = 0; c < array->zp_chemElementCount(); c++)
    {
        array->zp_setChemElementVisible(c, false);
    }
}
//==================================================================
void ZSpectrumArrayRepository::zh_onInvertChemElementsVisibilityAction()
{
    int arrayIndex = -1;
    emit zg_requestCurrentArrayIndex(arrayIndex);

    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return;
    }

    ZSpectrumArray* array = zv_arrayList.at(arrayIndex);
    for(int c = 0; c < array->zp_chemElementCount(); c++)
    {
        array->zp_setChemElementVisible(c, !array->zp_chemElementIsVisible(c));
    }
}
//==================================================================
void ZSpectrumArrayRepository::zh_onEnergyCalibrationAction()
{
    // create selected spectrum map
    QList<int> selectedSpectrumList;
    emit zg_requestSelectedSpectrumIndexList(selectedSpectrumList);

    if(selectedSpectrumList.isEmpty())
    {
        QString string = tr("There is no spectra for energy calibration!");
        QMessageBox::critical(nullptr, tr("Spectra removing"), string, QMessageBox::Ok);
        return;
    }

    int currentArrayIndex;
    emit zg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= zv_arrayList.count())
    {
        return;
    }

    ZSpeSpectrum* spectrum;
    QMap<quint8, QList<ZSpeSpectrum*> > spectrumMap;

    quint8 gainFactor = static_cast<quint8>(zv_arrayList.at(currentArrayIndex)->zp_gainFactor());
    QList<ZAbstractSpectrum*> spectrumList = zv_arrayList.at(currentArrayIndex)->zp_spectrumList();

    if(!spectrumMap.keys().contains(gainFactor))
    {
        spectrumMap.insert(gainFactor, QList<ZSpeSpectrum*>());
    }

    for(int s = 0; s < spectrumList.count(); s++)
    {
        if(!selectedSpectrumList.contains(s))
        {
            continue;
        }

        spectrum = qobject_cast<ZSpeSpectrum*>(spectrumList.at(s));
        if(spectrum)
        {
            spectrumMap[gainFactor].append(spectrum);
        }
    }

    ZEnergyCalibrationDialogV2 dialog(spectrumMap);
    //    connect(&dialog, &ZEnergyCalibrationDialogV2::zg_energyCalibrationChanged,
    //            this, &ZSpectrumArrayRepository::zh_writeEnergyCalibrationToSpectra);
    dialog.exec();
}
//==================================================================
void ZSpectrumArrayRepository::zh_writeEnergyCalibrationToSpectra(int gainFactor,
                                                                  const QList<double>& energyCalibrationFactors)
{
    for(int a = 0; a < zv_arrayList.count(); a++)
    {
        QList<ZAbstractSpectrum*>spectrumList = zv_arrayList.at(a)->zp_spectrumList();
        for(int s = 0; s <spectrumList.count(); s++)
        {
            ZSpeSpectrum* spectrum = qobject_cast<ZSpeSpectrum*>(spectrumList.at(s));
            if(!spectrum || spectrum->zp_gainFactor() != gainFactor)
            {
                continue;
            }

            spectrum->zp_setEnergyCalibration(energyCalibrationFactors);
        }
    }
}
//==================================================================
QList<ZRawSpectrumArray> ZSpectrumArrayRepository::zh_createRawArrayList() const
{
    QList<ZRawSpectrumArray> rawSpectrumArrayList;
    for(int a = 0; a < zv_arrayList.count(); a++)
    {
        ZRawSpectrumArray rawSpectrumArray = zv_arrayList.value(a)->zp_createRawSpectrumArray();
        rawSpectrumArrayList.append(rawSpectrumArray);
    }

    return rawSpectrumArrayList;
}
//==================================================================
void ZSpectrumArrayRepository::zh_actionEnablingControl()
{
    int arrayIndex = -1;
    emit zg_requestCurrentArrayIndex(arrayIndex);

    //    zv_appendArrayAction->setEnabled(true);
    //    zv_appendSpectrumToArrayAction->setEnabled(true);

    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        zv_removeArrayAction->setEnabled(false);
        zv_saveArrayAction->setEnabled(zv_dirty);
        zv_saveAsArrayAction->setEnabled(!zv_arrayList.isEmpty());

        zv_appendChemElementAction->setEnabled(false);
        zv_removeChemElementAction->setEnabled(false);

        zv_appendChemElementAction->setEnabled(false);
        zv_removeChemElementAction->setEnabled(false);

        //        zv_pasteConcentrationDataAction->setEnabled(false);
        //        zv_copyConcentrationDataAction->setEnabled(false);
        //        zv_clearConcentrationDataAction->setEnabled(false);

        zv_setSpectraVisibleAction->setEnabled(false);
        zv_setSpectraInvisibleAction->setEnabled(false);
        zv_invertSpectraVisibilityAction->setEnabled(false);

        zv_setChemElementsVisibleAction->setEnabled(false);
        zv_setChemElementsInvisibleAction->setEnabled(false);
        zv_invertChemElementsVisibilityAction->setEnabled(false);

        return;
    }

    zv_removeArrayAction->setEnabled(true);
    zv_saveArrayAction->setEnabled(zv_dirty);
    zv_saveAsArrayAction->setEnabled(!zv_arrayList.isEmpty());

    zv_appendChemElementAction->setEnabled(true);

    if(zv_arrayList.at(arrayIndex)->zp_spectrumCount() > 0)
    {
        zv_setSpectraVisibleAction->setEnabled(true);
        zv_setSpectraInvisibleAction->setEnabled(true);
        zv_invertSpectraVisibilityAction->setEnabled(true);
    }
    else
    {
        zv_setSpectraVisibleAction->setEnabled(false);
        zv_setSpectraInvisibleAction->setEnabled(false);
        zv_invertSpectraVisibilityAction->setEnabled(false);
    }

    if(zv_arrayList.at(arrayIndex)->zp_chemElementCount() > 0)
    {
        QList<int> selectedChemElementIndexList;
        emit zg_requestSelectedChemElementIndexList(selectedChemElementIndexList);
        zv_removeChemElementAction->setEnabled(!selectedChemElementIndexList.isEmpty());

        zv_setChemElementsVisibleAction->setEnabled(true);
        zv_setChemElementsInvisibleAction->setEnabled(true);
        zv_invertChemElementsVisibilityAction->setEnabled(true);
    }
    else
    {
        zv_removeChemElementAction->setEnabled(false);

        zv_setChemElementsVisibleAction->setEnabled(false);
        zv_setChemElementsInvisibleAction->setEnabled(false);
        zv_invertChemElementsVisibilityAction->setEnabled(false);
    }

    emit zg_arrayListDirtyChanged(zv_dirty, !zv_arrayList.isEmpty());
}
//==================================================================
void ZSpectrumArrayRepository::zh_setDirty(bool dirty)
{
    zv_dirty = dirty;
    emit zg_arrayListDirtyChanged(zv_dirty, !zv_arrayList.isEmpty());
    emit zg_currentFile(zv_dirty, zv_arrayFilePath);
    zh_actionEnablingControl();
}
//==================================================================
void ZSpectrumArrayRepository::zh_createActions()
{
    zv_appendArrayAction = new QAction(this);
    zv_appendArrayAction->setIcon(QIcon(NS_Icons::glAddArray));
    zv_appendArrayAction->setText(tr("New spectrum array"));
    zv_appendArrayAction->setToolTip(tr("Append a new spectrum array to the list"));
    zv_appendArrayAction->setEnabled(true);

    zv_openArrayAction = new QAction(this);
    zv_saveArrayAction = new QAction(this);
    zv_saveAsArrayAction = new QAction(this);

    zv_removeArrayAction = new QAction(this);
    zv_removeArrayAction->setIcon(QIcon(NS_Icons::glRemoveArrayIconString));
    zv_removeArrayAction->setText(tr("Remove current spectrum array"));
    zv_removeArrayAction->setToolTip(tr("Remove current spectrum array from the list"));
    zv_removeArrayAction->setEnabled(false);

    zv_appendSpectrumToArrayAction = new QAction(this);
    zv_appendSpectrumToArrayAction->setIcon(QIcon(NS_Icons::glAddSpectrumIconString));
    zv_appendSpectrumToArrayAction->setText(tr("Append spectra"));
    zv_appendSpectrumToArrayAction->setToolTip(tr("Append spectra to current array"));
    zv_appendSpectrumToArrayAction->setEnabled(true);

    zv_removeSpectrumFromArrayAction = new QAction(this);
    zv_removeSpectrumFromArrayAction->setIcon(QIcon(NS_Icons::glRemoveSpectrumIconString));
    zv_removeSpectrumFromArrayAction->setText(tr("Remove selected spectra"));
    zv_removeSpectrumFromArrayAction->setToolTip(tr("Remove selected spectra from the array"));
    zv_removeSpectrumFromArrayAction->setEnabled(false);

    zv_energyCalibrationAction = new QAction(this);
    zv_energyCalibrationAction->setEnabled(false);
    zv_energyCalibrationAction->setIcon(QIcon(NS_Icons::glIconEnergyCalibration));
    zv_energyCalibrationAction->setText(tr("Energy calibration"));
    zv_energyCalibrationAction->setToolTip(tr("Recalculate energy calibration for selected spectra"));

    zv_appendChemElementAction = new QAction(this);
    zv_appendChemElementAction->setIcon(QIcon(NS_Icons::glAddChemElementIconString));
    zv_appendChemElementAction->setText(tr("New chemical element"));
    zv_appendChemElementAction->setToolTip(tr("Append a new chemical element to the list"));
    zv_appendChemElementAction->setEnabled(false);

    zv_removeChemElementAction = new QAction(this);
    zv_removeChemElementAction->setIcon(QIcon(NS_Icons::glRemoveChemElementIconString));
    zv_removeChemElementAction->setText(tr("Remove current chemical element"));
    zv_removeChemElementAction->setToolTip(tr("Remove current chemical element from the list"));
    zv_removeChemElementAction->setEnabled(false);

    zv_copyConcentrationDataAction = new QAction(this);
    zv_copyConcentrationDataAction->setIcon(QIcon(NS_Icons::glCopyIconString));
    zv_copyConcentrationDataAction->setText(tr("Copy selected concentrations"));
    zv_copyConcentrationDataAction->setEnabled(false);

    zv_pasteConcentrationDataAction = new QAction(this);
    zv_pasteConcentrationDataAction->setIcon(QIcon(NS_Icons::glPasteIconString));
    zv_pasteConcentrationDataAction->setText(tr("Paste concentrations"));
    zv_pasteConcentrationDataAction->setEnabled(false);

    zv_clearConcentrationDataAction = new QAction(this);
    zv_clearConcentrationDataAction->setIcon(QIcon(NS_Icons::glClearIconString));
    zv_clearConcentrationDataAction->setText(tr("Clear selected concentrations"));
    zv_clearConcentrationDataAction->setEnabled(false);

    // visibility actions
    zv_setSpectraVisibleAction = new QAction(this);
    zv_setSpectraVisibleAction->setIcon(QIcon(NS_Icons::glIconVisible));
    zv_setSpectraVisibleAction->setText(tr("Set spectra visible"));
    zv_setSpectraVisibleAction->setEnabled(false);

    zv_setSpectraInvisibleAction = new QAction(this);
    zv_setSpectraInvisibleAction->setIcon(QIcon(NS_Icons::glIconInvisible));
    zv_setSpectraInvisibleAction->setText(tr("Set spectra invisible"));
    zv_setSpectraInvisibleAction->setEnabled(false);

    zv_invertSpectraVisibilityAction = new QAction(this);
    zv_invertSpectraVisibilityAction->setIcon(QIcon(NS_Icons::glIconInverseVisible));
    zv_invertSpectraVisibilityAction->setText(tr("Invert spectra visibility"));
    zv_invertSpectraVisibilityAction->setEnabled(false);

    zv_setChemElementsVisibleAction = new QAction(this);
    zv_setChemElementsVisibleAction->setIcon(QIcon(NS_Icons::glIconVisible));
    zv_setChemElementsVisibleAction->setText(tr("Set chemical elements visible"));
    zv_setChemElementsVisibleAction->setEnabled(false);

    zv_setChemElementsInvisibleAction = new QAction(this);
    zv_setChemElementsInvisibleAction->setIcon(QIcon(NS_Icons::glIconInvisible));
    zv_setChemElementsInvisibleAction->setText(tr("Set chemical elements invisible"));
    zv_setChemElementsInvisibleAction->setEnabled(false);

    zv_invertChemElementsVisibilityAction = new QAction(this);
    zv_invertChemElementsVisibilityAction->setIcon(QIcon(NS_Icons::glIconInverseVisible));
    zv_invertChemElementsVisibilityAction->setText(tr("Invert chemical elements visibility"));
    zv_invertChemElementsVisibilityAction->setEnabled(false);
}
//==================================================================
void ZSpectrumArrayRepository::zh_createConnections()
{
    connect(zv_appendArrayAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onAppendArrayAction);
    connect(zv_removeArrayAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onRemoveArrayAction);
    connect(zv_appendSpectrumToArrayAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onAppendSpectraToArrayAction);
    connect(zv_removeSpectrumFromArrayAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onRemoveSpectrumFromArrayAction);
    connect(zv_appendChemElementAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onAppendChemElementAction);
    connect(zv_removeChemElementAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onRemoveChemElementAction);

    connect(zv_copyConcentrationDataAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onCopyConcentrationDataAction);
    connect(zv_pasteConcentrationDataAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onPasteConcentrationDataAction);
    connect(zv_clearConcentrationDataAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onClearConcentrationDataAction);

    connect(zv_setSpectraVisibleAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onSetSpectraVisibleAction);
    connect(zv_setSpectraInvisibleAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onSetSpectraInvisibleAction);
    connect(zv_invertSpectraVisibilityAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onInvertSpectraVisibilityAction);
    connect(zv_setChemElementsVisibleAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onSetChemElementsVisibleAction);
    connect(zv_setChemElementsInvisibleAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onSetChemElementsInvisibleAction);
    connect(zv_invertChemElementsVisibilityAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onInvertChemElementsVisibilityAction);

    connect(zv_energyCalibrationAction, &QAction::triggered,
            this, &ZSpectrumArrayRepository::zh_onEnergyCalibrationAction);


    connect(qApp->clipboard(), &QClipboard::dataChanged,
            this, &ZSpectrumArrayRepository::zh_onClipboardContentChange);

}
//==================================================================
int ZSpectrumArrayRepository::zh_chemElementIndexForId(int arrayIndex, qint64 chemElementId)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return -1;
    }

    return zv_arrayList.value(arrayIndex)->zp_chemElementIndexForId(chemElementId);
}
//==================================================================


