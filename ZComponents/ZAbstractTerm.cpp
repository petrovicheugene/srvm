//============================================================
#include "ZAbstractTerm.h"
#include "ZCalibrationWindow.h"
#include "ZCalibration.h"

#include <QPointer>
//============================================================
ZAbstractTerm::ZAbstractTerm(ZCalibration *parent) : QObject(parent)
{
   zh_connectToCalibration(parent);
   zv_type = PT_NOT_DEFINED;
}
//============================================================
ZAbstractTerm::TermType ZAbstractTerm::zp_termType()
{
   return zv_type;
}
//============================================================
QString ZAbstractTerm::zp_termName()
{
    return zv_name;
}
//============================================================
void ZAbstractTerm::zh_connectToNormalizer(ZTermNormalizer* normalizer)
{
   connect(normalizer, &ZTermNormalizer::zg_normalizerChanged,
           this, &ZAbstractTerm::zh_normalizerChanged);
   connect(this, &ZAbstractTerm::zg_isNormalizerValid,
           normalizer, &ZTermNormalizer::zp_isValid);
   connect(this, &ZAbstractTerm::zg_normalizerValue,
           normalizer, &ZTermNormalizer::zp_value);

}
//============================================================
void ZAbstractTerm::zh_connectToCalibration(ZCalibration* calibration)
{
   calibration->zp_connectTermToCalibration(this);

   connect(calibration, &ZCalibration::zg_normalizerChanged,
           this, &ZAbstractTerm::zh_normalizerChanged);
   connect(this, &ZAbstractTerm::zg_isNormalizerValid,
           calibration, &ZCalibration::zp_isNormalizerValid);
   connect(this, &ZAbstractTerm::zg_normalizerValue,
           calibration, &ZCalibration::zp_normalizerValue);

}
//============================================================
void ZAbstractTerm::zh_normalizerChanged()
{
   emit zg_termChanged();
}
//============================================================
void ZAbstractTerm::zh_onWindowDestroying()
{
    emit zg_requestForDelete(this);
}
//============================================================
