//==================================================================
#include "X_CalibrationWindowModel.h"
#include "X_General.h"
//==================================================================
X_CalibrationWindowModel::X_CalibrationWindowModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    xv_dataManager = nullptr;
}
//==================================================================
Qt::ItemFlags	X_CalibrationWindowModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if(index.column() != 2)
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}
//==================================================================
int X_CalibrationWindowModel::columnCount(const QModelIndex & parent) const
{
    if(!xv_dataManager)
    {
        return 0;
    }

    return xv_dataManager->xp_columnCount();
}
//==================================================================
int X_CalibrationWindowModel::rowCount(const QModelIndex & parent) const
{
    if(!xv_dataManager)
    {
        return 0;
    }

    return xv_dataManager->xp_rowCount();
}
//==================================================================
QVariant X_CalibrationWindowModel::data(const QModelIndex & index, int role) const
{
    if(!xv_dataManager || !index.isValid()
            || index.row() < 0 || index.row() >=  rowCount()
            || index.column() < 0 || index.column() >= columnCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        return xv_dataManager->xp_data(index);
    }

    if(role == Qt::DecorationRole)
    {
        if(index.column() == 0)
        {
            return QVariant(xv_dataManager->xp_windowColor(index.row()));
        }
    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() == 0)
        {
            return QVariant(xv_dataManager->xp_isWindowVisible(index.row()));
        }
    }

    return QVariant();
}
//==================================================================
bool	X_CalibrationWindowModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!xv_dataManager || !index.isValid()
            || index.row() < 0 || index.row() >=  rowCount()
            || index.column() < 0 || index.column() >= columnCount()
            || !value.isValid())
    {
        return false;
    }

    if(role == Qt::EditRole)
    {
        bool res = xv_dataManager->xp_setData(index, value);
        QModelIndex intensityIndex = this->index(index.row(), 2);
        emit dataChanged(intensityIndex, intensityIndex);
        return res;
    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() == 0 && value.canConvert<bool>())
        {
            bool visible = value.toBool();
            bool res = xv_dataManager->xp_setWindowVisible(index.row(), visible);
            if (res)
            {
                emit dataChanged(index, index);
            }
            return res;
        }
    }

    return false;
}
//==================================================================
QVariant X_CalibrationWindowModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(!xv_dataManager)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            return QVariant(xv_dataManager->xp_columnName(section));
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
void X_CalibrationWindowModel::xp_connectToJointCalibrationWindowDataManager(X_CalibrationWindowDataManager* dataManager)
{
    beginResetModel();
    xv_dataManager = dataManager;
    // data manager <-> joint model
    connect(dataManager, &X_CalibrationWindowDataManager::xg_currentOperation,
            this, &X_CalibrationWindowModel::zh_onDataManagerOperation);

    //    connect(dataManager, &X_JointCalibrationWindowDataManager::xg_currentOperation,
    //            this, &X_JointCalibrationWindowModel::zh_onDataManagerOperation);

    endResetModel();
}
//==================================================================
void X_CalibrationWindowModel::zh_onDataManagerOperation(X_CalibrationWindowDataManager::OperationType type, int first, int last)
{
    if(!xv_dataManager)
    {
        return;
    }

    if(type == X_CalibrationWindowDataManager::OT_RESET_DATA)
    {
        beginResetModel();
    }
    else if(type == X_CalibrationWindowDataManager::OT_END_RESET_DATA)
    {
        endResetModel();
    }
    else if(type == X_CalibrationWindowDataManager::OT_INSERT_ROW)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == X_CalibrationWindowDataManager::OT_END_INSERT_ROW)
    {
        endInsertRows();
    }
    else if(type == X_CalibrationWindowDataManager::OT_REMOVE_ROW)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == X_CalibrationWindowDataManager::OT_END_REMOVE_ROW)
    {
        endRemoveRows();
    }
    else if(type == X_CalibrationWindowDataManager::OT_INSERT_COLUMN)
    {
        beginInsertColumns(QModelIndex(), first, last);
    }
    else if(type == X_CalibrationWindowDataManager::OT_END_INSERT_COLUMN)
    {
        endInsertColumns();
    }
    else if(type == X_CalibrationWindowDataManager::OT_REMOVE_COLUMN)
    {
        beginRemoveColumns(QModelIndex(), first, last);
    }
    else if(type == X_CalibrationWindowDataManager::OT_END_REMOVE_COLUMN)
    {
        endRemoveColumns();
    }
    else if(type == X_CalibrationWindowDataManager::OT_DATA_CHANGED)
    {
        QModelIndex leftTop = index(first, 0);
        QModelIndex rightBottom = index(last, xv_dataManager->xp_columnCount() - 1);
        emit dataChanged(leftTop, rightBottom);
    }
}
//==================================================================
