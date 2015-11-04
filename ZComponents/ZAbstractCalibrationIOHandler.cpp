//================================================================
#include "ZAbstractCalibrationIOHandler.h"
//================================================================
ZAbstractCalibrationIOHandler::ZAbstractCalibrationIOHandler(QObject *calibrationParent, QObject *parent)
    : QObject(parent)
{
    zv_calibrationParent = calibrationParent;
}
//================================================================
ZAbstractCalibrationIOHandler::~ZAbstractCalibrationIOHandler()
{

}
//================================================================
