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
qreal ZCrossProductTerm::zp_calcValue()
{
   return 0;
}
//===================================================================
QString ZCrossProductTerm::zp_termName() const
{
   return QString();
}
//===================================================================
