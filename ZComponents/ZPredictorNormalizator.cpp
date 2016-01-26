//======================================================================
#include "ZPredictorNormalizator.h"
//======================================================================
ZPredictorNormalizer::ZPredictorNormalizer(QObject *parent) : QObject(parent)
{

}
//======================================================================
void ZPredictorNormalizer::zp_isValid(bool& validFlag) const
{
   validFlag = false;
}
//======================================================================
void ZPredictorNormalizer::zp_value(qreal& value) const
{
   value = 1;
}
//======================================================================
