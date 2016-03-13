//==================================================================
#include "ZJointSpectraModel.h"
#include "globalVariables.h"

#include <QFont>
#include <QColor>
//==================================================================
ZJointSpectraModel::ZJointSpectraModel(QObject *parent) : QAbstractTableModel(parent)
{
    zv_dataManager = 0;
}
//==================================================================
Qt::ItemFlags	ZJointSpectraModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if(index.column() == 0)
    {
         flags |= Qt::ItemIsUserCheckable;
    }
    else if(zv_dataManager->zp_isColumnChemElement(index.column()))
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}
//==================================================================
int ZJointSpectraModel::columnCount(const QModelIndex & parent) const
{
    if(!zv_dataManager)
    {
        return 0;
    }

    return zv_dataManager->zp_columnCount();
}
//==================================================================
int ZJointSpectraModel::rowCount(const QModelIndex & parent) const
{
    if(!zv_dataManager)
    {
        return 0;
    }

    return zv_dataManager->zp_rowCount();
}
//==================================================================
QVariant ZJointSpectraModel::data(const QModelIndex & index, int role) const
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

    if(role == Qt::TextAlignmentRole)
    {
        Qt::Alignment flag = Qt::AlignRight | Qt::AlignVCenter;
        return QVariant(flag);
    }

    if(role == Qt::DecorationRole)
    {
        if(index.column() == 1)
        {
            return QVariant(zv_dataManager->zp_spectrumColor(index.row()));
        }
    }

    if(role == VisibleRole)
    {
        if(index.column() == 1)
        {
            return QVariant(zv_dataManager->zp_isSpectrumVisible(index.row()));
        }
    }

    if(role == Qt::CheckStateRole)
    {
        if(index.column() == 0)
        {
            if(zv_dataManager->zp_isSpectrumChecked(index.row()))
            {
                return QVariant(Qt::Checked);
            }
            else
            {
                return QVariant(Qt::Unchecked);
            }
        }
    }

    return QVariant();
}
//==================================================================
bool	ZJointSpectraModel::setData(const QModelIndex & index, const QVariant & value, int role)
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
        if(value.canConvert<QString>())
        {
            bool res =  zv_dataManager->zp_setChemConcentration(index.row(), index.column(), value.toString());
            if(res)
            {
                emit dataChanged(index, index);
            }
        }
    }

    if(role == VisibleRole)
    {
        if(index.column() == 1 && value.canConvert<bool>())
        {
            bool visible = value.toBool();
            return zv_dataManager->zp_setSpectrumVisible(index.row(), visible);
        }
    }

    if(role == Qt::CheckStateRole)
    {
        if(index.column() == 0 && value.canConvert<int>())
        {
            Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt());
            bool checked = checkState == Qt::Checked ? true : false;
            return zv_dataManager->zp_setSpectrumChecked(index.row(), checked);
        }
    }

    return false;
}
//==================================================================
QVariant ZJointSpectraModel::headerData(int section, Qt::Orientation orientation, int role) const
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
void ZJointSpectraModel::zp_connectToSpectraDataManager(ZJointSpectraDataManager* dataManager)
{
    beginResetModel();
    zv_dataManager = dataManager;
    // data manager <-> joint model
    connect(dataManager, &ZJointSpectraDataManager::zg_currentOperation,
            this, &ZJointSpectraModel::zh_onDataManagerOperation);
    endResetModel();
}
//==================================================================
void ZJointSpectraModel::zh_onDataManagerOperation(ZJointSpectraDataManager::OperationType type, int first, int last)
{
    if(!zv_dataManager)
    {
        return;
    }

    if(type == ZJointSpectraDataManager::OT_BEGIN_RESET)
    {
        beginResetModel();
    }
    else if(type == ZJointSpectraDataManager::OT_END_RESET)
    {
        endResetModel();
    }
    else if(type == ZJointSpectraDataManager::OT_BEGIN_INSERT_ROW)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZJointSpectraDataManager::OT_END_INSERT_ROW)
    {
        endInsertRows();
    }
    else if(type == ZJointSpectraDataManager::OT_BEGIN_REMOVE_ROW)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == ZJointSpectraDataManager::OT_END_REMOVE_ROW)
    {
        endRemoveRows();
    }
    else if(type == ZJointSpectraDataManager::OT_BEGIN_INSERT_COLUMN)
    {
        beginInsertColumns(QModelIndex(), first, last);
    }
    else if(type == ZJointSpectraDataManager::OT_END_INSERT_COLUMN)
    {
        endInsertColumns();
    }
    else if(type == ZJointSpectraDataManager::OT_BEGIN_REMOVE_COLUMN)
    {
        beginRemoveColumns(QModelIndex(), first, last);
    }
    else if(type == ZJointSpectraDataManager::OT_END_REMOVE_COLUMN)
    {
        endRemoveColumns();
    }
    else if(type == ZJointSpectraDataManager::OT_COLUMN_HEADER_CHANGED)
    {
        emit headerDataChanged(Qt::Horizontal, first, last);
    }
    else if(type == ZJointSpectraDataManager::OT_SEPECTRUM_VISIBLE_CHANGED)
    {
        QModelIndex leftTop = index(first, 0);
        QModelIndex rightBottom = index(last, zv_dataManager->zp_spectrumDataColumnCount() - 1);

        emit dataChanged(leftTop, rightBottom);
    }
    else if(type == ZJointSpectraDataManager::OT_COLUMN_DATA_CHANGED)
    {
        QModelIndex leftTop = index(0, first);
        QModelIndex rightBottom = index(zv_dataManager->zp_rowCount() - 1, last);

        emit dataChanged(leftTop, rightBottom);
    }
}
//==================================================================
