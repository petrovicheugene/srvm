//======================================================
#include "X_EnergyCalibrationLine.h"
#include "X_LocaleDoubleConverter.h"
//======================================================
X_EnergyCalibrationLine::X_EnergyCalibrationLine(QObject *parent)
    : QObject(parent)
{
    xv_channelNumber = 0.0;
    xv_energyValue = 0.0;
    xv_lineName = QString();
    xv_lineColor = QColor();
}
//======================================================
int X_EnergyCalibrationLine::xp_channelNumber() const
{
    return xv_channelNumber;
}
//======================================================
void X_EnergyCalibrationLine::xp_setChannelNumber(int channelNumber)
{
    if(xv_channelNumber == channelNumber)
    {
        return;
    }
    xv_channelNumber = channelNumber;
    emit xg_channelNumber(xv_channelNumber);
}
//======================================================
double X_EnergyCalibrationLine::xp_energyValue() const
{
    return xv_energyValue;
}
//======================================================
void X_EnergyCalibrationLine::xp_setEnergyValue(double energyValue)
{
    if(xv_energyValue - energyValue == 0.0)
    {
        return;
    }

    xv_energyValue = energyValue;
    emit xg_energyValue(xv_energyValue);
}
//======================================================
void X_EnergyCalibrationLine::xp_setEnergyValueString(const QString& value)
{
    bool ok;
    double dValue = X_LocaleDoubleConverter::toDouble(value, &ok);
    if(!ok)
    {
        return;
    }

    xp_setEnergyValue(dValue);
}
//======================================================
QString X_EnergyCalibrationLine::xp_lineName() const
{
    return xv_lineName;
}
//======================================================
void X_EnergyCalibrationLine::xp_setLineName(const QString& lineName)
{
    if(xv_lineName == lineName)
    {
        return;
    }

    xv_lineName = lineName;
    emit xg_lineName(xv_lineName);
}
//======================================================
QColor X_EnergyCalibrationLine::xp_lineColor() const
{
    return xv_lineColor;
}
//======================================================
void X_EnergyCalibrationLine::xp_setLineColor(QColor color)
{
    if(xv_lineColor == color)
    {
        return;
    }

    xv_lineColor = color;
    emit xg_lineColor(xv_lineColor);
}
//======================================================
bool X_EnergyCalibrationLine::xp_isEnabled() const
{
    return xv_enabled;
}
//======================================================
void X_EnergyCalibrationLine::xp_setEnabled(bool enabled)
{
    xv_enabled = enabled;
}
//======================================================
