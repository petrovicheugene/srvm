//==================================================================
#include "ZChemElementModel.h"
#include "globalVariables.h"

#include <QFont>
#include <QColor>
//==================================================================
ZChemElementModel::ZChemElementModel(QObject *parent) : QAbstractTableModel(parent)
{
    zv_dataManager = 0;
}
//==================================================================
Qt::ItemFlags	ZChemElementModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    flags |= Qt::ItemIsEnabled
            | Qt::ItemIsSelectable
            | Qt::ItemIsEditable;
         //   |Qt::ItemIsUserCheckable;
    return flags;
}
//==================================================================
int ZChemElementModel::columnCount(const QModelIndex & parent) const
{
    return 1;
}
//==================================================================
int ZChemElementModel::rowCount(const QModelIndex & parent) const
{
    if(zv_dataManager == 0)
    {
        return 0;
    }
    return zv_dataManager->zp_rowCount();
}
//==================================================================
QVariant ZChemElementModel::data(const QModelIndex & index, int role) const
{
    if(!zv_dataManager || !index.isValid()
            || index.row() < 0 || index.row() >= zv_dataManager->zp_rowCount()
            || index.column() < 0 || index.column() >= zv_dataManager->zp_columnCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            return zv_dataManager->zp_chemElementName(index.row());
        }
        return QVariant();
    }

//    if(role == Qt::CheckStateRole)
//    {
//        if(index.column() == 0)
//        {
//            bool visible = zv_dataManager->zp_chemElementIsVisible(index.row());
//            return visible ? Qt::Checked : Qt::Unchecked;
//        }
//        return QVariant();
//    }

    if(role == VisibleRole)
    {
        if(index.column() == 0)
        {
            return QVariant(zv_dataManager->zp_chemElementIsVisible(index.row()));
        }
    }


    if(role == Qt::DecorationRole)
    {
        if(index.column() == 0)
        {
            return QVariant(QColor(Qt::transparent));
        }
    }

    return QVariant();
}
//==================================================================
bool	ZChemElementModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!zv_dataManager || !index.isValid()
            || index.row() < 0 || index.row() >= zv_dataManager->zp_rowCount()
            || index.column() < 0 || index.column() >= zv_dataManager->zp_columnCount()
            || !value.isValid())
    {
        return false;
    }

    if(role == VisibleRole)
    {
        if(index.column() == 0)
        {
            if(!value.canConvert<bool>())
            {
                return false;
            }

            bool visibility = value.toBool();
            return zv_dataManager->zp_setVisible(index.row(), visibility);
        }
        return false;
    }
    else if(role == Qt::EditRole)
    {
        if(index.column() == 0)
        {
            if(!value.canConvert<QString>())
            {
                return false;
            }
            return zv_dataManager->zp_setChemElementName(index.row(), value.toString());
        }
        return false;
    }

    return false;
}
//==================================================================
QVariant ZChemElementModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            if(section == 0)
            {
                return QVariant(tr("Chemical element"));
            }
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
void ZChemElementModel::zp_connectToChemElementDataManager(ZChemElementDataManager *dataManager)
{
    beginResetModel();
    zv_dataManager = dataManager;
    // array repository <-> array model
    connect(dataManager, &ZChemElementDataManager::zg_currentOperation,
            this, &ZChemElementModel::zh_onRepositoryOperation);
    endResetModel();

}
//==================================================================
void ZChemElementModel::zh_onRepositoryOperation(ZChemElementDataManager::OperationType type,
                                                 int first, int last)
{
    if(type == ZChemElementDataManager::OT_RESET_DATA)
    {
        beginResetModel();
    }
    else if(type == ZChemElementDataManager::OT_END_RESET_DATA)
    {
        endResetModel();
    }
    else if(type == ZChemElementDataManager::OT_INSERT_ROW)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZChemElementDataManager::OT_END_INSERT_ROW)
    {
        endInsertRows();
    }
    else if(type == ZChemElementDataManager::OT_REMOVE_ROW)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == ZChemElementDataManager::OT_END_REMOVE_ROW)
    {
        endRemoveRows();
    }
    else if(type == ZChemElementDataManager::OT_INSERT_COLUMN)
    {
        beginInsertColumns(QModelIndex(), first, last);
    }
    else if(type == ZChemElementDataManager::OT_END_INSERT_COLUMN)
    {
        endInsertColumns();
    }
    else if(type == ZChemElementDataManager::OT_REMOVE_COLUMN)
    {
        beginRemoveColumns(QModelIndex(), first, last);
    }
    else if(type == ZChemElementDataManager::OT_END_REMOVE_COLUMN)
    {
        endRemoveColumns();
    }
    else if(type == ZChemElementDataManager::OT_DATA_CHANGED)
    {
        QModelIndex leftTop = index(first, 0);
        QModelIndex bottomRight = index(last, columnCount() - 1);

        emit dataChanged(leftTop, bottomRight);
    }

}
//==================================================================
