//======================================================
#include "X_ChemicalElementPropertyItem.h"
//======================================================
X_ChemicalElementPropertyItem::X_ChemicalElementPropertyItem(QObject* parent)
    : QObject(parent)
{
    xv_parentItem = qobject_cast<X_ChemicalElementPropertyItem*>(parent);
}
//======================================================
X_ChemicalElementPropertyItem::X_ChemicalElementPropertyItem(const QString& name,
                                                           QObject* parent) : QObject(parent)
{
    xv_parentItem = qobject_cast<X_ChemicalElementPropertyItem*>(parent);
    xv_name = name;
}
//======================================================
QString X_ChemicalElementPropertyItem::xp_name() const
{
    return xv_name;
}
//======================================================
void X_ChemicalElementPropertyItem::xp_setName(const QString& name)
{
    xv_name = name;
    emit xg_itemIsChanged(this);
}
//======================================================
bool X_ChemicalElementPropertyItem::xp_createName(const QString& nameTemplate,
                                                 const QString& separator)
{
    QStringList propertyNameList = nameTemplate.split(separator);
    QString newName;
    QString propertyValue;
    for(int i = 0; i < propertyNameList.count(); i++)
    {
        propertyValue = xp_chemicalElementPropertyValue(propertyNameList.at(i));
        if(propertyValue.isEmpty())
        {
            continue;
        }

        newName += propertyValue;
        if(i < propertyNameList.count() - 1)
        {
            newName += separator;
        }
    }

    if(newName.isEmpty() || newName == xv_name)
    {
        return false;
    }

    xv_name = newName;
    emit xg_itemIsChanged(this);
    return true;
}
//======================================================
void X_ChemicalElementPropertyItem::xp_addChild(X_ChemicalElementPropertyItem* child)
{
    emit xg_itemAboutToBeInserted(xp_childCount(), this);

    child->xv_parentItem = this;
    xv_childItemList.append(child);
    connect(child, &X_ChemicalElementPropertyItem::xg_itemInserted,
            this, &X_ChemicalElementPropertyItem::xg_itemInserted);
    connect(child, &X_ChemicalElementPropertyItem::xg_itemAboutToBeInserted,
            this, &X_ChemicalElementPropertyItem::xg_itemAboutToBeInserted);
    connect(child, &X_ChemicalElementPropertyItem::xg_requestItemRemovingPermission,
            this, &X_ChemicalElementPropertyItem::xg_requestItemRemovingPermission);
    connect(child, &X_ChemicalElementPropertyItem::xg_itemRemoved,
            this, &X_ChemicalElementPropertyItem::xg_itemRemoved);
    connect(child, &X_ChemicalElementPropertyItem::xg_itemAboutToBeRemoved,
            this, &X_ChemicalElementPropertyItem::xg_itemAboutToBeRemoved);
    connect(child, &X_ChemicalElementPropertyItem::xg_itemIsChanged,
            this, &X_ChemicalElementPropertyItem::xg_itemIsChanged);
    connect(child, &X_ChemicalElementPropertyItem::xg_itemPropertyNameIsChanged,
            this, &X_ChemicalElementPropertyItem::xg_itemPropertyNameIsChanged);
    connect(child, &X_ChemicalElementPropertyItem::xg_itemPropertyValueIsChanged,
            this, &X_ChemicalElementPropertyItem::xg_itemPropertyValueIsChanged);


    emit xg_itemInserted(child);
}
//======================================================
X_ChemicalElementPropertyItem* X_ChemicalElementPropertyItem::xp_createChild(const QString& name)
{
    X_ChemicalElementPropertyItem* child = new X_ChemicalElementPropertyItem(name, this);
    xp_addChild(child);
    return child;
}
//======================================================
void X_ChemicalElementPropertyItem::xp_setChemicalElementProperty(const QString& key, const QString& value)
{
    for(int i = 0; i < xv_chemicalElementPropertyList.count(); i++)
    {
        if(xv_chemicalElementPropertyList.at(i).first == key)
        {
            xv_chemicalElementPropertyList[i].second = value;
            emit xg_itemIsChanged(this);
            return;
        }
    }

    QPair<QString, QString> propertyPair = QPair<QString, QString>(key, value);
    xv_chemicalElementPropertyList.append(propertyPair);
    emit xg_itemIsChanged(this);
}
//======================================================
QList<QPair<QString, QString> > X_ChemicalElementPropertyItem::xp_chemicalElementPropertyList() const
{
    return xv_chemicalElementPropertyList;
}
//======================================================
QStringList X_ChemicalElementPropertyItem::xp_chemicalElementPropertySectionList() const
{
    QStringList sectionStringList;
    foreach(auto child, xv_childItemList)
    {
        sectionStringList.append(child->xp_name());
    }

    return sectionStringList;
}
//======================================================
bool X_ChemicalElementPropertyItem::xp_setChemicalElementPropertyName(int index, const QString& name)
{
    if(index < 0 || index >= xv_chemicalElementPropertyList.count())
    {
        return false;
    }

    xv_chemicalElementPropertyList[index].first = name;
    emit xg_itemPropertyNameIsChanged(this);
    return true;
}
//======================================================
bool X_ChemicalElementPropertyItem::xp_setChemicalElementPropertyValue(int index, const QString& value)
{
    if(index < 0 || index >= xv_chemicalElementPropertyList.count())
    {
        return false;
    }

    xv_chemicalElementPropertyList[index].second = value;
    emit xg_itemPropertyValueIsChanged(this);
    return true;
}
//======================================================
QString X_ChemicalElementPropertyItem::xp_chemicalElementPropertyValue(const QString& key) const
{
    for(int i = 0; i < xv_chemicalElementPropertyList.count(); i++)
    {
        if(xv_chemicalElementPropertyList.at(i).first == key)
        {
            return xv_chemicalElementPropertyList.at(i).second;
        }
    }

    return QString();
}
//======================================================
QPair<QString, QString> X_ChemicalElementPropertyItem::xp_chemicalElementProperty(int index) const
{
    if(index < 0 || index >= xv_chemicalElementPropertyList.count() )
    {
        return QPair<QString, QString>();
    }

    return xv_chemicalElementPropertyList.at(index);
}
//======================================================
void X_ChemicalElementPropertyItem::xp_setChemicalElementPropertyList(const QList<QPair<QString, QString> >& propertyList)
{
    foreach(auto& proprtyPair, propertyList)
    {
        xp_setChemicalElementProperty(proprtyPair.first, proprtyPair.second);
    }
}
//======================================================
X_ChemicalElementPropertyItem* X_ChemicalElementPropertyItem::xp_parent() const
{
    return xv_parentItem;
}
//======================================================
X_ChemicalElementPropertyItem* X_ChemicalElementPropertyItem::xp_childAtRow(int row) const
{
    if(row < 0 || row >= xv_childItemList.count())
    {
        return nullptr;
    }

    return xv_childItemList.at(row);
}
//======================================================
X_ChemicalElementPropertyItem* X_ChemicalElementPropertyItem::xp_childNamed(const QStringList& nameBranch)
{
    if(nameBranch.isEmpty())
    {
        return nullptr;
    }
    QStringList newNameBranch = nameBranch;
    QString name = newNameBranch.takeFirst();

    X_ChemicalElementPropertyItem* child = xp_childNamed(name);

    if(newNameBranch.isEmpty())
    {
        return child;
    }

    return child->xp_childNamed(newNameBranch);
}
//======================================================
X_ChemicalElementPropertyItem* X_ChemicalElementPropertyItem::xp_childNamed(const QString& name)
{
    for(int i = 0; i < xv_childItemList.count(); i++)
    {
        if(xv_childItemList.at(i)->xp_name() == name)
        {
            return xv_childItemList.value(i);
        }
    }

    return xp_createChild(name);
}
//======================================================
void X_ChemicalElementPropertyItem::xp_chemicalElementPropertySectionBranch(QStringList& branch)
{
    if(!xv_parentItem)
    {
        // root
        return;
    }

    branch.push_front(xv_name);
    xv_parentItem->xp_chemicalElementPropertySectionBranch(branch);
}
//======================================================
int X_ChemicalElementPropertyItem::xp_childCount() const
{
    return xv_childItemList.count();
}
//======================================================
bool X_ChemicalElementPropertyItem::xp_hasCildren() const
{
    return !xv_childItemList.isEmpty();
}
//======================================================
void X_ChemicalElementPropertyItem::xp_removeChildren()
{
    while(!xv_childItemList.isEmpty())
    {
        xp_removeChild(xv_childItemList.last());
    }
}
//======================================================
int X_ChemicalElementPropertyItem::xp_rowOfChildItem(const X_ChemicalElementPropertyItem* child) const
{
    if(!child)
    {
        return -1;
    }

    return xv_childItemList.indexOf(const_cast<X_ChemicalElementPropertyItem*>(child));
}
//======================================================
void X_ChemicalElementPropertyItem::xp_completeChemicalElementPropertyNameList(PropertyNames& propertyNames) const
{
    QStringList propertyNameStringList = propertyNames.isEmpty()? QStringList() : propertyNames.takeFirst();

    for(int i = 0; i < xv_chemicalElementPropertyList.count(); i++)
    {
        if(propertyNameStringList.contains(xv_chemicalElementPropertyList.at(i).first))
        {
            continue;
        }

        propertyNameStringList.append(xv_chemicalElementPropertyList.at(i).first);
    }

    foreach(X_ChemicalElementPropertyItem* item, xv_childItemList)
    {
        item->xp_completeChemicalElementPropertyNameList(propertyNames);
    }

    propertyNames.push_front(propertyNameStringList);
}
//======================================================
int X_ChemicalElementPropertyItem::xp_chemicalElementPropertyCount() const
{
    return xv_chemicalElementPropertyList.count();
}
//======================================================
int X_ChemicalElementPropertyItem::zh_elementLevelNumber() const
{
    if(xv_parentItem == nullptr)
    {
        return 0;
    }

    return xv_parentItem->zh_elementLevelNumber() + 1;
}
//======================================================
int X_ChemicalElementPropertyItem::xp_zNumber(const X_ChemicalElementPropertyItem * callingItem) const
{
    if(xv_parentItem == nullptr)
    {
        // root
        int childRow = xp_rowOfChildItem(callingItem);
        return childRow + 1;
    }

    return xv_parentItem->xp_zNumber(this);
}
//======================================================
void X_ChemicalElementPropertyItem::xp_totalDescendantList(QList<X_ChemicalElementPropertyItem*>& descendantList) const
{
    descendantList.append(xv_childItemList);
    foreach(X_ChemicalElementPropertyItem* childItem, xv_childItemList)
    {
        childItem->xp_totalDescendantList(descendantList);
    }
}
//======================================================
bool X_ChemicalElementPropertyItem::xp_initRemoving()
{
    for(int i = xv_childItemList.count()-1; i >= 0; i--)
    {
        xv_childItemList[i]->xp_initRemoving();
    }

    if(!xv_childItemList.isEmpty())
    {
        // some children remain
        return false;
    }

    bool removePermission = false;
    emit xg_requestItemRemovingPermission(this, removePermission);

    if(!removePermission)
    {
        return false;
    }

    // check out from parent list
    if(xv_parentItem)
    {
        xv_parentItem->xp_removeChild(this);
        deleteLater();
    }

    return true;
}
//======================================================
void X_ChemicalElementPropertyItem::xp_removeChild(X_ChemicalElementPropertyItem* child)
{
    int childRow = xp_rowOfChildItem(child);
    if(childRow < 0 || childRow >= xv_childItemList.count() )
    {
        return;
    }

    emit xg_itemAboutToBeRemoved(child);
    xv_childItemList[childRow]->xv_parentItem = nullptr;
    xv_childItemList.removeAt(childRow);
    emit xg_itemRemoved(child);
}
//======================================================
bool X_ChemicalElementPropertyItem::xp_removeChemicalElementProperty(int propertyIndex)
{
    if(propertyIndex < 0 || propertyIndex >= xv_chemicalElementPropertyList.count())
    {
        return false;
    }

    xv_chemicalElementPropertyList.removeAt(propertyIndex);
    emit xg_itemIsChanged(this);
    return true;
}
//======================================================
void X_ChemicalElementPropertyItem::xp_removeChemicalElementProperties()
{
    while(!xv_chemicalElementPropertyList.isEmpty())
    {
        xp_removeChemicalElementProperty(xv_chemicalElementPropertyList.count() - 1);
    }
}
//======================================================
void X_ChemicalElementPropertyItem::xp_clear()
{
    xp_removeChemicalElementProperties();
    xp_removeChildren();
}
//======================================================

