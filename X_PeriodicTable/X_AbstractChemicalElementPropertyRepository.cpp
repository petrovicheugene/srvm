//======================================================
#include "X_AbstractChemicalElementPropertyRepository.h"
//======================================================
X_AbstractChemicalElementPropertyRepository::X_AbstractChemicalElementPropertyRepository()
{
    xv_chemicalElementCount = 118;
}
//======================================================
X_AbstractChemicalElementPropertyRepository::~X_AbstractChemicalElementPropertyRepository()
{

}
//======================================================
int X_AbstractChemicalElementPropertyRepository::xp_chemicalElementCount() const
{
    return xv_chemicalElementCount;
}
//======================================================
QString X_AbstractChemicalElementPropertyRepository::xp_lastError() const
{
    return xv_errorMsg;
}
//======================================================
