//==================================================================
#include "ZJointSpectraDataManager.h"
#include "globalVariables.h"
#include "ZSpectrumPaintData.h"
//==================================================================
ZJointSpectraDataManager::ZJointSpectraDataManager(QObject *parent) : QObject(parent)
{
    zv_currentArrayId = -1;
    zv_currentArrayIndex = -1;
    zv_spectrumArrayRepository = 0;
    zv_calibrationRepository = 0;
    zv_visibleChemElementCount = 0;
    zv_visibleCalibrationCount = 0;

    zv_concentrationFormat = 'g';
    zv_concentrationPrecision = 6;
}
//==================================================================
void ZJointSpectraDataManager::zp_connectToSpectrumArrayRepository(ZSpectrumArrayRepository* repository)
{
    zv_spectrumArrayRepository = repository;
    // array repository <-> array model
    connect(repository, &ZSpectrumArrayRepository::zg_spectrumOperation,
            this, &ZJointSpectraDataManager::zh_onRepositoryArrayOperation);
    connect(repository, &ZSpectrumArrayRepository::zg_chemElementOperation,
            this, &ZJointSpectraDataManager::zh_onRepositoryChemElementOperation);
    connect(repository, &ZSpectrumArrayRepository::zg_currentArrayIdChanged,
            this, &ZJointSpectraDataManager::zh_currentSpectrumArrayChanged);

}
//==================================================================
void ZJointSpectraDataManager::zp_connectToCalibrationRepository(ZCalibrationRepository* repository)
{
    zv_calibrationRepository = repository;
    connect(repository, &ZCalibrationRepository::zg_calibrationOperation,
            this, &ZJointSpectraDataManager::zh_onRepositoryCalibrationOperation);
    connect(repository, &ZCalibrationRepository::zg_termOperation,
            this, &ZJointSpectraDataManager::zh_onRepositoryTermOperation);

}
//==================================================================
int ZJointSpectraDataManager::zp_rowCount() const
{
    if(zv_currentArrayIndex < 0 || zv_spectrumArrayRepository == 0)
    {
        return 0;
    }

    return zv_spectrumArrayRepository->zp_spectrumCount(zv_currentArrayIndex);
}
//==================================================================
int ZJointSpectraDataManager::zp_columnCount() const
{
    return zv_spectrumDataColumnCount + zv_visibleChemElementCount + zv_visibleCalibrationCount;
}
//==================================================================
int ZJointSpectraDataManager::zp_visibleChemElementCount() const
{
    if(zv_currentArrayIndex < 0 || zv_spectrumArrayRepository == 0)
    {
        return 0;
    }

    return zv_spectrumArrayRepository->zp_visibleChemElementCount(zv_currentArrayIndex);
}
//==================================================================
bool ZJointSpectraDataManager::zp_isColumnChemElement(int column) const
{
    if(zv_spectrumArrayRepository == 0)
    {
        return false;
    }

    if(column < zv_spectrumDataColumnCount
            || column > zv_spectrumArrayRepository->zp_visibleChemElementCount(zv_currentArrayIndex)
            + zv_spectrumDataColumnCount - 1)
    {
        return false;
    }

    return true;
}
//==================================================================
QVariant ZJointSpectraDataManager::zp_data(QModelIndex index) const
{
    if(!zv_spectrumArrayRepository || !index.isValid())
    {
        return QVariant();
    }

    if(index.column() == 0)
    {
        return QVariant(zv_spectrumArrayRepository->zp_spectrumName(zv_currentArrayIndex, index.row()));
    }
    if(index.column() == 1)
    {
        ZSpectrumPaintData paintData;
        paintData.spectrumData = zv_spectrumArrayRepository->zp_spectrumData(zv_currentArrayIndex, index.row());
        paintData.maxChannel = zv_spectrumArrayRepository->zp_arrayChannelCount(zv_currentArrayIndex);
        paintData.maxIntensity = zv_spectrumArrayRepository->zp_arrayMaxIntensity(zv_currentArrayIndex);
        return QVariant::fromValue(paintData);
    }

    // int visibleChemElementCount = zv_spectrumArrayRepositiry->zp_visibleChemElementCount(zv_currentArrayIndex);
    if(index.column() >= zv_spectrumDataColumnCount
            && index.column() < zv_visibleChemElementCount + zv_spectrumDataColumnCount)
    {
        QString concentrationString = zv_spectrumArrayRepository->zp_chemConcentration(zv_currentArrayIndex,
                                                                                       index.row(),
                                                                                       index.column() - zv_spectrumDataColumnCount);
        return QVariant(concentrationString);
    }

    if(index.column() >= zv_spectrumDataColumnCount + zv_visibleChemElementCount
            && index.column() < zv_spectrumDataColumnCount + zv_visibleChemElementCount + zv_visibleCalibrationCount)
    {
        //        QString calibrationString = zv_calibrationRepository->zp_calibrationName(index.column()
        //                                                                                 - zv_spectrumDataColumnCount - zv_visibleChemElementCount);

        int visibleCalibrationIndex = index.column() - zv_spectrumDataColumnCount - zv_visibleChemElementCount;
        qint64 calibrationId = zv_calibrationRepository->zp_visibleCalibrationId(visibleCalibrationIndex);

        if(!zv_calibrationConcentrationMap.keys().contains(calibrationId))
        {
            return QVariant();
        }

        QString str = zv_calibrationConcentrationMap.value(calibrationId).value(index.row());
        return str; // zv_calculatedConcentrationMap.value(calibrationString).value(index.row());
    }

    return QVariant();
}
//==================================================================
QString ZJointSpectraDataManager::zp_columnName(int columnIndex) const
{
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
        if(zv_spectrumArrayRepository == 0)
        {
            return QString();
        }

        int visibleChemElementIndex = columnIndex - zv_spectrumDataColumnCount;
        if(visibleChemElementIndex >= 0
                && visibleChemElementIndex < zv_spectrumArrayRepository->zp_visibleChemElementCount(zv_currentArrayIndex))
        {
            return zv_spectrumArrayRepository->zp_visibleChemElementName(zv_currentArrayIndex, visibleChemElementIndex);
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
bool ZJointSpectraDataManager::zp_isSpectrumVisible(int spectrumIndex) const
{
    if(!zv_spectrumArrayRepository || zv_currentArrayIndex < 0 )
    {
        return false;
    }

    return zv_spectrumArrayRepository->zp_isSpectrumVisible(zv_currentArrayIndex, spectrumIndex);
}
//==================================================================
QColor ZJointSpectraDataManager::zp_spectrumColor(int spectrumIndex) const
{
    if(!zv_spectrumArrayRepository || zv_currentArrayIndex < 0 )
    {
        return QColor();
    }

    return zv_spectrumArrayRepository->zp_spectrumColor(zv_currentArrayIndex, spectrumIndex);
}
//==================================================================
bool ZJointSpectraDataManager::zp_setSpectrumVisible(int spectrumIndex, bool visible)
{
    if(!zv_spectrumArrayRepository || zv_currentArrayIndex < 0 )
    {
        return false;
    }

    return zv_spectrumArrayRepository->zp_setSpectrumVisible(zv_currentArrayIndex, spectrumIndex, visible);
}
//==================================================================
bool ZJointSpectraDataManager::zp_isSpectrumChecked(int spectrumIndex) const
{
    if(!zv_spectrumArrayRepository)
    {
        return false;
    }

    return zv_spectrumArrayRepository->zp_isSpectrumChecked(zv_currentArrayIndex, spectrumIndex);
}
//==================================================================
bool ZJointSpectraDataManager::zp_setSpectrumChecked(int spectrumIndex, bool checked)
{
    if(!zv_spectrumArrayRepository || zv_currentArrayIndex < 0 )
    {
        return false;
    }

    return zv_spectrumArrayRepository->zp_setSpectrumChecked(zv_currentArrayIndex, spectrumIndex, checked);
}
//==================================================================
bool ZJointSpectraDataManager::zp_setChemConcentration(int row, int column, const QString& consentration)
{
    if(!zv_spectrumArrayRepository || !zp_isColumnChemElement(column)
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

    return zv_spectrumArrayRepository->zp_setChemConcentration(zv_currentArrayIndex,
                                                               row,
                                                               column - zv_spectrumDataColumnCount,
                                                               consentration);
}
//==================================================================
int ZJointSpectraDataManager::zp_spectrumDataColumnCount()
{
    return zv_spectrumDataColumnCount;
}
//==================================================================
void ZJointSpectraDataManager::zh_currentSpectrumArrayChanged(qint64 currentArrayId, int currentArrayIndex)
{
    if(zv_currentArrayIndex == currentArrayIndex && zv_currentArrayId == currentArrayId)
    {
        return;
    }

    emit zg_currentOperation(OT_BEGIN_RESET, -1, -1);
    zv_currentArrayIndex = currentArrayIndex;
    zv_currentArrayId = currentArrayId;
    zh_defineColumnCounts();
    zh_calculateCalibrationConcentrations();
    emit zg_currentOperation(OT_END_RESET, -1, -1);
}
//==================================================================
void ZJointSpectraDataManager::zh_onRepositoryArrayOperation(ZSpectrumArrayRepository::SpectrumOperationType type,
                                                             int arrayIndex, int first, int last)
{
    zh_defineColumnCounts();
    if(zv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    if(type == ZSpectrumArrayRepository::SOT_BEGIN_INSERT_SPECTRA)
    {
        emit zg_currentOperation(OT_BEGIN_INSERT_ROW, first, last);
    }
    else if(type == ZSpectrumArrayRepository::SOT_END_INSERT_SPECTRA)
    {
        emit zg_currentOperation(OT_END_INSERT_ROW, first, last);
    }
    else if(type == ZSpectrumArrayRepository::SOT_BEGIN_REMOVE_SPECTRA)
    {
        emit zg_currentOperation(OT_BEGIN_REMOVE_ROW, first, last);
    }
    else if(type == ZSpectrumArrayRepository::SOT_END_REMOVE_SPECTRA)
    {
        emit zg_currentOperation(OT_END_REMOVE_ROW, first, last);
    }
    else if(type == ZSpectrumArrayRepository::SOT_VISIBLE_CHANGED)
    {
        emit zg_currentOperation(OT_SEPECTRUM_VISIBLE_CHANGED, first, last);
    }
}
//==================================================================
void ZJointSpectraDataManager::zh_onRepositoryChemElementOperation(ZSpectrumArrayRepository::ChemElementOperationType type,
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

    if(type == ZSpectrumArrayRepository::CEOT_INSERT_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_BEGIN_INSERT_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_END_INSERT_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_END_INSERT_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_REMOVE_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_BEGIN_REMOVE_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_END_REMOVE_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_END_REMOVE_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_CHEM_ELEMENT_CHANGED)
    {
        emit zg_currentOperation(OT_COLUMN_HEADER_CHANGED, visibleFirst, visibleLast);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_CHEM_ELEMENT_VISIBILITY_CHANGE)
    {
        bool visible = zv_spectrumArrayRepository->zp_chemElementIsVisible(zv_currentArrayIndex, first);
        if(!visible)
        {
            emit zg_currentOperation(OT_BEGIN_INSERT_COLUMN, visibleFirst, visibleLast);
        }
        else
        {
            emit zg_currentOperation(OT_BEGIN_REMOVE_COLUMN, visibleFirst, visibleLast);
        }
    }
    else if(type == ZSpectrumArrayRepository::CEOT_END_CHEM_ELEMENT_VISIBILITY_CHANGE)
    {
        bool visible = zv_spectrumArrayRepository->zp_chemElementIsVisible(zv_currentArrayIndex, first);
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
void ZJointSpectraDataManager::zh_onRepositoryTermOperation(ZCalibrationRepository::TermOperationType type,
                                                            int calibrationIndex, int first, int last)
{
    if(!zv_calibrationRepository)
    {
        return;
    }

    if(!zv_calibrationRepository->zp_isCalibrationVisible(calibrationIndex))
    {
        return;
    }

    int visibleFirst;
    int visibleLast;
    if(!zh_getVisibleIndexesForOperation(calibrationIndex, calibrationIndex, visibleFirst, visibleLast))
    {
        return;
    }

    if(type == ZCalibrationRepository::TOT_TERM_WINDOW_MARGIN_CHANGED ||
            type == ZCalibrationRepository::TOT_TERM_STATE_CHANGED ||
            type == ZCalibrationRepository::TOT_TERM_FACTOR_CHANGED ||
            type == ZCalibrationRepository::TOT_END_REMOVE_TERM ||
            type == ZCalibrationRepository::TOT_END_INSERT_TERM ||
            type == ZCalibrationRepository::TOT_END_RESET)
    {
        zh_calculateCalibrationConcentrationForCalibration(calibrationIndex);
        emit zg_currentOperation(OT_COLUMN_DATA_CHANGED, visibleFirst, visibleLast);
    }
}
//==================================================================
void ZJointSpectraDataManager::zh_onRepositoryCalibrationOperation(ZCalibrationRepository::CalibrationOperationType type,
                                                                   int first, int last)
{
    if(!zv_calibrationRepository)
    {
        return;
    }

    zh_defineColumnCounts();

    int visibleFirst = -1;
    int visibleLast = -1;

    if(type == ZCalibrationRepository::COT_INSERT_CALIBRATIONS)
    {
        if(!zh_getVisibleIndexesForInsert(first, last, visibleFirst, visibleLast))
        {
            return;
        }
        emit zg_currentOperation(OT_BEGIN_INSERT_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZCalibrationRepository::COT_END_INSERT_CALIBRATIONS)
    {
        emit zg_currentOperation(OT_END_INSERT_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZCalibrationRepository::COT_REMOVE_CALIBRATIONS)
    {
        if(!zh_getVisibleIndexesForOperation(first, last, visibleFirst, visibleLast))
        {
            return;
        }

        emit zg_currentOperation(OT_BEGIN_REMOVE_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZCalibrationRepository::COT_END_REMOVE_CALIBRATIONS)
    {
        emit zg_currentOperation(OT_END_REMOVE_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZCalibrationRepository::COT_CALIBRATION_NAME_CHANGED)
    {
        if(!zh_getVisibleIndexesForOperation(first, last, visibleFirst, visibleLast))
        {
            return;
        }

        emit zg_currentOperation(OT_COLUMN_HEADER_CHANGED, visibleFirst, visibleLast);
    }
    else if(type == ZCalibrationRepository::COT_CALIBRATION_VISIBILITY_CHANGE)
    {
        bool visible = zv_calibrationRepository->zp_isCalibrationVisible(first);

        if(!visible)
        {
            if(!zh_getVisibleIndexesForInsert(first, last, visibleFirst, visibleLast))
            {
                return;
            }

            emit zg_currentOperation(OT_BEGIN_INSERT_COLUMN, visibleFirst, visibleLast);
        }
        else
        {
            if(!zh_getVisibleIndexesForOperation(first, last, visibleFirst, visibleLast))
            {
                return;
            }

            emit zg_currentOperation(OT_BEGIN_REMOVE_COLUMN, visibleFirst, visibleLast);
        }
    }
    else if(type == ZCalibrationRepository::COT_END_CALIBRATION_VISIBILITY_CHANGE)
    {
        bool visible = zv_calibrationRepository->zp_isCalibrationVisible(first);

        if(visible)
        {
            emit zg_currentOperation(OT_END_INSERT_COLUMN, visibleFirst, visibleLast);
        }
        else
        {
            emit zg_currentOperation(OT_END_REMOVE_COLUMN, visibleFirst, visibleLast);
        }
    }
    else if(type == ZCalibrationRepository::COT_CALIBRATION_EQUATION_TYPE_CHANGED ||
            type == ZCalibrationRepository::COT_CALIBRATION_FREE_MEMBER_CHANGED ||
            type == ZCalibrationRepository::COT_CALIBRATION_EQUATION_BASE_TERM_CHANGED
            )
    {
        if(!zh_getVisibleIndexesForOperation(first, last, visibleFirst, visibleLast))
        {
            return;
        }

        int calibrationIndex;
        for(int v = visibleFirst; v <= visibleLast; v++)
        {
            calibrationIndex = zh_convertVisibleIndexToCalibrationIndex(v);
            zh_calculateCalibrationConcentrationForCalibration(calibrationIndex);
            emit zg_currentOperation(OT_COLUMN_DATA_CHANGED, visibleFirst, visibleLast);
        }
    }
}
//==================================================================
int ZJointSpectraDataManager::zh_convertVisibleIndexToCalibrationIndex(int visibleIndex)
{
    if(visibleIndex < zv_spectrumDataColumnCount + zv_visibleChemElementCount ||
            visibleIndex >= zv_spectrumDataColumnCount +
            zv_visibleChemElementCount + zv_visibleCalibrationCount ||
            zv_calibrationRepository == 0)
    {
        return -1;
    }


    int visibleCalibrationIndex = visibleIndex - zv_spectrumDataColumnCount - zv_visibleChemElementCount;
    if(visibleCalibrationIndex >= 0
            && visibleCalibrationIndex < zv_calibrationRepository->zp_visibleCalibrationCount())
    {
        return zv_calibrationRepository->zp_calibrationIndexForVisibleIndex(visibleCalibrationIndex);
    }

    return -1;
}
//==================================================================
bool ZJointSpectraDataManager::zh_getVisibleIndexesForInsert(int first, int last, int& visibleFirst, int& visibleLast) const
{
    int visibleBeforeFirst = 0;
    for(int i = 0; i < first; i++)
    {
        if(!zv_calibrationRepository->zp_isCalibrationVisible(i))
        {
            continue;
        }

        visibleBeforeFirst++;
    }

    visibleFirst = zv_spectrumDataColumnCount + zv_visibleChemElementCount + visibleBeforeFirst;
    visibleLast = visibleFirst + (last - first);
    return true;
}
//==================================================================
bool ZJointSpectraDataManager::zh_getVisibleIndexesForOperation(int first, int last, int& visibleFirst, int& visibleLast) const
{
    int visibleBeforeFirst = 0;
    int visibleInInterval = 0;
    for(int i = 0; i <= last; i++)
    {
        if(!zv_calibrationRepository->zp_isCalibrationVisible(i))
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

    visibleFirst = zv_spectrumDataColumnCount + zv_visibleChemElementCount + visibleBeforeFirst;
    visibleLast = visibleFirst + visibleInInterval - 1;
    return true;
}
//==================================================================
int ZJointSpectraDataManager::zh_convertChemElementIndexToVisibleChemElementIndex(int arrayIndex,
                                                                                  int originalIndex)
{
    if(zv_spectrumArrayRepository == 0)
    {
        return -1;
    }

    return zv_spectrumArrayRepository->zp_numberVisibleChemElementsBeforeIndex(arrayIndex, originalIndex);
}
//==================================================================
void ZJointSpectraDataManager::zh_defineColumnCounts()
{
    if(zv_spectrumArrayRepository != 0 && zv_currentArrayIndex >= 0)
    {
        zv_visibleChemElementCount = zv_spectrumArrayRepository->zp_visibleChemElementCount(zv_currentArrayIndex);
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
void ZJointSpectraDataManager::zh_calculateCalibrationConcentrations()
{
    zv_calibrationConcentrationMap.clear();

    if(zv_spectrumArrayRepository == 0 || zv_calibrationRepository == 0
            || zv_currentArrayIndex < 0 || zv_currentArrayIndex >= zv_spectrumArrayRepository->zp_arrayCount())
    {
        return;
    }

    if(zv_spectrumArrayRepository->zp_spectrumCount(zv_currentArrayIndex) <= 0
            || zv_calibrationRepository->zp_calibrationCount() <= 0)
    {
        return;
    }

    //    bool res;
    //    qint64 calibrationId;
    //    qreal concentration;
    for(int c = 0; c < zv_calibrationRepository->zp_calibrationCount(); c++)
    {
        //        calibrationId = zv_calibrationRepository->zp_calibrationIdForCalibrationIndex(c);
        //        QStringList concentrationList;
        //        for(int s = 0; s < zv_spectrumArrayRepository->zp_spectrumCount(zv_currentArrayIndex); s++)
        //        {
        //            res = zv_calibrationRepository->zp_calculateConcentration(c, zv_spectrumArrayRepository->zp_spectrum(zv_currentArrayIndex, s), concentration);
        //            if(res)
        //            {
        //                concentrationList.append(QString::number(concentration, zv_concentrationFormat, zv_concentrationPrecision));
        //            }
        //            else
        //            {
        //                concentrationList.append(QString("N/A"));
        //            }
        //        }
        //        zv_calibrationConcentrationMap.insert(calibrationId, concentrationList);
        zh_calculateCalibrationConcentrationForCalibration(c);
    }
}
//==================================================================
bool ZJointSpectraDataManager::zh_calculateCalibrationConcentrationForCalibration(int calibrationIndex)
{
    if(!zv_calibrationRepository || calibrationIndex < 0 || calibrationIndex >= zv_calibrationRepository->zp_calibrationCount())
    {
        return false;
    }

    qint64 calibrationId = zv_calibrationRepository->zp_calibrationIdForCalibrationIndex(calibrationIndex);
    if(calibrationId < 0)
    {
        return false;
    }

    QStringList concentrationList;
    qreal concentration;
    bool res;
    for(int s = 0; s < zv_spectrumArrayRepository->zp_spectrumCount(zv_currentArrayIndex); s++)
    {
        res = zv_calibrationRepository->zp_calculateConcentration(calibrationIndex, zv_spectrumArrayRepository->zp_spectrum(zv_currentArrayIndex, s), concentration);
        if(res)
        {
            concentrationList.append(QString::number(concentration, zv_concentrationFormat, zv_concentrationPrecision));
        }
        else
        {
            concentrationList.append(QString("N/A"));
        }
    }
    zv_calibrationConcentrationMap.insert(calibrationId, concentrationList);
    return true;
}
//==================================================================
bool ZJointSpectraDataManager::zh_calculateCalibrationConcentrationForCalibrationId(qint64 calibrationId)
{
    if(calibrationId < 0 )
    {
        return false;
    }

    QStringList concentrationList;
    qreal concentration;
    bool res;
    for(int s = 0; s < zv_spectrumArrayRepository->zp_spectrumCount(zv_currentArrayIndex); s++)
    {
        res = zv_calibrationRepository->zp_calculateConcentrationForId(calibrationId, zv_spectrumArrayRepository->zp_spectrum(zv_currentArrayIndex, s), concentration);
        if(res)
        {
            concentrationList.append(QString::number(concentration, zv_concentrationFormat, zv_concentrationPrecision));
        }
        else
        {
            concentrationList.append(QString("N/A"));
        }
    }
    zv_calibrationConcentrationMap.insert(calibrationId, concentrationList);
    return true;
}
//==================================================================
