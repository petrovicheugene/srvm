//================================================
#include "X_RawTerm.h"
//================================================
X_RawTerm::X_RawTerm(X_AbstractTerm::TermType termType)
{
    name = QString();
    this->termType = termType;
    termState = X_AbstractTerm::TS_NOT_DEFINED;
    windowList = QStringList();
    customString = QString();
    factor = QString::number(0.0);
}
//================================================
X_RawCustomTerm::X_RawCustomTerm(X_AbstractTerm::TermType termType)
    : X_RawTerm(termType)
{
    termId = -1;
    descriptionString = QString();
}
//================================================
