//==================================================================
#include <math.h>
#include "X_JointSpectraDataManager.h"
#include "X_LocaleDoubleConverter.h"
#include "X_SpectrumPaintData.h"
#include <math.h>
//==================================================================
X_JointSpectraDataManager::X_JointSpectraDataManager(QObject *parent) : QObject(parent)
{
    xv_currentArrayId = -1;
    xv_currentArrayIndex = -1;
    xv_spectrumArrayRepository = nullptr;
    xv_calibrationRepository = nullptr;
    xv_visibleChemElementCount = 0;
    xv_visibleCalibrationCount = 0;

    xv_concentrationFormat = 'g';
    xv_concentrationPrecision = 6;
}
//==================================================================
void X_JointSpectraDataManager::xp_connectToSpectrumArrayRepository(X_SpectrumArrayRepository* repository)
{
    xv_spectrumArrayRepository = repository;
    // array repository <-> array model
    connect(repository, &X_SpectrumArrayRepository::xg_spectrumOperation,
            this, &X_JointSpectraDataManager::xh_onRepositoryArrayOperation);
    connect(repository, &X_SpectrumArrayRepository::xg_chemElementOperation,
            this, &X_JointSpectraDataManager::xh_onRepositoryChemElementOperation);
    connect(repository, &X_SpectrumArrayRepository::xg_currentArrayIdChanged,
            this, &X_JointSpectraDataManager::xh_currentSpectrumArrayChanged);
    connect(repository, &X_SpectrumArrayRepository::xg_requestCurrentChemConcentrationCellIndex,
            this, &X_JointSpectraDataManager::xh_currentChemConcentrationCellIndex);
    connect(this, &X_JointSpectraDataManager::xg_onSelectionChange,
            repository, &X_SpectrumArrayRepository::xp_onSelectionSpectraChange);

    connect(repository, &X_SpectrumArrayRepository::xg_requestClearSelected,
            this, &X_JointSpectraDataManager::xg_requestClearSelected);
    connect(repository, &X_SpectrumArrayRepository::xg_requestSelectedString,
            this, &X_JointSpectraDataManager::xg_requestSelectedString);

}
//==================================================================
void X_JointSpectraDataManager::xp_connectToCalibrationRepository(X_CalibrationRepository* repository)
{
    xv_calibrationRepository = repository;
    connect(repository, &X_CalibrationRepository::xg_calibrationOperation,
            this, &X_JointSpectraDataManager::xh_onRepositoryCalibrationOperation);
    connect(repository, &X_CalibrationRepository::xg_termOperation,
            this, &X_JointSpectraDataManager::xh_onRepositoryTermOperation);

}
//==================================================================
int X_JointSpectraDataManager::xp_rowCount() const
{
    if(xv_currentArrayIndex < 0 || xv_spectrumArrayRepository == nullptr)
    {
        return 0;
    }

    return xv_spectrumArrayRepository->xp_spectrumCount(xv_currentArrayIndex);
}
//==================================================================
int X_JointSpectraDataManager::xp_columnCount() const
{
    return xv_spectrumDataColumnCount + xv_visibleChemElementCount + xv_visibleCalibrationCount;
}
//==================================================================
int X_JointSpectraDataManager::xp_visibleChemElementCount() const
{
    if(xv_currentArrayIndex < 0 || xv_spectrumArrayRepository == nullptr)
    {
        return 0;
    }

    return xv_spectrumArrayRepository->xp_visibleChemElementCount(xv_currentArrayIndex);
}
//==================================================================
bool X_JointSpectraDataManager::xp_isColumnChemElement(int column) const
{
    if(xv_spectrumArrayRepository == nullptr)
    {
        return false;
    }

    if(column < xv_spectrumDataColumnCount
            || column > xv_spectrumArrayRepository->xp_visibleChemElementCount(xv_currentArrayIndex)
            + xv_spectrumDataColumnCount - 1)
    {
        return false;
    }

    return true;
}
//==================================================================
QVariant X_JointSpectraDataManager::xp_data(QModelIndex index) const
{
    if(!xv_spectrumArrayRepository || !index.isValid())
    {
        return QVariant();
    }

    if(index.column() == 0)
    {
        return QVariant(xv_spectrumArrayRepository->xp_spectrumName(xv_currentArrayIndex, index.row()));
    }
    if(index.column() == 1)
    {
        X_SpectrumPaintData paintData;
        paintData.spectrumData = xv_spectrumArrayRepository->xp_spectrumData(xv_currentArrayIndex, index.row());
        paintData.maxChannel = xv_spectrumArrayRepository->xp_arrayChannelCount(xv_currentArrayIndex);
        paintData.maxIntensity = xv_spectrumArrayRepository->xp_arrayMaxIntensity(xv_currentArrayIndex);
        return QVariant::fromValue(paintData);
    }

    // int visibleChemElementCount = xv_spectrumArrayRepositiry->xp_visibleChemElementCount(xv_currentArrayIndex);
    if(index.column() >= xv_spectrumDataColumnCount
            && index.column() < xv_visibleChemElementCount + xv_spectrumDataColumnCount)
    {
        QString concentrationString = xv_spectrumArrayRepository->xp_chemConcentration(xv_currentArrayIndex,
                                                                                       index.row(),
                                                                                       index.column() - xv_spectrumDataColumnCount);
        return QVariant(concentrationString);
    }

    if(index.column() >= xv_spectrumDataColumnCount + xv_visibleChemElementCount
            && index.column() < xv_spectrumDataColumnCount + xv_visibleChemElementCount + xv_visibleCalibrationCount)
    {
        //        QString calibrationString = xv_calibrationRepository->xp_calibrationName(index.column()
        //                                                                                 - xv_spectrumDataColumnCount - xv_visibleChemElementCount);

        int visibleCalibrationIndex = index.column() - xv_spectrumDataColumnCount - xv_visibleChemElementCount;
        qint64 calibrationId = xv_calibrationRepository->xp_visibleCalibrationId(visibleCalibrationIndex);

        if(!xv_calibrationConcentrationMap.keys().contains(calibrationId))
        {
            return QVariant();
        }

        QString str = xv_calibrationConcentrationMap.value(calibrationId).value(index.row());
        return str; // xv_calculatedConcentrationMap.value(calibrationString).value(index.row());
    }

    return QVariant();
}
//==================================================================
QString X_JointSpectraDataManager::xp_columnName(int columnIndex) const
{
    if(columnIndex == 0)
    {
        return tr("File");
    }
    else if(columnIndex == 1)
    {
        return tr("Spectrum");
    }
    else if(columnIndex > 1 && columnIndex < xv_spectrumDataColumnCount + xv_visibleChemElementCount)
    {
        if(xv_spectrumArrayRepository == nullptr)
        {
            return QString();
        }

        int visibleChemElementIndex = columnIndex - xv_spectrumDataColumnCount;
        if(visibleChemElementIndex >= 0
                && visibleChemElementIndex < xv_spectrumArrayRepository->xp_visibleChemElementCount(xv_currentArrayIndex))
        {
            return xv_spectrumArrayRepository->xp_visibleChemElementName(xv_currentArrayIndex, visibleChemElementIndex);
        }
        else
        {
            return QString();
        }
    }
    else if(columnIndex > 1 && columnIndex < xv_spectrumDataColumnCount +
            xv_visibleChemElementCount + xv_visibleCalibrationCount)
    {
        if(xv_calibrationRepository == nullptr)
        {
            return QString();
        }

        int visibleCalibrationIndex = columnIndex - xv_spectrumDataColumnCount - xv_visibleChemElementCount;
        if(visibleCalibrationIndex >= 0
                && visibleCalibrationIndex < xv_calibrationRepository->xp_visibleCalibrationCount())
        {
            return xv_calibrationRepository->xp_visibleCalibrationName(visibleCalibrationIndex);
        }
        else
        {
            return QString();
        }
    }
    return QString();
}
//==================================================================
bool X_JointSpectraDataManager::xp_isSpectrumVisible(int spectrumIndex) const
{
    if(!xv_spectrumArrayRepository || xv_currentArrayIndex < 0 )
    {
        return false;
    }

    return xv_spectrumArrayRepository->xp_isSpectrumVisible(xv_currentArrayIndex, spectrumIndex);
}
//==================================================================
QColor X_JointSpectraDataManager::xp_spectrumColor(int spectrumIndex) const
{
    if(!xv_spectrumArrayRepository || xv_currentArrayIndex < 0 )
    {
        return QColor();
    }

    return xv_spectrumArrayRepository->xp_spectrumColor(xv_currentArrayIndex, spectrumIndex);
}
//==================================================================
bool X_JointSpectraDataManager::xp_setSpectrumVisible(int spectrumIndex, bool visible)
{
    if(!xv_spectrumArrayRepository || xv_currentArrayIndex < 0 )
    {
        return false;
    }

    return xv_spectrumArrayRepository->xp_setSpectrumVisible(xv_currentArrayIndex, spectrumIndex, visible);
}
//==================================================================
bool X_JointSpectraDataManager::xp_isSpectrumChecked(int spectrumIndex) const
{
    if(!xv_spectrumArrayRepository)
    {
        return false;
    }

    return xv_spectrumArrayRepository->xp_isSpectrumChecked(xv_currentArrayIndex, spectrumIndex);
}
//==================================================================
bool X_JointSpectraDataManager::xp_setSpectrumChecked(int spectrumIndex, bool checked)
{
    if(!xv_spectrumArrayRepository || xv_currentArrayIndex < 0 )
    {
        return false;
    }

    return xv_spectrumArrayRepository->xp_setSpectrumChecked(xv_currentArrayIndex, spectrumIndex, checked);
}
//==================================================================
bool X_JointSpectraDataManager::xp_setChemConcentration(int row, int column, const QString& consentration)
{
    if(!xv_spectrumArrayRepository || !xp_isColumnChemElement(column)
            || xv_currentArrayIndex < 0 )
    {
        return false;
    }

    bool ok;
    X_LocaleDoubleConverter::toDouble(consentration, &ok);
    // consentration.toDouble(&ok);

    if(!ok)
    {
        return false;
    }

    return xv_spectrumArrayRepository->xp_setChemConcentration(xv_currentArrayIndex,
                                                               row,
                                                               column - xv_spectrumDataColumnCount,
                                                               consentration);
}
//==================================================================
int X_JointSpectraDataManager::xp_spectrumDataColumnCount()
{
    return xv_spectrumDataColumnCount;
}
//==================================================================
void X_JointSpectraDataManager::xh_currentSpectrumArrayChanged(qint64 currentArrayId, int currentArrayIndex)
{
    if(xv_currentArrayIndex == currentArrayIndex && xv_currentArrayId == currentArrayId)
    {
        return;
    }

    emit xg_currentOperation(OT_BEGIN_RESET, -1, -1);
    xv_currentArrayIndex = currentArrayIndex;
    xv_currentArrayId = currentArrayId;
    xh_defineColumnCounts();
    xh_calculateCalibrationConcentrations();
    emit xg_currentOperation(OT_END_RESET, -1, -1);
}
//==================================================================
void X_JointSpectraDataManager::xh_currentChemConcentrationCellIndex(int& row, int& chemConcColumn)
{
    QModelIndex currentIndex;
    emit xg_requestCurrentIndex(currentIndex);

    if(!currentIndex.isValid())
    {
        row = -1;
        chemConcColumn = -1;
        return;
    }

    // row without changing
    row = currentIndex.row();

    // column - index of visible. if out of chem element range then -1
    chemConcColumn = currentIndex.column() - xv_spectrumDataColumnCount;
    if(chemConcColumn > xv_visibleChemElementCount - 1)
    {
        chemConcColumn = -1;
    }
}
//==================================================================
void X_JointSpectraDataManager::xp_calculateCalibrationQualityData(bool saveToCalibration,
                                                                  qint64 calibrationId,
                                                                  int factorCount,
                                                                  qreal summSquareAverageConcentrationDispersion) const
{
    X_CalibrationQualityData qualityData;
    if(calibrationId < 0)
    {
        return;
    }

    QString chemElement = xv_calibrationRepository->xp_chemElementForCalibrationId(calibrationId);
    qint64 calibrationChemElementId = xv_spectrumArrayRepository->xp_chemElementIdForName(xv_currentArrayIndex, chemElement);

    if(calibrationChemElementId < 0)
    {
        emit xg_calibrationQualityData(saveToCalibration, calibrationId, qualityData);
        return;
    }

    int checkedSpectrumCount = 0;
    const X_AbstractSpectrum* spectrum;
    QStringList calibrationConcentrationList = xv_calibrationConcentrationMap.value(calibrationId);
    if(calibrationConcentrationList.isEmpty() || calibrationConcentrationList.count() != xv_spectrumArrayRepository->xp_spectrumCount(xv_currentArrayIndex))
    {
        emit xg_calibrationQualityData(saveToCalibration, calibrationId, qualityData);
        return;
    }

    qreal calibrationConcentration;
    qreal chemConcentration;
    qreal summSquareConcentrationDispersion = 0.0;

    bool ok;
    for(int s = 0; s < xv_spectrumArrayRepository->xp_spectrumCount(xv_currentArrayIndex); s++)
    {
        spectrum = xv_spectrumArrayRepository->xp_spectrum(xv_currentArrayIndex, s);
        if(spectrum == nullptr || !spectrum->xp_isSpectrumChecked())
        {
            continue;
        }

        checkedSpectrumCount++;
        // chemConc
        chemConcentration = spectrum->xp_concentrationValue(calibrationChemElementId);
        // calibr conc

        calibrationConcentration = X_LocaleDoubleConverter::toDouble(calibrationConcentrationList.at(s), &ok);
        if(!ok)
        {
            emit xg_calibrationQualityData(saveToCalibration, calibrationId, qualityData);
            return;
        }

        summSquareConcentrationDispersion += pow(chemConcentration - calibrationConcentration, 2.0);
    }

    if(checkedSpectrumCount <= 1)
    {
        emit xg_calibrationQualityData(saveToCalibration, calibrationId, qualityData);
        return;
    }

    // dispersion sigma square
    qreal squareSigma = summSquareConcentrationDispersion / checkedSpectrumCount;
    qualityData.squareSigma = QString::number(squareSigma);

    // standard deviation
    qreal standardDeviation = sqrt(summSquareConcentrationDispersion / (checkedSpectrumCount - 1));
    qualityData.standardDeviation = QString::number(standardDeviation);

    // R2 determination
    if(summSquareAverageConcentrationDispersion == 0.0)
    {
        //
        qualityData.determination = "Error";
        qualityData.adj_determination = "Error";
        emit xg_calibrationQualityData(saveToCalibration, calibrationId, qualityData);
        return;
    }

    qreal determination = 1 - (summSquareConcentrationDispersion / summSquareAverageConcentrationDispersion);
    qualityData.determination = QString::number(determination);

    // R2adj determination
    qreal denominator = checkedSpectrumCount - factorCount - 1;
    if(denominator == 0.0)
    {
        qualityData.adj_determination = "Error";
        emit xg_calibrationQualityData(saveToCalibration, calibrationId, qualityData);
        return;
    }

    qreal adjDetermination = 1 - (1 - determination)*( (checkedSpectrumCount - 1)/(denominator) );
    qualityData.adj_determination = QString::number(adjDetermination);
    emit xg_calibrationQualityData(saveToCalibration, calibrationId, qualityData);

}
//==================================================================
void X_JointSpectraDataManager::xh_onRepositoryArrayOperation(X_SpectrumArrayRepository::SpectrumOperationType type,
                                                             int arrayIndex, int first, int last)
{
    xh_defineColumnCounts();
    if(xv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    if(type == X_SpectrumArrayRepository::SOT_BEGIN_INSERT_SPECTRA)
    {
        emit xg_currentOperation(OT_BEGIN_INSERT_ROW, first, last);
    }
    else if(type == X_SpectrumArrayRepository::SOT_END_INSERT_SPECTRA)
    {
        emit xg_currentOperation(OT_END_INSERT_ROW, first, last);
        xh_calculateCalibrationConcentrations();
    }
    else if(type == X_SpectrumArrayRepository::SOT_BEGIN_REMOVE_SPECTRA)
    {
        emit xg_currentOperation(OT_BEGIN_REMOVE_ROW, first, last);
    }
    else if(type == X_SpectrumArrayRepository::SOT_END_REMOVE_SPECTRA)
    {
        emit xg_currentOperation(OT_END_REMOVE_ROW, first, last);
        xh_calculateCalibrationConcentrations();
    }
    else if(type == X_SpectrumArrayRepository::SOT_VISIBLE_CHANGED)
    {
        emit xg_currentOperation(OT_SEPECTRUM_VISIBLE_CHANGED, first, last);
    }
    else if(type == X_SpectrumArrayRepository::SOT_CONCENTRATION_CHANGED)
    {
        emit xg_currentOperation(OT_SEPECTRUM_CONCENTRATION_CHANGED, first, last);
    }

}
//==================================================================
void X_JointSpectraDataManager::xh_onRepositoryChemElementOperation(X_SpectrumArrayRepository::ChemElementOperationType type,
                                                                   int arrayIndex, int first, int last)
{
    xh_defineColumnCounts();
    if(xv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    int residual = last - first;
    int visibleFirst = xh_convertChemElementIndexToVisibleChemElementIndex(xv_currentArrayIndex, first);

    if(visibleFirst < 0)
    {
        visibleFirst = 0;
    }

    int visibleLast = visibleFirst + residual;

    visibleFirst += xv_spectrumDataColumnCount;
    visibleLast += xv_spectrumDataColumnCount;

    if(type == X_SpectrumArrayRepository::CEOT_INSERT_CHEM_ELEMENT)
    {
        emit xg_currentOperation(OT_BEGIN_INSERT_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == X_SpectrumArrayRepository::CEOT_END_INSERT_CHEM_ELEMENT)
    {
        emit xg_currentOperation(OT_END_INSERT_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == X_SpectrumArrayRepository::CEOT_REMOVE_CHEM_ELEMENT)
    {
        emit xg_currentOperation(OT_BEGIN_REMOVE_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == X_SpectrumArrayRepository::CEOT_END_REMOVE_CHEM_ELEMENT)
    {
        emit xg_currentOperation(OT_END_REMOVE_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == X_SpectrumArrayRepository::CEOT_CHEM_ELEMENT_CHANGED)
    {
        emit xg_currentOperation(OT_COLUMN_HEADER_CHANGED, visibleFirst, visibleLast);
    }
    else if(type == X_SpectrumArrayRepository::CEOT_CHEM_ELEMENT_VISIBILITY_CHANGE)
    {
        bool visible = xv_spectrumArrayRepository->xp_chemElementIsVisible(xv_currentArrayIndex, first);
        if(!visible)
        {
            emit xg_currentOperation(OT_BEGIN_INSERT_COLUMN, visibleFirst, visibleLast);
        }
        else
        {
            emit xg_currentOperation(OT_BEGIN_REMOVE_COLUMN, visibleFirst, visibleLast);
        }
    }
    else if(type == X_SpectrumArrayRepository::CEOT_END_CHEM_ELEMENT_VISIBILITY_CHANGE)
    {
        bool visible = xv_spectrumArrayRepository->xp_chemElementIsVisible(xv_currentArrayIndex, first);
        if(visible)
        {
            emit xg_currentOperation(OT_END_INSERT_COLUMN, visibleFirst, visibleLast);
        }
        else
        {
            emit xg_currentOperation(OT_END_REMOVE_COLUMN, visibleFirst, visibleLast);
        }
    }
}
//==================================================================
void X_JointSpectraDataManager::xh_onRepositoryTermOperation(X_CalibrationRepository::TermOperationType type,
                                                            int calibrationIndex, int first, int last)
{
    if(!xv_calibrationRepository)
    {
        return;
    }

    if(!xv_calibrationRepository->xp_isCalibrationVisible(calibrationIndex))
    {
        return;
    }

    int visibleFirst;
    int visibleLast;
    if(!xh_getVisibleIndexesForOperation(calibrationIndex, calibrationIndex, visibleFirst, visibleLast))
    {
        return;
    }

    if(type == X_CalibrationRepository::TOT_TERM_WINDOW_MARGIN_CHANGED ||
            type == X_CalibrationRepository::TOT_TERM_STATE_CHANGED ||
            type == X_CalibrationRepository::TOT_TERM_FACTOR_CHANGED ||
            type == X_CalibrationRepository::TOT_END_REMOVE_TERM ||
            type == X_CalibrationRepository::TOT_END_INSERT_TERM ||
            type == X_CalibrationRepository::TOT_END_RESET)
    {
        xh_calculateCalibrationConcentrationForCalibration(calibrationIndex);

        emit xg_currentOperation(OT_COLUMN_DATA_CHANGED, visibleFirst, visibleLast);
    }
}
//==================================================================
void X_JointSpectraDataManager::xh_onRepositoryCalibrationOperation(X_CalibrationRepository::CalibrationOperationType type,
                                                                   int first, int last)
{
    if(!xv_calibrationRepository)
    {
        return;
    }

    xh_defineColumnCounts();

    int visibleFirst = -1;
    int visibleLast = -1;

    if(type == X_CalibrationRepository::COT_INSERT_CALIBRATIONS)
    {
        if(!xh_getVisibleIndexesForInsert(first, last, visibleFirst, visibleLast))
        {
            return;
        }
        emit xg_currentOperation(OT_BEGIN_INSERT_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == X_CalibrationRepository::COT_END_INSERT_CALIBRATIONS)
    {
        emit xg_currentOperation(OT_END_INSERT_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == X_CalibrationRepository::COT_REMOVE_CALIBRATIONS)
    {
        if(!xh_getVisibleIndexesForOperation(first, last, visibleFirst, visibleLast))
        {
            return;
        }

        emit xg_currentOperation(OT_BEGIN_REMOVE_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == X_CalibrationRepository::COT_END_REMOVE_CALIBRATIONS)
    {
        emit xg_currentOperation(OT_END_REMOVE_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == X_CalibrationRepository::COT_CALIBRATION_NAME_CHANGED)
    {
        if(!xh_getVisibleIndexesForOperation(first, last, visibleFirst, visibleLast))
        {
            return;
        }

        emit xg_currentOperation(OT_COLUMN_HEADER_CHANGED, visibleFirst, visibleLast);
    }
    else if(type == X_CalibrationRepository::COT_CALIBRATION_VISIBILITY_CHANGE)
    {
        bool visible = xv_calibrationRepository->xp_isCalibrationVisible(first);

        if(!visible)
        {
            if(!xh_getVisibleIndexesForInsert(first, last, visibleFirst, visibleLast))
            {
                return;
            }

            emit xg_currentOperation(OT_BEGIN_INSERT_COLUMN, visibleFirst, visibleLast);
        }
        else
        {
            if(!xh_getVisibleIndexesForOperation(first, last, visibleFirst, visibleLast))
            {
                return;
            }

            emit xg_currentOperation(OT_BEGIN_REMOVE_COLUMN, visibleFirst, visibleLast);
        }
    }
    else if(type == X_CalibrationRepository::COT_END_CALIBRATION_VISIBILITY_CHANGE)
    {
        bool visible = xv_calibrationRepository->xp_isCalibrationVisible(first);

        if(visible)
        {
            emit xg_currentOperation(OT_END_INSERT_COLUMN, visibleFirst, visibleLast);
        }
        else
        {
            emit xg_currentOperation(OT_END_REMOVE_COLUMN, visibleFirst, visibleLast);
        }
    }
    else if(type == X_CalibrationRepository::COT_CALIBRATION_EQUATION_TYPE_CHANGED ||
            type == X_CalibrationRepository::COT_CALIBRATION_FREE_MEMBER_CHANGED ||
            type == X_CalibrationRepository::COT_CALIBRATION_EQUATION_BASE_TERM_CHANGED
            )
    {
        if(!xh_getVisibleIndexesForOperation(first, last, visibleFirst, visibleLast))
        {
            return;
        }

        int calibrationIndex;
        for(int v = visibleFirst; v <= visibleLast; v++)
        {
            calibrationIndex = xh_convertVisibleIndexToCalibrationIndex(v);
            xh_calculateCalibrationConcentrationForCalibration(calibrationIndex);
            emit xg_currentOperation(OT_COLUMN_DATA_CHANGED, visibleFirst, visibleLast);
        }
    }
}
//==================================================================
int X_JointSpectraDataManager::xh_convertVisibleIndexToCalibrationIndex(int visibleIndex)
{
    if(visibleIndex < xv_spectrumDataColumnCount + xv_visibleChemElementCount ||
            visibleIndex >= xv_spectrumDataColumnCount +
            xv_visibleChemElementCount + xv_visibleCalibrationCount ||
            xv_calibrationRepository == nullptr)
    {
        return -1;
    }


    int visibleCalibrationIndex = visibleIndex - xv_spectrumDataColumnCount - xv_visibleChemElementCount;
    if(visibleCalibrationIndex >= 0
            && visibleCalibrationIndex < xv_calibrationRepository->xp_visibleCalibrationCount())
    {
        return xv_calibrationRepository->xp_calibrationIndexForVisibleIndex(visibleCalibrationIndex);
    }

    return -1;
}
//==================================================================
bool X_JointSpectraDataManager::xh_getVisibleIndexesForInsert(int first, int last, int& visibleFirst, int& visibleLast) const
{
    int visibleBeforeFirst = 0;
    for(int i = 0; i < first; i++)
    {
        if(!xv_calibrationRepository->xp_isCalibrationVisible(i))
        {
            continue;
        }

        visibleBeforeFirst++;
    }

    visibleFirst = xv_spectrumDataColumnCount + xv_visibleChemElementCount + visibleBeforeFirst;
    visibleLast = visibleFirst + (last - first);
    return true;
}
//==================================================================
bool X_JointSpectraDataManager::xh_getVisibleIndexesForOperation(int first, int last, int& visibleFirst, int& visibleLast) const
{
    int visibleBeforeFirst = 0;
    int visibleInInterval = 0;
    for(int i = 0; i <= last; i++)
    {
        if(!xv_calibrationRepository->xp_isCalibrationVisible(i))
        {
            continue;
        }

        if(i < first)
        {
            visibleBeforeFirst++;
        }
        else
        {
            visibleInInterval++;
        }
    }

    if(visibleInInterval <= 0)
    {
        return false;
    }

    visibleFirst = xv_spectrumDataColumnCount + xv_visibleChemElementCount + visibleBeforeFirst;
    visibleLast = visibleFirst + visibleInInterval - 1;
    return true;
}
//==================================================================
int X_JointSpectraDataManager::xh_convertChemElementIndexToVisibleChemElementIndex(int arrayIndex,
                                                                                  int originalIndex)
{
    if(xv_spectrumArrayRepository == nullptr)
    {
        return -1;
    }

    return xv_spectrumArrayRepository->xp_numberVisibleChemElementsBeforeIndex(arrayIndex, originalIndex);
}
//==================================================================
void X_JointSpectraDataManager::xh_defineColumnCounts()
{
    if(xv_spectrumArrayRepository != nullptr && xv_currentArrayIndex >= 0)
    {
        xv_visibleChemElementCount = xv_spectrumArrayRepository->xp_visibleChemElementCount(xv_currentArrayIndex);
    }
    else
    {
        xv_visibleChemElementCount = 0;
    }

    if(xv_calibrationRepository != nullptr)
    {
        xv_visibleCalibrationCount = xv_calibrationRepository->xp_visibleCalibrationCount();
    }
    else
    {
        xv_visibleCalibrationCount = 0;
    }
}
//==================================================================
void X_JointSpectraDataManager::xh_calculateCalibrationConcentrations()
{
    xv_calibrationConcentrationMap.clear();

    if(xv_spectrumArrayRepository == nullptr || xv_calibrationRepository == nullptr
            || xv_currentArrayIndex < 0 || xv_currentArrayIndex >= xv_spectrumArrayRepository->xp_arrayCount())
    {
        return;
    }

    if(xv_spectrumArrayRepository->xp_spectrumCount(xv_currentArrayIndex) <= 0
            || xv_calibrationRepository->xp_calibrationCount() <= 0)
    {
        return;
    }

    for(int c = 0; c < xv_calibrationRepository->xp_calibrationCount(); c++)
    {
        xh_calculateCalibrationConcentrationForCalibration(c);
    }
}
//==================================================================
bool X_JointSpectraDataManager::xh_calculateCalibrationConcentrationForCalibration(int calibrationIndex)
{
    qint64 calibrationId = xv_calibrationRepository->xp_calibrationIdForCalibrationIndex(calibrationIndex);
    if(calibrationId < 0)
    {
        return false;
    }

    return xh_calculateCalibrationConcentrationForCalibrationId(calibrationId);
}
//==================================================================
bool X_JointSpectraDataManager::xh_calculateCalibrationConcentrationForCalibrationId(qint64 calibrationId)
{
    if(calibrationId < 0 )
    {
        return false;
    }

    QStringList concentrationList;
    //QString chemElementName = xv_calibrationRepository->xp_chemElementForCalibrationId(calibrationId);
    //qint64 chemElementId = xv_spectrumArrayRepository->xp_chemElementIdForName(xv_currentArrayId, chemElementName);
    qreal concentration;
    const X_AbstractSpectrum* spectrum;
    bool res;
    for(int s = 0; s < xv_spectrumArrayRepository->xp_spectrumCount(xv_currentArrayIndex); s++)
    {
        spectrum = xv_spectrumArrayRepository->xp_spectrum(xv_currentArrayIndex, s);
        if(!spectrum)
        {
            concentrationList.append(QString("N/A"));
            //residualList.append(std::numeric_limits::quiet_NaN());
            continue;
        }
        // calculate calibration concentration value
        res = xv_calibrationRepository->xp_calculateConcentrationForId(calibrationId, spectrum, concentration);
        if(res)
        {
            concentrationList.append(QString::number(concentration, xv_concentrationFormat, xv_concentrationPrecision));
            // get chem concentration
            // calc residual and put it in residualList
            //residualList.append(spectrum->xp_concentrationValue(chemElementId) - concentration);
        }
        else
        {
            concentrationList.append(QString("N/A"));
            // put in residual list nan value
            //residualList.append(std::numeric_limits::quiet_NaN());
        }
    }

    xv_calibrationConcentrationMap.insert(calibrationId, concentrationList);
    //xv_concentrationResidualMap.insert(calibrationId, residualList);
    emit xg_calibrationValuesChanged(calibrationId);
    return true;
}
//==================================================================
bool X_JointSpectraDataManager::xp_calculateConcentrationResidualList(qint64 calibrationId,
                                                                     QList<qreal>&residualDispersionList) const
{
    residualDispersionList.clear();
    QString chemElementName = xv_calibrationRepository->xp_chemElementForCalibrationId(calibrationId);
    qint64 chemElementId = xv_spectrumArrayRepository->xp_chemElementIdForName(xv_currentArrayIndex, chemElementName);

    if(chemElementId < 0)
    {
        return false;
    }

    qreal concentration = 0.0;
    const X_AbstractSpectrum* spectrum = nullptr;
    bool res = false;
    qreal residual = 0.0;
//    qreal averageResidual = 0.0;
    int checkedSpectrumCount = 0;
    for(int s = 0; s < xv_spectrumArrayRepository->xp_spectrumCount(xv_currentArrayIndex); s++)
    {
        spectrum = xv_spectrumArrayRepository->xp_spectrum(xv_currentArrayIndex, s);
        if(!spectrum)
        {
            return false;
        }

        if(!spectrum->xp_isSpectrumChecked())
        {
            continue;
        }
        checkedSpectrumCount ++;
        // calculate calibration concentration value
        res = xv_calibrationRepository->xp_calculateConcentrationForId(calibrationId, spectrum, concentration);

        //        res = xv_calibrationRepository->xp_calculateActiveTermValueSumForId(calibrationId,
        //                                                                       spectrum,
        //                                                                       termIntensitySum);

        if(res)
        {
            // get chem concentration
            //calc residual and put it in residualList
            residual = spectrum->xp_concentrationValue(chemElementId) - concentration;

//            averageResidual += residual;
            residualDispersionList.append(residual);
        }
        else
        {
            return false;
        }
    }

    if(checkedSpectrumCount < 1)
    {
        return false;
    }

//    averageResidual /= static_cast<qreal>(checkedSpectrumCount);

//    for(int r = 0; r < residualDispersionList.count(); r++)
//    {
//        residualDispersionList[r] = residualDispersionList.at(r) - averageResidual;
//    }
    return true;
}
//==================================================================
