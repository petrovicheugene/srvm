//===============================================
#include "MainWindow.h"
#include "globalVariables.h"

#include <windows.h>
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QDir>
#include <QSplashScreen>
#include <QPixmap>
//===============================================

extern const QString glAppExeBaseName =  APP_EXE_BASE_NAME; // w/o version
extern const QString glAppProduct =  APP_PRODUCT;
extern const QString glAppVersion = APP_VERSION;
extern const QString glAppCompany = APP_COMPANY;
extern const QString glAppCopyright = APP_COPYRIGHT;
extern const QString glAppCompanyURL = APP_COMPANY_URL;

extern const QString glDefaultChemElementString = "Not defined";

extern const QString glHorizontalDecreaseIconString = ":/images/Images/horizontalDecrease-2.png";
extern const QString glHorizontalIncreaseIconString = ":/images/Images/horizontalIncrease-2.png";
extern const QString glVerticalDecreaseIconString = ":/images/Images/verticalDecrease-2.png";
extern const QString glVerticalIncreaseIconString = ":/images/Images/verticalIncrease-2.png";
extern const QString glFitInRectIconString = ":/images/Images/fitInSceneRect.png";
extern const QString glAxisToEnergyIconString = ":/images/Images/energy.png";
extern const QString glShowGridIconString = ":/images/Images/gridVisible-3.png";

extern const QString glSidebarLeftOpenIconString = ":/images/ZWidgets/sidebarLeftOpen-1.png";
extern const QString glSidebarLeftCloseIconString = ":/images/ZWidgets/sidebarLeftClose-1.png";
extern const QString glSidebarRightOpenIconString = ":/images/ZWidgets/sidebarRightOpen-1.png";
extern const QString glSidebarRightCloseIconString = ":/images/ZWidgets/sidebarRightClose-1.png";


extern const QString glExitAppIconString = ":/images/Images/close.png";
extern const QString glAboutIconString = ":/images/Images/info.png";
extern const QString glHelpIconString = ":/images/Images/help.png";

extern const QString glVisibleSignIconString = ":/images/Images/eyeSignBlack-1.png";

extern const QString glVisibleIconString = ":/images/Images/visible-1.png";
extern const QString glInvisibleIconString = ":/images/Images/invisible-1.png";
extern const QString glInverseVisibleIconString = ":/images/Images/inverseRed-2.png";

extern const QString glConstExcludedIconString = ":/images/Images/Unchecked-16.png";
extern const QString glConstIncludedIconString = ":/images/Images/checked-16.png";
extern const QString glExamWaitingIconString = ":/images/Images/question.png";
extern const QString glIncludedIconString = ":/images/Images/openSpeFolder-2.png";
extern const QString glExcludedIconString = ":/images/Images/openSpeFolder-2.png";
extern const QString glBaseTermIconString = ":/images/Images/B-sign-16.png";

extern const QString glOpenArrayIconString = ":/images/Images/openSpeFolder-3.png";
extern const QString glSaveArrayIconString = ":/images/Images/saveSpeArray-2.png";
extern const QString glSaveAsArrayIconString = ":/images/Images/saveAsSpeArray-2.png";

extern const QString glOpenCalibrationIconString = ":/images/Images/openCalibration-3.png";
extern const QString glSaveCalibrationIconString = ":/images/Images/saveCalibration-3.png";
extern const QString glSaveAsCalibrationIconString = ":/images/Images/saveAsCalibration-3.png";
extern const QString glDuplicateCalibrationIconString = ":/images/Images/duplicateCalibration-3.png";

extern const QString glAddArrayIconString = ":/images/Images/addSpeFolder-3.png";
extern const QString glAddSpectrumIconString = ":/images/Images/addSpectrum-10.png";
extern const QString glAddChemElementIconString = ":/images/Images/addChemElement-6.png";
extern const QString glAddCalibrationIconString = ":/images/Images/addCalibration-3.png";
extern const QString glAddWindowIconString = ":/images/Images/addWindow-2.png";
extern const QString glAddTermIconString = ":/images/Images/T-Plus1.png";

extern const QString glRemoveIconString = ":/images/Images/cancel-red-1s.png";
extern const QString glRemoveSpectrumIconString = ":/images/Images/removeSpectrum-10.png";
extern const QString glRemoveChemElementIconString = ":/images/Images/removeChemElement-6.png";
extern const QString glRemoveArrayIconString = ":/images/Images/removeSpeFolder-3.png";
extern const QString glRemoveCalibrationIconString = ":/images/Images/removeCalibration-3.png";
extern const QString glRemoveWindowIconString = ":/images/Images/removeWindow-2.png";

extern const QString glClearIconString = ":/images/Images/clear-1.png";
extern const QString glCopyIconString = ":/images/Images/copy-1.png";
extern const QString glPasteIconString = ":/images/Images/paste-1.png";
extern const QString glCalcIconString = ":/images/Images/calc.png";

extern const QString glResetIconString = ":/images/Images/reset.png";
extern const QString glAddMixedTermsIconString = ":/images/Images/addMixedTerm.png";
extern const QString glRemoveMixedTermsIconString = ":/images/Images/removeMixedTerm.png";
extern const QString glAddCustomTermIconString = ":/images/Images/addCustomTerm.png";
extern const QString glRemoveCustomTermIconString = ":/images/Images/removeCustomTerm.png";


extern const QString glArrayIconString = ":/images/Images/AGreen.png";
extern const QString glSpectrumIconString = ":/images/Images/SPurple.png";
extern const QString glCalibrationIconString = ":/images/Images/CBlue.png";
extern const QString glWindowIconString = ":/images/Images/WBrown.png";
extern const QString glElementIconString = ":/images/Images/ECyan.png";

//===============================================
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/images/Images/SDC_TA3.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("Loading codecs...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    a.processEvents();

    QTextCodec* codec = QTextCodec::codecForName("windows-1251");
    QTextCodec::setCodecForLocale(codec);


    QApplication::setOrganizationName(glAppCompany);
    QApplication::setApplicationName(glAppExeBaseName);
    QApplication::setApplicationVersion(glAppVersion);

    splash.showMessage("Loading translations...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    a.processEvents();

    QTranslator appTranslator;
    QDir appDir(QApplication::applicationDirPath());

    bool res = appTranslator.load(appDir.absoluteFilePath(QString("%1_%2").arg(glAppExeBaseName, QLocale::system().name())));
    if(!res)
    {
        res = appTranslator.load(QString(":/trans/%1_%2").arg(glAppExeBaseName, QLocale::system().name()));
    }

    if(res)
    {
        a.installTranslator(&appTranslator);
    }

    QTranslator qtTranslator;
    res = qtTranslator.load(appDir.absoluteFilePath(QString("%1_%2").arg("qtbase", QLocale::system().name())));

    if(!res)
    {
        res = qtTranslator.load(QString(":/trans/qtbase_%2").arg(QLocale::system().name()));
    }

    if(res)
    {
        a.installTranslator(&qtTranslator);
    }

    splash.showMessage("Loading styles...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    a.processEvents();

    qApp->setStyleSheet(
                "QSplitter::handle:vertical   {height: 6px; image: url(:/images/Images/vSplitterHandle.png);}"
                "QSplitter::handle:horizontal {width:  6px; image: url(:/images/Images/hSplitterHandle.png);}"
                );

    splash.showMessage("Loading modules...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    a.processEvents();

    MainWindow w;
    w.show();
    splash.finish(&w);

    return a.exec();
}
//===============================================
