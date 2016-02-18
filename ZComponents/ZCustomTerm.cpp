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


    return 0;
}
//===================================================================
bool ZCustomTerm::zp_calcTermVariablePart(const ZAbstractSpectrum* spectrum, qint64 &value)  // w/o factor
{

}
//===================================================================
QString ZCustomTerm::zp_termName() const
{
   return QString();
}
//===================================================================
