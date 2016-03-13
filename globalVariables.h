//================================
#ifndef GLVARIABLES_H
#define GLVARIABLES_H
//================================
#include <QString>
#define DBG
#ifdef DBG
#include <QDebug>
#endif

enum UserRole {VisibleRole = Qt::UserRole + 1,
               IconRole = Qt::UserRole + 2
              };

extern const QString glAppExeBaseName;
extern const QString glAppProduct;
extern const QString glAppVersion;
extern const QString glAppCompany;
extern const QString glAppCopyright;
extern const QString glAppCompanyURL;

extern const QString glDefaultChemElementString;


#endif // GLVARIABLES_H
//================================
