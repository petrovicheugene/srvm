//====================================================
#include "ZCalibrationWindow.h"
#include <QObject>
//====================================================
qint64 ZCalibrationWindow::zv_lastWindowId = 0;
QMap<ZCalibrationWindow::WindowType, QString> ZCalibrationWindow::zv_typeNameMap = ZCalibrationWindow::zh_intTypeNameMap();
//====================================================
QMap<ZCalibrationWindow::WindowType, QString> ZCalibrationWindow::zh_intTypeNameMap()
{
    QMap<ZCalibrationWindow::WindowType, QString> map;
    map.insert(WT_NOT_DEFINED, QObject::tr("Not defined"));
    map.insert(WT_BASE_PEAK, QObject::tr("Base peak"));
    map.insert(WT_PEAK, QObject::tr("Peak"));
    map.insert(WT_COHERENT, QObject::tr("Coherent"));
    map.insert(WT_INCOHERENT, QObject::tr("Incoherent"));

    return map;
}
//====================================================
ZCalibrationWindow::ZCalibrationWindow()
{
    zv_name = QString();
    zv_firstChannel = 0;
    zv_lastChannel = 0;

    zv_type = WT_NOT_DEFINED;
    zv_visible = true;
    zv_windowId = zv_lastWindowId++;
}
//====================================================
ZCalibrationWindow::ZCalibrationWindow(const QString& name,  WindowType type)
{
    zv_name = name;
    zv_type = type;

    zv_firstChannel = 0;
    zv_lastChannel = 0;
    zv_visible = true;
    zv_windowId = zv_lastWindowId++;
}
//====================================================
bool ZCalibrationWindow::zp_isWindowVisible() const
{
    return zv_visible;
}
//====================================================
void ZCalibrationWindow::zp_setWindowVisible(bool visible)
{
    zv_visible = visible;
}
//====================================================
QString ZCalibrationWindow::zp_name() const
{
    return zv_name;
}
//====================================================
bool ZCalibrationWindow::zp_setName(const QString& name)
{
    if(name.isEmpty())
    {
        return false;
    }
    zv_name = name;
    return true;
}
//====================================================
int ZCalibrationWindow::zp_firstChannel() const
{
    return zv_firstChannel;
}
//====================================================
int ZCalibrationWindow::zp_lastChannel() const
{
    return zv_lastChannel;
}
//====================================================
bool ZCalibrationWindow::zp_setWindowMarginChannels(int first, int last)
{
    if(first == last || (first == zv_firstChannel && last == zv_lastChannel))
    {
        return false;
    }

    zv_firstChannel = first;
    zv_lastChannel = last;
    return true;
}
//====================================================
ZCalibrationWindow::WindowType ZCalibrationWindow::zp_type() const
{
    return zv_type;
}
//====================================================
void ZCalibrationWindow::zp_setType(ZCalibrationWindow::WindowType type)
{
    zv_type = type;
}
//====================================================
qint64 ZCalibrationWindow::zp_windowId() const
{
    return zv_windowId;
}
//====================================================
QString ZCalibrationWindow::zp_typeName(WindowType type)
{
    return zv_typeNameMap.value(type);
}
//====================================================

