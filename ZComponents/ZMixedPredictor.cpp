//===================================================================
#include "ZMixedPredictor.h"
//===================================================================
ZMixedPredictor::ZMixedPredictor(const ZCalibrationWindow& window1,
                                 const ZCalibrationWindow& window2,
                                 ZCalibration *parent) : ZAbstractPredictor(parent)
{
   zv_type = PT_MIXED;
}
//===================================================================
qreal ZMixedPredictor::zp_calcValue()
{
   return 0;
}
//===================================================================
QString ZMixedPredictor::zp_predictorName()
{
   return QString();
}
//===================================================================
