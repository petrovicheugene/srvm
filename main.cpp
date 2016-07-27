//===============================================
#include "MainWindow.h"
#include "ZConstants.h"

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

extern const QString glHorizontalDecreaseIconString = ":/images/ZImages/horizontalDecrease-2.png";
extern const QString glHorizontalIncreaseIconString = ":/images/ZImages/horizontalIncrease-2.png";
extern const QString glVerticalDecreaseIconString = ":/images/ZImages/verticalDecrease-2.png";
extern const QString glVerticalIncreaseIconString = ":/images/ZImages/verticalIncrease-2.png";
extern const QString glFitInRectIconString = ":/images/ZImages/fitInSceneRect.png";
extern const QString glAxisToEnergyIconString = ":/images/ZImages/energy.png";
extern const QString glShowGridIconString = ":/images/ZImages/gridVisible-3.png";

extern const QString glSidebarLeftOpenIconString = ":/images/ZWidgets/sidebarLeftOpen-1.png";
extern const QString glSidebarLeftCloseIconString = ":/images/ZWidgets/sidebarLeftClose-1.png";
extern const QString glSidebarRightOpenIconString = ":/images/ZWidgets/sidebarRightOpen-1.png";
extern const QString glSidebarRightCloseIconString = ":/images/ZWidgets/sidebarRightClose-1.png";


extern const QString glExitAppIconString = ":/images/ZImages/close.png";
extern const QString glAboutIconString = ":/images/ZImages/info.png";
extern const QString glHelpIconString = ":/images/ZImages/help.png";

extern const QString glVisibleSignIconString = ":/images/ZImages/eyeSignBlack-1.png";

extern const QString glVisibleIconString = ":/images/ZImages/visible-1.png";
extern const QString glInvisibleIconString = ":/images/ZImages/invisible-1.png";
extern const QString glInverseVisibleIconString = ":/images/ZImages/inverseRed-2.png";

extern const QString glConstExcludedIconString = ":/images/ZImages/Unchecked-16.png";
extern const QString glConstIncludedIconString = ":/images/ZImages/checked-16.png";
extern const QString glExamWaitingIconString = ":/images/ZImages/question.png";
extern const QString glIncludedIconString = ":/images/ZImages/openSpeFolder-2.png";
extern const QString glExcludedIconString = ":/images/ZImages/openSpeFolder-2.png";
extern const QString glBaseTermIconString = ":/images/ZImages/B-sign-16.png";

extern const QString glOpenArrayIconString = ":/images/ZImages/openSpeFolder-3.png";
extern const QString glSaveArrayIconString = ":/images/ZImages/saveSpeArray-2.png";
extern const QString glSaveAsArrayIconString = ":/images/ZImages/saveAsSpeArray-2.png";

extern const QString glOpenCalibrationIconString = ":/images/ZImages/openCalibration-3.png";
extern const QString glSaveCalibrationIconString = ":/images/ZImages/saveCalibration-3.png";
extern const QString glSaveAsCalibrationIconString = ":/images/ZImages/saveAsCalibration-3.png";
extern const QString glDuplicateCalibrationIconString = ":/images/ZImages/duplicateCalibration-3.png";

extern const QString glAddArrayIconString = ":/images/ZImages/addSpeFolder-3.png";
extern const QString glAddSpectrumIconString = ":/images/ZImages/addSpectrum-10.png";
extern const QString glAddChemElementIconString = ":/images/ZImages/addChemElement-6.png";
extern const QString glAddCalibrationIconString = ":/images/ZImages/addCalibration-3.png";
extern const QString glAddWindowIconString = ":/images/ZImages/addWindow-2.png";
extern const QString glAddTermIconString = ":/images/ZImages/T-Plus1.png";

extern const QString glRemoveIconString = ":/images/ZImages/cancel-red-1s.png";
extern const QString glRemoveSpectrumIconString = ":/images/ZImages/removeSpectrum-10.png";
extern const QString glRemoveChemElementIconString = ":/images/ZImages/removeChemElement-6.png";
extern const QString glRemoveArrayIconString = ":/images/ZImages/removeSpeFolder-3.png";
extern const QString glRemoveCalibrationIconString = ":/images/ZImages/removeCalibration-3.png";
extern const QString glRemoveWindowIconString = ":/images/ZImages/removeWindow-2.png";

extern const QString glClearIconString = ":/images/ZImages/clear-1.png";
extern const QString glCopyIconString = ":/images/ZImages/copy-1.png";
extern const QString glPasteIconString = ":/images/ZImages/paste-1.png";
extern const QString glCalcIconString = ":/images/ZImages/calc.png";

extern const QString glResetIconString = ":/images/ZImages/reset.png";
extern const QString glAddMixedTermsIconString = ":/images/ZImages/addMixedTerm.png";
extern const QString glRemoveMixedTermsIconString = ":/images/ZImages/removeMixedTerm.png";
extern const QString glAddCustomTermIconString = ":/images/ZImages/addCustomTerm.png";
extern const QString glRemoveCustomTermIconString = ":/images/ZImages/removeCustomTerm.png";


extern const QString glArrayIconString = ":/images/ZImages/AGreen.png";
extern const QString glSpectrumIconString = ":/images/ZImages/SPurple.png";
extern const QString glCalibrationIconString = ":/images/ZImages/CBlue.png";
extern const QString glWindowIconString = ":/images/ZImages/WBrown.png";
extern const QString glElementIconString = ":/images/ZImages/ECyan.png";

//===============================================
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/images/ZImages/SDC_TA3.png");
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
                "QSplitter::handle:vertical   {height: 6px; image: url(:/images/ZImages/vSplitterHandle.png);}"
                "QSplitter::handle:horizontal {width:  6px; image: url(:/images/ZImages/hSplitterHandle.png);}"
                );

    splash.showMessage("Loading modules...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    a.processEvents();

    MainWindow w;
    w.show();
    splash.finish(&w);

    return a.exec();
}
//===============================================
