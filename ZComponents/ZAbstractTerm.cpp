//============================================================
#include "ZAbstractTerm.h"
#include "globalVariables.h"
#include "ZCalibrationWindow.h"
#include "ZCalibration.h"
#include "ZSpectrumArray.h"
#include <QPointer>
//============================================================
qint64 ZAbstractTerm::zv_lastTermId = 0;
//============================================================
ZAbstractTerm::ZAbstractTerm(ZCalibration *parent) : QObject(parent)
{
    zh_connectToCalibration(parent);
    zv_type = TT_NOT_DEFINED;
    zv_termState = TS_CONST_EXCLUDED;
    zv_K = 0;
    zv_termId = zv_lastTermId++;
}
//============================================================
qint64 ZAbstractTerm::zp_termId()
{
    return zv_termId;
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
//void ZAbstractTerm::zp_calcAverageTermValueAndTermValueList(const ZSpectrumArray* array)
//{
//    zv_unnormalizedValueList.clear();

//    if(!array)
//    {
//        zv_averageValue = 0;
//        emit zg_termValuesChanged();
//        return;
//    }

//    qint64 value;
//    qint64 summaryValue = 0;
//    int checkedSpectrum = 0;
//    for(int s = 0; s < array->zp_spectrumCount(); s++)
//    {
//        if(!array->zp_spectrum(s)->zp_isSpectrumChecked())
//        {
//            continue;
//        }

//        if(!zp_calcTermVariablePart(array->zp_spectrum(s), value))
//        {
//            value = 0;
//        }
//        zv_unnormalizedValueList.append(value);
//        summaryValue += value;
//        checkedSpectrum++;
//    }

//    qreal averageValue;
//    if(checkedSpectrum > 0)
//    {
//        averageValue = (qreal)summaryValue / (qreal)checkedSpectrum;
//    }
//    else
//    {
//        averageValue = 0;
//    }

//    zv_averageValue = averageValue;
//    emit zg_termValuesChanged();

//}
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
void ZAbstractTerm::zp_setTermFactor(qreal factor)
{
    zv_K = factor;
}
//============================================================
void ZAbstractTerm::zh_connectToNormalizer(ZTermNormalizer* normalizer)
{
    connect(normalizer, &ZTermNormalizer::zg_normalizerChanged,
            this, &ZAbstractTerm::zh_normalizerChanged);
//    connect(this, &ZAbstractTerm::zg_requestIsNormalizerValid,
//            normalizer, &ZTermNormalizer::zp_isValid);
//    connect(this, &ZAbstractTerm::zg_requestNormalizerValue,
//            normalizer, &ZTermNormalizer::zp_value);

}
//============================================================
void ZAbstractTerm::zh_connectToCalibration(ZCalibration* calibration)
{
    connect(this, &ZAbstractTerm::zg_requestForDelete,
            calibration, &ZCalibration::zh_removeTerm);

    connect(calibration, &ZCalibration::zg_normalizerChanged,
            this, &ZAbstractTerm::zh_normalizerChanged);
    connect(this, &ZAbstractTerm::zg_requestIsNormalizerValid,
            calibration, &ZCalibration::zh_isNormalizerValid);
    connect(this, &ZAbstractTerm::zg_requestNormalizerValue,
            calibration, &ZCalibration::zh_normalizerValue);
    connect(this, &ZAbstractTerm::zg_termNameChanged,
            calibration, &ZCalibration::zh_onTermNameChange);

    connect(this, &ZAbstractTerm::zg_termWindowMarginChanged,
            calibration, &ZCalibration::zh_onTermWindowMarginChange);
//    connect(this, &ZAbstractTerm::zg_termValuesChanged,
//            calibration, &ZCalibration::zp_onTermAverageValueChange);

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
