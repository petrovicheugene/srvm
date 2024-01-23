//==================================================================
#include "X_TermCorrelationTableModel.h"
#include "X_General.h"
#include <QFont>
#include <QApplication>
#include <QStyle>
#include <QPainter>
//==================================================================
X_TermCorrelationTableModel::X_TermCorrelationTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    xv_dataManager = 0;
}
//==================================================================
Qt::ItemFlags X_TermCorrelationTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;

    if(!index.isValid())
    {
        return flags;
    }

    flags |= Qt::ItemIsEnabled;

    if(index.column() == 0)
    {
        if(xv_dataManager && xv_dataManager->xp_isRowEditable(index.row()))
        {
            flags |= Qt::ItemIsEditable;
        }
    }

    return flags;
}
//==================================================================
int X_TermCorrelationTableModel::columnCount(const QModelIndex & parent) const
{
    if(!xv_dataManager)
    {
        return 0;
    }
    // xv_columnToRemoveNumber used only while column removing
    return xv_dataManager->xp_columnCount();
}
//==================================================================
int X_TermCorrelationTableModel::rowCount(const QModelIndex & parent) const
{
    if(!xv_dataManager)
    {
        return 0;
    }

    return xv_dataManager->xp_rowCount();;
}
//==================================================================
QVariant	X_TermCorrelationTableModel::data(const QModelIndex & index, int role) const
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

    if(role == Qt::DecorationRole)
    {
        //       if(index.column() == 0)
        //       {
        //           return QVariant(xv_dataManager->xp_windowColor(index.row()));
        //       }
    }

    if(role == Qt::TextAlignmentRole)
    {
        Qt::Alignment alignment;
        alignment = Qt::AlignRight | Qt::AlignVCenter;
        return QVariant(alignment);
    }

    if(role == Qt::BackgroundRole)
    {
        return xv_dataManager->xp_cellColor(index);
    }

    return QVariant();
}
//==================================================================
bool X_TermCorrelationTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!index.isValid() || !xv_dataManager || !value.isValid())
    {
        return false;
    }

    if(role == Qt::EditRole)
    {
        if(index.column() == 0)
        {
            return xv_dataManager->xp_setData(index, value);
        }
    }

    return false;
}
//==================================================================
QVariant	X_TermCorrelationTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(!xv_dataManager)
    {
        return QVariant();
    }


    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            return QVariant(xv_dataManager->xp_horizontalColumnName(section));
        }

        if(orientation == Qt::Vertical)
        {
            return QVariant(xv_dataManager->xp_verticalColumnName(section));
        }
    }

    //    if(role == Qt::DecorationRole)
    //    {
    //        if(orientation == Qt::Vertical)
    //        {
    //            return QVariant(QColor(Qt::red));
    //        }
    //    }

//    if(role == Qt::FontRole)
//    {
//        if(orientation == Qt::Horizontal)
//        {
//            QFont font = QAbstractItemModel::headerData(section, orientation, role).value<QFont>();
//            font.setBold(true);
//            return QVariant(font);
//        }
//    }

//    if(role == Qt::ForegroundRole)
//    {
//        if(orientation == Qt::Horizontal)
//        {
//            return QVariant(QColor(Qt::darkGreen));
//        }
//    }

    if(role == NS_DataRole::IconRole)
    {
        if(orientation == Qt::Vertical)
        {
            return QVariant(xv_dataManager->xp_termStateIcon(section));
        }
    }

    return QVariant();
}
//==================================================================
void X_TermCorrelationTableModel::xp_connectToTermCorrelationTableManager(X_TermCorrelationTableManager* dataManager)
{
    beginResetModel();
    xv_dataManager = dataManager;
    // data manager <-> joint model
    connect(xv_dataManager, &X_TermCorrelationTableManager::xg_currentOperation,
            this, &X_TermCorrelationTableModel::xh_onDataManagerOperation);

    connect(this, &X_TermCorrelationTableModel::xg_currentTermChanged,
            xv_dataManager, &X_TermCorrelationTableManager::xg_currentTermChanged);
    connect(this, &X_TermCorrelationTableModel::xg_termDoubleClicked,
            xv_dataManager, &X_TermCorrelationTableManager::xg_termDoubleClicked);

    endResetModel();
}
//==================================================================
void X_TermCorrelationTableModel::xp_onUserChangesTermState(int termLogIndex)
{
    if(!xv_dataManager)
    {
        return;
    }

    xv_dataManager->xp_setNextUsersTermState(termLogIndex);
}
//==================================================================
void X_TermCorrelationTableModel::xh_onDataManagerOperation(X_TermCorrelationTableManager::OperationType type, int first, int last)
{
    if(!xv_dataManager)
    {
        return;
    }

    if(type == X_TermCorrelationTableManager::TOT_BEGIN_RESET)
    {
        beginResetModel();
    }
    else if(type == X_TermCorrelationTableManager::TOT_END_RESET)
    {
        endResetModel();
    }
    else if(type == X_TermCorrelationTableManager::TOT_BEGIN_INSERT_ROW)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else if(type == X_TermCorrelationTableManager::TOT_END_INSERT_ROW)
    {
        endInsertRows();
    }
    else if(type == X_TermCorrelationTableManager::TOT_BEGIN_REMOVE_ROW)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else if(type == X_TermCorrelationTableManager::TOT_END_REMOVE_ROW)
    {
        endRemoveRows();
    }
    else if(type == X_TermCorrelationTableManager::TOT_BEGIN_INSERT_COLUMN)
    {
        beginInsertColumns(QModelIndex(), first, last);
    }
    else if(type == X_TermCorrelationTableManager::TOT_END_INSERT_COLUMN)
    {
        endInsertColumns();
    }
    else if(type == X_TermCorrelationTableManager::TOT_BEGIN_REMOVE_COLUMN)
    {
        beginRemoveColumns(QModelIndex(), first, last);
    }
    else if(type == X_TermCorrelationTableManager::TOT_END_REMOVE_COLUMN)
    {
        endRemoveColumns();
    }
    else if(type == X_TermCorrelationTableManager::TOT_DATA_CHANGED)
    {
        QModelIndex leftTop = index(first, 0);
        QModelIndex rightBottom = index(last, xv_dataManager->xp_columnCount() - 1);
        emit dataChanged(leftTop, rightBottom);
    }
    else if(type == X_TermCorrelationTableManager::TOT_HORIX_ONTAL_HEADER_CHANGED)
    {
        emit headerDataChanged(Qt::Horizontal, first, last);
    }
    else if(type == X_TermCorrelationTableManager::TOT_VERTICAL_HEADER_CHANGED)
    {
        emit headerDataChanged(Qt::Vertical, first, last);
    }

}
//==================================================================

