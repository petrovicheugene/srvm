//===========================================================
#ifndef ZJOINTCALIBRATIONWINDOWMODEL_H
#define ZJOINTCALIBRATIONWINDOWMODEL_H
//===========================================================
#include <QAbstractTableModel>
#include "ZJointCalibrationWindowDataManager.h"
//===========================================================

//===========================================================
class ZJointCalibrationWindowModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZJointCalibrationWindowModel(QObject *parent = 0);

    virtual Qt::ItemFlags	flags(const QModelIndex & index) const;
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;

    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool	setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void zp_connectToJointCalibrationWindowDataManager(ZJointCalibrationWindowDataManager* dataManager);

signals:

public slots:


private:
    // VARS
    ZJointCalibrationWindowDataManager* zv_dataManager;

};
//===========================================================
#endif // ZJOINTCALIBRATIONWINDOWMODEL_H
