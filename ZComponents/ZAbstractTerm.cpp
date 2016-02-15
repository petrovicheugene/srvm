//============================================================
#include "ZAbstractTerm.h"
#include "ZCalibrationWindow.h"
#include "ZCalibration.h"

#include <QPointer>
//============================================================
ZAbstractTerm::ZAbstractTerm(ZCalibration *parent) : QObject(parent)
{
    zh_connectToCalibration(parent);
    zv_type = TT_NOT_DEFINED;
    zv_termState = TS_CONST_EXCLUDED;
    zv_K = 0;
}
//============================================================
ZAbstractTerm::TermType ZAbstractTerm::zp_termType() const
{
    return zv_type;
}
//============================================================
ZAbstractTerm::TermState ZAbstractTerm::zp_termState() const
{
    return zv_termState;
}
//============================================================
bool ZAbstractTerm::zp_setTermState(ZAbstractTerm::TermState state)
{
    if(zv_termState == state)
    {
        return false;
    }

    zv_termState = state;
    emit zg_termStateChanged(zv_termState);
    return true;
}
//============================================================
QString ZAbstractTerm::zp_termName() const
{
    return zv_name;
}
//============================================================
qreal ZAbstractTerm::zp_termFactor() const
{
    return zv_K;
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
    connect(this, &ZAbstractTerm::zg_termNameChanged,
            calibration, &ZCalibration::zp_onTermNameChange);


}
//============================================================
void ZAbstractTerm::zh_normalizerChanged()
{
    emit zg_termNameChanged();
}
//============================================================
void ZAbstractTerm::zh_onWindowDestroying()
{
    emit zg_requestForDelete(this);
}
//============================================================
