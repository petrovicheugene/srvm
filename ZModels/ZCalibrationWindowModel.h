//===========================================================
#ifndef ZCALIBRATIONWINDOWMODEL_H
#define ZCALIBRATIONWINDOWMODEL_H
//===========================================================
#include <QAbstractTableModel>
#include "ZCalibrationWindowDataManager.h"
//===========================================================

//===========================================================
class ZCalibrationWindowModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZCalibrationWindowModel(QObject *parent = 0);

    virtual Qt::ItemFlags	flags(const QModelIndex & index) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;

    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool	setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void zp_connectToJointCalibrationWindowDataManager(ZCalibrationWindowDataManager* dataManager);

signals:

public slots:


private slots:

    void zh_onDataManagerOperation(ZCalibrationWindowDataManager::OperationType type, int first, int last);


private:
    // VARS
    ZCalibrationWindowDataManager* zv_dataManager;

};
//===========================================================
#endif // ZCALIBRATIONWINDOWMODEL_H
