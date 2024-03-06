//======================================================================
#include "X_TermNormalizer.h"
#include "X_AbstractSpectrum.h"
#include "X_Calibration.h"
#include "X_SpeSpectrum.h"
//======================================================================
// STATIC
//======================================================================
QMap<X_TermNormalizer::NormaType, QPair<QString, QString>> X_TermNormalizer::xv_normaTypeStringMap =
    X_TermNormalizer::xh_initNormaTypeStringMap();
//======================================================================
void X_TermNormalizer::xp_initStaticVariables()
{
    xv_normaTypeStringMap = X_TermNormalizer::xh_initNormaTypeStringMap();
}
//======================================================================
QString X_TermNormalizer::xp_displayNormalizerTypeName(X_TermNormalizer::NormaType type)
{
    return xv_normaTypeStringMap.value(type).second;
}
//======================================================================
QString X_TermNormalizer::xp_normalizerTypeName(X_TermNormalizer::NormaType type)
{
    return xv_normaTypeStringMap.value(type).first;
}
//======================================================================
X_TermNormalizer::NormaType X_TermNormalizer::xp_normalizerTypeFromString(const QString& typestring)
{
    QMap<X_TermNormalizer::NormaType, QPair<QString, QString>>::iterator it;
    for (it = xv_normaTypeStringMap.begin(); it != xv_normaTypeStringMap.end(); it++)
    {
        if (it.value().first == typestring || it.value().second == typestring)
        {
            return it.key();
        }
    }

    return NT_NONE;
}
//======================================================================
QMap<X_TermNormalizer::NormaType, QPair<QString, QString>> X_TermNormalizer::xh_initNormaTypeStringMap()
{
    QMap<X_TermNormalizer::NormaType, QPair<QString, QString> > map;
    map.insert(NT_NONE, QPair<QString, QString>("None", QObject::tr("None")));
    map.insert(NT_COHERENT, QPair<QString, QString>("Coherent", QObject::tr("Coherent")));
    map.insert(NT_INCOHERENT, QPair<QString, QString>("Incoherent", QObject::tr("Incoherent")));
    map.insert(NT_COHERENT_INCOHERENT, QPair<QString, QString>("Coherent / Incoherent", QObject::tr("Coherent / Incoherent")));
    map.insert(NT_INCOHERENT_COHERENT, QPair<QString, QString>("Incoherent / Coherent", QObject::tr("Incoherent / Coherent")));
    map.insert(NT_CUSTOM, QPair<QString, QString>("Custom", QObject::tr("Custom")));
    return map;
}
// END STATIC
//======================================================================
//======================================================================
X_TermNormalizer::X_TermNormalizer(X_Calibration *parent) : QObject(parent)
{
    xv_calibration = parent;
    xv_normaType = NT_NONE;
    xv_customNormaString = QString();
    xh_createConnections();
}
//======================================================================
void X_TermNormalizer::xp_connectToWindow(X_CalibrationWindow* window)
{
    connect(window, &X_CalibrationWindow::xg_windowTypeChanged,
            this, &X_TermNormalizer::xh_onWindowTypeChange);
    connect(window, &X_CalibrationWindow::destroyed,
            this, &X_TermNormalizer::xh_onWindowDestroy);
    connect(window, &X_CalibrationWindow::xg_windowMarginsChanged,
            this, &X_TermNormalizer::xh_onWindowMarginsChange);

}
//======================================================================
//bool X_TermNormalizer::xp_isValid() const
//{
//    return false;
//}
////======================================================================
//qreal X_TermNormalizer::xp_value() const
//{
//    return 0;
//}
//======================================================================
X_TermNormalizer::NormaType X_TermNormalizer::xp_normaType() const
{
    return xv_normaType;
}
//======================================================================
bool X_TermNormalizer::xp_setNormaType(X_TermNormalizer::NormaType type)
{
    if(type == xv_normaType)
    {
        return false;
    }

    xv_normaType = type;
    emit xg_normalizerChanged();
    return true;
}
//======================================================================
bool X_TermNormalizer::xp_setCustomNormaString(const QString& customString)
{
    xv_customNormaString = customString;
    emit xg_normalizerChanged();
    return true;
}
//======================================================================
QString X_TermNormalizer::xp_normaCustomString() const
{
    return xv_customNormaString;
}
//======================================================================
bool X_TermNormalizer::xp_setNormalizerParameters(X_TermNormalizer::NormaType type,
                                                  const QString& customString)
{
    bool res = false;
    if(type != xv_normaType)
    {
        xv_normaType = type;
        res = true;
    }

    if(xv_customNormaString != customString)
    {
        xv_customNormaString = customString;
        res = true;
    }

    if(res)
    {
        emit xg_normalizerChanged();
    }

    return res;
}
//======================================================================
bool X_TermNormalizer::xp_normalizeValue(const X_AbstractSpectrum* spectrum, qreal& value )
{
    if(xv_normaType == NT_NONE)
    {
        return true;
    }

    qreal normaValue;
    if(!xh_calcNormaValue(spectrum, normaValue))
    {
        return false;
    }

    if(normaValue == 0)
    {
        return false;
    }

    value /= normaValue;
    return true;
}
//======================================================================
bool X_TermNormalizer::xp_normalizeValue(qreal &value)
{
    if(xv_normaType == NT_NONE)
    {
        return true;
    }

    if(xv_normaValueBuffer == 0)
    {
        return false;
    }

    value /= xv_normaValueBuffer;
    return true;
}
//======================================================================
bool X_TermNormalizer::xp_calcAndSetNormaValue(const X_AbstractSpectrum *spectrum)
{
    xp_resetNormaValue();
    qreal normaValue;
    if(!xh_calcNormaValue(spectrum, normaValue))
    {
        return false;
    }

    xv_spectrumIdBuffer = spectrum->xp_spectrumId();
    xv_normaValueBuffer = normaValue;

    return true;
}
//======================================================================
bool X_TermNormalizer::xh_calcNormaValue(const X_AbstractSpectrum *spectrum, qreal& normaValue)
{
    normaValue = 0;
    if(!spectrum)
    {
        return false;
    }

    if(xv_normaType == NT_NONE)
    {
        return true;
    }

    if(xv_normaType == NT_COHERENT)
    {
        qint64 windowIntensity = 0;
        if(!xh_getWindowsValue(X_CalibrationWindow::WT_COHERENT, spectrum, windowIntensity))
        {
            return false;
        }
        normaValue = static_cast<qreal>(windowIntensity);
    }
    else  if(xv_normaType == NT_INCOHERENT)
    {
        qint64 windowIntensity = 0;
        if(!xh_getWindowsValue(X_CalibrationWindow::WT_INCOHERENT, spectrum, windowIntensity))
        {
            return false;
        }
        normaValue = static_cast<qreal>(windowIntensity);
    }
    else if(xv_normaType == NT_COHERENT_INCOHERENT)
    {
        qint64 coherentValue = 0;
        qint64 incoherentValue = 0;
        if(!xh_getWindowsValue(X_CalibrationWindow::WT_INCOHERENT, spectrum, incoherentValue))
        {
            return false;
        }

        if(!xh_getWindowsValue(X_CalibrationWindow::WT_COHERENT, spectrum, coherentValue))
        {
            return false;
        }

        if(incoherentValue == 0)
        {
            return false;
        }

        normaValue = static_cast<qreal>(coherentValue) / static_cast<qreal>(incoherentValue);
    }
    else if(xv_normaType == NT_INCOHERENT_COHERENT)
    {
        qint64 coherentValue = 0;
        qint64 incoherentValue = 0;
        if(!xh_getWindowsValue(X_CalibrationWindow::WT_INCOHERENT, spectrum, incoherentValue))
        {
            return false;
        }

        if(!xh_getWindowsValue(X_CalibrationWindow::WT_COHERENT, spectrum, coherentValue))
        {
            return false;
        }

        if(coherentValue == 0)
        {
            return false;
        }

        normaValue = static_cast<qreal>(incoherentValue) / static_cast<qreal>(coherentValue);
    }
    else if(xv_normaType == NT_CUSTOM)
    {

        xv_spectrum = static_cast<const X_SpeSpectrum*>(spectrum);
        bool res = xv_mathExpressionHandler.xp_calculateExpression(xv_customNormaString, normaValue);
        xv_spectrum = nullptr;
        return res;
    }

    return true;
}
//======================================================================
void X_TermNormalizer::xp_resetNormaValue()
{
    xv_spectrumIdBuffer = -1;
    xv_normaValueBuffer = 0;
}
//======================================================================
void X_TermNormalizer::xh_createConnections()
{
    connect(this, &X_TermNormalizer::xg_normalizerChanged,
            xv_calibration, &X_Calibration::xh_onNormalizerChange);

    connect(&xv_mathExpressionHandler,
            &X_MathExpressionHandler::xg_requestVariableValue,
            this,
            &X_TermNormalizer::xh_handleWindowIntensityRequest);
    connect(&xv_mathExpressionHandler,
            &X_MathExpressionHandler::xg_errorReport,
            this,
            &X_TermNormalizer::xh_handleErrorReport);

}
//======================================================================
bool X_TermNormalizer::xh_getWindowsValue(X_CalibrationWindow::WindowType type,
                                          const X_AbstractSpectrum* spectrum, qint64& value) const
{
    qreal currentValue;
    bool ok;
    value = 0;
    for(int w = 0; w < xv_calibration->xv_windowList.count(); w++)
    {
        if(xv_calibration->xv_windowList.at(w)->xp_windowType() != type)
        {
            continue;
        }

        xv_calibration->xv_windowList.at(w)->xp_calcWindowIntensity(spectrum, currentValue, true, &ok);
        if(!ok)
        {
            return false;
        }
        value += currentValue;
    }
    return true;
}
//======================================================================
void X_TermNormalizer::xh_onWindowTypeChange(X_CalibrationWindow::WindowType previousType,
                                             X_CalibrationWindow::WindowType currentType) const
{
    if(xv_normaType == NT_NONE)
    {
        return;
    }
    else if(xv_normaType == NT_CUSTOM)
    {
        // TODO check out is destroyed window parted in custom norma
        emit xg_normalizerChanged();
        return;
    }

    if((previousType == X_CalibrationWindow::WT_COHERENT || currentType == X_CalibrationWindow::WT_COHERENT) &&
        (xv_normaType == NT_COHERENT ||  xv_normaType == NT_COHERENT_INCOHERENT
         || xv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit xg_normalizerChanged();
    }
    else if((previousType == X_CalibrationWindow::WT_INCOHERENT || currentType == X_CalibrationWindow::WT_INCOHERENT) &&
             (xv_normaType == NT_INCOHERENT ||  xv_normaType == NT_COHERENT_INCOHERENT
              || xv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit xg_normalizerChanged();
    }
}
//======================================================================
void X_TermNormalizer::xh_onWindowDestroy(QObject* obj) const
{
    if(xv_normaType == NT_NONE)
    {
        return;
    }
    else if(xv_normaType == NT_CUSTOM)
    {
        // TODO check out is destroyed window parted in custom norma
        emit xg_normalizerChanged();
        return;
    }

    X_CalibrationWindow* window = qobject_cast<X_CalibrationWindow*>(obj);
    if(!window)
    {
        emit xg_normalizerChanged();
        return;
    }

    X_CalibrationWindow::WindowType windowType = window->xp_windowType();
    if(windowType == X_CalibrationWindow::WT_COHERENT &&
        (xv_normaType == NT_COHERENT ||  xv_normaType == NT_COHERENT_INCOHERENT
         || xv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit xg_normalizerChanged();
    }
    else if(windowType == X_CalibrationWindow::WT_INCOHERENT &&
             (xv_normaType == NT_INCOHERENT ||  xv_normaType == NT_COHERENT_INCOHERENT
              || xv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit xg_normalizerChanged();
    }
}
//======================================================================
void X_TermNormalizer::xh_onWindowMarginsChange() const
{
    X_CalibrationWindow* window = qobject_cast<X_CalibrationWindow*>(sender());

    if(window->xp_windowType() == X_CalibrationWindow::WT_INCOHERENT &&
        (xv_normaType == NT_INCOHERENT ||  xv_normaType == NT_COHERENT_INCOHERENT
         || xv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit xg_normalizerChanged();
    }
    else if(window->xp_windowType() == X_CalibrationWindow::WT_COHERENT &&
             (xv_normaType == NT_COHERENT ||  xv_normaType == NT_COHERENT_INCOHERENT
              || xv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit xg_normalizerChanged();
    }
}
//======================================================================
void X_TermNormalizer::xh_handleWindowIntensityRequest(const QString& windowName,
                                                       double& value,
                                                       bool& bRes)
{
    emit xg_calcWindowIntensity(windowName, (const QObject*) xv_spectrum, value, true, &bRes);
}
//===================================================================
void X_TermNormalizer::xh_handleErrorReport(const QString& errorString,
                                            int errorTokenStartPosition,
                                            int errorTokenEndPosition) const
{
    emit xg_errorReport(QString("%1 (%2)!").arg(errorString, QString::number(errorTokenStartPosition)));

}
//===================================================================
