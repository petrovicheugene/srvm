//===================================================================
#include "ZSimplePredictor.h"
#include "ZCalibrationWindow.h"
//===================================================================
ZSimplePredictor::ZSimplePredictor(const ZCalibrationWindow &window,
                                   ZCalibration *parent) : ZAbstractPredictor(parent)
{

}
//===================================================================
qreal ZSimplePredictor::zp_calcValue()
{
   return 0;
}
//===================================================================
QString ZSimplePredictor::zp_predictorName()
{
   return QString();
}
//===================================================================
