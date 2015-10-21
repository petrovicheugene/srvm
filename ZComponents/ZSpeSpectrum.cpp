//===================================================
#include "ZSpeSpectrum.h"
//===================================================
ZSpeSpectrum::ZSpeSpectrum() : ZAbstractSpectrum()
{
    zv_type = ST_SPE;
}
//===================================================
ZSpeSpectrum::ZSpeSpectrum(const QString& name) : ZAbstractSpectrum(name)
{
    zv_type = ST_SPE;
}
//===================================================
ZSpeSpectrum::~ZSpeSpectrum()
{

}
//===================================================
ZAbstractSpectrumAuxData ZSpeSpectrum::zp_auxData()
{
    return zv_auxData;
}
//===================================================
