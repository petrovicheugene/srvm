//==================================================================
#include "ZTermCorrelationTableModel.h"
#include "globalVariables.h"
#include <QFont>
#include <QApplication>
#include <QStyle>
#include <QPainter>
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
    flags |= Qt::ItemIsEnabled;
    if(index.column() == 0)
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}
//==================================================================
int ZTermCorrelationTableModel::columnCount(const QModelIndex & parent) const
{
    if(!zv_dataManager)
    {
        return 0;
    }
    // zv_columnToRemoveNumber used only while column removing
    return zv_dataManager->zp_columnCount();
}
//==================================================================
int ZTermCorrelationTableModel::rowCount(const QModelIndex & parent) const
{
    if(!zv_dataManager)
    {
        return 0;
    }

    return zv_dataManager->zp_rowCount();;
}
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

    if(role == Qt::TextAlignmentRole)
    {
        Qt::Alignment alignment;
        alignment = Qt::AlignRight | Qt::AlignVCenter;
        return QVariant(alignment);
    }

    if(role == Qt::BackgroundRole)
    {
        return zv_dataManager->zp_cellColor(index);
    }

    return QVariant();
}
//==================================================================
bool ZTermCorrelationTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!index.isValid() || !zv_dataManager || !value.isValid())
    {
        return false;
    }

    if(role == Qt::EditRole)
    {
        if(index.column() == 0)
        {
            return zv_dataManager->zp_setData(index, value);
        }
    }

    return false;
}
//==================================================================
QVariant	ZTermCorrelationTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(!zv_dataManager)
    {
        return QVariant();
    }


    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            return QVariant(zv_dataManager->zp_horizontalColumnName(section));
        }

        if(orientation == Qt::Vertical)
        {
            return QVariant(zv_dataManager->zp_verticalColumnName(section));
        }
    }

    if(role == Qt::DecorationRole)
    {
        if(orientation == Qt::Vertical)
        {
            return QVariant(QColor(Qt::red));
        }
    }

    if(role == IconRole)
    {
        if(orientation == Qt::Vertical)
        {
            return QVariant(zv_dataManager->zp_termStateIcon(section));
        }
    }

    return QVariant();
}
//==================================================================
void ZTermCorrelationTableModel::zp_connectToTermCorrelationTableManager(ZTermCorrelationTableManager* dataManager)
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
void ZTermCorrelationTableModel::zp_onUserChangesTermState(int termLogIndex)
{
    if(!zv_dataManager)
    {
        return;
    }

    zv_dataManager->zp_setNextUsersTermState(termLogIndex);
}
//==================================================================
void ZTermCorrelationTableModel::zh_onDataManagerOperation(ZTermCorrelationTableManager::OperationType type, int first, int last)
{
    if(!zv_dataManager)
    {
        return;
    }

    if(type == ZTermCorrelationTableManager::TOT_BEGIN_RESET)
    {
        beginResetModel();
    }
    else if(type == ZTermCorrelationTableManager::TOT_END_RESET)
    {
        endResetModel();
    }
    else if(type == ZTermCorrelationTableManager::TOT_BEGIN_INSERT_ROW)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZTermCorrelationTableManager::TOT_END_INSERT_ROW)
    {
        endInsertRows();
    }
    else if(type == ZTermCorrelationTableManager::TOT_BEGIN_REMOVE_ROW)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == ZTermCorrelationTableManager::TOT_END_REMOVE_ROW)
    {
        endRemoveRows();
    }
    else if(type == ZTermCorrelationTableManager::TOT_BEGIN_INSERT_COLUMN)
    {
        beginInsertColumns(QModelIndex(), first, last);
    }
    else if(type == ZTermCorrelationTableManager::TOT_END_INSERT_COLUMN)
    {
        endInsertColumns();
    }
    else if(type == ZTermCorrelationTableManager::TOT_BEGIN_REMOVE_COLUMN)
    {
        beginRemoveColumns(QModelIndex(), first, last);
    }
    else if(type == ZTermCorrelationTableManager::TOT_END_REMOVE_COLUMN)
    {
        endRemoveColumns();
    }
    else if(type == ZTermCorrelationTableManager::TOT_DATA_CHANGED)
    {
        QModelIndex leftTop = index(first, 0);
        QModelIndex rightBottom = index(last, zv_dataManager->zp_columnCount() - 1);
        emit dataChanged(leftTop, rightBottom);
    }
    else if(type == ZTermCorrelationTableManager::TOT_HORIZONTAL_HEADER_CHANGED)
    {
        emit headerDataChanged(Qt::Horizontal, first, last);
    }
    else if(type == ZTermCorrelationTableManager::TOT_VERTICAL_HEADER_CHANGED)
    {
        emit headerDataChanged(Qt::Vertical, first, last);
    }

}
//==================================================================

