//============================================================
#include "X_AbstractTerm.h"
#include "X_General.h"
#include "X_CalibrationWindow.h"
#include "X_Calibration.h"
#include "X_SpectrumArray.h"
#include <QPointer>
//============================================================
// STATIC
qint64 X_AbstractTerm::xv_lastTermId = 0;
int X_AbstractTerm::xv_precision = 15;
QMap<X_AbstractTerm::TermType, QString> X_AbstractTerm::xv_typeNameMap = X_AbstractTerm::zh_intTypeNameMap();
QMap<X_AbstractTerm::TermState, QString> X_AbstractTerm::xv_stateNameMap = X_AbstractTerm::zh_intStateNameMap();
//============================================================
QMap<X_AbstractTerm::TermType, QString> X_AbstractTerm::zh_intTypeNameMap()
{
    QMap<X_AbstractTerm::TermType, QString> map;
    map.insert(TT_NOT_DEFINED, "Not defined");
    map.insert(TT_SIMPLE, "Simple");
    map.insert(TT_QUADRATIC, "Quadratic");
    map.insert(TT_MIXED, "Mixed");
    map.insert(TT_CUSTOM, "Custom");

    return map;
}
//============================================================
QMap<X_AbstractTerm::TermState, QString> X_AbstractTerm::zh_intStateNameMap()
{
    QMap<X_AbstractTerm::TermState, QString> map;
    map.insert(TS_NOT_DEFINED, "Not defined");
    map.insert(TS_CONST_INCLUDED, "Constantly Included");
    map.insert(TS_CONST_EXCLUDED, "Constantly Excluded");
    map.insert(TS_EXAM_WAITING, "Exam Waiting");
    map.insert(TS_EXCEPTED, "Excepted");
    map.insert(TS_INCLUDED, "Included");
    map.insert(TS_BASE, "Base");

    return map;
}
//============================================================
QString X_AbstractTerm::xp_termTypeName(X_AbstractTerm::TermType termType)
{
    if(!xv_typeNameMap.keys().contains(termType))
    {
        return QString();
    }

    return xv_typeNameMap.value(termType);
}
//============================================================
QString X_AbstractTerm::xp_termStateName(X_AbstractTerm::TermState termState)
{
    if(!xv_stateNameMap.keys().contains(termState))
    {
        return QString();
    }

    return xv_stateNameMap.value(termState);
}
//============================================================
X_AbstractTerm::TermType X_AbstractTerm::xp_termTypeFromString(const QString& typeString)
{
    if(!xv_typeNameMap.values().contains(typeString))
    {
        return X_AbstractTerm::TT_NOT_DEFINED;
    }

    return xv_typeNameMap.key(typeString);
}
//============================================================
X_AbstractTerm::TermState X_AbstractTerm::xp_termStateFromString(const QString& stateString)
{
    if(!xv_stateNameMap.values().contains(stateString))
    {
        return X_AbstractTerm::TS_NOT_DEFINED;
    }

    return xv_stateNameMap.key(stateString);
}
//============================================================
// END STATIC
//============================================================
X_AbstractTerm::X_AbstractTerm(X_Calibration *calibration) :
      QObject(calibration)
{
    zh_connectToCalibration(calibration);
    xv_type = TT_NOT_DEFINED;
    xv_termState = TS_CONST_EXCLUDED;
    // xp_setTermFactor(0.0); // xv_termFactorString = 0;
    zh_setTermFactor("0.0");
    xv_termId = xv_lastTermId++;
}
//============================================================
qint64 X_AbstractTerm::xp_termId()
{
    return xv_termId;
}
//============================================================
X_AbstractTerm::TermType X_AbstractTerm::xp_termType() const
{
    return xv_type;
}
//============================================================
X_AbstractTerm::TermState X_AbstractTerm::xp_termState() const
{
    return xv_termState;
}
//============================================================
bool X_AbstractTerm::xp_setTermState(X_AbstractTerm::TermState state)
{
    if(xv_termState == state)
    {
        return false;
    }

    xv_termState = state;
    emit xg_termStateChanged(xv_termState);
    return true;
}
//============================================================
void X_AbstractTerm::zh_conformStringWithValue()
{
    if(qAbs(xv_termFactor) < 1)
    {
        bool started = false;
        xv_termFactorString = QString::number(xv_termFactor, 'f', xv_precision);
        // first zero count after point
        int firstX_eroCount = 0;
        for(int i = 0; i < xv_termFactorString.length(); i++)
        {
            if(!started && (xv_termFactorString.at(i) == '.' || xv_termFactorString.at(i) == ','))
            {
                started = true;
                continue;
            }

            if(xv_termFactorString.at(i) == '0')
            {
                firstX_eroCount++;
                continue;
            }

            break;
        }

        if(firstX_eroCount > 9)
        {
            xv_termFactorString = QString::number(xv_termFactor, 'g', xv_precision);
        }
        else
        {
            // chop tail zero if they exist
            for(int i = xv_termFactorString.length()-1; i > 0; i--)
            {
                if(xv_termFactorString.at(i) == '0' && (xv_termFactorString.at(i - 1) != '.' || xv_termFactorString.at(i - 1) != ','))
                {
                    xv_termFactorString.remove(i, 1);
                    continue;
                }
                break;
            }
        }
    }
    else
    {
        xv_termFactorString = QString::number(xv_termFactor, 'g', xv_precision);
    }
}
//============================================================
bool X_AbstractTerm::xp_setPrecision(int precision)
{
    if(precision < 0 )
    {
        return false;
    }
    xv_precision = precision;
    return true;
}
//============================================================
//void X_AbstractTerm::xp_calcAverageTermValueAndTermValueList(const X_SpectrumArray* array)
//{
//    xv_unnormalizedValueList.clear();

//    if(!array)
//    {
//        xv_averageValue = 0;
//        emit xg_termValuesChanged();
//        return;
//    }

//    qint64 value;
//    qint64 summaryValue = 0;
//    int checkedSpectrum = 0;
//    for(int s = 0; s < array->xp_spectrumCount(); s++)
//    {
//        if(!array->xp_spectrum(s)->xp_isSpectrumChecked())
//        {
//            continue;
//        }

//        if(!xp_calcTermVariablePart(array->xp_spectrum(s), value))
//        {
//            value = 0;
//        }
//        xv_unnormalizedValueList.append(value);
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

//    xv_averageValue = averageValue;
//    emit xg_termValuesChanged();

//}
//============================================================
QString X_AbstractTerm::xp_termName() const
{
    return xv_name;
}
//============================================================
qreal X_AbstractTerm::xp_termFactor() const
{
    return xv_termFactor;
}
//============================================================
qreal* X_AbstractTerm::zh_termFactorPointer()
{
    return &xv_termFactor;
}
//============================================================
QString X_AbstractTerm::xp_termFactorString() const
{
    return xv_termFactorString;
}
//============================================================
bool X_AbstractTerm::zh_setTermFactor(qreal factor)
{
    if(xv_termFactor == factor)
    {
        return false;
    }

    xv_termFactor = factor;
    zh_conformStringWithValue();
//    xv_termFactorString = QString::number(factor, 'f', 15);
//    zh_chopTailX_eroesFromTermFactorString();
    return true;
}
//============================================================
bool X_AbstractTerm::zh_setTermFactor(const QString& termFactorString)
{
    bool ok;
    qreal termFactor = termFactorString.toDouble(&ok);
    if(!ok)
    {
        return false;
    }

    xv_termFactorString = termFactorString;
    xv_termFactor = termFactor;
    return true;
}
//============================================================
void X_AbstractTerm::zh_chopTailX_eroesFromTermFactorString()
{
    for(int i = xv_termFactorString.length() - 1; i > 0; i--)
    {
        if(xv_termFactorString.at(i) != '0' || (xv_termFactorString.at(i - 1) == '.' || xv_termFactorString.at(i - 1) == ',' ))
        {
            break;
        }
        xv_termFactorString.chop(1);
    }
}
//============================================================
void X_AbstractTerm::zh_connectToNormalizer(X_TermNormalizer* normalizer)
{
    connect(normalizer, &X_TermNormalizer::xg_normalizerChanged,
            this, &X_AbstractTerm::zh_normalizerChanged);
    //    connect(this, &X_AbstractTerm::xg_requestIsNormalizerValid,
    //            normalizer, &X_TermNormalizer::xp_isValid);
    //    connect(this, &X_AbstractTerm::xg_requestNormalizerValue,
    //            normalizer, &X_TermNormalizer::xp_value);

}
//============================================================
void X_AbstractTerm::zh_connectToCalibration(X_Calibration* calibration)
{
    connect(this, &X_AbstractTerm::xg_requestForDelete,
            calibration, &X_Calibration::zh_removeTerm);

    connect(calibration, &X_Calibration::xg_normalizerChanged,
            this, &X_AbstractTerm::zh_normalizerChanged);
    //    connect(this, &X_AbstractTerm::xg_requestNormalizerValue,
    //            calibration, &X_Calibration::zh_normalizerValue);
    connect(this, &X_AbstractTerm::xg_termNameChanged,
            calibration, &X_Calibration::zh_onTermNameChange);

    connect(this, &X_AbstractTerm::xg_termWindowMarginChanged,
            calibration, &X_Calibration::zh_onTermWindowMarginChange);

    //    connect(this, &X_AbstractTerm::xg_termValuesChanged,
    //            calibration, &X_Calibration::xp_onTermAverageValueChange);

}
//============================================================
void X_AbstractTerm::zh_normalizerChanged()
{
    emit xg_termNameChanged();
}
//============================================================
void X_AbstractTerm::zh_onWindowDestroying()
{
    emit xg_requestForDelete(this);
}
//============================================================
