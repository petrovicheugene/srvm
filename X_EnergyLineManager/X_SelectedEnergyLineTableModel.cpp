//=============================================================
#include "X_SelectedEnergyLineTableModel.h"
#include "X_General.h"
#include "X_LocaleDoubleConverter.h"

#include <algorithm>
#include <QDebug>
#include <QLocale>
//=============================================================
//=============================================================
X_SelectedEnergyLineTableModel::X_SelectedEnergyLineTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}
//=============================================================
X_SelectedEnergyLineTableModel::~X_SelectedEnergyLineTableModel()
{

    // xv_itemMap.values();
}
//=============================================================
Qt::ItemFlags	X_SelectedEnergyLineTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags	flags;
    flags |= Qt::ItemIsEnabled;

    // flags |= Qt::ItemIsEditable;

    return flags;
}
//=============================================================
int	 X_SelectedEnergyLineTableModel::columnCount(const QModelIndex & parent) const
{
    return xv_columnHederList.count() + 1;
}
//=============================================================
int	 X_SelectedEnergyLineTableModel::rowCount(const QModelIndex & parent) const
{
    return xv_itemMap.count();
}
//=============================================================
QVariant X_SelectedEnergyLineTableModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid() || index.row() < 0 || index.row() >= rowCount() || index.column() < 0
            || index.column() >= columnCount())
    {
        return QVariant();
    }

    int X_Number = xv_itemMap.keys().at(index.row());
    QString energyLineName = xv_columnHederList.value(index.column() - 1, QString());
    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            QString symbolString = xv_itemMap.value(X_Number)->xp_symbol();
            return QVariant(symbolString);
        }
        else
        {
            QString energyLineValue;
            if(xv_itemMap.value(X_Number)->xp_energyLineValue(energyLineName, energyLineValue))
            {
                return QVariant(energyLineValue);
            }
        }
    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() == 0)
        {
            return QVariant();
        }
        else
        {
            bool visible;
            if(!xv_itemMap.value(X_Number)->xp_isEnergyLineVisible(energyLineName, visible))
            {
                return QVariant();
            }

            return QVariant(visible);
        }
    }

    if(role == Qt::DecorationRole)
    {
        if(index.column() == 0)
        {
            return QVariant();
        }
        else
        {
            QVariant vData = index.data(Qt::DisplayRole);
            if(!vData.isValid() || vData.isNull() || !vData.canConvert<QString>() ||
                    vData.toString().isEmpty())
            {
                return QVariant();
            }

            QColor energyLineColor;
            if(xv_itemMap.value(X_Number)->xp_energyLineColor(energyLineName, energyLineColor))
            {
                return QVariant(energyLineColor);
            }

            return QVariant(QColor(Qt::black));
        }
    }

    return QVariant();
}
//=============================================================
bool X_SelectedEnergyLineTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!index.isValid() || index.row() < 0 || index.row() >= rowCount() || index.column() < 1
            || index.column() >= columnCount() || !value.isValid() || !value.canConvert<bool>())
    {
        return false;
    }

    if(role ==  NS_DataRole::VisibleRole)
    {
        int X_Number = xv_itemMap.keys().at(index.row());
        QString energyLineName = xv_columnHederList.value(index.column() - 1, QString());
        return xv_itemMap.value(X_Number)->xp_setEnergyLineVisible(energyLineName, value.toBool());
    }

    return false;
}
//=============================================================
QVariant X_SelectedEnergyLineTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Vertical)
        {
            return QVariant(QString::number(section+1));
        }
        else
        {
            if(section == 0)
            {
                return QVariant(tr("Element"));
            }

            if(section - 1 < xv_columnHederList.count())
            {
                return QVariant(xv_columnHederList.at(section - 1));
            }
        }
    }

    return QVariant();
}
//=============================================================
bool X_SelectedEnergyLineTableModel::setHeaderData(int section, Qt::Orientation orientation,
                                                  const QVariant & data, int role)
{
    return false;
}
//=============================================================
void X_SelectedEnergyLineTableModel::xp_onSelectedChemicalElementChange(int X_Number,
                                                                       bool selected)
{
    if(selected)
    {
        if(!xv_itemMap.keys().contains(X_Number))
        {
            int row = xh_findRowToInsert(X_Number);
            QString symbol;
            emit xg_requestChemicalElementSymbol(X_Number, symbol);

            beginInsertRows(QModelIndex(), row, row);
            X_EnergyLineSetItem* item = new X_EnergyLineSetItem(X_Number,
                                                              symbol, this);
            connect(item, &X_EnergyLineSetItem::xg_energyLineOperation,
                    this, &X_SelectedEnergyLineTableModel::xg_energyLineOperation);

            xv_itemMap.insert(X_Number, item);
            xh_loadEnergyLinesToItem(item);

            endInsertRows();
        }
    }
    else
    {
        if(xv_itemMap.keys().contains(X_Number))
        {
            int row = xv_itemMap.keys().indexOf(X_Number);
            beginRemoveRows(QModelIndex(), row, row);

            X_EnergyLineSetItem* item  = xv_itemMap.take(X_Number);
            if(item)
            {
                delete item;
            }

            endRemoveRows();
        }
    }

    xh_updateColumns();
}
//=============================================================
void X_SelectedEnergyLineTableModel::xh_loadEnergyLinesToItem(X_EnergyLineSetItem* item)

{
    // energy values
    PropertyList propertyList;
    emit xg_requestEnergyLinesForX_Number(item->xp_X_Number(), propertyList);

    item->xp_createEnergyLines(propertyList);

    // relative intensity
    propertyList.clear();
    emit xg_requestEnergyLineRelativeIntensityForX_Number(item->xp_X_Number(), propertyList);
    item->xp_loadRelativeIntensity(propertyList);
}
//=============================================================
int X_SelectedEnergyLineTableModel::xh_findRowToInsert(int X_Number) const
{
    if(xv_itemMap.isEmpty())
    {
        return 0;
    }

    for(int row = 0; row < xv_itemMap.count(); row++)
    {
        if(xv_itemMap.keys().at(row) < X_Number)
        {
            continue;
        }

        return row;
    }

    return xv_itemMap.count();
}
//=============================================================
void X_SelectedEnergyLineTableModel::xh_updateColumns()
{
    // create new column header list
    QStringList newColumnHeaderList;

    QMap<int, X_EnergyLineSetItem*>::const_iterator it;
    QStringList elementEnergyNameStringList;
    for(it = xv_itemMap.begin(); it != xv_itemMap.end(); it++ )
    {
        elementEnergyNameStringList = it.value()->xp_energyLineNameStringList();
        foreach(auto& energyLineName, elementEnergyNameStringList)
        {
            if(!newColumnHeaderList.contains(energyLineName))
            {
                newColumnHeaderList.append(energyLineName);
            }
        }
    }

    // qSort(newColumnHeaderList);
    std::sort(newColumnHeaderList.begin(), newColumnHeaderList.end());
    // remove all columns except 0
    if(columnCount() > 1)
    {
        beginRemoveColumns(QModelIndex(), 1, columnCount()-1);
        xv_columnHederList.clear();
        endRemoveColumns();
    }

    if(!newColumnHeaderList.isEmpty())
    {
        // insert new columns
        beginInsertColumns(QModelIndex(), 1, newColumnHeaderList.count());
        xv_columnHederList = newColumnHeaderList;
        endInsertColumns();
    }

    return;
}
//=============================================================
bool X_SelectedEnergyLineTableModel::xp_energyLineEnergyValue(const QString& elementSymbol,
                       const QString& lineName,
                       double& energyValue) const
{
    QMap<int, X_EnergyLineSetItem*>::const_iterator it;
    for(it = xv_itemMap.begin(); it != xv_itemMap.end(); it++ )
    {
        if(it.value()->xp_symbol() != elementSymbol)
        {
            continue;
        }

        if(!it.value()->xp_energyLineNameStringList().contains(lineName))
        {
            continue;
        }

        QString energyValueString;
        if(!it.value()->xp_energyLineValue(lineName, energyValueString))
        {
            continue;
        }

        bool ok;
        energyValue = X_LocaleDoubleConverter::toDouble(energyValueString, &ok);
        if(!ok)
        {
            continue;
        }

        return true;
    }

    return false;
}
//=============================================================
bool X_SelectedEnergyLineTableModel::xp_energyLineReletiveIntensity(const QString& elementSymbol,
                                                                   const QString& lineName,
                                                                   int& relativeIntensity) const
{
    QMap<int, X_EnergyLineSetItem*>::const_iterator it;
    for(it = xv_itemMap.begin(); it != xv_itemMap.end(); it++ )
    {
        if(it.value()->xp_symbol() != elementSymbol)
        {
            continue;
        }

        if(!it.value()->xp_energyLineNameStringList().contains(lineName))
        {
            continue;
        }

        QString relativeIntensityString;
        if(!it.value()->xp_energyLineRelativeIntensity(lineName, relativeIntensityString))
        {
            continue;
        }

        bool ok;
        relativeIntensity = relativeIntensityString.toInt(&ok);
        if(!ok)
        {
            continue;
        }

        return true;
    }

    return false;
}
//=============================================================
bool X_SelectedEnergyLineTableModel::xp_energyLineVisibility(const QString& elementSymbol,
                                                            const QString& lineName,
                                                            bool& visible) const
{
    QMap<int, X_EnergyLineSetItem*>::const_iterator it;
    for(it = xv_itemMap.begin(); it != xv_itemMap.end(); it++ )
    {
        if(it.value()->xp_symbol() != elementSymbol)
        {
            continue;
        }

        if(!it.value()->xp_energyLineNameStringList().contains(lineName))
        {
            continue;
        }

        QString energyValueString;
        if(!it.value()->xp_isEnergyLineVisible(lineName, visible))
        {
            continue;
        }

        return true;
    }

    return false;
}
//=============================================================
bool X_SelectedEnergyLineTableModel::xp_energyLineColor(const QString& elementSymbol,
                       const QString& lineName,
                       QColor& color) const
{
    QMap<int, X_EnergyLineSetItem*>::const_iterator it;
    for(it = xv_itemMap.begin(); it != xv_itemMap.end(); it++ )
    {
        if(it.value()->xp_symbol() != elementSymbol)
        {
            continue;
        }

        if(!it.value()->xp_energyLineNameStringList().contains(lineName))
        {
            continue;
        }

        QString energyValueString;
        if(!it.value()->xp_energyLineColor(lineName, color))
        {
            continue;
        }

        return true;
    }

    return false;
}
//=============================================================

