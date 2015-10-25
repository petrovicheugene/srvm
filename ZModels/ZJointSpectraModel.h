//===========================================================
#ifndef ZJOINTSPECTRAMODEL_H
#define ZJOINTSPECTRAMODEL_H
//===========================================================
#include <QAbstractTableModel>
#include "ZSpectraJointDataManager.h"
//===========================================================
//===========================================================
class ZJointSpectraModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZJointSpectraModel(QObject *parent = 0);

    virtual Qt::ItemFlags	flags(const QModelIndex & index) const;
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;

    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool	setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    // FUNCS
    void zp_setDataManager(ZJointSpectraDataManager* dataManager);

public slots:


signals:




private:

    // VARS
    ZJointSpectraDataManager* zv_dataManager;

private slots:

    void zh_onDataManagerOperation(ZJointSpectraDataManager::OperationType, int, int);

};
//===========================================================
#endif // ZJOINTSPECTRAMODEL_H
