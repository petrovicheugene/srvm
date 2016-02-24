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
    zv_currentArrayId = -1;
    zv_currentArrayIndex = -1;


    zv_greenCell = QColor(Qt::green);
    zv_orangeCell = QColor(Qt::darkYellow);
    zv_yellowCell = QColor(Qt::yellow);
    zv_redCell = QColor(Qt::red);

    zv_greenCell.setAlpha(20);
    zv_orangeCell.setAlpha(20);
    zv_yellowCell.setAlpha(20);
    zv_redCell.setAlpha(20);

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
        qreal factor;
        bool ok = zv_calibrationRepository->zp_termFactor(zv_currentCalibrationId, index.row(), factor);

        if(!ok)
        {
            return tr("#Error");
        }

        return QVariant(factor);
    }
    else if(index.column() == 1)
    {

        if(index.row() >= zv_chemElementCorrelationList.count())
        {
            return QVariant();
        }

        return QVariant(zv_chemElementCorrelationList.at(index.row()));
    }
    else if(index.column() >= zv_firstNonTermColumnCount)
    {
        int termRow = index.row();
        int termColumn = index.column() - zv_firstNonTermColumnCount;

        if(termColumn == termRow)
        {
            return QVariant();
        }
        else if(termColumn < termRow)
        {
            return QVariant(zv_termIntercorrelationMap.value(QPair<int, int>(termColumn, termRow)));
        }
        else if(termColumn > termRow)
        {
            return QVariant(zv_termIntercorrelationMap.value(QPair<int, int>(termRow, termColumn)));
        }
    }

    return QVariant();
}
//=============================================================================
bool ZTermCorrelationTableManager::zp_setData(QModelIndex index, QVariant vFactor)
{
    if(!zv_calibrationRepository || !vFactor.isValid() || vFactor.isNull() || !vFactor.canConvert<qreal>())
    {
        return false;
    }

    qreal factor = vFactor.value<qreal>();
    return zv_calibrationRepository->zp_setTermFactor(zv_currentCalibrationId, index.row(), factor);
}
//=============================================================================
QVariant ZTermCorrelationTableManager::zp_cellColor(QModelIndex index) const
{
    if(!zv_calibrationRepository || !index.isValid() || index.row() < 0 || index.column() < 0
            || index.row() >= zp_rowCount() || index.column() >= zp_columnCount())
    {
        return QVariant();
    }

    qreal correlationValue;
    bool ok;

    // chem Correlations
    if( index.column() == 1)
    {
        correlationValue = qAbs(zv_chemElementCorrelationList.value(index.row(), QString()).toDouble(&ok));
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
        // Intercorrelations
        int termRow = index.row();
        int termColumn = index.column() - zv_firstNonTermColumnCount;

        if(termColumn == termRow)
        {
            return QVariant();
        }

        if(termColumn < termRow)
        {
            correlationValue = qAbs(zv_termIntercorrelationMap.value(QPair<int, int>(termColumn, termRow), QString()).toDouble(&ok));
        }
        else if(termColumn > termRow)
        {
            correlationValue = qAbs(zv_termIntercorrelationMap.value(QPair<int, int>(termRow, termColumn), QString()).toDouble(&ok));
        }

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
    zh_calcCorrelations();
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
        zh_calcCorrelations();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_REMOVE_CHEM_ELEMENT)
    {
        zv_chemElementCorrelationList.clear();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_END_REMOVE_CHEM_ELEMENT)
    {
        //        zh_calcChemElementCorrelations();
        //        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_CHEM_ELEMENT_CHANGED)
    {
        zh_calcCorrelations();
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
    zh_calcCorrelations();
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

        zh_calcCorrelations();
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
        zh_calcCorrelations();
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
    else if(type == ZCalibrationRepository::TOT_TERM_VALUE_CHANGED)
    {
        zh_calcCorrelations();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
#ifdef DBG
        qDebug() << "WINDOW CHANGE RECALC";
#endif
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
                zv_chemElementCorrelationList.clear();
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
            }
            else if(type == ZCalibrationRepository::COT_END_RESET)
            {
                zh_calcCorrelations();
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
            }
            else if(type == ZCalibrationRepository::COT_INSERT_CALIBRATIONS)
            {
                zv_chemElementCorrelationList.clear();
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
            }
            else if(type == ZCalibrationRepository::COT_END_INSERT_CALIBRATIONS)
            {
                zh_calcCorrelations();
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
            }
            else if(type == ZCalibrationRepository::COT_REMOVE_CALIBRATIONS)
            {
                zv_chemElementCorrelationList.clear();
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
            }
            else if(type == ZCalibrationRepository::COT_END_REMOVE_CALIBRATIONS)
            {
                zh_calcCorrelations();
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
            }
            else if(type == ZCalibrationRepository::COT_CALIBRATION_NAME_CHANGED)
            {
                zh_calcCorrelations();
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
            }
            else if(type == ZCalibrationRepository::COT_CALIBRATION_CHEM_ELEMENT_CHANGED)
            {
                zv_chemElementCorrelationList.clear();
                zh_calcCorrelations();
                emit zg_currentOperation(TOT_HORIZONTAL_HEADER_CHANGED, 0, 0);
                emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() - 1);
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
        zh_calcCorrelations();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() -1);
    }
    else if(type == ZSpectrumArrayRepository::SOT_BEGIN_REMOVE_SPECTRA)
    {

    }
    else if(type == ZSpectrumArrayRepository::SOT_END_REMOVE_SPECTRA)
    {
        zh_calcCorrelations();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() -1);
    }
    else if(type == ZSpectrumArrayRepository::SOT_CHECKED_CHANGED)
    {
        zh_calcCorrelations();
        emit zg_currentOperation(TOT_DATA_CHANGED, 0, zp_rowCount() -1);
    }
}
//=============================================================================
//bool ZTermCorrelationTableManager::zh_calcChemElementCorrelation(int termIndex, qreal& correlationValue) const
//{
//    if(!zv_calibrationRepository || !zv_spectrumArrayRepository || zv_currentCalibrationId < 0 || zv_currentArrayId < 0)
//    {
//        return false;
//    }

//    QString calibrationChemElement = zv_calibrationRepository->zp_chemElementForCalibrationId(zv_currentCalibrationId);
//    qint64 currentCalibrationChemId = zv_spectrumArrayRepository->zp_chemElementIdForName(zv_currentArrayId, calibrationChemElement);

//    // TODO zp_averageChemConcentration
//    qreal averageChemConcentration;
//    qreal termAverage;

//    bool res = zv_spectrumArrayRepository->zp_averageChemConcentrationForChemElementId(zv_currentArrayId,
//                                                                                       currentCalibrationChemId, averageChemConcentration);
//    res = res && zv_calibrationRepository->zp_termAverageValue(zv_currentCalibrationId, termIndex, termAverage);

//    if(!res)
//    {
//        return false;
//    }

//    QList<QPair<qint64, qreal> > termValueAndChemConcentrationList;
//    zh_createTermValueAndChemConcentrationList(termIndex,
//                                               currentCalibrationChemId,
//                                               termValueAndChemConcentrationList);

//    if(termValueAndChemConcentrationList.isEmpty() )
//    {
//        return false;
//    }

//    qreal numerator = 0;
//    qreal denominator1 = 0;
//    qreal denominator2 = 0;

//    qreal currentTermDelta = 0;
//    qreal currentConcDelta = 0;

//    for(int i = 0; i < termValueAndChemConcentrationList.count(); i++)
//    {
//        currentTermDelta = (termValueAndChemConcentrationList.at(i).first - termAverage);
//        currentConcDelta = (termValueAndChemConcentrationList.at(i).second - averageChemConcentration);

//        numerator += currentTermDelta * currentConcDelta;
//        denominator1 += pow(currentTermDelta,2);
//        denominator2 += pow(currentConcDelta,2);
//    }

//    if(numerator == 0)
//    {
//        correlationValue = 0;
//        return true;
//    }

//    qreal denominator = sqrt(denominator1) * sqrt(denominator2);
//    if(denominator == 0)
//    {
//        return false;
//    }

//    correlationValue = numerator / denominator;
//    return true;
//}
////=============================================================================
//void ZTermCorrelationTableManager::zh_createTermValueAndChemConcentrationList(int termIndex,
//                                                                              qint64 chemElementId,
//                                                                              QList<QPair<qint64, qreal> >& termAndConcentrationList) const
//{
//    termAndConcentrationList.clear();
//    const ZAbstractSpectrum* spectrum;
//    qint64 termValue;
//    QPair<qint64, qreal>  valuePair;
//    bool ok;
//    qreal concentrationValue;
//    for(int i = 0; i < zv_spectrumArrayRepository->zp_spectrumCount(zv_currentArrayId); i++)
//    {
//        spectrum = zv_spectrumArrayRepository->zp_spectrum(zv_currentArrayId, i);
//        if(!spectrum)
//        {
//            continue;
//        }

//        if(!spectrum->zp_isSpectrumChecked())
//        {
//            continue;
//        }

//        ok = zv_calibrationRepository->zp_termVariablePart(zv_currentCalibrationId, termIndex, spectrum, termValue);
//        if(!ok)
//        {
//            termValue = 0;
//        }
//        valuePair.first = termValue;
//        concentrationValue = spectrum->zp_concentration(chemElementId).toDouble(&ok);
//        if(!ok)
//        {
//            concentrationValue = 0;
//        }
//        valuePair.second = concentrationValue;
//        termAndConcentrationList.append(valuePair);
//    }
//}
//=============================================================================
void ZTermCorrelationTableManager::zh_calcCorrelations()
{
#ifdef DBG
    qDebug() << "CHEM CORRELATION RECALCULATION";
#endif

    zv_chemElementCorrelationList.clear();
    zv_termIntercorrelationMap.clear();

    if(!zv_spectrumArrayRepository || zv_currentArrayId < 0
            || !zv_calibrationRepository || zv_currentCalibrationId < 0)
    {
        return;
    }

    // Collect data from repositories
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

    QList< QPair<qreal, QList<qreal> > > chemConcentrationAndTermValueList;
    const ZAbstractSpectrum* spectrum;
    QString concentrationString;
    qreal concentration;
    bool ok;

    qreal averageChemElementValue = 0.0;
    bool averageChemElementOk = true;
    QSet<int> invalidTermRows;

    for(int s = 0; s < zv_spectrumArrayRepository->zp_spectrumCount(zv_currentArrayId); s++)
    {
        // get spectrum
        spectrum = zv_spectrumArrayRepository->zp_spectrum(zv_currentArrayId, s);
        if(!spectrum->zp_isSpectrumChecked())
        {
            continue;
        }

        // get term values for the spectrum
        QList<qreal> termValueList;
        qreal termValue;

        for(int t = 0; t < zp_rowCount(); t++)
        {
            if(!zv_calibrationRepository->zp_termVariablePart(zv_currentCalibrationId, t, spectrum, termValue))
            {
                termValue = -1;
                invalidTermRows << t;
            }
            termValueList.append(termValue);
        }

        // concentration for chem element
        if(averageChemElementOk)
        {
            concentrationString = spectrum->zp_concentration(chemElementId);
            if(concentrationString.isEmpty())
            {
                concentration = 0;
                averageChemElementOk = false;
            }
            else
            {
                concentration = concentrationString.toDouble(&ok);
                if(!ok)
                {
                    concentration = 0;
                    averageChemElementOk = false;
                }
                else
                {
                    averageChemElementValue += concentration;
                }
            }
        }
        chemConcentrationAndTermValueList.append(QPair<qreal, QList<qreal> >(concentration, termValueList));
    }

    // No a checked spectrum
    if(chemConcentrationAndTermValueList.isEmpty())
    {
        return;
    }


    // data from repositories are collected
    // average term values
    QMap<int, qreal> averageTermValueMap;
    qreal summTermValue;

    for(int t = 0; t < zp_rowCount(); t++)
    {
        if(invalidTermRows.contains(t))
        {
            continue;
        }

        summTermValue = 0.0;
        for(int i = 0; i < chemConcentrationAndTermValueList.count(); i++)
        {
            summTermValue += chemConcentrationAndTermValueList.at(i).second.at(t);
        }

        averageTermValueMap.insert(t, summTermValue / chemConcentrationAndTermValueList.count());
    }

    if(averageChemElementOk && !averageTermValueMap.isEmpty())
    {
        // chem average and recalc chem correlations
        averageChemElementValue /= chemConcentrationAndTermValueList.count();
        zh_calcChemElementCorrelations(chemConcentrationAndTermValueList,
                                       averageChemElementValue,
                                       averageTermValueMap);
    }

    if(!averageTermValueMap.isEmpty())
    {
        // recalc intercorrelations
        zh_calcIntercorrelations(chemConcentrationAndTermValueList,
                                 averageTermValueMap);
    }
}
//=============================================================================
void ZTermCorrelationTableManager::zh_calcChemElementCorrelations(const QList< QPair<qreal, QList<qreal> > >& chemConcentrationAndTermValueList,
                                                                  qreal averageChemElementValue,
                                                                  const QMap<int, qreal>& averageTermValueMap)
{
    qreal correlationValue;
    qreal numerator = 0.0;

    qreal denominator1 = 0;
    qreal denominator2 = 0;

    qreal currentTermDelta = 0;
    qreal currentConcDelta = 0;

    for(int t = 0; t < zp_rowCount(); t++)
    {
        if(!averageTermValueMap.contains(t))
        {
            zv_chemElementCorrelationList.append("Error");
            continue;
        }

        numerator = 0.0;
        denominator1 = 0.0;
        denominator2 = 0.0;
        for(int s = 0; s < chemConcentrationAndTermValueList.count(); s++)
        {
            currentTermDelta = chemConcentrationAndTermValueList.at(s).second.at(t) - averageTermValueMap.value(t);
            currentConcDelta = chemConcentrationAndTermValueList.at(s).first - averageChemElementValue;
            numerator += currentTermDelta * currentConcDelta;

            denominator1 += pow(currentTermDelta,2);
            denominator2 += pow(currentConcDelta,2);
        }

        if(numerator == 0)
        {
            zv_chemElementCorrelationList.append(QString::number(0.0, 'f', 2));
            continue;
        }

        qreal denominator = sqrt(denominator1) * sqrt(denominator2);
        if(denominator == 0)
        {
            zv_chemElementCorrelationList.append("Error");
            continue;
        }

        correlationValue = numerator / denominator;
        zv_chemElementCorrelationList.append(QString::number(correlationValue, 'f', 2));
    }
}
//=============================================================================
void ZTermCorrelationTableManager::zh_calcIntercorrelations(const QList< QPair<qreal, QList<qreal> > >& chemConcentrationAndTermValueList,
                                                            const QMap<int, qreal>& averageTermValueMap)
{
    QMap<int, QList<qreal> > deltaMap;
//    qreal rowTermDelta = 0;
//    qreal columnTermDelta = 0;

    qreal denominator1 = 0;
    qreal denominator2 = 0;

    qreal correlationValue;
    qreal numerator = 0.0;
    QString intercorrelationString;


    for(int r = 0; r < zp_rowCount(); r++)
    {
        if(!deltaMap.contains(r) && averageTermValueMap.contains(r))
        {
            QList<qreal> deltaList;
            for(int s = 0; s < chemConcentrationAndTermValueList.count(); s++)
            {
                deltaList.append(chemConcentrationAndTermValueList.at(s).second.at(r) - averageTermValueMap.value(r));
            }

            deltaMap.insert(r, deltaList);
        }

        for(int c = r + 1; c <  zp_rowCount(); c++)
        {
            if(!deltaMap.contains(c) && averageTermValueMap.contains(c))
            {
                QList<qreal> deltaList;
                for(int s = 0; s < chemConcentrationAndTermValueList.count(); s++)
                {
                    deltaList.append(chemConcentrationAndTermValueList.at(s).second.at(c) - averageTermValueMap.value(c));
                }

                deltaMap.insert(c, deltaList);
            }

            numerator = 0.0;
            denominator1 = 0;
            denominator2 = 0;

            for(int s = 0; s < chemConcentrationAndTermValueList.count(); s++)
            {
                numerator += deltaMap.value(r).value(s) * deltaMap.value(c).value(s);
                denominator1 += pow(deltaMap.value(r).value(s), 2);
                denominator2 += pow(deltaMap.value(c).value(s), 2);
            }

            if(numerator == 0)
            {
                zv_termIntercorrelationMap.insert(QPair<int, int>(r,c), QString::number(0.0, 'f', 2));
                continue;
            }

            qreal denominator = sqrt(denominator1) * sqrt(denominator2);
            if(denominator == 0)
            {
                zv_termIntercorrelationMap.insert(QPair<int, int>(r,c), "Error");
                continue;
            }

            correlationValue = numerator / denominator;

            intercorrelationString = QString::number(correlationValue, 'f', 2);
            zv_termIntercorrelationMap.insert(QPair<int, int>(r,c), intercorrelationString);
        }
    }
}
//=============================================================================

