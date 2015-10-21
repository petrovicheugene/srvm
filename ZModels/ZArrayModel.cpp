//==================================================================
#include "ZArrayModel.h"
//==================================================================
ZArrayModel::ZArrayModel(QObject* parent) : QAbstractTableModel(parent)
{

}
//==================================================================
Qt::ItemFlags	ZArrayModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
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
    return 1;
}
//==================================================================
QVariant ZArrayModel::data(const QModelIndex & index, int role) const
{
    return QVariant();
}
//==================================================================
bool	ZArrayModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    return false;
}
//==================================================================
QVariant ZArrayModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
//==================================================================
