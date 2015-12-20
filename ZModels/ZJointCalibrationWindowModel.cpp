//==================================================================
#include "ZJointCalibrationWindowModel.h"
//==================================================================
ZJointCalibrationWindowModel::ZJointCalibrationWindowModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    zv_dataManager = 0;
}
//==================================================================
Qt::ItemFlags	ZJointCalibrationWindowModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
//    flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;

//    if(zv_dataManager->zp_isColumnChemElement(index.column()))
//    {
//        flags |= Qt::ItemIsEditable;
//    }

    return flags;
}
//==================================================================
int ZJointCalibrationWindowModel::columnCount(const QModelIndex & parent) const
{
    return 2;
//    if(!zv_dataManager)
//    {
//        return 0;
//    }

//    return zv_dataManager->zp_columnCount();
}
//==================================================================
int ZJointCalibrationWindowModel::rowCount(const QModelIndex & parent) const
{
    return 2;
//    if(!zv_dataManager)
//    {
//        return 0;
//    }

//    return zv_dataManager->zp_rowCount();

}
//==================================================================
QVariant ZJointCalibrationWindowModel::data(const QModelIndex & index, int role) const
{
//    if(!zv_dataManager || !index.isValid()
//            || index.row() < 0 || index.row() >=  rowCount()
//            || index.column() < 0 || index.column() >= columnCount())
//    {
//        return QVariant();
//    }

//    if(role == Qt::DisplayRole)
//    {
//        return zv_dataManager->zp_data(index);
//    }
//    if(role == Qt::DecorationRole)
//    {
//        if(index.column() == 0)
//        {
//            return QVariant(zv_dataManager->zp_spectrumColor(index.row()));
//        }
//    }

//    if(role == VisibleRole)
//    {
//        if(index.column() == 0)
//        {
//            return QVariant(zv_dataManager->zp_isSpectrumVisible(index.row()));
//        }
//    }

    return QVariant();
}
//==================================================================
bool	ZJointCalibrationWindowModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
//    if(!zv_dataManager || !index.isValid()
//            || index.row() < 0 || index.row() >=  rowCount()
//            || index.column() < 0 || index.column() >= columnCount()
//            || !value.isValid())
//    {
//        return false;
//    }

//    if(role == Qt::EditRole)
//    {
//        if(value.canConvert<QString>())
//        {
//            return zv_dataManager->zp_setChemConcentration(index.row(), index.column(), value.toString());
//        }
//    }

//    if(role == VisibleRole)
//    {
//        if(index.column() == 0 && value.canConvert<bool>())
//        {
//            bool visible = value.toBool();
//            return zv_dataManager->zp_setSpectrumVisible(index.row(), visible);
//        }
//    }

    return false;
}
//==================================================================
QVariant ZJointCalibrationWindowModel::headerData(int section, Qt::Orientation orientation, int role) const
{
//    if(!zv_dataManager)
//    {
//        return QVariant();
//    }

//    if(role == Qt::DisplayRole)
//    {
//        if(orientation == Qt::Horizontal)
//        {
//            return QVariant(zv_dataManager->zp_columnName(section));
//        }
//        else
//        {
//            return QVariant(QString::number(section + 1));
//        }
//    }

//    if(role == Qt::FontRole)
//    {
//        QFont font = QAbstractItemModel::headerData(section, orientation, role).value<QFont>();
//        font.setBold(true);
//        return QVariant(font);
//    }

//    if(role == Qt::ForegroundRole)
//    {
//        return QVariant(QColor(Qt::darkGreen));
//    }

    return QVariant();
}
//==================================================================
void ZJointCalibrationWindowModel::zp_connectToJointCalibrationWindowDataManager(ZJointCalibrationWindowDataManager* dataManager)
{
    beginResetModel();
    zv_dataManager = dataManager;
    // data manager <-> joint model
//    connect(dataManager, &ZJointSpectraDataManager::zg_currentOperation,
//            this, &ZJointSpectraModel::zh_onDataManagerOperation);
    endResetModel();
}
//==================================================================
