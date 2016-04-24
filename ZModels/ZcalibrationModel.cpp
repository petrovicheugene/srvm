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

    if(index.column() <= 4)
    {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}
//==================================================================
int ZCalibrationModel::columnCount(const QModelIndex & parent) const
{
    return 8;
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
            || index.column() < 0 || index.column() >= columnCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            return QVariant(zv_calibrationRepository->zp_calibrationName(index.row()));
        }
        else if(index.column() == 1)
        {
            return QVariant(zv_calibrationRepository->zp_chemElement(index.row()));
        }
        else if(index.column() == 2)
        {
            ZCalibration::EquationType equationType =  zv_calibrationRepository->zp_equationType(index.row());
            QString equationString = ZCalibration::zp_equationTypeString(equationType);
            if(equationType == ZCalibration::ET_FRACTIONAL)
            {
                QString baseString = zv_calibrationRepository->zp_baseMemberString(index.row());
                if(baseString.isEmpty())
                {
                    equationString += tr(", no base");
                }
                else
                {
                    equationString += tr(", base:") + baseString;
                }
            }
            return QVariant(equationString);
        }
        else if(index.column() == 3)
        {
            ZTermNormalizer::NormaType type = zv_calibrationRepository->zp_normaType(index.row());
            QString normaType = ZTermNormalizer::zp_normaTypeString(type);
            return QVariant(normaType);
        }
        else if(index.column() == 4)
        {
            QString freeMemberString = zv_calibrationRepository->zp_equationFreeMemeberString(index.row());
            return QVariant(freeMemberString);
        }
        else if(index.column() == 5)
        {
            qint64 calibrationId = zv_calibrationRepository->zp_calibrationIdForCalibrationIndex(index.row());
            if(!zv_qualityData.contains(calibrationId))
            {
                return QVariant();
            }

            return zv_qualityData.value(calibrationId).determination;
        }
        else if(index.column() == 6)
        {
            qint64 calibrationId = zv_calibrationRepository->zp_calibrationIdForCalibrationIndex(index.row());
            if(!zv_qualityData.contains(calibrationId))
            {
                return QVariant();
            }

            return zv_qualityData.value(calibrationId).adj_determination;
        }
        else if(index.column() == 7)
        {
            qint64 calibrationId = zv_calibrationRepository->zp_calibrationIdForCalibrationIndex(index.row());
            if(!zv_qualityData.contains(calibrationId))
            {
                return QVariant();
            }

            return zv_qualityData.value(calibrationId).standardDeviation;
        }
    }

    if(role == Qt::TextAlignmentRole)
    {
        if(index.column() == 4)
        {
            Qt::Alignment flag = Qt::AlignRight | Qt::AlignVCenter;
            return QVariant(flag);
        }
    }

    if(role == Qt::EditRole)
    {
        if(index.column() == 2)
        {
            ZEquationSettingsData equationData = zv_calibrationRepository->zp_equationDataForSettings(index.row());
            return QVariant::fromValue(equationData);
        }

        if(index.column() == 3)
        {
            ZNormaSettingsData normaData = zv_calibrationRepository->zp_normaSettingsData(index.row());
            return QVariant::fromValue(normaData);
        }

    }

    if(role == VisibleRole)
    {
        if(index.column() == 0)
        {
            return QVariant(zv_calibrationRepository->zp_isCalibrationVisible(index.row()));
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
            return QVariant(zv_calibrationRepository->zp_calibrationColor(index.row()));
        }
    }

    return QVariant();
}
//==================================================================
bool	ZCalibrationModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!zv_calibrationRepository || !index.isValid()
            || index.row() < 0 || index.row() >=  zv_calibrationRepository->zp_calibrationCount()
            || index.column() < 0 || index.column() >= columnCount()
            || !value.isValid())
    {
        return false;
    }

    if(role == Qt::EditRole)
    {
        if(index.column() == 0 )
        {
            if(!value.canConvert<QString>())
            {
                return false;
            }
            return zv_calibrationRepository->zp_setCalibrationName(index.row(), value.toString());
        }

        if(index.column() == 1 )
        {
            if(!value.canConvert<QString>())
            {
                return false;
            }
            return zv_calibrationRepository->zp_setChemElement(index.row(), value.toString());
        }

        if(index.column() == 2 )
        {
            if(!value.isValid() || value.isNull() || !value.canConvert<ZEquationSettingsData>())
            {
                return false;
            }

            ZEquationSettingsData equationSetting = value.value<ZEquationSettingsData>();
            return zv_calibrationRepository->zp_setEquationDataFromSettings(index.row(), equationSetting);
        }

        if(index.column() == 3)
        {
            if(!value.isValid() || value.isNull() || !value.canConvert<ZNormaSettingsData>())
            {
                return false;
            }

            ZNormaSettingsData equationSetting = value.value<ZNormaSettingsData>();
            return zv_calibrationRepository->zp_setNormaSettings(index.row(), equationSetting);
        }

        if(index.column() == 4)
        {
            if(!value.isValid() || value.isNull() || !value.canConvert<QString>())
            {
                return false;
            }
            QString freeMemberValue = value.toString();
            return zv_calibrationRepository->zp_setEquationFreeMemberString(index.row(), freeMemberValue);
        }

        return false;
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
            QString header;
            if(section == 0)
            {
                header = tr("Calibration");
            }
            else if(section == 1)
            {
                header = tr("Chem. element");
            }
            else if(section == 2)
            {
                header = tr("Equation");
            }
            else if(section == 3)
            {
                header = tr("Norma");
            }
            else if(section == 4)
            {
                header = tr("Free member");
            }
            else if(section == 5)
            {
                header = QString("R%1").arg(QChar(0x00B2));
            }
            else if(section == 6)
            {
                header = QString("R%1 adj.").arg(QChar(0x00B2));
            }
//            else if(section == 7)
//            {
//                header = "\u03C3";
//            }
            else if(section == 7)
            {
                header = "s";
            }

            return QVariant(header);
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
void ZCalibrationModel::zp_connectToCalibrationRepository(ZCalibrationRepository* repository)
{
    beginResetModel();

    zv_calibrationRepository = repository;
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_calibrationOperation,
            this, &ZCalibrationModel::zh_onCalibrationRepositoryOperation);

    endResetModel();
}
//==================================================================
void ZCalibrationModel::zp_calibrationQualityDataChanged(qint64 calibrationId, ZCalibrationQualityData qualityData)
{
    zv_qualityData.insert(calibrationId, qualityData);

    int row = -1;
    for(int i = 0; i < zv_calibrationRepository->zp_calibrationCount(); i++)
    {
        if(zv_calibrationRepository->zp_calibrationIdForCalibrationIndex(i) == calibrationId)
        {
            row = i;
            break;
        }
    }

    if(row < 0)
    {
        return;
    }

    QModelIndex firstIndex = index(row, 5);
    QModelIndex lastIndex = index(row, 7);

    emit dataChanged(firstIndex, lastIndex);

}
//==================================================================
void ZCalibrationModel::zh_onCalibrationRepositoryOperation(ZCalibrationRepository::CalibrationOperationType type, int first, int last)
{
    if(type == ZCalibrationRepository::COT_BEGIN_RESET)
    {
        beginResetModel();
    }
    else if(type == ZCalibrationRepository::COT_END_RESET)
    {
        zv_qualityData.clear();
        endResetModel();
    }
    else if(type == ZCalibrationRepository::COT_INSERT_CALIBRATIONS)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZCalibrationRepository::COT_END_INSERT_CALIBRATIONS)
    {
        endInsertRows();
    }
    else if(type == ZCalibrationRepository::COT_REMOVE_CALIBRATIONS)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == ZCalibrationRepository::COT_END_REMOVE_CALIBRATIONS)
    {
        endRemoveRows();
        qint64 calibrationId;
        for(int ci = first; ci <= last; ci++)
        {
            calibrationId = zv_calibrationRepository->zp_calibrationIdForCalibrationIndex(ci);
            zv_qualityData.remove(calibrationId);
        }
    }
    else if(type == ZCalibrationRepository::COT_CALIBRATION_FREE_MEMBER_CHANGED)
    {
        QModelIndex firstIndex = index(first, 4);
        QModelIndex lastIndex = index(last, 4);
        emit dataChanged(firstIndex, lastIndex);
    }
    else if(type == ZCalibrationRepository::COT_CALIBRATION_NAME_CHANGED)
    {
        QModelIndex firstIndex = index(first, 0);
        QModelIndex lastIndex = index(last, 0);
        emit dataChanged(firstIndex, lastIndex);
    }
    else if(type == ZCalibrationRepository::COT_CALIBRATION_CHEM_ELEMENT_CHANGED)
    {
        QModelIndex firstIndex = index(first, 1);
        QModelIndex lastIndex = index(last, 1);
        emit dataChanged(firstIndex, lastIndex);
    }
    else if(type == ZCalibrationRepository::COT_CALIBRATION_EQUATION_BASE_TERM_CHANGED)
    {
        QModelIndex firstIndex = index(first, 2);
        QModelIndex lastIndex = index(last, 2);
        emit dataChanged(firstIndex, lastIndex);
    }
    else if(type == ZCalibrationRepository::COT_CALIBRATION_EQUATION_TYPE_CHANGED)
    {
        QModelIndex firstIndex = index(first, 2);
        QModelIndex lastIndex = index(last, 2);
        emit dataChanged(firstIndex, lastIndex);
    }
    else if(type == ZCalibrationRepository::COT_CALIBRATION_NORMA_CHANGED)
    {
        QModelIndex firstIndex = index(first, 3);
        QModelIndex lastIndex = index(last, 3);
        emit dataChanged(firstIndex, lastIndex);
    }
}
//==================================================================
