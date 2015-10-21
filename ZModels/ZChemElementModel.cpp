//==================================================================
#include "ZChemElementModel.h"
//==================================================================
ZChemElementModel::ZChemElementModel(QObject *parent) : QAbstractTableModel(parent)
{

}
//==================================================================
Qt::ItemFlags	ZChemElementModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
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
    return 1;
}
//==================================================================
QVariant ZChemElementModel::data(const QModelIndex & index, int role) const
{
    return QVariant();
}
//==================================================================
bool	ZChemElementModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    return false;
}
//==================================================================
QVariant ZChemElementModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
//==================================================================
