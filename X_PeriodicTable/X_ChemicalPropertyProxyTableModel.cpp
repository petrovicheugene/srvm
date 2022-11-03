//====================================================
#include "X_ChemicalPropertyProxyTableModel.h"
#include "X_ChemicalElementPropertyTreeModel.h"

#include <QAbstractItemModel>
#include <QDebug>

//====================================================
X_ChemicalPropertyProxyTableModel::X_ChemicalPropertyProxyTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    xv_itemIsEditable = true;
    xv_currentSourceModelIndex = QModelIndex();
    xv_headerList.append(tr("Property"));
    xv_headerList.append(tr("Value"));

}
//====================================================
Qt::ItemFlags	X_ChemicalPropertyProxyTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags	flags;
    flags |= Qt::ItemIsEnabled
            | Qt::ItemIsSelectable;
    if(xv_itemIsEditable)
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}
//====================================================
int	 X_ChemicalPropertyProxyTableModel::columnCount(const QModelIndex & parent) const
{
    return 2;
}
//====================================================
int	 X_ChemicalPropertyProxyTableModel::rowCount(const QModelIndex & parent) const
{
    if(!xv_currentSourceModelIndex.isValid() || !xv_currentSourceModelIndex.model())
    {
        return 0;
    }

    QVariant vData = xv_currentSourceModelIndex.data(PropertyCountRole);
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return 0;
    }

    return vData.toInt();
}
//====================================================
QVariant	X_ChemicalPropertyProxyTableModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid() || index.row() < 0 || index.row() >= rowCount()
            || index.column() < 0 || index.column() >= columnCount()
            || !xv_currentSourceModelIndex.model())
    {
        return QVariant();
    }

    const QAbstractItemModel* model = xv_currentSourceModelIndex.model();
    if(!model)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        // define data role
        int newRole;
        if(index.column() == 0)
        {
            newRole = PropertyNameRole;
        }
        else // column == 1
        {
            newRole = PropertyValueRole;
        }

        // create new source index
        // proxy index.row converts to src column
        QModelIndex newSrcIndex = model->index(xv_currentSourceModelIndex.row(), index.row(),
                                               xv_currentSourceModelIndex.parent());
        if(!newSrcIndex.isValid())
        {
            return QVariant();
        }

        return newSrcIndex.data(newRole);
    }

    return QVariant();
}
//====================================================
bool X_ChemicalPropertyProxyTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(role == Qt::EditRole)
    {
        // define data role
        int newRole;
        if(index.column() == 0)
        {
            newRole = PropertyNameRole;
        }
        else // column == 1
        {
            newRole = PropertyValueRole;
        }

        const QAbstractItemModel* model = xv_currentSourceModelIndex.model();
        if(!model)
        {
            return false;
        }

        QModelIndex newSrcIndex = model->index(xv_currentSourceModelIndex.row(), index.row(),
                                               xv_currentSourceModelIndex.parent());

        const_cast<QAbstractItemModel*>(model)->setData(newSrcIndex, value, newRole);
    }

    return false;
}
//====================================================
QVariant	X_ChemicalPropertyProxyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole)
        {
            return QVariant(xv_headerList.value(section));
        }
    }
    else // Qt::Vertical
    {
        if(role == Qt::DisplayRole)
        {
            return QVariant(QString::number(section + 1));
        }
    }

    return QVariant();
}
//====================================================
bool X_ChemicalPropertyProxyTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & data, int role)
{
    if(orientation != Qt::Horizontal || role != Qt::EditRole || section < 0 || section >= 2 )
    {
        return false;
    }

    while(xv_headerList.count() <= section)
    {
        xv_headerList.append(QString());
    }

    QString dataString;
    if(data.isValid() && data.canConvert<QString>())
    {
        dataString = data.toString();
    }

    xv_headerList[section] = dataString;

    return true;
}
//====================================================
void X_ChemicalPropertyProxyTableModel::xp_setItemIsEditable(bool editable)
{
    xv_itemIsEditable = editable;
}
//====================================================
void X_ChemicalPropertyProxyTableModel::xp_onCurrentTreeElementChanged(const QModelIndex& current,
                                                                      const QModelIndex& previous)
{
    beginResetModel();
    xv_currentSourceModelIndex = current;
    endResetModel();
}
//====================================================
void X_ChemicalPropertyProxyTableModel::xp_onSourceModelDataChanged(const QModelIndex& topLeft,
                                                                   const QModelIndex& bottomRight)
{
    if(topLeft.parent() == xv_currentSourceModelIndex.parent()
            && topLeft.row() == xv_currentSourceModelIndex.row())
    {
        beginResetModel();
        endResetModel();
    }
}
//====================================================
