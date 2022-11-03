//======================================================
#include "X_ChemicalElementPropertyTreeModel.h"
#include "X_ChemicalElementPropertyItem.h"
#include "X_ChemicalElementPropertyIOManager.h"

#include <QApplication>
#include <QDebug>
#include <QMetaType>
#include <QSettings>
//======================================================

//======================================================
X_ChemicalElementPropertyTreeModel::X_ChemicalElementPropertyTreeModel(QObject *parent)
    : QAbstractItemModel(parent), X_AbstractChemicalElementPropertyRepository()
{
    qRegisterMetaType<X_ChemicalElementPropertyItemCPtr>();

    xv_zNumberPropertyName = "X_";
    xv_symbolPropertyName = "Symbol";
    xv_namePropertyName = "Name";
    xv_settingsGroupName = metaObject()->className();
    xv_dirty = false;

    xh_createComponents();
    xh_createConnections();
    xh_loadBasicChemicalElementProperties();
    //QString chemicalElementItemNameTemplate = "Symbol";
    //xh_defineChemicalElementItemNames(chemicalElementItemNameTemplate);
}
//======================================================
X_ChemicalElementPropertyTreeModel::~X_ChemicalElementPropertyTreeModel()
{
    delete xv_root;
}
//======================================================
Qt::ItemFlags	X_ChemicalElementPropertyTreeModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    if(index.isValid())
    {
        flags |= Qt::ItemIsEnabled
                | Qt::ItemIsSelectable;

        if(index.column() != 0 || xh_itemForIndex(index.parent()) != xv_root)
        {
            flags |= Qt::ItemIsEditable;
        }

    }
    return flags;
}
//======================================================
QModelIndex X_ChemicalElementPropertyTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!xv_root || row < 0 || row >= rowCount(parent) || column < 0 || column >= columnCount()
            /*|| (parent.isValid() && column != 0)*/)
    {
        return QModelIndex();
    }

    X_ChemicalElementPropertyItem* parentItem = xh_itemForIndex(parent);
    if(!parentItem)
    {
        return QModelIndex();
    }

    X_ChemicalElementPropertyItem* item = parentItem->xp_childAtRow(row);
    if(!item)
    {
        return QModelIndex();
    }
    return createIndex(row, column, item);
}
//======================================================
int X_ChemicalElementPropertyTreeModel::columnCount(const QModelIndex & parent) const
{
    if(parent.isValid() && parent.column() != 0)
    {
        return 0;
    }

    X_ChemicalElementPropertyItem* parentItem = xh_itemForIndex(parent);

    if(!parentItem)
    {
        return 0;
    }

    if(parentItem == xv_root)
    {
        return xh_propertyNameCount(xv_propertyNames) + 1;
    }

    return 0;
}
//======================================================
int	 X_ChemicalElementPropertyTreeModel::rowCount(const QModelIndex & parent) const
{
    if(parent.isValid() && parent.column() != 0)
    {
        return 0;
    }

    X_ChemicalElementPropertyItem* parentItem = xh_itemForIndex(parent);
    if(!parentItem)
    {
        return 0;
    }

    return parentItem->xp_childCount();
}
//======================================================
QVariant X_ChemicalElementPropertyTreeModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    X_ChemicalElementPropertyItem* item = xh_itemForIndex(index);
    if(!item)
    {
        return QVariant();
    }

    if(role == PropertyCountRole)
    {
        // for any column
        return QVariant(item->xp_chemicalElementPropertyCount());
    }

    if(role == PropertyFirstColumnRole)
    {
        int column;
        if(!xh_firstPropertyColumnForItem(item, column))
        {
            return QVariant();
        }

        // increase column by one item tree column
        column++;
        if(column < 0 || column >= columnCount())
        {
            return QVariant();
        }

        return QVariant(column);
    }

    if(role == PropertyNameRole)
    {
        // colunm - number of property
        return item->xp_chemicalElementProperty(index.column()).first;
    }

    if(role == PropertyValueRole)
    {
        // colunm - number of property
        return item->xp_chemicalElementProperty(index.column()).second;
    }

    // common columns count
    if(role == X_NumberRole)
    {
        if(index.column() == 0)
        {
            return QVariant(item->xp_chemicalElementPropertyValue(xv_zNumberPropertyName));
        }
    }

    if(role == SymbolRole)
    {
        if(index.column() == 0)
        {
            return QVariant(item->xp_chemicalElementPropertyValue(xv_symbolPropertyName));
        }
    }

    if(role == NameRole)
    {
        if(index.column() == 0)
        {
            return QVariant(item->xp_chemicalElementPropertyValue(xv_namePropertyName));
        }
    }

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(index.column() == 0)
        {
            if(item->xp_parent() == xv_root /*&& !item->xp_chemicalElementPropertyValue(xv_symbolPropertyName).isEmpty()*/)
            {
                return QVariant(item->xp_chemicalElementPropertyValue(xv_symbolPropertyName));
            }

            return QVariant(item->xp_name());
        }

        QVariant vData = headerData(index.column(), Qt::Horizontal, Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            return QVariant();
        }

        return QVariant(item->xp_chemicalElementPropertyValue(vData.toString()));
    }

    return QVariant();
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!index.isValid() || index.row() < 0 || index.row() >= rowCount(index.parent())
            || index.column() < 0 || index.column() >= columnCount() || !value.isValid()
            || !value.canConvert<QString>())
    {
        return false;
    }

    X_ChemicalElementPropertyItem* item = xh_itemForIndex(index);
    if(!item || item == xv_root)
    {
        return false;
    }

    if(role == Qt::EditRole)
    {
        if(index.column() == 0)
        {
            // section name
            if(item->xp_parent() == xv_root)
            {
                return false;
            }

            item->xp_setName(value.toString());
        }
        else
        {
            // property
            // get property name
            QVariant vData = headerData(index.column(), Qt::Horizontal, Qt::DisplayRole);
            if(!vData.isValid() || !vData.canConvert<QString>())
            {
                return false;
            }

            QString propertyName = vData.toString();
            item->xp_setChemicalElementProperty(propertyName, value.toString());
        }

        //emit dataChanged(index, index);
        xv_dirty = true;
        return true;
    }

    if(role == PropertyNameRole)
    {
        // colunm - number of property
        if(item->xp_setChemicalElementPropertyName(index.column(), value.toString()))
        {
            xv_dirty = true;
            return true;
        }

        return false;
    }

    if(role == PropertyValueRole)
    {
        // colunm - number of property
        if(item->xp_setChemicalElementPropertyValue(index.column(), value.toString()))
        {
            xv_dirty = true;
            return true;
        }

        return false;
    }

    return false;
}
//======================================================
QVariant X_ChemicalElementPropertyTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole)
        {
            if(section == 0)
            {
                return QVariant();
            }

            int levelNameIndex = section - 1;
            foreach(QStringList levelPropertyNameList, xv_propertyNames)
            {
                if(levelPropertyNameList.count() <= levelNameIndex)
                {
                    levelNameIndex -= levelPropertyNameList.count();
                    continue;
                }

                return QVariant(levelPropertyNameList.at(levelNameIndex));
            }
        }
    }

    return QVariant();
}
//======================================================
QModelIndex X_ChemicalElementPropertyTreeModel::parent(const QModelIndex& child) const
{
    if(!child.isValid())
    {
        return QModelIndex();
    }

    X_ChemicalElementPropertyItem* childItem = xh_itemForIndex(child);
    if(!childItem || childItem == xv_root)
    {
        return QModelIndex();
    }

    X_ChemicalElementPropertyItem* parentItem = childItem->xp_parent();
    if(!parentItem || parentItem == xv_root)
    {
        return QModelIndex();
    }

    X_ChemicalElementPropertyItem* grandParentItem = parentItem->xp_parent();
    int parentRow = grandParentItem->xp_rowOfChildItem(parentItem);
    return createIndex(parentRow, 0, parentItem);
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::hasChildren(const QModelIndex &parent) const
{
    X_ChemicalElementPropertyItem* item = xh_itemForIndex(parent);
    if(!item)
    {
        return false;
    }

    return item->xp_hasCildren();
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xh_createComponents()
{
    // create root
    xv_root = new X_ChemicalElementPropertyItem(nullptr);
    QString defaultName;
    // append 118 chemical Elements to the root
    for(int z = 1; z <= xv_chemicalElementCount; z++)
    {
        defaultName = QString::number(z);
        X_ChemicalElementPropertyItem* item = new X_ChemicalElementPropertyItem(defaultName, this);
        xv_root->xp_addChild(item);
    }
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xh_createConnections()
{
    connect(xv_root, &X_ChemicalElementPropertyItem::xg_itemInserted,
            this, &X_ChemicalElementPropertyTreeModel::xh_onItemInsertion);
    connect(xv_root, &X_ChemicalElementPropertyItem::xg_itemAboutToBeInserted,
            this, &X_ChemicalElementPropertyTreeModel::xh_onItemAboutToBeInserted);

    connect(xv_root, &X_ChemicalElementPropertyItem::xg_requestItemRemovingPermission,
            this, &X_ChemicalElementPropertyTreeModel::xh_itemRemovingPermission);
    connect(xv_root, &X_ChemicalElementPropertyItem::xg_itemAboutToBeRemoved,
            this, &X_ChemicalElementPropertyTreeModel::xh_notifyItemAboutToBeRemoved);
    connect(xv_root, &X_ChemicalElementPropertyItem::xg_itemRemoved,
            this, &X_ChemicalElementPropertyTreeModel::xh_notifyItemRemoved);
    connect(xv_root, &X_ChemicalElementPropertyItem::xg_itemIsChanged,
            this, &X_ChemicalElementPropertyTreeModel::xh_notifyItemIsChanged);
    connect(xv_root, &X_ChemicalElementPropertyItem::xg_itemPropertyNameIsChanged,
            this, &X_ChemicalElementPropertyTreeModel::xh_notifyItemPropertyNameIsChanged);
    connect(xv_root, &X_ChemicalElementPropertyItem::xg_itemPropertyValueIsChanged,
            this, &X_ChemicalElementPropertyTreeModel::xh_notifyItemPropertyValueIsChanged);

}
//======================================================
void X_ChemicalElementPropertyTreeModel::xh_onItemAboutToBeInserted(int row, const X_ChemicalElementPropertyItem* parentItem)
{
    QModelIndex parentIndex = xh_indexForItem(parentItem);
    beginInsertRows(parentIndex, row, row);
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xh_onItemInsertion(const X_ChemicalElementPropertyItem* item)
{
    endInsertRows();
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xh_loadBasicChemicalElementProperties()
{
    // ListOfElementProperties-init.csv
    beginResetModel();
    X_ChemicalElementPropertyIOManager chemicalElementPropertyLoader;
    QString propertiesSourceFileName = xp_propertiesSourceFilePath();
    if(!chemicalElementPropertyLoader.xp_extractFromFile(this, propertiesSourceFileName)
            && propertiesSourceFileName != xv_defaultPropertiesSourceFileName)
    {
        xp_clearData();
        chemicalElementPropertyLoader.xp_extractFromFile(this, xv_defaultPropertiesSourceFileName);
    }

    xh_redefinePropertyNameList();
    endResetModel();
    xv_dirty = false;
}
//======================================================
QString X_ChemicalElementPropertyTreeModel::xp_propertiesSourceFilePath() const
{
    QSettings settings;
    // TODO basic properties file name from settings
    if(!xh_openDialogSettingsGroup(&settings))
    {
        return xv_defaultPropertiesSourceFileName;
    }

    QVariant vData = settings.value(xv_propertyFilePathSettingsName);
    xh_closeDialogSettingsGroup(&settings);

    if(!vData.isValid() || !vData.canConvert<QString>())
    {
        return xv_defaultPropertiesSourceFileName;
    }

    return vData.toString();
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xp_setPropertiesSourceFileName(const QString& propertiesFileName)
{
    QSettings settings;
    // TODO basic properties file name from settings
    if(!xh_openDialogSettingsGroup(&settings))
    {
        return false;
    }

    settings.setValue(xv_propertyFilePathSettingsName, QVariant(propertiesFileName));

    xh_closeDialogSettingsGroup(&settings);
    return true;
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xp_loadDataFromFile(const QString& filePath)
{
    xv_errorStringList.clear();
    X_ChemicalElementPropertyIOManager chemicalElementPropertyLoader;
    if(!chemicalElementPropertyLoader.xp_extractFromFile(this, filePath))
    {
        // TODO error report
        xv_errorStringList.append(chemicalElementPropertyLoader.xp_lastErrors());
        xh_loadBasicChemicalElementProperties();
        return false;
    }

    emit xg_propertiesSourceFilePathChanged(filePath);
    xv_dirty = false;
    return true;
}
//======================================================
QModelIndex X_ChemicalElementPropertyTreeModel::xp_indexForSection(int X_Number, const QStringList& sectionBranch) const
{
    X_ChemicalElementPropertyItem* item = xh_itemForX_Number(X_Number);
    if(!item)
    {
        return QModelIndex();
    }

    // branch is empty => add properties to chemical elemnt item
    if(sectionBranch.isEmpty())
    {
        return xh_indexForItem(item);
    }

    X_ChemicalElementPropertyItem* child = item->xp_childNamed(sectionBranch);
    if(!child)
    {
        return QModelIndex();
    }

    return xh_indexForItem(child);
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xp_setChemicalElementProperty(int X_Number,
                                                                      const QString& key,
                                                                      const QString& value)
{
    //beginResetModel();
    bool res = xh_setCemicalElementProperty(X_Number,
                                            key,
                                            value);
    xh_redefinePropertyNameList();
    xv_dirty = true;

    //endResetModel();
    return res;
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xp_setChemicalElementProperties(int X_Number,
                                                                        const QStringList &propertySectionBranch,
                                                                        const PropertyList& propertyList)
{
    //beginResetModel();
    bool res = xh_setCemicalElementProperties(X_Number,
                                              propertySectionBranch,
                                              propertyList);
    xh_redefinePropertyNameList();
    xv_dirty = true;

    //endResetModel();
    return res;
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xp_chemicalElementPropertySectionBranch(const QModelIndex& index,
                                                                                QStringList& branch)
{
    if(!index.isValid())
    {
        return false;
    }

    X_ChemicalElementPropertyItem* item = xh_itemForIndex(index);
    if(!item || item == xv_root)
    {
        return false;
    }

    item->xp_chemicalElementPropertySectionBranch(branch);
    return !branch.isEmpty();
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xp_removeChemicalElementProperty(int X_Number,
                                                                         const QStringList& propertySectionBranch,
                                                                         int propertyIndex)
{
    X_ChemicalElementPropertyItem* item = xh_itemForX_Number(X_Number);
    if(!item)
    {
        xv_errorMsg = tr("Cannot get model item for X_ number %1!").arg(QString::number(X_Number));
        return false;
    }

    // branch is empty => add properties to chemical elemnt item
    if(propertySectionBranch.isEmpty())
    {
        if(item->xp_removeChemicalElementProperty(propertyIndex))
        {
            xv_dirty = true;
            return true;
        }
    }

    X_ChemicalElementPropertyItem* child = item->xp_childNamed(propertySectionBranch);
    if(!child)
    {
        QString currentSectionBranchString;
        for(int s = 0; s < propertySectionBranch.count(); s++)
        {
            currentSectionBranchString += propertySectionBranch.at(s);
            if(s < propertySectionBranch.count() - 1)
            {
                currentSectionBranchString += "/";
            }
        }
        xv_errorMsg = tr("Cannot get model item \"%1\" for X_ number %2!").arg(currentSectionBranchString,
                                                                              QString::number(X_Number));
        return false;
    }

    if(item->xp_removeChemicalElementProperty(propertyIndex))
    {
        xv_dirty = true;
        return true;
    }

    return false;
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xp_chemicalElementProperties(int X_Number,
                                                                     const QStringList& propertySectionBranch,
                                                                     PropertyList& propertyList)
{
    propertyList.clear();
    X_ChemicalElementPropertyItem* item = xh_itemForX_Number(X_Number);
    if(!item)
    {
        xv_errorMsg = tr("Cannot get model item for X_ number %1!").arg(QString::number(X_Number));
        return false;
    }

    // branch is empty => add properties to chemical elemnt item
    if(propertySectionBranch.isEmpty())
    {
        propertyList = item->xp_chemicalElementPropertyList();
        return true;
    }

    X_ChemicalElementPropertyItem* child = item->xp_childNamed(propertySectionBranch);
    if(!child)
    {
        QString currentSectionBranchString;
        for(int s = 0; s < propertySectionBranch.count(); s++)
        {
            currentSectionBranchString += propertySectionBranch.at(s);
            if(s < propertySectionBranch.count() - 1)
            {
                currentSectionBranchString += "/";
            }
        }
        xv_errorMsg = tr("Cannot get model item \"%1\" for X_ number %2!").arg(currentSectionBranchString,
                                                                              QString::number(X_Number));
        return false;
    }

    propertyList = child->xp_chemicalElementPropertyList();
    return true;
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xp_chemicalElementPropertySectionList(int X_Number,
                                                                              const QStringList& propertySectionBranch,
                                                                              QStringList& sectionList)
{
    sectionList.clear();
    X_ChemicalElementPropertyItem* item = xh_itemForX_Number(X_Number);
    if(!item)
    {
        xv_errorMsg = tr("Cannot get model item for X_ number %1!").arg(QString::number(X_Number));
        return false;
    }

    // branch is empty => add properties to chemical elemnt item
    if(propertySectionBranch.isEmpty())
    {
        sectionList = item->xp_chemicalElementPropertySectionList();
        return true;
    }

    X_ChemicalElementPropertyItem* child = item->xp_childNamed(propertySectionBranch);
    if(!child)
    {
        QString currentSectionBranchString;
        for(int s = 0; s < propertySectionBranch.count(); s++)
        {
            currentSectionBranchString += propertySectionBranch.at(s);
            if(s < propertySectionBranch.count() - 1)
            {
                currentSectionBranchString += "/";
            }
        }
        xv_errorMsg = tr("Cannot get model item \"%1\" for X_ number %2!").arg(currentSectionBranchString,
                                                                              QString::number(X_Number));
        return false;
    }

    sectionList = child->xp_chemicalElementPropertySectionList();
    return true;
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xp_clearData()
{
    beginResetModel();
    for(int i = 0; i < xv_root->xp_childCount(); i++)
    {
        xv_root->xp_childAtRow(i)->xp_clear();
    }
    endResetModel();
}
//======================================================
QStringList X_ChemicalElementPropertyTreeModel::xp_lastErrorStringList() const
{
    return xv_errorStringList;
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xp_addChemicalElementPropertyBranch(int X_Number,
                                                                            const QStringList& propertyGroupNameBranch)
{
    // beginResetModel();
    bool res = xh_addCemicalElementPropertyBranch(X_Number,
                                                  propertyGroupNameBranch);
    xh_redefinePropertyNameList();
    xv_dirty = true;
    // endResetModel();
    return res;
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xh_defineChemicalElementItemNames(const QString& nameTemplate,
                                                                          QString separator)
{
    if(!xv_root)
    {
        return;
    }

    QModelIndex currentIndex;
    for(int i = 0; i < xv_root->xp_childCount(); i++)
    {
        if(xv_root->xp_childAtRow(i)->xp_createName(nameTemplate, separator))
        {
            currentIndex = index(i, 0, QModelIndex());
            emit dataChanged(currentIndex, currentIndex);
        }
    }
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xp_zNumberForIndex(const QModelIndex& index, int& zNumber) const
{
    X_ChemicalElementPropertyItem* item = xh_itemForIndex(index);
    if(!item)
    {
        return false;
    }

    zNumber = item->xp_zNumber(item);
    return zNumber > 0 && zNumber <= xv_chemicalElementCount;
}
//======================================================
QString X_ChemicalElementPropertyTreeModel::xp_zNumberPropertyName() const
{
    return xv_zNumberPropertyName;
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xp_setX_NumberPropertyName(const QString& name)
{
    xv_zNumberPropertyName = name;
}
//======================================================
QString X_ChemicalElementPropertyTreeModel::xp_symbolPropertyName() const
{
    return xv_symbolPropertyName;
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xp_setSymbolPropertyName(const QString& name)
{
    xv_symbolPropertyName = name;
}
//======================================================
QString X_ChemicalElementPropertyTreeModel::xp_namePropertyName() const
{
    return xv_namePropertyName;
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xp_setNamePropertyName(const QString& name)
{
    xv_namePropertyName = name;
}
//======================================================
int X_ChemicalElementPropertyTreeModel::xp_chemicalElementCount() const
{
    return xv_chemicalElementCount;
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xp_isRemovable(const QModelIndex& index) const
{
    if(!index.isValid())
    {
        // root
        return false;
    }

    if(index.column() != 0)
    {
        return true;
    }

    X_ChemicalElementPropertyItem* item = xh_itemForIndex(index);
    if(!item)
    {
        return false;
    }

    int levelNumber = item->xh_elementLevelNumber();
    return levelNumber > 1;
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xp_removeItems(const QModelIndexList& indexList)
{
    // create list of removed items
    QList<X_ChemicalElementPropertyItem*> removedItemList;
    X_ChemicalElementPropertyItem* item;
    foreach(QModelIndex index, indexList)
    {
        item = xh_itemForIndex(index);
        if(!item)
        {
            continue;
        }

        removedItemList.append(item);
    }

    QList<X_ChemicalElementPropertyItem*> childItemList;
    while(!removedItemList.isEmpty())
    {
        childItemList.clear();
        item = removedItemList.takeFirst();
        item->xp_totalDescendantList(childItemList);

        // remove children from removeed item list
        while(!childItemList.isEmpty())
        {
            removedItemList.removeAll(childItemList.takeFirst());
        }

        item->xp_initRemoving();
    }
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xp_isDirty() const
{
    return xv_dirty;
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xp_saveDataToXMLFile(const QString& filePath)
{
    X_ChemicalElementPropertyIOManager chemicalElementPropertyLoader;
    if(!chemicalElementPropertyLoader.xp_saveToXML(this, filePath))
    {
        return false;
    }

    xg_propertiesSourceFilePathChanged(filePath);
    xv_dirty = false;
    return true;;
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xh_notifyItemAboutToBeRemoved(const X_ChemicalElementPropertyItem* item)
{
    if(!item)
    {
        return;
    }

    QModelIndex parentIndex = xh_indexForItem(item->xp_parent());
    int itemRow = item->xp_parent()->xp_rowOfChildItem(item);

    if(parentIndex.isValid() && itemRow >= 0)
    {
        beginRemoveRows(parentIndex, itemRow, itemRow);
    }
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xh_notifyItemRemoved(const X_ChemicalElementPropertyItem *item)
{
    endRemoveRows();
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xh_notifyItemIsChanged(const X_ChemicalElementPropertyItem *item)
{
    if(!item || item == xv_root || !item->xp_parent() )
    {
        return;
    }

    QModelIndex parent = xh_indexForItem(item->xp_parent());
    int row = item->xp_parent()->xp_rowOfChildItem(item);
    QModelIndex topLeft = index(row, 0, parent);
    QModelIndex bottomRight =  index(row, columnCount() - 1, parent);

    //xh_redefinePropertyNameList();
    emit dataChanged(topLeft, bottomRight);
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xh_notifyItemPropertyNameIsChanged(const X_ChemicalElementPropertyItem *item)
{
    xh_redefinePropertyNameList();
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xh_notifyItemPropertyValueIsChanged(const X_ChemicalElementPropertyItem *item)
{
    xh_redefinePropertyNameList();
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xh_itemRemovingPermission(const X_ChemicalElementPropertyItem* item,
                                                                  bool& permissionToRemove)
{
    if(item == nullptr)
    {
        permissionToRemove = false;
        return;
    }

    QModelIndex itemIndex = xh_indexForItem(item);
    permissionToRemove = xp_isRemovable(itemIndex);
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xh_setCemicalElementProperty(int X_Number,
                                                                     const QString& key,
                                                                     const QString& value)
{
    X_ChemicalElementPropertyItem* item = xh_itemForX_Number(X_Number);
    if(!item)
    {
        xv_errorMsg = tr("Cannot get model item for X_ number %1!").arg(QString::number(X_Number));
        return false;
    }

    item->xp_setChemicalElementProperty(key, value);
    return true;
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xh_setCemicalElementProperties(int X_Number,
                                                                       const QStringList& propertySectionBranch,
                                                                       const PropertyList& propertyList)
{
    X_ChemicalElementPropertyItem* item = xh_itemForX_Number(X_Number);
    if(!item)
    {
        xv_errorMsg = tr("Cannot get model item for X_ number %1!").arg(QString::number(X_Number));
        return false;
    }

    // branch is empty => add properties to chemical elemnt item
    if(propertySectionBranch.isEmpty())
    {
        item->xp_setChemicalElementPropertyList(propertyList);
        return true;
    }

    X_ChemicalElementPropertyItem* child = item->xp_childNamed(propertySectionBranch);
    if(!child)
    {
        QString currentSectionBranchString;
        for(int s = 0; s < propertySectionBranch.count(); s++)
        {
            currentSectionBranchString += propertySectionBranch.at(s);
            if(s < propertySectionBranch.count() - 1)
            {
                currentSectionBranchString += "/";
            }
        }
        xv_errorMsg = tr("Cannot get model item \"%1\" for X_ number %2!").arg(currentSectionBranchString,
                                                                              QString::number(X_Number));
        return false;
    }

    child->xp_setChemicalElementPropertyList(propertyList);
    return true;
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xh_addCemicalElementPropertyBranch(int X_Number,
                                                                           const QStringList& propertySectionBranch)
{
    X_ChemicalElementPropertyItem* item = xh_itemForX_Number(X_Number);
    if(!item)
    {
        xv_errorMsg = tr("Cannot get model item for X_ number %1!").arg(QString::number(X_Number));
        return false;
    }

    X_ChemicalElementPropertyItem* child = item->xp_childNamed(propertySectionBranch);
    return child != nullptr;
}
//======================================================
X_ChemicalElementPropertyItem* X_ChemicalElementPropertyTreeModel::xh_itemForIndex(const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return xv_root;
    }

    X_ChemicalElementPropertyItem* item = static_cast<X_ChemicalElementPropertyItem*>(index.internalPointer());
    if(!item)
    {
        return xv_root;
    }

    return item;
}
//======================================================
QModelIndex X_ChemicalElementPropertyTreeModel::xh_indexForItem(const X_ChemicalElementPropertyItem* item) const
{
    if(item == nullptr)
    {
        return QModelIndex();
    }

    X_ChemicalElementPropertyItem* parentItem = item->xp_parent();
    if(parentItem == nullptr)
    {
        // root
        return QModelIndex();
    }

    int itemRow = parentItem->xp_rowOfChildItem(item);
    if(itemRow < 0 || itemRow >= parentItem->xp_childCount())
    {
        return QModelIndex();
    }

    int column = 0;

    return createIndex(itemRow, column, const_cast<X_ChemicalElementPropertyItem*>(item));

}
//======================================================
X_ChemicalElementPropertyItem* X_ChemicalElementPropertyTreeModel::xh_itemForX_Number(int X_Number) const
{
    if(X_Number < 1)
    {
        return xv_root;
    }

    if(!xv_root || xv_root->xp_childCount() < X_Number)
    {
        return nullptr;
    }

    return xv_root->xp_childAtRow(X_Number - 1);
}
//======================================================
void X_ChemicalElementPropertyTreeModel::xh_redefinePropertyNameList()
{
    if(columnCount() > 1)
    {
        beginRemoveColumns(QModelIndex(), 1, columnCount()-1);
        xv_propertyNames.clear();
        endRemoveColumns();
    }

    if(!xv_root)
    {
        return;
    }

    PropertyNames propertyNames;
    for(int i = 0; i < xv_root->xp_childCount(); i++)
    {
        xv_root->xp_childAtRow(i)->xp_completeChemicalElementPropertyNameList(propertyNames);
        xv_propertyNames.append(propertyNames);
    }

    int newColumnCount = xh_propertyNameCount(propertyNames);
    beginInsertColumns(QModelIndex(), 1, 1 + newColumnCount);
    xv_propertyNames = propertyNames;
    endInsertColumns();
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xh_firstPropertyColumnForItem(X_ChemicalElementPropertyItem* item,
                                                                      int& columnNumber) const
{
    // define element level number (zero level - root)
    int levelNumber = item->xh_elementLevelNumber();

    // get first column for appropriate level from xv_propertyNames
    columnNumber = 0;
    for(int i = 0; i < levelNumber - 1; i++)
    {
        if(i >= xv_propertyNames.count())
        {
            return false;
        }

        columnNumber += xv_propertyNames.at(i).count();
    }

    return true;
}
//======================================================
int X_ChemicalElementPropertyTreeModel::xh_propertyNameCount(const PropertyNames& propertyNames) const
{
    if(propertyNames.isEmpty())
    {
        return 0;
    }

    int count = 0;
    // columns for properties
    foreach(QStringList levelPropertyNameList, propertyNames)
    {
        count += levelPropertyNameList.count();
    }
    return count;
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xh_openDialogSettingsGroup(QSettings* settings) const
{
    if(settings == nullptr)
    {
        return false;
    }
    // open version group
    if(!qApp->applicationVersion().isEmpty())
    {
        settings->beginGroup(qApp->applicationVersion());
    }

    // open dialog group
    if(!xv_settingsGroupName.isEmpty())
    {
        settings->beginGroup(xv_settingsGroupName);
    }
    return true;
}
//======================================================
bool X_ChemicalElementPropertyTreeModel::xh_closeDialogSettingsGroup(QSettings *settings) const
{
    if(settings == nullptr)
    {
        return false;
    }

    // close groups
    while(!settings->group().isEmpty())
    {
        settings->endGroup();
    }

    return true;
}
//======================================================

