//============================================================
#include "ZAbstractPredictor.h"
#include "ZCalibrationWindow.h"
#include <QPointer>
//============================================================
ZAbstractPredictor::ZAbstractPredictor(ZCalibration *parent) : QObject(parent)
{
   zh_connectToCalibration(parent);

}
//============================================================
void ZAbstractPredictor::zh_connectToNormalizer(ZPredictorNormalizer* normalizer)
{
   connect(normalizer, &ZPredictorNormalizer::zg_normalizerChanged,
           this, &ZAbstractPredictor::zh_normalizerChanged);
   connect(this, &ZAbstractPredictor::zg_isNormalizerValid,
           normalizer, &ZPredictorNormalizer::zp_isValid);
   connect(this, &ZAbstractPredictor::zg_normalizerValue,
           normalizer, &ZPredictorNormalizer::zp_value);

}
//============================================================
void ZAbstractPredictor::zh_connectToCalibration(ZCalibration* calibration)
{
   calibration->zp_connectPredictorToCalibration(this);

   connect(calibration, &ZCalibration::zg_normalizerChanged,
           this, &ZAbstractPredictor::zh_normalizerChanged);
   connect(this, &ZAbstractPredictor::zg_isNormalizerValid,
           calibration, &ZCalibration::zp_isNormalizerValid);
   connect(this, &ZAbstractPredictor::zg_normalizerValue,
           calibration, &ZCalibration::zp_normalizerValue);

}
//============================================================
void ZAbstractPredictor::zh_normalizerChanged()
{
   emit zg_predictorChanged();
}
//============================================================
