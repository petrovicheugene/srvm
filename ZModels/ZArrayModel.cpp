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
    flags |= Qt::ItemIsEnabled |Qt::ItemIsSelectable;
    if(index.column() < 2)
    {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}
//==================================================================
QModelIndex ZArrayModel::index(int row, int column, const QModelIndex &parent) const
{
    if(zv_repositiry == 0 || row < 0 || row >= zv_repositiry->zp_arrayCount()
            || column < 0 || column >= columnCount())
    {
        return QModelIndex();
    }

    return createIndex(row, column);
}
//==================================================================
int ZArrayModel::columnCount(const QModelIndex & parent) const
{
    return 3;
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
        if(index.column() == 0)
        {
            return QVariant(zv_repositiry->zp_arrayName(index.row()));
        }
        if(index.column() == 1)
        {
            int gainFactor = zv_repositiry->zp_gainFactor(index.row());
            if(gainFactor >= 0)
            {
                return QVariant(gainFactor);
            }
        }
        if(index.column() == 2)
        {
            int exposition = zv_repositiry->zp_exposition(index.row());
            if(exposition >= 0)
            {
                return QVariant(exposition);
            }
        }
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
        if(index.column() == 1)
        {
            if(!value.canConvert<int>())
            {
                return false;
            }
            return zv_repositiry->zp_setGainFactor(index.row(), value.toInt());
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
            if(section == 1)
            {
                return QVariant(tr("Gain Factor"));
            }
            if(section == 2)
            {
                return QVariant(tr("Exposition"));
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
void ZArrayModel::zp_connectToSpectraArrayRepository(ZSpectrumArrayRepository* repository)
{
    beginResetModel();
    zv_repositiry = repository;
    // array repository <-> array model
    connect(repository, &ZSpectrumArrayRepository::zg_spectrumArrayOperation,
            this, &ZArrayModel::zh_onRepositoryOperation);
    endResetModel();
}
//==================================================================
void ZArrayModel::zh_onRepositoryOperation(ZSpectrumArrayRepository::ArrayOperationType type, int first, int last)
{
    if(type == ZSpectrumArrayRepository::AOT_INSERT_ARRAYS)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZSpectrumArrayRepository::AOT_END_INSERT_ARRAYS)
    {
        endInsertRows();
    }
    else if(type == ZSpectrumArrayRepository::AOT_REMOVE_ARRAYS)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == ZSpectrumArrayRepository::AOT_END_REMOVE_ARRAYS)
    {
        endRemoveRows();
    }
    else if(type == ZSpectrumArrayRepository::AOT_DATA_CHANGED)
    {
        QModelIndex firstIndex = index(first, 0, QModelIndex());
        QModelIndex lastIndex = index(last, columnCount() - 1, QModelIndex());
        emit dataChanged(firstIndex, lastIndex);
    }

    emit zg_checkCurrentArray();
}
//==================================================================
