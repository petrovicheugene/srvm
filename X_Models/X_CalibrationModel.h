//===========================================================
#ifndef X_CALIBRATIONMODEL_H
#define X_CALIBRATIONMODEL_H
//===========================================================
#include <QAbstractTableModel>
#include "X_CalibrationRepository.h"
#include "X_CalibrationQualityData.h"
#include "QMap"
//===========================================================

//===========================================================
class X_CalibrationModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit X_CalibrationModel(QObject *parent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool	setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    // FUNCS
    void xp_connectToCalibrationRepository(X_CalibrationRepository* repository);

signals:

public slots:

    void xp_calibrationQualityDataChanged(bool saveToCalibration, qint64 calibrationId, X_CalibrationQualityData);

private:

    // VARS
    X_CalibrationRepository* xv_calibrationRepository;

    QMap<qint64, X_CalibrationQualityData> xv_qualityData;

private slots:

    void zh_onCalibrationRepositoryOperation(X_CalibrationRepository::CalibrationOperationType, int, int);

};
//===========================================================
#endif // X_CALIBRATIONMODEL_H
