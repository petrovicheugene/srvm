//=================================================
#ifndef ZSDCALIBRATION_H
#define ZSDCALIBRATION_H
//=================================================
#include "ZAbstractCalibration.h"
//=================================================


//=================================================
class ZSDCalibration : public ZAbstractCalibration
{
    Q_OBJECT
public:
    explicit ZSDCalibration(const QString& path, QObject *parent = 0);
    virtual ~ZSDCalibration();


    double zp_calcConcentration(ZAbstractSpectrum* const spectrum, bool *ok = 0) override;

};
//=================================================
#endif // ZSDCALIBRATION_H
