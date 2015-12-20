//=============================================================================
#include "ZJointCalibrationWindowDataManager.h"
#include "globalVariables.h"

//=============================================================================
ZJointCalibrationWindowDataManager::ZJointCalibrationWindowDataManager(QObject *parent) : QObject(parent)
{

}
//=============================================================================
void ZJointCalibrationWindowDataManager::zp_connectToSpectraArrayRepository(ZSpectrumArrayRepository* repository)
{
//    zv_spectrumArrayRepositiry = repository;
//    // array repository <-> array model
//    connect(repository, &ZSpectraArrayRepository::zg_currentSpectrumOperation,
//            this, &ZJointSpectraDataManager::zh_onRepositoryArrayOperation);
//    connect(repository, &ZSpectraArrayRepository::zg_currentChemElementOperation,
//            this, &ZJointSpectraDataManager::zh_onRepositoryChemElementOperation);
}
//=============================================================================
void ZJointCalibrationWindowDataManager::zp_connectToCalibrationRepository(ZCalibrationRepository* repository)
{
//    zv_calibrationRepository = repository;
//    connect(repository, &ZCalibrationRepository::zg_currentOperation,
//            this, &ZJointSpectraDataManager::zh_onRepositoryCalibrationOperation);

}
//=============================================================================
