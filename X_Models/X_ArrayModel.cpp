//==================================================================
#include "X_ArrayModel.h"
#include <QFont>
#include <QColor>
//==================================================================
X_ArrayModel::X_ArrayModel(QObject* parent) : QAbstractTableModel(parent)
{
    xv_repositiry = 0;
}
//==================================================================
Qt::ItemFlags	X_ArrayModel::flags(const QModelIndex & index) const
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
QModelIndex X_ArrayModel::index(int row, int column, const QModelIndex &parent) const
{
    if(xv_repositiry == 0 || row < 0 || row >= xv_repositiry->xp_arrayCount()
            || column < 0 || column >= columnCount())
    {
        return QModelIndex();
    }

    return createIndex(row, column);
}
//==================================================================
int X_ArrayModel::columnCount(const QModelIndex & parent) const
{
    return 3;
}
//==================================================================
int X_ArrayModel::rowCount(const QModelIndex & parent) const
{
    if(!xv_repositiry)
    {
        return 0;
    }

    return xv_repositiry->xp_arrayCount();
}
//==================================================================
QVariant X_ArrayModel::data(const QModelIndex & index, int role) const
{
    if(!xv_repositiry)
    {
        return QVariant();
    }

    if(!index.isValid() || index.row() < 0 || index.row() >= xv_repositiry->xp_arrayCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            return QVariant(xv_repositiry->xp_arrayName(index.row()));
        }
        if(index.column() == 1)
        {
            int gainFactor = xv_repositiry->xp_gainFactor(index.row());
            if(gainFactor >= 0)
            {
                return QVariant(gainFactor);
            }
        }
        if(index.column() == 2)
        {
            int exposition = xv_repositiry->xp_exposition(index.row());
            if(exposition >= 0)
            {
                return QVariant(exposition);
            }
        }
    }

    return QVariant();
}
//==================================================================
bool	X_ArrayModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(xv_repositiry == 0 || !index.isValid() || index.row() < 0 || index.row() >= rowCount()
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
            return xv_repositiry->xp_setSpectrumArrayName(index.row(), value.toString());
        }
        if(index.column() == 1)
        {
            if(!value.canConvert<int>())
            {
                return false;
            }
            return xv_repositiry->xp_setGainFactor(index.row(), value.toInt());
        }
    }

    return false;
}
//==================================================================
QVariant X_ArrayModel::headerData(int section, Qt::Orientation orientation, int role) const
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
void X_ArrayModel::xp_connectToSpectraArrayRepository(X_SpectrumArrayRepository* repository)
{
    beginResetModel();
    xv_repositiry = repository;
    // array repository <-> array model
    connect(repository, &X_SpectrumArrayRepository::xg_spectrumArrayOperation,
            this, &X_ArrayModel::zh_onRepositoryOperation);
    endResetModel();
}
//==================================================================
void X_ArrayModel::zh_onRepositoryOperation(X_SpectrumArrayRepository::ArrayOperationType type, int first, int last)
{
    if(type == X_SpectrumArrayRepository::AOT_INSERT_ARRAYS)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == X_SpectrumArrayRepository::AOT_END_INSERT_ARRAYS)
    {
        endInsertRows();
    }
    else if(type == X_SpectrumArrayRepository::AOT_REMOVE_ARRAYS)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == X_SpectrumArrayRepository::AOT_END_REMOVE_ARRAYS)
    {
        endRemoveRows();
    }
    else if(type == X_SpectrumArrayRepository::AOT_DATA_CHANGED)
    {
        QModelIndex firstIndex = index(first, 0, QModelIndex());
        QModelIndex lastIndex = index(last, columnCount() - 1, QModelIndex());
        emit dataChanged(firstIndex, lastIndex);
    }

    emit xg_checkCurrentArray();
}
//==================================================================
