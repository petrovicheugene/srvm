//===============================================
#include "ZSpectrumArray.h"
#include "ZSpeSpectrum.h"
#include "ZSpeIOHandler.h"
#include <QFileInfo>
#include <QFile>
//===============================================
//ZSpectrumArray::ZSpectrumArray(QObject* parent) : QObject(parent)
//{

//}
//===============================================
ZSpectrumArray::ZSpectrumArray(const QString& name, QObject* parent)  : QObject(parent)
{
    zv_arrayName = name;
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
int ZSpectrumArray::zp_spectrumCount() const
{
    return zv_spectrumList.count();
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
QList<int> ZSpectrumArray::zp_spectrumData(int index) const
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return QList<int>();
    }

    return zv_spectrumList.value(index)->zp_spectrumData();
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
        }
    }

    delete ioHandler;
    return res;
}
//===============================================
//bool ZSpectrumArray::zp_setSpectrumAtIndex(int index, const ZAbstractSpectrum& spectrum)
//{
//    if(index < 0 || index >= zv_spectrumList.count())
//    {
//        return false;
//    }

//    *(zv_spectrumList[index]) = spectrum;
//    return true;
//}
//===============================================
//bool ZSpectrumArray::zp_appendSpectrum(const QString& fileName)
//{
//    QFileInfo fileInfo(fileName);
//    ZAbstractSpectrumIOHandler* ioHandler;

//    QString suffix = fileInfo.suffix();
//    if(!fileInfo.exists() || !fileInfo.isFile())
//    {
//        QString error = tr("Error: \"%1\" is not a file!").arg(fileName);
//        return false;
//    }
//    else if(suffix == "spe")
//    {
//        ioHandler = new ZSpeIOHandler(this);
//    }
//    else
//    {
//        error = QObject::tr("Cannot handle file of type \"%1\"!").arg(suffix);
//        return false;
//    }

//}
//===============================================
//bool ZSpectrumArray::zp_applySpectrum(const QString& fileName, QString& error)
//{

//    QFileInfo fileInfo(fileName);
//    ZAbstractSpectrumIOHandler* ioHandler;

//    QString suffix = fileInfo.suffix();
//    if(!fileInfo.exists() || !fileInfo.isFile())
//    {
//        QString error = tr("Error: \"%1\" is not a file!").arg(fileName);
//        return false;
//    }
//    else if(suffix == "spe")
//    {
//        ioHandler = new ZSpeIOHandler(this);
//     }
//    else
//    {
//        error = QObject::tr("Cannot handle file of type \"%1\"!").arg(suffix);
//        return false;
//    }

//    ZSpeSpectrum* spectrum = new ZSpeSpectrum(fileName);

//    zv_spectrumList.append(spectrum);
//    error.clear();
//    return true;


//}
//===============================================
