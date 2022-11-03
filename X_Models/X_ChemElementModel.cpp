//==================================================================
#include "X_ChemElementModel.h"
#include "X_General.h"

#include <QFont>
#include <QColor>
//==================================================================
X_ChemElementModel::X_ChemElementModel(QObject *parent) : QAbstractTableModel(parent)
{
    xv_dataManager = 0;
}
//==================================================================
Qt::ItemFlags	X_ChemElementModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    flags |= Qt::ItemIsEnabled
            | Qt::ItemIsSelectable
            | Qt::ItemIsEditable;
         //   |Qt::ItemIsUserCheckable;
    return flags;
}
//==================================================================
int X_ChemElementModel::columnCount(const QModelIndex & parent) const
{
    return 1;
}
//==================================================================
int X_ChemElementModel::rowCount(const QModelIndex & parent) const
{
    if(xv_dataManager == 0)
    {
        return 0;
    }
    return xv_dataManager->xp_rowCount();
}
//==================================================================
QVariant X_ChemElementModel::data(const QModelIndex & index, int role) const
{
    if(!xv_dataManager || !index.isValid()
            || index.row() < 0 || index.row() >= xv_dataManager->xp_rowCount()
            || index.column() < 0 || index.column() >= xv_dataManager->xp_columnCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            return xv_dataManager->xp_chemElementName(index.row());
        }
        return QVariant();
    }

//    if(role == Qt::CheckStateRole)
//    {
//        if(index.column() == 0)
//        {
//            bool visible = xv_dataManager->xp_chemElementIsVisible(index.row());
//            return visible ? Qt::Checked : Qt::Unchecked;
//        }
//        return QVariant();
//    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() == 0)
        {
            return QVariant(xv_dataManager->xp_chemElementIsVisible(index.row()));
        }
    }


    if(role == Qt::DecorationRole)
    {
        if(index.column() == 0)
        {
            return QVariant(QColor());
        }
    }

    return QVariant();
}
//==================================================================
bool	X_ChemElementModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!xv_dataManager || !index.isValid()
            || index.row() < 0 || index.row() >= xv_dataManager->xp_rowCount()
            || index.column() < 0 || index.column() >= xv_dataManager->xp_columnCount()
            || !value.isValid())
    {
        return false;
    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() == 0)
        {
            if(!value.canConvert<bool>())
            {
                return false;
            }

            bool visibility = value.toBool();
            return xv_dataManager->xp_setVisible(index.row(), visibility);
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
            return xv_dataManager->xp_setChemElementName(index.row(), value.toString());
        }
        return false;
    }

    return false;
}
//==================================================================
QVariant X_ChemElementModel::headerData(int section, Qt::Orientation orientation, int role) const
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
void X_ChemElementModel::xp_connectToChemElementDataManager(X_ChemElementDataManager *dataManager)
{
    beginResetModel();
    xv_dataManager = dataManager;
    // array repository <-> array model
    connect(dataManager, &X_ChemElementDataManager::xg_currentOperation,
            this, &X_ChemElementModel::xh_onRepositoryOperation);
    endResetModel();

}
//==================================================================
void X_ChemElementModel::xh_onRepositoryOperation(X_ChemElementDataManager::OperationType type,
                                                 int first, int last)
{
    if(type == X_ChemElementDataManager::OT_RESET_DATA)
    {
        beginResetModel();
    }
    else if(type == X_ChemElementDataManager::OT_END_RESET_DATA)
    {
        endResetModel();
    }
    else if(type == X_ChemElementDataManager::OT_INSERT_ROW)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == X_ChemElementDataManager::OT_END_INSERT_ROW)
    {
        endInsertRows();
    }
    else if(type == X_ChemElementDataManager::OT_REMOVE_ROW)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == X_ChemElementDataManager::OT_END_REMOVE_ROW)
    {
        endRemoveRows();
    }
    else if(type == X_ChemElementDataManager::OT_INSERT_COLUMN)
    {
        beginInsertColumns(QModelIndex(), first, last);
    }
    else if(type == X_ChemElementDataManager::OT_END_INSERT_COLUMN)
    {
        endInsertColumns();
    }
    else if(type == X_ChemElementDataManager::OT_REMOVE_COLUMN)
    {
        beginRemoveColumns(QModelIndex(), first, last);
    }
    else if(type == X_ChemElementDataManager::OT_END_REMOVE_COLUMN)
    {
        endRemoveColumns();
    }
    else if(type == X_ChemElementDataManager::OT_DATA_CHANGED)
    {
        QModelIndex leftTop = index(first, 0);
        QModelIndex bottomRight = index(last, columnCount() - 1);

        emit dataChanged(leftTop, bottomRight);
    }

}
//==================================================================
