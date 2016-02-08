//==================================================================
#include "ZTermCorrelationTableModel.h"
//==================================================================
ZTermCorrelationTableModel::ZTermCorrelationTableModel(QObject *parent) :
   QAbstractTableModel(parent)
{
   zv_dataManager = 0;
}
//==================================================================
Qt::ItemFlags ZTermCorrelationTableModel::flags(const QModelIndex & index) const
{
   Qt::ItemFlags flags;
   return flags;

}
//==================================================================
int ZTermCorrelationTableModel::columnCount(const QModelIndex & parent) const
{
   if(!zv_dataManager)
   {
      return 0;
   }
   return zv_dataManager->zp_columnCount();
}
//==================================================================
int ZTermCorrelationTableModel::rowCount(const QModelIndex & parent) const
{
   if(!zv_dataManager)
   {
      return 0;
   }

   return zv_dataManager->zp_rowCount();}
//==================================================================
QVariant	ZTermCorrelationTableModel::data(const QModelIndex & index, int role) const
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
bool ZTermCorrelationTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
   return false;
}
//==================================================================
QVariant	ZTermCorrelationTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   if(role == Qt::DisplayRole)
   {
       if(orientation == Qt::Horizontal)
       {
           return QVariant(zv_dataManager->zp_columnName(section));
       }

       if(orientation == Qt::Vertical)
       {
           return QVariant(section + 1);
       }
   }

   return QVariant();
}
//==================================================================
void ZTermCorrelationTableModel::zp_connectToPredicorTableManager(ZTermCorrelationTableManager* dataManager)
{
   beginResetModel();
   zv_dataManager = dataManager;
   // data manager <-> joint model
   connect(dataManager, &ZTermCorrelationTableManager::zg_currentOperation,
           this, &ZTermCorrelationTableModel::zh_onDataManagerOperation);

   //    connect(dataManager, &ZJointCalibrationWindowDataManager::zg_currentOperation,
   //            this, &ZJointCalibrationWindowModel::zh_onDataManagerOperation);

   endResetModel();
}
//==================================================================
void ZTermCorrelationTableModel::zh_onDataManagerOperation(ZTermCorrelationTableManager::OperationType type, int first, int last)
{
    if(!zv_dataManager)
    {
        return;
    }

    if(type == ZTermCorrelationTableManager::OT_RESET_DATA)
    {
        beginResetModel();
    }
    else if(type == ZTermCorrelationTableManager::OT_END_RESET_DATA)
    {
        endResetModel();
    }
    else if(type == ZTermCorrelationTableManager::OT_INSERT_ROW)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZTermCorrelationTableManager::OT_END_INSERT_ROW)
    {
        endInsertRows();
    }
    else if(type == ZTermCorrelationTableManager::OT_REMOVE_ROW)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == ZTermCorrelationTableManager::OT_END_REMOVE_ROW)
    {
        endRemoveRows();
    }
    else if(type == ZTermCorrelationTableManager::OT_INSERT_COLUMN)
    {
        beginInsertColumns(QModelIndex(), first, last);
    }
    else if(type == ZTermCorrelationTableManager::OT_END_INSERT_COLUMN)
    {
        endInsertColumns();
    }
    else if(type == ZTermCorrelationTableManager::OT_REMOVE_COLUMN)
    {
        beginRemoveColumns(QModelIndex(), first, last);
    }
    else if(type == ZTermCorrelationTableManager::OT_END_REMOVE_COLUMN)
    {
        endRemoveColumns();
    }
    else if(type == ZTermCorrelationTableManager::OT_DATA_CHANGED)
    {
        QModelIndex leftTop = index(first, 0);
        QModelIndex rightBottom = index(last, zv_dataManager->zp_columnCount() - 1);
        emit dataChanged(leftTop, rightBottom);
    }
}
//==================================================================

