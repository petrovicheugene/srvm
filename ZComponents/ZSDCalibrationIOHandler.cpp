//================================================================
#include "ZSDCalibrationIOHandler.h"
#include "ZSDCalibration.h"
//================================================================
ZSDCalibrationIOHandler::ZSDCalibrationIOHandler(QObject *calibrationParent, QObject *parent)
    : ZAbstractCalibrationIOHandler(calibrationParent, parent)
{

}
//================================================================
ZSDCalibrationIOHandler::~ZSDCalibrationIOHandler()
{

}
//================================================================
bool ZSDCalibrationIOHandler::zp_getCalibrationFromFile(QFile & file,
                                                        ZAbstractCalibration*& calibration)
{
    return false;
}
//================================================================
bool ZSDCalibrationIOHandler::zp_getCalibrationFromFile(const QString& fileName,
                                                        ZAbstractCalibration*& calibration)
{
    calibration = new ZSDCalibration(fileName, zv_calibrationParent);
    return true;
}
//================================================================
