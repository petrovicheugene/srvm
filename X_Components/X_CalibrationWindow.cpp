//====================================================
#include "X_CalibrationWindow.h"
#include "X_AbstractSpectrum.h"
#include "X_AbstractTerm.h"
#include <QPointer>

//====================================================
qint64 X_CalibrationWindow::xv_lastWindowId = 0;
QMap<X_CalibrationWindow::WindowType, QPair<QString, QString>> X_CalibrationWindow::xv_typeNameMap
    = X_CalibrationWindow::zh_intTypeNameMap();
//====================================================
QMap<X_CalibrationWindow::WindowType, QPair<QString, QString>> X_CalibrationWindow::zh_intTypeNameMap()
{
    QMap<X_CalibrationWindow::WindowType, QPair<QString, QString>> map;
    QPair<QString, QString> pair;
    pair.first = "Not defined";
    pair.second = QObject::tr("Not defined");
    map.insert(WT_NOT_DEFINED, pair);

    pair.first = "Peak";
    pair.second = QObject::tr("Peak");
    map.insert(WT_PEAK, pair);

    pair.first = "Coherent";
    pair.second = QObject::tr("Coherent");
    map.insert(WT_COHERENT, pair);

    pair.first = "Incoherent";
    pair.second = QObject::tr("Incoherent");
    map.insert(WT_INCOHERENT, pair);

    return map;
}
//====================================================
X_CalibrationWindow::X_CalibrationWindow(QObject* parent) : QObject(parent)
{
    xv_windowName = QString();
    xv_firstChannel = 0;
    xv_lastChannel = 0;
    xv_spectrumId = -1;
    xv_windowIntensityValue = 0;

    xv_type = WT_NOT_DEFINED;
    xv_visible = true;
    xv_windowId = xv_lastWindowId++;
}
//====================================================
//X_CalibrationWindow::X_CalibrationWindow(const QString& name,  WindowType type)
//{
//    xv_windowName = name;
//    xv_type = type;

//    xv_firstChannel = 0;
//    xv_lastChannel = 0;
//    xv_visible = true;
//    xv_windowId = xv_lastWindowId++;
//}
//====================================================
X_CalibrationWindow::X_CalibrationWindow(
    const QString& name, WindowType type, int firstChannel, int lastChannel, QObject* parent)
    : QObject(parent)
{
    xv_windowName = name;
    xv_type = type;

    if (firstChannel > lastChannel)
    {
        qSwap(firstChannel, lastChannel);
    }
    xv_firstChannel = firstChannel;
    xv_lastChannel = lastChannel;
    xv_visible = true;
    xv_windowId = xv_lastWindowId++;
}
//====================================================
bool X_CalibrationWindow::xp_isWindowVisible() const
{
    return xv_visible;
}
//====================================================
bool X_CalibrationWindow::xp_setWindowVisible(bool visible)
{
    if (xv_visible == visible)
    {
        return false;
    }

    xv_visible = visible;
    return true;
}
//====================================================
QString X_CalibrationWindow::xp_windowName() const
{
    return xv_windowName;
}
//====================================================
bool X_CalibrationWindow::xp_setWindowName(const QString& name)
{
    if (name.isEmpty() || name == xv_windowName)
    {
        return false;
    }
    xv_windowName = name;
    emit xg_windowNameChanged();
    return true;
}
//====================================================
int X_CalibrationWindow::xp_firstChannel() const
{
    return xv_firstChannel;
}
//====================================================
int X_CalibrationWindow::xp_lastChannel() const
{
    return xv_lastChannel;
}
//====================================================
bool X_CalibrationWindow::xp_setWindowFirstChannel(int channel)
{
    if (xv_firstChannel == channel || channel < 0 || channel > xv_lastChannel)
    {
        return false;
    }
    xv_firstChannel = channel;
    emit xg_windowMarginsChanged();
    return true;
}
//====================================================
bool X_CalibrationWindow::xp_setWindowLastChannel(int channel)
{
    if (xv_lastChannel == channel || channel < 0 || channel < xv_firstChannel)
    {
        return false;
    }
    xv_lastChannel = channel;
    emit xg_windowMarginsChanged();
    return true;
}
//====================================================
bool X_CalibrationWindow::xp_setWindowMarginChannels(int first, int last)
{
    if (first == xv_firstChannel && last == xv_lastChannel)
    {
        return false;
    }

    if (first > last)
    {
        qSwap(first, last);
    }
    xv_firstChannel = first;
    xv_lastChannel = last;
    emit xg_windowMarginsChanged();
    return true;
}
//====================================================
X_CalibrationWindow::WindowType X_CalibrationWindow::xp_windowType() const
{
    return xv_type;
}
//====================================================
bool X_CalibrationWindow::xp_setWindowType(X_CalibrationWindow::WindowType type)
{
    if (xv_type == type)
    {
        return false;
    }

    X_CalibrationWindow::WindowType previousType = xv_type;
    xv_type = type;
    emit xg_windowTypeChanged(previousType, xv_type);
    return true;
}
//====================================================
qint64 X_CalibrationWindow::xp_windowId() const
{
    return xv_windowId;
}
//====================================================
QString X_CalibrationWindow::xp_displayTypeName(WindowType type)
{
    if (!xv_typeNameMap.keys().contains(type))
    {
        return QString();
    }
    return xv_typeNameMap.value(type).second;
}
//====================================================
QString X_CalibrationWindow::xp_typeName(WindowType type)
{
    if (!xv_typeNameMap.keys().contains(type))
    {
        return QString();
    }
    return xv_typeNameMap.value(type).first;
}
//====================================================
X_CalibrationWindow::WindowType X_CalibrationWindow::xp_typeFromString(const QString& typeName)
{
    QMap<X_CalibrationWindow::WindowType, QPair<QString, QString>>::const_iterator it;
    for (it = xv_typeNameMap.begin(); it != xv_typeNameMap.end(); it++)
    {
        if (it.value().first == typeName || it.value().second == typeName)
        {
            return it.key();
        }
    }

    return X_CalibrationWindow::WT_NOT_DEFINED;
}
//====================================================
void X_CalibrationWindow::xp_calcWindowIntensity(const QObject* spectrumObject,
                                                qreal& intensityValue,
                                                bool useBuffer,
                                                bool* ok)
{
    const X_AbstractSpectrum* spectrum = qobject_cast<const X_AbstractSpectrum*>(spectrumObject);

    if (!spectrum)
    {
        intensityValue = 0.0;
        if (ok)
        {
            *ok = false;
        }
        return;
    }

    if (xv_spectrumId >= 0 && xv_spectrumId == spectrum->xp_spectrumId() && useBuffer)
    {
        intensityValue = xv_windowIntensityValue;
        if (ok)
        {
            *ok = true;
        }
        return;
    }

    bool res = spectrum->xp_intensityInWindow(xv_firstChannel, xv_lastChannel, intensityValue);
    if (res && useBuffer)
    {
        xv_windowIntensityValue = intensityValue;
        xv_spectrumId = spectrum->xp_spectrumId();
    }
    else
    {
        xv_windowIntensityValue = 0;
        xv_spectrumId = -1;
    }

    if (ok)
    {
        *ok = res;
    }
}
//====================================================
