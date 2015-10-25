//==================================================================
#include "ZSpectraArrayRepository.h"
#include "glVariables.h"
#include <QMessageBox>

//==================================================================
ZSpectraArrayRepository::ZSpectraArrayRepository(QObject *parent) : QObject(parent)
{
    zv_path = QString();
    zv_dirty = false;
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
bool ZSpectraArrayRepository::zp_isEmpty() const
{
    return zv_arrayList.isEmpty();
}
//==================================================================
void ZSpectraArrayRepository::zp_clear()
{
    if(!zv_arrayList.isEmpty())
    {
        emit zg_currentOperation(OT_REMOVE_ARRAYS, 0, zv_arrayList.count() - 1);
        qDeleteAll(zv_arrayList);
        zv_arrayList.clear();
        emit zg_currentOperation(OT_END_REMOVE_ARRAYS, 0, 0);
    }
    zv_dirty = false;
    zv_path.clear();
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
        return -1;
    }

    return zv_arrayList.value(arrayIndex)->zp_spectrumCount();
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
QString ZSpectraArrayRepository::zp_spectrumName(int arrayIndex, int spectrumIndex) const
{
    if(arrayIndex < 0 || arrayIndex >= zv_arrayList.count() )
    {
        return QString();
    }

    if(spectrumIndex < 0 || spectrumIndex >= zv_arrayList.value(arrayIndex)->zp_spectrumCount())
    {
        return QString();
    }

    return zv_arrayList.value(arrayIndex)->zp_spectrumFileName(spectrumIndex);
}
//==================================================================
QList<int> ZSpectraArrayRepository::zp_spectrum(int arrayIndex, int spectrumIndex) const
{
    return zv_arrayList.value(arrayIndex)->zp_spectrumData(spectrumIndex);
}
//==================================================================
void ZSpectraArrayRepository::zp_appendArrays(QString path, QList<ZRawArray> rawArrayList)
{
    bool changePath = true;
    if(path == zv_path || (zv_path.isEmpty() && !zv_dirty))
    {
        zp_clear();
    }
    else
    {
        if(zv_dirty)
        {
            QString question;
            if(!zv_path.isEmpty())
            {
                question = tr("File \"%1\" has been changed. ").arg(zv_path);
            }
            question += tr("Do you want to save the changes?");

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

    foreach(ZRawArray rawArray, rawArrayList)
    {
        zp_createArray(rawArray);
    }

    if(changePath)
    {
        zv_path = path;
    }
    emit zg_currentFile(zv_dirty, zv_path);
}
//==================================================================
void ZSpectraArrayRepository::zp_createArray(const ZRawArray& rawArray)
{
    emit zg_currentOperation(OT_INSERT_ARRAYS, zv_arrayList.count(), zv_arrayList.count());

    ZSpectrumArray* array = new ZSpectrumArray(rawArray.name, this);
    zv_arrayList << array;

    connect(array, &ZSpectrumArray::zg_message,
            this, &ZSpectraArrayRepository::zg_message);

    for(int s = 0; s < rawArray.spectrumList.count(); s++)
    {
        array->zp_appendSpectrum(rawArray.spectrumList.value(s));
    }

    emit zg_currentOperation(OT_END_INSERT_ARRAYS, zv_arrayList.count() - 1, zv_arrayList.count() - 1);

}
//==================================================================
void ZSpectraArrayRepository::zh_removeArray(int arrayIndex)
{
    int nextCurrentIndex;
    if(arrayIndex == zv_arrayList.count() - 1 && zv_arrayList.count() > 1)
    {
        nextCurrentIndex = zv_arrayList.count() - 2;
    }
    else
    {
        nextCurrentIndex = arrayIndex;
    }

    emit zg_currentOperation(OT_REMOVE_ARRAYS, arrayIndex, arrayIndex);

    delete zv_arrayList.takeAt(arrayIndex);

    emit zg_currentOperation(OT_END_REMOVE_ARRAYS, -1, -1);
    emit zg_setCurrentArrayIndex(nextCurrentIndex);
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
void ZSpectraArrayRepository::zh_onAppendArrayAction()
{
#ifdef DBG
    qDebug() << "APPEND ARRAY";
#endif

    ZRawArray rawArray;
    rawArray.name = tr("Array #")+QString::number(zv_arrayList.count() + 1);
    zp_createArray(rawArray);
    zv_dirty = true;
    emit zg_currentFile(zv_dirty, zv_path);
    emit zg_setCurrentArrayIndex(zv_arrayList.count() - 1);
}
//==================================================================
void ZSpectraArrayRepository::zh_onRemoveArrayAction()
{
#ifdef DBG
    qDebug() << "REMOVE ARRAY";
#endif
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
    }
}
//==================================================================
void ZSpectraArrayRepository::zh_onAppendSpectrumToArrayAction()
{
#ifdef DBG
    qDebug() << "APPEND SPECTRUM";
#endif
}
//==================================================================
void ZSpectraArrayRepository::zh_onRemoveSpectrumFromArrayAction()
{
#ifdef DBG
    qDebug() << "REMOVE SPECTRUM";
#endif
}
//==================================================================
void ZSpectraArrayRepository::zh_createActions()
{
    zv_appendArrayAction = new QAction(this);
    zv_appendArrayAction->setIcon(QIcon(":/images/addGreen.png"));
    zv_appendArrayAction->setText(tr("Append a new array"));
    zv_appendArrayAction->setToolTip(tr("Append a new array to the list"));

    zv_removeArrayAction = new QAction(this);
    zv_removeArrayAction->setIcon(QIcon(":/images/trashBasket.png"));
    zv_removeArrayAction->setText(tr("Remove current array"));
    zv_removeArrayAction->setToolTip(tr("Remove current array from the list"));

    zv_appendSpectrumToArrayAction = new QAction(this);
    zv_appendSpectrumToArrayAction->setIcon(QIcon(":/images/addGreen.png"));
    zv_appendSpectrumToArrayAction->setText(tr("Append spectrum"));
    zv_appendSpectrumToArrayAction->setToolTip(tr("Append a spectrum to current array"));

    zv_removeSpectrumFromArrayAction = new QAction(this);
    zv_removeSpectrumFromArrayAction->setIcon(QIcon(":/images/trashBasket.png"));
    zv_removeSpectrumFromArrayAction->setText(tr("Remove current spectrum"));
    zv_removeSpectrumFromArrayAction->setToolTip(tr("Remove current spectrum from the array"));
}
//==================================================================
void ZSpectraArrayRepository::zh_createConnections()
{
    connect(zv_appendArrayAction, &QAction::triggered,
            this, &ZSpectraArrayRepository::zh_onAppendArrayAction);
    connect(zv_removeArrayAction, &QAction::triggered,
            this, &ZSpectraArrayRepository::zh_onRemoveArrayAction);
    connect(zv_appendSpectrumToArrayAction, &QAction::triggered,
            this, &ZSpectraArrayRepository::zh_onAppendSpectrumToArrayAction);
    connect(zv_removeSpectrumFromArrayAction, &QAction::triggered,
            this, &ZSpectraArrayRepository::zh_onRemoveSpectrumFromArrayAction);

}
//==================================================================

