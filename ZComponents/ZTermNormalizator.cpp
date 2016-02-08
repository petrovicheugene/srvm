//======================================================================
#include "ZTermNormalizer.h"
//======================================================================
ZTermNormalizer::ZTermNormalizer(QObject *parent) : QObject(parent)
{

}
//======================================================================
void ZTermNormalizer::zp_isValid(bool& validFlag) const
{
   validFlag = false;
}
//======================================================================
void ZTermNormalizer::zp_value(qreal& value) const
{
   value = 1;
}
//======================================================================
