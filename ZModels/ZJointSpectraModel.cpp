//==================================================================
#include "ZJointSpectraModel.h"
//==================================================================
ZJointSpectraModel::ZJointSpectraModel(QObject *parent) : QAbstractTableModel(parent)
{

}
//==================================================================
Qt::ItemFlags	ZJointSpectraModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    return flags;
}
//==================================================================
int ZJointSpectraModel::columnCount(const QModelIndex & parent) const
{
    return 1;
}
//==================================================================
int ZJointSpectraModel::rowCount(const QModelIndex & parent) const
{
    return 1;
}
//==================================================================
QVariant ZJointSpectraModel::data(const QModelIndex & index, int role) const
{
    return QVariant();
}
//==================================================================
bool	ZJointSpectraModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    return false;
}
//==================================================================
QVariant ZJointSpectraModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
//==================================================================
