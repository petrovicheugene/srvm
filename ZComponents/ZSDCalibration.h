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

};
//=================================================
#endif // ZSDCALIBRATION_H
