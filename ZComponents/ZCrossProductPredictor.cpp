//===================================================================
#include "ZCrossProductPredictor.h"
//===================================================================
ZCrossProductPredictor::ZCrossProductPredictor(const ZCalibrationWindow& window1,
                                 const ZCalibrationWindow& window2,
                                 ZCalibration *parent) : ZAbstractPredictor(parent)
{
   zv_type = PT_MIXED;
}
//===================================================================
qreal ZCrossProductPredictor::zp_calcValue()
{
   return 0;
}
//===================================================================
QString ZCrossProductPredictor::zp_predictorName()
{
   return QString();
}
//===================================================================
