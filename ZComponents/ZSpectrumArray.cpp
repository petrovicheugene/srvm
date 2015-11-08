//===============================================
#include "ZSpectrumArray.h"
#include "ZSpeSpectrum.h"
#include "ZSpeIOHandler.h"
#include <QFileInfo>
#include <QFile>
//===============================================
ZSpectrumArray::ZSpectrumArray(const QString& name, QObject* parent)  : QObject(parent)
{
    zv_arrayName = name;
    zh_createConnections();
}
//===============================================
QString ZSpectrumArray::zp_arrayName() const
{
    return zv_arrayName;
}
//===============================================
void ZSpectrumArray::zp_setArrayName(const QString& name)
{
    zv_arrayName = name;
}
//===============================================
ZRawSpectrumArray ZSpectrumArray::zp_createRawSpectrumArray()
{
    ZRawSpectrumArray rawSpectrumArray;
    rawSpectrumArray.name = zv_arrayName;

    for(int s = 0; s < zv_spectrumList.count(); s++)
    {
        ZRawSpectrum rawSpectrum;
        rawSpectrum.path = zv_spectrumList.value(s)->zp_path();
        // concentration list
        for(int e = 0; e < zv_chemElementList.zp_chemElementCount(); e++)
        {
            QString chemElement = zv_chemElementList.zp_chemElementName(e);
            if(chemElement.isEmpty())
            {
                continue;
            }
            QString concentration = zv_spectrumList.value(s)->zp_concentration(chemElement);
            rawSpectrum.concentrationMap.insert(chemElement, concentration);
        }

        rawSpectrumArray.spectrumList << rawSpectrum;
    }

    return rawSpectrumArray;
}
//===============================================
int ZSpectrumArray::zp_spectrumCount() const
{
    return zv_spectrumList.count();
}
//===============================================
int ZSpectrumArray::zp_chemElementCount() const
{
    return zv_chemElementList.zp_chemElementCount();
}
//===============================================
int ZSpectrumArray::zp_chemElementIndex(const QString& chemElement) const
{
    for(int i = 0; i < zv_chemElementList.zp_chemElementCount(); i++)
    {
        if(zv_chemElementList.zp_chemElementName(i) == chemElement)
        {
            return i;
        }
    }
    return -1;
}
//===============================================
int ZSpectrumArray::zp_numberVisibleChemElementsBeforeIndex(int index) const
{
    return zv_chemElementList.zp_numberVisibleChemElementsBeforeIndex(index);
}
//===============================================
int ZSpectrumArray::zp_visibleChemElementCount() const
{
    return zv_chemElementList.zp_visibleElementCount();
}
//===============================================
QStringList ZSpectrumArray::zp_chemElementList() const
{
    return zv_chemElementList.zp_chemElementList();
}
//===============================================
QString ZSpectrumArray::zp_visibleChemElementName(int visibleIndex) const
{
    return zv_chemElementList.zp_visibleChemElementName(visibleIndex);
}
//===============================================
QString ZSpectrumArray::zp_chemElementName(int index) const
{
    return zv_chemElementList.zp_chemElementName(index);
}
//===============================================
bool ZSpectrumArray::zp_chemElementIsVisible(int index) const
{
    return zv_chemElementList.zp_chemElementIsVisible(index);
}
//===============================================
bool ZSpectrumArray::zp_setChemElementVisible(int row, bool visible)
{
    return zv_chemElementList.zp_setChemElementVisible(row, visible);
}
//===============================================
bool ZSpectrumArray::zp_setChemElementName(int row, const QString& name)
{
    return zv_chemElementList.zp_setChemElementName(row, name);
}
//===============================================
QString ZSpectrumArray::zp_chemConcentration(const QString& chemElement,
                                             int spectrumIndex) const
{
    if(spectrumIndex < 0 || spectrumIndex >= zp_spectrumCount()
            || !zv_chemElementList.zp_containsElement(chemElement))
    {
        return QString();
    }

    return zv_spectrumList.value(spectrumIndex)->zp_concentration(chemElement);
}
//===============================================
bool ZSpectrumArray::zp_setChemConcentration(const QString& chemElement,
                             int spectrumIndex, const QString& concentration)
{
    if(spectrumIndex < 0 || spectrumIndex >= zp_spectrumCount()
            || !zv_chemElementList.zp_containsElement(chemElement))
    {
        return false;
    }

    return zv_spectrumList.at(spectrumIndex)->zp_setConcentration(chemElement, concentration);
}
//===============================================
QString ZSpectrumArray::zp_spectrumFileName(int index) const
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return QString();
    }
    return zv_spectrumList.value(index)->zp_name();
}
//===============================================
QList<int> ZSpectrumArray::zp_spectrumIntensityArray(int index) const
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return QList<int>();
    }

    return zv_spectrumList.value(index)->zp_spectrumIntensityArray();
}
//===============================================
bool ZSpectrumArray::zp_removeSpectrum(int index)
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return false;
    }

    delete zv_spectrumList.takeAt(index);
    return true;
}
//===============================================
void ZSpectrumArray::zp_clearArray()
{
    qDeleteAll(zv_spectrumList);
    zv_spectrumList.clear();
}
//===============================================
bool ZSpectrumArray::zp_appendSpectrum(const ZRawSpectrum& rawSpectrum)
{
    QFileInfo fileInfo(rawSpectrum.path);
    ZAbstractSpectrumIOHandler* ioHandler;

    QString suffix = fileInfo.suffix();
    if(!fileInfo.exists() || !fileInfo.isFile())
    {
        QString error = tr("Error: \"%1\" is not a file!").arg(rawSpectrum.path);
        emit zg_message(error);
        return false;
    }
    else if(suffix == "spe")
    {
        ioHandler = new ZSpeIOHandler(this, this);
    }
    else
    {
        QString error = QObject::tr("Cannot handle file of type \"%1\"!").arg(suffix);
        emit zg_message(error);
        return false;
    }

    ZAbstractSpectrum* abstractSpectrum;
    bool res = ioHandler->zp_getSpectrumFromFile(rawSpectrum.path, abstractSpectrum);
    if(res)
    {
        ZSpeSpectrum* speSpectrum = qobject_cast<ZSpeSpectrum*>(abstractSpectrum);
        zv_spectrumList.append(speSpectrum);

        QMap<QString, QString>::const_iterator it;

        for(it = rawSpectrum.concentrationMap.begin(); it != rawSpectrum.concentrationMap.end(); it++)
        {
            speSpectrum->zp_insertConcentration(it.key(), it.value());
            if(!zv_chemElementList.zp_containsElement(it.key()))
            {
                // TODO signals column number increased
                zv_chemElementList.zp_appendElement(it.key());
            }
        }
    }

    delete ioHandler;
    return res;
}
//===============================================
bool ZSpectrumArray::zp_appendNewChemElement()
{
    QString chemElement = "Element #";
    int nextElementIndex = zv_chemElementList.zp_chemElementCount() + 1;
    while(zv_chemElementList.zp_containsElement(chemElement+QString::number(nextElementIndex)))
    {
        nextElementIndex++;
    }

    return zv_chemElementList.zp_appendElement(chemElement+QString::number(nextElementIndex));
}
//===============================================
bool ZSpectrumArray::zp_removeChemElement(int chemElementIndex)
{
    if(chemElementIndex < 0 || chemElementIndex >= zv_chemElementList.zp_chemElementCount())
    {
        return false;
    }

    QString chemElement = zv_chemElementList.zp_chemElementName(chemElementIndex);
    if(chemElement.isEmpty())
    {
        return false;
    }

    return zv_chemElementList.zp_removeElement(chemElement);
}
//===============================================
void ZSpectrumArray::zh_createConnections()
{
    connect(&zv_chemElementList, &ZChemElementList::zg_operationType,
            this, &ZSpectrumArray::zg_chemElementOperation);
}
//===============================================
