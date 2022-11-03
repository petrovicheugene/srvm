//===================================================
#include "X_SpeSpectrum.h"
//===================================================
//X_SpeSpectrum::X_SpeSpectrum(QObject *parent) : X_AbstractSpectrum(parent)
//{
//    xv_type = ST_SPE;
//}
//===================================================
X_SpeSpectrum::X_SpeSpectrum(const QList<quint32>& intensityList, const X_SpeAuxData& speAuxData,
                           const QString& name, QColor color, bool completed, QObject *parent)
    : X_AbstractSpectrum(intensityList, name, color, completed, parent)
{
    xv_auxData = speAuxData;
}
//===================================================
X_SpeSpectrum::~X_SpeSpectrum()
{

}
//===================================================
const X_AbstractSpectrumAuxData* X_SpeSpectrum::xp_auxData() const
{
    return &xv_auxData;
}
//===================================================
const X_SpeAuxData* X_SpeSpectrum::xp_speAuxData() const
{
    return &xv_auxData;
}
//===================================================
QStringList X_SpeSpectrum::xp_isEnergyCalibrationAndExpositionSuitable(const QString& energyUnit,
                                                                      qreal K0, qreal K1, qreal K2, int exposition)
{
    QStringList inconsistenciesList;

    if(xv_auxData.xp_energyUnit() != energyUnit)
    {
        inconsistenciesList << tr("energy unit");
    }

    if(xv_auxData.xp_energyK0() - K0 != 0.0)
    {
        inconsistenciesList << tr("energy factor K0");
    }

    if(xv_auxData.xp_energyK1() - K1 != 0.0)
    {
        inconsistenciesList << tr("energy factor K1");
    }

    if(xv_auxData.xp_energyK2() - K2 != 0.0)
    {
        inconsistenciesList << tr("energy factor K2");
    }

    if(xv_auxData.xp_exposition() != exposition)
    {
        inconsistenciesList << tr("exposition");
    }

    return inconsistenciesList;
}
//===================================================
void X_SpeSpectrum::xp_setSpectrumDateTime(QDateTime datetime)
{
    xv_auxData.xp_setDateTime(datetime);
}
//===================================================
void X_SpeSpectrum::xp_setExposition(int exposition)
{
    xv_auxData.xp_setExposition(exposition);
}
//===================================================
void X_SpeSpectrum::xp_setGainFactor(quint8 gainFactor)
{
    xv_auxData.xp_setGainFactor(gainFactor);
    emit xg_gainFactorChanged();
}
//===================================================
quint8 X_SpeSpectrum::xp_gainFactor() const
{
    return xv_auxData.xp_gainFactor();
}
//===================================================
void X_SpeSpectrum::xp_setAliveTime(quint32 aliveTime)
{
    xv_auxData.xp_setAliveTime(aliveTime);
}
//===================================================
void X_SpeSpectrum::xp_setEnergyUnit(const QString& energyUnit)
{
    xv_auxData.xp_setEnergyUnit(energyUnit);
}
//===================================================
void X_SpeSpectrum::xp_setEnergyCalibration(const QList<double>& energyCalibrationFactorList)
{
    xv_auxData.xp_setEnergyK0(energyCalibrationFactorList.value(0, 0.0));
    xv_auxData.xp_setEnergyK1(energyCalibrationFactorList.value(1, 0.0));
    xv_auxData.xp_setEnergyK2(energyCalibrationFactorList.value(2, 0.0));
    emit xg_energyCalibrationChanged();
}
//===================================================
QList<double> X_SpeSpectrum::xp_energyCalibration() const
{
    QList<double> energyCalibrationFactorList;
    energyCalibrationFactorList.append(xv_auxData.xp_energyK0());
    energyCalibrationFactorList.append(xv_auxData.xp_energyK1());
    energyCalibrationFactorList.append(xv_auxData.xp_energyK2());
    return energyCalibrationFactorList;
}
//===================================================
