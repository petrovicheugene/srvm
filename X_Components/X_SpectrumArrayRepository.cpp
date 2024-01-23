//==================================================================
#include "X_SpectrumArrayRepository.h"
#include "X_General.h"
#include "X_FileActionManager.h"
#include "X_EnergyCalibrationDialogV2.h"

#include <math.h>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>
#include <QtAlgorithms>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QMessageBox>
#include <QStringList>
#include <math.h>
//==================================================================
X_SpectrumArrayRepository::X_SpectrumArrayRepository(QObject *parent) : QObject(parent)
{
    xv_arrayFilePath = QString();
    xv_dirty = false;
    xv_defaultArrayBaseName = tr("Array #");
    xh_createActions();
    xh_createConnections();
}
//==================================================================
void X_SpectrumArrayRepository::xp_appendActionsToMenu(QMenu* menu) const
{
    if(menu->objectName() == "Edit")
    {
        //        QMenu* arrayMenu = new QMenu(tr("Spectrum arrays"));
        //        arrayMenu->setIcon(QIcon(glArrayIconString));
        //        arrayMenu->addAction(xv_appendArrayAction);
        //        arrayMenu->addAction(xv_removeArrayAction);
        //        menu->addMenu(arrayMenu);

        //        QMenu* spectraMenu = new QMenu(tr("Spectra"));
        //        spectraMenu->setIcon(QIcon(glSpectrumIconString));
        //        spectraMenu->addAction(xv_appendSpectrumToArrayAction);
        //        spectraMenu->addAction(xv_removeSpectrumFromArrayAction);
        //        menu->addMenu(spectraMenu);

        //        QMenu* elementMenu = new QMenu(tr("Chemical elements"));
        //        elementMenu->setIcon(QIcon(glElementIconString));
        //        elementMenu->addAction(xv_appendChemElementAction);
        //        elementMenu->addAction(xv_removeChemElementAction);
        //        menu->addMenu(elementMenu);

        // spectra array
        menu->addAction(xv_appendArrayAction);
        menu->addAction(xv_removeArrayAction);
        menu->addSeparator();

        // spectra
        menu->addAction(xv_appendSpectrumToArrayAction);
        menu->addAction(xv_removeSpectrumFromArrayAction);
        menu->addSeparator();
        menu->addAction(xv_copyConcentrationDataAction);
        menu->addAction(xv_pasteConcentrationDataAction);
        menu->addSeparator();
        menu->addAction(xv_clearConcentrationDataAction);
        menu->addSeparator();

        // chemical
        menu->addAction(xv_appendChemElementAction);
        menu->addAction(xv_removeChemElementAction);
    }
    else if(menu->objectName() == "Actions")
    {
        menu->addAction(xv_energyCalibrationAction);
        menu->addSeparator();
    }

}
//==================================================================
QList<QAction*> X_SpectrumArrayRepository::xp_arrayActions() const
{
    QList<QAction*> actionList;
    actionList << xv_appendArrayAction;
    actionList << xv_removeArrayAction;
    return actionList;
}
//==================================================================
QList<QAction*> X_SpectrumArrayRepository::xp_arrayContextMenuActions() const
{
    QList<QAction*> actionList;
    actionList << xv_appendArrayAction;
    actionList << xv_openArrayAction;
    actionList << xv_removeArrayAction;
    actionList << nullptr;
    actionList << xv_saveArrayAction;
    actionList << xv_saveAsArrayAction;

    return actionList;
}
//==================================================================
QList<QAction*> X_SpectrumArrayRepository::xp_spectrumActions() const
{
    QList<QAction*> actionList;
    actionList << xv_appendSpectrumToArrayAction;
    actionList << xv_removeSpectrumFromArrayAction;
    //    actionList << 0; // separator
    //    actionList << xv_copyConcentrationDataAction;
    //    actionList << xv_pasteConcentrationDataAction;
    //    actionList << 0; // separator
    //    actionList << xv_clearConcentrationDataAction;

    return actionList;
}
//==================================================================
QList<QAction*> X_SpectrumArrayRepository::xp_spectrumContextMenuActions() const
{
    QList<QAction*> actionList;
    actionList << xv_appendSpectrumToArrayAction;
    actionList << xv_removeSpectrumFromArrayAction;
    actionList << nullptr; // separator
    actionList << xv_copyConcentrationDataAction;
    actionList << xv_pasteConcentrationDataAction;
    actionList << nullptr; // separator
    actionList << xv_clearConcentrationDataAction;
    actionList << nullptr; // separator
    actionList << xv_setSpectraVisibleAction;
    actionList << xv_setSpectraInvisibleAction;
    actionList << xv_invertSpectraVisibilityAction;
    actionList << nullptr; // separator
    actionList << xv_setChemElementsVisibleAction;
    actionList << xv_setChemElementsInvisibleAction;
    actionList << xv_invertChemElementsVisibilityAction;
    actionList << nullptr; // separator
    actionList << xv_energyCalibrationAction;
    return actionList;
}
//==================================================================
QList<QAction*> X_SpectrumArrayRepository::xp_chemElementActions() const
{
    QList<QAction*> actionList;
    actionList << xv_appendChemElementAction;
    actionList << xv_removeChemElementAction;
    return actionList;
}
//==================================================================
QList<QAction*> X_SpectrumArrayRepository::xp_chemElementContextMenuActions() const
{
    QList<QAction*> actionList;
    actionList << xv_appendChemElementAction;
    actionList << xv_removeChemElementAction;
    actionList << nullptr; // separator
    actionList << xv_setChemElementsVisibleAction;
    actionList << xv_setChemElementsInvisibleAction;
    actionList << xv_invertChemElementsVisibilityAction;

    return actionList;
}
//==================================================================
void X_SpectrumArrayRepository::xp_connectToFileActionManager(X_FileActionManager* manager)
{
    manager->xp_initOpenSpectrumArrayAction(xv_openArrayAction);
    manager->xp_initSaveSpectrumArrayAction(xv_saveArrayAction);
    manager->xp_initSaveAsSpectrumArrayAction(xv_saveAsArrayAction);

    connect(manager, &X_FileActionManager::xg_spectrumRawArrayList,
            this, &X_SpectrumArrayRepository::xp_appendArrays);
    connect(manager, &X_FileActionManager::xg_spectrumFileListToOpen,
            this, &X_SpectrumArrayRepository::xp_appendSpectraToArray);
    connect(manager, &X_FileActionManager::xg_spectrumArraySaved,
            this, &X_SpectrumArrayRepository::xh_onSpectrumArraySaving);

    connect(this, &X_SpectrumArrayRepository::xg_initSpectraAppending,
            manager, &X_FileActionManager::xp_defineSpectrumFilesAndInitAppending);
    connect(manager, &X_FileActionManager::xg_requestRawArrayListAndInitSaving,
            this, &X_SpectrumArrayRepository::xh_createRawArrayListAndStartSaving);
    connect(this, &X_SpectrumArrayRepository::xg_saveSpectraArrayList,
            manager, &X_FileActionManager::xp_saveSpectraArrayListToFile);

    connect(this, &X_SpectrumArrayRepository::xg_arrayListDirtyChanged,
            manager, &X_FileActionManager::xp_onArrayListDirtyChange);

}
//==================================================================
bool X_SpectrumArrayRepository::xp_isEmpty() const
{
    return xv_arrayList.isEmpty();
}
//==================================================================
void X_SpectrumArrayRepository::xp_clear()
{
    if(!xv_arrayList.isEmpty())
    {
        emit xg_spectrumArrayOperation(AOT_REMOVE_ARRAYS, 0, xv_arrayList.count() - 1);
        for(int i = xv_arrayList.count()-1; i >= 0; i-- )
        {
            delete xv_arrayList.takeAt(i);
        }
        emit xg_spectrumArrayOperation(AOT_END_REMOVE_ARRAYS, 0, xv_arrayList.count() - 1);
    }

    xv_arrayFilePath.clear();
    xh_setDirty(false);
}
//==================================================================
bool X_SpectrumArrayRepository::xp_isDirty() const
{
    return xv_dirty;
}
//==================================================================
const X_SpectrumArray* X_SpectrumArrayRepository::xp_array(int arrayIndex)
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return nullptr;
    }

    return xv_arrayList.at(arrayIndex);
}
//==================================================================
int X_SpectrumArrayRepository::xp_arrayCount() const
{
    return xv_arrayList.count();
}
//==================================================================
int X_SpectrumArrayRepository::xp_spectrumCount(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return 0;
    }

    return xv_arrayList.value(arrayIndex)->xp_spectrumCount();
}
//==================================================================
int X_SpectrumArrayRepository::xp_numberVisibleChemElementsBeforeIndex(int arrayIndex, int index) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return -1;
    }

    return xv_arrayList.value(arrayIndex)->xp_numberVisibleChemElementsBeforeIndex(index);
}
//==================================================================
int X_SpectrumArrayRepository::xp_chemElementCount(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return 0;
    }

    return xv_arrayList.value(arrayIndex)->xp_chemElementCount();
}
//==================================================================
int X_SpectrumArrayRepository::xp_visibleChemElementCount(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return 0;
    }

    return xv_arrayList.value(arrayIndex)->xp_visibleChemElementCount();
}
//==================================================================
QStringList X_SpectrumArrayRepository::xp_chemElementList(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return QStringList();
    }

    return xv_arrayList.value(arrayIndex)->xp_chemElementList();
}
//==================================================================
QString X_SpectrumArrayRepository::xp_visibleChemElementName(int arrayIndex, int visibleIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count()
            || visibleIndex < 0)
    {
        return QString();
    }

    return xv_arrayList.value(arrayIndex)->xp_visibleChemElementName(visibleIndex);
}
//==================================================================
QString X_SpectrumArrayRepository::xp_chemElementName(int arrayIndex, int row) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count()
            || row < 0)
    {
        return QString();
    }

    return xv_arrayList.value(arrayIndex)->xp_chemElementName(row);
}
//==================================================================
qint64 X_SpectrumArrayRepository::xp_visibleChemElementId(int arrayIndex, int visibleIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count()
            || visibleIndex < 0)
    {
        return -1;
    }

    return xv_arrayList.value(arrayIndex)->xp_visibleChemElementId(visibleIndex);
}
//==================================================================
qint64 X_SpectrumArrayRepository::xp_chemElementId(int arrayIndex, int row) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count()
            || row < 0)
    {
        return -1;
    }

    return xv_arrayList.value(arrayIndex)->xp_chemElementId(row);
}
//==================================================================
qint64 X_SpectrumArrayRepository::xp_chemElementIdForName(int arrayIndex, const QString& name) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count() || name.isEmpty())
    {
        return -1;
    }

    //    for(int a = 0; a < xv_arrayList.count(); a++)
    //    {
    for(int c = 0; c < xv_arrayList.at(arrayIndex)->xp_chemElementCount(); c++)
    {
        if(xv_arrayList.at(arrayIndex)->xp_chemElementName(c) == name)
        {
            return xv_arrayList.at(arrayIndex)->xp_chemElementId(c);
        }
    }
    //    }

    return -1;
}
//==================================================================
QString X_SpectrumArrayRepository::xp_chemConcentration(int arrayIndex,
                                                       int spectrumIndex, int visibleChemElementIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return QString();
    }

    if(spectrumIndex < 0 || spectrumIndex >= xp_spectrumCount(arrayIndex))
    {
        return QString();
    }

    if(visibleChemElementIndex < 0 || visibleChemElementIndex >= xp_visibleChemElementCount(arrayIndex))
    {
        return QString();
    }

    qint64 chemElementId = xp_visibleChemElementId(arrayIndex, visibleChemElementIndex);

    return xv_arrayList.value(arrayIndex)->xp_chemConcentration(chemElementId, spectrumIndex);
}
//==================================================================
bool X_SpectrumArrayRepository::xp_setChemConcentration(int arrayIndex, int spectrumIndex,
                                                       int visibleChemElementIndex, const QString& concentration)
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return false;
    }

    if(spectrumIndex < 0 || spectrumIndex >= xp_spectrumCount(arrayIndex))
    {
        return false;
    }

    if(visibleChemElementIndex < 0 || visibleChemElementIndex >= xp_visibleChemElementCount(arrayIndex))
    {
        return false;
    }

    qint64 chemElementId = xp_visibleChemElementId(arrayIndex, visibleChemElementIndex);
    bool res = xv_arrayList.value(arrayIndex)->xp_setChemConcentration(chemElementId, spectrumIndex, concentration);
    if(res)
    {
        //        int chemElementIndex = xh_chemElementIndexForId(arrayIndex, chemElementId);
        //        emit xg_chemElementOperation(CEOT_CHEM_ELEMENT_VALUE_CHANGED,
        //                                     arrayIndex, chemElementIndex, chemElementIndex);


        xh_setDirty(true);
    }

    return res;
}
//==================================================================
bool X_SpectrumArrayRepository::xp_setChemElementName(int arrayIndex, int chemElementIndex, const QString& chemElementName)
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return false;
    }

    return xv_arrayList.at(arrayIndex)->xp_setChemElementName(chemElementIndex, chemElementName);
}
//==================================================================
//bool X_SpectrumArrayRepository::xp_energyCalibration(int arrayIndex, qreal& K0, qreal& K1, qreal& K2, QString& energyUnit)
//{
//    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
//    {
//        return false;
//    }

//    return xv_arrayList.at(arrayIndex)->xp_energyCalibration(K0, K1, K2, energyUnit);
//}
////==================================================================
//bool X_SpectrumArrayRepository::xp_energyCalibrationForArrayId(qint64 arrayId, qreal& K0, qreal& K1, qreal& K2, QString& energyUnit)
//{
//    for(int i = 0; i < xv_arrayList.count(); i++)
//    {
//        if(xv_arrayList.at(i)->xp_arrayId() == arrayId)
//        {
//            return xp_energyCalibration(i, K0, K1, K2, energyUnit);
//        }
//    }

//    return false;
//}
//==================================================================
int X_SpectrumArrayRepository::xp_gainFactor(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return -1;
    }

    return xv_arrayList.at(arrayIndex)->xp_gainFactor();
}
//==================================================================
bool X_SpectrumArrayRepository::xp_setGainFactor(int arrayIndex, int gainFactor)
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return false;
    }

    bool res = xv_arrayList.at(arrayIndex)->xp_setGainFactor(gainFactor);
    if(res)
    {
        emit xg_spectrumArrayOperation(AOT_DATA_CHANGED, arrayIndex, arrayIndex);
        xh_setDirty(true);
    }

    return res;
}
//==================================================================
int X_SpectrumArrayRepository::xp_exposition(int arrayIndex)
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return -1;
    }

    return xv_arrayList.at(arrayIndex)->xp_exposition();
}
//==================================================================
bool X_SpectrumArrayRepository::xp_expositionForArrayId(qint64 arrayId, int& exposition)
{
    for(int i = 0; i < xv_arrayList.count(); i++)
    {
        if(xv_arrayList.at(i)->xp_arrayId() == arrayId)
        {
            exposition = xp_exposition(i);
            return true;
        }
    }

    return false;
}
//==================================================================
qint64 X_SpectrumArrayRepository::xp_arrayIdForArrayIndex(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return -1;
    }

    return xv_arrayList.at(arrayIndex)->xp_arrayId();
}
//==================================================================
const X_AbstractSpectrum* X_SpectrumArrayRepository::xp_spectrumForId(qint64 id) const
{
    for(int a = 0; a < xv_arrayList.count(); a++)
    {
        for(int s = 0; s < xv_arrayList.at(a)->xp_spectrumCount(); s++)
        {
            if(xv_arrayList.at(a)->xp_spectrumId(s) == id)
            {
                return xv_arrayList.at(a)->xp_spectrum(s);
            }
        }
    }

    return nullptr;
}
//==================================================================
//bool X_SpectrumArrayRepository::xp_averageChemConcentration(int arrayIndex, const QString& chemElement, qreal& averageValue ) const
//{
//    return xv_arrayList.at(arrayIndex)->xp_averageChemConcentration(chemElement, averageValue);
//}
////==================================================================
//bool X_SpectrumArrayRepository::xp_averageChemConcentration(int arrayIndex, int chemElementindex, qreal& averageValue ) const
//{
//    return xv_arrayList.at(arrayIndex)->xp_averageChemConcentration(chemElementindex, averageValue);
//}
////==================================================================
//bool X_SpectrumArrayRepository::xp_averageChemConcentrationForChemElementId(int arrayIndex, qint64 chemElementId , qreal& averageValue ) const
//{
//    return xv_arrayList.at(arrayIndex)->xp_averageChemConcentrationForChemElementId(chemElementId, averageValue);
//}
//==================================================================
bool X_SpectrumArrayRepository::xp_chemElementIsVisible(int arrayIndex, int row) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count()
            || row < 0)
    {
        return false;
    }

    return xv_arrayList.value(arrayIndex)->xp_chemElementIsVisible(row);
}
//==================================================================
bool X_SpectrumArrayRepository::xp_setChemElementVisible(int arrayIndex, int row, bool visible) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count()
            || row < 0)
    {
        return false;
    }

    return xv_arrayList.value(arrayIndex)->xp_setChemElementVisible(row, visible);

}
//==================================================================
QString X_SpectrumArrayRepository::xp_arrayName(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return QString();
    }

    return xv_arrayList.value(arrayIndex)->xp_arrayName();
}
//==================================================================
bool X_SpectrumArrayRepository::xp_setSpectrumArrayName(int arrayIndex, const QString& name)
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count() || name.isEmpty())
    {
        return false;
    }

    xv_arrayList[arrayIndex]->xp_setArrayName(name);
    emit xg_spectrumArrayOperation(AOT_DATA_CHANGED, arrayIndex, arrayIndex);

    xh_setDirty(true);

    return true;
}
//==================================================================
QString X_SpectrumArrayRepository::xp_spectrumName(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count() )
    {
        return QString();
    }

    return xv_arrayList.value(arrayIndex)->xp_spectrumFileName(spectrumIndex);
}
//======================================================
qint64 X_SpectrumArrayRepository::xp_spectrumId(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count() )
    {
        return 0;
    }

    return xv_arrayList.value(arrayIndex)->xp_spectrumId(spectrumIndex);
}
//======================================================
QColor X_SpectrumArrayRepository::xp_spectrumColor(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count() )
    {
        return QColor();
    }

    return xv_arrayList.value(arrayIndex)->xp_spectrumColor(spectrumIndex);
}
//======================================================
bool X_SpectrumArrayRepository::xp_isSpectrumVisible(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count() )
    {
        return false;
    }

    return xv_arrayList.value(arrayIndex)->xp_isSpectrumVisible(spectrumIndex);
}
//======================================================
bool X_SpectrumArrayRepository::xp_setSpectrumVisible(int arrayIndex, int spectrumIndex, bool visible)
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count() )
    {
        return false;
    }

    bool res = xv_arrayList.value(arrayIndex)->xp_setSpectrumVisible(spectrumIndex, visible);
    if(res)
    {
        emit xg_spectrumOperation(SOT_VISIBLE_CHANGED, arrayIndex, spectrumIndex, spectrumIndex);
    }
    return res;
}
//==================================================================
bool X_SpectrumArrayRepository::xp_isSpectrumVisible(qint64 arrayId, qint64 spectrumId) const
{
    for(int i = 0; i < xv_arrayList.count(); i++)
    {
        if(xv_arrayList.at(i)->xp_arrayId() != arrayId)
        {
            continue;
        }

        for(int s = 0; s < xv_arrayList.at(i)->xp_spectrumCount(); s++)
        {
            if(xv_arrayList.at(i)->xp_spectrumId(s) != spectrumId)
            {
                continue;
            }

            return xv_arrayList.at(i)->xp_isSpectrumVisible(s);
        }
        return false;
    }
    return false;
}
//==================================================================
bool X_SpectrumArrayRepository::xp_setSpectrumVisible(qint64 arrayId, qint64 spectrumId, bool visible)
{
    for(int i = 0; i < xv_arrayList.count(); i++)
    {
        if(xv_arrayList.at(i)->xp_arrayId() != arrayId)
        {
            continue;
        }

        for(int s = 0; s < xv_arrayList.at(i)->xp_spectrumCount(); s++)
        {
            if(xv_arrayList.at(i)->xp_spectrumId(s) != spectrumId)
            {
                continue;
            }

            return xv_arrayList.at(i)->xp_setSpectrumVisible(s, visible);
        }
        return false;
    }
    return false;
}
//==================================================================
bool X_SpectrumArrayRepository::xp_isSpectrumCheckedForId(qint64 arrayId, qint64 spectrumId) const
{
    for(int i = 0; i < xv_arrayList.count(); i++)
    {
        if(xv_arrayList.at(i)->xp_arrayId() != arrayId)
        {
            continue;
        }

        for(int s = 0; s < xv_arrayList.at(i)->xp_spectrumCount(); s++)
        {
            if(xv_arrayList.at(i)->xp_spectrumId(s) != spectrumId)
            {
                continue;
            }

            return xv_arrayList.at(i)->xp_isSpectrumChecked(s);
        }
        return false;
    }
    return false;
}
//==================================================================
bool X_SpectrumArrayRepository::xp_isSpectrumChecked(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return false;
    }
    return xv_arrayList.at(arrayIndex)->xp_isSpectrumChecked(spectrumIndex);
}
//==================================================================
bool X_SpectrumArrayRepository::xp_setSpectrumChecked(int arrayIndex, int spectrumIndex, bool checked)
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return false;
    }

    bool res = xv_arrayList.at(arrayIndex)->xp_setSpectrumChecked(spectrumIndex, checked);
    if(res)
    {
        emit xg_spectrumOperation(SOT_CHECKED_CHANGED,  arrayIndex, spectrumIndex, spectrumIndex);
    }
    return res;
}
//==================================================================
bool X_SpectrumArrayRepository::xp_setSpectrumCheckedForId(qint64 arrayId, qint64 spectrumId, bool checked)
{
    for(int i = 0; i < xv_arrayList.count(); i++)
    {
        if(xv_arrayList.at(i)->xp_arrayId() != arrayId)
        {
            continue;
        }

        for(int s = 0; s < xv_arrayList.at(i)->xp_spectrumCount(); s++)
        {
            if(xv_arrayList.at(i)->xp_spectrumId(s) != spectrumId)
            {
                continue;
            }

            bool res = xv_arrayList.at(i)->xp_setSpectrumChecked(s, checked);
            if(res)
            {
                emit xg_spectrumOperation(SOT_CHECKED_CHANGED,  i, s, s);
            }

            return res;
        }
    }
    return false;
}
//==================================================================
void X_SpectrumArrayRepository::xp_setSpectrumCurrent(qint64 spectrumId) const
{
    if(spectrumId < 0)
    {
        return;
    }

    int currentArrayIndex = -1;
    emit xg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= xv_arrayList.count())
    {
        return;
    }

    for(int s = 0; s < xv_arrayList.at(currentArrayIndex)->xp_spectrumCount(); s++)
    {
        if(xv_arrayList.at(currentArrayIndex)->xp_spectrumId(s) == spectrumId)
        {
            emit xg_setCurrentSpectrumIndex(s);
            break;
        }
    }
}
//==================================================================
const X_AbstractSpectrum* X_SpectrumArrayRepository::xp_spectrum(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count() )
    {
        return nullptr;
    }

    return xv_arrayList.value(arrayIndex)->xp_spectrum(spectrumIndex);
}
//==================================================================
QList<X_AbstractSpectrum*> X_SpectrumArrayRepository::xp_spectrumListForArray(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count() )
    {
        return QList<X_AbstractSpectrum*>();
    }

    return xv_arrayList.value(arrayIndex)->xp_spectrumList();
}
//==================================================================
QList<quint32> X_SpectrumArrayRepository::xp_spectrumData(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count() )
    {
        return QList<quint32>();
    }

    return xv_arrayList.at(arrayIndex)->xp_spectrumData(spectrumIndex);
}
//==================================================================
int X_SpectrumArrayRepository::xp_arrayChannelCount(int arrayIndex)
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count() )
    {
        return 0;
    }

    return xv_arrayList.value(arrayIndex)->xp_maxChannelCount();
}
//==================================================================
int X_SpectrumArrayRepository::xp_arrayMaxIntensity(int arrayIndex)
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count() )
    {
        return 0;
    }

    return xv_arrayList.value(arrayIndex)->xp_maxIntensity();
}
//==================================================================
void X_SpectrumArrayRepository::xp_appendArrays(QString path, QList<X_RawSpectrumArray> rawArrayList)
{
    bool changePath = true;
    if(path == xv_arrayFilePath || (xv_arrayFilePath.isEmpty() && !xv_dirty))
    {
        xp_clear();
    }
    else
    {
        if(xv_dirty)
        {
            QString question;
            if(!xv_arrayFilePath.isEmpty())
            {
                question = tr("File \"%1\" has been changed. ").arg(xv_arrayFilePath);
            }
            question += tr("Do you want to save current array changes?");

            if(QMessageBox::question(nullptr, tr("Array appending"), question, QMessageBox::Yes, QMessageBox::No)
                    == QMessageBox::Yes)
            {
                // TODO SAVING ARRAY LIST TO FILE
                // xp_saveArrayListToFile();
            }
        }

        if(xv_arrayList.count() > 0)
        {
            QString question = tr("Clear current array list?");
            if(QMessageBox::question(nullptr, tr("Array appending"), question, QMessageBox::Yes, QMessageBox::No)
                    == QMessageBox::Yes)
            {
                xp_clear();
            }
            else
            {
                changePath = false;
                xh_setDirty(true);
            }
        }
    }

    foreach(X_RawSpectrumArray rawArray, rawArrayList)
    {
        xh_createArray(rawArray);
    }

    if(changePath)
    {
        xv_arrayFilePath = path;

    }


    emit xg_currentFile(xv_dirty, xv_arrayFilePath);
    emit xg_fitPlotInBoundingRect();
    xh_actionEnablingControl();
}
//==================================================================
void X_SpectrumArrayRepository::xp_appendSpectraToArray(QStringList fileNameList)
{
    if(xv_currentArrayIndex < 0 || xv_currentArrayIndex >= xv_arrayList.count())
    {
        return;
    }

    int spectraStartCount = xv_arrayList.at(xv_currentArrayIndex)->xp_spectrumCount();
    bool dontAsk = false;
    bool continueAppend = true;
    for(int i = 0; i < fileNameList.count(); i++)
    {
        QFileInfo fileInfo(fileNameList.at(i));
        if(!fileInfo.exists())
        {
            QString msg = tr("File \"%1\" does not exist!").arg(xv_arrayList.at(xv_currentArrayIndex)->xp_arrayName());
            emit xg_message(msg);
            qCritical().noquote() <<  msg;
            continue;
        }

        if(fileInfo.isDir())
        {
            QString msg = tr("\"%1\" is a folder!").arg(xv_arrayList.at(xv_currentArrayIndex)->xp_arrayName());
            emit xg_message(msg);
            qCritical().noquote() <<  msg;
            continue;
        }

        X_RawSpectrum rawSpectrum;
        rawSpectrum.path = fileNameList.at(i);

        // int spectrumIndex = xv_arrayList[arrayIndex]->xp_spectrumCount();
        // emit xg_currentSpectrumOperation(SOT_INSERT_SPECTRA, arrayIndex, spectrumIndex, spectrumIndex);
        if(!xv_arrayList[xv_currentArrayIndex]->xp_appendSpectrum(rawSpectrum, fileNameList.count()-1 == i, dontAsk, continueAppend))
        {

        }

        if(!continueAppend)
        {
            break;
        }

        // emit xg_currentSpectrumOperation(SOT_END_INSERT_SPECTRA, arrayIndex, spectrumIndex, spectrumIndex);
    }

    int loaded = xv_arrayList.at(xv_currentArrayIndex)->xp_spectrumCount() - spectraStartCount;
    if(loaded > 0)
    {
        QString spectrumString = loaded > 1? tr("spectra") : tr("spectrum");
        QString msg = tr("%1 %2 loaded", "", 3).arg(QString::number(loaded), spectrumString);
        emit xg_message(msg);
        qInfo().noquote() <<  msg;

        if(spectraStartCount < 1)
        {
            emit xg_fitPlotInBoundingRect();
        }
        else
        {
            bool plotIsScaled;
            emit xg_requestIsPlotScaled(plotIsScaled);
            if(!plotIsScaled)
            {
                emit xg_fitPlotInBoundingRect();
            }
        }

        xh_setDirty(true);

    }
    xh_actionEnablingControl();

    int currentArrayIndex;
    emit xg_requestCurrentArrayIndex(currentArrayIndex);
    if(xv_currentArrayIndex == currentArrayIndex)
    {

    }
}
//==================================================================
void X_SpectrumArrayRepository::xh_createArray(const X_RawSpectrumArray& rawArray)
{
    int nextArrayIndex = xv_arrayList.count();
    emit xg_spectrumArrayOperation(AOT_INSERT_ARRAYS, nextArrayIndex, nextArrayIndex);

    X_SpectrumArray* array = new X_SpectrumArray(rawArray.name, this);
    array->xp_setGainFactor(rawArray.gainFactor);
    xv_arrayList << array;

    connect(array, &X_SpectrumArray::xg_message,
            this, &X_SpectrumArrayRepository::xg_message);
    connect(array, &X_SpectrumArray::xg_chemElementOperation,
            this, &X_SpectrumArrayRepository::xh_onChemElementOperation);
    connect(array, &X_SpectrumArray::xg_spectrumOperation,
            this, &X_SpectrumArrayRepository::xh_onSpectrumOperation);
    connect(array, &X_SpectrumArray::xg_arrayMaxParametersChanged,
            this, &X_SpectrumArrayRepository::xg_arrayMaxParametersChanged);

    bool dontAsk = false;
    bool continueAppend = true;
    for(int s = 0; s < rawArray.spectrumList.count(); s++)
    {
        if(!array->xp_appendSpectrum(rawArray.spectrumList.value(s), rawArray.spectrumList.count()-1 == s, dontAsk, continueAppend))
        {

        }

        if(!continueAppend)
        {
            break;
        }
    }

    emit xg_fitPlotInBoundingRect();
    emit xg_spectrumArrayOperation(AOT_END_INSERT_ARRAYS, nextArrayIndex, nextArrayIndex);
}
//==================================================================
bool X_SpectrumArrayRepository::xh_removeArray(int arrayIndex)
{
    int nextCurrentIndex;
    if(arrayIndex == xv_arrayList.count() - 1 && xv_arrayList.count() > 1)
    {
        nextCurrentIndex = xv_arrayList.count() - 2;
    }
    else if(arrayIndex == xv_arrayList.count() - 1 && xv_arrayList.count() == 1)
    {
        nextCurrentIndex = -1;
    }
    else
    {
        nextCurrentIndex = arrayIndex;
    }

    emit xg_spectrumArrayOperation(AOT_REMOVE_ARRAYS, arrayIndex, arrayIndex);

    delete xv_arrayList.at(arrayIndex);
    xv_arrayList.removeAt(arrayIndex);

    emit xg_spectrumArrayOperation(AOT_END_REMOVE_ARRAYS, arrayIndex, arrayIndex);
    emit xg_setCurrentArrayIndex(nextCurrentIndex);
    return true;
}
//==================================================================
bool X_SpectrumArrayRepository::xh_removeSpectrum(int arrayIndex, int spectrumIndex)
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return false;
    }

    if(spectrumIndex < 0 || spectrumIndex >= xv_arrayList.at(arrayIndex)->xp_spectrumCount())
    {
        return false;
    }

    return xv_arrayList[arrayIndex]->xp_removeSpectrum(spectrumIndex);
}
//==================================================================
bool X_SpectrumArrayRepository::xh_removeChemicalElement(int arrayIndex, int elementIndex)
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return false;
    }

    if(elementIndex < 0 || elementIndex >= xv_arrayList.at(arrayIndex)->xp_chemElementCount())
    {
        return false;
    }

    bool res = xv_arrayList[arrayIndex]->xp_removeChemElement(elementIndex);
    if(res)
    {
        xh_setDirty(true);
    }
    return  res;
}
//==================================================================
void X_SpectrumArrayRepository::xp_getArrayCount(int& arrayCount) const
{
    arrayCount = xv_arrayList.count();
}
//==================================================================
void X_SpectrumArrayRepository::xp_getSpectrumCount(int arrayIndex, int& spectrumCount) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        spectrumCount = -1;
        return;
    }

    spectrumCount = xv_arrayList.value(arrayIndex)->xp_spectrumCount();
}
//==================================================================
void X_SpectrumArrayRepository::xp_getArrayName(int arrayIndex, QString& arrayName) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        arrayName = QString();
        return;
    }

    arrayName = xv_arrayList.value(arrayIndex)->xp_arrayName();
}
//==================================================================
void X_SpectrumArrayRepository::xp_getSpectrumName(int arrayIndex, int spectrumIndex, QString& spectrumName) const
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count() )
    {
        spectrumName = QString();
        return;
    }

    if(spectrumIndex < 0 || spectrumIndex >= xv_arrayList.value(arrayIndex)->xp_spectrumCount())
    {
        spectrumName = QString();
        return;
    }

    spectrumName = xv_arrayList.value(arrayIndex)->xp_spectrumFileName(spectrumIndex);
}
//==================================================================
void X_SpectrumArrayRepository::xp_currentArrayChanged(int current, int previous)
{
    qint64 arrayId;
    if(current < 0 || current >= xv_arrayList.count() )
    {
        arrayId = -1;
    }
    else
    {
        arrayId = xv_arrayList.at(current)->xp_arrayId();
    }

    xv_pasteConcentrationDataAction->setEnabled(current >= 0 && xv_pasteData.xp_isValid());

    emit xg_currentArrayIsAboutChange(arrayId, current);
    emit xg_currentArrayIdChanged(arrayId, current);

    xh_actionEnablingControl();
}
//==================================================================
void X_SpectrumArrayRepository::xp_currentSpectrumChanged(int currentSpectrumIndex, int previousSpectrumIndex)
{
    int currentArrayIndex;
    qint64 currentSpectrumId;
    qint64 previousSpectrumId;
    QList<double> energyCalibration;
    QString energyUnit;

    emit xg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= xv_arrayList.count())
    {
        currentSpectrumId = -1;
        previousSpectrumId = -1;
    }
    else
    {
        if(currentSpectrumIndex < 0 || currentSpectrumIndex >= xv_arrayList.at(currentArrayIndex)->xp_spectrumCount())
        {
            currentSpectrumId = -1;
        }
        else
        {
            currentSpectrumId = xv_arrayList.at(currentArrayIndex)->xp_spectrumId(currentSpectrumIndex);
            xv_arrayList.at(currentArrayIndex)->xp_energyCalibration(currentSpectrumIndex, energyCalibration, energyUnit);
        }

        if(previousSpectrumIndex < 0 || previousSpectrumIndex >= xv_arrayList.at(currentArrayIndex)->xp_spectrumCount())
        {
            previousSpectrumId = -1;
        }
        else
        {
            previousSpectrumId = xv_arrayList.at(currentArrayIndex)->xp_spectrumId(previousSpectrumIndex);
        }
    }

    emit xg_currentSpectrumChanged(currentSpectrumId, currentSpectrumIndex, previousSpectrumId, previousSpectrumIndex);
    emit xg_energyCalibrationChanged(energyCalibration);
}
//==================================================================
void X_SpectrumArrayRepository::xp_chemElementListForCurrentArray(QStringList& chemElementList)
{
    int currentArray;
    emit xg_requestCurrentArrayIndex(currentArray);
    chemElementList = xp_chemElementList(currentArray);
}
//==================================================================
void X_SpectrumArrayRepository::xp_onSelectionSpectraChange(bool selectionEnabled,
                                                           bool concentrationSelected,
                                                           bool spectrumSelected)
{
    // xh_actionEnablingControl();
    xv_removeSpectrumFromArrayAction->setEnabled(selectionEnabled);
    xv_energyCalibrationAction->setEnabled(spectrumSelected);

    xv_copyConcentrationDataAction->setEnabled(selectionEnabled);
    xv_clearConcentrationDataAction->setEnabled(concentrationSelected);
}
//==================================================================
void X_SpectrumArrayRepository::xp_currentSpectrumWindowIntensity(int firstChannel,
                                                                 int lastChennel, qreal& intensity) const
{
    intensity = sqrt(-1); // NAN value

    // get current spectrum array
    int currentArrayIndex = -1;
    emit xg_requestCurrentArrayIndex(currentArrayIndex);

    if(currentArrayIndex < 0 || currentArrayIndex >= xv_arrayList.count())
    {
        // RETURN ERROR
        return;
    }

    int currentSpectrumIndex = -1;
    bool ok = false;
    emit xg_requestCurrentSpectrumRow(currentSpectrumIndex, &ok);

    if(!ok || currentSpectrumIndex < 0 || currentSpectrumIndex >= xv_arrayList.at(currentArrayIndex)->xp_spectrumCount())
    {
        // RETURN ERROR
        return;
    }


    const X_AbstractSpectrum* spectrum =  xv_arrayList.at(currentArrayIndex)->xp_spectrum(currentSpectrumIndex);
    if(!spectrum)
    {
        // RETURN ERROR
        return;
    }

    spectrum->xp_intensityInWindow(firstChannel, lastChennel, intensity);
}
//==================================================================
void X_SpectrumArrayRepository::xh_onSpectrumArraySaving(QString filePath)
{
    xv_arrayFilePath = filePath;
    xh_setDirty(false);
}
//==================================================================
void X_SpectrumArrayRepository::xh_onAppendArrayAction()
{
    X_RawSpectrumArray rawArray;
    // new array name
    int maxArrayNumber = 1;
    for(int i = 0; i < xv_arrayList.count(); i++)
    {
        QString name = xv_arrayList.at(i)->xp_arrayName();
        if(name.startsWith(xv_defaultArrayBaseName))
        {
            QString digitalPart = name.mid(xv_defaultArrayBaseName.length());
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

    rawArray.name = xv_defaultArrayBaseName + QString::number(maxArrayNumber);

    xh_createArray(rawArray);
    xh_setDirty(true);

    emit xg_setCurrentArrayIndex(xv_arrayList.count() - 1);
    emit xg_startCurrentArrayEdition();
    xh_actionEnablingControl();

}
//==================================================================
void X_SpectrumArrayRepository::xh_onRemoveArrayAction()
{
    int currentArrayIndex = -1;
    emit xg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= xv_arrayList.count())
    {
        return;
    }

    QString question = tr("Do you want to remove array \"%1\" from the list?").arg(xv_arrayList.value(currentArrayIndex)->xp_arrayName());
    if(QMessageBox::question(nullptr, tr("Array removing"), question, QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::Yes)
    {
        bool res = xh_removeArray(currentArrayIndex);
        if(res)
        {
            if(xv_arrayList.isEmpty())
            {
                xh_setDirty(false);
                xv_arrayFilePath = QString();
            }
            else
            {
                xh_setDirty(true);
            }
        }
    }
    xh_actionEnablingControl();

}
//==================================================================
void X_SpectrumArrayRepository::xh_onAppendSpectraToArrayAction()
{
    if(xv_arrayList.count() < 1)
    {
        // a new array creation
        xv_appendArrayAction->trigger();
    }

    emit xg_requestCurrentArrayIndex(xv_currentArrayIndex);

    if(xv_currentArrayIndex < 0 || xv_currentArrayIndex >= xv_arrayList.count())
    {
        QString string = tr("Select an array you want to append spectra to!");
        QMessageBox::critical(nullptr, tr("Spectra appending"), string, QMessageBox::Ok);
        return;
    }

    emit xg_initSpectraAppending(xv_currentArrayIndex);

    xh_actionEnablingControl();

}
//==================================================================
void X_SpectrumArrayRepository::xh_onRemoveSpectrumFromArrayAction()
{
    QList<int> selectedSpectrumList;
    emit xg_requestSelectedSpectrumIndexList(selectedSpectrumList);

    if(selectedSpectrumList.isEmpty())
    {
        QString string = tr("There is no spectra to remove!");
        QMessageBox::critical(nullptr, tr("Spectra removing"), string, QMessageBox::Ok);
        return;
    }

    int currentArrayIndex;
    emit xg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= xv_arrayList.count())
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
        res = xh_removeSpectrum(currentArrayIndex, selectedSpectrumList.value(i)) || res;
    }

    if(res)
    {
        bool plotIsScaled;
        emit xg_requestIsPlotScaled(plotIsScaled);
        if(!plotIsScaled)
        {
            emit xg_fitPlotInBoundingRect();
        }

        if(xv_arrayList.isEmpty())
        {
            xh_setDirty(false);

            xv_arrayFilePath = QString();
        }
        else
        {
            xh_setDirty(true);
        }
    }
    xh_actionEnablingControl();

}
//==================================================================
void X_SpectrumArrayRepository::xh_onAppendChemElementAction()
{
    int currentArrayIndex = -1;
    emit xg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= xv_arrayList.count())
    {
        return;
    }

    if(xv_arrayList[currentArrayIndex]->xp_appendNewChemElement())
    {
        xh_setDirty(true);

        emit xg_setCurrentChemElementIndex(xv_arrayList.at(currentArrayIndex)->xp_chemElementCount() - 1);
        emit xg_startCurrentChemElementEdition();
    }
    xh_actionEnablingControl();

}
//==================================================================
void X_SpectrumArrayRepository::xh_onRemoveChemElementAction()
{
    QList<int> selectedChemElementList;
    emit xg_requestSelectedChemElementIndexList(selectedChemElementList);

    if(selectedChemElementList.isEmpty())
    {
        QString string = tr("There is no chemical elements to remove!");
        QMessageBox::critical(nullptr, tr("Chemical element removing"), string, QMessageBox::Ok);
        return;
    }

    int currentArrayIndex;
    emit xg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= xv_arrayList.count())
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
        res = xh_removeChemicalElement(currentArrayIndex, selectedChemElementList.value(i)) || res;
    }

    xh_actionEnablingControl();
}
//==================================================================
void X_SpectrumArrayRepository::xh_onChemElementOperation(X_ChemElementList::OperationType type, int first, int last)
{
    QObject* signalSender = sender();
    if(signalSender == nullptr)
    {
        return;
    }

    for(int i = 0; i < xv_arrayList.count(); i++)
    {
        if(xv_arrayList.value(i) == signalSender)
        {
            ChemElementOperationType chemElementOperationType;
            if(type == X_ChemElementList::OT_INSERT_CHEM_ELEMENT)
            {
                chemElementOperationType = CEOT_INSERT_CHEM_ELEMENT;
            }
            else if(type == X_ChemElementList::OT_END_INSERT_CHEM_ELEMENT)
            {
                chemElementOperationType = CEOT_END_INSERT_CHEM_ELEMENT;
            }
            else if(type == X_ChemElementList::OT_REMOVE_CHEM_ELEMENT)
            {
                chemElementOperationType = CEOT_REMOVE_CHEM_ELEMENT;
            }
            else if(type == X_ChemElementList::OT_END_REMOVE_CHEM_ELEMENT)
            {
                chemElementOperationType = CEOT_END_REMOVE_CHEM_ELEMENT;
            }
            else if(type == X_ChemElementList::OT_CHANGED)
            {
                chemElementOperationType = CEOT_CHEM_ELEMENT_CHANGED;
            }
            else if(type == X_ChemElementList::OT_VISIBLE_CHANGE)
            {
                chemElementOperationType = CEOT_CHEM_ELEMENT_VISIBILITY_CHANGE;
            }
            else if(type == X_ChemElementList::OT_END_VISIBLE_CHANGE)
            {
                chemElementOperationType = CEOT_END_CHEM_ELEMENT_VISIBILITY_CHANGE;
            }
            else
            {
                return;
            }

            emit xg_chemElementOperation(chemElementOperationType, i, first, last);
            return;
        }
    }
}
//==================================================================
void X_SpectrumArrayRepository::xh_createRawArrayListAndStartSaving(QString& filePath, bool& res) const
{
    if(filePath.isEmpty())
    {
        filePath = xv_arrayFilePath;
    }

    QList<X_RawSpectrumArray> rawSpectrumArrayList = xh_createRawArrayList();
    emit xg_saveSpectraArrayList(filePath, rawSpectrumArrayList, res);
    // TODO after saving FileActionManager must send back a signal for reset dirty flag
    // TODO signal from fileActionManager with saved filePath. xv_path have to be canged
}
//==================================================================
void X_SpectrumArrayRepository::xh_onSpectrumOperation(X_SpectrumArray::OperationType type, int first, int last) const
{
    if(!sender())
    {
        return;
    }
    X_SpectrumArray* array = static_cast<X_SpectrumArray*>(sender());
    for(int arrayIndex = 0; arrayIndex < xv_arrayList.count(); arrayIndex++)
    {
        if(xv_arrayList.at(arrayIndex) != array)
        {
            continue;
        }

        if(type == X_SpectrumArray::OT_INSERT_SPECTRA)
        {
            emit xg_spectrumOperation(SOT_BEGIN_INSERT_SPECTRA, arrayIndex, first, last);
        }
        else if(type == X_SpectrumArray::OT_END_INSERT_SPECTRA)
        {
            emit xg_spectrumOperation(SOT_END_INSERT_SPECTRA, arrayIndex, first, last);
        }
        else if(type == X_SpectrumArray::OT_REMOVE_SPECTRA)
        {
            emit xg_spectrumOperation(SOT_BEGIN_REMOVE_SPECTRA, arrayIndex, first, last);
        }
        else if(type == X_SpectrumArray::OT_END_REMOVE_SPECTRA)
        {
            emit xg_spectrumOperation(SOT_END_REMOVE_SPECTRA, arrayIndex, first, last);
        }
        else if(type == X_SpectrumArray::OT_CONCENTRATION_CHANGED)
        {
            emit xg_spectrumOperation(SOT_CONCENTRATION_CHANGED, arrayIndex, first, last);
        }
        else if(type == X_SpectrumArray::OT_SPECTRUM_ENERGY_CALIBRATION_CHANGED)
        {
            // if calling spectrum is current then get energy calibration and emit signal

            // define current spectrum
            int currentArrayIndex = -1;
            emit xg_requestCurrentArrayIndex(currentArrayIndex);

            if(currentArrayIndex >= 0 && currentArrayIndex < xv_arrayList.count() &&
                    currentArrayIndex == arrayIndex)
            {
                // get current spectrum
                int currentRow = -1;
                bool ok = false;
                emit xg_requestCurrentSpectrumRow(currentRow, &ok);

                if(ok && currentRow >= first && currentRow <= last)
                {
                    QList<double> energyCalibration;
                    QString energyUnit;
                    array->xp_energyCalibration(currentRow, energyCalibration, energyUnit);
                    emit xg_energyCalibrationChanged(energyCalibration);
                }
            }
        }

        break;
    }
}
//==================================================================
void X_SpectrumArrayRepository::xh_onClipboardContentChange()
{
    xv_pasteData.xp_reset();

    QClipboard* clipboard = qApp->clipboard();
    if(!clipboard->mimeData()->hasText())
    {
        xv_pasteConcentrationDataAction->setEnabled(false);
        return;
    }

    QString sourceString = clipboard->text();

    int currentArrayIndex = -1;
    emit xg_requestCurrentArrayIndex(currentArrayIndex);
    X_SpectrumArray* currentArray = xv_arrayList.value(currentArrayIndex);
    QStringList chemElementList;
    if(currentArray)
    {
        chemElementList = currentArray->xp_chemElementList();
    }

    if(!xv_pasteData.xp_loadData(sourceString, chemElementList))
    {
        xv_pasteConcentrationDataAction->setEnabled(false);
        return;
    }

    xv_pasteConcentrationDataAction->setEnabled(currentArrayIndex >= 0 && currentArrayIndex < xv_arrayList.count());
    return;
}
//==================================================================
void X_SpectrumArrayRepository::xh_onPasteConcentrationDataAction()
{
    if(!xv_pasteData.xp_isValid())
    {
        xv_pasteConcentrationDataAction->setEnabled(false);
        return;
    }

    int currentArrayIndex = -1;
    emit xg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= xv_arrayList.count())
    {
        QString warning = tr("Select a spectrum array for data pasting.");
        QMessageBox::warning(nullptr, xv_messageBoxPasteTitle, warning, QMessageBox::Ok);
        return;
    }

    X_SpectrumArray* currentArray = xv_arrayList.value(currentArrayIndex);
    if(!currentArray)
    {
        return;
    }

    if(xv_pasteData.xp_isHeaderListValid())
    {
        // Headers exist. fill every chemElement from first spectrum
        // check chemElements in pastingData
        QStringList chemElementList = currentArray->xp_chemElementList();
        QStringList absentChemElementList = xv_pasteData.xp_absentHeaderList(chemElementList);

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
            if(QMessageBox::question(nullptr, xv_messageBoxPasteTitle, question, QMessageBox::Yes | QMessageBox::No)
                    == QMessageBox::Yes)
            {
                // create new chem elements
                foreach(QString str, absentChemElementList)
                {
                    currentArray->xp_appendNewChemElement(str);
                }
            }
        } // end if(!absentChemElementList.isEmpty())

        // check dimension
        QString question;
        if(xv_pasteData.xp_dataRowCount() > currentArray->xp_spectrumCount())
        {
            question = tr("The number of pasting data row exceeds the number of spectrum in the array.");
        }
        else if(xv_pasteData.xp_dataRowCount() < currentArray->xp_spectrumCount())
        {
            question = tr("The number of spectrum in the array exceeds the number of pasting data.");
        }

        if(!question.isEmpty())
        {
            question += "\nContinue anyway?";
            if(QMessageBox::question(nullptr, xv_messageBoxPasteTitle, question, QMessageBox::Yes | QMessageBox::No)
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
        for(int col = 0; col < xv_pasteData.xp_columnCount(); col++)
        {
            header = xv_pasteData.xp_columnHeader(col);
            chemElementIndex = currentArray->xp_chemElementIndexForName(header);
            chemElementId = currentArray->xp_chemElementId(chemElementIndex);

            if(chemElementId < 0 || chemElementIndex < 0 || chemElementIndex >= currentArray->xp_chemElementCount())
            {
                continue;
            }

            if(!currentArray->xp_chemElementIsVisible(chemElementIndex) && answerResult != AR_YES)
            {
                if(answerResult == AR_NOT_DEFINED)
                {
                    QString question = tr("There are hidden chemical element columns.\n"
                                          "Do you want to paste data to them?");
                    int res = QMessageBox::question(nullptr, xv_messageBoxPasteTitle, question,
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

            concentrationColumn = xv_pasteData.xp_columnData(col);
            for(int s = 0; (s < currentArray->xp_spectrumCount() && s < concentrationColumn.count()); s++)
            {
                currentArray->xp_setChemConcentration(chemElementId, s, concentrationColumn.at(s));
            }
        }
    }
    else
    {
        // data only. fill from current cell

        // get start cell to paste
        int row = -1;
        int column = -1;
        emit xg_requestCurrentChemConcentrationCellIndex(row, column);
        if(row < 0 || row >= currentArray->xp_spectrumCount()
                || column < 0 || column >= currentArray->xp_visibleChemElementCount())
        {
            QString warning = tr("Select a top left cell in a chemical concentration column for data pasting.");
            QMessageBox::warning(nullptr, xv_messageBoxPasteTitle, warning, QMessageBox::Ok);
            return;
        }

        // check dimension
        QString question;
        if(xv_pasteData.xp_dataRowCount() > currentArray->xp_spectrumCount() - row)
        {
            question = tr("The number of inserted rows exceeds the number of spectra.");
        }

        if(xv_pasteData.xp_columnCount() > currentArray->xp_visibleChemElementCount() - column)
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
            if(QMessageBox::question(nullptr, xv_messageBoxPasteTitle, question, QMessageBox::Yes | QMessageBox::No)
                    == QMessageBox::No)
            {
                return;
            }
        }

        // create chemElementIdList that corresponding with pastingData columns
        QList<qint64> chemElementIdList;
        for(int col = column; col < currentArray->xp_visibleChemElementCount(); col++)
        {
            chemElementIdList.append(currentArray->xp_visibleChemElementId(col));
        }

        // paste data
        qint64 chemElementId;
        QStringList concentrationColumn;
        for(int col = 0; col < xv_pasteData.xp_columnCount(); col++)
        {
            if(col >= chemElementIdList.count())
            {
                return;
            }
            chemElementId = chemElementIdList.at(col);
            concentrationColumn = xv_pasteData.xp_columnData(col);

            for(int s = row, cr = 0; (s < currentArray->xp_spectrumCount() && cr < concentrationColumn.count()); s++, cr++)
            {
                currentArray->xp_setChemConcentration(chemElementId, s, concentrationColumn.at(cr));
            }
        } // end pasting
    }
}
//==================================================================
void X_SpectrumArrayRepository::xh_onCopyConcentrationDataAction()
{
    QString selectedString;
    emit xg_requestSelectedString(selectedString);

    if(selectedString.isEmpty())
    {
        return;
    }

    QClipboard* clipboard = qApp->clipboard();
    clipboard->setText(selectedString);
}
//==================================================================
void X_SpectrumArrayRepository::xh_onClearConcentrationDataAction()
{
    emit xg_requestClearSelected();
}
//==================================================================
void X_SpectrumArrayRepository::xh_onSetSpectraVisibleAction()
{
    int arrayIndex = -1;
    emit xg_requestCurrentArrayIndex(arrayIndex);

    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return;
    }

    X_SpectrumArray* array = xv_arrayList.at(arrayIndex);
    bool res;
    for(int s = 0; s < array->xp_spectrumCount(); s++)
    {
        res = array->xp_setSpectrumVisible(s, true);
        if(res)
        {
            emit xg_spectrumOperation(SOT_VISIBLE_CHANGED, arrayIndex, s, s);
        }
    }
}
//==================================================================
void X_SpectrumArrayRepository::xh_onSetSpectraInvisibleAction()
{
    int arrayIndex = -1;
    emit xg_requestCurrentArrayIndex(arrayIndex);

    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return;
    }

    X_SpectrumArray* array = xv_arrayList.at(arrayIndex);
    bool res;
    for(int s = 0; s < array->xp_spectrumCount(); s++)
    {
        res = array->xp_setSpectrumVisible(s, false);
        if(res)
        {
            emit xg_spectrumOperation(SOT_VISIBLE_CHANGED, arrayIndex, s, s);
        }
    }
}
//==================================================================
void X_SpectrumArrayRepository::xh_onInvertSpectraVisibilityAction()
{
    int arrayIndex = -1;
    emit xg_requestCurrentArrayIndex(arrayIndex);

    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return;
    }

    X_SpectrumArray* array = xv_arrayList.at(arrayIndex);
    bool res;
    for(int s = 0; s < array->xp_spectrumCount(); s++)
    {
        res = array->xp_setSpectrumVisible(s, !array->xp_isSpectrumVisible(s));
        if(res)
        {
            emit xg_spectrumOperation(SOT_VISIBLE_CHANGED, arrayIndex, s, s);
        }
    }
}
//==================================================================
void X_SpectrumArrayRepository::xh_onSetChemElementsVisibleAction()
{
    int arrayIndex = -1;
    emit xg_requestCurrentArrayIndex(arrayIndex);

    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return;
    }

    X_SpectrumArray* array = xv_arrayList.at(arrayIndex);
    for(int c = 0; c < array->xp_chemElementCount(); c++)
    {
        array->xp_setChemElementVisible(c, true);
    }
}
//==================================================================
void X_SpectrumArrayRepository::xh_onSetChemElementsInvisibleAction()
{
    int arrayIndex = -1;
    emit xg_requestCurrentArrayIndex(arrayIndex);

    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return;
    }

    X_SpectrumArray* array = xv_arrayList.at(arrayIndex);
    for(int c = 0; c < array->xp_chemElementCount(); c++)
    {
        array->xp_setChemElementVisible(c, false);
    }
}
//==================================================================
void X_SpectrumArrayRepository::xh_onInvertChemElementsVisibilityAction()
{
    int arrayIndex = -1;
    emit xg_requestCurrentArrayIndex(arrayIndex);

    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return;
    }

    X_SpectrumArray* array = xv_arrayList.at(arrayIndex);
    for(int c = 0; c < array->xp_chemElementCount(); c++)
    {
        array->xp_setChemElementVisible(c, !array->xp_chemElementIsVisible(c));
    }
}
//==================================================================
void X_SpectrumArrayRepository::xh_onEnergyCalibrationAction()
{
    // create selected spectrum map
    QList<int> selectedSpectrumList;
    emit xg_requestSelectedSpectrumIndexList(selectedSpectrumList);

    if(selectedSpectrumList.isEmpty())
    {
        QString string = tr("There is no spectra for energy calibration!");
        QMessageBox::critical(nullptr, tr("Spectra removing"), string, QMessageBox::Ok);
        return;
    }

    int currentArrayIndex;
    emit xg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= xv_arrayList.count())
    {
        return;
    }

    X_SpeSpectrum* spectrum;
    QMap<quint8, QList<X_SpeSpectrum*> > spectrumMap;

    quint8 gainFactor = static_cast<quint8>(xv_arrayList.at(currentArrayIndex)->xp_gainFactor());
    QList<X_AbstractSpectrum*> spectrumList = xv_arrayList.at(currentArrayIndex)->xp_spectrumList();

    if(!spectrumMap.keys().contains(gainFactor))
    {
        spectrumMap.insert(gainFactor, QList<X_SpeSpectrum*>());
    }

    for(int s = 0; s < spectrumList.count(); s++)
    {
        if(!selectedSpectrumList.contains(s))
        {
            continue;
        }

        spectrum = qobject_cast<X_SpeSpectrum*>(spectrumList.at(s));
        if(spectrum)
        {
            spectrumMap[gainFactor].append(spectrum);
        }
    }

    X_EnergyCalibrationDialogV2 dialog(spectrumMap);
    //    connect(&dialog, &X_EnergyCalibrationDialogV2::xg_energyCalibrationChanged,
    //            this, &X_SpectrumArrayRepository::xh_writeEnergyCalibrationToSpectra);
    dialog.exec();
}
//==================================================================
void X_SpectrumArrayRepository::xh_writeEnergyCalibrationToSpectra(int gainFactor,
                                                                  const QList<double>& energyCalibrationFactors)
{
    for(int a = 0; a < xv_arrayList.count(); a++)
    {
        QList<X_AbstractSpectrum*>spectrumList = xv_arrayList.at(a)->xp_spectrumList();
        for(int s = 0; s <spectrumList.count(); s++)
        {
            X_SpeSpectrum* spectrum = qobject_cast<X_SpeSpectrum*>(spectrumList.at(s));
            if(!spectrum || spectrum->xp_gainFactor() != gainFactor)
            {
                continue;
            }

            spectrum->xp_setEnergyCalibration(energyCalibrationFactors);
        }
    }
}
//==================================================================
QList<X_RawSpectrumArray> X_SpectrumArrayRepository::xh_createRawArrayList() const
{
    QList<X_RawSpectrumArray> rawSpectrumArrayList;
    for(int a = 0; a < xv_arrayList.count(); a++)
    {
        X_RawSpectrumArray rawSpectrumArray = xv_arrayList.value(a)->xp_createRawSpectrumArray();
        rawSpectrumArrayList.append(rawSpectrumArray);
    }

    return rawSpectrumArrayList;
}
//==================================================================
void X_SpectrumArrayRepository::xh_actionEnablingControl()
{
    int arrayIndex = -1;
    emit xg_requestCurrentArrayIndex(arrayIndex);

    //    xv_appendArrayAction->setEnabled(true);
    //    xv_appendSpectrumToArrayAction->setEnabled(true);

    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        xv_removeArrayAction->setEnabled(false);
        xv_saveArrayAction->setEnabled(xv_dirty);
        xv_saveAsArrayAction->setEnabled(!xv_arrayList.isEmpty());

        xv_appendChemElementAction->setEnabled(false);
        xv_removeChemElementAction->setEnabled(false);

        xv_appendChemElementAction->setEnabled(false);
        xv_removeChemElementAction->setEnabled(false);

        //        xv_pasteConcentrationDataAction->setEnabled(false);
        //        xv_copyConcentrationDataAction->setEnabled(false);
        //        xv_clearConcentrationDataAction->setEnabled(false);

        xv_setSpectraVisibleAction->setEnabled(false);
        xv_setSpectraInvisibleAction->setEnabled(false);
        xv_invertSpectraVisibilityAction->setEnabled(false);

        xv_setChemElementsVisibleAction->setEnabled(false);
        xv_setChemElementsInvisibleAction->setEnabled(false);
        xv_invertChemElementsVisibilityAction->setEnabled(false);

        return;
    }

    xv_removeArrayAction->setEnabled(true);
    xv_saveArrayAction->setEnabled(xv_dirty);
    xv_saveAsArrayAction->setEnabled(!xv_arrayList.isEmpty());

    xv_appendChemElementAction->setEnabled(true);

    if(xv_arrayList.at(arrayIndex)->xp_spectrumCount() > 0)
    {
        xv_setSpectraVisibleAction->setEnabled(true);
        xv_setSpectraInvisibleAction->setEnabled(true);
        xv_invertSpectraVisibilityAction->setEnabled(true);
    }
    else
    {
        xv_setSpectraVisibleAction->setEnabled(false);
        xv_setSpectraInvisibleAction->setEnabled(false);
        xv_invertSpectraVisibilityAction->setEnabled(false);
    }

    if(xv_arrayList.at(arrayIndex)->xp_chemElementCount() > 0)
    {
        QList<int> selectedChemElementIndexList;
        emit xg_requestSelectedChemElementIndexList(selectedChemElementIndexList);
        xv_removeChemElementAction->setEnabled(!selectedChemElementIndexList.isEmpty());

        xv_setChemElementsVisibleAction->setEnabled(true);
        xv_setChemElementsInvisibleAction->setEnabled(true);
        xv_invertChemElementsVisibilityAction->setEnabled(true);
    }
    else
    {
        xv_removeChemElementAction->setEnabled(false);

        xv_setChemElementsVisibleAction->setEnabled(false);
        xv_setChemElementsInvisibleAction->setEnabled(false);
        xv_invertChemElementsVisibilityAction->setEnabled(false);
    }

    emit xg_arrayListDirtyChanged(xv_dirty, !xv_arrayList.isEmpty());
}
//==================================================================
void X_SpectrumArrayRepository::xh_setDirty(bool dirty)
{
    xv_dirty = dirty;
    emit xg_arrayListDirtyChanged(xv_dirty, !xv_arrayList.isEmpty());
    emit xg_currentFile(xv_dirty, xv_arrayFilePath);
    xh_actionEnablingControl();
}
//==================================================================
void X_SpectrumArrayRepository::xh_createActions()
{
    xv_appendArrayAction = new QAction(this);
    xv_appendArrayAction->setIcon(QIcon(NS_Icons::glAddArray));
    xv_appendArrayAction->setText(tr("New spectrum array"));
    xv_appendArrayAction->setToolTip(tr("Append a new spectrum array to the list"));
    xv_appendArrayAction->setEnabled(true);

    xv_openArrayAction = new QAction(this);
    xv_saveArrayAction = new QAction(this);
    xv_saveAsArrayAction = new QAction(this);

    xv_removeArrayAction = new QAction(this);
    xv_removeArrayAction->setIcon(QIcon(NS_Icons::glRemoveArrayIconString));
    xv_removeArrayAction->setText(tr("Remove current spectrum array"));
    xv_removeArrayAction->setToolTip(tr("Remove current spectrum array from the list"));
    xv_removeArrayAction->setEnabled(false);

    xv_appendSpectrumToArrayAction = new QAction(this);
    xv_appendSpectrumToArrayAction->setIcon(QIcon(NS_Icons::glAddSpectrumIconString));
    xv_appendSpectrumToArrayAction->setText(tr("Append spectra"));
    xv_appendSpectrumToArrayAction->setToolTip(tr("Append spectra to current array"));
    xv_appendSpectrumToArrayAction->setEnabled(true);

    xv_removeSpectrumFromArrayAction = new QAction(this);
    xv_removeSpectrumFromArrayAction->setIcon(QIcon(NS_Icons::glRemoveSpectrumIconString));
    xv_removeSpectrumFromArrayAction->setText(tr("Remove selected spectra"));
    xv_removeSpectrumFromArrayAction->setToolTip(tr("Remove selected spectra from the array"));
    xv_removeSpectrumFromArrayAction->setEnabled(false);

    xv_energyCalibrationAction = new QAction(this);
    xv_energyCalibrationAction->setEnabled(false);
    xv_energyCalibrationAction->setIcon(QIcon(NS_Icons::glIconEnergyCalibration));
    xv_energyCalibrationAction->setText(tr("Energy calibration"));
    xv_energyCalibrationAction->setToolTip(tr("Recalculate energy calibration for selected spectra"));

    xv_appendChemElementAction = new QAction(this);
    xv_appendChemElementAction->setIcon(QIcon(NS_Icons::glAddChemElementIconString));
    xv_appendChemElementAction->setText(tr("New chemical element"));
    xv_appendChemElementAction->setToolTip(tr("Append a new chemical element to the list"));
    xv_appendChemElementAction->setEnabled(false);

    xv_removeChemElementAction = new QAction(this);
    xv_removeChemElementAction->setIcon(QIcon(NS_Icons::glRemoveChemElementIconString));
    xv_removeChemElementAction->setText(tr("Remove current chemical element"));
    xv_removeChemElementAction->setToolTip(tr("Remove current chemical element from the list"));
    xv_removeChemElementAction->setEnabled(false);

    xv_copyConcentrationDataAction = new QAction(this);
    xv_copyConcentrationDataAction->setIcon(QIcon(NS_Icons::glCopyIconString));
    xv_copyConcentrationDataAction->setText(tr("Copy selected concentrations"));
    xv_copyConcentrationDataAction->setEnabled(false);

    xv_pasteConcentrationDataAction = new QAction(this);
    xv_pasteConcentrationDataAction->setIcon(QIcon(NS_Icons::glPasteIconString));
    xv_pasteConcentrationDataAction->setText(tr("Paste concentrations"));
    xv_pasteConcentrationDataAction->setEnabled(false);

    xv_clearConcentrationDataAction = new QAction(this);
    xv_clearConcentrationDataAction->setIcon(QIcon(NS_Icons::glClearIconString));
    xv_clearConcentrationDataAction->setText(tr("Clear selected concentrations"));
    xv_clearConcentrationDataAction->setEnabled(false);

    // visibility actions
    xv_setSpectraVisibleAction = new QAction(this);
    xv_setSpectraVisibleAction->setIcon(QIcon(NS_Icons::glIconVisible));
    xv_setSpectraVisibleAction->setText(tr("Set spectra visible"));
    xv_setSpectraVisibleAction->setEnabled(false);

    xv_setSpectraInvisibleAction = new QAction(this);
    xv_setSpectraInvisibleAction->setIcon(QIcon(NS_Icons::glIconInvisible));
    xv_setSpectraInvisibleAction->setText(tr("Set spectra invisible"));
    xv_setSpectraInvisibleAction->setEnabled(false);

    xv_invertSpectraVisibilityAction = new QAction(this);
    xv_invertSpectraVisibilityAction->setIcon(QIcon(NS_Icons::glIconInverseVisible));
    xv_invertSpectraVisibilityAction->setText(tr("Invert spectra visibility"));
    xv_invertSpectraVisibilityAction->setEnabled(false);

    xv_setChemElementsVisibleAction = new QAction(this);
    xv_setChemElementsVisibleAction->setIcon(QIcon(NS_Icons::glIconVisible));
    xv_setChemElementsVisibleAction->setText(tr("Set chemical elements visible"));
    xv_setChemElementsVisibleAction->setEnabled(false);

    xv_setChemElementsInvisibleAction = new QAction(this);
    xv_setChemElementsInvisibleAction->setIcon(QIcon(NS_Icons::glIconInvisible));
    xv_setChemElementsInvisibleAction->setText(tr("Set chemical elements invisible"));
    xv_setChemElementsInvisibleAction->setEnabled(false);

    xv_invertChemElementsVisibilityAction = new QAction(this);
    xv_invertChemElementsVisibilityAction->setIcon(QIcon(NS_Icons::glIconInverseVisible));
    xv_invertChemElementsVisibilityAction->setText(tr("Invert chemical elements visibility"));
    xv_invertChemElementsVisibilityAction->setEnabled(false);
}
//==================================================================
void X_SpectrumArrayRepository::xh_createConnections()
{
    connect(xv_appendArrayAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onAppendArrayAction);
    connect(xv_removeArrayAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onRemoveArrayAction);
    connect(xv_appendSpectrumToArrayAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onAppendSpectraToArrayAction);
    connect(xv_removeSpectrumFromArrayAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onRemoveSpectrumFromArrayAction);
    connect(xv_appendChemElementAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onAppendChemElementAction);
    connect(xv_removeChemElementAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onRemoveChemElementAction);

    connect(xv_copyConcentrationDataAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onCopyConcentrationDataAction);
    connect(xv_pasteConcentrationDataAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onPasteConcentrationDataAction);
    connect(xv_clearConcentrationDataAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onClearConcentrationDataAction);

    connect(xv_setSpectraVisibleAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onSetSpectraVisibleAction);
    connect(xv_setSpectraInvisibleAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onSetSpectraInvisibleAction);
    connect(xv_invertSpectraVisibilityAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onInvertSpectraVisibilityAction);
    connect(xv_setChemElementsVisibleAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onSetChemElementsVisibleAction);
    connect(xv_setChemElementsInvisibleAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onSetChemElementsInvisibleAction);
    connect(xv_invertChemElementsVisibilityAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onInvertChemElementsVisibilityAction);

    connect(xv_energyCalibrationAction, &QAction::triggered,
            this, &X_SpectrumArrayRepository::xh_onEnergyCalibrationAction);


    connect(qApp->clipboard(), &QClipboard::dataChanged,
            this, &X_SpectrumArrayRepository::xh_onClipboardContentChange);

}
//==================================================================
int X_SpectrumArrayRepository::xh_chemElementIndexForId(int arrayIndex, qint64 chemElementId)
{
    if(arrayIndex < 0 || arrayIndex >= xv_arrayList.count())
    {
        return -1;
    }

    return xv_arrayList.value(arrayIndex)->xp_chemElementIndexForId(chemElementId);
}
//==================================================================


