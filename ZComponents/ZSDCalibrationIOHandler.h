//================================================================
#ifndef ZSDCALIBRATIONIOHANDLER_H
#define ZSDCALIBRATIONIOHANDLER_H
//================================================================
#include "ZAbstractCalibrationIOHandler.h"
//================================================================
class ZSDCalibrationIOHandler : public ZAbstractCalibrationIOHandler
{
    Q_OBJECT
public:
    explicit ZSDCalibrationIOHandler(QObject* calibrationParent, QObject* parent = 0);
    virtual ~ZSDCalibrationIOHandler();
    virtual  bool zp_getCalibrationFromFile(QFile &, ZAbstractCalibration*&) override;
    virtual  bool zp_getCalibrationFromFile(const QString&, ZAbstractCalibration*&) override;

};
//================================================================
#endif // ZSDCALIBRATIONIOHANDLER_H
