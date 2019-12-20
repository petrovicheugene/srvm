//=============================================================================
#include "ZTermCorrelationTableManager.h"
#include "ZLESGaussSolver.h"
#include "ZGeneral.h"
#include "ZJointSpectraDataManager.h"

#include <math.h>
#include <QPair>
#include <math.h>
//=============================================================================
ZTermCorrelationTableManager::ZTermCorrelationTableManager(QObject *parent) : QObject(parent)
{

    zv_calibrationRepository = nullptr;
    zv_spectrumArrayRepository = nullptr;
    zv_jointSpectraDataManager = nullptr;
    zv_currentCalibrationId = -1;
    zv_currentCalibrationIndex = -1;
    zv_columnCountCorrector = 0;
    zv_currentArrayId = -1;
    zv_currentArrayIndex = -1;
    zv_averageEquationIntercept = std::numeric_limits<double>::quiet_NaN();

    zv_greenCell = QColor(Qt::green);
    zv_yellowCell = QColor(Qt::yellow);
    zv_blueCell = QColor(Qt::blue);
    zv_cyanCell = QColor(Qt::cyan);
    zv_magentaCell = QColor(Qt::cyan);
    zv_redCell = QColor(Qt::red);

    zv_greenCell.setAlpha(30);
    zv_yellowCell.setAlpha(30);
    zv_blueCell.setAlpha(30);
    zv_cyanCell.setAlpha(30);
    zv_magentaCell.setAlpha(30);
    zv_redCell.setAlpha(30);

}
//=============================================================================
void ZTermCorrelationTableManager::zp_connectToCalibrationRepository(ZCalibrationRepository* repository)
{
    emit zg_currentOperation(TOT_BEGIN_RESET, -1, -1);
    zv_calibrationRepository = repository;
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_termOperation,
            this, &ZTermCorrelationTableManager::zh_onRepositoryTermOperation);
    //    connect(repository, &ZCalibrationRepository::zg_calibrationWindowOperation,
    //            this, &ZTermCorrelationTableManager::zh_onCalibrationWindowOperation);

    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_currentCalibrationChanged,
            this, &ZTermCorrelationTableManager::zh_currentCalibrationChanged);
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_calibrationOperation,
            this, &ZTermCorrelationTableManager::zh_onCalibrationRepositoryOperation);
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_normalizerChanged,
            this, &ZTermCorrelationTableManager::zh_onCalibrationNormalizerChange);

    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_invokeCalibrationRecalc,
            this, &ZTermCorrelationTableManager::zh_recalcCalibrationFactors);

    connect(this, &ZTermCorrelationTableManager::zg_currentTermChanged,
            zv_calibrationRepository, &ZCalibrationRepository::zp_onCurrentTermChange);
    connect(this, &ZTermCorrelationTableManager::zg_termDoubleClicked,
            zv_calibrationRepository, &ZCalibrationRepository::zp_onTermDoubleClocked);


    emit zg_currentOperation(TOT_END_RESET, -1, -1);

}
//=============================================================================
void ZTermCorrelationTableManager::zp_connectToSpectrumArrayRepository(ZSpectrumArrayRepository* repository)
{
    zv_spectrumArrayRepository = repository;
    // array repository <-> array model
    //    connect(repository, &ZSpectrumArrayRepository::zg_spectrumOperation,
    //            this, &ZJointSpectraDataManager::zh_onRepositoryArrayOperation);
    connect(repository, &ZSpectrumArrayRepository::zg_chemElementOperation,
            this, &ZTermCorrelationTableManager::zh_onRepositoryChemElementOperation);
    connect(repository, &ZSpectrumArrayRepository::zg_currentArrayIdChanged,
            this, &ZTermCorrelationTableManager::zh_currentSpectrumArrayChanged);
    connect(repository, &ZSpectrumArrayRepository::zg_spectrumOperation,
            this, &ZTermCorrelationTableManager::zh_onSpectrumOperation);

}
//=============================================================================
void ZTermCorrelationTableManager::zp_connectToJointSpectraDataManager(ZJointSpectraDataManager* jointSpectraDataManager)
{
    zv_jointSpectraDataManager = jointSpectraDataManager;
    connect(this, &ZTermCorrelationTableManager::zg_calculateCalibrationQualityData,
            jointSpectraDataManager, &ZJointSpectraDataManager::zp_calculateCalibrationQualityData);
    connect(jointSpectraDataManager, &ZJointSpectraDataManager::zg_calibrationValuesChanged,
            this, &ZTermCorrelationTableManager::zh_onCalibrationValuesChanged);

}
//=============================================================================
int ZTermCorrelationTableManager::zp_rowCount() const
{
    if(zv_currentCalibrationId < 0 || zv_calibrationRepository == nullptr)
    {
        return 0;
    }

    return zv_calibrationRepository->zp_termCount(zv_currentCalibrationId);
}
//=============================================================================
int ZTermCorrelationTableManager::zp_columnCount() const
{
    if(zv_calibrationRepository == nullptr)
    {
        return 0;
    }

    return zv_calibrationRepository->zp_termCount(zv_currentCalibrationId) + zv_firstNonTermColumnCount + zv_columnCountCorrector; // 2 - term column and chem conc column
}
//=============================================================================
bool ZTermCorrelationTableManager::zp_isRowEditable(int row) const
{
    if(!zv_calibrationRepository || row < 0 || row >= zp_rowCount())
    {
        return false;
    }

    return zv_calibrationRepository->zp_equationType(zv_currentCalibrationIndex) != ZCalibration::ET_FRACTIONAL
            || zv_calibrationRepository->zp_baseTermIndex(zv_currentCalibrationIndex) != row;

}
//=============================================================================
QVariant ZTermCorrelationTableManager::zp_data(QModelIndex index) const
{
    if(!zv_calibrationRepository || !index.isValid() || index.row() < 0 || index.column() < 0
            || index.row() >= zp_rowCount() || index.column() >= zp_columnCount())
    {
        return QVariant();
    }

    if(index.column() == 0)
    {
        QString factorString;
        if(zv_calibrationRepository->zp_equationType(zv_currentCalibrationIndex) == ZCalibration::ET_FRACTIONAL
                && zv_calibrationRepository->zp_baseTermIndex(zv_currentCalibrationIndex) == index.row())
        {
            return tr("Base");
        }

        bool ok = zv_calibrationRepository->zp_termFactorString(zv_currentCalibrationId, index.row(), factorString);
        if(!ok)
        {
            return tr("#Error");
        }

        return QVariant(factorString);
    }
    else if(index.column() == 1)
    {
        if(index.row() >= zv_concentrationCorrelationList.count())
        {
            return QVariant();
        }

        return QVariant(zv_concentrationCorrelationList.at(index.row()));
    }
    else if(index.column() == 2)
    {
        if(index.row() >= zv_residualCorrelationList.count())
        {
            return QVariant();
        }

        return QVariant(zv_residualCorrelationList.at(index.row()));
    }
    else if(index.column() >= zv_firstNonTermColumnCount)
    {
        // col index conversion
        int col = index.column() - zv_firstNonTermColumnCount;
        int row = index.row();

        if(col == row)
        {
            return QVariant();
        }

        if(!zh_convertColRowForInterCorrelationMatrix(row, col))
        {
            return QVariant(tr("Error"));
        }

        return QVariant(zv_termIntercorrelationMatrix.at(row).at(col));// value(QPair<int, int>(termColumn, termRow)));
    }

    return QVariant();
}

//=============================================================================
bool ZTermCorrelationTableManager::zp_setData(QModelIndex index, QVariant vFactor)
{
    if(!zv_calibrationRepository || !index.isValid() || index.row() < 0 || index.row() >= zp_rowCount()
            || index.column() != 0
            || !vFactor.isValid() || vFactor.isNull() || !vFactor.canConvert<QString>())
    {
        return false;
    }

    if(zv_calibrationRepository->zp_baseTermIndexForCalibrationId(zv_currentCalibrationId) == index.row())
    {
        return false;
    }

    QString factorString = vFactor.toString();
    return zv_calibrationRepository->zp_setTermFactorString(zv_currentCalibrationId, index.row(), factorString);
}
//=============================================================================
QVariant ZTermCorrelationTableManager::zp_cellColor(QModelIndex index) const
{
    if(!zv_calibrationRepository || !index.isValid() || index.row() < 0 || index.column() < 0
            || index.row() >= zp_rowCount() || index.column() >= zp_columnCount())
    {
        return QVariant();
    }

    qreal correlationValue = 0;
    bool ok;

    // chem Correlations
    if(index.column() < zv_firstNonTermColumnCount)
    {
        // correlations in column
        if(index.column() == 1)
        {
            correlationValue = qAbs(zv_concentrationCorrelationList.value(index.row(), QString()).toDouble(&ok));
        }
        else if(index.column() == 2)
        {
            correlationValue = qAbs(zv_residualCorrelationList.value(index.row(), QString()).toDouble(&ok));
        }
        else
        {
            return QVariant();
        }

        // colors
        if(!ok)
        {
            return QVariant();
        }

        if(correlationValue >= 0.7)
        {
            return QVariant(zv_greenCell);
        }
        else if(correlationValue < 0.7 && correlationValue >= 0.5)
        {
            return QVariant(zv_cyanCell);
        }
        else if(correlationValue < 0.5 && correlationValue >= 0.3)
        {
            return QVariant(zv_magentaCell);
        }
    }
    else //if(index.column() >=  zv_firstNonTermColumnCount)
    {
        QVariant vData = zp_data(index);

        if(!vData.isValid() || vData.isNull() || !vData.canConvert<double>())
        {
            return QVariant();
        }

        correlationValue = qAbs(vData.toDouble(&ok));

        if(!ok)
        {
            return QVariant();
        }

        if(correlationValue >= 0.7)
        {
            return QVariant(zv_redCell);
        }
        else if(correlationValue < 0.7 && correlationValue >= 0.5)
        {
            return QVariant(zv_yellowCell);
        }
    }
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
        if(zv_currentCalibrationId < 0 || zv_calibrationRepository == nullptr)
        {
            return zv_defaultChemElementString;
        }

        QString chemElementString = zv_calibrationRepository->zp_chemElementForCalibrationId(zv_currentCalibrationId);
        if(chemElementString.isEmpty() || chemElementString == glDefaultChemElementString)
        {
            return zv_defaultChemElementString;
        }

        return chemElementString;
    }
    else if(column == 2)
    {
        return tr("Residual");
    }
    else
    {
        if(zv_currentCalibrationId < 0 || zv_calibrationRepository == nullptr)
        {
            return QString();
        }
        return zv_calibrationRepository->zp_termName(zv_currentCalibrationId, column - zv_firstNonTermColumnCount);
    }

}
//=============================================================================
QString ZTermCorrelationTableManager::zp_verticalColumnName(int row) const
{
    if(row < 0 || row >= zv_calibrationRepository->zp_termCount(zv_currentCalibrationId))
    {
        return QString();
    }

    if(zv_currentCalibrationId < 0 || zv_calibrationRepository == nullptr)
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

    if(zv_currentCalibrationId < 0 || zv_calibrationRepository == nullptr)
    {
        return QPixmap();
    }

    ZAbstractTerm::TermState state = zv_calibrationRepository->zp_termState(zv_currentCalibrationId, row);
    QPixmap pixmap;
    switch(state)
    {
    case ZAbstractTerm::TS_BASE :
        pixmap = QPixmap(NS_Icons::glBaseTermIconString);
        break;
    case ZAbstractTerm::TS_CONST_INCLUDED :
        pixmap = QPixmap(NS_Icons::glConstIncludedIconString);
        break;
    case ZAbstractTerm::TS_EXAM_WAITING :
        pixmap = QPixmap(NS_Icons::glExamWaitingIconString);
        break;
    case ZAbstractTerm::TS_INCLUDED :
        pixmap = QPixmap(NS_Icons::glIncludedIconString);
        break;
    case ZAbstractTerm::TS_EXCEPTED :
        pixmap = QPixmap(NS_Icons::glExcludedIconString);
        break;
    case ZAbstractTerm::TS_CONST_EXCLUDED :
        pixmap = QPixmap(NS_Icons::glConstExcludedIconString);
        break;

    case ZAbstractTerm::TS_NOT_DEFINED :
    default:
        pixmap = QPixmap();

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
void ZTermCorrelationTableManager::zh_onCalibrationValuesChanged(qint64 calibrationId)
{
    if(zv_currentCalibrationId != calibrationId)
    {
        return;
    }

    zh_calcResidualTermCorrelation();
    int factorCount = 0;
    ZAbstractTerm::TermState termState;
    for(int t = 0; t < zv_calibrationRepository->zp_termCount(zv_currentCalibrationId); t++)
    {
        termState = zv_calibrationRepository->zp_termState(zv_currentCalibrationId, t);
        if(termState == ZAbstractTerm::TS_CONST_INCLUDED
                || termState == ZAbstractTerm::TS_INCLUDED)
        {
            factorCount ++;
        }
    }

    emit zg_calculateCalibrationQualityData(false, zv_currentCalibrationId, factorCount + 1, zv_sumSquareAverageConcentrationDispersion);
}
//=============================================================================
void ZTermCorrelationTableManager::zh_currentSpectrumArrayChanged(qint64 currentArrayId, int currentArrayIndex)
{
    if((zv_currentArrayIndex == currentArrayIndex && zv_currentArrayId == currentArrayId) || !zv_calibrationRepository)
    {
        return;
    }


    // emit zg_currentOperation(TOT_BEGIN_RESET, -1, -1);

    zv_currentArrayIndex = currentArrayIndex;
    zv_currentArrayId = currentArrayId;
    zh_startCalculationCorrelationsAndCovariations();
    emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);

    // equation quality recalc
    int factorCount = 0;
    ZAbstractTerm::TermState termState;
    for(int t = 0; t < zv_calibrationRepository->zp_termCount(zv_currentCalibrationId); t++)
    {
        termState = zv_calibrationRepository->zp_termState(zv_currentCalibrationId, t);
        if(termState == ZAbstractTerm::TS_CONST_INCLUDED
                || termState == ZAbstractTerm::TS_INCLUDED)
        {
            factorCount ++;
        }
    }

    emit zg_calculateCalibrationQualityData(false, zv_currentCalibrationId, factorCount + 1, zv_sumSquareAverageConcentrationDispersion);
}
//==================================================================
void ZTermCorrelationTableManager::zh_onRepositoryChemElementOperation(ZSpectrumArrayRepository::ChemElementOperationType type,
                                                                       int arrayIndex, int first, int last)
{
    if(zv_currentArrayIndex != arrayIndex || zv_currentCalibrationId < 0
            || zv_spectrumArrayRepository == nullptr || zv_calibrationRepository == nullptr)
    {
        return;
    }

    // current calibration chem
    QString currentCalibrationChemElement = zv_calibrationRepository->zp_chemElementForCalibrationId(zv_currentCalibrationId);
    bool chemElementAccordance = false;
    for(int c = first; c <= last; c++)
    {
        if(zv_spectrumArrayRepository->zp_chemElementName(zv_currentArrayIndex, c) == currentCalibrationChemElement)
        {
            chemElementAccordance = true;
            break;
        }
    }

    if(!chemElementAccordance)
    {
        return;
    }

    // operation was for calibration chem element

    if(type == ZSpectrumArrayRepository::CEOT_INSERT_CHEM_ELEMENT)
    {

    }
    else if(type == ZSpectrumArrayRepository::CEOT_END_INSERT_CHEM_ELEMENT)
    {
        zh_startCalculationCorrelationsAndCovariations();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_REMOVE_CHEM_ELEMENT)
    {
        zv_concentrationCorrelationList.clear();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_END_REMOVE_CHEM_ELEMENT)
    {
        //        zh_calcChemElementCorrelations();
        //        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_CHEM_ELEMENT_CHANGED)
    {
        zh_startCalculationCorrelationsAndCovariations();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
        //        emit zg_currentOperation(OT_COLUMN_HEADER_CHANGED, visibleFirst, visibleLast);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_CHEM_ELEMENT_VISIBILITY_CHANGE)
    {
        //        bool visible = zv_spectrumArrayRepository->zp_chemElementIsVisible(zv_currentArrayIndex, first);
        //        if(!visible)
        //        {
        //            emit zg_currentOperation(OT_BEGIN_INSERT_COLUMN, visibleFirst, visibleLast);
        //        }
        //        else
        //        {
        //            emit zg_currentOperation(OT_BEGIN_REMOVE_COLUMN, visibleFirst, visibleLast);
        //        }
    }
    else if(type == ZSpectrumArrayRepository::CEOT_END_CHEM_ELEMENT_VISIBILITY_CHANGE)
    {
        //        bool visible = zv_spectrumArrayRepository->zp_chemElementIsVisible(zv_currentArrayIndex, first);
        //        if(visible)
        //        {
        //            emit zg_currentOperation(OT_END_INSERT_COLUMN, visibleFirst, visibleLast);
        //        }
        //        else
        //        {
        //            emit zg_currentOperation(OT_END_REMOVE_COLUMN, visibleFirst, visibleLast);
        //        }
    }
}
//=============================================================================
void ZTermCorrelationTableManager::zh_currentCalibrationChanged(qint64 calibrationId, int calibrationIndex)
{
    if(zv_currentCalibrationId == calibrationId && zv_currentCalibrationIndex == calibrationIndex)
    {
        return;
    }

    emit zg_currentOperation(TOT_BEGIN_RESET, -1, -1);

    zv_currentCalibrationId = calibrationId;
    zv_currentCalibrationIndex = calibrationIndex;
    zh_startCalculationCorrelationsAndCovariations();
    emit zg_currentOperation(TOT_END_RESET, -1, -1);
}
//=============================================================================
void ZTermCorrelationTableManager::zh_onRepositoryTermOperation(ZCalibrationRepository::TermOperationType type,
                                                                int calibrationIndex, int first, int last)
{
    if(zv_calibrationRepository == nullptr)
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
    }
    else if(type == ZCalibrationRepository::TOT_END_INSERT_TERM)
    {
        zv_columnCountCorrector = (last - first + 1) * -1;
        emit zg_currentOperation(TOT_END_INSERT_ROW, first, last);
        emit zg_currentOperation(TOT_BEGIN_INSERT_COLUMN, first + zv_firstNonTermColumnCount, last + zv_firstNonTermColumnCount);
        zv_columnCountCorrector = 0;
        emit zg_currentOperation(TOT_END_INSERT_COLUMN, first + zv_firstNonTermColumnCount, last + zv_firstNonTermColumnCount);

        zh_startCalculationCorrelationsAndCovariations();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
    }
    else if(type == ZCalibrationRepository::TOT_BEGIN_REMOVE_TERM)
    {
        emit zg_currentOperation(TOT_BEGIN_REMOVE_ROW, first, last);
    }
    else if(type == ZCalibrationRepository::TOT_END_REMOVE_TERM)
    {
        zv_columnCountCorrector = last - first + 1;
        emit zg_currentOperation(TOT_END_REMOVE_ROW, first, last);
        emit zg_currentOperation(TOT_BEGIN_REMOVE_COLUMN, first + zv_firstNonTermColumnCount, last + zv_firstNonTermColumnCount);
        zv_columnCountCorrector = 0;
        emit zg_currentOperation(TOT_END_REMOVE_COLUMN, first + zv_firstNonTermColumnCount, last + zv_firstNonTermColumnCount);
        zh_startCalculationCorrelationsAndCovariations();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
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
    else if(type == ZCalibrationRepository::TOT_TERM_WINDOW_MARGIN_CHANGED)
    {
        zh_startCalculationCorrelationsAndCovariations();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
    }
    else if(type == ZCalibrationRepository::TOT_TERM_FACTOR_CHANGED)
    {
        emit zg_currentOperation(TOT_DATA_CHANGED, first, last);
    }
}
//=============================================================================
void ZTermCorrelationTableManager::zh_onCalibrationRepositoryOperation(ZCalibrationRepository::CalibrationOperationType type,
                                                                       int first, int last)
{
    if(zv_calibrationRepository == nullptr || zv_currentCalibrationId < 0)
    {
        return;
    }

    for(int i = first; i <= last; i++)
    {
        if(zv_calibrationRepository->zp_calibrationIdForCalibrationIndex(i) == zv_currentCalibrationId)
        {
            if(type == ZCalibrationRepository::COT_BEGIN_RESET)
            {
                zv_concentrationCorrelationList.clear();
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
            }
            else if(type == ZCalibrationRepository::COT_END_RESET)
            {
                zh_startCalculationCorrelationsAndCovariations();
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
            }
            else if(type == ZCalibrationRepository::COT_INSERT_CALIBRATIONS)
            {
                zv_concentrationCorrelationList.clear();
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
            }
            else if(type == ZCalibrationRepository::COT_END_INSERT_CALIBRATIONS)
            {
                zh_startCalculationCorrelationsAndCovariations();
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
            }
            else if(type == ZCalibrationRepository::COT_REMOVE_CALIBRATIONS)
            {
                zv_concentrationCorrelationList.clear();
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
            }
            else if(type == ZCalibrationRepository::COT_END_REMOVE_CALIBRATIONS)
            {
                zh_startCalculationCorrelationsAndCovariations();
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
            }
            else if(type == ZCalibrationRepository::COT_CALIBRATION_NAME_CHANGED)
            {
                zh_startCalculationCorrelationsAndCovariations();
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
            }
            else if(type == ZCalibrationRepository::COT_CALIBRATION_CHEM_ELEMENT_CHANGED)
            {
                zh_startCalculationCorrelationsAndCovariations();
                emit zg_currentOperation(TOT_HORIZONTAL_HEADER_CHANGED, 0, 0);
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
            }
            else if(type == ZCalibrationRepository::COT_CALIBRATION_EQUATION_TYPE_CHANGED)
            {
                zh_startCalculationCorrelationsAndCovariations();
            }
            else if(type == ZCalibrationRepository::COT_CALIBRATION_EQUATION_BASE_TERM_CHANGED)
            {
                zh_startCalculationCorrelationsAndCovariations();
            }
        }
    }
}
//=============================================================================
void ZTermCorrelationTableManager::zh_onSpectrumOperation(ZSpectrumArrayRepository::SpectrumOperationType type,
                                                          int arrayIndex, int first, int last)
{
    if(!zv_spectrumArrayRepository || zv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    if(type == ZSpectrumArrayRepository::SOT_BEGIN_INSERT_SPECTRA)
    {

    }
    else if(type == ZSpectrumArrayRepository::SOT_END_INSERT_SPECTRA
            || type == ZSpectrumArrayRepository::SOT_END_REMOVE_SPECTRA
            || type == ZSpectrumArrayRepository::SOT_CHECKED_CHANGED
            || type == ZSpectrumArrayRepository::SOT_CONCENTRATION_CHANGED)
    {
        zh_startCalculationCorrelationsAndCovariations();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() -1);

        // equation quality recalc
        int factorCount = 0;
        ZAbstractTerm::TermState termState;
        for(int t = 0; t < zv_calibrationRepository->zp_termCount(zv_currentCalibrationId); t++)
        {
            termState = zv_calibrationRepository->zp_termState(zv_currentCalibrationId, t);
            if(termState == ZAbstractTerm::TS_CONST_INCLUDED
                    || termState == ZAbstractTerm::TS_INCLUDED)
            {
                factorCount ++;
            }
        }

        emit zg_calculateCalibrationQualityData(false, zv_currentCalibrationId, factorCount + 1, zv_sumSquareAverageConcentrationDispersion);
    }
    else if(type == ZSpectrumArrayRepository::SOT_BEGIN_REMOVE_SPECTRA)
    {

    }
    //    else if(type == ZSpectrumArrayRepository::SOT_END_REMOVE_SPECTRA)
    //    {
    //        zh_startCalculationCorrelationsAndCovariations();
    //        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() -1);
    //        // equation quality recalc
    //        int factorCount = 0;
    //        ZAbstractTerm::TermState termState;
    //        for(int t = 0; t < zv_calibrationRepository->zp_termCount(zv_currentCalibrationId); t++)
    //        {
    //            termState = zv_calibrationRepository->zp_termState(zv_currentCalibrationId, t);
    //            if(termState == ZAbstractTerm::TS_CONST_INCLUDED
    //                    || termState == ZAbstractTerm::TS_INCLUDED)
    //            {
    //                factorCount ++;
    //            }
    //        }

    //        emit zg_calculateCalibrationQualityData(false, zv_currentCalibrationId, factorCount + 1, zv_sumSquareAverageConcentrationDispersion);

    //    }
    //    else if(type == ZSpectrumArrayRepository::SOT_CHECKED_CHANGED)
    //    {
    //        zh_startCalculationCorrelationsAndCovariations();
    //        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() -1);
    //        // equation quality recalc
    //        int factorCount = 0;
    //        ZAbstractTerm::TermState termState;
    //        for(int t = 0; t < zv_calibrationRepository->zp_termCount(zv_currentCalibrationId); t++)
    //        {
    //            termState = zv_calibrationRepository->zp_termState(zv_currentCalibrationId, t);
    //            if(termState == ZAbstractTerm::TS_CONST_INCLUDED
    //                    || termState == ZAbstractTerm::TS_INCLUDED)
    //            {
    //                factorCount ++;
    //            }
    //        }

    //        emit zg_calculateCalibrationQualityData(false, zv_currentCalibrationId, factorCount + 1, zv_sumSquareAverageConcentrationDispersion);

    //    }


    /*

    // emit zg_currentOperation(TOT_BEGIN_RESET, -1, -1);

    zv_currentArrayIndex = currentArrayIndex;
    zv_currentArrayId = currentArrayId;
    zh_startCalculationCorrelationsAndCovariations();
    emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);

    // equation quality recalc
    int factorCount = 0;
    ZAbstractTerm::TermState termState;
    for(int t = 0; t < zv_calibrationRepository->zp_termCount(zv_currentCalibrationId); t++)
    {
        termState = zv_calibrationRepository->zp_termState(zv_currentCalibrationId, t);
        if(termState == ZAbstractTerm::TS_CONST_INCLUDED
                || termState == ZAbstractTerm::TS_INCLUDED)
        {
            factorCount ++;
        }
    }

    emit zg_calculateCalibrationQualityData(false, zv_currentCalibrationId, factorCount + 1, zv_sumSquareAverageConcentrationDispersion);

    */


}
//=============================================================================
void ZTermCorrelationTableManager::zh_onCalibrationNormalizerChange(qint64 calibrationId)
{
    if(zv_currentCalibrationId != calibrationId)
    {
        return;
    }

    zh_startCalculationCorrelationsAndCovariations();
    emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() -1);
}
//=============================================================================
void ZTermCorrelationTableManager::zh_recalcCalibrationFactors()
{
    if(!zv_calibrationRepository || zv_currentCalibrationId < 0)
    {
        return;
    }

    //  check matrixes
    if(zv_freeTermCovariationList.count() != zp_rowCount())
    {
        // TODO Error report
        return;
    }

    // check average free term (if a != a  then a == nan)
    if(zv_averageEquationIntercept - zv_averageEquationIntercept != 0.0)
    {
        // TODO Error report
        return;
    }

    if(zv_termCovariationMatrix.count() != zp_rowCount())
    {
        // TODO Error report
        return;
    }
    // matrixes - ok

    // get calibration that will be recalced
    ZCalibration* calibration = zv_calibrationRepository->zh_calibrationForId(zv_currentCalibrationId);
    if(!calibration)
    {
        // TODO Error message
        return;
    }

    // before solvation reset all termFactors of the calibration
    calibration->zp_resetEquationTerms();

    // set to calibration exposition and energy calibration factors
    if(zv_spectrumArrayRepository != nullptr)
    {
//        qreal K0 = 0.0;
//        qreal K1 = 0.0;
//        qreal K2 = 0.0;
//        QString energyUnit;
//        if(zv_spectrumArrayRepository->zp_energyCalibration(zv_currentArrayIndex,
//                                                            K0, K1, K2, energyUnit))
//        {
//            calibration->zp_setEnergyCalibration(K0, K1, K2, energyUnit);
//        }

        int exposition = zv_spectrumArrayRepository->zp_exposition(zv_currentArrayIndex);
        calibration->zp_setExposition(exposition);

        int gainFactor = zv_spectrumArrayRepository->zp_gainFactor(zv_currentArrayIndex);
        calibration->zp_setGainFactor(gainFactor);
    }

    // pointers to factors and free term
    QMap<int, qreal*> factorToIndexMap;
    qreal* freeTerm;
    calibration->zp_createEquationDataForEquationRecalc(factorToIndexMap, freeTerm);

    if(factorToIndexMap.isEmpty())
    {
        if(calibration->zp_equationType() != ZCalibration::ET_FRACTIONAL)
        {
            // TODO Error msg
            return;
        }
        // calc only free term freeTerm = averageNumerator / averageConcentration
        // chem element id
        QString chemElement = calibration->zp_chemElement();
        qint64 chemElementId = zv_spectrumArrayRepository->zp_chemElementIdForName(zv_currentArrayIndex, chemElement);
        if(chemElementId < 0)
        {
            return;
        }

        qreal averageConcentration = 0.0;
        qreal averageBaseValue = 0.0;
        int checkedSpectrumCount = 0;
        qreal value;
        const ZAbstractSpectrum* spectrum;
        for(int s = 0; s < zv_spectrumArrayRepository->zp_spectrumCount(zv_currentArrayIndex); s++)
        {
            spectrum = zv_spectrumArrayRepository->zp_spectrum(zv_currentArrayIndex, s);
            if(!spectrum )
            {
                continue;
            }

            if(!spectrum->zp_isSpectrumChecked())
            {
                continue;
            }

            checkedSpectrumCount++;
            // averageConcentration
            averageConcentration += spectrum->zp_concentrationValue(chemElementId);

            // averageNumerator
            if(!calibration->zp_calcBaseTermValue(spectrum, value))
            {
                // TODO Error msg
                return;
            }
            averageBaseValue += value;
        }

        if(checkedSpectrumCount == 0 || averageConcentration == 0.0)
        {

            return;
        }
        averageConcentration /= checkedSpectrumCount;
        averageBaseValue /= checkedSpectrumCount;

        *freeTerm = averageBaseValue / averageConcentration;
        // force signal emit
        zv_calibrationRepository->zh_notifyCalibrationRecalc(zv_currentCalibrationId);
        emit zg_calculateCalibrationQualityData(true, zv_currentCalibrationId, 1, zv_sumSquareAverageConcentrationDispersion);
        return;
    }

    ZLESGaussSolver solver;
    connect(&solver, &ZLESGaussSolver::zg_message,
            this, &ZTermCorrelationTableManager::zg_message);

    // create cov column for each factor
    QList<int> factorIndexList = factorToIndexMap.keys();
    int factorCount = factorIndexList.count();

    // column list init
    QList<qreal> columnList;
    QList<qreal> freeTermList;
    int row;
    int col;

    // passage through rows of covariance matrix
    for(int rowi = 0; rowi < factorCount; rowi++)
    {
        columnList.clear();
        // passage through columns of covariance matrix
        for(int coli = 0; coli < factorCount; coli++)
        {
            row = factorIndexList.at(rowi);
            col = factorIndexList.at(coli);
            if(!zh_convertColRowForCovariationMatrix(row, col))
            {
                // TODO error report
                return;
            }

            columnList.append(zv_termCovariationMatrix.at(row).at(col));
        }
        // append column to solver
        solver.zp_appendTermColumn(factorToIndexMap.value(factorIndexList.at(rowi)), columnList);
        // free term list formation
        freeTermList.append(zv_freeTermCovariationList.at(factorIndexList.at(rowi)));
    }

    // append free term column to solver
    solver.zp_appendFreeTermList(freeTermList);


    if(!solver.zp_solve())
    {
        // TODO Error report ?? (solver has already reported ?)
        return;
    }

    // Equation free term calculation
    // first initialization of free term
    *freeTerm = zv_averageEquationIntercept;
    QMap<int, qreal*>::const_iterator it;
    qreal averageTerm;
    for(it = factorToIndexMap.begin(); it != factorToIndexMap.end(); it++)
    {
        averageTerm = zv_averageTermValueList.at(it.key());
        if(averageTerm != averageTerm)
        {
            // one of averageTermValues is nan value. Free term cannot be calculated
            // reset all factors
            calibration->zp_resetEquationTerms();

            // TODO error report
            break;

        }
        *freeTerm -= (*it.value()) * averageTerm;
    }

    // force signal emit
    zv_calibrationRepository->zh_notifyCalibrationRecalc(zv_currentCalibrationId);
    // factor count + 1 - free member

    emit zg_calculateCalibrationQualityData(true, zv_currentCalibrationId, factorCount + 1, zv_sumSquareAverageConcentrationDispersion);
    calibration->zp_setCurrentDateTime();
}
//=============================================================================
bool ZTermCorrelationTableManager::zh_convertColRowForInterCorrelationMatrix(int& row, int& col) const
{
    if(col < row)
    {
        qSwap(col, row);
    }

    col -= row;

    if(row >= zv_termIntercorrelationMatrix.count())
    {
        return false;
    }
    if(col >= zv_termIntercorrelationMatrix.at(row).count())
    {
        return false;
    }

    return true;
}
//=============================================================================
bool ZTermCorrelationTableManager::zh_convertColRowForCovariationMatrix(int& row, int& col) const
{
    if(col < row)
    {
        qSwap(col, row);
    }

    col -= row;

    if(row >= zv_termCovariationMatrix.count())
    {
        return false;
    }
    if(col >= zv_termCovariationMatrix.at(row).count())
    {
        return false;
    }

    return true;
}
//=============================================================================
void ZTermCorrelationTableManager::zh_startCalculationCorrelationsAndCovariations()
{
    if(!zv_spectrumArrayRepository || zv_currentArrayId < 0 || !zv_calibrationRepository)
    {
        // clear average values
        zv_averageEquationIntercept = 0.0;
        zv_averageTermValueList.clear();

        // clear dispersion data
        zv_termDispersionMatrix.clear();
        zv_concentrationDispersionList.clear();
        zv_sumSquareAverageConcentrationDispersion = 0.0;
        zv_interceptDispersionList.clear();

        // clear correlation and covariation data
        zv_termIntercorrelationMatrix.clear();
        zv_concentrationCorrelationList.clear();
        zv_termCovariationMatrix.clear();
        zv_freeTermCovariationList.clear();
        zv_residualCorrelationList.clear();
        return;
    }

    // dispersions
    zh_calcTermDispersions();
    zh_calcConcentrationAndFreeTermDispersions();

    if(zv_termDispersionMatrix.isEmpty())
    {
        return;
    }
    // correlations
    zh_calcIntercorrelationsAndCovariations();
    zh_calcResidualTermCorrelation();
    if(zv_concentrationDispersionList.isEmpty())
    {
        return;
    }
    zh_calcConcentrationCorrelationsAndCavariations();
}
//=============================================================================
void ZTermCorrelationTableManager::zh_calcTermDispersions()
{
    zv_termDispersionMatrix.clear();
    zv_averageTermValueList.clear();

    // local vars for circles
    qreal averageTerm;
    qreal termValue;
    const ZAbstractSpectrum* spectrum;

    // term circle
    for(int t = 0; t < zp_rowCount(); t++)
    {
        // add dispersion list for the current term
        zv_termDispersionMatrix.append(QList<qreal>());

        // vars init
        averageTerm = 0.0;
        termValue = 0.0;
        // spectrum circle
        for(int s = 0; s < zv_spectrumArrayRepository->zp_spectrumCount(zv_currentArrayIndex); s++)
        {
            // get spectrum
            spectrum = zv_spectrumArrayRepository->zp_spectrum(zv_currentArrayIndex, s);
            if(spectrum == nullptr || !spectrum->zp_isSpectrumChecked())
            {
                continue;
            }

            // get term value from spectrum
            if(!zv_calibrationRepository->zp_termVariablePart(zv_currentCalibrationId, t, spectrum, termValue))
            {
                termValue = std::numeric_limits<double>::quiet_NaN();
                // invalidTermRows << t;
            }

            // term value nan check
            if(termValue != termValue)
            {
                // nan detected
                // make dispersion list empty and average term nan
                averageTerm = std::numeric_limits<double>::quiet_NaN();
                zv_termDispersionMatrix.last().clear();
                // get out of spectrum circle
                break;
            }
            zv_termDispersionMatrix.last().append(termValue);
            averageTerm += termValue;
        }

        // average term nan check
        if(averageTerm != averageTerm)
        {
            // add average nan val to average list
            zv_averageTermValueList.append(averageTerm);
            // jump to the next term
            continue;
        }

        if(zv_termDispersionMatrix.last().isEmpty())
        {
            // jump to the next term
            continue;
        }

        // average calc
        int checkedSpectrumNumber = zv_termDispersionMatrix.last().count();
        averageTerm /= checkedSpectrumNumber;
        // add average  val to average list
        zv_averageTermValueList.append(averageTerm);

        // dispersion calc
        for(int i = 0; i < checkedSpectrumNumber; i++)
        {
            zv_termDispersionMatrix.last()[i] = zv_termDispersionMatrix.last().at(i) - averageTerm;
        }
    }
}
//=============================================================================
void ZTermCorrelationTableManager::zh_calcConcentrationAndFreeTermDispersions()
{
    zv_concentrationDispersionList.clear();
    zv_sumSquareAverageConcentrationDispersion = 0.0;
    zv_interceptDispersionList.clear();

    // current conc chem element id
    QString calibrationChemElementString = zv_calibrationRepository->zp_chemElementForCalibrationId(zv_currentCalibrationId);
    if(calibrationChemElementString.isEmpty())
    {
        return;
    }

    qint64 chemElementId = zv_spectrumArrayRepository->zp_chemElementIdForName(zv_currentArrayIndex, calibrationChemElementString);
    if(chemElementId < 0)
    {
        return;
    }

    // equation type
    ZCalibration::EquationType equationType;
    zv_calibrationRepository->zp_equationTypeForCalibrationId(zv_currentCalibrationId, equationType);

    // local vars for circles
    qreal averageConcentration = 0.0;
    qreal concentration = 0.0;
    const ZAbstractSpectrum* spectrum;

    qreal equationFreeTerm = 0.0;
    qreal baseTermValue = 0.0;
    qreal averageFractionalFreeTerm = 0.0;
    bool fractionalDispersionError = false;

    // spectrum circle
    for(int s = 0; s < zv_spectrumArrayRepository->zp_spectrumCount(zv_currentArrayIndex); s++)
    {
        // get spectrum
        spectrum = zv_spectrumArrayRepository->zp_spectrum(zv_currentArrayIndex, s);
        if(spectrum == nullptr || !spectrum->zp_isSpectrumChecked())
        {
            continue;
        }

        // concentration for chem element
        concentration = spectrum->zp_concentrationValue(chemElementId);
        averageConcentration += concentration;
        zv_concentrationDispersionList.append(concentration);

        if(equationType == ZCalibration::ET_FRACTIONAL)
        {
            // fractional dispersions
            if(concentration == 0.0)
            {
                fractionalDispersionError = true;
                zv_interceptDispersionList.clear();
            }

            if(!fractionalDispersionError)
            {
                if(!zv_calibrationRepository->zp_calcBaseTermValue(zv_currentCalibrationId, spectrum, baseTermValue))
                {
                    fractionalDispersionError = true;
                    zv_interceptDispersionList.clear();
                    continue;
                }

                equationFreeTerm = baseTermValue / concentration;
                averageFractionalFreeTerm += equationFreeTerm;
                zv_interceptDispersionList.append(equationFreeTerm);
            }
        } // end equationType == ZCalibration::ET_FRACTIONAL
    }

    // average concentration
    if(zv_concentrationDispersionList.isEmpty())
    {
        // assign nan to average concentration
        zv_averageEquationIntercept = std::numeric_limits<double>::quiet_NaN();
        // jump to the next term
        return;
    }

    // average calc
    int checkedSpectrumNumber = zv_concentrationDispersionList.count();
    averageConcentration /= checkedSpectrumNumber;

    zv_sumSquareAverageConcentrationDispersion = 0.0;
    // dispersion calc
    for(int i = 0; i < checkedSpectrumNumber; i++)
    {
        zv_concentrationDispersionList[i] = zv_concentrationDispersionList.at(i) - averageConcentration;
        zv_sumSquareAverageConcentrationDispersion += pow(zv_concentrationDispersionList.at(i), 2.0);
    }

    if(equationType == ZCalibration::ET_FRACTIONAL)
    {
        averageFractionalFreeTerm /= checkedSpectrumNumber;
        if(!fractionalDispersionError)
        {
            for(int i = 0; i < checkedSpectrumNumber; i++)
            {
                zv_interceptDispersionList[i] = zv_interceptDispersionList.at(i) - averageFractionalFreeTerm;
            }
            zv_averageEquationIntercept = averageFractionalFreeTerm;
        }
        else
        {
            zv_averageEquationIntercept = std::numeric_limits<double>::quiet_NaN();
        }
    }
    else
    {
        zv_interceptDispersionList = zv_concentrationDispersionList;
        zv_averageEquationIntercept = averageConcentration;
    }
}
//=============================================================================
void ZTermCorrelationTableManager::zh_calcIntercorrelationsAndCovariations()
{
    zv_termIntercorrelationMatrix.clear();
    zv_termCovariationMatrix.clear();

    // local vars
    qreal numerator;
    qreal tDenominator;
    qreal iDenominator;
    qreal correlation;
    qreal tDispersion;
    qreal iDispersion;
    QString intercorrelationString;
    int termCount = zp_rowCount();

    // term circle
    for(int t = 0; t < termCount; t++)
    {
        // add correlation list for the current term
        zv_termIntercorrelationMatrix.append(QList<QString>());

        //        // append term itself correlation
        //        zv_termIntercorrelationMatrix.last().append("1.00");

        //
        zv_termCovariationMatrix.append(QList<qreal>());
        // check out dispersion t-list
        if(zv_termDispersionMatrix.at(t).isEmpty())
        {
            continue;
        }

        // intercorrelated term circle
        for(int i = t; i < termCount; i++)
        {
            // check out dispersion i-list
            int checkedSpeCount = zv_termDispersionMatrix.at(t).count();
            if(zv_termDispersionMatrix.at(i).count() != checkedSpeCount)
            {
                // TODO Error report
                continue;
            }

            // init numerator
            numerator = 0.0;
            tDenominator = 0.0;
            iDenominator = 0.0;

            // term dispersion circle
            for(int s = 0; s < checkedSpeCount; s++)
            {
                tDispersion = zv_termDispersionMatrix.at(t).at(s);
                iDispersion = zv_termDispersionMatrix.at(i).at(s);

                numerator += tDispersion * iDispersion;
                tDenominator += pow(tDispersion, 2);
                iDenominator += pow(iDispersion, 2);
            }

            // covariation
            zv_termCovariationMatrix.last().append(numerator);

            if(tDenominator == 0.0 || iDenominator == 0.0)
            {
                zv_termIntercorrelationMatrix.last().append("Error");
            }

            correlation = numerator / (sqrt(tDenominator) * sqrt(iDenominator));
            intercorrelationString = QString::number(correlation, 'f', 3);
            zv_termIntercorrelationMatrix.last().append(intercorrelationString);
        }
    }
}
//=============================================================================
void ZTermCorrelationTableManager::zh_calcConcentrationCorrelationsAndCavariations()
{
    // clear list
    zv_concentrationCorrelationList.clear();
    zv_freeTermCovariationList.clear();

    // local vars
    qreal numerator;
    qreal tDenominator;
    qreal cDenominator;
    qreal tDispersion;
    qreal cDispersion;
    qreal freeTermCovariation;
    qreal correlation;
    QString correlationString;
    int termCount = zp_rowCount();
    int checkedSpeCount = zv_concentrationDispersionList.count();
    bool freeDispersionOk = zv_interceptDispersionList.count() == checkedSpeCount;

    // term circle
    for(int t = 0; t < termCount; t++)
    {
        // check term dispersion list and conc dispersion
        if(zv_termDispersionMatrix.at(t).count() != checkedSpeCount)
        {
            zv_concentrationCorrelationList.append("Error");
            continue;
        }

        // init
        numerator = 0.0;
        tDenominator = 0.0;
        cDenominator = 0.0;
        freeTermCovariation = 0.0;

        // term dispersion circle
        for(int s = 0; s < checkedSpeCount; s++)
        {
            tDispersion =  zv_termDispersionMatrix.at(t).at(s);
            cDispersion = zv_concentrationDispersionList.at(s);
            if(freeDispersionOk)
            {
                freeTermCovariation += zv_interceptDispersionList.at(s) * tDispersion;
            }
            numerator += tDispersion * cDispersion;
            tDenominator += pow(tDispersion, 2);
            cDenominator += pow(cDispersion, 2);
        }

        // covariation
        if(freeDispersionOk)
        {
            zv_freeTermCovariationList.append(freeTermCovariation);
        }

        if(tDenominator == 0.0 || cDenominator == 0.0)
        {
            zv_concentrationCorrelationList.append("Error");
        }

        correlation = numerator / (sqrt(tDenominator) * sqrt(cDenominator));
        correlationString = QString::number(correlation, 'f', 3);
        zv_concentrationCorrelationList.append(correlationString);

    }
}
//=============================================================================
void ZTermCorrelationTableManager::zh_calcResidualTermCorrelation()
{
    zv_residualCorrelationList.clear();

    // term count
    int termCount = zp_rowCount();
    // matrix check
    if(zv_termDispersionMatrix.count() != termCount)
    {
        return;
    }
    // get residual list
    QList<qreal> residualDispersionList;
    if(!zv_jointSpectraDataManager->zp_calculateConcentrationResidualList(zv_currentCalibrationId,
                                                                          residualDispersionList))
    {
        return;
    }

    qreal numerator;
    qreal tDenominator;
    qreal rDenominator;
    qreal tDispersion;
    qreal rDispersion;
    qreal correlation;
    QString correlationString;

    int checkedSpeCount = residualDispersionList.count();

    // term circle
    for(int t = 0; t < termCount; t++)
    {
        // check term dispersion list and conc dispersion

        if(zv_termDispersionMatrix.at(t).count() != checkedSpeCount)
        {
            zv_residualCorrelationList.append("Error");
            continue;
        }

        numerator = 0.0;
        tDenominator = 0.0;
        rDenominator = 0.0;

        // term dispersion circle
        for(int s = 0; s < checkedSpeCount; s++)
        {
            tDispersion =  zv_termDispersionMatrix.at(t).at(s);
            rDispersion = residualDispersionList.at(s);
            numerator += tDispersion * rDispersion;
            tDenominator += pow(tDispersion, 2);
            rDenominator += pow(rDispersion, 2);
        }

        if(tDenominator < 0.0 || rDenominator < 0.0)
        {
            zv_residualCorrelationList.append("Error");
        }

        correlation = numerator / (sqrt(tDenominator) * sqrt(rDenominator));
        correlationString = QString::number(correlation, 'f', 3);
        zv_residualCorrelationList.append(correlationString);
    }
}
//=============================================================================
