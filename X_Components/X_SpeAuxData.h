//=================================================
#ifndef X_SPEAUXDATA_H
#define X_SPEAUXDATA_H
//=================================================
#include "X_AbstractSpectrumAuxData.h"
#include <QDateTime>
#include <QMap>
#include <QString>
//=================================================
class X_SpeAuxData : public X_AbstractSpectrumAuxData
{
public:
    X_SpeAuxData();

    void xp_setSrvN(const QString&);
    bool xp_setDate(const QString&);
    bool xp_setTime(const QString&);
    void xp_setDate(const QDate&);
    void xp_setTime(const QTime&);
    void xp_setDateTime(const QDateTime&);

    bool xp_setExposition(const QString&);
    void xp_setExposition(int exposition);

    bool xp_setAliveTime(const QString&);
    void xp_setAliveTime(quint32 aliveTime);

    bool xp_setGainFactor(const QString&);
    void xp_setGainFactor(quint8 gainFactor);

    bool xp_setEnergyUnit(const QString&);

    bool xp_setEnergyK0(const QString&);
    void xp_setEnergyK0(double K0);

    bool xp_setEnergyK1(const QString&);
    void xp_setEnergyK1(double K1);

    bool xp_setEnergyK2(const QString&);
    void xp_setEnergyK2(double K2);

    bool xp_setPeakWidth(const QString&);
    bool xp_setPeakWidthK0(const QString&);
    bool xp_setPeakWidthK1(const QString&);
    bool xp_setPeakWidthK2(const QString&);


    void xp_setComment(int lineNumber, const QString& comment);
    QString xp_comment(int lineNumber) const;

    QString xp_srvN() const;
    QDateTime xp_dateTime() const;
    QDate xp_date() const;
    QTime xp_time() const;
    int xp_exposition() const;
    quint8 xp_gainFactor() const;
    int xp_aliveTime() const;
    QString xp_energyUnit() const;
    qreal xp_energyK0() const;
    qreal xp_energyK1() const;
    qreal xp_energyK2() const;

    int xp_peakWidth() const;
    qreal xp_peakWidthK0() const;
    qreal xp_peakWidthK1() const;
    qreal xp_peakWidthK2() const;

    void xp_setEnergyCalibrationFactors(QList<double> energyCalibrationFactors );

    private:

    // VARS
    QMap<int, QString> xv_commentMap;
    QString xv_srvN;
    QDate xv_date;
    QTime xv_time;
    int xv_exposition;
    quint32 xv_aliveTime;
    quint8 xv_gainFactor;

    QString xv_energyUnit;
    qreal xv_energyK0;
    qreal xv_energyK1;
    qreal xv_energyK2;

    int xv_peakWidth;
    qreal xv_peakWidthK0;
    qreal xv_peakWidthK1;
    qreal xv_peakWidthK2;


};
//=================================================
#endif // X_SPEAUXDATA_H
