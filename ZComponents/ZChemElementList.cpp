//=====================================================
#include "ZChemElementList.h"
#include "globalVariables.h"
//=====================================================
qint64 ZChemElementItem::zv_lastId = 0;

//=====================================================
ZChemElementList::ZChemElementList(QObject *parent) : QObject(parent)
{

}
//=====================================================
bool ZChemElementList::zp_appendElement(const QString& element, qint64& chemElementId)
{
    if(zp_containsElement(element))
    {
        chemElementId = -1;
        return false;
    }

    int index = zv_elementList.count();
    ZChemElementItem chemElementItem(element);
    chemElementId = chemElementItem.zp_chemElementId();
    emit zg_operationType(OT_INSERT_CHEM_ELEMENT, index, index);
    zv_elementList.append(chemElementItem);
    emit zg_operationType(OT_END_INSERT_CHEM_ELEMENT, index, index);
    return true;
}
//=====================================================
bool ZChemElementList::zp_appendElement(const QString& element)
{
    if(zp_containsElement(element))
    {
        return false;
    }

    int index = zv_elementList.count();
    ZChemElementItem chemElementItem(element);
    emit zg_operationType(OT_INSERT_CHEM_ELEMENT, index, index);
    zv_elementList.append(chemElementItem);
    emit zg_operationType(OT_END_INSERT_CHEM_ELEMENT, index, index);
    return true;
}
//=====================================================
bool ZChemElementList::zp_removeElement(const QString& element)
{
    int elementIndex = zp_findElement(element);
    if(elementIndex < 0)
    {
        return false;
    }

    emit zg_operationType(OT_REMOVE_CHEM_ELEMENT, elementIndex, elementIndex);
    zv_elementList.removeAt(elementIndex);
    emit zg_operationType(OT_END_REMOVE_CHEM_ELEMENT, elementIndex, elementIndex);

    return true;
}
//=====================================================
void ZChemElementList::zp_clearList()
{
    if(!zv_elementList.isEmpty())
    {
        int lastElementIndex = zv_elementList.count() - 1;
        emit zg_operationType(OT_REMOVE_CHEM_ELEMENT, 0, lastElementIndex);
        zv_elementList.clear();
        emit zg_operationType(OT_END_REMOVE_CHEM_ELEMENT, 0, lastElementIndex);
    }
}
//=====================================================
QStringList ZChemElementList::zp_chemElementList() const
{
    QStringList elementList;
    for(int i = 0; i < zv_elementList.count(); i++)
    {
        elementList << zv_elementList.value(i).element;
    }

    return elementList;
}
//=====================================================
bool ZChemElementList::zp_containsElement(const QString& element) const
{
    for(int i = 0; i < zv_elementList.count(); i++)
    {
        if(zv_elementList.value(i).element == element)
        {
            return true;
        }
    }

    return false;
}
//=====================================================
bool ZChemElementList::zp_containsElementId(qint64 chemElementId) const
{
    for(int i = 0; i < zv_elementList.count(); i++)
    {
        if(zv_elementList.value(i).zp_chemElementId() == chemElementId)
        {
            return true;
        }
    }

    return false;
}
//=====================================================
bool ZChemElementList::zp_isElementVisible(const QString& element) const
{
    int elementIndex = zp_findElement(element);
    if(elementIndex < 0)
    {
        return false;
    }

    return zv_elementList.value(elementIndex).visibility;

}
//=====================================================
bool ZChemElementList::zp_setChemElementVisible(const QString& element, bool visible)
{
    int elementIndex = zp_findElement(element);
    if(elementIndex < 0)
    {
        return false;
    }

    return zp_setChemElementVisible(elementIndex, visible);
}
//=====================================================
bool ZChemElementList::zp_setAverageValue(const QString& element, qreal averageValue)
{
    int elementIndex = zp_findElement(element);
    if(elementIndex < 0)
    {
        return false;
    }

    return zp_setAverageValue(elementIndex, averageValue);
}
//=====================================================
qreal ZChemElementList::zp_averageValue(const QString& element) const
{
    int elementIndex = zp_findElement(element);
    if(elementIndex < 0)
    {
        return 0;
    }

    return zp_averageValue(elementIndex);
}
//=====================================================
bool ZChemElementList::zp_setAverageValue(int index, qreal averageValue)
{
    if(index < 0 || index >= zv_elementList.count())
    {
        return false;
    }

    if(zv_elementList.value(index).averageConcentrationValue == averageValue)
    {
        return false;
    }

    zv_elementList[index].averageConcentrationValue = averageValue;
    emit zg_operationType(OT_AVERAGE_VALUE_CHANGED, index, index);
    return true;
}
//=====================================================
qreal ZChemElementList::zp_averageValue(int index) const
{
    if(index < 0 || index >= zv_elementList.count())
    {
        return 0;
    }

    return zv_elementList.value(index).averageConcentrationValue;
}
//=====================================================
int ZChemElementList::zp_findElement(const QString& element) const
{
    for(int i = 0; i < zv_elementList.count(); i++)
    {
        if(zv_elementList.value(i).element == element)
        {
            return i;
        }
    }

    return -1;
}
//=====================================================
int ZChemElementList::zp_chemElementCount() const
{
    return zv_elementList.count();
}
//=====================================================
int ZChemElementList::zp_visibleElementCount() const
{
    int visible = 0;
    for(int i = 0; i < zv_elementList.count(); i++)
    {
        if(zv_elementList.value(i).visibility)
        {
            visible++;
        }
    }
    return visible;
}
//=====================================================
int ZChemElementList::zp_numberVisibleChemElementsBeforeIndex(int index) const
{
    if(index < 0)
    {
        return -1;
    }

    int limitIndex;
    if(index > zv_elementList.count())
    {
        limitIndex = zv_elementList.count();
    }
    else
    {
        limitIndex = index;
    }

    int visible = 0;
    for(int i = 0; i < limitIndex; i++)
    {
        if(zv_elementList.value(i).visibility)
        {
            visible++;
        }
    }

    return visible;
}
//=====================================================
bool ZChemElementList::zp_isEmpty() const
{
    return zv_elementList.isEmpty();
}
//=====================================================
QString ZChemElementList::zp_visibleChemElementName(int visibleIndex) const
{
    if(visibleIndex < 0 || visibleIndex >= zp_visibleElementCount())
    {
        return QString();
    }

    int visible = 0;
    for(int i = 0; i < zv_elementList.count(); i++)
    {
        if(zv_elementList.value(i).visibility)
        {
            if(visible == visibleIndex)
            {
                return zv_elementList.value(i).element;
            }
            visible++;
        }
    }

    return QString();
}
//=====================================================
qint64 ZChemElementList::zp_visibleChemElementId(int visibleIndex) const
{
    if(visibleIndex < 0 || visibleIndex >= zp_visibleElementCount())
    {
        return -1;
    }

    int visible = 0;
    for(int i = 0; i < zv_elementList.count(); i++)
    {
        if(zv_elementList.value(i).visibility)
        {
            if(visible == visibleIndex)
            {
                return zv_elementList.value(i).zp_chemElementId();
            }
            visible++;
        }
    }

    return -1;
}
//=====================================================
qint64 ZChemElementList::zp_chemElementId(int index) const
{
    if(index < 0 || index >= zv_elementList.count())
    {
        return -1;
    }

    return zv_elementList.value(index).zp_chemElementId();
}
//=====================================================
QString ZChemElementList::zp_chemElementName(int index) const
{
    if(index < 0 || index >= zv_elementList.count())
    {
        return QString();
    }

    return zv_elementList.value(index).element;
}
//=====================================================
bool ZChemElementList::zp_setChemElementName(int index, const QString& name)
{
    if(index < 0 || index >= zv_elementList.count() || name.isEmpty())
    {
        return false;
    }

    zv_elementList[index].element = name;
    emit zg_operationType(OT_CHANGED, index, index);
    return true;
}
//=====================================================
bool ZChemElementList::zp_chemElementIsVisible(int index) const
{
    if(index < 0 || index >= zv_elementList.count())
    {
        return false;
    }

    return zv_elementList.value(index).visibility;
}
//=====================================================
bool ZChemElementList::zp_setChemElementVisible(int index, bool visible)
{
    if(index < 0 || index >= zv_elementList.count())
    {
        return false;
    }

    if(zv_elementList.value(index).visibility == visible)
    {
        return false;
    }

    emit zg_operationType(OT_VISIBLE_CHANGE, index, index);
    zv_elementList[index].visibility = visible;
    emit zg_operationType(OT_END_VISIBLE_CHANGE, index, index);
    return true;
}
//=====================================================

