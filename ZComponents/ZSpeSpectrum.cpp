//===================================================
#include "ZSpeSpectrum.h"
//===================================================
//ZSpeSpectrum::ZSpeSpectrum(QObject *parent) : ZAbstractSpectrum(parent)
//{
//    zv_type = ST_SPE;
//}
//===================================================
ZSpeSpectrum::ZSpeSpectrum(const QList<int>& intensityList, const QString& name, QColor color, QObject *parent)
    : ZAbstractSpectrum(intensityList, name, color, parent)
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
