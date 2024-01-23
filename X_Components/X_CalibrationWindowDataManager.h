//=============================================================================
#ifndef X_CALIBRATIONWINDOWDATAMANAGER_H
#define X_CALIBRATIONWINDOWDATAMANAGER_H
//=============================================================================
#include <QObject>
#include <QModelIndex>
#include "X_SpectrumArrayRepository.h"
#include "X_CalibrationRepository.h"
//=============================================================================

//=============================================================================
class X_CalibrationWindowDataManager : public QObject
{
    Q_OBJECT
public:
    explicit X_CalibrationWindowDataManager(QObject *parent = 0);

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
    void xp_connectToSpectraArrayRepository(X_SpectrumArrayRepository*);
    void xp_connectToCalibrationRepository(X_CalibrationRepository*);

    int xp_rowCount() const;
    int xp_columnCount() const;
    QString xp_columnName(int) const;
    QVariant xp_data(QModelIndex index) const;
    bool xp_setData(QModelIndex index, QVariant data);
    QColor xp_windowColor(int) const;
    bool xp_isWindowVisible(int) const;
    bool xp_setWindowVisible(int windowIndex, bool visibility);

signals:

    void xg_currentOperation(OperationType, int, int) const;
    void xg_setChannelMinMax(int first, int last);
    void xg_currentSpectrumArrayMaxParameters();


public slots:



private slots:

    void xh_onCurrentCalibrationChange(qint64 calibrationId, int calibrationIndex);
    void xh_onRepositoryWindowOperation(X_CalibrationRepository::WindowOperationType, int, int, int);
    void xh_currentSpectrumArrayChanged(qint64, int);
    void xh_onArrayMaxParametersChanged(qint64 arrayId, int intensity, int channels);
    void xh_onCurrentSpectrumChange(qint64, int, qint64, int);

private:

    // VARS
    X_CalibrationRepository* xv_calibrationRepository;
    X_SpectrumArrayRepository* xv_spectrumArrayRepositiry;
    qint64 xv_currentCalibrationId;
    qint64 xv_currentSpectrumArrayId;

    // FUNCS

};
//=============================================================================
#endif // X_CALIBRATIONWINDOWDATAMANAGER_H
