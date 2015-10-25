//==================================================================
#include "ZSpectraJointDataManager.h"
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
            this, &ZJointSpectraDataManager::zh_onRepositoryOperation);
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
        return 0;
    }

    return 2;
}
//==================================================================
QVariant ZJointSpectraDataManager::zp_data(QModelIndex index) const
{
    if(!zv_repositiry)
    {
        return QVariant();
    }

    if(index.column() == 0)
    {
        return QVariant(zv_repositiry->zp_spectrumName(zv_currentArrayIndex, index.row()));
    }
    if(index.column() == 1)
    {

        // return QVariant(zv_repositiry->zp_spectrum(zv_currentArrayIndex, index.row()));
    }
    if(index.column() == 2)
    {
      //  return QVariant(zv_repositiry->zp_concentration(zv_currentChemElement, index.row()));
    }
    if(index.column() == 3)
    {

    }

    return QVariant();
}
//==================================================================
QString ZJointSpectraDataManager::zp_columnName(int columnIndex) const
{
    QString name;
    switch(columnIndex)
    {
    case 0:
        name = "File";
        break;
    case 1:
        name = "Spectrum";
        break;
    case 2:
        name = "B";
        break;
    case 3:
        name = "A";
        break;
    case 4:
        name = "M";
        break;

    default:
        name = "N/A";
    }
    return name;
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
void ZJointSpectraDataManager::zh_onRepositoryOperation(ZSpectraArrayRepository::SpectrumOperationType type,
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

