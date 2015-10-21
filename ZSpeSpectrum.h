//===================================================
#ifndef ZSPESPECTRUM_H
#define ZSPESPECTRUM_H
//===================================================
#include <QStringList>
#include "ZAbstractSpectrum.h"
#include "ZSpeAuxData.h"
//===================================================
class ZSpeSpectrum : public ZAbstractSpectrum
{
public:

    ZSpeSpectrum();
    ZSpeSpectrum(const QString& name);
    virtual ~ZSpeSpectrum();

    ZAbstractSpectrumAuxData zp_auxData();

private:

    // VARS
    ZSpeAuxData zv_auxData;


//    int zv_deviceNumber;
//    QDate zv_date;
//    QTime zv_time;
//    int zv_exposition;
//    int zv_aliveTime;
//    QStringList zv_comments;
//    QString zv_energyUnit;
//    QList<double> zv_energyFactorList;
//    int zv_peakWidht;

};
//===================================================
#endif // ZSPESPECTRUM_H
