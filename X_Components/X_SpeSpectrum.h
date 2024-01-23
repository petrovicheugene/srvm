//===================================================
#ifndef X_SPESPECTRUM_H
#define X_SPESPECTRUM_H
//===================================================
#include <QStringList>
#include "X_AbstractSpectrum.h"
#include "X_SpeAuxData.h"
//===================================================
class X_SpeSpectrum : public X_AbstractSpectrum
{
    Q_OBJECT
public:

    // explicit X_SpeSpectrum(QObject* parent);
    explicit X_SpeSpectrum(const QList<quint32> &intensityList, const X_SpeAuxData& speAuxdata,
                          const QString& path, QColor color, bool completed, QObject* parent);

    virtual ~X_SpeSpectrum();

    const X_AbstractSpectrumAuxData *xp_auxData() const;
    const X_SpeAuxData* xp_speAuxData() const;

    QStringList xp_isEnergyCalibrationAndExpositionSuitable(const QString&,
                                        qreal K0, qreal K1, qreal K2, int exposition);


    void xp_setSpectrumDateTime(QDateTime datetime);
    void xp_setExposition(int exposition);
    void xp_setGainFactor(quint8 gainFactor);
    quint8 xp_gainFactor() const;
    void xp_setAliveTime(quint32 aliveTime);
    void xp_setEnergyUnit(const QString& energyUnit);
    void xp_setEnergyCalibration(const QList<double> &energyCalibrationFactorList);
    QList<double> xp_energyCalibration() const;

signals:

    void xg_energyCalibrationChanged() const;
    void xg_gainFactorChanged() const;

private:

    // VARS
    X_SpeAuxData xv_auxData;

    // int xv_deviceNumber;
    // QDate xv_date;
    // QTime xv_time;
    // int xv_exposition;
    // int xv_aliveTime;
    // QStringList xv_comments;
    // QString xv_energyUnit;
    // QList<double> xv_energyFactorList;
    // int xv_peakWidht;

};
//===================================================
#endif // X_SPESPECTRUM_H
