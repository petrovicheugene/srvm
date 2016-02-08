//=============================================================================
#include "ZTermCorrelationTableManager.h"
#include "globalVariables.h"

//=============================================================================
ZTermCorrelationTableManager::ZTermCorrelationTableManager(QObject *parent) : QObject(parent)
{
    zv_calibrationRepository = 0;
    zv_currentCalibrationId = -1;
    zv_columnCountCorrector = 0;
}
//=============================================================================
void ZTermCorrelationTableManager::zp_connectToCalibrationRepository(ZCalibrationRepository* repository)
{
    zv_calibrationRepository = repository;
    connect(repository, &ZCalibrationRepository::zg_termOperation,
            this, &ZTermCorrelationTableManager::zh_onRepositoryTermOperation);
    connect(repository, &ZCalibrationRepository::zg_currentCalibrationChanged,
            this, &ZTermCorrelationTableManager::zh_currentCalibrationChanged);
}
//=============================================================================
int ZTermCorrelationTableManager::zp_rowCount() const
{
    if(zv_currentCalibrationId < 0 || zv_calibrationRepository == 0)
    {
        return 0;
    }

    return zv_calibrationRepository->zp_termCount(zv_currentCalibrationId);
}
//=============================================================================
int ZTermCorrelationTableManager::zp_columnCount() const
{
    if(zv_currentCalibrationId < 0 || zv_calibrationRepository == 0)
    {
        return 0;
    }

    return zv_calibrationRepository->zp_termCount(zv_currentCalibrationId) + 2 + zv_columnCountCorrector; // 2 - term column and chem conc column
}
//=============================================================================
QVariant ZTermCorrelationTableManager::zp_data(QModelIndex index) const
{
    if(!zv_calibrationRepository || !index.isValid())
    {
        return QVariant();
    }

    if(index.column() == 0)
    {
        return QVariant(zv_calibrationRepository->zp_termName(zv_currentCalibrationId, index.row()));
    }
    //    if(index.column() == 1)
    //    {
    //        ZSpectrumPaintData paintData;
    //        paintData.spectrumData = zv_spectrumArrayRepositiry->zp_spectrumData(zv_currentArrayIndex, index.row());
    //        paintData.maxChannel = zv_spectrumArrayRepositiry->zp_arrayChannelCount(zv_currentArrayIndex);
    //        paintData.maxIntensity = zv_spectrumArrayRepositiry->zp_arrayMaxIntensity(zv_currentArrayIndex);
    //        return QVariant::fromValue(paintData);
    //    }

    //    // int visibleChemElementCount = zv_spectrumArrayRepositiry->zp_visibleChemElementCount(zv_currentArrayIndex);
    //    if(index.column() >= zv_spectrumDataColumnCount
    //            && index.column() < zv_visibleChemElementCount + zv_spectrumDataColumnCount)
    //    {
    //        QString concentrationString = zv_spectrumArrayRepositiry->zp_chemConcentration(zv_currentArrayIndex,
    //                                                                                       index.row(),
    //                                                                                       index.column() - zv_spectrumDataColumnCount);
    //        return QVariant(concentrationString);
    //    }

    //    if(index.column() >= zv_spectrumDataColumnCount + zv_visibleChemElementCount
    //            && index.column() < zv_spectrumDataColumnCount + zv_visibleChemElementCount + zv_visibleCalibrationCount)
    //    {
    //        QString calibrationString = zv_calibrationRepository->zp_calibrationName(index.column()
    //                                                                                 - zv_spectrumDataColumnCount - zv_visibleChemElementCount);
    //        if(!zv_calculatedConcentrationMap.keys().contains(calibrationString))
    //        {
    //            return QVariant();
    //        }

    //        QString str = zv_calculatedConcentrationMap.value(calibrationString).value(index.row());
    //        return str; // zv_calculatedConcentrationMap.value(calibrationString).value(index.row());
    //    }

    return QVariant();

}
//=============================================================================
QString ZTermCorrelationTableManager::zp_columnName(int columnIndex) const
{

    if(columnIndex < 0 || columnIndex >= zv_calibrationRepository->zp_termCount(zv_currentCalibrationId) + zv_firstNonTermColumnCount)
    {
        return QString();
    }

    if(columnIndex == 0)
    {
        return tr("Term");
    }
    else if(columnIndex == 1)
    {
        if(zv_currentCalibrationId < 0 || zv_calibrationRepository == 0)
        {
            return zv_defaultChemElementString;
        }

        QString chemElementString = zv_calibrationRepository->zp_calibrationChemicalElementForId(zv_currentCalibrationId);
        if(chemElementString.isEmpty())
        {
            return zv_defaultChemElementString;
        }

        return chemElementString;
    }
    else
    {
        if(zv_currentCalibrationId < 0 || zv_calibrationRepository == 0)
        {
            return QString();
        }
        return zv_calibrationRepository->zp_termName(zv_currentCalibrationId, columnIndex - zv_firstNonTermColumnCount);
    }

    //    if(columnIndex == 0)
    //    {
    //        return tr("File");
    //    }
    //    else if(columnIndex == 1)
    //    {
    //        return tr("Spectrum");
    //    }
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
void ZTermCorrelationTableManager::zh_currentCalibrationChanged(qreal calibrationId, int calibrationIndex)
{
    if(zv_currentCalibrationId == calibrationId)
    {
        return;
    }

    zv_currentCalibrationId = calibrationId;

    emit zg_currentOperation(TOT_BEGIN_RESET, -1, -1);

    emit zg_currentOperation(TOT_END_RESET, -1, -1);
}
//=============================================================================
void ZTermCorrelationTableManager::zh_onRepositoryTermOperation(ZCalibrationRepository::TermOperationType type,
                                                                int calibrationIndex, int first, int last)
{
    if(zv_calibrationRepository == 0)
    {
        return;
    }

    qint64 calibrationId = zv_calibrationRepository->zp_calibrationIdForCalibrationIndex(calibrationIndex);
    if(zv_currentCalibrationId != calibrationId)
    {
        return;
    }

    if(type == ZCalibrationRepository::TOT_BEGIN_RESET)
    {
        emit zg_currentOperation(TOT_BEGIN_RESET, first, last);
    }
    else if(type == ZCalibrationRepository::TOT_END_RESET)
    {
        emit zg_currentOperation(TOT_END_RESET, first, last);
    }
    else if(type == ZCalibrationRepository::TOT_BEGIN_INSERT_TERM)
    {
        emit zg_currentOperation(TOT_BEGIN_INSERT_ROW, first, last);
        //emit zg_currentOperation(TOT_BEGIN_INSERT_COLUMN, first + zv_firstNonTermColumnCount, last + zv_firstNonTermColumnCount);
    }
    else if(type == ZCalibrationRepository::TOT_END_INSERT_TERM)
    {
        zv_columnCountCorrector = (last - first + 1) * -1;
        emit zg_currentOperation(TOT_END_INSERT_ROW, first, last);
        emit zg_currentOperation(TOT_BEGIN_INSERT_COLUMN, first + zv_firstNonTermColumnCount, last + zv_firstNonTermColumnCount);
        zv_columnCountCorrector = 0;
        emit zg_currentOperation(TOT_END_INSERT_COLUMN, first + zv_firstNonTermColumnCount, last + zv_firstNonTermColumnCount);
    }
    else if(type == ZCalibrationRepository::TOT_BEGIN_REMOVE_TERM)
    {
        emit zg_currentOperation(TOT_BEGIN_REMOVE_ROW, first, last);
        // emit zg_currentOperation(TOT_BEGIN_REMOVE_COLUMN, first + zv_firstNonTermColumnCount, last + zv_firstNonTermColumnCount);
    }
    else if(type == ZCalibrationRepository::TOT_END_REMOVE_TERM)
    {
        zv_columnCountCorrector = last - first + 1;
        emit zg_currentOperation(TOT_END_REMOVE_ROW, first, last);
        emit zg_currentOperation(TOT_BEGIN_REMOVE_COLUMN, first + zv_firstNonTermColumnCount, last + zv_firstNonTermColumnCount);
        zv_columnCountCorrector = 0;
        emit zg_currentOperation(TOT_END_REMOVE_COLUMN, first + zv_firstNonTermColumnCount, last + zv_firstNonTermColumnCount);
    }
    else if(type == ZCalibrationRepository::TOT_TERM_CHANGED)
    {
        emit zg_currentOperation(TOT_DATA_CHANGED, first, last);
    }
}
//=============================================================================
