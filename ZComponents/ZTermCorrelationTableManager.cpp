//=============================================================================
#include "ZTermCorrelationTableManager.h"
#include "ZLSEGaussSolver.h"
#include "globalVariables.h"
#include <QPair>
//=============================================================================
ZTermCorrelationTableManager::ZTermCorrelationTableManager(QObject *parent) : QObject(parent)
{

    zv_calibrationRepository = 0;
    zv_spectrumArrayRepository = 0;
    zv_currentCalibrationId = -1;
    zv_columnCountCorrector = 0;
    zv_currentArrayId = -1;
    zv_currentArrayIndex = -1;
    zv_averageEquationFreeTerm = std::numeric_limits<double>::quiet_NaN();

    zv_greenCell = QColor(Qt::green);
    zv_orangeCell = QColor(Qt::yellow);
    zv_yellowCell = QColor(Qt::yellow);
    zv_redCell = QColor(Qt::red);

    zv_greenCell.setAlpha(30);
    zv_orangeCell.setAlpha(30);
    zv_yellowCell.setAlpha(30);
    zv_redCell.setAlpha(30);

}
//=============================================================================
void ZTermCorrelationTableManager::zp_connectToCalibrationRepository(ZCalibrationRepository* repository)
{
    emit zg_currentOperation(TOT_BEGIN_RESET, -1, -1);
    zv_calibrationRepository = repository;
    connect(repository, &ZCalibrationRepository::zg_termOperation,
            this, &ZTermCorrelationTableManager::zh_onRepositoryTermOperation);
    //    connect(repository, &ZCalibrationRepository::zg_calibrationWindowOperation,
    //            this, &ZTermCorrelationTableManager::zh_onCalibrationWindowOperation);

    connect(repository, &ZCalibrationRepository::zg_currentCalibrationChanged,
            this, &ZTermCorrelationTableManager::zh_currentCalibrationChanged);
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_calibrationOperation,
            this, &ZTermCorrelationTableManager::zh_onCalibrationRepositoryOperation);
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_normalizerChanged,
            this, &ZTermCorrelationTableManager::zh_onCalibrationNormalizerChange);

    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_invokeCalibrationRecalc,
            this, &ZTermCorrelationTableManager::zh_recalcCalibrationFactors);

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
    if(!zv_calibrationRepository || !index.isValid() || index.row() < 0 || index.column() < 0
            || index.row() >= zp_rowCount() || index.column() >= zp_columnCount())
    {
        return QVariant();
    }

    if(index.column() == 0)
    {
        QString factorString;
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
    if(!zv_calibrationRepository || !vFactor.isValid() || vFactor.isNull() || !vFactor.canConvert<QString>())
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
    if( index.column() == 1)
    {
        correlationValue = qAbs(zv_concentrationCorrelationList.value(index.row(), QString()).toDouble(&ok));
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
            return QVariant(zv_yellowCell);
        }
    }
    else if(index.column() >=  zv_firstNonTermColumnCount)
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
            return QVariant(zv_orangeCell);
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
        if(zv_currentCalibrationId < 0 || zv_calibrationRepository == 0)
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
        pixmap = QPixmap(":/images/question-yellow-1s.png");
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
}
//==================================================================
void ZTermCorrelationTableManager::zh_onRepositoryChemElementOperation(ZSpectrumArrayRepository::ChemElementOperationType type,
                                                                       int arrayIndex, int first, int last)
{
    if(zv_currentArrayIndex != arrayIndex || zv_currentCalibrationId < 0
            || zv_spectrumArrayRepository == 0 || zv_calibrationRepository == 0)
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
void ZTermCorrelationTableManager::zh_currentCalibrationChanged(qreal calibrationId, int calibrationIndex)
{
    if(zv_currentCalibrationId == calibrationId)
    {
        return;
    }

    emit zg_currentOperation(TOT_BEGIN_RESET, -1, -1);

    zv_currentCalibrationId = calibrationId;
    zh_startCalculationCorrelationsAndCovariations();
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
    else if(type == ZSpectrumArrayRepository::SOT_END_INSERT_SPECTRA)
    {
        zh_startCalculationCorrelationsAndCovariations();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() -1);
    }
    else if(type == ZSpectrumArrayRepository::SOT_BEGIN_REMOVE_SPECTRA)
    {

    }
    else if(type == ZSpectrumArrayRepository::SOT_END_REMOVE_SPECTRA)
    {
        zh_startCalculationCorrelationsAndCovariations();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() -1);
    }
    else if(type == ZSpectrumArrayRepository::SOT_CHECKED_CHANGED)
    {
        zh_startCalculationCorrelationsAndCovariations();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() -1);
    }
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

    // check average free term
    if(zv_averageEquationFreeTerm != zv_averageEquationFreeTerm)
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

    // pointers to factors and free term
    QMap<int, qreal*> factorToIndexMap;
    qreal* freeTerm;
    calibration->zp_createEquationDataForEquationRecalc(factorToIndexMap, freeTerm);

    if(factorToIndexMap.isEmpty())
    {
        // TODO Error msg
        return;
    }

    ZLSEGaussSolver solver;
    connect(&solver, &ZLSEGaussSolver::zg_message,
            this, &ZTermCorrelationTableManager::zg_message);

    // create cov column for each factor
    QList<int> factorIndexList = factorToIndexMap.keys();
    int factorCount = factorIndexList.count();

#ifdef DBG
    qDebug() << "FACTORS BEFORE";
#endif
    // column list init
    QList<qreal> columnList;
    QList<qreal> freeTermList;
    int row;
    int col;

    // passage through rows of covariance matrix
    for(int rowi = 0; rowi < factorCount; rowi++)
    {
        columnList.clear();
        row = factorIndexList.at(rowi);

#ifdef DBG
        qDebug() << row << *factorToIndexMap.value(row);
#endif

        // passage through columns of covariance matrix
        for(int coli = 0; coli < factorCount; coli++)
        {
            col = factorIndexList.at(coli);
            if(!zh_convertColRowForCovariationMatrix(row, col))
            {
                // TODO error report
                return;
            }
            columnList.append(zv_termCovariationMatrix.at(row).at(col));
        }
        // append column to solver
        solver.zp_appendTermColumn(factorToIndexMap.value(rowi), columnList);
        // free term list formation
        freeTermList.append(zv_freeTermCovariationList.at(rowi));
    }

    // append free term column to solver
    solver.zp_appendFreeTermList(freeTermList);

    // before solvation reset all termFactors of the calibration
    calibration->zp_resetEquationTerms();

    if(!solver.zp_solve())
    {
        // TODO Error report ?? (solver has already reported ?)
        return;
    }

#ifdef DBG
    qDebug() << "FACTORS AFTER";

    QMap<int, qreal*>::iterator itr;
    for(itr = factorToIndexMap.begin(); itr != factorToIndexMap.end(); itr++)
    {
        qDebug() << itr.key() << *itr.value();
    }

#endif

    //    int firstfactorIndex = factorIndexList.at(0);
    //    qreal firstCovariationValue = zv_termCovariationMatrix.at(firstfactorIndex).at(firstfactorIndex);
    //    if(firstCovariationValue == 0)
    //    {
    //        // TODO Error report
    //        return;
    //    }


    //    zv_freeTermCovariationList.at(firstfactorIndex) / firstCovariationValue;

    // Equation free term calculation
    // first initialization of free term
    *freeTerm = zv_averageEquationFreeTerm;
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
        zv_averageEquationFreeTerm = 0.0;
        zv_averageTermValueList.clear();

        // clear dispersion data
        zv_termDispersionMatrix.clear();
        zv_concentrationDispersionList.clear();
        zv_freeTermDispersionList.clear();

        // clear correlation and covariation data
        zv_termIntercorrelationMatrix.clear();
        zv_concentrationCorrelationList.clear();
        zv_termCovariationMatrix.clear();
        zv_freeTermCovariationList.clear();
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

    if(zv_concentrationDispersionList.isEmpty())
    {
        return;
    }
    zh_calcConcentrationCorrelationsAndCavariations();

    //#ifdef DBG
    //    qDebug() <<  "TERM COVARIATION CHECK";

    //    for(int t = 0; t < zv_termCovariationMatrix.count(); t++)
    //    {
    //        QString str = QString::number(t) + ":";
    //        for(int i = 0; i < zv_termCovariationMatrix.at(t).count(); i++)
    //        {
    //            str += " " + QString::number(zv_termCovariationMatrix.at(t).at(i));
    //        }
    //        qDebug() <<  str;

    //    }

    //    qDebug() <<  "FREE TERM COVARIATION CHECK";

    //    for(int t = 0; t < zv_freeTermCovariationList.count(); t++)
    //    {
    //        qDebug() <<  QString::number(zv_freeTermCovariationList.at(t));
    //    }

    //#endif

}
//=============================================================================
void ZTermCorrelationTableManager::zh_calcTermDispersions()
{
    zv_termDispersionMatrix.clear();

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
            if(spectrum == 0 || !spectrum->zp_isSpectrumChecked())
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
    zv_freeTermDispersionList.clear();

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
        if(spectrum == 0 || !spectrum->zp_isSpectrumChecked())
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
            if(concentration == 0)
            {
                fractionalDispersionError = true;
                zv_freeTermDispersionList.clear();
            }

            if(!fractionalDispersionError)
            {
                if(!zv_calibrationRepository->zp_calcBaseTermValue(zv_currentCalibrationId, spectrum, baseTermValue))
                {
                    fractionalDispersionError = true;
                    zv_freeTermDispersionList.clear();
                    continue;
                }

                equationFreeTerm = baseTermValue / concentration;
                averageFractionalFreeTerm += equationFreeTerm;
                zv_freeTermDispersionList.append(equationFreeTerm);
            }
        } // end equationType == ZCalibration::ET_FRACTIONAL
    }

    // average concentration
    if(zv_concentrationDispersionList.isEmpty())
    {
        // assign nan to average concentration
        zv_averageEquationFreeTerm = std::numeric_limits<double>::quiet_NaN();
        // jump to the next term
        return;
    }

    // average calc
    int checkedSpectrumNumber = zv_concentrationDispersionList.count();
    averageConcentration /= checkedSpectrumNumber;

    // dispersion calc
    for(int i = 0; i < checkedSpectrumNumber; i++)
    {
        zv_concentrationDispersionList[i] = zv_concentrationDispersionList.at(i) - averageConcentration;
    }

    if(equationType == ZCalibration::ET_FRACTIONAL)
    {
        averageFractionalFreeTerm /= checkedSpectrumNumber;
        if(!fractionalDispersionError)
        {
            for(int i = 0; i < checkedSpectrumNumber; i++)
            {
                zv_freeTermDispersionList[i] = zv_freeTermDispersionList.at(i) - averageFractionalFreeTerm;
            }
            zv_averageEquationFreeTerm = averageFractionalFreeTerm;
        }
        else
        {
            zv_averageEquationFreeTerm = std::numeric_limits<double>::quiet_NaN();
        }
    }
    else
    {
        zv_freeTermDispersionList = zv_concentrationDispersionList;
        zv_averageEquationFreeTerm = averageConcentration;
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

            if(tDenominator == 0 || iDenominator == 0)
            {
                zv_termIntercorrelationMatrix.last().append("Error");
            }

            correlation = numerator / (sqrt(tDenominator) * sqrt(iDenominator));
            intercorrelationString = QString::number(correlation, 'f', 2);
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
    bool freeDispersionOk = zv_freeTermDispersionList.count() == checkedSpeCount;

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
                freeTermCovariation += zv_freeTermDispersionList.at(s) * tDispersion;
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

        if(tDenominator == 0 || cDenominator == 0)
        {
            zv_concentrationCorrelationList.append("Error");
        }

        correlation = numerator / (sqrt(tDenominator) * sqrt(cDenominator));
        correlationString = QString::number(correlation, 'f', 2);
        zv_concentrationCorrelationList.append(correlationString);

    }
}
//=============================================================================

