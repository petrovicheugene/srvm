//======================================================================
#include "ZTermNormalizer.h"
#include "ZAbstractSpectrum.h"
#include "ZCalibration.h"
//======================================================================
QMap<ZTermNormalizer::NormaType, QString> ZTermNormalizer::zv_normaTypeStringMap =
        ZTermNormalizer::zh_initNormaTypeStringMap();
//======================================================================
// STATIC
QStringList ZTermNormalizer::zp_normaTypeList()
{
    return zv_normaTypeStringMap.values();
}
//======================================================================
QString ZTermNormalizer::zp_normaTypeString(ZTermNormalizer::NormaType type)
{
    return zv_normaTypeStringMap.value(type);
}
//======================================================================
ZTermNormalizer::NormaType ZTermNormalizer::zp_normaTypeForString(const QString& string)
{
    if(!zv_normaTypeStringMap.values().contains(string))
    {
        return NT_NONE;
    }
    return zv_normaTypeStringMap.key(string);
}
//======================================================================
QMap<ZTermNormalizer::NormaType, QString> ZTermNormalizer::zh_initNormaTypeStringMap()
{
    QMap<ZTermNormalizer::NormaType, QString> map;
    map.insert(NT_NONE, tr("None"));
    map.insert(NT_COHERENT, tr("Coherent"));
    map.insert(NT_INCOHERENT, tr("Incoherent"));
    map.insert(NT_COHERENT_INCOHERENT, tr("Coherent / Incoherent"));
    map.insert(NT_INCOHERENT_COHERENT, tr("Incoherent / Coherent"));
    map.insert(NT_CUSTOM, tr("Custom"));
    return map;
}
// END STATIC
//======================================================================
ZTermNormalizer::ZTermNormalizer(ZCalibration *parent) : QObject(parent)
{
    zv_calibration = parent;
    zv_normaType = NT_NONE;
    zh_createConnections();

}
//======================================================================
void ZTermNormalizer::zp_connectToWindow(ZCalibrationWindow* window)
{
    connect(window, &ZCalibrationWindow::zg_windowTypeChanged,
            this, &ZTermNormalizer::zh_onWindowTypeChange);
    connect(window, &ZCalibrationWindow::destroyed,
            this, &ZTermNormalizer::zh_onWindowDestroy);
    connect(window, &ZCalibrationWindow::zg_windowMarginsChanged,
            this, &ZTermNormalizer::zh_onWindowMarginsChange);

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
ZTermNormalizer::NormaType ZTermNormalizer::zp_normaType() const
{
    return zv_normaType;
}
//======================================================================
bool ZTermNormalizer::zp_setNormaType(ZTermNormalizer::NormaType type)
{
    if(type == zv_normaType)
    {
        return false;
    }

    zv_normaType = type;
    emit zg_normalizerChanged();
    return true;
}
//======================================================================
bool ZTermNormalizer::zp_calcNormalizedValue(const ZAbstractSpectrum* spectrum, qreal& value)
{
    if(zv_normaType == NT_NONE || !spectrum)
    {
        zv_spectrumId = -1;
        zv_normaValue = 0;
        return true;
    }

    qreal normaValue = 0;
    if(zv_normaType == NT_COHERENT)
    {
        qint64 windowIntensity = 0;
        if(!zh_getWindowsValue(ZCalibrationWindow::WT_COHERENT, spectrum, windowIntensity))
        {
            return false;
        }
        normaValue = static_cast<qreal>(windowIntensity);
    }
    else  if(zv_normaType == NT_INCOHERENT)
    {
        qint64 windowIntensity = 0;
        if(!zh_getWindowsValue(ZCalibrationWindow::WT_INCOHERENT, spectrum, windowIntensity))
        {
            return false;
        }
        normaValue = static_cast<qreal>(windowIntensity);
    }
    else if(zv_normaType == NT_COHERENT_INCOHERENT)
    {
        qint64 coherentValue = 0;
        qint64 incoherentValue = 0;
        if(!zh_getWindowsValue(ZCalibrationWindow::WT_INCOHERENT, spectrum, incoherentValue))
        {
            return false;
        }

        if(!zh_getWindowsValue(ZCalibrationWindow::WT_COHERENT, spectrum, coherentValue))
        {
            return false;
        }

        if(incoherentValue == 0)
        {
            return false;
        }

        normaValue = static_cast<qreal>(coherentValue) / static_cast<qreal>(incoherentValue);
    }
    else if(zv_normaType == NT_INCOHERENT_COHERENT)
    {
        qint64 coherentValue = 0;
        qint64 incoherentValue = 0;
        if(!zh_getWindowsValue(ZCalibrationWindow::WT_INCOHERENT, spectrum, incoherentValue))
        {
            return false;
        }

        if(!zh_getWindowsValue(ZCalibrationWindow::WT_COHERENT, spectrum, coherentValue))
        {
            return false;
        }

        if(coherentValue == 0)
        {
            return false;
        }

        normaValue = static_cast<qreal>(incoherentValue) / static_cast<qreal>(coherentValue);
    }
    else if(zv_normaType == NT_CUSTOM)
    {
        return value;
    }

    if(normaValue == 0)
    {
        return false;
    }

    value /= normaValue;
    return true;
}
//======================================================================
void ZTermNormalizer::zh_createConnections()
{
    connect(this, &ZTermNormalizer::zg_normalizerChanged,
            zv_calibration, &ZCalibration::zh_onNormalizerChange);
}
//======================================================================
bool ZTermNormalizer::zh_getWindowsValue(ZCalibrationWindow::WindowType type,
                                         const ZAbstractSpectrum* spectrum, qint64& value)
{
    qint64 currentValue;
    bool ok;
    value = 0;
    for(int w = 0; w < zv_calibration->zv_windowList.count(); w++)
    {
        if(zv_calibration->zv_windowList.at(w)->zp_windowType() != type)
        {
            continue;
        }

        zv_calibration->zv_windowList.at(w)->zp_calcWindowIntensity(spectrum, currentValue, true, &ok);
        if(!ok)
        {
            return false;
        }

        value += currentValue;
    }

    return true;
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
void ZTermNormalizer::zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType,
                                            ZCalibrationWindow::WindowType currentType) const
{
    if(zv_normaType == NT_NONE)
    {
        return;
    }
    else if(zv_normaType == NT_CUSTOM)
    {
        // TODO check out is destroyed window parted in custom norma
        emit zg_normalizerChanged();
        return;
    }

    if((previousType == ZCalibrationWindow::WT_COHERENT || currentType == ZCalibrationWindow::WT_COHERENT) &&
            (zv_normaType == NT_COHERENT ||  zv_normaType == NT_COHERENT_INCOHERENT
             || zv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit zg_normalizerChanged();
    }
    else if((previousType == ZCalibrationWindow::WT_INCOHERENT || currentType == ZCalibrationWindow::WT_INCOHERENT) &&
            (zv_normaType == NT_INCOHERENT ||  zv_normaType == NT_COHERENT_INCOHERENT
             || zv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit zg_normalizerChanged();
    }

}
//======================================================================
void ZTermNormalizer::zh_onWindowDestroy(QObject* obj) const
{
    if(zv_normaType == NT_NONE)
    {
        return;
    }
    else if(zv_normaType == NT_CUSTOM)
    {
        // TODO check out is destroyed window parted in custom norma
        emit zg_normalizerChanged();
        return;
    }


    ZCalibrationWindow* window = qobject_cast<ZCalibrationWindow*>(obj);
    if(!window)
    {
        emit zg_normalizerChanged();
        return;
    }

    ZCalibrationWindow::WindowType windowType = window->zp_windowType();
    if(windowType == ZCalibrationWindow::WT_COHERENT &&
            (zv_normaType == NT_COHERENT ||  zv_normaType == NT_COHERENT_INCOHERENT
             || zv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit zg_normalizerChanged();
    }
    else if(windowType == ZCalibrationWindow::WT_INCOHERENT &&
            (zv_normaType == NT_INCOHERENT ||  zv_normaType == NT_COHERENT_INCOHERENT
             || zv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit zg_normalizerChanged();
    }
}
//======================================================================
void ZTermNormalizer::zh_onWindowMarginsChange() const
{
    ZCalibrationWindow* window = qobject_cast<ZCalibrationWindow*>(sender());

    if(window->zp_windowType() == ZCalibrationWindow::WT_INCOHERENT &&
            (zv_normaType == NT_INCOHERENT ||  zv_normaType == NT_COHERENT_INCOHERENT
             || zv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit zg_normalizerChanged();
    }
    else if(window->zp_windowType() == ZCalibrationWindow::WT_COHERENT &&
            (zv_normaType == NT_COHERENT ||  zv_normaType == NT_COHERENT_INCOHERENT
             || zv_normaType == NT_INCOHERENT_COHERENT))
    {
        emit zg_normalizerChanged();
    }
}
//======================================================================
