//=============================================================================
#include "ZTermCorrelationTableManager.h"
#include "globalVariables.h"

//=============================================================================
ZTermCorrelationTableManager::ZTermCorrelationTableManager(QObject *parent) : QObject(parent)
{
    zv_calibrationRepository = 0;
    zv_spectrumArrayRepository = 0;
    zv_currentCalibrationId = -1;
    zv_columnCountCorrector = 0;
}
//=============================================================================
void ZTermCorrelationTableManager::zp_connectToCalibrationRepository(ZCalibrationRepository* repository)
{
    emit zg_currentOperation(TOT_BEGIN_RESET, -1, -1);
    zv_calibrationRepository = repository;
    connect(repository, &ZCalibrationRepository::zg_termOperation,
            this, &ZTermCorrelationTableManager::zh_onRepositoryTermOperation);
    connect(repository, &ZCalibrationRepository::zg_currentCalibrationChanged,
            this, &ZTermCorrelationTableManager::zh_currentCalibrationChanged);
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_calibrationOperation,
            this, &ZTermCorrelationTableManager::zh_onCalibrationRepositoryOperation);
    emit zg_currentOperation(TOT_END_RESET, -1, -1);

}
//=============================================================================
void ZTermCorrelationTableManager::zp_connectToSpectrumArrayRepository(ZSpectrumArrayRepository* repository)
{
    zv_spectrumArrayRepository = repository;
    // array repository <-> array model
    //    connect(repository, &ZSpectrumArrayRepository::zg_spectrumOperation,
    //            this, &ZJointSpectraDataManager::zh_onRepositoryArrayOperation);
    //    connect(repository, &ZSpectrumArrayRepository::zg_chemElementOperation,
    //            this, &ZJointSpectraDataManager::zh_onRepositoryChemElementOperation);
    //    connect(repository, &ZSpectrumArrayRepository::zg_currentArrayIdChanged,
    //            this, &ZJointSpectraDataManager::zh_currentSpectrumArrayChanged);

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
    if(zv_calibrationRepository == 0)
    {
        return 0;
    }

    return zv_calibrationRepository->zp_termCount(zv_currentCalibrationId) + zv_firstNonTermColumnCount + zv_columnCountCorrector; // 2 - term column and chem conc column
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
        bool ok;
        qreal factor = zv_calibrationRepository->zp_termFactor(zv_currentCalibrationId, index.row(), &ok);
        if(!ok)
        {
            return tr("#Error");
        }

        return QVariant(factor);
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
QString ZTermCorrelationTableManager::zp_horizontalColumnName(int column) const
{
    if(column < 0 || column >= zv_calibrationRepository->zp_termCount(zv_currentCalibrationId) + zv_firstNonTermColumnCount)
    {
        return QString();
    }

    if(column == 0)
    {
        return tr("Factor");
    }
    else if(column == 1)
    {
        if(zv_currentCalibrationId < 0 || zv_calibrationRepository == 0)
        {
            return zv_defaultChemElementString;
        }

        QString chemElementString = zv_calibrationRepository->zp_calibrationChemicalElementForId(zv_currentCalibrationId);
        if(chemElementString.isEmpty() || chemElementString == glDefaultChemElementString)
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
        return zv_calibrationRepository->zp_termName(zv_currentCalibrationId, column - zv_firstNonTermColumnCount);
    }

    return QString();
}
//=============================================================================
QString ZTermCorrelationTableManager::zp_verticalColumnName(int row) const
{
    if(row < 0 || row >= zv_calibrationRepository->zp_termCount(zv_currentCalibrationId))
    {
        return QString();
    }

    if(zv_currentCalibrationId < 0 || zv_calibrationRepository == 0)
    {
        return QString();
    }

    return zv_calibrationRepository->zp_termName(zv_currentCalibrationId, row);
}
//=============================================================================
QPixmap ZTermCorrelationTableManager::zp_termStateIcon(int row) const
{
    if(row < 0 || row >= zv_calibrationRepository->zp_termCount(zv_currentCalibrationId))
    {
        return QPixmap();
    }

    if(zv_currentCalibrationId < 0 || zv_calibrationRepository == 0)
    {
        return QPixmap();
    }

    ZAbstractTerm::TermState state = zv_calibrationRepository->zp_termState(zv_currentCalibrationId, row);
    QPixmap pixmap;
    switch(state)
    {

    case ZAbstractTerm::TS_CONST_INCLUDED :
        pixmap = QPixmap(":/images/check-green-1s.png");
        break;
    case ZAbstractTerm::TS_EXAM_WAITING :
        pixmap = QPixmap(":/images/question-yellow.png");
        break;
    case ZAbstractTerm::TS_INCLUDED :
        pixmap = QPixmap(":/images/plus-blue-1s.png");
        break;
    case ZAbstractTerm::TS_EXCEPTED :
        pixmap = QPixmap(":/images/minus-magenta-1s.png");
        break;

    case ZAbstractTerm::TS_CONST_EXCLUDED :
    default:
        pixmap = QPixmap(":/images/cancel-red-1s.png");
        break;
    }

    return pixmap;
}
//=============================================================================
void ZTermCorrelationTableManager::zp_setNextUsersTermState(int termLogIndex)
{
    if(zv_currentCalibrationId < 0 )
    {
        return;
    }

    zv_calibrationRepository->zp_setNextUsersTermState(zv_currentCalibrationId, termLogIndex);
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
    else if(type == ZCalibrationRepository::TOT_TERM_NAME_CHANGED)
    {
        emit zg_currentOperation(TOT_VERTICAL_HEADER_CHANGED, first, last);
        emit zg_currentOperation(TOT_HORIZONTAL_HEADER_CHANGED, first + zv_firstNonTermColumnCount, last + zv_firstNonTermColumnCount);
    }
    else if(type == ZCalibrationRepository::TOT_TERM_STATE_CHANGED)
    {
        emit zg_currentOperation(TOT_VERTICAL_HEADER_CHANGED, first, last);
    }
}
//=============================================================================
void ZTermCorrelationTableManager::zh_onCalibrationRepositoryOperation(ZCalibrationRepository::CalibrationOperationType type,
                                                                       int first, int last)
{
    if(zv_calibrationRepository == 0 || zv_currentCalibrationId < 0)
    {
        return;
    }

    for(int i = first; i <= last; i++)
    {
        if(zv_calibrationRepository->zp_calibrationIdForCalibrationIndex(i) == zv_currentCalibrationId)
        {
            if(type == ZCalibrationRepository::COT_BEGIN_RESET)
            {
                emit zg_currentOperation(TOT_BEGIN_RESET, first, last);
            }
            else if(type == ZCalibrationRepository::COT_END_RESET)
            {
                emit zg_currentOperation(TOT_END_RESET, first, last);
            }
            else if(type == ZCalibrationRepository::COT_INSERT_CALIBRATIONS)
            {
                emit zg_currentOperation(TOT_BEGIN_RESET, first, last);
            }
            else if(type == ZCalibrationRepository::COT_END_INSERT_CALIBRATIONS)
            {
                emit zg_currentOperation(TOT_END_RESET, first, last);
            }
            else if(type == ZCalibrationRepository::COT_REMOVE_CALIBRATIONS)
            {
                emit zg_currentOperation(TOT_BEGIN_RESET, first, last);
            }
            else if(type == ZCalibrationRepository::COT_END_REMOVE_CALIBRATIONS)
            {
                emit zg_currentOperation(TOT_END_RESET, first, last);
            }
            else if(type == ZCalibrationRepository::COT_CALIBRATION_CHANGED)
            {
                emit zg_currentOperation(TOT_BEGIN_RESET, first, last);
                emit zg_currentOperation(TOT_END_RESET, first, last);
            }
        }
    }
}
//=============================================================================
