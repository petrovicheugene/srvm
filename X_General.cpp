//===========================================================
#include <X_General.h>
#include <QObject>
//===========================================================
extern const QString glDefaultDBSuffixString = "sqlite";
extern const QString glDefaultChemElementString = "Not defined";
extern const QString glAppSettingsKeyName = "AppSettings";
extern const QString glAppGeometryKeyName = "AppGeometry";
extern const QString glAppStateKeyName = "AppState";

// Icons
namespace NS_Icons
{
extern const QString glIconSettings = "";
extern const QString glIconExitApp = ":/images/X_Images/exit-1.png";
extern const QString glIconAbout = ":/images/X_Images/info-1.png";
extern const QString glIconHelp = ":/images/X_Images/help-1.png";
extern const QString glIconQt = ":/images/X_Images/Qt_logo.png";

extern const QString glIconSidebarLeftOpen = ":/images/X_Widgets/sidebarLeftOpen-1.png";
extern const QString glIconSidebarLeftClose = ":/images/X_Widgets/sidebarLeftClose-1.png";
extern const QString glIconSidebarRightOpen = ":/images/X_Widgets/sidebarRightOpen-1.png";
extern const QString glIconSidebarRightClose = ":/images/X_Widgets/sidebarRightClose-1.png";

extern const QString glIconAxisToEnergy = ":/images/X_Images/energy.png";
extern const QString glIconShowGrid = ":/images/X_Images/gridVisible-2.png";
extern const QString glIconVerticalIncrease = ":/images/X_Images/verticalIncrease-2.png";
extern const QString glIconVerticalDecrease = ":/images/X_Images/verticalDecrease-2.png";
extern const QString glIconHorizontalIncrease = ":/images/X_Images/horizontalIncrease-2.png";
extern const QString glIconHorizontalDecrease = ":/images/X_Images/horizontalDecrease-2.png";
extern const QString glIconFitInRect = ":/images/X_Images/fitInSceneRect.png";


extern const QString glIconOpenArray = ":/images/X_Images/openArray-1.png";
extern const QString glIconSaveArray = ":/images/X_Images/save-1.png";
extern const QString glIconSaveAsArray = "";
extern const QString glIconOpenCalibration = ":/images/X_Images/openCalibration-1.png";
extern const QString glIconSaveCalibration = ":/images/X_Images/saveCalibration-1.png";
extern const QString glIconSaveAsCalibration = "";

extern const QString glIconAddCalibration = ":/images/X_Images/addCalibration-1.png";
extern const QString glIconRemoveCalibration = ":/images/X_Images/removeCalibration-1.png";
extern const QString glIconAddWindow = ":/images/X_Images/addWindow-1.png";
extern const QString glIconRemoveWindow = ":/images/X_Images/removeWindow-1.png";

extern const QString glIconCalc = ":/images/X_Images/calculate-1.png";
extern const QString glIconEnergyCalibration = "";
extern const QString glIconAddMixedTerms = "";
extern const QString glIconRemoveMixedTerms = "";

extern const QString glIconAddCustomTerm = "";
extern const QString glIconRemoveCustomTerm = "";

extern const QString glIconVisible = ":/images/X_Images/eyeSignBlack-1.png";
extern const QString glIconInvisible = "";
extern const QString glIconInverseVisible = "";
extern const QString glIconDuplicateCalibration = ":/images/X_Images/copy-1.png";
extern const QString glIconRemove = ":/images/X_Images/remove-16.png";
extern const QString glIconReset = "";

extern const QString glAddArray = ":/images/X_Images/addArray-3.png";
extern const QString glRemoveArrayIconString = ":/images/X_Images/removeArray-4.png";
extern const QString glAddSpectrumIconString = ":/images/X_Images/addSpectrum-2.png";
extern const QString glRemoveSpectrumIconString = ":/images/X_Images/removeSpectrum-4.png";
extern const QString glAddChemElementIconString = ":/images/X_Images/addElement-2.png";
extern const QString glRemoveChemElementIconString = ":/images/X_Images/removeElement-2.png";
extern const QString glCopyIconString = ":/images/X_Images/copy-1.png";
extern const QString glPasteIconString = ":/images/X_Images/paste-1.png";
extern const QString glClearIconString = ":/images/X_Images/clear-8.png";

extern const QString glConstExcludedIconString = ":/images/X_Images/Unchecked-16.png";
extern const QString glConstIncludedIconString = ":/images/X_Images/checked-16.png";
extern const QString glExamWaitingIconString = ":/images/X_Images/question.png";
extern const QString glIncludedIconString = ":/images/X_Images/openSpeFolder-2.png";
extern const QString glExcludedIconString = ":/images/X_Images/openSpeFolder-2.png";
extern const QString glBaseTermIconString = ":/images/X_Images/B-sign-16.png";

}

// Common non translatable strings

// object Names
namespace NS_ObjectNames
{
extern const QString glObjectNameMenuFile = "File";
extern const QString glObjectNameMenuEdit = "Edit";
extern const QString glObjectNameMenuView = "View";
extern const QString glObjectNameMenuActions = "Actions";
extern const QString glObjectNameMenuHelp = "Help";
}

// Common strings
namespace NS_CommonStrings
{
extern const QString glSettings = QObject::tr("Settings");
extern const QString glSettingsToolTip = QObject::tr("Run settings of the application");
extern const QString glExit = QObject::tr("Exit");
extern const QString glExitToolTip = QObject::tr("Exit the application");
extern const QString glAbout = QObject::tr("About");
extern const QString glAboutToolTip = QObject::tr("About the application");
extern const QString glHelp = QObject::tr("Help");
extern const QString glHelpToolTip = QObject::tr("Show user guide");

extern const QString glError = QObject::tr("Error");
extern const QString glWarning = QObject::tr("Warning");
extern const QString glCommon = QObject::tr("Common");
extern const QString glColor = QObject::tr("Color");
extern const QString glSelectColor = QObject::tr("Color...");
}

// Menu strings
namespace NS_Menus
{
extern const QString glMenuFile = QObject::tr("File");
extern const QString glMenuEdit = QObject::tr("Edit");
extern const QString glMenuView = QObject::tr("View");
extern const QString glMenuActions = QObject::tr("Actions");
extern const QString glMenuHelp = QObject::tr("Help");
}
// button strings
namespace NS_Buttons
{
extern const QString glButtonOk = QObject::tr("OK");
extern const QString glButtonApply = QObject::tr("Apply");
extern const QString glButtonCancel = QObject::tr("Cancel");
extern const QString glButtonBrowse = QObject::tr("Browse");
extern const QString glButtonNew = QObject::tr("New");

}
// window captions
namespace NS_WindowCaptions
{
extern const QString glOpenFile = QObject::tr("Open file");
extern const QString glSelectColor = QObject::tr("Select color");
}

extern const int glMaxGainFactor = 255;
extern const int glSpectrumDataType = 1;

namespace NS_DataRole
{

extern const int PathRole = Qt::UserRole + 1;
extern const int CommentRole = Qt::UserRole + 2;
extern const int DataTypeRole = Qt::UserRole + 3;
extern const int VisibleRole = Qt::UserRole + 4;
extern const int AbstractSpectrumRole = Qt::UserRole + 5;
extern const int IconRole =  Qt::UserRole + 6;
}

// FUNCS
QString glCreateCaption(const QString& simpleText)
{
    if(simpleText.isEmpty())
    {
        return QString();
    }

    return QString("<font color=darkGreen><b>%1</b></font>").arg(simpleText);
}
