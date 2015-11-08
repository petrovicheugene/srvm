//==================================================================
#include "ZArrayModel.h"
#include <QFont>
#include <QColor>
//==================================================================
ZArrayModel::ZArrayModel(QObject* parent) : QAbstractTableModel(parent)
{
    zv_repositiry = 0;
}
//==================================================================
Qt::ItemFlags	ZArrayModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    flags |= Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
    return flags;
}
//==================================================================
int ZArrayModel::columnCount(const QModelIndex & parent) const
{
    return 1;
}
//==================================================================
int ZArrayModel::rowCount(const QModelIndex & parent) const
{
    if(!zv_repositiry)
    {
        return 0;
    }

    return zv_repositiry->zp_arrayCount();
}
//==================================================================
QVariant ZArrayModel::data(const QModelIndex & index, int role) const
{
    if(!zv_repositiry)
    {
        return QVariant();
    }

    if(!index.isValid() || index.row() < 0 || index.row() >= zv_repositiry->zp_arrayCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        return QVariant(zv_repositiry->zp_arrayName(index.row()));
    }

    return QVariant();
}
//==================================================================
bool	ZArrayModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(zv_repositiry == 0 || !index.isValid() || index.row() < 0 || index.row() >= rowCount()
            || index.column() < 0 || index.column() >= columnCount() || !value.isValid())
    {
        return false;
    }

    if(role == Qt::EditRole)
    {
        if(index.column() == 0)
        {
            if(!value.canConvert<QString>())
            {
                return false;
            }
            return zv_repositiry->zp_setSpectrumArrayName(index.row(), value.toString());
        }
    }

    return false;
}
//==================================================================
QVariant ZArrayModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            if(section == 0)
            {
                return QVariant(tr("Array"));
            }

            return QVariant();
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
void ZArrayModel::zp_setSpectraArrayRepository(ZSpectraArrayRepository* repository)
{
    beginResetModel();
    zv_repositiry = repository;
    // array repository <-> array model
    connect(repository, &ZSpectraArrayRepository::zg_currentArrayOperation,
            this, &ZArrayModel::zh_onRepositoryOperation);
    endResetModel();
}
//==================================================================
void ZArrayModel::zh_onRepositoryOperation(ZSpectraArrayRepository::ArrayOperationType type, int first, int last)
{
    if(type == ZSpectraArrayRepository::AOT_INSERT_ARRAYS)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZSpectraArrayRepository::AOT_END_INSERT_ARRAYS)
    {
        endInsertRows();
    }
    else if(type == ZSpectraArrayRepository::AOT_REMOVE_ARRAYS)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == ZSpectraArrayRepository::AOT_END_REMOVE_ARRAYS)
    {
        endRemoveRows();
    }

    emit zg_checkCurrentArray();
}
//==================================================================
