//===================================================================
#include "ZCustomTerm.h"
//===================================================================
ZCustomTerm::ZCustomTerm(ZCalibration *parent) : ZAbstractTerm(parent)
{
   zv_type = PT_CUSTOM;
}
//===================================================================
qreal ZCustomTerm::zp_calcValue()
{
   return 0;
}
//===================================================================
QString ZCustomTerm::zp_termName()
{
   return QString();
}
//===================================================================
