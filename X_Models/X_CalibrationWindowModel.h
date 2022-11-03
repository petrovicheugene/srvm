//===========================================================
#ifndef X_CALIBRATIONWINDOWMODEL_H
#define X_CALIBRATIONWINDOWMODEL_H
//===========================================================
#include <QAbstractTableModel>
#include "X_CalibrationWindowDataManager.h"
//===========================================================

//===========================================================
class X_CalibrationWindowModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit X_CalibrationWindowModel(QObject *parent = nullptr);

    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;

    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void xp_connectToJointCalibrationWindowDataManager(X_CalibrationWindowDataManager* dataManager);

signals:

public slots:



private slots:

    void xh_onDataManagerOperation(X_CalibrationWindowDataManager::OperationType type, int first, int last);

private:
    // VARS
    X_CalibrationWindowDataManager* xv_dataManager;

};
//===========================================================
#endif // X_CALIBRATIONWINDOWMODEL_H
