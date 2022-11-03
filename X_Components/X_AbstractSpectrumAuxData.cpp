//================================================
#include "X_AbstractSpectrumAuxData.h"
//================================================
X_AbstractSpectrumAuxData::X_AbstractSpectrumAuxData(X_AbstractSpectrumAuxData::DataType type)
{
    xv_type = type;
}
//================================================
X_AbstractSpectrumAuxData::DataType X_AbstractSpectrumAuxData::xp_type() const
{
    return xv_type;
}
//================================================
