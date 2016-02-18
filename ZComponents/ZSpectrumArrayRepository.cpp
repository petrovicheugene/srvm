//==================================================================
#include "ZSpectrumArrayRepository.h"
#include "globalVariables.h"
#include "ZFileActionManager.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QtAlgorithms>
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
        menu->addAction(zv_appendArrayAction);
        menu->addAction(zv_removeArrayAction);
        menu->addSeparator();
        menu->addAction(zv_appendSpectrumToArrayAction);
        menu->addAction(zv_removeSpectrumFromArrayAction);
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
QList<QAction*> ZSpectrumArrayRepository::zp_spectrumActions() const
{
    QList<QAction*> actionList;
    actionList << zv_appendSpectrumToArrayAction;
    actionList << zv_removeSpectrumFromArrayAction;
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
void ZSpectrumArrayRepository::zp_connectToFileActionManager(ZFileActionManager* manager)
{
    connect(manager, &ZFileActionManager::zg_spectrumRawArrayList,
            this, &ZSpectrumArrayRepository::zp_appendArrays);
    connect(manager, &ZFileActionManager::zg_spectrumFileListToOpen,
            this, &ZSpectrumArrayRepository::zp_appendSpectraToArray);
    connect(manager, &ZFileActionManager::zg_spectrumArraySaved,
            this, &ZSpectrumArrayRepository::zh_onSpectrumArraySaving);

    connect(this, &ZSpectrumArrayRepository::zg_initSpectraAppending,
            manager, &ZFileActionManager::zp_defineSpectrumFilesAndInitAppending);
    connect(manager, &ZFileActionManager::zg_requestRawArrayListAndInitSaving,
            this, &ZSpectrumArrayRepository::zh_createRawArrayListAndStartSaving);
    connect(this, &ZSpectrumArrayRepository::zg_saveSpectraArrayList,
            manager, &ZFileActionManager::zp_saveSpectraArrayList);
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
    zv_dirty = false;
    zv_arrayFilePath.clear();
}
//==================================================================
const ZSpectrumArray* ZSpectrumArrayRepository::zp_array(int arrayIndex)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return 0;
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

    for(int a = 0; a < zv_arrayList.count(); a++)
    {
        for(int c = 0; c < zv_arrayList.at(a)->zp_chemElementCount(); c++)
        {
            if(zv_arrayList.at(a)->zp_chemElementName(c) == name)
            {
                return zv_arrayList.at(a)->zp_chemElementId(c);
            }
        }
    }

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
        int chemElementIndex = zh_chemElementIndexForId(arrayIndex, chemElementId);
        emit zg_chemElementOperation(CEOT_CHEM_ELEMENT_CHANGED,
                                     arrayIndex, chemElementIndex, chemElementIndex);
        zv_dirty = true;
        emit zg_currentFile(zv_dirty, zv_arrayFilePath);
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
bool ZSpectrumArrayRepository::zp_energyCalibration(int arrayIndex, qreal& K0, qreal& K1, qreal& K2, QString& energyUnit)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return false;
    }

    return zv_arrayList.at(arrayIndex)->zp_energyCalibration(K0, K1, K2, energyUnit);
}
//==================================================================
bool ZSpectrumArrayRepository::zp_energyCalibrationForArrayId(qint64 arrayId, qreal& K0, qreal& K1, qreal& K2, QString& energyUnit)
{
    for(int i = 0; i < zv_arrayList.count(); i++)
    {
        if(zv_arrayList.at(i)->zp_arrayId() == arrayId)
        {
            return zp_energyCalibration(i, K0, K1, K2, energyUnit);
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
bool ZSpectrumArrayRepository::zp_averageChemConcentration(int arrayIndex, const QString& chemElement, qreal& averageValue ) const
{
    return zv_arrayList.at(arrayIndex)->zp_averageChemConcentration(chemElement, averageValue);
}
//==================================================================
bool ZSpectrumArrayRepository::zp_averageChemConcentration(int arrayIndex, int chemElementindex, qreal& averageValue ) const
{
    return zv_arrayList.at(arrayIndex)->zp_averageChemConcentration(chemElementindex, averageValue);
}
//==================================================================
bool ZSpectrumArrayRepository::zp_averageChemConcentrationForChemElementId(int arrayIndex, qint64 chemElementId , qreal& averageValue ) const
{
    return zv_arrayList.at(arrayIndex)->zp_averageChemConcentrationForChemElementId(chemElementId, averageValue);
}
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

    zv_dirty = true;
    emit zg_currentFile(zv_dirty, zv_arrayFilePath);
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
bool ZSpectrumArrayRepository::zp_isSpectrumChecked(qint64 arrayId, int spectrumIndex) const
{
    for(int i = 0; i < zv_arrayList.count(); i++)
    {
        if(zv_arrayList.at(i)->zp_arrayId() != arrayId)
        {
            continue;
        }

        return zv_arrayList.at(arrayId)->zp_isSpectrumChecked(spectrumIndex);
    }
    return false;
}
//==================================================================
bool ZSpectrumArrayRepository::zp_setSpectrumChecked(qint64 arrayId, int spectrumIndex, bool checked)
{
    for(int i = 0; i < zv_arrayList.count(); i++)
    {
        if(zv_arrayList.at(i)->zp_arrayId() != arrayId)
        {
            continue;
        }

        bool res = zv_arrayList.at(arrayId)->zp_setSpectrumChecked(spectrumIndex, checked);
        if(res)
        {
            emit zg_spectrumOperation(SOT_CHECKED_CHANGED, i, spectrumIndex, spectrumIndex);
        }
        return res;
    }
    return false;
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
const ZAbstractSpectrum* ZSpectrumArrayRepository::zp_spectrum(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return 0;
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
QList<int> ZSpectrumArrayRepository::zp_spectrumData(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return QList<int>();
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

            if(QMessageBox::question(0, tr("Array appending"), question, QMessageBox::Yes, QMessageBox::No)
                    == QMessageBox::Yes)
            {
                // TODO SAVING ARRAY LIST TO FILE
                // zp_saveArrayListToFile();
            }
        }

        if(zv_arrayList.count() > 0)
        {
            QString question = tr("Clear existing array list?");
            if(QMessageBox::question(0, tr("Array appending"), question, QMessageBox::Yes, QMessageBox::No)
                    == QMessageBox::Yes)
            {
                zp_clear();
            }
            else
            {
                changePath = false;
                zv_dirty = true;
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
    for(int i = 0; i < fileNameList.count(); i++)
    {
        QFileInfo fileInfo(fileNameList.at(i));
        if(!fileInfo.exists())
        {
            QString msg = tr("File \"%1\" does not exist!").arg(zv_arrayList.at(arrayIndex)->zp_arrayName());
            emit zg_message(msg);
            continue;
        }

        if(fileInfo.isDir())
        {
            QString msg = tr("\"%1\" is a folder!").arg(zv_arrayList.at(arrayIndex)->zp_arrayName());
            emit zg_message(msg);
            continue;
        }

        ZRawSpectrum rawSpectrum;
        rawSpectrum.path = fileNameList.at(i);

        // int spectrumIndex = zv_arrayList[arrayIndex]->zp_spectrumCount();
        // emit zg_currentSpectrumOperation(SOT_INSERT_SPECTRA, arrayIndex, spectrumIndex, spectrumIndex);
        if(!zv_arrayList[arrayIndex]->zp_appendSpectrum(rawSpectrum, fileNameList.count()-1 == i, dontAsk))
        {
            if(dontAsk)
            {
                break;
            }
        }
        // emit zg_currentSpectrumOperation(SOT_END_INSERT_SPECTRA, arrayIndex, spectrumIndex, spectrumIndex);
    }

    int loaded = zv_arrayList.at(arrayIndex)->zp_spectrumCount() - spectraStartCount;
    if(loaded > 0)
    {
        QString msg = tr("%1 spectra loaded", "", 3).arg(QString::number(loaded));
        emit zg_message(msg);
        zv_dirty = true;
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
        emit zg_currentFile(zv_dirty, zv_arrayFilePath);
    }
}
//==================================================================
void ZSpectrumArrayRepository::zh_createArray(const ZRawSpectrumArray& rawArray)
{
    int nextArrayIndex = zv_arrayList.count();
    emit zg_spectrumArrayOperation(AOT_INSERT_ARRAYS, nextArrayIndex, nextArrayIndex);

    ZSpectrumArray* array = new ZSpectrumArray(rawArray.name, this);
    zv_arrayList << array;

    connect(array, &ZSpectrumArray::zg_message,
            this, &ZSpectrumArrayRepository::zg_message);
    connect(array, &ZSpectrumArray::zg_chemElementOperation,
            this, &ZSpectrumArrayRepository::zh_onChemElementOperation);
    connect(array, &ZSpectrumArray::zg_spectrumOperation,
            this, &ZSpectrumArrayRepository::zh_onSpectrumOperation);
    connect(array, &ZSpectrumArray::zg_energyCalibrationChanged,
            this, &ZSpectrumArrayRepository::zg_energyCalibrationChanged);
    connect(array, &ZSpectrumArray::zg_arrayMaxParametersChanged,
            this, &ZSpectrumArrayRepository::zg_arrayMaxParametersChanged);

    bool dontAsk = false;
    for(int s = 0; s < rawArray.spectrumList.count(); s++)
    {
        if(array->zp_appendSpectrum(rawArray.spectrumList.value(s), rawArray.spectrumList.count()-1 == s, dontAsk))
        {
            if(dontAsk)
            {
                break;
            }
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
    delete zv_arrayList.takeAt(arrayIndex);
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
void ZSpectrumArrayRepository::zh_removeChemicalElement(int arrayIndex, int elementIndex)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return;
    }

    if(elementIndex < 0 || elementIndex >= zv_arrayList.at(arrayIndex)->zp_chemElementCount())
    {
        return;
    }

    if(zv_arrayList[arrayIndex]->zp_removeChemElement(elementIndex))
    {
        zv_dirty = true;
        emit zg_currentFile(zv_dirty, zv_arrayFilePath);
    }
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

    emit zg_currentArrayIsAboutChange(arrayId, current);
    emit zg_currentArrayIdChanged(arrayId, current);

}
//==================================================================
void ZSpectrumArrayRepository::zp_currentSpectrumChanged(int currentSpectrumIndex, int previousSpectrumIndex)
{
    int currentArrayIndex;
    qint64 currentSpectrumId;
    qint64 previousSpectrumId;

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
}
//==================================================================
void ZSpectrumArrayRepository::zp_chemElementListForCurrentArray(QStringList& chemElementList)
{
    int currentArray;
    emit zg_requestCurrentArrayIndex(currentArray);
    chemElementList = zp_chemElementList(currentArray);
}
//==================================================================
void ZSpectrumArrayRepository::zh_onSpectrumArraySaving(QString filePath)
{
    zv_arrayFilePath = filePath;
    zv_dirty = false;
    emit zg_currentFile(zv_dirty, zv_arrayFilePath);
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
    zv_dirty = true;
    emit zg_currentFile(zv_dirty, zv_arrayFilePath);
    emit zg_setCurrentArrayIndex(zv_arrayList.count() - 1);
    emit zg_startCurrentArrayEdition();
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
    if(QMessageBox::question(0, tr("Array removing"), question, QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::Yes)
    {
        zh_removeArray(currentArrayIndex);
        zv_dirty = true;
        emit zg_currentFile(zv_dirty, zv_arrayFilePath);
    }
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
        QMessageBox::critical(0, tr("Spectra appending"), string, QMessageBox::Ok);
        return;
    }

    emit zg_initSpectraAppending(currentArrayIndex);

}
//==================================================================
void ZSpectrumArrayRepository::zh_onRemoveSpectrumFromArrayAction()
{
    QList<int> selectedSpectrumList;
    emit zg_requestSelectedSpectrumIndexList(selectedSpectrumList);

    if(selectedSpectrumList.isEmpty())
    {
        QString string = tr("There is no spectra to remove!");
        QMessageBox::critical(0, tr("Spectra removing"), string, QMessageBox::Ok);
        return;
    }

    int currentArrayIndex;
    emit zg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= zv_arrayList.count())
    {
        return;
    }

    QString question = tr("Do you want to remove the selected spectra from the list?");
    if(QMessageBox::question(0, tr("Spectra removing"), question, QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::No)
    {
        return;
    }
    qSort(selectedSpectrumList);
    for(int i = selectedSpectrumList.count() - 1; i >= 0; i--)
    {
        zh_removeSpectrum(currentArrayIndex, selectedSpectrumList.value(i));
    }

    bool plotIsScaled;
    emit zg_requestIsPlotScaled(plotIsScaled);
    if(!plotIsScaled)
    {
        emit zg_fitPlotInBoundingRect();
    }
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
        zv_dirty = true;
        emit zg_currentFile(zv_dirty, zv_arrayFilePath);
        emit zg_setCurrentChemElementIndex(zv_arrayList.at(currentArrayIndex)->zp_chemElementCount() - 1);
        emit zg_startCurrentChemElementEdition();
    }
}
//==================================================================
void ZSpectrumArrayRepository::zh_onRemoveChemElementAction()
{
    QList<int> selectedChemElementList;
    emit zg_requestSelectedChemElementIndexList(selectedChemElementList);

    if(selectedChemElementList.isEmpty())
    {
        QString string = tr("There is no chemical elements to remove!");
        QMessageBox::critical(0, tr("Chemical element removing"), string, QMessageBox::Ok);
        return;
    }

    int currentArrayIndex;
    emit zg_requestCurrentArrayIndex(currentArrayIndex);
    if(currentArrayIndex < 0 || currentArrayIndex >= zv_arrayList.count())
    {
        return;
    }

    QString question = tr("Do you want to remove the selected chemical elemnts from the list?");
    if(QMessageBox::question(0, tr("Chemical elements removing"), question, QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::No)
    {
        return;
    }

    qSort(selectedChemElementList);
    for(int i = selectedChemElementList.count() - 1; i >= 0; i--)
    {
        zh_removeChemicalElement(currentArrayIndex, selectedChemElementList.value(i));
    }
}
//==================================================================
void ZSpectrumArrayRepository::zh_onChemElementOperation(ZChemElementList::OperationType type, int first, int last)
{
    QObject* signalSender = sender();
    if(signalSender == 0)
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
void ZSpectrumArrayRepository::zh_createRawArrayListAndStartSaving(QString filePath) const
{
    if(filePath.isEmpty())
    {
        filePath = zv_arrayFilePath;
    }

    QList<ZRawSpectrumArray> rawSpectrumArrayList = zh_createRawArrayList();
    emit zg_saveSpectraArrayList(filePath, rawSpectrumArrayList);
    // TODO after saving FileActionManager must send back a signal for reset dirty flag
    // TODO signal from fileActionManager with saved filePath. zv_path have to be canged
}
//==================================================================
void ZSpectrumArrayRepository::zh_onSpectrumOperation(ZSpectrumArray::OperationType type, int first, int last)
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
            emit zg_spectrumOperation(SOT_INSERT_SPECTRA, arrayIndex, first, last);
        }
        else if(type == ZSpectrumArray::OT_END_INSERT_SPECTRA)
        {
            emit zg_spectrumOperation(SOT_END_INSERT_SPECTRA, arrayIndex, first, last);
        }
        else if(type == ZSpectrumArray::OT_REMOVE_SPECTRA)
        {
            emit zg_spectrumOperation(SOT_REMOVE_SPECTRA, arrayIndex, first, last);
        }
        else if(type == ZSpectrumArray::OT_END_REMOVE_SPECTRA)
        {
            emit zg_spectrumOperation(SOT_END_REMOVE_SPECTRA, arrayIndex, first, last);
        }

        break;
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
void ZSpectrumArrayRepository::zh_createActions()
{
    zv_appendArrayAction = new QAction(this);
    zv_appendArrayAction->setIcon(QIcon(":/images/addGreen.png"));
    zv_appendArrayAction->setText(tr("Append a new spectrum array"));
    zv_appendArrayAction->setToolTip(tr("Append a new spectrum array to the list"));

    zv_removeArrayAction = new QAction(this);
    zv_removeArrayAction->setIcon(QIcon(":/images/ARemove3Green.png"));
    zv_removeArrayAction->setText(tr("Remove current spectrum array"));
    zv_removeArrayAction->setToolTip(tr("Remove current spectrum array from the list"));

    zv_appendSpectrumToArrayAction = new QAction(this);
    zv_appendSpectrumToArrayAction->setIcon(QIcon(":/images/addGreen.png"));
    zv_appendSpectrumToArrayAction->setText(tr("Append spectra"));
    zv_appendSpectrumToArrayAction->setToolTip(tr("Append spectra to current array"));

    zv_removeSpectrumFromArrayAction = new QAction(this);
    zv_removeSpectrumFromArrayAction->setIcon(QIcon(":/images/SRemove3Purple.png"));
    zv_removeSpectrumFromArrayAction->setText(tr("Remove selected spectra"));
    zv_removeSpectrumFromArrayAction->setToolTip(tr("Remove selected spectra from the array"));

    zv_appendChemElementAction = new QAction(this);
    zv_appendChemElementAction->setIcon(QIcon(":/images/addGreen.png"));
    zv_appendChemElementAction->setText(tr("Append a new chemical element"));
    zv_appendChemElementAction->setToolTip(tr("Append a new chemical element to the list"));

    zv_removeChemElementAction = new QAction(this);
    zv_removeChemElementAction->setIcon(QIcon(":/images/ARemove3Green.png"));
    zv_removeChemElementAction->setText(tr("Remove current chemical element"));
    zv_removeChemElementAction->setToolTip(tr("Remove current chemical element from the list"));

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


