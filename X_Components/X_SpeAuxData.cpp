//=================================================
#include "X_SpeAuxData.h"
//=================================================
X_SpeAuxData::X_SpeAuxData()
    : X_AbstractSpectrumAuxData(DT_SPE)
{
    xv_date = QDate();
    xv_time = QTime();
    xv_exposition = -1;
    xv_aliveTime = -1;
    xv_energyUnit = QString();
    xv_energyK0 = 0;
    xv_energyK1 = 0;
    xv_energyK2 = 0;

    xv_peakWidth = 0;
    xv_peakWidthK0 = 0;
    xv_peakWidthK1 = 0;
    xv_peakWidthK2 = 0;

}
//=================================================
void X_SpeAuxData::xp_setSrvN(const QString& srvN)
{
    xv_srvN = srvN;
}
//=================================================
bool X_SpeAuxData::xp_setDate(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }

    xv_date = QDate::fromString(string, "dd.MM.yyyy");
    return xv_date.isValid();
}
//=================================================
bool X_SpeAuxData::xp_setTime(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }

    xv_time = QTime::fromString(string);
    return xv_time.isValid();
}
//=================================================
void X_SpeAuxData::xp_setDate(const QDate& date)
{
    xv_date = date;
}
//=================================================
void X_SpeAuxData::xp_setTime(const QTime& time)
{
    xv_time = time;
}
//=================================================
void X_SpeAuxData::xp_setDateTime(const QDateTime& dateTime)
{
    xv_date = dateTime.date();
    xv_time = dateTime.time();
}
//=================================================
bool X_SpeAuxData::xp_setExposition(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    xv_exposition = string.toInt(&ok);
    return ok;
}
//=================================================
void X_SpeAuxData::xp_setExposition(int exposition)
{
    xv_exposition = exposition;
}
//=================================================
bool X_SpeAuxData::xp_setAliveTime(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    xv_aliveTime = static_cast<quint32>(string.toInt(&ok));
    return ok;
}
//=================================================
void X_SpeAuxData::xp_setAliveTime(quint32 aliveTime)
{
    xv_aliveTime = aliveTime;
}
//=================================================
bool X_SpeAuxData::xp_setGainFactor(const QString& gainFactor)
{
    if(gainFactor.isEmpty())
    {
        return false;
    }
    bool ok;
    xv_gainFactor = static_cast<quint8>(gainFactor.toUInt(&ok));
    return ok;
}
//=================================================
void X_SpeAuxData::xp_setGainFactor(quint8 gainFactor)
{
    xv_gainFactor = gainFactor;
}
//=================================================
bool X_SpeAuxData::xp_setEnergyUnit(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    if(string == "1")
    {
        xv_energyUnit = "kEv";
        return true;
    }
    else if(string == "2")
    {
        xv_energyUnit = "MEv";
        return true;
    }
    else
    {
        xv_energyUnit = QString();
        return false;
    }
}
//=================================================
bool X_SpeAuxData::xp_setEnergyK0(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    xv_energyK0 = string.toDouble(&ok);
    return ok;
}
//=================================================
void X_SpeAuxData::xp_setEnergyK0(double K0)
{
    xv_energyK0 = K0;
}
//=================================================
bool X_SpeAuxData::xp_setEnergyK1(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    xv_energyK1 = string.toDouble(&ok);
    return ok;
}
//=================================================
void X_SpeAuxData::xp_setEnergyK1(double K1)
{
    xv_energyK1 = K1;
}
//=================================================
bool X_SpeAuxData::xp_setEnergyK2(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    xv_energyK2 = string.toDouble(&ok);
    return ok;
}
//=================================================
void X_SpeAuxData::xp_setEnergyK2(double K2)
{
    xv_energyK2 = K2;
}
//=================================================
bool X_SpeAuxData::xp_setPeakWidth(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    xv_peakWidth = string.toInt(&ok);
    return ok;
}
//=================================================
bool X_SpeAuxData::xp_setPeakWidthK0(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    xv_peakWidthK0 = string.toDouble(&ok);
    return ok;
}
//=================================================
bool X_SpeAuxData::xp_setPeakWidthK1(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    xv_peakWidthK1 = string.toDouble(&ok);
    return ok;
}
//=================================================
bool X_SpeAuxData::xp_setPeakWidthK2(const QString& string)
{
    if(string.isEmpty())
    {
        return false;
    }
    bool ok;
    xv_peakWidthK2 = string.toDouble(&ok);
    return ok;
}
//=================================================
void X_SpeAuxData::xp_setComment(int lineNumber, const QString& comment)
{
    xv_commentMap[lineNumber] = comment;
}
//=================================================
QString X_SpeAuxData::xp_comment(int lineNumber) const
{
    return xv_commentMap.value(lineNumber, QString());
}
//=================================================
QString X_SpeAuxData::xp_srvN() const
{
    return xv_srvN;
}
//=================================================
QDateTime X_SpeAuxData::xp_dateTime() const
{
    QDateTime dateTime = QDateTime(xv_date, xv_time);
    return dateTime;
}
//=================================================
QDate X_SpeAuxData::xp_date() const
{
    return xv_date;
}
//=================================================
QTime X_SpeAuxData::xp_time() const
{
    return xv_time;
}
//=================================================
int X_SpeAuxData::xp_exposition() const
{
    return xv_exposition;
}
//=================================================
quint8 X_SpeAuxData::xp_gainFactor() const
{
    return xv_gainFactor;
}
//=================================================
int X_SpeAuxData::xp_aliveTime() const
{
    return xv_aliveTime;
}
//=================================================
QString X_SpeAuxData::xp_energyUnit() const
{
    return xv_energyUnit;
}
//=================================================
qreal X_SpeAuxData::xp_energyK0() const
{
    return xv_energyK0;
}
//=================================================
qreal X_SpeAuxData::xp_energyK1() const
{
    return xv_energyK1;
}
//=================================================
qreal X_SpeAuxData::xp_energyK2() const
{
    return xv_energyK2;
}
//=================================================
int X_SpeAuxData::xp_peakWidth() const
{
    return xv_peakWidth;
}
//=================================================
qreal X_SpeAuxData::xp_peakWidthK0() const
{
    return xv_peakWidthK0;
}
//=================================================
qreal X_SpeAuxData::xp_peakWidthK1() const
{
    return xv_peakWidthK1;
}
//=================================================
qreal X_SpeAuxData::xp_peakWidthK2() const
{
    return xv_peakWidthK2;
}
//=================================================
void X_SpeAuxData::xp_setEnergyCalibrationFactors(QList<double> energyCalibrationFactors )
{
    xv_energyK0 = energyCalibrationFactors.value(0, 0.0);
    xv_energyK1 = energyCalibrationFactors.value(1, 0.0);
    xv_energyK2 = energyCalibrationFactors.value(2, 0.0);

}
//=================================================
