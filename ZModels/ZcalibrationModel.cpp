//==================================================================
#include "ZCalibrationModel.h"
#include "globalVariables.h"

#include <QFont>
#include <QColor>
//==================================================================
ZCalibrationModel::ZCalibrationModel(QObject *parent) : QAbstractTableModel(parent)
{
    zv_calibrationRepository = 0;
}
//==================================================================
Qt::ItemFlags	ZCalibrationModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    flags |= Qt::ItemIsEnabled
            | Qt::ItemIsSelectable;

//    if(index.column() == 0)
//    {
//        flags |= Qt::ItemIsUserCheckable;
//    }
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
    if(!zv_calibrationRepository)
    {
        return 0;
    }

    return zv_calibrationRepository->zp_calibrationCount();

}
//==================================================================
QVariant ZCalibrationModel::data(const QModelIndex & index, int role) const
{
    if(!zv_calibrationRepository || !index.isValid()
            || index.row() < 0 || index.row() >=  zv_calibrationRepository->zp_calibrationCount()
            || index.column() < 0 || index.column() >= zv_calibrationRepository->zp_calibrationCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            return QVariant(zv_calibrationRepository->zp_calibrationName(index.row()));
        }
    }

    if(role == VisibleRole)
    {
        if(index.column() == 0)
        {
            return QVariant(zv_calibrationRepository->zp_calibrationIsVisible(index.row()));
        }
    }

    if(role == Qt::ForegroundRole)
    {
        if(zv_calibrationRepository->zp_isDirty(index.row()))
        {
            return QVariant(QColor(Qt::red));
        }
        else
        {
            return QVariant();
        }
    }

    if(role == Qt::DecorationRole)
    {
        if(index.column() == 0)
        {
            return QVariant(QColor(Qt::transparent));
        }
    }

    return QVariant();
}
//==================================================================
bool	ZCalibrationModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!zv_calibrationRepository || !index.isValid()
            || index.row() < 0 || index.row() >=  zv_calibrationRepository->zp_calibrationCount()
            || index.column() < 0 || index.column() >= zv_calibrationRepository->zp_calibrationCount()
            || !value.isValid())
    {
        return false;
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0 )
        {
            return false;
        }
    }
    else if(role == VisibleRole)
    {
        if(index.column() == 0)
        {
            if(!value.canConvert<bool>())
            {
                return false;
            }

            bool visibility = value.toBool();
            return zv_calibrationRepository->zp_setVisible(index.row(), visibility);
        }
    }

    return false;
}
//==================================================================
QVariant ZCalibrationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            if(section == 0)
            {
                return QVariant(tr("Calibration"));
            }
//            if(section == 1)
//            {
//                return QVariant(tr("Type"));
//            }

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
void ZCalibrationModel::zp_setCalibrationRepository(ZCalibrationRepository* repository)
{
    beginResetModel();

    zv_calibrationRepository = repository;
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_currentOperation,
            this, &ZCalibrationModel::zh_onCalibrationRepositoryOperation);

    endResetModel();
}
//==================================================================
void ZCalibrationModel::zh_onCalibrationRepositoryOperation(ZCalibrationRepository::OperationType type, int first, int last)
{
    if(type == ZCalibrationRepository::OT_BEGIN_RESET)
    {
        beginResetModel();
    }
    else if(type == ZCalibrationRepository::OT_END_RESET)
    {
        endResetModel();
    }
    else if(type == ZCalibrationRepository::OT_INSERT_CALIBRATIONS)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZCalibrationRepository::OT_END_INSERT_CALIBRATIONS)
    {
        endInsertRows();
    }
    else if(type == ZCalibrationRepository::OT_REMOVE_CALIBRATIONS)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == ZCalibrationRepository::OT_END_REMOVE_CALIBRATIONS)
    {
        endRemoveRows();
    }
}
//==================================================================
