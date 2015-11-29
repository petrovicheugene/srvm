//===========================================================
#include "ZPlotterDataManager.h"
//===========================================================
ZPlotterDataManager::ZPlotterDataManager(QObject *parent) : QObject(parent)
{
    zv_spectrumArrayRepositiry = 0;
    zv_currentArrayIndex = -1;
}
//===========================================================
void ZPlotterDataManager::zp_setSpectraArrayRepository(ZSpectraArrayRepository* repository)
{
    zv_spectrumArrayRepositiry = repository;
    // array repository <->  model
    connect(repository, &ZSpectraArrayRepository::zg_currentSpectrumOperation,
            this, &ZPlotterDataManager::zh_onRepositoryArrayOperation);

}
//===========================================================
void ZPlotterDataManager::zp_currentArrayChanged(int current, int previous)
{
    if(zv_currentArrayIndex == current)
    {
        return;
    }
    zv_currentArrayIndex = current;
}
//===========================================================
void ZPlotterDataManager::zh_onRepositoryArrayOperation(ZSpectraArrayRepository::SpectrumOperationType type,
                                                             int arrayIndex, int first, int last)
{
    if(zv_currentArrayIndex != arrayIndex)
    {
        return;
    }

    if(type == ZSpectraArrayRepository::SOT_INSERT_SPECTRA)
    {

    }
    else if(type == ZSpectraArrayRepository::SOT_END_INSERT_SPECTRA)
    {

    }
    else if(type == ZSpectraArrayRepository::SOT_REMOVE_SPECTRA)
    {

    }
    else if(type == ZSpectraArrayRepository::SOT_END_REMOVE_SPECTRA)
    {

    }
    else if(type == ZSpectraArrayRepository::SOT_DATA_CHANGED)
    {

    }
}
//===========================================================
