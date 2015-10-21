//==================================================================
#include "ZCalibrationModel.h"
//==================================================================
ZCalibrationModel::ZCalibrationModel(QObject *parent) : QAbstractTableModel(parent)
{

}
//==================================================================
Qt::ItemFlags	ZCalibrationModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    return flags;
}
//==================================================================
int ZCalibrationModel::columnCount(const QModelIndex & parent) const
{
    return 1;
}
//==================================================================
int ZCalibrationModel::rowCount(const QModelIndex & parent) const
{
    return 1;
}
//==================================================================
QVariant ZCalibrationModel::data(const QModelIndex & index, int role) const
{
    return QVariant();
}
//==================================================================
bool	ZCalibrationModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    return false;
}
//==================================================================
QVariant ZCalibrationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
//==================================================================
