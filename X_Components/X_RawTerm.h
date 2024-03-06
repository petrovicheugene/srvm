//================================================
#ifndef X_RAWTERM_H
#define X_RAWTERM_H
//================================================
#include "X_AbstractTerm.h"
#include <QString>
//================================================
class X_RawTerm
{
public:
    X_RawTerm(X_AbstractTerm::TermType termType = X_AbstractTerm::TT_NOT_DEFINED);
    virtual ~X_RawTerm() {};
    // VARS
    QString name;
    X_AbstractTerm::TermType termType;
    X_AbstractTerm::TermState termState;
    QStringList windowList;
    QString customString;
    QString factor;

    qint64 termId = -1;
    QString descriptionString;
};

class X_RawCustomTerm : public X_RawTerm
{
public:
    X_RawCustomTerm(X_AbstractTerm::TermType termType = X_AbstractTerm::TT_NOT_DEFINED);
    virtual ~X_RawCustomTerm() {};
    // VARS
//    qint64 termId;
//    QString descriptionString;
};

//================================================
#endif // X_RAWTERM_H
