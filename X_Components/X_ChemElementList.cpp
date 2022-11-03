//=====================================================
#include "X_ChemElementList.h"
#include "X_General.h"
//=====================================================
qint64 X_ChemElementItem::xv_lastId = 0;

//=====================================================
X_ChemElementList::X_ChemElementList(QObject *parent) : QObject(parent)
{

}
//=====================================================
bool X_ChemElementList::xp_appendElement(const QString& element, qint64& chemElementId)
{
    if(xp_containsElement(element))
    {
        chemElementId = -1;
        return false;
    }

    int index = xv_elementList.count();
    X_ChemElementItem chemElementItem(element);
    chemElementId = chemElementItem.xp_id();
    emit xg_operationType(OT_INSERT_CHEM_ELEMENT, index, index);
    xv_elementList.append(chemElementItem);
    emit xg_operationType(OT_END_INSERT_CHEM_ELEMENT, index, index);
    return true;
}
//=====================================================
bool X_ChemElementList::xp_removeElement(const QString& element, qint64& chemElementId)
{
    int elementIndex = xp_findElement(element);
    if(elementIndex < 0)
    {
        return false;
    }

    chemElementId = xp_chemElementId(elementIndex);
    emit xg_operationType(OT_REMOVE_CHEM_ELEMENT, elementIndex, elementIndex);
    xv_elementList.removeAt(elementIndex);
    emit xg_operationType(OT_END_REMOVE_CHEM_ELEMENT, elementIndex, elementIndex);

    return true;
}
//=====================================================
void X_ChemElementList::xp_clearList()
{
    if(!xv_elementList.isEmpty())
    {
        int lastElementIndex = xv_elementList.count() - 1;
        emit xg_operationType(OT_REMOVE_CHEM_ELEMENT, 0, lastElementIndex);
        xv_elementList.clear();
        emit xg_operationType(OT_END_REMOVE_CHEM_ELEMENT, 0, lastElementIndex);
    }
}
//=====================================================
QStringList X_ChemElementList::xp_chemElementList() const
{
    QStringList elementList;
    for(int i = 0; i < xv_elementList.count(); i++)
    {
        elementList << xv_elementList.value(i).element;
    }

    return elementList;
}
//=====================================================
bool X_ChemElementList::xp_containsElement(const QString& element) const
{
    for(int i = 0; i < xv_elementList.count(); i++)
    {
        if(xv_elementList.value(i).element == element)
        {
            return true;
        }
    }

    return false;
}
//=====================================================
bool X_ChemElementList::xp_containsElementId(qint64 chemElementId) const
{
    for(int i = 0; i < xv_elementList.count(); i++)
    {
        if(xv_elementList.value(i).xp_id() == chemElementId)
        {
            return true;
        }
    }

    return false;
}
//=====================================================
bool X_ChemElementList::xp_isElementVisible(const QString& element) const
{
    int elementIndex = xp_findElement(element);
    if(elementIndex < 0)
    {
        return false;
    }

    return xv_elementList.value(elementIndex).visibility;

}
//=====================================================
bool X_ChemElementList::xp_setChemElementVisible(const QString& element, bool visible)
{
    int elementIndex = xp_findElement(element);
    if(elementIndex < 0)
    {
        return false;
    }

    return xp_setChemElementVisible(elementIndex, visible);
}
//=====================================================
//bool X_ChemElementList::xp_setAverageChemConcentration(const QString& element, qreal averageValue)
//{
//    int elementIndex = xp_findElement(element);
//    if(elementIndex < 0)
//    {
//        return false;
//    }

//    return xp_setAverageChemConcentration(elementIndex, averageValue);
//}
////=====================================================
//bool X_ChemElementList::xp_averageChemConcentration(const QString& element, qreal& value) const
//{
//    int elementIndex = xp_findElement(element);
//    if(elementIndex < 0)
//    {
//        value = 0.0;
//        return false;
//    }

//    return xp_averageChemConcentration(elementIndex, value);
//}
////=====================================================
//bool X_ChemElementList::xp_setAverageChemConcentration(int index, qreal averageValue)
//{
//    if(index < 0 || index >= xv_elementList.count())
//    {
//        return false;
//    }

//    if(xv_elementList.value(index).averageConcentrationValue == averageValue)
//    {
//        return false;
//    }

//    xv_elementList[index].averageConcentrationValue = averageValue;
//    emit xg_operationType(OT_AVERAGE_VALUE_CHANGED, index, index);
//    return true;
//}
////=====================================================
//bool X_ChemElementList::xp_averageChemConcentration(int index, qreal& value) const
//{
//    if(index < 0 || index >= xv_elementList.count())
//    {
//        value = 0.0;
//        return false;
//    }

//    value = xv_elementList.value(index).averageConcentrationValue;
//    return true;
//}
////=====================================================
//bool X_ChemElementList::xp_setAverageChemConcentrationForChemElementId(qint64 chemId, qreal value)
//{
//    if(chemId < 0)
//    {
//        return false;
//    }

//    for(int i = 0; i < xv_elementList.count(); i++)
//    {
//        if(xv_elementList.at(i).xp_id() != chemId)
//        {
//            continue;
//        }

//        xv_elementList[i].averageConcentrationValue = value;
//        emit xg_operationType(OT_AVERAGE_VALUE_CHANGED, i, i);

//        return true;
//    }
//    return false;
//}
////=====================================================
//bool X_ChemElementList::xp_averageChemConcentrationForChemElementId(qint64 chemId, qreal &value) const
//{
//    if(chemId < 0)
//    {
//        return false;
//    }

//    for(int i = 0; i < xv_elementList.count(); i++)
//    {
//        if(xv_elementList.at(i).xp_id() != chemId)
//        {
//            continue;
//        }

//        value = xv_elementList.at(i).averageConcentrationValue;
//        return true;
//    }
//    return false;
//}
//=====================================================
int X_ChemElementList::xp_findElement(const QString& element) const
{
    for(int i = 0; i < xv_elementList.count(); i++)
    {
        if(xv_elementList.value(i).element == element)
        {
            return i;
        }
    }

    return -1;
}
//=====================================================
int X_ChemElementList::xp_chemElementCount() const
{
    return xv_elementList.count();
}
//=====================================================
int X_ChemElementList::xp_visibleElementCount() const
{
    int visible = 0;
    for(int i = 0; i < xv_elementList.count(); i++)
    {
        if(xv_elementList.value(i).visibility)
        {
            visible++;
        }
    }
    return visible;
}
//=====================================================
int X_ChemElementList::xp_numberVisibleChemElementsBeforeIndex(int index) const
{
    if(index < 0)
    {
        return -1;
    }

    int limitIndex;
    if(index > xv_elementList.count())
    {
        limitIndex = xv_elementList.count();
    }
    else
    {
        limitIndex = index;
    }

    int visible = 0;
    for(int i = 0; i < limitIndex; i++)
    {
        if(xv_elementList.value(i).visibility)
        {
            visible++;
        }
    }

    return visible;
}
//=====================================================
bool X_ChemElementList::xp_isEmpty() const
{
    return xv_elementList.isEmpty();
}
//=====================================================
QString X_ChemElementList::xp_visibleChemElementName(int visibleIndex) const
{
    if(visibleIndex < 0 || visibleIndex >= xp_visibleElementCount())
    {
        return QString();
    }

    int visible = 0;
    for(int i = 0; i < xv_elementList.count(); i++)
    {
        if(xv_elementList.value(i).visibility)
        {
            if(visible == visibleIndex)
            {
                return xv_elementList.value(i).element;
            }
            visible++;
        }
    }

    return QString();
}
//=====================================================
qint64 X_ChemElementList::xp_visibleChemElementId(int visibleIndex) const
{
    if(visibleIndex < 0 || visibleIndex >= xp_visibleElementCount())
    {
        return -1;
    }

    int visible = 0;
    for(int i = 0; i < xv_elementList.count(); i++)
    {
        if(xv_elementList.value(i).visibility)
        {
            if(visible == visibleIndex)
            {
                return xv_elementList.value(i).xp_id();
            }
            visible++;
        }
    }

    return -1;
}
//=====================================================
qint64 X_ChemElementList::xp_chemElementId(int index) const
{
    if(index < 0 || index >= xv_elementList.count())
    {
        return -1;
    }

    return xv_elementList.value(index).xp_id();
}
//=====================================================
QString X_ChemElementList::xp_chemElementName(int index) const
{
    if(index < 0 || index >= xv_elementList.count())
    {
        return QString();
    }

    return xv_elementList.value(index).element;
}
//=====================================================
bool X_ChemElementList::xp_setChemElementName(int index, const QString& name)
{
    if(index < 0 || index >= xv_elementList.count() || name.isEmpty())
    {
        return false;
    }

    xv_elementList[index].element = name;
    emit xg_operationType(OT_CHANGED, index, index);
    return true;
}
//=====================================================
bool X_ChemElementList::xp_chemElementIsVisible(int index) const
{
    if(index < 0 || index >= xv_elementList.count())
    {
        return false;
    }

    return xv_elementList.value(index).visibility;
}
//=====================================================
bool X_ChemElementList::xp_setChemElementVisible(int index, bool visible)
{
    if(index < 0 || index >= xv_elementList.count())
    {
        return false;
    }

    if(xv_elementList.value(index).visibility == visible)
    {
        return false;
    }

    emit xg_operationType(OT_VISIBLE_CHANGE, index, index);
    xv_elementList[index].visibility = visible;
    emit xg_operationType(OT_END_VISIBLE_CHANGE, index, index);
    return true;
}
//=====================================================

