//================================
#ifndef GLVARIABLES_H
#define GLVARIABLES_H
//================================
#include <QString>
#include <QIcon>
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

// icons
extern const QString glOpenIconString;
extern const QString glAddArrayIconString;
extern const QString glAddSpectrumIconString;
extern const QString glAddChemElementIconString;
extern const QString glAddCalibrationIconString;
extern const QString glAddWindowIconString;
extern const QString glAddTermIconString;

extern const QString glRemoveIconString;
extern const QString glRemoveSpectrumIconString;
extern const QString glRemoveChemElementIconString;
extern const QString glRemoveArrayIconString;
extern const QString glRemoveCalibrationIconString;
extern const QString glRemoveWindowIconString;

extern const QString glClearIconString;
extern const QString glCopyIconString;
extern const QString glPasteIconString;

extern const QString glCalcIconString;
extern const QString glArrayIconString;
extern const QString glSpectrumIconString;
extern const QString glCalibrationIconString;
extern const QString glWindowIconString;
extern const QString glElementIconString;

#endif // GLVARIABLES_H
//================================
