//==================================================================
#include "ZJointSpectraModel.h"
#include "glVariables.h"

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

    if(zv_dataManager->zp_isColumnChemElementColumn(index.column()))
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

    int columnC = zv_dataManager->zp_columnCount();
#ifdef DBG
        qDebug() << "COLUMN COUNT" << columnC;
#endif

    return columnC; // zv_dataManager->zp_columnCount();
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
            return zv_dataManager->zp_setChemConcentration(index.row(), index.column(), value.toString());
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
            QString hn = zv_dataManager->zp_columnName(section);

#ifdef DBG
        qDebug() << "COLUMN NAME" << section << hn;
#endif

            return hn; //QVariant(zv_dataManager->zp_columnName(section));
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
void ZJointSpectraModel::zp_setDataManager(ZJointSpectraDataManager* dataManager)
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
    if(type == ZJointSpectraDataManager::OT_RESET_DATA)
    {
        beginResetModel();
    }
    else if(type == ZJointSpectraDataManager::OT_END_RESET_DATA)
    {
        endResetModel();
    }
    else if(type == ZJointSpectraDataManager::OT_INSERT_ROW)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZJointSpectraDataManager::OT_END_INSERT_ROW)
    {
        endInsertRows();
    }
    else if(type == ZJointSpectraDataManager::OT_REMOVE_ROW)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == ZJointSpectraDataManager::OT_END_REMOVE_ROW)
    {
        endRemoveRows();
    }
    else if(type == ZJointSpectraDataManager::OT_INSERT_COLUMN)
    {
#ifdef DBG
        qDebug() << "OT_INSERT_COLUMN";
#endif
        beginInsertColumns(QModelIndex(), first, last);
    }
    else if(type == ZJointSpectraDataManager::OT_END_INSERT_COLUMN)
    {
#ifdef DBG
        qDebug() << "OT_END_INSERT_COLUMN";
#endif
        endInsertColumns();
    }
    else if(type == ZJointSpectraDataManager::OT_REMOVE_COLUMN)
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
}
//==================================================================
