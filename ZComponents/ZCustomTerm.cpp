//===================================================================
#include "ZCustomTerm.h"
//===================================================================
ZCustomTerm::ZCustomTerm(ZCalibration *parent) : ZAbstractTerm(parent)
{
   zv_type = TT_CUSTOM;
}
//===================================================================
bool ZCustomTerm::zp_calcValue(const ZAbstractSpectrum * spectrum, qreal& value)
{
    value = 0;
    return true;
}
//===================================================================
bool ZCustomTerm::zp_calcTermVariablePart(const ZAbstractSpectrum* spectrum, qint64 &value)  // w/o factor
{

}
//===================================================================
bool ZCustomTerm::zp_termBelongsToWindow(const ZCalibrationWindow* window) const
{
    return false;
}
//===================================================================
QString ZCustomTerm::zp_termName() const
{
   return QString();
}
//===================================================================
