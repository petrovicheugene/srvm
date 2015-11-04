//==================================================================
#include "ZJointSpectraDataManager.h"
#include "glVariables.h"
//==================================================================
ZJointSpectraDataManager::ZJointSpectraDataManager(QObject *parent) : QObject(parent)
{
    zv_currentArrayIndex = -1;
    zv_repositiry = 0;
}
//==================================================================
void ZJointSpectraDataManager::zp_setSpectraArrayRepository(ZSpectraArrayRepository* repository)
{
    zv_repositiry = repository;
    // array repository <-> array model
    connect(repository, &ZSpectraArrayRepository::zg_currentSpectrumOperation,
            this, &ZJointSpectraDataManager::zh_onRepositoryArrayOperation);
    connect(repository, &ZSpectraArrayRepository::zg_currentChemElementOperation,
            this, &ZJointSpectraDataManager::zh_onRepositoryChemElementOperation);

}
//==================================================================
int ZJointSpectraDataManager::zp_rowCount() const
{
    if(zv_currentArrayIndex < 0 || zv_repositiry == 0)
    {
        return 0;
    }

    return zv_repositiry->zp_spectrumCount(zv_currentArrayIndex);
}
//==================================================================
int ZJointSpectraDataManager::zp_columnCount() const
{
    if(zv_currentArrayIndex < 0 || zv_repositiry == 0)
    {
        return zv_spectrumDataColumnCount;
    }

    int visibleElementCount = zv_repositiry->zp_visibleChemElementCount(zv_currentArrayIndex);

    return zv_spectrumDataColumnCount + visibleElementCount;
}
//==================================================================
int ZJointSpectraDataManager::zp_visibleChemElementCount() const
{
    if(zv_currentArrayIndex < 0 || zv_repositiry == 0)
    {
        return 0;
    }

    return zv_repositiry->zp_visibleChemElementCount(zv_currentArrayIndex);
}
//==================================================================
bool ZJointSpectraDataManager::zp_isColumnChemElementColumn(int column) const
{
    if(zv_repositiry == 0)
    {
        return false;
    }

    if(column < zv_spectrumDataColumnCount
            || column > zv_repositiry->zp_visibleChemElementCount(zv_currentArrayIndex)
            + zv_spectrumDataColumnCount)
    {
        return false;
    }

    return true;
}
//==================================================================
QVariant ZJointSpectraDataManager::zp_data(QModelIndex index) const
{
    if(!zv_repositiry || !index.isValid())
    {
        return QVariant();
    }

    if(index.column() == 0)
    {
        return QVariant(zv_repositiry->zp_spectrumName(zv_currentArrayIndex, index.row()));
    }
    if(index.column() == 1)
    {

        return QVariant();
        // return QVariant(zv_repositiry->zp_spectrum(zv_currentArrayIndex, index.row()));
    }

    int visibleChemElementCount = zv_repositiry->zp_visibleChemElementCount(zv_currentArrayIndex);
    if(index.column() >= zv_spectrumDataColumnCount && index.column() < visibleChemElementCount + zv_spectrumDataColumnCount)
    {
        QString concentrationString = zv_repositiry->zp_chemConcentration(zv_currentArrayIndex,
                                                                          index.row(),
                                                                          index.column() - zv_spectrumDataColumnCount);
#ifdef DBG
        qDebug() << concentrationString << "COL" << index.column() - zv_spectrumDataColumnCount <<
                    "ROW" << index.row();
#endif
        return QVariant(concentrationString);
    }



    return QVariant();
}
//==================================================================
QString ZJointSpectraDataManager::zp_columnName(int columnIndex) const
{

    if(columnIndex == 0)
    {
        return tr("File");
    }
    else if(columnIndex == 1)
    {
        return tr("Spectrum");
    }

    if(zv_repositiry == 0)
    {
        return QString();
    }

    int chemElementIndex = columnIndex - zv_spectrumDataColumnCount;
    if(chemElementIndex >= 0 && chemElementIndex < zv_repositiry->zp_visibleChemElementCount(zv_currentArrayIndex))
    {
        return zv_repositiry->zp_visibleChemElementName(zv_currentArrayIndex, chemElementIndex);
    }

    return QString();
}
//==================================================================
bool ZJointSpectraDataManager::zp_setChemConcentration(int row, int column, const QString& consentration)
{
    if(!zv_repositiry || !zp_isColumnChemElementColumn(column)
            || zv_currentArrayIndex < 0 )
    {
        return false;
    }

    bool ok;
    consentration.toDouble(&ok);
    if(!ok)
    {
        return false;
    }

    return zv_repositiry->zp_setChemConcentration(zv_currentArrayIndex,
                                                  row,
                                                  column - zv_spectrumDataColumnCount,
                                                  consentration);
}
//==================================================================
void ZJointSpectraDataManager::zp_currentArrayChanged(int current, int previous)
{
#ifdef DBG
    qDebug() << "ARRAY CURRENT" << current << "PREVIOUS" << previous;
#endif

    emit zg_currentOperation(OT_RESET_DATA, -1, -1);
    zv_currentArrayIndex = current;
    emit zg_currentOperation(OT_END_RESET_DATA, -1, -1);

}
//==================================================================
void ZJointSpectraDataManager::zh_onRepositoryArrayOperation(ZSpectraArrayRepository::SpectrumOperationType type,
                                                             int arrayIndex, int first, int last)
{
    if(zv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    if(type == ZSpectraArrayRepository::SOT_INSERT_SPECTRA)
    {
        emit zg_currentOperation(OT_INSERT_ROW, first, last);
    }
    else if(type == ZSpectraArrayRepository::SOT_END_INSERT_SPECTRA)
    {
        emit zg_currentOperation(OT_END_INSERT_ROW, first, last);
    }
    else if(type == ZSpectraArrayRepository::SOT_REMOVE_SPECTRA)
    {
        emit zg_currentOperation(OT_REMOVE_ROW, first, last);
    }
    else if(type == ZSpectraArrayRepository::SOT_END_REMOVE_SPECTRA)
    {
        emit zg_currentOperation(OT_END_REMOVE_ROW, first, last);
    }

}
//==================================================================
void ZJointSpectraDataManager::zh_onRepositoryChemElementOperation(ZSpectraArrayRepository::ChemElementOperationType type,
                                                                   int arrayIndex, int first, int last)
{
    if(zv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    int residual = last - first;

    int visibleFirst = zh_convertChemElementIndexToVisibleChemElementIndex(zv_currentArrayIndex, first);

    if(visibleFirst < 0)
    {
        visibleFirst = 0;
    }

    int visibleLast = visibleFirst + residual;

    visibleFirst += zv_spectrumDataColumnCount;
    visibleLast += zv_spectrumDataColumnCount;

    if(type == ZSpectraArrayRepository::CEOT_INSERT_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_INSERT_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZSpectraArrayRepository::CEOT_END_INSERT_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_END_INSERT_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZSpectraArrayRepository::CEOT_REMOVE_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_REMOVE_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZSpectraArrayRepository::CEOT_END_REMOVE_CHEM_ELEMENT)
    {
        emit zg_currentOperation(OT_END_REMOVE_COLUMN, visibleFirst, visibleLast);
    }
    else if(type == ZSpectraArrayRepository::CEOT_CHEM_ELEMENT_CHANGED)
    {
        // TODO  Changing column name
        // emit zg_currentOperation(OT_);

        //        emit zg_currentOperation(OT_INSERT_COLUMN, first, last);
        //        emit zg_currentOperation(OT_END_INSERT_COLUMN, first, last);
    }
    else if(type == ZSpectraArrayRepository::CEOT_CHEM_ELEMENT_VISIBLE_CHANGED)
    {
        bool visible = zv_repositiry->zp_chemElementIsVisible(zv_currentArrayIndex, first);
        if(visible)
        {
            emit zg_currentOperation(OT_INSERT_COLUMN, visibleFirst, visibleLast);
            emit zg_currentOperation(OT_END_INSERT_COLUMN, visibleFirst, visibleLast);
        }
        else
        {
            emit zg_currentOperation(OT_REMOVE_COLUMN, visibleFirst, visibleLast);
            emit zg_currentOperation(OT_END_REMOVE_COLUMN, visibleFirst, visibleLast);
        }
    }
}
//==================================================================
int ZJointSpectraDataManager::zh_convertChemElementIndexToVisibleChemElementIndex(int arrayIndex,
                                                                                  int originalIndex)
{
    if(zv_repositiry == 0)
    {
        return -1;
    }

    return zv_repositiry->zp_numberVisibleChemElementsBeforeIndex(arrayIndex, originalIndex);
}
//==================================================================
