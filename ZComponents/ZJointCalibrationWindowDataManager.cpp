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
int ZJointCalibrationWindowDataManager::zp_rowCount() const
{
    return 1;
//    if(zv_currentArrayIndex < 0 || zv_spectrumArrayRepositiry == 0)
//    {
//        return 0;
//    }

//    return zv_spectrumArrayRepositiry->zp_spectrumCount(zv_currentArrayIndex);
}
//=============================================================================
int ZJointCalibrationWindowDataManager::zp_columnCount() const
{
    return 6;
   // return zv_spectrumDataColumnCount + zv_visibleChemElementCount + zv_visibleCalibrationCount;
}
//=============================================================================
QString ZJointCalibrationWindowDataManager::zp_columnName(int columnIndex) const
{
    if(columnIndex == 0)
    {
        return tr("Window");
    }
    else if(columnIndex == 1)
    {
        return tr("Type");
    }
    else if(columnIndex == 2)
    {
        return tr("First channel");
    }
    else if(columnIndex == 3)
    {
        return tr("Last channel");
    }
    else if(columnIndex == 4)
    {
        return tr("Simple factor");
    }
    else if(columnIndex == 5)
    {
        return tr("Square factor");
    }


//    else if(columnIndex > 1 && columnIndex < zv_spectrumDataColumnCount + zv_visibleChemElementCount)
//    {
//        if(zv_spectrumArrayRepositiry == 0)
//        {
//            return QString();
//        }

//        int visibleChemElementIndex = columnIndex - zv_spectrumDataColumnCount;
//        if(visibleChemElementIndex >= 0
//                && visibleChemElementIndex < zv_spectrumArrayRepositiry->zp_visibleChemElementCount(zv_currentArrayIndex))
//        {
//            return zv_spectrumArrayRepositiry->zp_visibleChemElementName(zv_currentArrayIndex, visibleChemElementIndex);
//        }
//        else
//        {
//            return QString();
//        }
//    }
//    else if(columnIndex > 1 && columnIndex < zv_spectrumDataColumnCount +
//            zv_visibleChemElementCount + zv_visibleCalibrationCount)
//    {
//        if(zv_calibrationRepository == 0)
//        {
//            return QString();
//        }

//        int visibleCalibrationIndex = columnIndex - zv_spectrumDataColumnCount - zv_visibleChemElementCount;
//        if(visibleCalibrationIndex >= 0
//                && visibleCalibrationIndex < zv_calibrationRepository->zp_visibleCalibrationCount())
//        {
//            return zv_calibrationRepository->zp_visibleCalibrationName(visibleCalibrationIndex);
//        }
//        else
//        {
//            return QString();
//        }

//    }

    return QString();
}
//=============================================================================


