//===================================================================
#include "ZCustomPredictor.h"
//===================================================================
ZCustomPredictor::ZCustomPredictor(ZCalibration *parent) : ZAbstractPredictor(parent)
{

}
//===================================================================
qreal ZCustomPredictor::zp_calcValue()
{
   return 0;
}
//===================================================================
QString ZCustomPredictor::zp_predictorName()
{
   return QString();
}
//===================================================================
