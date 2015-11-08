//==================================================================
#include "ZJointSpectraDataManager.h"
#include "glVariables.h"
//==================================================================
ZJointSpectraDataManager::ZJointSpectraDataManager(QObject *parent) : QObject(parent)
{
    zv_currentArrayIndex = -1;
    zv_spectrumArrayRepositiry = 0;
    zv_calibrationRepository = 0;
    zv_visibleChemElementCount = 0;
    zv_visibleCalibrationCount = 0;

}
//==================================================================
void ZJointSpectraDataManager::zp_setSpectraArrayRepository(ZSpectraArrayRepository* repository)
{
    zv_spectrumArrayRepositiry = repository;
    // array repository <-> array model
    connect(repository, &ZSpectraArrayRepository::zg_currentSpectrumOperation,
            this, &ZJointSpectraDataManager::zh_onRepositoryArrayOperation);
    connect(repository, &ZSpectraArrayRepository::zg_currentChemElementOperation,
            this, &ZJointSpectraDataManager::zh_onRepositoryChemElementOperation);

}
//==================================================================
void ZJointSpectraDataManager::zp_setCalibrationRepository(ZCalibrationRepository* repository)
{
    zv_calibrationRepository = repository;
    connect(repository, &ZCalibrationRepository::zg_currentOperation,
            this, &ZJointSpectraDataManager::zh_onRepositoryCalibrationOperation);

}
//==================================================================
int ZJointSpectraDataManager::zp_rowCount() const
{
    if(zv_currentArrayIndex < 0 || zv_spectrumArrayRepositiry == 0)
    {
        return 0;
    }

    return zv_spectrumArrayRepositiry->zp_spectrumCount(zv_currentArrayIndex);
}
//==================================================================
int ZJointSpectraDataManager::zp_columnCount()
{
//    if(zv_spectrumArrayRepositiry != 0 && zv_currentArrayIndex >= 0)
//    {
//        zv_visibleChemElementCount = zv_spectrumArrayRepositiry->zp_visibleChemElementCount(zv_currentArrayIndex);
//    }
//    else
//    {
//        zv_visibleChemElementCount = 0;
//    }

//    if(zv_calibrationRepository != 0)
//    {
//        zv_visibleCalibrationCount = zv_calibrationRepository->zp_visibleCalibrationCount();
//    }
//    else
//    {
//        zv_visibleCalibrationCount = 0;
//    }

    return zv_spectrumDataColumnCount + zv_visibleChemElementCount + zv_visibleCalibrationCount;
}
//==================================================================
int ZJointSpectraDataManager::zp_visibleChemElementCount() const
{
    if(zv_currentArrayIndex < 0 || zv_spectrumArrayRepositiry == 0)
    {
        return 0;
    }

    return zv_spectrumArrayRepositiry->zp_visibleChemElementCount(zv_currentArrayIndex);
}
//==================================================================
bool ZJointSpectraDataManager::zp_isColumnChemElementColumn(int column) const
{
    if(zv_spectrumArrayRepositiry == 0)
    {
        return false;
    }

    if(column < zv_spectrumDataColumnCount
            || column > zv_spectrumArrayRepositiry->zp_visibleChemElementCount(zv_currentArrayIndex)
            + zv_spectrumDataColumnCount)
    {
        return false;
    }

    return true;
}
//==================================================================
QVariant ZJointSpectraDataManager::zp_data(QModelIndex index) const
{
    if(!zv_spectrumArrayRepositiry || !index.isValid())
    {
        return QVariant();
    }

    if(index.column() == 0)
    {
        return QVariant(zv_spectrumArrayRepositiry->zp_spectrumName(zv_currentArrayIndex, index.row()));
    }
    if(index.column() == 1)
    {

        return QVariant();
        // return QVariant(zv_repositiry->zp_spectrum(zv_currentArrayIndex, index.row()));
    }

    int visibleChemElementCount = zv_spectrumArrayRepositiry->zp_visibleChemElementCount(zv_currentArrayIndex);
    if(index.column() >= zv_spectrumDataColumnCount && index.column() < visibleChemElementCount + zv_spectrumDataColumnCount)
    {
        QString concentrationString = zv_spectrumArrayRepositiry->zp_chemConcentration(zv_currentArrayIndex,
                                                                                       index.row(),
                                                                                       index.column() - zv_spectrumDataColumnCount);
        return QVariant(concentrationString);
    }



    return QVariant();
}
//==================================================================
QString ZJointSpectraDataManager::zp_columnName(int columnIndex) const
{

    if(columnIndex == 2)
    {

    }

    if(columnIndex == 0)
    {
        return tr("File");
    }
    else if(columnIndex == 1)
    {
        return tr("Spectrum");
    }
    else if(columnIndex > 1 && columnIndex < zv_spectrumDataColumnCount + zv_visibleChemElementCount)
    {
        if(zv_spectrumArrayRepositiry == 0)
        {
            return QString();
        }

        int visibleChemElementIndex = columnIndex - zv_spectrumDataColumnCount;
        if(visibleChemElementIndex >= 0
                && visibleChemElementIndex < zv_spectrumArrayRepositiry->zp_visibleChemElementCount(zv_currentArrayIndex))
        {
            return zv_spectrumArrayRepositiry->zp_visibleChemElementName(zv_currentArrayIndex, visibleChemElementIndex);
        }
        else
        {
            return QString();
        }
    }
    else if(columnIndex > 1 && columnIndex < zv_spectrumDataColumnCount +
            zv_visibleChemElementCount + zv_visibleCalibrationCount)
    {
        if(zv_calibrationRepository == 0)
        {
            return QString();
        }

        int visibleCalibrationIndex = columnIndex - zv_spectrumDataColumnCount - zv_visibleChemElementCount;
        if(visibleCalibrationIndex >= 0
                && visibleCalibrationIndex < zv_calibrationRepository->zp_visibleCalibrationCount())
        {
            return zv_calibrationRepository->zp_visibleCalibrationName(visibleCalibrationIndex);
        }
        else
        {
            return QString();
        }

    }

    return QString();
}
//==================================================================
bool ZJointSpectraDataManager::zp_setChemConcentration(int row, int column, const QString& consentration)
{
    if(!zv_spectrumArrayRepositiry || !zp_isColumnChemElementColumn(column)
            || zv_currentArrayIndex < 0 )
    {
        return false;
    }

    bool ok;
    consentration.toDouble(&ok);
    if(!ok)
    {
        return false;
    }

    return zv_spectrumArrayRepositiry->zp_setChemConcentration(zv_currentArrayIndex,
                                                               row,
                                                               column - zv_spectrumDataColumnCount,
                                                               consentration);
}
//==================================================================
void ZJointSpectraDataManager::zp_currentArrayChanged(int current, int previous)
{
    emit zg_currentOperation(OT_RESET_DATA, -1, -1);
    zv_currentArrayIndex = current;
//    if(zv_currentArrayIndex >= 0 && zv_spectrumArrayRepositiry != 0)
//    {
//        zv_visibleChemElementCount = zv_spectrumArrayRepositiry->zp_visibleChemElementCount(zv_currentArrayIndex);
//    }

    zh_defineColumnCounts();
    emit zg_currentOperation(OT_END_RESET_DATA, -1, -1);
}
//==================================================================
void ZJointSpectraDataManager::zh_onRepositoryArrayOperation(ZSpectraArrayRepository::SpectrumOperationType type,
                                                             int arrayIndex, int first, int last)
{
    zh_defineColumnCounts();
    if(zv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    if(type == ZSpectraArrayRepository::SOT_INSERT_SPECTRA)
    {
        emit zg_currentOperation(OT_INSERT_ROW, first, last);
    }
    else if(type == ZSpectraArrayRepository::SOT_END_INSERT_SPECTRA)
    {
        emit zg_currentOperation(OT_END_INSERT_ROW, first, last);
    }
    else if(type == ZSpectraArrayRepository::SOT_REMOVE_SPECTRA)
    {
        emit zg_currentOperation(OT_REMOVE_ROW, first, last);
    }
    else if(type == ZSpectraArrayRepository::SOT_END_REMOVE_SPECTRA)
    {
        emit zg_currentOperation(OT_END_REMOVE_ROW, first, last);
    }

}
//==================================================================
void ZJointSpectraDataManager::zh_onRepositoryChemElementOperation(ZSpectraArrayRepository::ChemElementOperationType type,
                                                                   int arrayIndex, int first, int last)
{
    zh_defineColumnCounts();
    if(zv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    int residual = last - first;
    int visibleFirst = zh_convertChemElementIndexToVisibleChemElementIndex(zv_currentArrayIndex, first);

    if(visibleFirst < 0)
    {
        visibleFirst = 0;
    }

    int visibleLast = visibleFirst + residual;

    visibleFirst += zv_spectrumDataColumnCount;
    visibleLast += zv_spectrumDataColumnCount;

    if(type == ZSpectraArrayRepository::CEOT_INSERT_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_INSERT_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZSpectraArrayRepository::CEOT_END_INSERT_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_END_INSERT_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZSpectraArrayRepository::CEOT_REMOVE_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_REMOVE_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZSpectraArrayRepository::CEOT_END_REMOVE_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_END_REMOVE_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZSpectraArrayRepository::CEOT_CHEM_ELEMENT_CHANGED)
    {
        emit zg_currentOperation(OT_COLUMN_HEADER_CHANGED, visibleFirst, visibleLast);
    }
    else if(type == ZSpectraArrayRepository::CEOT_CHEM_ELEMENT_VISIBILITY_CHANGE)
    {
        bool visible = zv_spectrumArrayRepositiry->zp_chemElementIsVisible(zv_currentArrayIndex, first);
        if(!visible)
        {
            emit zg_currentOperation(OT_INSERT_COLUMN, visibleFirst, visibleLast);
        }
        else
        {
            emit zg_currentOperation(OT_REMOVE_COLUMN, visibleFirst, visibleLast);
        }
    }
    else if(type == ZSpectraArrayRepository::CEOT_END_CHEM_ELEMENT_VISIBILITY_CHANGE)
    {
        bool visible = zv_spectrumArrayRepositiry->zp_chemElementIsVisible(zv_currentArrayIndex, first);
        if(visible)
        {
            emit zg_currentOperation(OT_END_INSERT_COLUMN, visibleFirst, visibleLast);
        }
        else
        {
            emit zg_currentOperation(OT_END_REMOVE_COLUMN, visibleFirst, visibleLast);
        }
    }
}
//==================================================================
void ZJointSpectraDataManager::zh_onRepositoryCalibrationOperation(ZCalibrationRepository::OperationType type, int first, int last)
{
    zh_defineColumnCounts();
    int visibleFirst = zv_spectrumDataColumnCount + zv_visibleChemElementCount;
    int visibleLast = visibleFirst + (last - first);

    if(type == ZCalibrationRepository::OT_INSERT_CALIBRATIONS)
    {
        emit zg_currentOperation(OT_INSERT_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZCalibrationRepository::OT_END_INSERT_CALIBRATIONS)
    {
        emit zg_currentOperation(OT_END_INSERT_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZCalibrationRepository::OT_REMOVE_CALIBRATIONS)
    {
        emit zg_currentOperation(OT_REMOVE_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZCalibrationRepository::OT_END_REMOVE_CALIBRATIONS)
    {
        emit zg_currentOperation(OT_END_REMOVE_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZCalibrationRepository::OT_CALIBRATION_CHANGED)
    {
        emit zg_currentOperation(OT_COLUMN_HEADER_CHANGED, visibleFirst, visibleLast);
    }
    else if(type == ZCalibrationRepository::OT_CALIBRATION_VISIBILITY_CHANGE)
    {
        bool visible = zv_calibrationRepository->zp_calibrationIsVisible(first);
        if(!visible)
        {
            emit zg_currentOperation(OT_INSERT_COLUMN, visibleFirst, visibleLast);
        }
        else
        {
            emit zg_currentOperation(OT_REMOVE_COLUMN, visibleFirst, visibleLast);
        }
    }
    else if(type == ZCalibrationRepository::OT_END_CALIBRATION_VISIBILITY_CHANGE)
    {
        bool visible = zv_calibrationRepository->zp_calibrationIsVisible(first);
        if(visible)
        {
            emit zg_currentOperation(OT_END_INSERT_COLUMN, visibleFirst, visibleLast);
        }
        else
        {
            emit zg_currentOperation(OT_END_REMOVE_COLUMN, visibleFirst, visibleLast);
        }
    }
}
//==================================================================
int ZJointSpectraDataManager::zh_convertChemElementIndexToVisibleChemElementIndex(int arrayIndex,
                                                                                  int originalIndex)
{
    if(zv_spectrumArrayRepositiry == 0)
    {
        return -1;
    }

    return zv_spectrumArrayRepositiry->zp_numberVisibleChemElementsBeforeIndex(arrayIndex, originalIndex);
}
//==================================================================
void ZJointSpectraDataManager::zh_defineColumnCounts()
{
    if(zv_spectrumArrayRepositiry != 0 && zv_currentArrayIndex >= 0)
    {
        zv_visibleChemElementCount = zv_spectrumArrayRepositiry->zp_visibleChemElementCount(zv_currentArrayIndex);
    }
    else
    {
        zv_visibleChemElementCount = 0;
    }

    if(zv_calibrationRepository != 0)
    {
        zv_visibleCalibrationCount = zv_calibrationRepository->zp_visibleCalibrationCount();
    }
    else
    {
        zv_visibleCalibrationCount = 0;
    }
}
//==================================================================