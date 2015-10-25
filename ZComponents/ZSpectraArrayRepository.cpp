//==================================================================
#include "ZSpectraArrayRepository.h"
#include <QMessageBox>
//==================================================================
ZSpectraArrayRepository::ZSpectraArrayRepository(QObject *parent) : QObject(parent)
{
    zv_path = QString();
}
//==================================================================
bool ZSpectraArrayRepository::zp_isEmpty()
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
