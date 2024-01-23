//=============================================================================
#include "X_CalibrationWindowDataManager.h"
#include "X_General.h"
//=============================================================================
X_CalibrationWindowDataManager::X_CalibrationWindowDataManager(QObject *parent) : QObject(parent)
{
    xv_calibrationRepository = nullptr;
    xv_spectrumArrayRepositiry = nullptr;
    xv_currentCalibrationId = -1;
    xv_currentSpectrumArrayId = -1;
}
//=============================================================================
void X_CalibrationWindowDataManager::xp_connectToSpectraArrayRepository(X_SpectrumArrayRepository* repository)
{
    xv_spectrumArrayRepositiry = repository;
    connect(xv_spectrumArrayRepositiry, &X_SpectrumArrayRepository::xg_currentArrayIdChanged,
            this, &X_CalibrationWindowDataManager::xh_currentSpectrumArrayChanged);
    connect(xv_spectrumArrayRepositiry, &X_SpectrumArrayRepository::xg_arrayMaxParametersChanged,
            this, &X_CalibrationWindowDataManager::xh_onArrayMaxParametersChanged);
    connect(xv_spectrumArrayRepositiry, &X_SpectrumArrayRepository::xg_currentSpectrumChanged,
            this, &X_CalibrationWindowDataManager::xh_onCurrentSpectrumChange);

}
//=============================================================================
void X_CalibrationWindowDataManager::xp_connectToCalibrationRepository(X_CalibrationRepository* repository)
{
    xv_calibrationRepository = repository;
    connect(repository, &X_CalibrationRepository::xg_calibrationWindowOperation,
            this, &X_CalibrationWindowDataManager::xh_onRepositoryWindowOperation);
    connect(repository, &X_CalibrationRepository::xg_currentCalibrationChanged,
            this, &X_CalibrationWindowDataManager::xh_onCurrentCalibrationChange);
}
//=============================================================================
int X_CalibrationWindowDataManager::xp_rowCount() const
{
    if(xv_currentCalibrationId < 0 || xv_calibrationRepository == nullptr)
    {
        return 0;
    }

    return xv_calibrationRepository->xp_calibrationWindowCount(xv_currentCalibrationId);
}
//=============================================================================
int X_CalibrationWindowDataManager::xp_columnCount() const
{
    return 5;
    // return xv_spectrumDataColumnCount + xv_visibleChemElementCount + xv_visibleCalibrationCount;
}
//=============================================================================
void X_CalibrationWindowDataManager::xh_onArrayMaxParametersChanged(qint64 arrayId, int intensity, int channels)
{
    if(xv_currentSpectrumArrayId != arrayId)
    {
        return;
    }

    emit xg_setChannelMinMax(0, channels);
}
//=============================================================================
void X_CalibrationWindowDataManager::xh_onCurrentCalibrationChange(qint64 calibrationId, int calibrationIndex)
{
    if(xv_currentCalibrationId == calibrationId)
    {
        return;
    }

    xv_currentCalibrationId = calibrationId;

    emit xg_currentOperation(OT_RESET_DATA, -1, -1);

    emit xg_currentOperation(OT_END_RESET_DATA, -1, -1);
}
//=============================================================================
void X_CalibrationWindowDataManager::xh_onRepositoryWindowOperation(X_CalibrationRepository::WindowOperationType type,
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

    if(type == X_CalibrationRepository::WOT_BEGIN_RESET)
    {
        emit xg_currentOperation(OT_RESET_DATA, first, last);
    }
    else if(type == X_CalibrationRepository::WOT_END_RESET)
    {
        emit xg_currentOperation(OT_END_RESET_DATA, first, last);
    }
    else if(type == X_CalibrationRepository::WOT_INSERT_WINDOWS)
    {
        emit xg_currentOperation(OT_INSERT_ROW, first, last);
    }
    else if(type == X_CalibrationRepository::WOT_END_INSERT_WINDOWS)
    {
        emit xg_currentOperation(OT_END_INSERT_ROW, first, last);
    }
    else if(type == X_CalibrationRepository::WOT_REMOVE_WINDOWS)
    {
        emit xg_currentOperation(OT_REMOVE_ROW, first, last);
    }
    else if(type == X_CalibrationRepository::WOT_END_REMOVE_WINDOWS)
    {
        emit xg_currentOperation(OT_END_REMOVE_ROW, first, last);
    }
    else if(type == X_CalibrationRepository::WOT_WINDOW_CHANGED)
    {
        emit xg_currentOperation(OT_RESET_DATA, first, last);
    }
}
//=============================================================================
void X_CalibrationWindowDataManager::xh_currentSpectrumArrayChanged(qint64 arrayId, int arrayIndex)
{
    xv_currentSpectrumArrayId = arrayId;
    int channelCount = xv_spectrumArrayRepositiry->xp_arrayChannelCount(arrayIndex);
    emit xg_setChannelMinMax(0, channelCount);
    emit xg_currentOperation(OT_DATA_CHANGED, 0, xp_rowCount() - 1);
}
//=============================================================================
void X_CalibrationWindowDataManager::xh_onCurrentSpectrumChange(qint64, int, qint64, int)
{
    emit xg_currentOperation(OT_DATA_CHANGED, 0, xp_rowCount() - 1);
}
//=============================================================================
QString X_CalibrationWindowDataManager::xp_columnName(int columnIndex) const
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
QVariant X_CalibrationWindowDataManager::xp_data(QModelIndex index) const
{
    if(!xv_calibrationRepository || !index.isValid())
    {
        return QVariant();
    }

    if(index.column() == 0)
    {
        return QVariant(xv_calibrationRepository->xp_calibrationWindowName(xv_currentCalibrationId, index.row()));
    }
    if(index.column() == 1)
    {
        X_CalibrationWindow::WindowType type = xv_calibrationRepository->xp_calibrationWindowType(xv_currentCalibrationId, index.row());
        return QVariant(X_CalibrationWindow::xp_displayTypeName(type));
    }
    if(index.column() == 2)
    {
        QString intensityString;
        if(xv_spectrumArrayRepositiry)
        {
            int firstChannel = xv_calibrationRepository->xp_calibrationWindowFirstChannel(xv_currentCalibrationId, index.row());
            int lastChannel = xv_calibrationRepository->xp_calibrationWindowLastChannel(xv_currentCalibrationId, index.row());
            qreal intensity;
            // request to Spectrum array repo

            xv_spectrumArrayRepositiry->xp_currentSpectrumWindowIntensity(firstChannel, lastChannel, intensity);

            if(intensity == intensity) // val not nan
            {
                intensityString = QString::number(static_cast<qint64>(intensity));
            }
        }
        return QVariant(intensityString);
    }
    if(index.column() == 3)
    {
        return QVariant(xv_calibrationRepository->xp_calibrationWindowFirstChannel(xv_currentCalibrationId, index.row()));
    }
    if(index.column() == 4)
    {
        return QVariant(xv_calibrationRepository->xp_calibrationWindowLastChannel(xv_currentCalibrationId, index.row()));
    }

    return QVariant();
}
//=============================================================================
bool X_CalibrationWindowDataManager::xp_setData(QModelIndex index, QVariant data)
{
    if(!xv_calibrationRepository || !index.isValid() || !data.isValid())
    {
        return false;
    }

    if(index.column() == 0)
    {
        if(!data.canConvert<QString>())
        {
            return false;
        }
        return xv_calibrationRepository->xp_setCalibrationWindowName(xv_currentCalibrationId, index.row(), data.toString());
    }
    if(index.column() == 1)
    {
        if(!data.canConvert<QString>())
        {
            return false;
        }

        X_CalibrationWindow::WindowType type = X_CalibrationWindow::xp_typeFromString(data.toString());
        return xv_calibrationRepository->xp_setCalibrationWindowType(xv_currentCalibrationId, index.row(), type);
    }
    if(index.column() == 3)
    {
        if(!data.canConvert<int>())
        {
            return false;
        }

        return xv_calibrationRepository->xp_setCalibrationWindowFirstChannel(xv_currentCalibrationId, index.row(), data.toInt());
    }
    if(index.column() == 4)
    {
        if(!data.canConvert<int>())
        {
            return false;
        }

        return xv_calibrationRepository->xp_setCalibrationWindowLastChannel(xv_currentCalibrationId, index.row(), data.toInt());
    }

    return false;
}
//=============================================================================
QColor X_CalibrationWindowDataManager::xp_windowColor(int windowIndex) const
{
    if(!xv_calibrationRepository || windowIndex < 0 )
    {
        return QColor();
    }

    return xv_calibrationRepository->xp_calibrationWindowColor(xv_currentCalibrationId, windowIndex);
}
//=============================================================================
bool X_CalibrationWindowDataManager::xp_isWindowVisible(int windowIndex) const
{
    if(!xv_calibrationRepository || windowIndex < 0 )
    {
        return false;
    }

    return xv_calibrationRepository->xp_isCalibrationWindowVisible(xv_currentCalibrationId, windowIndex);
}
//=============================================================================
bool X_CalibrationWindowDataManager::xp_setWindowVisible(int windowIndex, bool visibility)
{
    if(!xv_calibrationRepository || windowIndex < 0 )
    {
        return false;
    }

    return xv_calibrationRepository->xp_setCalibrationWindowVisible(xv_currentCalibrationId, windowIndex, visibility);

}
//=============================================================================


