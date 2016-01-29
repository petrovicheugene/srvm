//=============================================================================
#include "ZPredictorTableManager.h"
#include "globalVariables.h"

//=============================================================================
ZPredictorTableManager::ZPredictorTableManager(QObject *parent) : QObject(parent)
{
   zv_calibrationRepository = 0;
   zv_currentCalibrationId = -1;
}
//=============================================================================
void ZPredictorTableManager::zp_connectToCalibrationRepository(ZCalibrationRepository* repository)
{
    zv_calibrationRepository = repository;
//    connect(repository, &ZCalibrationRepository::zg_calibrationWindowOperation,
//            this, &ZCalibrationWindowDataManager::zh_onRepositoryWindowOperation);
    connect(repository, &ZCalibrationRepository::zg_currentCalibrationChanged,
            this, &ZPredictorTableManager::zh_currentCalibrationChanged);
}
//=============================================================================
int ZPredictorTableManager::zp_rowCount() const
{
    if(zv_currentCalibrationId < 0 || zv_calibrationRepository == 0)
    {
        return 0;
    }

    return zv_calibrationRepository->zp_calibrationWindowCount(zv_currentCalibrationId);
}
//=============================================================================
int ZPredictorTableManager::zp_columnCount() const
{
   if(zv_currentCalibrationId < 0 || zv_calibrationRepository == 0)
   {
       return 0;
   }

   return zv_calibrationRepository->zp_calibrationWindowCount(zv_currentCalibrationId);
}
//=============================================================================
void ZPredictorTableManager::zh_currentCalibrationChanged(qreal calibrationId, int calibrationIndex)
{
#ifdef DBG
    qDebug() << "CURRENT CALIBRATION" << "ID" << calibrationId << calibrationIndex;
#endif

    if(zv_currentCalibrationId == calibrationId)
    {
        return;
    }

    zv_currentCalibrationId = calibrationId;

#ifdef DBG
    qDebug() << "CALIBRATION MANAGER DATA RESET";
#endif
    emit zg_currentOperation(OT_RESET_DATA, -1, -1);

    emit zg_currentOperation(OT_END_RESET_DATA, -1, -1);
}
//=============================================================================
