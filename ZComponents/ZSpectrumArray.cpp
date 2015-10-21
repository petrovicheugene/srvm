//===============================================
#include "ZSpectrumArray.h"
#include "ZSpeSpectrum.h"

#include <QObject>
//===============================================
ZSpectrumArray::ZSpectrumArray()
{

}
//===============================================
ZSpectrumArray::~ZSpectrumArray()
{
    qDeleteAll(zv_spectrumList);
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
    return zv_spectrumList.value(index)->zp_name();
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
void ZSpectrumArray::zp_appendSpectrum(ZAbstractSpectrum* spectrum)
{
    zv_spectrumList << spectrum;
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
