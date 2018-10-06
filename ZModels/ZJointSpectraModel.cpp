//==================================================================
#include "ZJointSpectraModel.h"
#include "ZGeneral.h"

#include <QFont>
#include <QColor>
#include <QApplication>
#include <QStyle>
#include <QStyledItemDelegate>
//==================================================================
ZJointSpectraModel::ZJointSpectraModel(QObject *parent) : QAbstractTableModel(parent)
{
    zv_dataManager = 0;
}
//==================================================================
Qt::ItemFlags	ZJointSpectraModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if(index.column() == 0)
    {
        flags |= Qt::ItemIsUserCheckable;
    }
    else if(zv_dataManager->zp_isColumnChemElement(index.column()))
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}
//==================================================================
int ZJointSpectraModel::columnCount(const QModelIndex & parent) const
{
    if(!zv_dataManager)
    {
        return 0;
    }

    return zv_dataManager->zp_columnCount();
}
//==================================================================
int ZJointSpectraModel::rowCount(const QModelIndex & parent) const
{
    if(!zv_dataManager)
    {
        return 0;
    }

    return zv_dataManager->zp_rowCount();
}
//==================================================================
QVariant ZJointSpectraModel::data(const QModelIndex & index, int role) const
{
    if(!zv_dataManager || !index.isValid()
            || index.row() < 0 || index.row() >=  rowCount()
            || index.column() < 0 || index.column() >= columnCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        return zv_dataManager->zp_data(index);
    }

    if(role == Qt::TextAlignmentRole)
    {
        Qt::Alignment flag = Qt::AlignRight | Qt::AlignVCenter;
        return QVariant(flag);
    }

    if(role == Qt::DecorationRole)
    {
        if(index.column() == 1)
        {
            return QVariant(zv_dataManager->zp_spectrumColor(index.row()));
        }
    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() == 1)
        {
            return QVariant(zv_dataManager->zp_isSpectrumVisible(index.row()));
        }
    }

    if(role == Qt::CheckStateRole)
    {
        if(index.column() == 0)
        {
            if(zv_dataManager->zp_isSpectrumChecked(index.row()))
            {
                return QVariant(Qt::Checked);
            }
            else
            {
                return QVariant(Qt::Unchecked);
            }
        }
    }

    return QVariant();
}
//==================================================================
bool	ZJointSpectraModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!zv_dataManager || !index.isValid()
            || index.row() < 0 || index.row() >=  rowCount()
            || index.column() < 0 || index.column() >= columnCount()
            || !value.isValid())
    {
        return false;
    }

    if(role == Qt::EditRole)
    {
        if(value.canConvert<QString>())
        {
            bool res =  zv_dataManager->zp_setChemConcentration(index.row(), index.column(), value.toString());
            if(res)
            {
                emit dataChanged(index, index);
            }
        }
    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() == 1 && value.canConvert<bool>())
        {
            bool visible = value.toBool();
            return zv_dataManager->zp_setSpectrumVisible(index.row(), visible);
        }
    }

    if(role == Qt::CheckStateRole)
    {
        if(index.column() == 0 && value.canConvert<int>())
        {
            Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt());
            bool checked = checkState == Qt::Checked ? true : false;
            return zv_dataManager->zp_setSpectrumChecked(index.row(), checked);
        }
    }

    return false;
}
//==================================================================
QVariant ZJointSpectraModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(!zv_dataManager)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            return QVariant(zv_dataManager->zp_columnName(section));
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
void ZJointSpectraModel::zp_connectToSpectraDataManager(ZJointSpectraDataManager* dataManager)
{
    beginResetModel();
    zv_dataManager = dataManager;
    // data manager <-> joint model
    connect(dataManager, &ZJointSpectraDataManager::zg_currentOperation,
            this, &ZJointSpectraModel::zh_onDataManagerOperation);
    connect(dataManager, &ZJointSpectraDataManager::zg_requestCurrentIndex,
            this, &ZJointSpectraModel::zg_requestCurrentIndex);
    connect(this, &ZJointSpectraModel::zg_selectionChanged,
            dataManager, &ZJointSpectraDataManager::zg_onSelectionChange);

    connect(dataManager, &ZJointSpectraDataManager::zg_requestClearSelected,
            this, &ZJointSpectraModel::zp_clearSelectedConcentrations);
    connect(dataManager, &ZJointSpectraDataManager::zg_requestSelectedString,
            this, &ZJointSpectraModel::zp_convertSelectedToString);


    endResetModel();
}
//==================================================================
void ZJointSpectraModel::zh_onDataManagerOperation(ZJointSpectraDataManager::OperationType type, int first, int last)
{
    if(!zv_dataManager)
    {
        return;
    }

    if(type == ZJointSpectraDataManager::OT_BEGIN_RESET)
    {
        beginResetModel();
    }
    else if(type == ZJointSpectraDataManager::OT_END_RESET)
    {
        endResetModel();
    }
    else if(type == ZJointSpectraDataManager::OT_BEGIN_INSERT_ROW)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == ZJointSpectraDataManager::OT_END_INSERT_ROW)
    {
        endInsertRows();
    }
    else if(type == ZJointSpectraDataManager::OT_BEGIN_REMOVE_ROW)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == ZJointSpectraDataManager::OT_END_REMOVE_ROW)
    {
        endRemoveRows();
    }
    else if(type == ZJointSpectraDataManager::OT_BEGIN_INSERT_COLUMN)
    {
        beginInsertColumns(QModelIndex(), first, last);
    }
    else if(type == ZJointSpectraDataManager::OT_END_INSERT_COLUMN)
    {
        endInsertColumns();
    }
    else if(type == ZJointSpectraDataManager::OT_BEGIN_REMOVE_COLUMN)
    {
        beginRemoveColumns(QModelIndex(), first, last);
    }
    else if(type == ZJointSpectraDataManager::OT_END_REMOVE_COLUMN)
    {
        endRemoveColumns();
    }
    else if(type == ZJointSpectraDataManager::OT_COLUMN_HEADER_CHANGED)
    {
        emit headerDataChanged(Qt::Horizontal, first, last);
    }
    else if(type == ZJointSpectraDataManager::OT_SEPECTRUM_VISIBLE_CHANGED)
    {
        QModelIndex leftTop = index(first, 0);
        QModelIndex rightBottom = index(last, zv_dataManager->zp_spectrumDataColumnCount() - 1);

        emit dataChanged(leftTop, rightBottom);
    }
    else if(type == ZJointSpectraDataManager::OT_SEPECTRUM_CONCENTRATION_CHANGED)
    {
        QModelIndex leftTop = index(first, zv_dataManager->zv_spectrumDataColumnCount - 1);
        QModelIndex rightBottom = index(last, zv_dataManager->zv_spectrumDataColumnCount
                                        + zv_dataManager->zv_visibleChemElementCount);

        emit dataChanged(leftTop, rightBottom);
    }

    else if(type == ZJointSpectraDataManager::OT_COLUMN_DATA_CHANGED)
    {
        QModelIndex leftTop = index(0, first);
        QModelIndex rightBottom = index(zv_dataManager->zp_rowCount() - 1, last);

        emit dataChanged(leftTop, rightBottom);
    }
}
//==================================================================
void ZJointSpectraModel::zp_onSelectedIndexListChange(QModelIndexList selectedIndexList)
{
    zv_selectedIndexMap.clear();
    bool concentrationSelected = false;
    foreach(QModelIndex index, selectedIndexList)
    {
        // skip spectrum
        if(index.column() == 1)
        {
            continue;
        }

        // check chemElementColumns in selection
        if(!concentrationSelected)
        {
            if(index.column() >= zv_dataManager->zv_spectrumDataColumnCount
                    && index.column() < zv_dataManager->zv_visibleChemElementCount
                    + zv_dataManager->zv_spectrumDataColumnCount)
            {
                concentrationSelected = true;
            }
        }
        zv_selectedIndexMap[index.row()].insert(index.column(), index);
    }

    emit zg_selectionChanged(!zv_selectedIndexMap.isEmpty(), concentrationSelected);
}
//==================================================================
void ZJointSpectraModel::zp_clearSelectedConcentrations()
{
    QMap<int, QMap<int, QModelIndex> >::const_iterator rit;
    QMap<int, QModelIndex> columnList;
    QMap<int, QModelIndex>::const_iterator cit;
    for(rit = zv_selectedIndexMap.begin(); rit != zv_selectedIndexMap.end(); rit++)
    {
        columnList = rit.value();
        for(cit = columnList.begin(); cit != columnList.end(); cit++)
        {
            setData(cit.value(), QVariant("0.0"));
        }
    }
}
//==================================================================
void ZJointSpectraModel::zp_convertSelectedToString(QString& selectionString) const
{
    selectionString.clear();
    if(zv_selectedIndexMap.isEmpty())
    {
        return;
    }

    QMap<int, QMap<int, QModelIndex> >::const_iterator rit;
    QMap<int, QModelIndex> columnList;
    QMap<int, QModelIndex>::const_iterator cit;
    QVariant vData;
    QChar separator = '\t';
    for(rit = zv_selectedIndexMap.begin(); rit != zv_selectedIndexMap.end(); rit++)
    {
        columnList = rit.value();
        for(cit = columnList.begin(); cit != columnList.end(); cit++)
        {
            vData = data(cit.value());
            if(!vData.isValid() && !vData.canConvert<QString>())
            {
                selectionString += separator;
                continue;
            }
            selectionString += vData.toString();

            if(cit != columnList.end() - 1)
            {
                selectionString += separator;
            }
        }
        // row is comlete
        if(rit != zv_selectedIndexMap.end() - 1)
        {
            selectionString.append('\n');
        }
    }
}
//==================================================================
