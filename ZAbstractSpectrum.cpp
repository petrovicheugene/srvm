//==========================================================
#include "ZAbstractSpectrum.h"
#include <QFileInfo>
//==========================================================
ZAbstractSpectrum::ZAbstractSpectrum()
{
    zv_type = ST_NA;
}
//==========================================================
ZAbstractSpectrum::ZAbstractSpectrum(const QString& name)
{
    zv_type = ST_NA;
    zv_name = name;
}
//==========================================================
ZAbstractSpectrum::~ZAbstractSpectrum()
{

}
//==========================================================
ZAbstractSpectrum::SpectrumType ZAbstractSpectrum::zp_type()
{
    return zv_type;
}
//==========================================================
QString ZAbstractSpectrum::zp_name()
{
    return zv_name;
}
//==========================================================
