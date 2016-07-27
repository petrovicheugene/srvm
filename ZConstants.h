//================================
#ifndef GLVARIABLES_H
#define GLVARIABLES_H
//================================
#include <QString>
#include <QIcon>
//#define DBG
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

extern const QString glHorizontalDecreaseIconString;
extern const QString glHorizontalIncreaseIconString;
extern const QString glVerticalDecreaseIconString;
extern const QString glVerticalIncreaseIconString;
extern const QString glFitInRectIconString;
extern const QString glAxisToEnergyIconString;
extern const QString glShowGridIconString;

extern const QString glSidebarLeftOpenIconString;
extern const QString glSidebarLeftCloseIconString;
extern const QString glSidebarRightOpenIconString;
extern const QString glSidebarRightCloseIconString;


extern const QString glExitAppIconString;
extern const QString glAboutIconString;
extern const QString glHelpIconString;

extern const QString glVisibleSignIconString;

extern const QString glVisibleIconString;
extern const QString glInvisibleIconString;
extern const QString glInverseVisibleIconString;

extern const QString glConstExcludedIconString;
extern const QString glConstIncludedIconString;
extern const QString glExamWaitingIconString;
extern const QString glIncludedIconString;
extern const QString glExcludedIconString;
extern const QString glBaseTermIconString;

extern const QString glOpenArrayIconString;
extern const QString glSaveArrayIconString;
extern const QString glSaveAsArrayIconString;

extern const QString glOpenCalibrationIconString;
extern const QString glSaveCalibrationIconString;
extern const QString glSaveAsCalibrationIconString;
extern const QString glDuplicateCalibrationIconString;

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

extern const QString glResetIconString;
extern const QString glAddMixedTermsIconString;
extern const QString glRemoveMixedTermsIconString;
extern const QString glAddCustomTermIconString;
extern const QString glRemoveCustomTermIconString;

extern const QString glArrayIconString;
extern const QString glSpectrumIconString;
extern const QString glCalibrationIconString;
extern const QString glWindowIconString;
extern const QString glElementIconString;

#endif // GLVARIABLES_H
//================================
