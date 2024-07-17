//==================================================================
#include "X_JointSpectraModel.h"
#include "X_General.h"

#include <QFont>
#include <QColor>
#include <QApplication>
#include <QStyle>
#include <QStyledItemDelegate>
//==================================================================
X_JointSpectraModel::X_JointSpectraModel(QObject *parent) : QAbstractTableModel(parent)
{
    xv_dataManager = 0;
}
//==================================================================
Qt::ItemFlags	X_JointSpectraModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if(index.column() == 0)
    {
        flags |= Qt::ItemIsUserCheckable;
    }
    else if(xv_dataManager->xp_isColumnChemElement(index.column()))
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}
//==================================================================
int X_JointSpectraModel::columnCount(const QModelIndex & parent) const
{
    if(!xv_dataManager)
    {
        return 0;
    }

    return xv_dataManager->xp_columnCount();
}
//==================================================================
int X_JointSpectraModel::rowCount(const QModelIndex & parent) const
{
    if(!xv_dataManager)
    {
        return 0;
    }

    return xv_dataManager->xp_rowCount();
}
//==================================================================
QVariant X_JointSpectraModel::data(const QModelIndex & index, int role) const
{
    if(!xv_dataManager || !index.isValid()
            || index.row() < 0 || index.row() >=  rowCount()
            || index.column() < 0 || index.column() >= columnCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        return xv_dataManager->xp_data(index);
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
            return QVariant(xv_dataManager->xp_spectrumColor(index.row()));
        }
    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() == 1)
        {
            return QVariant(xv_dataManager->xp_isSpectrumVisible(index.row()));
        }
    }

    if(role == Qt::CheckStateRole)
    {
        if(index.column() == 0)
        {
            if(xv_dataManager->xp_isSpectrumChecked(index.row()))
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
bool X_JointSpectraModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!xv_dataManager || !index.isValid()
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
            bool res =  xv_dataManager->xp_setChemConcentration(index.row(), index.column(), value.toString());
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
            if(xv_dataManager->xp_setSpectrumVisible(index.row(), visible))
            {
                emit dataChanged(index, index);
                return true;
            }
        }
    }

    if(role == Qt::CheckStateRole)
    {
        if(index.column() == 0 && value.canConvert<int>())
        {
            Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt());
            bool checked = checkState == Qt::Checked ? true : false;
            if(xv_dataManager->xp_setSpectrumChecked(index.row(), checked))
            {
                emit dataChanged(index, index);
                return true;
            }

        }
    }

    return false;
}
//==================================================================
QVariant X_JointSpectraModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(!xv_dataManager)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            return QVariant(xv_dataManager->xp_columnName(section));
        }
        else
        {
            return QVariant(QString::number(section + 1));
        }
    }

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
void X_JointSpectraModel::xp_connectToSpectraDataManager(X_JointSpectraDataManager* dataManager)
{
    beginResetModel();
    xv_dataManager = dataManager;
    // data manager <-> joint model
    connect(dataManager, &X_JointSpectraDataManager::xg_currentOperation,
            this, &X_JointSpectraModel::xh_onDataManagerOperation);
    connect(dataManager, &X_JointSpectraDataManager::xg_requestCurrentIndex,
            this, &X_JointSpectraModel::xg_requestCurrentIndex);
    connect(this, &X_JointSpectraModel::xg_selectionChanged,
            dataManager, &X_JointSpectraDataManager::xg_onSelectionChange);

    connect(dataManager, &X_JointSpectraDataManager::xg_requestClearSelected,
            this, &X_JointSpectraModel::xp_clearSelectedConcentrations);
    connect(dataManager, &X_JointSpectraDataManager::xg_requestSelectedString,
            this, &X_JointSpectraModel::xp_convertSelectedToString);


    endResetModel();
}
//==================================================================
void X_JointSpectraModel::xh_onDataManagerOperation(X_JointSpectraDataManager::OperationType type, int first, int last)
{
    if(!xv_dataManager)
    {
        return;
    }

    if(type == X_JointSpectraDataManager::OT_BEGIN_RESET)
    {
        beginResetModel();
    }
    else if(type == X_JointSpectraDataManager::OT_END_RESET)
    {
        endResetModel();
    }
    else if(type == X_JointSpectraDataManager::OT_BEGIN_INSERT_ROW)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == X_JointSpectraDataManager::OT_END_INSERT_ROW)
    {
        endInsertRows();
    }
    else if(type == X_JointSpectraDataManager::OT_BEGIN_REMOVE_ROW)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == X_JointSpectraDataManager::OT_END_REMOVE_ROW)
    {
        endRemoveRows();
    }
    else if(type == X_JointSpectraDataManager::OT_BEGIN_INSERT_COLUMN)
    {
        beginInsertColumns(QModelIndex(), first, last);
    }
    else if(type == X_JointSpectraDataManager::OT_END_INSERT_COLUMN)
    {
        endInsertColumns();
    }
    else if(type == X_JointSpectraDataManager::OT_BEGIN_REMOVE_COLUMN)
    {
        beginRemoveColumns(QModelIndex(), first, last);
    }
    else if(type == X_JointSpectraDataManager::OT_END_REMOVE_COLUMN)
    {
        endRemoveColumns();
    }
    else if(type == X_JointSpectraDataManager::OT_COLUMN_HEADER_CHANGED)
    {
        emit headerDataChanged(Qt::Horizontal, first, last);
    }
    else if(type == X_JointSpectraDataManager::OT_SEPECTRUM_VISIBLE_CHANGED)
    {
        QModelIndex leftTop = index(first, 0);
        QModelIndex rightBottom = index(last, xv_dataManager->xp_spectrumDataColumnCount() - 1);

        emit dataChanged(leftTop, rightBottom);
    }
    else if(type == X_JointSpectraDataManager::OT_SEPECTRUM_CONCENTRATION_CHANGED)
    {
        QModelIndex leftTop = index(first, xv_dataManager->xv_spectrumDataColumnCount - 1);
        QModelIndex rightBottom = index(last, xv_dataManager->xv_spectrumDataColumnCount
                                        + xv_dataManager->xv_visibleChemElementCount);

        emit dataChanged(leftTop, rightBottom);
    }

    else if(type == X_JointSpectraDataManager::OT_COLUMN_DATA_CHANGED)
    {
        QModelIndex leftTop = index(0, first);
        QModelIndex rightBottom = index(xv_dataManager->xp_rowCount() - 1, last);

        emit dataChanged(leftTop, rightBottom);
    }
}
//==================================================================
void X_JointSpectraModel::xp_onSelectedIndexListChange(QModelIndexList selectedIndexList)
{
    xv_selectedIndexMap.clear();
    bool concentrationSelected = false;
    bool spectrumSelected = false;
    foreach(QModelIndex index, selectedIndexList)
    {
        // skip spectrum
        if(index.column() == 1)
        {
            spectrumSelected = true;
            continue;
        }

        // check chemElementColumns in selection
        if(!concentrationSelected)
        {
            if(index.column() >= xv_dataManager->xv_spectrumDataColumnCount
                    && index.column() < xv_dataManager->xv_visibleChemElementCount
                    + xv_dataManager->xv_spectrumDataColumnCount)
            {
                concentrationSelected = true;
            }
        }

        xv_selectedIndexMap[index.row()].insert(index.column(), index);
    }

    emit xg_selectionChanged(!xv_selectedIndexMap.isEmpty(), concentrationSelected, spectrumSelected);
}
//==================================================================
void X_JointSpectraModel::xp_clearSelectedConcentrations()
{
    QMap<int, QMap<int, QModelIndex> >::const_iterator rit;
    QMap<int, QModelIndex> columnList;
    QMap<int, QModelIndex>::const_iterator cit;
    for(rit = xv_selectedIndexMap.cbegin(); rit != xv_selectedIndexMap.cend(); rit++)
    {
        columnList = rit.value();
        for(cit = columnList.cbegin(); cit != columnList.cend(); cit++)
        {
            setData(cit.value(), QVariant("0.0"));
        }
    }
}
//==================================================================
void X_JointSpectraModel::xp_convertSelectedToString(QString& selectionString) const
{
    selectionString.clear();
    if(xv_selectedIndexMap.isEmpty())
    {
        return;
    }

    QMap<int, QMap<int, QModelIndex> >::const_iterator rit;
    QMap<int, QModelIndex> columnList;
    QMap<int, QModelIndex>::const_iterator cit;
    QVariant vData;
    QChar separator = '\t';

    for(rit = xv_selectedIndexMap.begin(); rit != xv_selectedIndexMap.end(); rit++)
    {
        columnList = rit.value();
        for(cit = columnList.cbegin(); cit != columnList.cend(); cit++)
        {
            vData = data(cit.value());
            if(!vData.isValid() && !vData.canConvert<QString>())
            {
                selectionString += separator;
                continue;
            }
            selectionString += vData.toString();

            // if(cit != columnList.end() - 1)
            if(cit.key() != columnList.lastKey())
            {
                selectionString += separator;
            }
        }

        // row is comlete
        // if(rit != xv_selectedIndexMap.end() - 1)
        if(rit.key() != xv_selectedIndexMap.lastKey())
        {
            selectionString.append('\n');
        }
    }
}
//==================================================================
