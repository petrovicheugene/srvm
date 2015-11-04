//===================================================
#include "ZSpeSpectrum.h"
//===================================================
//ZSpeSpectrum::ZSpeSpectrum(QObject *parent) : ZAbstractSpectrum(parent)
//{
//    zv_type = ST_SPE;
//}
//===================================================
ZSpeSpectrum::ZSpeSpectrum(const QList<int>& intensityList, const QString& name, QObject *parent)
    : ZAbstractSpectrum(intensityList, name, parent)
{

}
//===================================================
ZSpeSpectrum::~ZSpeSpectrum()
{

}
//===================================================
ZAbstractSpectrumAuxData ZSpeSpectrum::zp_auxData() const
{
    return zv_auxData;
}
//===================================================
