//==================================================================
#include "ZCalibrationWindowModel.h"
#include "globalVariables.h"
//==================================================================
ZCalibrationWindowModel::ZCalibrationWindowModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    zv_dataManager = 0;
}
//==================================================================
Qt::ItemFlags	ZCalibrationWindowModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    //    if(zv_dataManager->zp_isColumnChemElement(index.column()))
    //    {
    flags |= Qt::ItemIsEditable;
    //    }

    return flags;
}
//==================================================================
int ZCalibrationWindowModel::columnCount(const QModelIndex & parent) const
{
    if(!zv_dataManager)
    {
        return 0;
    }

    return zv_dataManager->zp_columnCount();
}
//==================================================================
int ZCalibrationWindowModel::rowCount(const QModelIndex & parent) const
{
    if(!zv_dataManager)
    {
        return 0;
    }

    return zv_dataManager->zp_rowCount();
}
//==================================================================
QVariant ZCalibrationWindowModel::data(const QModelIndex & index, int role) const
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
        if(index.column() == 0)
        {
            return QVariant(zv_dataManager->zp_windowColor(index.row()));
        }
    }

    if(role == VisibleRole)
    {
        if(index.column() == 0)
        {
            return QVariant(zv_dataManager->zp_isWindowVisible(index.row()));
        }
    }

    return QVariant();
}
//==================================================================
bool	ZCalibrationWindowModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!zv_dataManager || !index.isValid()
            || index.row() < 0 || index.row() >=  rowCount()
            || index.column() < 0 || index.column() >= columnCount()
            || !value.isValid())
    {
        return false;
    }

    if(role == Qt::EditRole)
    {
        return zv_dataManager->zp_setData(index, value);
    }

    if(role == VisibleRole)
    {
        if(index.column() == 0 && value.canConvert<bool>())
        {
            bool visible = value.toBool();
            return zv_dataManager->zp_setWindowVisible(index.row(), visible);
        }
    }

    return false;
}
//==================================================================
QVariant ZCalibrationWindowModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(!zv_dataManager)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            return QVariant(zv_dataManager->zp_columnName(section));
        }
        else
        {
            return QVariant(QString::number(section + 1));
        }
    }

    if(role == Qt::FontRole)
    {
        QFont font = QAbstractItemModel::headerData(section, orientation, role).value<QFont>();
        font.setBold(true);
        return QVariant(font);
    }

    if(role == Qt::ForegroundRole)
    {
        return QVariant(QColor(Qt::darkGreen));
    }

    return QVariant();
}
//==================================================================
void ZCalibrationWindowModel::zp_connectToJointCalibrationWindowDataManager(ZCalibrationWindowDataManager* dataManager)
{
    beginResetModel();
    zv_dataManager = dataManager;
    // data manager <-> joint model
    connect(dataManager, &ZCalibrationWindowDataManager::zg_currentOperation,
            this, &ZCalibrationWindowModel::zh_onDataManagerOperation);

    //    connect(dataManager, &ZJointCalibrationWindowDataManager::zg_currentOperation,
    //            this, &ZJointCalibrationWindowModel::zh_onDataManagerOperation);

    endResetModel();
}
//==================================================================
void ZCalibrationWindowModel::zh_onDataManagerOperation(ZCalibrationWindowDataManager::OperationType type, int first, int last)
{
    if(!zv_dataManager)
    {
        return;
    }

    if(type == ZCalibrationWindowDataManager::OT_RESET_DATA)
    {
        beginResetModel();
    }
    else if(type == ZCalibrationWindowDataManager::OT_END_RESET_DATA)
    {
        endResetModel();
    }
    else if(type == ZCalibrationWindowDataManager::OT_INSERT_ROW)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZCalibrationWindowDataManager::OT_END_INSERT_ROW)
    {
        endInsertRows();
    }
    else if(type == ZCalibrationWindowDataManager::OT_REMOVE_ROW)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == ZCalibrationWindowDataManager::OT_END_REMOVE_ROW)
    {
        endRemoveRows();
    }
    else if(type == ZCalibrationWindowDataManager::OT_INSERT_COLUMN)
    {
        beginInsertColumns(QModelIndex(), first, last);
    }
    else if(type == ZCalibrationWindowDataManager::OT_END_INSERT_COLUMN)
    {
        endInsertColumns();
    }
    else if(type == ZCalibrationWindowDataManager::OT_REMOVE_COLUMN)
    {
        beginRemoveColumns(QModelIndex(), first, last);
    }
    else if(type == ZCalibrationWindowDataManager::OT_END_REMOVE_COLUMN)
    {
        endRemoveColumns();
    }
    else if(type == ZCalibrationWindowDataManager::OT_DATA_CHANGED)
    {
        QModelIndex leftTop = index(first, 0);
        QModelIndex rightBottom = index(last, zv_dataManager->zp_columnCount() - 1);
        emit dataChanged(leftTop, rightBottom);
    }
}
//==================================================================
