//=============================================================================
#include "ZCalibrationWindowDataManager.h"
#include "ZGeneral.h"
//=============================================================================
ZCalibrationWindowDataManager::ZCalibrationWindowDataManager(QObject *parent) : QObject(parent)
{
    zv_calibrationRepository = nullptr;
    zv_spectrumArrayRepositiry = nullptr;
    zv_currentCalibrationId = -1;
    zv_currentSpectrumArrayId = -1;
}
//=============================================================================
void ZCalibrationWindowDataManager::zp_connectToSpectraArrayRepository(ZSpectrumArrayRepository* repository)
{
    zv_spectrumArrayRepositiry = repository;
    connect(zv_spectrumArrayRepositiry, &ZSpectrumArrayRepository::zg_currentArrayIdChanged,
            this, &ZCalibrationWindowDataManager::zh_currentSpectrumArrayChanged);
    connect(zv_spectrumArrayRepositiry, &ZSpectrumArrayRepository::zg_arrayMaxParametersChanged,
            this, &ZCalibrationWindowDataManager::zh_onArrayMaxParametersChanged);
    connect(zv_spectrumArrayRepositiry, &ZSpectrumArrayRepository::zg_currentSpectrumChanged,
            this, &ZCalibrationWindowDataManager::zh_onCurrentSpectrumChange);

}
//=============================================================================
void ZCalibrationWindowDataManager::zp_connectToCalibrationRepository(ZCalibrationRepository* repository)
{
    zv_calibrationRepository = repository;
    connect(repository, &ZCalibrationRepository::zg_calibrationWindowOperation,
            this, &ZCalibrationWindowDataManager::zh_onRepositoryWindowOperation);
    connect(repository, &ZCalibrationRepository::zg_currentCalibrationChanged,
            this, &ZCalibrationWindowDataManager::zh_onCurrentCalibrationChange);
}
//=============================================================================
int ZCalibrationWindowDataManager::zp_rowCount() const
{
    if(zv_currentCalibrationId < 0 || zv_calibrationRepository == nullptr)
    {
        return 0;
    }

    return zv_calibrationRepository->zp_calibrationWindowCount(zv_currentCalibrationId);
}
//=============================================================================
int ZCalibrationWindowDataManager::zp_columnCount() const
{
    return 5;
    // return zv_spectrumDataColumnCount + zv_visibleChemElementCount + zv_visibleCalibrationCount;
}
//=============================================================================
void ZCalibrationWindowDataManager::zh_onArrayMaxParametersChanged(qint64 arrayId, int intensity, int channels)
{
    if(zv_currentSpectrumArrayId != arrayId)
    {
        return;
    }

    emit zg_setChannelMinMax(0, channels);
}
//=============================================================================
void ZCalibrationWindowDataManager::zh_onCurrentCalibrationChange(qint64 calibrationId, int calibrationIndex)
{
    if(zv_currentCalibrationId == calibrationId)
    {
        return;
    }

    zv_currentCalibrationId = calibrationId;

    emit zg_currentOperation(OT_RESET_DATA, -1, -1);

    emit zg_currentOperation(OT_END_RESET_DATA, -1, -1);
}
//=============================================================================
void ZCalibrationWindowDataManager::zh_onRepositoryWindowOperation(ZCalibrationRepository::WindowOperationType type,
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

    if(type == ZCalibrationRepository::WOT_BEGIN_RESET)
    {
        emit zg_currentOperation(OT_RESET_DATA, first, last);
    }
    else if(type == ZCalibrationRepository::WOT_END_RESET)
    {
        emit zg_currentOperation(OT_END_RESET_DATA, first, last);
    }
    else if(type == ZCalibrationRepository::WOT_INSERT_WINDOWS)
    {
        emit zg_currentOperation(OT_INSERT_ROW, first, last);
    }
    else if(type == ZCalibrationRepository::WOT_END_INSERT_WINDOWS)
    {
        emit zg_currentOperation(OT_END_INSERT_ROW, first, last);
    }
    else if(type == ZCalibrationRepository::WOT_REMOVE_WINDOWS)
    {
        emit zg_currentOperation(OT_REMOVE_ROW, first, last);
    }
    else if(type == ZCalibrationRepository::WOT_END_REMOVE_WINDOWS)
    {
        emit zg_currentOperation(OT_END_REMOVE_ROW, first, last);
    }
    else if(type == ZCalibrationRepository::WOT_WINDOW_CHANGED)
    {
        emit zg_currentOperation(OT_RESET_DATA, first, last);
    }
}
//=============================================================================
void ZCalibrationWindowDataManager::zh_currentSpectrumArrayChanged(qint64 arrayId, int arrayIndex)
{
    zv_currentSpectrumArrayId = arrayId;
    int channelCount = zv_spectrumArrayRepositiry->zp_arrayChannelCount(arrayIndex);
    emit zg_setChannelMinMax(0, channelCount);
    emit zg_currentOperation(OT_DATA_CHANGED, 0, zp_rowCount() - 1);
}
//=============================================================================
void ZCalibrationWindowDataManager::zh_onCurrentSpectrumChange(qint64, int, qint64, int)
{
    emit zg_currentOperation(OT_DATA_CHANGED, 0, zp_rowCount() - 1);
}
//=============================================================================
QString ZCalibrationWindowDataManager::zp_columnName(int columnIndex) const
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
        return tr("Intensity");
    }
    else if(columnIndex == 3)
    {
        return tr("First channel");
    }
    else if(columnIndex == 4)
    {
        return tr("Last channel");
    }
    else if(columnIndex == 5)
    {
        return tr("Simple factor");
    }
    else if(columnIndex == 6)
    {
        return tr("Square factor");
    }

    return QString();
}
//=============================================================================
QVariant ZCalibrationWindowDataManager::zp_data(QModelIndex index) const
{
    if(!zv_calibrationRepository || !index.isValid())
    {
        return QVariant();
    }

    if(index.column() == 0)
    {
        return QVariant(zv_calibrationRepository->zp_calibrationWindowName(zv_currentCalibrationId, index.row()));
    }
    if(index.column() == 1)
    {
        ZCalibrationWindow::WindowType type = zv_calibrationRepository->zp_calibrationWindowType(zv_currentCalibrationId, index.row());
        return QVariant(ZCalibrationWindow::zp_displayTypeName(type));
    }
    if(index.column() == 2)
    {
        QString intensityString;
        if(zv_spectrumArrayRepositiry)
        {
            int firstChannel = zv_calibrationRepository->zp_calibrationWindowFirstChannel(zv_currentCalibrationId, index.row());
            int lastChannel = zv_calibrationRepository->zp_calibrationWindowLastChannel(zv_currentCalibrationId, index.row());
            qreal intensity;
            // request to Spectrum array repo

            zv_spectrumArrayRepositiry->zp_currentSpectrumWindowIntensity(firstChannel, lastChannel, intensity);

            if(intensity == intensity) // val not nan
            {
                intensityString = QString::number(static_cast<qint64>(intensity));
            }
        }
        return QVariant(intensityString);
    }
    if(index.column() == 3)
    {
        return QVariant(zv_calibrationRepository->zp_calibrationWindowFirstChannel(zv_currentCalibrationId, index.row()));
    }
    if(index.column() == 4)
    {
        return QVariant(zv_calibrationRepository->zp_calibrationWindowLastChannel(zv_currentCalibrationId, index.row()));
    }

    return QVariant();
}
//=============================================================================
bool ZCalibrationWindowDataManager::zp_setData(QModelIndex index, QVariant data)
{
    if(!zv_calibrationRepository || !index.isValid() || !data.isValid())
    {
        return false;
    }

    if(index.column() == 0)
    {
        if(!data.canConvert<QString>())
        {
            return false;
        }
        return zv_calibrationRepository->zp_setCalibrationWindowName(zv_currentCalibrationId, index.row(), data.toString());
    }
    if(index.column() == 1)
    {
        if(!data.canConvert<QString>())
        {
            return false;
        }

        ZCalibrationWindow::WindowType type = ZCalibrationWindow::zp_typeFromString(data.toString());
        return zv_calibrationRepository->zp_setCalibrationWindowType(zv_currentCalibrationId, index.row(), type);
    }
    if(index.column() == 3)
    {
        if(!data.canConvert<int>())
        {
            return false;
        }

        return zv_calibrationRepository->zp_setCalibrationWindowFirstChannel(zv_currentCalibrationId, index.row(), data.toInt());
    }
    if(index.column() == 4)
    {
        if(!data.canConvert<int>())
        {
            return false;
        }

        return zv_calibrationRepository->zp_setCalibrationWindowLastChannel(zv_currentCalibrationId, index.row(), data.toInt());
    }

    return false;
}
//=============================================================================
QColor ZCalibrationWindowDataManager::zp_windowColor(int windowIndex) const
{
    if(!zv_calibrationRepository || windowIndex < 0 )
    {
        return QColor();
    }

    return zv_calibrationRepository->zp_calibrationWindowColor(zv_currentCalibrationId, windowIndex);
}
//=============================================================================
bool ZCalibrationWindowDataManager::zp_isWindowVisible(int windowIndex) const
{
    if(!zv_calibrationRepository || windowIndex < 0 )
    {
        return false;
    }

    return zv_calibrationRepository->zp_isCalibrationWindowVisible(zv_currentCalibrationId, windowIndex);
}
//=============================================================================
bool ZCalibrationWindowDataManager::zp_setWindowVisible(int windowIndex, bool visibility)
{
    if(!zv_calibrationRepository || windowIndex < 0 )
    {
        return false;
    }

    return zv_calibrationRepository->zp_setCalibrationWindowVisible(zv_currentCalibrationId, windowIndex, visibility);

}
//=============================================================================


