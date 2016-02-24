//======================================================================
#include "ZTermNormalizer.h"
//======================================================================
ZTermNormalizer::ZTermNormalizer(QObject *parent) : QObject(parent)
{

}
//======================================================================
bool ZTermNormalizer::zp_isValid() const
{
    return false;
}
//======================================================================
qreal ZTermNormalizer::zp_value() const
{
    return 0;
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
