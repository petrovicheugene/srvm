//=========================================================
#include "ZChemElementDataManager.h"
#include "globalVariables.h"
//=========================================================
ZChemElementDataManager::ZChemElementDataManager(QObject *parent) : QObject(parent)
{
    zv_currentArrayIndex = -1;
    zv_currentArrayId = -1;
    zv_repositiry = 0;
}
//=========================================================
void ZChemElementDataManager::zp_connectToSpectraArrayRepository(ZSpectrumArrayRepository* repository)
{
    zv_repositiry = repository;
    // array repository <-> array model
    connect(repository, &ZSpectrumArrayRepository::zg_currentChemElementOperation,
            this, &ZChemElementDataManager::zh_onRepositoryChemElementOperation);
    connect(repository, &ZSpectrumArrayRepository::zg_currentArrayId,
            this, &ZChemElementDataManager::zp_currentArrayChanged);
}
//=========================================================
int ZChemElementDataManager::zp_rowCount() const
{
    if(zv_currentArrayIndex < 0 || zv_repositiry == 0)
    {
        return 0;
    }

    return zv_repositiry->zp_chemElementCount(zv_currentArrayIndex);
}
//=========================================================
int ZChemElementDataManager::zp_columnCount() const
{
    return 1;
}
//=========================================================
QString ZChemElementDataManager::zp_chemElementName(int row) const
{
    if(zv_repositiry == 0)
    {
        return QString();
    }

    return zv_repositiry->zp_chemElementName(zv_currentArrayIndex, row);
}
//=========================================================
bool ZChemElementDataManager::zp_chemElementIsVisible(int row) const
{
    if(zv_repositiry == 0)
    {
        return false;
    }

    return zv_repositiry->zp_chemElementIsVisible(zv_currentArrayIndex, row);
}
//=========================================================
QString ZChemElementDataManager::zp_columnName(int columnIndex) const
{
    if(columnIndex == 0)
    {
        return tr("Chemical element");
    }

    return QString();
}
//=========================================================
bool ZChemElementDataManager::zp_setVisible(int row, bool visible) const
{
    if(zv_repositiry == 0)
    {
        return false;
    }

    return zv_repositiry->zp_setChemElementVisible(zv_currentArrayIndex, row, visible);
}
//=========================================================
bool ZChemElementDataManager::zp_setChemElementName(int chemElementIndex, const QString& chemElementName) const
{
    if(zv_repositiry == 0)
    {
        return false;
    }

    return zv_repositiry->zp_setChemElementName(zv_currentArrayIndex, chemElementIndex, chemElementName);
}
//=========================================================
void ZChemElementDataManager::zp_currentArrayChanged(qint64 currentArrayId, int currentArrayIndex)
{
    if(zv_currentArrayIndex == currentArrayIndex && zv_currentArrayId == currentArrayId)
    {
        return;
    }

#ifdef DBG
    qDebug() << "ChemElementDataManager" << currentArrayId << currentArrayIndex;
#endif

    emit zg_currentOperation(OT_RESET_DATA, -1, -1);
    zv_currentArrayIndex = currentArrayIndex;
    zv_currentArrayId = currentArrayId;
    emit zg_currentOperation(OT_END_RESET_DATA, -1, -1);
}
//=========================================================
void ZChemElementDataManager::zh_onRepositoryChemElementOperation(ZSpectrumArrayRepository::ChemElementOperationType type,
                                                                  int arrayIndex, int first, int last)
{
    if(zv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    if(type == ZSpectrumArrayRepository::CEOT_INSERT_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_INSERT_ROW, first, last);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_END_INSERT_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_END_INSERT_ROW, first, last);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_REMOVE_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_REMOVE_ROW, first, last);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_END_REMOVE_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_END_REMOVE_ROW, first, last);
    }
    else if(type == ZSpectrumArrayRepository::CEOT_CHEM_ELEMENT_CHANGED ||
            type == ZSpectrumArrayRepository::CEOT_END_CHEM_ELEMENT_VISIBILITY_CHANGE)
    {
        emit zg_currentOperation(OT_DATA_CHANGED, first, last);
    }
 }
//=========================================================

