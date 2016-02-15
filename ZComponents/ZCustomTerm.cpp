//===================================================================
#include "ZCustomTerm.h"
//===================================================================
ZCustomTerm::ZCustomTerm(ZCalibration *parent) : ZAbstractTerm(parent)
{
   zv_type = TT_CUSTOM;
}
//===================================================================
qreal ZCustomTerm::zp_calcValue()
{
   return 0;
}
//===================================================================
QString ZCustomTerm::zp_termName() const
{
   return QString();
}
//===================================================================
