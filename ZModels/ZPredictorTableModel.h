//===========================================================
#ifndef ZPREDICTORTABLEMODEL_H
#define ZPREDICTORTABLEMODEL_H
//===========================================================
#include <QAbstractTableModel>
#include "ZPredictorTableManager.h"

//===========================================================
class ZPredictorTableManager;
//===========================================================
class ZPredictorTableModel : public QAbstractTableModel
{
   Q_OBJECT
public:
   explicit ZPredictorTableModel(QObject* parent = 0);

   virtual Qt::ItemFlags	flags(const QModelIndex & index) const;
   virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
   virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;

   virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
   virtual bool	setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
   virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

   void zp_connectToPredicorTableManager(ZPredictorTableManager* dataManager);

private slots:

    void zh_onDataManagerOperation(ZPredictorTableManager::OperationType type, int first, int last);

private:

    // VARS
    ZPredictorTableManager* zv_dataManager;

};
//===========================================================
#endif // ZPREDICTORTABLEMODEL_H
