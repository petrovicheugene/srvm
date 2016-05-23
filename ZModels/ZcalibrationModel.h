//===========================================================
#ifndef ZCALIBRATIONMODEL_H
#define ZCALIBRATIONMODEL_H
//===========================================================
#include <QAbstractTableModel>
#include "ZCalibrationRepository.h"
#include "ZCalibrationQualityData.h"
#include "QMap"
//===========================================================

//===========================================================
class ZCalibrationModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZCalibrationModel(QObject *parent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool	setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    // FUNCS
    void zp_connectToCalibrationRepository(ZCalibrationRepository* repository);

signals:

public slots:

    void zp_calibrationQualityDataChanged(bool saveTocalibration, qint64 calibrationId, ZCalibrationQualityData);

private:

    // VARS
    ZCalibrationRepository* zv_calibrationRepository;

    QMap<qint64, ZCalibrationQualityData> zv_qualityData;

private slots:

    void zh_onCalibrationRepositoryOperation(ZCalibrationRepository::CalibrationOperationType, int, int);

};
//===========================================================
#endif // ZCALIBRATIONMODEL_H
