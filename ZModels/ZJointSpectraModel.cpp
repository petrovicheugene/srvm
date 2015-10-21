//==================================================================
#include "ZJointSpectraModel.h"
//==================================================================
ZSpectraModel::ZSpectraModel(QObject *parent) : QAbstractTableModel(parent)
{

}
//==================================================================
Qt::ItemFlags	ZChemElementModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    return flags;
}
//==================================================================
int ZSpectraModel::columnCount(const QModelIndex & parent) const
{
    return 1;
}
//==================================================================
int ZSpectraModel::rowCount(const QModelIndex & parent) const
{
    return 1;
}
//==================================================================
QVariant ZSpectraModel::data(const QModelIndex & index, int role) const
{
    return QVariant();
}
//==================================================================
bool	ZSpectraModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    return false;
}
//==================================================================
QVariant ZSpectraModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
//==================================================================
