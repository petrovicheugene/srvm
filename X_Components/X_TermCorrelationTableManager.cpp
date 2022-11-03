//=============================================================================
#include "X_TermCorrelationTableManager.h"
#include "X_LESGaussSolver.h"
#include "X_General.h"
#include "X_JointSpectraDataManager.h"

#include <math.h>
#include <QDebug>
#include <QPair>
#include <QTextEdit>
//=============================================================================
X_TermCorrelationTableManager::X_TermCorrelationTableManager(QObject *parent) : QObject(parent)
{

    xv_calibrationRepository = nullptr;
    xv_spectrumArrayRepository = nullptr;
    xv_jointSpectraDataManager = nullptr;
    xv_currentCalibrationId = -1;
    xv_currentCalibrationIndex = -1;
    xv_columnCountCorrector = 0;
    xv_currentArrayId = -1;
    xv_currentArrayIndex = -1;
    xv_averageEquationIntercept = std::numeric_limits<double>::quiet_NaN();

    xv_greenCell = QColor(Qt::green);
    xv_yellowCell = QColor(Qt::yellow);
    xv_blueCell = QColor(Qt::blue);
    xv_cyanCell = QColor(Qt::cyan);
    xv_magentaCell = QColor(Qt::cyan);
    xv_redCell = QColor(Qt::red);

    xv_greenCell.setAlpha(30);
    xv_yellowCell.setAlpha(30);
    xv_blueCell.setAlpha(30);
    xv_cyanCell.setAlpha(30);
    xv_magentaCell.setAlpha(30);
    xv_redCell.setAlpha(30);

}
//=============================================================================
void X_TermCorrelationTableManager::xp_connectToCalibrationRepository(X_CalibrationRepository* repository)
{
    emit xg_currentOperation(TOT_BEGIN_RESET, -1, -1);
    xv_calibrationRepository = repository;
    connect(xv_calibrationRepository, &X_CalibrationRepository::xg_termOperation,
            this, &X_TermCorrelationTableManager::xh_onRepositoryTermOperation);
    //    connect(repository, &X_CalibrationRepository::xg_calibrationWindowOperation,
    //            this, &X_TermCorrelationTableManager::xh_onCalibrationWindowOperation);

    connect(xv_calibrationRepository, &X_CalibrationRepository::xg_currentCalibrationChanged,
            this, &X_TermCorrelationTableManager::xh_currentCalibrationChanged);
    connect(xv_calibrationRepository, &X_CalibrationRepository::xg_calibrationOperation,
            this, &X_TermCorrelationTableManager::xh_onCalibrationRepositoryOperation);
    connect(xv_calibrationRepository, &X_CalibrationRepository::xg_normalizerChanged,
            this, &X_TermCorrelationTableManager::xh_onCalibrationNormalizerChange);

    connect(xv_calibrationRepository, &X_CalibrationRepository::xg_invokeCalibrationRecalc,
            this, &X_TermCorrelationTableManager::xh_recalcCalibrationFactors);

    connect(this, &X_TermCorrelationTableManager::xg_currentTermChanged,
            xv_calibrationRepository, &X_CalibrationRepository::xp_onCurrentTermChange);
    connect(this, &X_TermCorrelationTableManager::xg_termDoubleClicked,
            xv_calibrationRepository, &X_CalibrationRepository::xp_onTermDoubleClocked);


    emit xg_currentOperation(TOT_END_RESET, -1, -1);

}
//=============================================================================
void X_TermCorrelationTableManager::xp_connectToSpectrumArrayRepository(X_SpectrumArrayRepository* repository)
{
    xv_spectrumArrayRepository = repository;
    // array repository <-> array model
    //    connect(repository, &X_SpectrumArrayRepository::xg_spectrumOperation,
    //            this, &X_JointSpectraDataManager::xh_onRepositoryArrayOperation);
    connect(repository, &X_SpectrumArrayRepository::xg_chemElementOperation,
            this, &X_TermCorrelationTableManager::xh_onRepositoryChemElementOperation);
    connect(repository, &X_SpectrumArrayRepository::xg_currentArrayIdChanged,
            this, &X_TermCorrelationTableManager::xh_currentSpectrumArrayChanged);
    connect(repository, &X_SpectrumArrayRepository::xg_spectrumOperation,
            this, &X_TermCorrelationTableManager::xh_onSpectrumOperation);

}
//=============================================================================
void X_TermCorrelationTableManager::xp_connectToJointSpectraDataManager(X_JointSpectraDataManager* jointSpectraDataManager)
{
    xv_jointSpectraDataManager = jointSpectraDataManager;
    connect(this, &X_TermCorrelationTableManager::xg_calculateCalibrationQualityData,
            jointSpectraDataManager, &X_JointSpectraDataManager::xp_calculateCalibrationQualityData);
    connect(jointSpectraDataManager, &X_JointSpectraDataManager::xg_calibrationValuesChanged,
            this, &X_TermCorrelationTableManager::xh_onCalibrationValuesChanged);

}
//=============================================================================
int X_TermCorrelationTableManager::xp_rowCount() const
{
    if(xv_currentCalibrationId < 0 || xv_calibrationRepository == nullptr)
    {
        return 0;
    }

    return xv_calibrationRepository->xp_termCount(xv_currentCalibrationId);
}
//=============================================================================
int X_TermCorrelationTableManager::xp_columnCount() const
{
    if(xv_calibrationRepository == nullptr)
    {
        return 0;
    }

    return xv_calibrationRepository->xp_termCount(xv_currentCalibrationId) + xv_firstNonTermColumnCount + xv_columnCountCorrector; // 2 - term column and chem conc column
}
//=============================================================================
bool X_TermCorrelationTableManager::xp_isRowEditable(int row) const
{
    if(!xv_calibrationRepository || row < 0 || row >= xp_rowCount())
    {
        return false;
    }

    return xv_calibrationRepository->xp_equationType(xv_currentCalibrationIndex) != X_Calibration::ET_FRACTIONAL
            || xv_calibrationRepository->xp_baseTermIndex(xv_currentCalibrationIndex) != row;

}
//=============================================================================
QVariant X_TermCorrelationTableManager::xp_data(QModelIndex index) const
{
    if(!xv_calibrationRepository || !index.isValid() || index.row() < 0 || index.column() < 0
            || index.row() >= xp_rowCount() || index.column() >= xp_columnCount())
    {
        return QVariant();
    }

    if(index.column() == 0)
    {
        QString factorString;
        if(xv_calibrationRepository->xp_equationType(xv_currentCalibrationIndex) == X_Calibration::ET_FRACTIONAL
                && xv_calibrationRepository->xp_baseTermIndex(xv_currentCalibrationIndex) == index.row())
        {
            return tr("Base");
        }

        bool ok = xv_calibrationRepository->xp_termFactorString(xv_currentCalibrationId, index.row(), factorString);
        if(!ok)
        {
            return tr("#Error");
        }

        return QVariant(factorString);
    }
    else if(index.column() == 1)
    {
        if(index.row() >= xv_concentrationCorrelationList.count())
        {
            return QVariant();
        }

        return QVariant(xv_concentrationCorrelationList.at(index.row()));
    }
    else if(index.column() == 2)
    {
        if(index.row() >= xv_residualCorrelationList.count())
        {
            return QVariant();
        }

        return QVariant(xv_residualCorrelationList.at(index.row()));
    }
    else if(index.column() >= xv_firstNonTermColumnCount)
    {
        // col index conversion
        int col = index.column() - xv_firstNonTermColumnCount;
        int row = index.row();

        if(col == row)
        {
            return QVariant();
        }

        if(!xh_convertColRowForInterCorrelationMatrix(row, col))
        {
            return QVariant(tr("Error"));
        }

        return QVariant(xv_termIntercorrelationMatrix.at(row).at(col));// value(QPair<int, int>(termColumn, termRow)));
    }

    return QVariant();
}

//=============================================================================
bool X_TermCorrelationTableManager::xp_setData(QModelIndex index, QVariant vFactor)
{
    if(!xv_calibrationRepository || !index.isValid() || index.row() < 0 || index.row() >= xp_rowCount()
            || index.column() != 0
            || !vFactor.isValid() || vFactor.isNull() || !vFactor.canConvert<QString>())
    {
        return false;
    }

    if(xv_calibrationRepository->xp_baseTermIndexForCalibrationId(xv_currentCalibrationId) == index.row())
    {
        return false;
    }

    QString factorString = vFactor.toString();
    return xv_calibrationRepository->xp_setTermFactorString(xv_currentCalibrationId, index.row(), factorString);
}
//=============================================================================
QVariant X_TermCorrelationTableManager::xp_cellColor(QModelIndex index) const
{
    if(!xv_calibrationRepository || !index.isValid() || index.row() < 0 || index.column() < 0
            || index.row() >= xp_rowCount() || index.column() >= xp_columnCount())
    {
        return QVariant();
    }

    qreal correlationValue = 0;
    bool ok;

    // chem Correlations
    if(index.column() < xv_firstNonTermColumnCount)
    {
        // correlations in column
        if(index.column() == 1)
        {
            correlationValue = qAbs(xv_concentrationCorrelationList.value(index.row(), QString()).toDouble(&ok));
        }
        else if(index.column() == 2)
        {
            correlationValue = qAbs(xv_residualCorrelationList.value(index.row(), QString()).toDouble(&ok));
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
            return QVariant(xv_greenCell);
        }
        else if(correlationValue < 0.7 && correlationValue >= 0.5)
        {
            return QVariant(xv_cyanCell);
        }
        else if(correlationValue < 0.5 && correlationValue >= 0.3)
        {
            return QVariant(xv_magentaCell);
        }
    }
    else //if(index.column() >=  xv_firstNonTermColumnCount)
    {
        QVariant vData = xp_data(index);

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
            return QVariant(xv_redCell);
        }
        else if(correlationValue < 0.7 && correlationValue >= 0.5)
        {
            return QVariant(xv_yellowCell);
        }
    }
    return QVariant();
}
//=============================================================================
QString X_TermCorrelationTableManager::xp_horizontalColumnName(int column) const
{
    if(column < 0 || column >= xv_calibrationRepository->xp_termCount(xv_currentCalibrationId) + xv_firstNonTermColumnCount)
    {
        return QString();
    }

    if(column == 0)
    {
        return tr("Factor");
    }
    else if(column == 1)
    {
        if(xv_currentCalibrationId < 0 || xv_calibrationRepository == nullptr)
        {
            return xv_defaultChemElementString;
        }

        QString chemElementString = xv_calibrationRepository->xp_chemElementForCalibrationId(xv_currentCalibrationId);
        if(chemElementString.isEmpty() || chemElementString == glDefaultChemElementString)
        {
            return xv_defaultChemElementString;
        }

        return chemElementString;
    }
    else if(column == 2)
    {
        return tr("Residual");
    }
    else
    {
        if(xv_currentCalibrationId < 0 || xv_calibrationRepository == nullptr)
        {
            return QString();
        }
        return xv_calibrationRepository->xp_termName(xv_currentCalibrationId, column - xv_firstNonTermColumnCount);
    }

}
//=============================================================================
QString X_TermCorrelationTableManager::xp_verticalColumnName(int row) const
{
    if(row < 0 || row >= xv_calibrationRepository->xp_termCount(xv_currentCalibrationId))
    {
        return QString();
    }

    if(xv_currentCalibrationId < 0 || xv_calibrationRepository == nullptr)
    {
        return QString();
    }

    return xv_calibrationRepository->xp_termName(xv_currentCalibrationId, row);
}
//=============================================================================
QPixmap X_TermCorrelationTableManager::xp_termStateIcon(int row) const
{
    if(row < 0 || row >= xv_calibrationRepository->xp_termCount(xv_currentCalibrationId))
    {
        return QPixmap();
    }

    if(xv_currentCalibrationId < 0 || xv_calibrationRepository == nullptr)
    {
        return QPixmap();
    }

    X_AbstractTerm::TermState state = xv_calibrationRepository->xp_termState(xv_currentCalibrationId, row);
    QPixmap pixmap;
    switch(state)
    {
    case X_AbstractTerm::TS_BASE :
        pixmap = QPixmap(NS_Icons::glBaseTermIconString);
        break;
    case X_AbstractTerm::TS_CONST_INCLUDED :
        pixmap = QPixmap(NS_Icons::glConstIncludedIconString);
        break;
    case X_AbstractTerm::TS_EXAM_WAITING :
        pixmap = QPixmap(NS_Icons::glExamWaitingIconString);
        break;
    case X_AbstractTerm::TS_INCLUDED :
        pixmap = QPixmap(NS_Icons::glIncludedIconString);
        break;
    case X_AbstractTerm::TS_EXCEPTED :
        pixmap = QPixmap(NS_Icons::glExcludedIconString);
        break;
    case X_AbstractTerm::TS_CONST_EXCLUDED :
        pixmap = QPixmap(NS_Icons::glConstExcludedIconString);
        break;

    case X_AbstractTerm::TS_NOT_DEFINED :
    default:
        pixmap = QPixmap();

    }

    return pixmap;
}
//=============================================================================
void X_TermCorrelationTableManager::xp_setNextUsersTermState(int termLogIndex)
{
    if(xv_currentCalibrationId < 0 )
    {
        return;
    }

    xv_calibrationRepository->xp_setNextUsersTermState(xv_currentCalibrationId, termLogIndex);
}
//=============================================================================
void X_TermCorrelationTableManager::xh_onCalibrationValuesChanged(qint64 calibrationId)
{
    if(xv_currentCalibrationId != calibrationId)
    {
        return;
    }

    xh_calcResidualTermCorrelation();
    int factorCount = 0;
    X_AbstractTerm::TermState termState;
    for(int t = 0; t < xv_calibrationRepository->xp_termCount(xv_currentCalibrationId); t++)
    {
        termState = xv_calibrationRepository->xp_termState(xv_currentCalibrationId, t);
        if(termState == X_AbstractTerm::TS_CONST_INCLUDED
                || termState == X_AbstractTerm::TS_INCLUDED)
        {
            factorCount ++;
        }
    }

    emit xg_calculateCalibrationQualityData(false, xv_currentCalibrationId, factorCount + 1, xv_sumSquareAverageConcentrationDispersion);
}
//=============================================================================
void X_TermCorrelationTableManager::xh_currentSpectrumArrayChanged(qint64 currentArrayId, int currentArrayIndex)
{
    if((xv_currentArrayIndex == currentArrayIndex && xv_currentArrayId == currentArrayId) || !xv_calibrationRepository)
    {
        return;
    }


    // emit xg_currentOperation(TOT_BEGIN_RESET, -1, -1);

    xv_currentArrayIndex = currentArrayIndex;
    xv_currentArrayId = currentArrayId;
    xh_startCalculationCorrelationsAndCovariations();
    emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);

    // equation quality recalc
    int factorCount = 0;
    X_AbstractTerm::TermState termState;
    for(int t = 0; t < xv_calibrationRepository->xp_termCount(xv_currentCalibrationId); t++)
    {
        termState = xv_calibrationRepository->xp_termState(xv_currentCalibrationId, t);
        if(termState == X_AbstractTerm::TS_CONST_INCLUDED
                || termState == X_AbstractTerm::TS_INCLUDED)
        {
            factorCount ++;
        }
    }

    emit xg_calculateCalibrationQualityData(false, xv_currentCalibrationId, factorCount + 1, xv_sumSquareAverageConcentrationDispersion);
}
//==================================================================
void X_TermCorrelationTableManager::xh_onRepositoryChemElementOperation(X_SpectrumArrayRepository::ChemElementOperationType type,
                                                                       int arrayIndex, int first, int last)
{
    if(xv_currentArrayIndex != arrayIndex || xv_currentCalibrationId < 0
            || xv_spectrumArrayRepository == nullptr || xv_calibrationRepository == nullptr)
    {
        return;
    }

    // current calibration chem
    QString currentCalibrationChemElement = xv_calibrationRepository->xp_chemElementForCalibrationId(xv_currentCalibrationId);
    bool chemElementAccordance = false;
    for(int c = first; c <= last; c++)
    {
        if(xv_spectrumArrayRepository->xp_chemElementName(xv_currentArrayIndex, c) == currentCalibrationChemElement)
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

    if(type == X_SpectrumArrayRepository::CEOT_INSERT_CHEM_ELEMENT)
    {

    }
    else if(type == X_SpectrumArrayRepository::CEOT_END_INSERT_CHEM_ELEMENT)
    {
        xh_startCalculationCorrelationsAndCovariations();
        emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);
    }
    else if(type == X_SpectrumArrayRepository::CEOT_REMOVE_CHEM_ELEMENT)
    {
        xv_concentrationCorrelationList.clear();
        emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);
    }
    else if(type == X_SpectrumArrayRepository::CEOT_END_REMOVE_CHEM_ELEMENT)
    {
        //        xh_calcChemElementCorrelations();
        //        emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);
    }
    else if(type == X_SpectrumArrayRepository::CEOT_CHEM_ELEMENT_CHANGED)
    {
        xh_startCalculationCorrelationsAndCovariations();
        emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);
        //        emit xg_currentOperation(OT_COLUMN_HEADER_CHANGED, visibleFirst, visibleLast);
    }
    else if(type == X_SpectrumArrayRepository::CEOT_CHEM_ELEMENT_VISIBILITY_CHANGE)
    {
        //        bool visible = xv_spectrumArrayRepository->xp_chemElementIsVisible(xv_currentArrayIndex, first);
        //        if(!visible)
        //        {
        //            emit xg_currentOperation(OT_BEGIN_INSERT_COLUMN, visibleFirst, visibleLast);
        //        }
        //        else
        //        {
        //            emit xg_currentOperation(OT_BEGIN_REMOVE_COLUMN, visibleFirst, visibleLast);
        //        }
    }
    else if(type == X_SpectrumArrayRepository::CEOT_END_CHEM_ELEMENT_VISIBILITY_CHANGE)
    {
        //        bool visible = xv_spectrumArrayRepository->xp_chemElementIsVisible(xv_currentArrayIndex, first);
        //        if(visible)
        //        {
        //            emit xg_currentOperation(OT_END_INSERT_COLUMN, visibleFirst, visibleLast);
        //        }
        //        else
        //        {
        //            emit xg_currentOperation(OT_END_REMOVE_COLUMN, visibleFirst, visibleLast);
        //        }
    }
}
//=============================================================================
void X_TermCorrelationTableManager::xh_currentCalibrationChanged(qint64 calibrationId, int calibrationIndex)
{
    if(xv_currentCalibrationId == calibrationId && xv_currentCalibrationIndex == calibrationIndex)
    {
        return;
    }

    emit xg_currentOperation(TOT_BEGIN_RESET, -1, -1);

    xv_currentCalibrationId = calibrationId;
    xv_currentCalibrationIndex = calibrationIndex;
    xh_startCalculationCorrelationsAndCovariations();
    emit xg_currentOperation(TOT_END_RESET, -1, -1);
}
//=============================================================================
void X_TermCorrelationTableManager::xh_onRepositoryTermOperation(X_CalibrationRepository::TermOperationType type,
                                                                int calibrationIndex, int first, int last)
{
    if(xv_calibrationRepository == nullptr)
    {
        return;
    }

    qint64 calibrationId = xv_calibrationRepository->xp_calibrationIdForCalibrationIndex(calibrationIndex);
    if(xv_currentCalibrationId != calibrationId)
    {
        return;
    }

    if(type == X_CalibrationRepository::TOT_BEGIN_RESET)
    {
        emit xg_currentOperation(TOT_BEGIN_RESET, first, last);
    }
    else if(type == X_CalibrationRepository::TOT_END_RESET)
    {
        emit xg_currentOperation(TOT_END_RESET, first, last);
    }
    else if(type == X_CalibrationRepository::TOT_BEGIN_INSERT_TERM)
    {
        emit xg_currentOperation(TOT_BEGIN_INSERT_ROW, first, last);
    }
    else if(type == X_CalibrationRepository::TOT_END_INSERT_TERM)
    {
        xv_columnCountCorrector = (last - first + 1) * -1;
        emit xg_currentOperation(TOT_END_INSERT_ROW, first, last);
        emit xg_currentOperation(TOT_BEGIN_INSERT_COLUMN, first + xv_firstNonTermColumnCount, last + xv_firstNonTermColumnCount);
        xv_columnCountCorrector = 0;
        emit xg_currentOperation(TOT_END_INSERT_COLUMN, first + xv_firstNonTermColumnCount, last + xv_firstNonTermColumnCount);

        xh_startCalculationCorrelationsAndCovariations();
        emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);
    }
    else if(type == X_CalibrationRepository::TOT_BEGIN_REMOVE_TERM)
    {
        emit xg_currentOperation(TOT_BEGIN_REMOVE_ROW, first, last);
    }
    else if(type == X_CalibrationRepository::TOT_END_REMOVE_TERM)
    {
        xv_columnCountCorrector = last - first + 1;
        emit xg_currentOperation(TOT_END_REMOVE_ROW, first, last);
        emit xg_currentOperation(TOT_BEGIN_REMOVE_COLUMN, first + xv_firstNonTermColumnCount, last + xv_firstNonTermColumnCount);
        xv_columnCountCorrector = 0;
        emit xg_currentOperation(TOT_END_REMOVE_COLUMN, first + xv_firstNonTermColumnCount, last + xv_firstNonTermColumnCount);
        xh_startCalculationCorrelationsAndCovariations();
        emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);
    }
    else if(type == X_CalibrationRepository::TOT_TERM_NAME_CHANGED)
    {
        emit xg_currentOperation(TOT_VERTICAL_HEADER_CHANGED, first, last);
        emit xg_currentOperation(TOT_HORIX_ONTAL_HEADER_CHANGED, first + xv_firstNonTermColumnCount, last + xv_firstNonTermColumnCount);
    }
    else if(type == X_CalibrationRepository::TOT_TERM_STATE_CHANGED)
    {
        emit xg_currentOperation(TOT_VERTICAL_HEADER_CHANGED, first, last);
    }
    else if(type == X_CalibrationRepository::TOT_TERM_WINDOW_MARGIN_CHANGED)
    {
        xh_startCalculationCorrelationsAndCovariations();
        emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);
    }
    else if(type == X_CalibrationRepository::TOT_TERM_FACTOR_CHANGED)
    {
        emit xg_currentOperation(TOT_DATA_CHANGED, first, last);
    }
}
//=============================================================================
void X_TermCorrelationTableManager::xh_onCalibrationRepositoryOperation(X_CalibrationRepository::CalibrationOperationType type,
                                                                       int first, int last)
{
    if(xv_calibrationRepository == nullptr || xv_currentCalibrationId < 0)
    {
        return;
    }

    for(int i = first; i <= last; i++)
    {
        if(xv_calibrationRepository->xp_calibrationIdForCalibrationIndex(i) == xv_currentCalibrationId)
        {
            if(type == X_CalibrationRepository::COT_BEGIN_RESET)
            {
                xv_concentrationCorrelationList.clear();
                emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);
            }
            else if(type == X_CalibrationRepository::COT_END_RESET)
            {
                xh_startCalculationCorrelationsAndCovariations();
                emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);
            }
            else if(type == X_CalibrationRepository::COT_INSERT_CALIBRATIONS)
            {
                xv_concentrationCorrelationList.clear();
                emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);
            }
            else if(type == X_CalibrationRepository::COT_END_INSERT_CALIBRATIONS)
            {
                xh_startCalculationCorrelationsAndCovariations();
                emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);
            }
            else if(type == X_CalibrationRepository::COT_REMOVE_CALIBRATIONS)
            {
                xv_concentrationCorrelationList.clear();
                emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);
            }
            else if(type == X_CalibrationRepository::COT_END_REMOVE_CALIBRATIONS)
            {
                xh_startCalculationCorrelationsAndCovariations();
                emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);
            }
            else if(type == X_CalibrationRepository::COT_CALIBRATION_NAME_CHANGED)
            {
                xh_startCalculationCorrelationsAndCovariations();
                emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);
            }
            else if(type == X_CalibrationRepository::COT_CALIBRATION_CHEM_ELEMENT_CHANGED)
            {
                xh_startCalculationCorrelationsAndCovariations();
                emit xg_currentOperation(TOT_HORIX_ONTAL_HEADER_CHANGED, 0, 0);
                emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);
            }
            else if(type == X_CalibrationRepository::COT_CALIBRATION_EQUATION_TYPE_CHANGED)
            {
                xh_startCalculationCorrelationsAndCovariations();
            }
            else if(type == X_CalibrationRepository::COT_CALIBRATION_EQUATION_BASE_TERM_CHANGED)
            {
                xh_startCalculationCorrelationsAndCovariations();
            }
        }
    }
}
//=============================================================================
void X_TermCorrelationTableManager::xh_onSpectrumOperation(X_SpectrumArrayRepository::SpectrumOperationType type,
                                                          int arrayIndex, int first, int last)
{
    if(!xv_spectrumArrayRepository || xv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    if(type == X_SpectrumArrayRepository::SOT_BEGIN_INSERT_SPECTRA)
    {

    }
    else if(type == X_SpectrumArrayRepository::SOT_END_INSERT_SPECTRA
            || type == X_SpectrumArrayRepository::SOT_END_REMOVE_SPECTRA
            || type == X_SpectrumArrayRepository::SOT_CHECKED_CHANGED
            || type == X_SpectrumArrayRepository::SOT_CONCENTRATION_CHANGED)
    {
        xh_startCalculationCorrelationsAndCovariations();
        emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() -1);

        // equation quality recalc
        int factorCount = 0;
        X_AbstractTerm::TermState termState;
        for(int t = 0; t < xv_calibrationRepository->xp_termCount(xv_currentCalibrationId); t++)
        {
            termState = xv_calibrationRepository->xp_termState(xv_currentCalibrationId, t);
            if(termState == X_AbstractTerm::TS_CONST_INCLUDED
                    || termState == X_AbstractTerm::TS_INCLUDED)
            {
                factorCount ++;
            }
        }

        emit xg_calculateCalibrationQualityData(false, xv_currentCalibrationId, factorCount + 1, xv_sumSquareAverageConcentrationDispersion);
    }
    else if(type == X_SpectrumArrayRepository::SOT_BEGIN_REMOVE_SPECTRA)
    {

    }
    //    else if(type == X_SpectrumArrayRepository::SOT_END_REMOVE_SPECTRA)
    //    {
    //        xh_startCalculationCorrelationsAndCovariations();
    //        emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() -1);
    //        // equation quality recalc
    //        int factorCount = 0;
    //        X_AbstractTerm::TermState termState;
    //        for(int t = 0; t < xv_calibrationRepository->xp_termCount(xv_currentCalibrationId); t++)
    //        {
    //            termState = xv_calibrationRepository->xp_termState(xv_currentCalibrationId, t);
    //            if(termState == X_AbstractTerm::TS_CONST_INCLUDED
    //                    || termState == X_AbstractTerm::TS_INCLUDED)
    //            {
    //                factorCount ++;
    //            }
    //        }

    //        emit xg_calculateCalibrationQualityData(false, xv_currentCalibrationId, factorCount + 1, xv_sumSquareAverageConcentrationDispersion);

    //    }
    //    else if(type == X_SpectrumArrayRepository::SOT_CHECKED_CHANGED)
    //    {
    //        xh_startCalculationCorrelationsAndCovariations();
    //        emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() -1);
    //        // equation quality recalc
    //        int factorCount = 0;
    //        X_AbstractTerm::TermState termState;
    //        for(int t = 0; t < xv_calibrationRepository->xp_termCount(xv_currentCalibrationId); t++)
    //        {
    //            termState = xv_calibrationRepository->xp_termState(xv_currentCalibrationId, t);
    //            if(termState == X_AbstractTerm::TS_CONST_INCLUDED
    //                    || termState == X_AbstractTerm::TS_INCLUDED)
    //            {
    //                factorCount ++;
    //            }
    //        }

    //        emit xg_calculateCalibrationQualityData(false, xv_currentCalibrationId, factorCount + 1, xv_sumSquareAverageConcentrationDispersion);

    //    }


    /*

    // emit xg_currentOperation(TOT_BEGIN_RESET, -1, -1);

    xv_currentArrayIndex = currentArrayIndex;
    xv_currentArrayId = currentArrayId;
    xh_startCalculationCorrelationsAndCovariations();
    emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() - 1);

    // equation quality recalc
    int factorCount = 0;
    X_AbstractTerm::TermState termState;
    for(int t = 0; t < xv_calibrationRepository->xp_termCount(xv_currentCalibrationId); t++)
    {
        termState = xv_calibrationRepository->xp_termState(xv_currentCalibrationId, t);
        if(termState == X_AbstractTerm::TS_CONST_INCLUDED
                || termState == X_AbstractTerm::TS_INCLUDED)
        {
            factorCount ++;
        }
    }

    emit xg_calculateCalibrationQualityData(false, xv_currentCalibrationId, factorCount + 1, xv_sumSquareAverageConcentrationDispersion);

    */


}
//=============================================================================
void X_TermCorrelationTableManager::xh_onCalibrationNormalizerChange(qint64 calibrationId)
{
    if(xv_currentCalibrationId != calibrationId)
    {
        return;
    }

    xh_startCalculationCorrelationsAndCovariations();
    emit xg_currentOperation(TOT_DATA_CHANGED, 0, xp_rowCount() -1);
}
//=============================================================================
void X_TermCorrelationTableManager::xh_recalcCalibrationFactors()
{
    if(!xv_calibrationRepository || xv_currentCalibrationId < 0)
    {
        return;
    }

    //  check matrixes
    if(xv_freeTermCovariationList.count() != xp_rowCount())
    {
        // TODO Error report
        return;
    }

    // check average free term (if a != a  then a == nan)
    if (xv_averageEquationIntercept != xv_averageEquationIntercept)
    {
        // TODO Error report
        return;
    }

    if(xv_termCovariationMatrix.count() != xp_rowCount())
    {
        // TODO Error report
        return;
    }
    // matrixes - ok

    // get calibration that will be recalced
    X_Calibration* calibration = xv_calibrationRepository->xh_calibrationForId(xv_currentCalibrationId);
    if(!calibration)
    {
        // TODO Error message
        return;
    }

    // before solvation reset all termFactors of the calibration
    calibration->xp_resetEquationTerms();

    // set to calibration exposition and energy calibration factors
    if(xv_spectrumArrayRepository != nullptr)
    {
//        qreal K0 = 0.0;
//        qreal K1 = 0.0;
//        qreal K2 = 0.0;
//        QString energyUnit;
//        if(xv_spectrumArrayRepository->xp_energyCalibration(xv_currentArrayIndex,
//                                                            K0, K1, K2, energyUnit))
//        {
//            calibration->xp_setEnergyCalibration(K0, K1, K2, energyUnit);
//        }

        int exposition = xv_spectrumArrayRepository->xp_exposition(xv_currentArrayIndex);
        calibration->xp_setExposition(exposition);

        int gainFactor = xv_spectrumArrayRepository->xp_gainFactor(xv_currentArrayIndex);
        calibration->xp_setGainFactor(gainFactor);
    }

    // pointers to factors and free term
    QMap<int, qreal*> factorToIndexMap;
    qreal* freeTerm;
    calibration->xp_createEquationDataForEquationRecalc(factorToIndexMap, freeTerm);

    qDebug() << factorToIndexMap;

    if (factorToIndexMap.isEmpty())
    {
        if(calibration->xp_equationType() != X_Calibration::ET_FRACTIONAL)
        {
            // TODO Error msg
            return;
        }
        // calc only free term freeTerm = averageNumerator / averageConcentration
        // chem element id
        QString chemElement = calibration->xp_chemElement();
        qint64 chemElementId = xv_spectrumArrayRepository->xp_chemElementIdForName(xv_currentArrayIndex, chemElement);
        if(chemElementId < 0)
        {
            return;
        }

        qreal averageConcentration = 0.0;
        qreal averageBaseValue = 0.0;
        int checkedSpectrumCount = 0;
        qreal value;
        const X_AbstractSpectrum* spectrum;
        for(int s = 0; s < xv_spectrumArrayRepository->xp_spectrumCount(xv_currentArrayIndex); s++)
        {
            spectrum = xv_spectrumArrayRepository->xp_spectrum(xv_currentArrayIndex, s);
            if(!spectrum )
            {
                continue;
            }

            if(!spectrum->xp_isSpectrumChecked())
            {
                continue;
            }

            checkedSpectrumCount++;
            // averageConcentration
            averageConcentration += spectrum->xp_concentrationValue(chemElementId);

            // averageNumerator
            if(!calibration->xp_calcBaseTermValue(spectrum, value))
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
        xv_calibrationRepository->xh_notifyCalibrationRecalc(xv_currentCalibrationId);
        emit xg_calculateCalibrationQualityData(true, xv_currentCalibrationId, 1, xv_sumSquareAverageConcentrationDispersion);
        return;
    }

    X_LESGaussSolver solver;
    connect(&solver, &X_LESGaussSolver::xg_message,
            this, &X_TermCorrelationTableManager::xg_message);

    // create cov column for each factor
    QList<int> factorIndexList = factorToIndexMap.keys();
    int factorCount = factorIndexList.count();

    // column list init
    QList<qreal> columnList;
    QList<qreal> freeTermList;
    int row;
    int col;

    QString testColString;
    QString testFreeTermString;
    // passage through rows of covariance matrix
    for(int rowi = 0; rowi < factorCount; rowi++)
    {
        testColString.clear();
        columnList.clear();
        // passage through columns of covariance matrix
        for(int coli = 0; coli < factorCount; coli++)
        {
            row = factorIndexList.at(rowi);
            col = factorIndexList.at(coli);
            if(!xh_convertColRowForCovariationMatrix(row, col))
            {
                // TODO error report
                return;
            }

            columnList.append(xv_termCovariationMatrix.at(row).at(col));

            testColString.append(
                QString("%1,").arg(QString::number(xv_termCovariationMatrix.at(row).at(col))));
        }
        // append column to solver
        solver.xp_appendTermColumn(factorToIndexMap.value(factorIndexList.at(rowi)), columnList);

        // free term list formation
        freeTermList.append(xv_freeTermCovariationList.at(factorIndexList.at(rowi)));
        //TEST
        testFreeTermString.append(QString("%1,").arg(
            QString::number(xv_freeTermCovariationList.at(factorIndexList.at(rowi)))));
    }

    // append free term column to solver
    solver.xp_appendFreeTermList(freeTermList);

    if(!solver.xp_solve())
    {
        // TODO Error report ?? (solver has already reported ?)
        return;
    }

    // Equation free term calculation
    // first initialization of free term


    *freeTerm = xv_averageEquationIntercept;

    QMap<int, qreal*>::const_iterator it;
    qreal averageTerm;
    for(it = factorToIndexMap.begin(); it != factorToIndexMap.end(); it++)
    {
        averageTerm = xv_averageTermValueList.at(it.key());
        if(averageTerm != averageTerm)
        {
            // one of averageTermValues is nan value. Free term cannot be calculated
            // reset all factors
            calibration->xp_resetEquationTerms();

            // TODO error report
            break;

        }

        *freeTerm -= (*it.value()) * averageTerm;
    }

    // force signal emit
    xv_calibrationRepository->xh_notifyCalibrationRecalc(xv_currentCalibrationId);
    // factor count + 1 - free member

    emit xg_calculateCalibrationQualityData(true, xv_currentCalibrationId, factorCount + 1, xv_sumSquareAverageConcentrationDispersion);
    calibration->xp_setCurrentDateTime();
}
//=============================================================================
bool X_TermCorrelationTableManager::xh_convertColRowForInterCorrelationMatrix(int& row, int& col) const
{
    if(col < row)
    {
        qSwap(col, row);
    }

    col -= row;

    if(row >= xv_termIntercorrelationMatrix.count())
    {
        return false;
    }
    if(col >= xv_termIntercorrelationMatrix.at(row).count())
    {
        return false;
    }

    return true;
}
//=============================================================================
bool X_TermCorrelationTableManager::xh_convertColRowForCovariationMatrix(int& row, int& col) const
{
    if(col < row)
    {
        qSwap(col, row);
    }

    col -= row;

    if(row >= xv_termCovariationMatrix.count())
    {
        return false;
    }
    if(col >= xv_termCovariationMatrix.at(row).count())
    {
        return false;
    }

    return true;
}
//=============================================================================
void X_TermCorrelationTableManager::xh_startCalculationCorrelationsAndCovariations()
{
    if(!xv_spectrumArrayRepository || xv_currentArrayId < 0 || !xv_calibrationRepository)
    {
        // clear average values
        xv_averageEquationIntercept = 0.0;
        xv_averageTermValueList.clear();

        // clear dispersion data
        xv_termDispersionMatrix.clear();
        xv_concentrationDispersionList.clear();
        xv_sumSquareAverageConcentrationDispersion = 0.0;
        xv_interceptDispersionList.clear();

        // clear correlation and covariation data
        xv_termIntercorrelationMatrix.clear();
        xv_concentrationCorrelationList.clear();
        xv_termCovariationMatrix.clear();
        xv_freeTermCovariationList.clear();
        xv_residualCorrelationList.clear();
        return;
    }

    // dispersions

    xh_calcTermDispersions();
    xh_calcConcentrationAndFreeTermDispersions();

    if(xv_termDispersionMatrix.isEmpty())
    {
        return;
    }
    // correlations
    xh_calcIntercorrelationsAndCovariations();
    xh_calcResidualTermCorrelation();
    if(xv_concentrationDispersionList.isEmpty())
    {
        return;
    }
    xh_calcConcentrationCorrelationsAndCavariations();
}
//=============================================================================
void X_TermCorrelationTableManager::xh_calcTermDispersions()
{
    xv_termDispersionMatrix.clear();
    xv_averageTermValueList.clear();

    // local vars for circles
    qreal averageTerm;
    qreal termValue;
    const X_AbstractSpectrum *spectrum;

    // term circle
    for(int t = 0; t < xp_rowCount(); t++)
    {
        // add dispersion list for the current term
        xv_termDispersionMatrix.append(QList<qreal>());

        // vars init
        averageTerm = 0.0;
        termValue = 0.0;
        // spectrum circle
        for(int s = 0; s < xv_spectrumArrayRepository->xp_spectrumCount(xv_currentArrayIndex); s++)
        {
            // get spectrum
            spectrum = xv_spectrumArrayRepository->xp_spectrum(xv_currentArrayIndex, s);
            if(spectrum == nullptr || !spectrum->xp_isSpectrumChecked())
            {
                continue;
            }

            // get term value from spectrum
            if(!xv_calibrationRepository->xp_termVariablePart(xv_currentCalibrationId, t, spectrum, termValue))
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
                xv_termDispersionMatrix.last().clear();
                // get out of spectrum circle
                break;
            }
            xv_termDispersionMatrix.last().append(termValue);
            averageTerm += termValue;

        }

        // average term nan check
        if(averageTerm != averageTerm)
        {
            // add average nan val to average list
            xv_averageTermValueList.append(averageTerm);
            // jump to the next term
            continue;
        }

        if(xv_termDispersionMatrix.last().isEmpty())
        {
            // jump to the next term
            continue;
        }

        // average calc
        int checkedSpectrumNumber = xv_termDispersionMatrix.last().count();
        averageTerm /= checkedSpectrumNumber;
        // add average  val to average list
        xv_averageTermValueList.append(averageTerm);

        // dispersion calc
        for(int i = 0; i < checkedSpectrumNumber; i++)
        {
            xv_termDispersionMatrix.last()[i] = xv_termDispersionMatrix.last().at(i) - averageTerm;
        }
    }
}
//=============================================================================
void X_TermCorrelationTableManager::xh_calcConcentrationAndFreeTermDispersions()
{
    xv_concentrationDispersionList.clear();
    xv_sumSquareAverageConcentrationDispersion = 0.0;
    xv_interceptDispersionList.clear();

    // current conc chem element id
    QString calibrationChemElementString = xv_calibrationRepository->xp_chemElementForCalibrationId(xv_currentCalibrationId);
    if(calibrationChemElementString.isEmpty())
    {
        return;
    }

    qint64 chemElementId = xv_spectrumArrayRepository->xp_chemElementIdForName(xv_currentArrayIndex, calibrationChemElementString);
    if(chemElementId < 0)
    {
        return;
    }

    // equation type
    X_Calibration::EquationType equationType;
    xv_calibrationRepository->xp_equationTypeForCalibrationId(xv_currentCalibrationId, equationType);

    // local vars for circles
    qreal averageConcentration = 0.0;
    qreal concentration = 0.0;
    const X_AbstractSpectrum *spectrum = nullptr;

    qreal equationFreeTerm = 0.0;
    qreal baseTermValue = 0.0;
    qreal averageFractionalFreeTerm = 0.0;
    bool fractionalDispersionError = false;

    // spectrum circle
    for(int s = 0; s < xv_spectrumArrayRepository->xp_spectrumCount(xv_currentArrayIndex); s++)
    {
        // get spectrum
        spectrum = xv_spectrumArrayRepository->xp_spectrum(xv_currentArrayIndex, s);
        if(spectrum == nullptr || !spectrum->xp_isSpectrumChecked())
        {
            continue;
        }

        // concentration for chem element
        concentration = spectrum->xp_concentrationValue(chemElementId);
        averageConcentration += concentration;
        xv_concentrationDispersionList.append(concentration);

        if(equationType == X_Calibration::ET_FRACTIONAL)
        {
            // fractional dispersions
            if(concentration == 0.0)
            {
                fractionalDispersionError = true;
                xv_interceptDispersionList.clear();
            }

            if(!fractionalDispersionError)
            {
                if(!xv_calibrationRepository->xp_calcBaseTermValue(xv_currentCalibrationId, spectrum, baseTermValue))
                {
                    fractionalDispersionError = true;
                    xv_interceptDispersionList.clear();
                    continue;
                }

                equationFreeTerm = baseTermValue / concentration;
                averageFractionalFreeTerm += equationFreeTerm;
                xv_interceptDispersionList.append(equationFreeTerm);
            }
        } // end equationType == X_Calibration::ET_FRACTIONAL
    }

    // average concentration
    if(xv_concentrationDispersionList.isEmpty())
    {
        // assign nan to average concentration
        xv_averageEquationIntercept = std::numeric_limits<double>::quiet_NaN();
        // jump to the next term
        return;
    }

    // average calc
    int checkedSpectrumNumber = xv_concentrationDispersionList.count();
    averageConcentration /= checkedSpectrumNumber;

    xv_sumSquareAverageConcentrationDispersion = 0.0;
    // dispersion calc
    for(int i = 0; i < checkedSpectrumNumber; i++)
    {
        xv_concentrationDispersionList[i] = xv_concentrationDispersionList.at(i) - averageConcentration;
        xv_sumSquareAverageConcentrationDispersion += pow(xv_concentrationDispersionList.at(i), 2.0);
    }

    if(equationType == X_Calibration::ET_FRACTIONAL)
    {
        averageFractionalFreeTerm /= checkedSpectrumNumber;
        if(!fractionalDispersionError)
        {
            for(int i = 0; i < checkedSpectrumNumber; i++)
            {
                xv_interceptDispersionList[i] = xv_interceptDispersionList.at(i) - averageFractionalFreeTerm;
            }
            xv_averageEquationIntercept = averageFractionalFreeTerm;
        }
        else
        {
            xv_averageEquationIntercept = std::numeric_limits<double>::quiet_NaN();
        }
    }
    else
    {
        xv_interceptDispersionList = xv_concentrationDispersionList;
        xv_averageEquationIntercept = averageConcentration;
    }
}
//=============================================================================
void X_TermCorrelationTableManager::xh_calcIntercorrelationsAndCovariations()
{
    xv_termIntercorrelationMatrix.clear();
    xv_termCovariationMatrix.clear();

    // local vars
    qreal numerator;
    qreal tDenominator;
    qreal iDenominator;
    qreal correlation;
    qreal tDispersion;
    qreal iDispersion;
    QString intercorrelationString;
    int termCount = xp_rowCount();

    // term circle
    for(int t = 0; t < termCount; t++)
    {
        // add correlation list for the current term
        xv_termIntercorrelationMatrix.append(QList<QString>());

        //        // append term itself correlation
        //        xv_termIntercorrelationMatrix.last().append("1.00");

        //
        xv_termCovariationMatrix.append(QList<qreal>());
        // check out dispersion t-list
        if(xv_termDispersionMatrix.at(t).isEmpty())
        {
            continue;
        }

        // intercorrelated term circle
        for(int i = t; i < termCount; i++)
        {
            // check out dispersion i-list
            int checkedSpeCount = xv_termDispersionMatrix.at(t).count();
            if(xv_termDispersionMatrix.at(i).count() != checkedSpeCount)
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
                tDispersion = xv_termDispersionMatrix.at(t).at(s);
                iDispersion = xv_termDispersionMatrix.at(i).at(s);

                numerator += tDispersion * iDispersion;
                tDenominator += pow(tDispersion, 2);
                iDenominator += pow(iDispersion, 2);
            }

            // covariation
            xv_termCovariationMatrix.last().append(numerator);

            if(tDenominator == 0.0 || iDenominator == 0.0)
            {
                xv_termIntercorrelationMatrix.last().append("Error");
            }

            correlation = numerator / (sqrt(tDenominator) * sqrt(iDenominator));
            intercorrelationString = QString::number(correlation, 'f', 3);
            xv_termIntercorrelationMatrix.last().append(intercorrelationString);
        }
    }
}
//=============================================================================
void X_TermCorrelationTableManager::xh_calcConcentrationCorrelationsAndCavariations()
{
    // clear list
    xv_concentrationCorrelationList.clear();
    xv_freeTermCovariationList.clear();

    // local vars
    qreal numerator;
    qreal tDenominator;
    qreal cDenominator;
    qreal tDispersion;
    qreal cDispersion;
    qreal freeTermCovariation;
    qreal correlation;
    QString correlationString;
    int termCount = xp_rowCount();
    int checkedSpeCount = xv_concentrationDispersionList.count();
    bool freeDispersionOk = xv_interceptDispersionList.count() == checkedSpeCount;

    // term circle
    for(int t = 0; t < termCount; t++)
    {
        // check term dispersion list and conc dispersion
        if(xv_termDispersionMatrix.at(t).count() != checkedSpeCount)
        {
            xv_concentrationCorrelationList.append("Error");
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
            tDispersion =  xv_termDispersionMatrix.at(t).at(s);
            cDispersion = xv_concentrationDispersionList.at(s);
            if(freeDispersionOk)
            {
                freeTermCovariation += xv_interceptDispersionList.at(s) * tDispersion;
            }
            numerator += tDispersion * cDispersion;
            tDenominator += pow(tDispersion, 2);
            cDenominator += pow(cDispersion, 2);
        }

        // covariation
        if(freeDispersionOk)
        {
            xv_freeTermCovariationList.append(freeTermCovariation);
        }

        if(tDenominator == 0.0 || cDenominator == 0.0)
        {
            xv_concentrationCorrelationList.append("Error");
        }

        correlation = numerator / (sqrt(tDenominator) * sqrt(cDenominator));
        correlationString = QString::number(correlation, 'f', 3);
        xv_concentrationCorrelationList.append(correlationString);

    }
}
//=============================================================================
void X_TermCorrelationTableManager::xh_calcResidualTermCorrelation()
{
    xv_residualCorrelationList.clear();

    // term count
    int termCount = xp_rowCount();
    // matrix check
    if(xv_termDispersionMatrix.count() != termCount)
    {
        return;
    }
    // get residual list
    QList<qreal> residualDispersionList;
    if(!xv_jointSpectraDataManager->xp_calculateConcentrationResidualList(xv_currentCalibrationId,
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

        if(xv_termDispersionMatrix.at(t).count() != checkedSpeCount)
        {
            xv_residualCorrelationList.append("Error");
            continue;
        }

        numerator = 0.0;
        tDenominator = 0.0;
        rDenominator = 0.0;

        // term dispersion circle
        for(int s = 0; s < checkedSpeCount; s++)
        {
            tDispersion =  xv_termDispersionMatrix.at(t).at(s);
            rDispersion = residualDispersionList.at(s);
            numerator += tDispersion * rDispersion;
            tDenominator += pow(tDispersion, 2);
            rDenominator += pow(rDispersion, 2);
        }

        if(tDenominator < 0.0 || rDenominator < 0.0)
        {
            xv_residualCorrelationList.append("Error");
        }

        correlation = numerator / (sqrt(tDenominator) * sqrt(rDenominator));
        correlationString = QString::number(correlation, 'f', 3);
        xv_residualCorrelationList.append(correlationString);
    }
}
//=============================================================================
