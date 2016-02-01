//==================================================================
#include "ZPredictorTableModel.h"
//==================================================================
ZPredictorTableModel::ZPredictorTableModel(QObject *parent) :
   QAbstractTableModel(parent)
{
   zv_dataManager = 0;
}
//==================================================================
Qt::ItemFlags ZPredictorTableModel::flags(const QModelIndex & index) const
{
   Qt::ItemFlags flags;
   return flags;

}
//==================================================================
int ZPredictorTableModel::columnCount(const QModelIndex & parent) const
{
   if(!zv_dataManager)
   {
      return 0;
   }
   return zv_dataManager->zp_columnCount();
}
//==================================================================
int ZPredictorTableModel::rowCount(const QModelIndex & parent) const
{
   if(!zv_dataManager)
   {
      return 0;
   }

   return zv_dataManager->zp_rowCount();}
//==================================================================
QVariant	ZPredictorTableModel::data(const QModelIndex & index, int role) const
{
   if(!zv_dataManager || !index.isValid()
         || index.row() < 0 || index.row() >=  rowCount()
         || index.column() < 0 || index.column() >= columnCount())
   {
      return QVariant();
   }

   if(role == Qt::DisplayRole)
   {
      return zv_dataManager->zp_data(index);
   }

   if(role == Qt::DecorationRole)
   {
      //       if(index.column() == 0)
      //       {
      //           return QVariant(zv_dataManager->zp_windowColor(index.row()));
      //       }
   }

   //   if(role == VisibleRole)
   //   {
   //       if(index.column() == 0)
   //       {
   //           return QVariant(zv_dataManager->zp_isWindowVisible(index.row()));
   //       }
   //   }

   return QVariant();
}
//==================================================================
bool ZPredictorTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
   return false;
}
//==================================================================
QVariant	ZPredictorTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   if(role == Qt::DisplayRole)
   {
       return QVariant(zv_dataManager->zp_columnName(orientation, section));
   }

   return QVariant();
}
//==================================================================
void ZPredictorTableModel::zp_connectToPredicorTableManager(ZPredictorTableManager* dataManager)
{
   beginResetModel();
   zv_dataManager = dataManager;
   // data manager <-> joint model
   connect(dataManager, &ZPredictorTableManager::zg_currentOperation,
           this, &ZPredictorTableModel::zh_onDataManagerOperation);

   //    connect(dataManager, &ZJointCalibrationWindowDataManager::zg_currentOperation,
   //            this, &ZJointCalibrationWindowModel::zh_onDataManagerOperation);

   endResetModel();
}
//==================================================================
void ZPredictorTableModel::zh_onDataManagerOperation(ZPredictorTableManager::OperationType type, int first, int last)
{
    if(!zv_dataManager)
    {
        return;
    }

    if(type == ZPredictorTableManager::OT_RESET_DATA)
    {
        beginResetModel();
    }
    else if(type == ZPredictorTableManager::OT_END_RESET_DATA)
    {
        endResetModel();
    }
    else if(type == ZPredictorTableManager::OT_INSERT_ROW)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZPredictorTableManager::OT_END_INSERT_ROW)
    {
        endInsertRows();
    }
    else if(type == ZPredictorTableManager::OT_REMOVE_ROW)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == ZPredictorTableManager::OT_END_REMOVE_ROW)
    {
        endRemoveRows();
    }
    else if(type == ZPredictorTableManager::OT_INSERT_COLUMN)
    {
        beginInsertColumns(QModelIndex(), first, last);
    }
    else if(type == ZPredictorTableManager::OT_END_INSERT_COLUMN)
    {
        endInsertColumns();
    }
    else if(type == ZPredictorTableManager::OT_REMOVE_COLUMN)
    {
        beginRemoveColumns(QModelIndex(), first, last);
    }
    else if(type == ZPredictorTableManager::OT_END_REMOVE_COLUMN)
    {
        endRemoveColumns();
    }
    else if(type == ZPredictorTableManager::OT_DATA_CHANGED)
    {
        QModelIndex leftTop = index(first, 0);
        QModelIndex rightBottom = index(last, zv_dataManager->zp_columnCount() - 1);
        emit dataChanged(leftTop, rightBottom);
    }
}
//==================================================================

