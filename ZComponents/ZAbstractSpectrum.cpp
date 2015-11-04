//==========================================================
#include "ZAbstractSpectrum.h"
#include <QFileInfo>

//==========================================================
ZAbstractSpectrum::ZAbstractSpectrum(const QList<int> &intensityList, const QString& path, QObject *parent)  : QObject(parent)
{
    zv_path = path;
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
    zv_intensityList = intensityList;
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
    zv_concentrationMap.insert(chemElement, concentration);
    return true;
}
//==========================================================
QList<int> ZAbstractSpectrum::zp_spectrumIntensityArray()
{
    return zv_intensityList;
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
