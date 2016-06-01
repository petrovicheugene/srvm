//================================================
#include "ZRawTerm.h"
//================================================
ZRawTerm::ZRawTerm()
{
    termType = ZAbstractTerm::TT_NOT_DEFINED;
    termState = ZAbstractTerm::TS_NOT_DEFINED;
    windowList = QStringList();
    customString = QString();
    factor = 0.0;
}
//================================================

