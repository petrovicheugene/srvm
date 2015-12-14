//==================================================================
#include "ZSpectraArrayRepository.h"
#include "globalVariables.h"
#include "ZFileActionManager.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QtAlgorithms>
//==================================================================
ZSpectraArrayRepository::ZSpectraArrayRepository(QObject *parent) : QObject(parent)
{
    zv_arrayFilePath = QString();
    zv_dirty = false;
    zv_defaultArrayBaseName = tr("Array #");
    zh_createActions();
    zh_createConnections();
}
//==================================================================
void ZSpectraArrayRepository::zp_appendActionsToMenu(QMenu* menu) const
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
QList<QAction*> ZSpectraArrayRepository::zp_arrayActions() const
{
    QList<QAction*> actionList;
    actionList << zv_appendArrayAction;
    actionList << zv_removeArrayAction;
    return actionList;
}
//==================================================================
QList<QAction*> ZSpectraArrayRepository::zp_spectrumActions() const
{
    QList<QAction*> actionList;
    actionList << zv_appendSpectrumToArrayAction;
    actionList << zv_removeSpectrumFromArrayAction;
    return actionList;
}
//==================================================================
QList<QAction*> ZSpectraArrayRepository::zp_chemElementActions() const
{
    QList<QAction*> actionList;
    actionList << zv_appendChemElementAction;
    actionList << zv_removeChemElementAction;
    return actionList;
}
//==================================================================
void ZSpectraArrayRepository::zp_connectToFileActionManager(ZFileActionManager* manager)
{
    connect(manager, &ZFileActionManager::zg_spectrumRawArrayList,
            this, &ZSpectraArrayRepository::zp_appendArrays);
    connect(manager, &ZFileActionManager::zg_spectrumFileListToOpen,
            this, &ZSpectraArrayRepository::zp_appendSpectraToArray);
    connect(manager, &ZFileActionManager::zg_spectrumArraySaved,
            this, &ZSpectraArrayRepository::zh_onSpectrumArraySaving);

    connect(this, &ZSpectraArrayRepository::zg_initSpectraAppending,
            manager, &ZFileActionManager::zp_defineSpectrumFilesAndInitAppending);
    connect(manager, &ZFileActionManager::zg_requestRawArrayListAndInitSaving,
            this, &ZSpectraArrayRepository::zh_createRawArrayListAndStartSaving);
    connect(this, &ZSpectraArrayRepository::zg_saveSpectraArrayList,
            manager, &ZFileActionManager::zp_saveSpectraArrayList);
}
//==================================================================
bool ZSpectraArrayRepository::zp_isEmpty() const
{
    return zv_arrayList.isEmpty();
}
//==================================================================
void ZSpectraArrayRepository::zp_clear()
{
    if(!zv_arrayList.isEmpty())
    {
        emit zg_currentArrayOperation(AOT_REMOVE_ARRAYS, 0, zv_arrayList.count() - 1);
        for(int i = zv_arrayList.count()-1; i >= 0; i-- )
        {
            delete zv_arrayList.takeAt(i);
        }
        emit zg_currentArrayOperation(AOT_END_REMOVE_ARRAYS, 0, zv_arrayList.count() - 1);
    }
    zv_dirty = false;
    zv_arrayFilePath.clear();
}
//==================================================================
int ZSpectraArrayRepository::zp_arrayCount() const
{
    return zv_arrayList.count();
}
//==================================================================
int ZSpectraArrayRepository::zp_spectrumCount(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return 0;
    }

    return zv_arrayList.value(arrayIndex)->zp_spectrumCount();
}
//==================================================================
int ZSpectraArrayRepository::zp_numberVisibleChemElementsBeforeIndex(int arrayIndex, int index) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return -1;
    }

    return zv_arrayList.value(arrayIndex)->zp_numberVisibleChemElementsBeforeIndex(index);
}
//==================================================================
int ZSpectraArrayRepository::zp_chemElementCount(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return 0;
    }

    return zv_arrayList.value(arrayIndex)->zp_chemElementCount();
}
//==================================================================
int ZSpectraArrayRepository::zp_visibleChemElementCount(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return 0;
    }

    return zv_arrayList.value(arrayIndex)->zp_visibleChemElementCount();
}
//==================================================================
QStringList ZSpectraArrayRepository::zp_chemElementList(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return QStringList();
    }

    return zv_arrayList.value(arrayIndex)->zp_chemElementList();
}
//==================================================================
QString ZSpectraArrayRepository::zp_visibleChemElementName(int arrayIndex, int visibleIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count()
            || visibleIndex < 0)
    {
        return QString();
    }

    return zv_arrayList.value(arrayIndex)->zp_visibleChemElementName(visibleIndex);
}
//==================================================================
QString ZSpectraArrayRepository::zp_chemElementName(int arrayIndex, int row) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count()
            || row < 0)
    {
        return QString();
    }

    return zv_arrayList.value(arrayIndex)->zp_chemElementName(row);
}
//==================================================================
QString ZSpectraArrayRepository::zp_chemConcentration(int arrayIndex,
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

    QString chemElement = zp_visibleChemElementName(arrayIndex, visibleChemElementIndex);
    return zv_arrayList.value(arrayIndex)->zp_chemConcentration(chemElement, spectrumIndex);
}
//==================================================================
bool ZSpectraArrayRepository::zp_setChemConcentration(int arrayIndex, int spectrumIndex,
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

    QString chemElement = zp_visibleChemElementName(arrayIndex, visibleChemElementIndex);
    bool res = zv_arrayList.value(arrayIndex)->zp_setChemConcentration(chemElement, spectrumIndex, concentration);
    if(res)
    {
        int chemElementIndex = zh_chemElementIndex(arrayIndex, chemElement);
        emit zg_currentChemElementOperation(CEOT_CHEM_ELEMENT_CHANGED,
                                            arrayIndex, chemElementIndex, chemElementIndex);
        zv_dirty = true;
        emit zg_currentFile(zv_dirty, zv_arrayFilePath);
    }

    return res;
}
//==================================================================
bool ZSpectraArrayRepository::zp_setChemElementName(int arrayIndex, int chemElementIndex, const QString& chemElementName)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return false;
    }

    return zv_arrayList.at(arrayIndex)->zp_setChemElementName(chemElementIndex, chemElementName);
}
//==================================================================
bool ZSpectraArrayRepository::zp_energyCalibration(int arrayIndex, qreal& K0, qreal& K1, qreal& K2, QString& energyUnit)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return false;
    }

    return zv_arrayList.at(arrayIndex)->zp_energyCalibration(K0, K1, K2, energyUnit);
}
//==================================================================
bool ZSpectraArrayRepository::zp_energyCalibrationForArrayId(qint64 arrayId, qreal& K0, qreal& K1, qreal& K2, QString& energyUnit)
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
qint64 ZSpectraArrayRepository::zp_arrayIdForArrayIndex(int arrayIndex)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return -1;
    }

    return zv_arrayList.at(arrayIndex)->zp_arrayId();
}
//==================================================================
bool ZSpectraArrayRepository::zp_chemElementIsVisible(int arrayIndex, int row) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count()
            || row < 0)
    {
        return false;
    }

    return zv_arrayList.value(arrayIndex)->zp_chemElementIsVisible(row);
}
//==================================================================
bool ZSpectraArrayRepository::zp_setChemElementVisible(int arrayIndex, int row, bool visible) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count()
            || row < 0)
    {
        return false;
    }

    return zv_arrayList.value(arrayIndex)->zp_setChemElementVisible(row, visible);

}
//==================================================================
QString ZSpectraArrayRepository::zp_arrayName(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return QString();
    }

    return zv_arrayList.value(arrayIndex)->zp_arrayName();
}
//==================================================================
bool ZSpectraArrayRepository::zp_setSpectrumArrayName(int arrayIndex, const QString& name)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() || name.isEmpty())
    {
        return false;
    }

    zv_arrayList[arrayIndex]->zp_setArrayName(name);
    emit zg_currentArrayOperation(AOT_DATA_CHANGED, arrayIndex, arrayIndex);

    zv_dirty = true;
    emit zg_currentFile(zv_dirty, zv_arrayFilePath);
    return true;
}
//==================================================================
QString ZSpectraArrayRepository::zp_spectrumName(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return QString();
    }

    return zv_arrayList.value(arrayIndex)->zp_spectrumFileName(spectrumIndex);
}
//======================================================
qint64 ZSpectraArrayRepository::zp_spectrumId(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return 0;
    }

    return zv_arrayList.value(arrayIndex)->zp_spectrumId(spectrumIndex);
}
//======================================================
QColor ZSpectraArrayRepository::zp_spectrumColor(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return QColor();
    }

    return zv_arrayList.value(arrayIndex)->zp_spectrumColor(spectrumIndex);
}
//======================================================
bool ZSpectraArrayRepository::zp_isSpectrumVisible(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return false;
    }

    return zv_arrayList.value(arrayIndex)->zp_isSpectrumVisible(spectrumIndex);
}
//======================================================
bool  ZSpectraArrayRepository::zp_setSpectrumVisible(int arrayIndex, int spectrumIndex, bool visible)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return false;
    }

    bool res = zv_arrayList.value(arrayIndex)->zp_setSpectrumVisible(spectrumIndex, visible);
    if(res)
    {
        emit zg_currentSpectrumOperation(SOT_DATA_CHANGED, arrayIndex, spectrumIndex, spectrumIndex);
    }
    return res;
}
//==================================================================
const ZAbstractSpectrum* ZSpectraArrayRepository::zp_spectrum(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return 0;
    }

    return zv_arrayList.value(arrayIndex)->zp_spectrum(spectrumIndex);
}
//==================================================================
QList<ZAbstractSpectrum*> ZSpectraArrayRepository::zp_spectrumListForArray(int arrayIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return QList<ZAbstractSpectrum*>();
    }

    return zv_arrayList.value(arrayIndex)->zp_spectrumList();
}
//==================================================================
QList<int> ZSpectraArrayRepository::zp_spectrumData(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return QList<int>();
    }

    return zv_arrayList.at(arrayIndex)->zp_spectrumData(spectrumIndex);
}
//==================================================================
int ZSpectraArrayRepository::zp_arrayChannelCount(int arrayIndex)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return 0;
    }

    return zv_arrayList.value(arrayIndex)->zp_maxChannelCount();
}
//==================================================================
int ZSpectraArrayRepository::zp_arrayMaxIntensity(int arrayIndex)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return 0;
    }

    return zv_arrayList.value(arrayIndex)->zp_maxIntensity();
}
//==================================================================
void ZSpectraArrayRepository::zp_appendArrays(QString path, QList<ZRawSpectrumArray> rawArrayList)
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
    emit zg_arrayFileLoaded();
}
//==================================================================
void ZSpectraArrayRepository::zp_appendSpectraToArray(int arrayIndex, QStringList fileNameList)
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
        emit zg_currentFile(zv_dirty, zv_arrayFilePath);
    }
}
//==================================================================
void ZSpectraArrayRepository::zh_createArray(const ZRawSpectrumArray& rawArray)
{
    int nextArrayIndex = zv_arrayList.count();
    emit zg_currentArrayOperation(AOT_INSERT_ARRAYS, nextArrayIndex, nextArrayIndex);

    ZSpectrumArray* array = new ZSpectrumArray(rawArray.name, this);
    zv_arrayList << array;

    connect(array, &ZSpectrumArray::zg_message,
            this, &ZSpectraArrayRepository::zg_message);
    connect(array, &ZSpectrumArray::zg_chemElementOperation,
            this, &ZSpectraArrayRepository::zh_onChemElementOperation);
    connect(array, &ZSpectrumArray::zg_spectrumOperation,
            this, &ZSpectraArrayRepository::zh_onSpectrumOperation);
    connect(array, &ZSpectrumArray::zg_energyCalibrationChanged,
            this, &ZSpectraArrayRepository::zg_energyCalibrationChanged);
    connect(array, &ZSpectrumArray::zg_arrayMaxParametersChanged,
            this, &ZSpectraArrayRepository::zg_arrayMaxParametersChanged);

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

    emit zg_currentArrayOperation(AOT_END_INSERT_ARRAYS, nextArrayIndex, nextArrayIndex);
}
//==================================================================
void ZSpectraArrayRepository::zh_removeArray(int arrayIndex)
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

    emit zg_currentArrayOperation(AOT_REMOVE_ARRAYS, arrayIndex, arrayIndex);
    delete zv_arrayList.takeAt(arrayIndex);
    emit zg_currentArrayOperation(AOT_END_REMOVE_ARRAYS, arrayIndex, arrayIndex);
#ifdef DBG
    qDebug() << "Repo Remove array: Next Array" << nextCurrentIndex;
#endif
    emit zg_setCurrentArrayIndex(nextCurrentIndex);
}
//==================================================================
void ZSpectraArrayRepository::zh_removeSpectrum(int arrayIndex, int spectrumIndex)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return;
    }

    if(spectrumIndex < 0 || spectrumIndex >= zv_arrayList.at(arrayIndex)->zp_spectrumCount())
    {
        return;
    }

    // emit zg_currentSpectrumOperation(SOT_REMOVE_SPECTRA, arrayIndex, spectrumIndex, spectrumIndex);
    zv_arrayList[arrayIndex]->zp_removeSpectrum(spectrumIndex);
    // emit zg_currentSpectrumOperation(SOT_END_REMOVE_SPECTRA, arrayIndex, spectrumIndex, spectrumIndex);
}
//==================================================================
void ZSpectraArrayRepository::zh_removeChemicalElement(int arrayIndex, int elementIndex)
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
void ZSpectraArrayRepository::zp_getArrayCount(int& arrayCount) const
{
    arrayCount = zv_arrayList.count();
}
//==================================================================
void ZSpectraArrayRepository::zp_getSpectrumCount(int arrayIndex, int& spectrumCount) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        spectrumCount = -1;
        return;
    }

    spectrumCount = zv_arrayList.value(arrayIndex)->zp_spectrumCount();
}
//==================================================================
void ZSpectraArrayRepository::zp_getArrayName(int arrayIndex, QString& arrayName) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        arrayName = QString();
        return;
    }

    arrayName = zv_arrayList.value(arrayIndex)->zp_arrayName();
}
//==================================================================
void ZSpectraArrayRepository::zp_getSpectrumName(int arrayIndex, int spectrumIndex, QString& spectrumName) const
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
void ZSpectraArrayRepository::zp_currentArrayChanged(int current, int previous)
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

#ifdef DBG
    qDebug() << "Repo Current Ch" << current << arrayId;
#endif

    emit zg_currentArrayId(arrayId, current);
}
//==================================================================
void ZSpectraArrayRepository::zh_onSpectrumArraySaving(QString filePath)
{
    zv_arrayFilePath = filePath;
    zv_dirty = false;
    emit zg_currentFile(zv_dirty, zv_arrayFilePath);
}
//==================================================================
void ZSpectraArrayRepository::zh_onAppendArrayAction()
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
void ZSpectraArrayRepository::zh_onRemoveArrayAction()
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
void ZSpectraArrayRepository::zh_onAppendSpectraToArrayAction()
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
void ZSpectraArrayRepository::zh_onRemoveSpectrumFromArrayAction()
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
}
//==================================================================
void ZSpectraArrayRepository::zh_onAppendChemElementAction()
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
void ZSpectraArrayRepository::zh_onRemoveChemElementAction()
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
void ZSpectraArrayRepository::zh_onChemElementOperation(ZChemElementList::OperationType type, int first, int last)
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

            emit zg_currentChemElementOperation(chemElementOperationType, i, first, last);
            return;
        }
    }
}
//==================================================================
void ZSpectraArrayRepository::zh_createRawArrayListAndStartSaving(QString filePath) const
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
void ZSpectraArrayRepository::zh_onSpectrumOperation(ZSpectrumArray::OperationType type, int first, int last)
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
            emit zg_currentSpectrumOperation(SOT_INSERT_SPECTRA, arrayIndex, first, last);
        }
        else if(type == ZSpectrumArray::OT_END_INSERT_SPECTRA)
        {
            emit zg_currentSpectrumOperation(SOT_END_INSERT_SPECTRA, arrayIndex, first, last);
        }
        else if(type == ZSpectrumArray::OT_REMOVE_SPECTRA)
        {
            emit zg_currentSpectrumOperation(SOT_REMOVE_SPECTRA, arrayIndex, first, last);
        }
        else if(type == ZSpectrumArray::OT_END_REMOVE_SPECTRA)
        {
            emit zg_currentSpectrumOperation(SOT_END_REMOVE_SPECTRA, arrayIndex, first, last);
        }

        break;
    }
}
//==================================================================
QList<ZRawSpectrumArray> ZSpectraArrayRepository::zh_createRawArrayList() const
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
void ZSpectraArrayRepository::zh_createActions()
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
void ZSpectraArrayRepository::zh_createConnections()
{
    connect(zv_appendArrayAction, &QAction::triggered,
            this, &ZSpectraArrayRepository::zh_onAppendArrayAction);
    connect(zv_removeArrayAction, &QAction::triggered,
            this, &ZSpectraArrayRepository::zh_onRemoveArrayAction);
    connect(zv_appendSpectrumToArrayAction, &QAction::triggered,
            this, &ZSpectraArrayRepository::zh_onAppendSpectraToArrayAction);
    connect(zv_removeSpectrumFromArrayAction, &QAction::triggered,
            this, &ZSpectraArrayRepository::zh_onRemoveSpectrumFromArrayAction);
    connect(zv_appendChemElementAction, &QAction::triggered,
            this, &ZSpectraArrayRepository::zh_onAppendChemElementAction);
    connect(zv_removeChemElementAction, &QAction::triggered,
            this, &ZSpectraArrayRepository::zh_onRemoveChemElementAction);
}
//==================================================================
int ZSpectraArrayRepository::zh_chemElementIndex(int arrayIndex, const QString& chemElement)
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count())
    {
        return -1;
    }

    return zv_arrayList.value(arrayIndex)->zp_chemElementCount();
}
//==================================================================


