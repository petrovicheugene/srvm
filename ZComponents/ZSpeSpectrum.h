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
    Q_OBJECT
public:

    // explicit ZSpeSpectrum(QObject* parent);
    explicit ZSpeSpectrum(const QList<int> &intensityList, const QString& path, QObject* parent);
    virtual ~ZSpeSpectrum();

    ZAbstractSpectrumAuxData zp_auxData() const override;

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
