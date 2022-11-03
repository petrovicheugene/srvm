//==================================================================
#include "X_CalibrationModel.h"
#include "X_General.h"

#include <QFont>
#include <QColor>
//==================================================================
X_CalibrationModel::X_CalibrationModel(QObject *parent) : QAbstractTableModel(parent)
{
    xv_calibrationRepository = 0;
}
//==================================================================
Qt::ItemFlags	X_CalibrationModel::flags(const QModelIndex & index) const
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
int X_CalibrationModel::columnCount(const QModelIndex & parent) const
{
    return 8;
}
//==================================================================
int X_CalibrationModel::rowCount(const QModelIndex & parent) const
{
    if(!xv_calibrationRepository)
    {
        return 0;
    }

    return xv_calibrationRepository->xp_calibrationCount();
}
//==================================================================
QVariant X_CalibrationModel::data(const QModelIndex & index, int role) const
{
    if(!xv_calibrationRepository || !index.isValid()
            || index.row() < 0 || index.row() >=  xv_calibrationRepository->xp_calibrationCount()
            || index.column() < 0 || index.column() >= columnCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            return QVariant(xv_calibrationRepository->xp_calibrationName(index.row()));
        }
        else if(index.column() == 1)
        {
            return QVariant(xv_calibrationRepository->xp_chemElement(index.row()));
        }
        else if(index.column() == 2)
        {
            X_Calibration::EquationType equationType =  xv_calibrationRepository->xp_equationType(index.row());
            QString equationString = X_Calibration::xp_equationTypeString(equationType);
            if(equationType == X_Calibration::ET_FRACTIONAL)
            {
                QString baseString = xv_calibrationRepository->xp_baseTermString(index.row());
                if(baseString.isEmpty())
                {
                    equationString += tr(", no base");
                }
                else
                {
                    equationString += tr(", base: ") + baseString;
                }
            }
            return QVariant(equationString);
        }
        else if(index.column() == 3)
        {
            X_TermNormalizer::NormaType type = xv_calibrationRepository->xp_normaType(index.row());
            QString normaType = X_TermNormalizer::xp_normaTypeString(type);
            return QVariant(normaType);
        }
        else if(index.column() == 4)
        {
            QString interceptString = xv_calibrationRepository->xp_equationInterceptString(index.row());
            return QVariant(interceptString);
        }
        else if(index.column() == 5)
        {
            qint64 calibrationId = xv_calibrationRepository->xp_calibrationIdForCalibrationIndex(index.row());
            if(!xv_qualityData.contains(calibrationId))
            {
                return QVariant();
            }
            X_CalibrationQualityData calibrationQualityData = xv_calibrationRepository->xp_calibrationQualityData(calibrationId);
            return calibrationQualityData.determination;

            // return xv_qualityData.value(calibrationId).determination;
        }
        else if(index.column() == 6)
        {
            qint64 calibrationId = xv_calibrationRepository->xp_calibrationIdForCalibrationIndex(index.row());
            if(!xv_qualityData.contains(calibrationId))
            {
                return QVariant();
            }

            X_CalibrationQualityData calibrationQualityData = xv_calibrationRepository->xp_calibrationQualityData(calibrationId);
            return calibrationQualityData.adj_determination;

            //return xv_qualityData.value(calibrationId).adj_determination;
        }
        else if(index.column() == 7)
        {
            qint64 calibrationId = xv_calibrationRepository->xp_calibrationIdForCalibrationIndex(index.row());
            if(!xv_qualityData.contains(calibrationId))
            {
                return QVariant();
            }

            X_CalibrationQualityData calibrationQualityData = xv_calibrationRepository->xp_calibrationQualityData(calibrationId);
            return calibrationQualityData.standardDeviation;

            //return xv_qualityData.value(calibrationId).standardDeviation;
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
            X_EquationSettingsData equationData = xv_calibrationRepository->xp_equationDataForSettings(index.row());
            return QVariant::fromValue(equationData);
        }

        if(index.column() == 3)
        {
            X_NormaSettingsData normaData = xv_calibrationRepository->xp_normaSettingsData(index.row());
            return QVariant::fromValue(normaData);
        }

    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() == 0)
        {
            return QVariant(xv_calibrationRepository->xp_isCalibrationVisible(index.row()));
        }
    }

    if(role == Qt::ForegroundRole)
    {
        if(xv_calibrationRepository->xp_isDirty(index.row()))
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
            return QVariant(xv_calibrationRepository->xp_calibrationColor(index.row()));
        }
    }

    return QVariant();
}
//==================================================================
bool	X_CalibrationModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!xv_calibrationRepository || !index.isValid()
            || index.row() < 0 || index.row() >=  xv_calibrationRepository->xp_calibrationCount()
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
            return xv_calibrationRepository->xp_setCalibrationName(index.row(), value.toString());
        }

        if(index.column() == 1 )
        {
            if(!value.canConvert<QString>())
            {
                return false;
            }
            return xv_calibrationRepository->xp_setChemElement(index.row(), value.toString());
        }

        if(index.column() == 2 )
        {
            if(!value.isValid() || value.isNull() || !value.canConvert<X_EquationSettingsData>())
            {
                return false;
            }

            X_EquationSettingsData equationSetting = value.value<X_EquationSettingsData>();
            return xv_calibrationRepository->xp_setEquationDataFromSettings(index.row(), equationSetting);
        }

        if(index.column() == 3)
        {
            if(!value.isValid() || value.isNull() || !value.canConvert<X_NormaSettingsData>())
            {
                return false;
            }

            X_NormaSettingsData equationSetting = value.value<X_NormaSettingsData>();
            return xv_calibrationRepository->xp_setNormaSettings(index.row(), equationSetting);
        }

        if(index.column() == 4)
        {
            if(!value.isValid() || value.isNull() || !value.canConvert<QString>())
            {
                return false;
            }
            QString interceptValue = value.toString();
            return xv_calibrationRepository->xp_setEquationInterceptString(index.row(), interceptValue);
        }

        return false;
    }
    else if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() == 0)
        {
            if(!value.canConvert<bool>())
            {
                return false;
            }

            bool visibility = value.toBool();
            return xv_calibrationRepository->xp_setCalibrationVisible(index.row(), visibility);
        }
    }

    return false;
}
//==================================================================
QVariant X_CalibrationModel::headerData(int section, Qt::Orientation orientation, int role) const
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
                header = tr("Normalization");
            }
            else if(section == 4)
            {
                header = tr("Intercept");
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
void X_CalibrationModel::xp_connectToCalibrationRepository(X_CalibrationRepository* repository)
{
    beginResetModel();

    xv_calibrationRepository = repository;
    connect(xv_calibrationRepository, &X_CalibrationRepository::xg_calibrationOperation,
            this, &X_CalibrationModel::zh_onCalibrationRepositoryOperation);

    endResetModel();
}
//==================================================================
void X_CalibrationModel::xp_calibrationQualityDataChanged(bool saveToCalibration,
                                                         qint64 calibrationId,
                                                         X_CalibrationQualityData qualityData)
{
    xv_qualityData.insert(calibrationId, qualityData);

    int row = -1;
    for(int i = 0; i < xv_calibrationRepository->xp_calibrationCount(); i++)
    {
        if(xv_calibrationRepository->xp_calibrationIdForCalibrationIndex(i) == calibrationId)
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
void X_CalibrationModel::zh_onCalibrationRepositoryOperation(X_CalibrationRepository::CalibrationOperationType type, int first, int last)
{
    if(type == X_CalibrationRepository::COT_BEGIN_RESET)
    {
        beginResetModel();
    }
    else if(type == X_CalibrationRepository::COT_END_RESET)
    {
        xv_qualityData.clear();
        endResetModel();
    }
    else if(type == X_CalibrationRepository::COT_INSERT_CALIBRATIONS)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == X_CalibrationRepository::COT_END_INSERT_CALIBRATIONS)
    {
        endInsertRows();
    }
    else if(type == X_CalibrationRepository::COT_REMOVE_CALIBRATIONS)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == X_CalibrationRepository::COT_END_REMOVE_CALIBRATIONS)
    {
        endRemoveRows();
        qint64 calibrationId;
        for(int ci = first; ci <= last; ci++)
        {
            calibrationId = xv_calibrationRepository->xp_calibrationIdForCalibrationIndex(ci);
            xv_qualityData.remove(calibrationId);
        }
    }
    else if(type == X_CalibrationRepository::COT_CALIBRATION_FREE_MEMBER_CHANGED)
    {
        QModelIndex firstIndex = index(first, 4);
        QModelIndex lastIndex = index(last, 4);
        emit dataChanged(firstIndex, lastIndex);
    }
    else if(type == X_CalibrationRepository::COT_CALIBRATION_NAME_CHANGED)
    {
        QModelIndex firstIndex = index(first, 0);
        QModelIndex lastIndex = index(last, 0);
        emit dataChanged(firstIndex, lastIndex);
    }
    else if(type == X_CalibrationRepository::COT_CALIBRATION_CHEM_ELEMENT_CHANGED)
    {
        QModelIndex firstIndex = index(first, 1);
        QModelIndex lastIndex = index(last, 1);
        emit dataChanged(firstIndex, lastIndex);
    }
    else if(type == X_CalibrationRepository::COT_CALIBRATION_EQUATION_BASE_TERM_CHANGED)
    {
        QModelIndex firstIndex = index(first, 2);
        QModelIndex lastIndex = index(last, 2);
        emit dataChanged(firstIndex, lastIndex);
    }
    else if(type == X_CalibrationRepository::COT_CALIBRATION_EQUATION_TYPE_CHANGED)
    {
        QModelIndex firstIndex = index(first, 2);
        QModelIndex lastIndex = index(last, 2);
        emit dataChanged(firstIndex, lastIndex);
    }
    else if(type == X_CalibrationRepository::COT_CALIBRATION_NORMA_CHANGED)
    {
        QModelIndex firstIndex = index(first, 3);
        QModelIndex lastIndex = index(last, 3);
        emit dataChanged(firstIndex, lastIndex);
    }
    else if(type == X_CalibrationRepository::COT_CALIBRATION_DIRTY_CHANGED)
    {
        QModelIndex firstIndex = index(first, 0);
        QModelIndex lastIndex = index(last, columnCount() - 1);
        emit dataChanged(firstIndex, lastIndex);
    }
}
//==================================================================
