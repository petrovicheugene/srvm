//==========================================================
#include "ZAbstractSpectrum.h"
#include <QFileInfo>

//==========================================================
ZAbstractSpectrum::ZAbstractSpectrum(const QList<int> &intensityList,
                                     const QString& path, QColor color, QObject *parent)
    : QObject(parent)
{
    zv_visible = true;
    zv_path = path;
    zv_color = color;

    QFileInfo fileInfo(path);
    QString suffix = fileInfo.suffix();
    if(suffix == "spe")
    {
        zv_type = ST_SPE;
    }
    else
    {
        zv_type = ST_NA;
    }

    zv_name = fileInfo.fileName();
    zv_spectrumData = intensityList;
    zv_channelCount = intensityList.count();
    zv_maxIntensity = 0;
    for(int i = 0; i < zv_channelCount; i++)
    {
        if(zv_maxIntensity < intensityList.value(i))
        {
            zv_maxIntensity = intensityList.value(i);
        }
    }
}
//==========================================================
ZAbstractSpectrum::~ZAbstractSpectrum()
{

}

//==========================================================
ZAbstractSpectrum::SpectrumType ZAbstractSpectrum::zp_type() const
{
    return zv_type;
}
//==========================================================
bool ZAbstractSpectrum::zp_isSpectrumVisible() const
{
    return zv_visible;
}
//==========================================================
void ZAbstractSpectrum::zp_setSpectrumVisible(bool visible)
{
    zv_visible = visible;
}
//==========================================================
void ZAbstractSpectrum::zp_insertConcentration(const QString& chemElement, const QString& concentration)
{
    zv_concentrationMap.insert(chemElement, concentration);
}
//==========================================================
QStringList ZAbstractSpectrum::zp_chemElementList() const
{
    return zv_concentrationMap.keys();
}
//==========================================================
QString ZAbstractSpectrum::zp_concentration(const QString& chemElement) const
{
    if(!zv_concentrationMap.keys().contains(chemElement))
    {
        return QString();
    }

    return zv_concentrationMap.value(chemElement);
}
//==========================================================
bool ZAbstractSpectrum::zp_setConcentration(const QString& chemElement,
                                            const QString& concentration)
{
    if(chemElement.isEmpty())
    {
        return false;
    }

    if(zv_concentrationMap.contains(chemElement)
            && ( zv_concentrationMap.value(chemElement) == concentration))
    {
        return false;
    }

    zv_concentrationMap.insert(chemElement, concentration);
    return true;
}
//==========================================================
QList<int> ZAbstractSpectrum::zp_spectrumData()
{
    return zv_spectrumData;
}
//==========================================================
QString ZAbstractSpectrum::zp_name() const
{
    return zv_name;
}
//==========================================================
QString ZAbstractSpectrum::zp_path() const
{
    return zv_path;
}
//==========================================================
int ZAbstractSpectrum::zp_intensityInWindow(int startChannel, int lastChannel, bool* ok)
{
    if(startChannel < 0)
    {
        startChannel = 0;
    }
    else if(startChannel > zv_spectrumData.count())
    {
        startChannel = zv_spectrumData.count() - 1;
    }

    if(lastChannel < 0)
    {
        lastChannel = 0;
    }
    else if(lastChannel > zv_spectrumData.count())
    {
        lastChannel = zv_spectrumData.count() - 1;
    }

    if(startChannel == lastChannel)
    {
        if(ok != 0)
        {
            *ok = false;
        }
        return 0;
    }

    if(startChannel > lastChannel)
    {
        qSwap(startChannel, lastChannel);
    }

    int intensity = 0;
    for(int c = startChannel; c <= lastChannel; c++)
    {
        intensity += zv_spectrumData.value(c);
    }

    if(ok != 0)
    {
        *ok = true;
    }

    return intensity;
}
//==========================================================
int ZAbstractSpectrum::zp_channelCount()
{
    return zv_channelCount;
}
//==========================================================
int ZAbstractSpectrum::zp_maxIntensity()
{
    return zv_maxIntensity;
}
//==========================================================
QColor ZAbstractSpectrum::zp_color()
{
    return zv_color;
}
//==========================================================
