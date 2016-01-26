//===================================================================
#include "ZQuadraticPredictor.h"
//===================================================================
ZQuadraticPredictor::ZQuadraticPredictor(const ZCalibrationWindow& window,
                                         ZCalibration *parent) :
   ZAbstractPredictor(parent)
{

}
//===================================================================
qreal ZQuadraticPredictor::zp_calcValue()
{
   return 0;
}
//===================================================================
QString ZQuadraticPredictor::zp_predictorName()
{
   return QString();
}
//===================================================================
