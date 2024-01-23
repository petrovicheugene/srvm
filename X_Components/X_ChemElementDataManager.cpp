//=========================================================
#include "X_ChemElementDataManager.h"
#include "X_General.h"
//=========================================================
X_ChemElementDataManager::X_ChemElementDataManager(QObject *parent) : QObject(parent)
{
    xv_currentArrayIndex = -1;
    xv_currentArrayId = -1;
    xv_repositiry = 0;
}
//=========================================================
void X_ChemElementDataManager::xp_connectToSpectraArrayRepository(X_SpectrumArrayRepository* repository)
{
    xv_repositiry = repository;
    // array repository <-> array model
    connect(repository, &X_SpectrumArrayRepository::xg_chemElementOperation,
            this, &X_ChemElementDataManager::xh_onRepositoryChemElementOperation);
    connect(repository, &X_SpectrumArrayRepository::xg_currentArrayIdChanged,
            this, &X_ChemElementDataManager::xp_currentArrayChanged);
}
//=========================================================
int X_ChemElementDataManager::xp_rowCount() const
{
    if(xv_currentArrayIndex < 0 || xv_repositiry == 0)
    {
        return 0;
    }

    return xv_repositiry->xp_chemElementCount(xv_currentArrayIndex);
}
//=========================================================
int X_ChemElementDataManager::xp_columnCount() const
{
    return 1;
}
//=========================================================
QString X_ChemElementDataManager::xp_chemElementName(int row) const
{
    if(xv_repositiry == 0)
    {
        return QString();
    }

    return xv_repositiry->xp_chemElementName(xv_currentArrayIndex, row);
}
//=========================================================
bool X_ChemElementDataManager::xp_chemElementIsVisible(int row) const
{
    if(xv_repositiry == 0)
    {
        return false;
    }

    return xv_repositiry->xp_chemElementIsVisible(xv_currentArrayIndex, row);
}
//=========================================================
QString X_ChemElementDataManager::xp_columnName(int columnIndex) const
{
    if(columnIndex == 0)
    {
        return tr("Chemical element");
    }

    return QString();
}
//=========================================================
bool X_ChemElementDataManager::xp_setVisible(int row, bool visible) const
{
    if(xv_repositiry == 0)
    {
        return false;
    }

    return xv_repositiry->xp_setChemElementVisible(xv_currentArrayIndex, row, visible);
}
//=========================================================
bool X_ChemElementDataManager::xp_setChemElementName(int chemElementIndex, const QString& chemElementName) const
{
    if(xv_repositiry == 0)
    {
        return false;
    }

    return xv_repositiry->xp_setChemElementName(xv_currentArrayIndex, chemElementIndex, chemElementName);
}
//=========================================================
void X_ChemElementDataManager::xp_currentArrayChanged(qint64 currentArrayId, int currentArrayIndex)
{
    if(xv_currentArrayIndex == currentArrayIndex && xv_currentArrayId == currentArrayId)
    {
        return;
    }

    emit xg_currentOperation(OT_RESET_DATA, -1, -1);
    xv_currentArrayIndex = currentArrayIndex;
    xv_currentArrayId = currentArrayId;
    emit xg_currentOperation(OT_END_RESET_DATA, -1, -1);
}
//=========================================================
void X_ChemElementDataManager::xh_onRepositoryChemElementOperation(X_SpectrumArrayRepository::ChemElementOperationType type,
                                                                  int arrayIndex, int first, int last)
{
    if(xv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    if(type == X_SpectrumArrayRepository::CEOT_INSERT_CHEM_ELEMENT)
    {
        emit xg_currentOperation(OT_INSERT_ROW, first, last);
    }
    else if(type == X_SpectrumArrayRepository::CEOT_END_INSERT_CHEM_ELEMENT)
    {
        emit xg_currentOperation(OT_END_INSERT_ROW, first, last);
    }
    else if(type == X_SpectrumArrayRepository::CEOT_REMOVE_CHEM_ELEMENT)
    {
        emit xg_currentOperation(OT_REMOVE_ROW, first, last);
    }
    else if(type == X_SpectrumArrayRepository::CEOT_END_REMOVE_CHEM_ELEMENT)
    {
        emit xg_currentOperation(OT_END_REMOVE_ROW, first, last);
    }
    else if(type == X_SpectrumArrayRepository::CEOT_CHEM_ELEMENT_CHANGED ||
            type == X_SpectrumArrayRepository::CEOT_END_CHEM_ELEMENT_VISIBILITY_CHANGE)
    {
        emit xg_currentOperation(OT_DATA_CHANGED, first, last);
    }
 }
//=========================================================

