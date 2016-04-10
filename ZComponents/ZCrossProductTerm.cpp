//===================================================================
#include "ZCrossProductTerm.h"
//===================================================================
ZCrossProductTerm::ZCrossProductTerm(const ZCalibrationWindow& window1,
                                 const ZCalibrationWindow& window2,
                                 ZCalibration *parent) : ZAbstractTerm(parent)
{
   zv_type = TT_MIXED;
}
//===================================================================
bool ZCrossProductTerm::zp_calcValue(const ZAbstractSpectrum * spectrum, qreal& value)
{

    value = 0;
    return true;
}
//===================================================================
bool ZCrossProductTerm::zp_calcTermVariablePart(const ZAbstractSpectrum* spectrum, qreal &value)  // w/o factor
{

}
//===================================================================
bool ZCrossProductTerm::zp_termBelongsToWindow(const ZCalibrationWindow* window) const
{
    return false;
}
//===================================================================
QString ZCrossProductTerm::zp_termName() const
{
   return QString();
}
//===================================================================
