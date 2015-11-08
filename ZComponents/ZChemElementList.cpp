//=====================================================
#include "ZChemElementList.h"
#include "glVariables.h"
//=====================================================
ZChemElementList::ZChemElementList(QObject *parent) : QObject(parent)
{

}
//=====================================================
bool ZChemElementList::zp_appendElement(const QString& element)
{
    if(zp_containsElement(element))
    {
        return false;
    }

    int index = zv_elementList.count();
    emit zg_operationType(OT_INSERT_CHEM_ELEMENT, index, index);
    zv_elementList.append(ZChemElement(element));
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
    //    if(zv_elementList[elementIndex].visibility == visible)
    //    {
    //        return false;
    //    }

    //    zv_elementList[elementIndex].visibility = visible;
    //    emit zg_operationType(OT_CHANGED, elementIndex, elementIndex);
    //    return true;
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

