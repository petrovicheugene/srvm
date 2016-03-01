//=============================================================================
#ifndef ZCALIBRATIONWINDOWDATAMANAGER_H
#define ZCALIBRATIONWINDOWDATAMANAGER_H
//=============================================================================
#include <QObject>
#include <QModelIndex>
#include "ZSpectrumArrayRepository.h"
#include "ZCalibrationRepository.h"
//=============================================================================

//=============================================================================
class ZCalibrationWindowDataManager : public QObject
{
    Q_OBJECT
public:
    explicit ZCalibrationWindowDataManager(QObject *parent = 0);

    enum OperationType {OT_RESET_DATA,
                        OT_END_RESET_DATA,
                        OT_INSERT_ROW,
                        OT_END_INSERT_ROW,
                        OT_REMOVE_ROW,
                        OT_END_REMOVE_ROW,
                        OT_INSERT_COLUMN,
                        OT_END_INSERT_COLUMN,
                        OT_REMOVE_COLUMN,
                        OT_END_REMOVE_COLUMN,
                        OT_DATA_CHANGED};

    // FUNCS
    void zp_connectToSpectraArrayRepository(ZSpectrumArrayRepository*);
    void zp_connectToCalibrationRepository(ZCalibrationRepository*);

    int zp_rowCount() const;
    int zp_columnCount() const;
    QString zp_columnName(int) const;
    QVariant zp_data(QModelIndex index) const;
    bool zp_setData(QModelIndex index, QVariant data);
    QColor zp_windowColor(int) const;
    bool zp_isWindowVisible(int) const;
    bool zp_setWindowVisible(int windowIndex, bool visibility);

signals:

    void zg_currentOperation(OperationType, int, int) const;
    void zg_setChannelMinMax(int first, int last);
    void zg_currentSpectrumArrayMaxparameters();

public slots:


private slots:

    void zh_onCurrentCalibrationChange(qreal calibrationId, int calibrationIndex);
    void zh_onRepositoryWindowOperation(ZCalibrationRepository::WindowOperationType, int, int, int);
    void zh_currentSpectrumArrayChanged(qint64, int);
    void zh_onArrayMaxParametersChanged(qint64 arrayId, int intensity, int channels);

private:

    // VARS
    ZCalibrationRepository* zv_calibrationRepository;
    ZSpectrumArrayRepository* zv_spectrumArrayRepositiry;
    qint64 zv_currentCalibrationId;
    qint64 zv_currentSpectrumArrayId;

    // FUNCS

};
//=============================================================================
#endif // ZCALIBRATIONWINDOWDATAMANAGER_H
