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

extern const QString glHorizontalDecreaseIconString = ":/images/horizontalDecrease-2.png";
extern const QString glHorizontalIncreaseIconString = ":/images/horizontalIncrease-2.png";
extern const QString glVerticalDecreaseIconString = ":/images/verticalDecrease-2.png";
extern const QString glVerticalIncreaseIconString = ":/images/verticalIncrease-2.png";
extern const QString glFitInRectIconString = ":/images/fitInSceneRect.png";
extern const QString glAxisToEnergyIconString = ":/images/energy.png";
extern const QString glShowGridIconString = ":/images/gridVisible-3.png";

extern const QString glSidebarLeftOpenIconString = ":/images/ZWidgets/sidebarLeftOpen-1.png";
extern const QString glSidebarLeftCloseIconString = ":/images/ZWidgets/sidebarLeftClose-1.png";
extern const QString glSidebarRightOpenIconString = ":/images/ZWidgets/sidebarRightOpen-1.png";
extern const QString glSidebarRightCloseIconString = ":/images/ZWidgets/sidebarRightClose-1.png";


extern const QString glExitAppIconString = ":/images/close.png";
extern const QString glAboutIconString = ":/images/info.png";
extern const QString glHelpIconString = ":/images/help.png";

extern const QString glVisibleIconString = ":/images/visible-1.png";
extern const QString glInvisibleIconString = ":/images/invisible-1.png";
extern const QString glInverseVisibleIconString = ":/images/inverseRed-2.png";

extern const QString glConstExcludedIconString = ":/images/Unchecked-16.png";
extern const QString glConstIncludedIconString = ":/images/checked-16.png";
extern const QString glExamWaitingIconString = ":/images/question.png";
extern const QString glIncludedIconString = ":/images/openSpeFolder-2.png";
extern const QString glExcludedIconString = ":/images/openSpeFolder-2.png";

extern const QString glOpenArrayIconString = ":/images/openSpeFolder-3.png";
extern const QString glSaveArrayIconString = ":/images/saveSpeArray-2.png";
extern const QString glSaveAsArrayIconString = ":/images/saveAsSpeArray-2.png";

extern const QString glOpenCalibrationIconString = ":/images/openCalibration-3.png";
extern const QString glSaveCalibrationIconString = ":/images/saveCalibration-3.png";
extern const QString glSaveAsCalibrationIconString = ":/images/saveAsCalibration-3.png";
extern const QString glDuplicateCalibrationIconString = ":/images/duplicateCalibration-3.png";

extern const QString glAddArrayIconString = ":/images/addSpeFolder-3.png";
extern const QString glAddSpectrumIconString = ":/images/addSpectrum-10.png";
extern const QString glAddChemElementIconString = ":/images/addChemElement-6.png";
extern const QString glAddCalibrationIconString = ":/images/addCalibration-3.png";
extern const QString glAddWindowIconString = ":/images/addWindow-2.png";
extern const QString glAddTermIconString = ":/images/T-Plus1.png";

extern const QString glRemoveIconString = ":/images/cancel-red-1s.png";
extern const QString glRemoveSpectrumIconString = ":/images/removeSpectrum-10.png";
extern const QString glRemoveChemElementIconString = ":/images/removeChemElement-6.png";
extern const QString glRemoveArrayIconString = ":/images/removeSpeFolder-3.png";
extern const QString glRemoveCalibrationIconString = ":/images/removeCalibration-3.png";
extern const QString glRemoveWindowIconString = ":/images/removeWindow-2.png";

extern const QString glClearIconString = ":/images/clear-1.png";
extern const QString glCopyIconString = ":/images/copy-1.png";
extern const QString glPasteIconString = ":/images/paste-1.png";
extern const QString glCalcIconString = ":/images/calc.png";

extern const QString glResetIconString = ":/images/reset.png";
extern const QString glAddMixedTermsIconString = ":/images/addMixedTerm.png";
extern const QString glRemoveMixedTermsIconString = ":/images/removeMixedTerm.png";
extern const QString glAddCustomTermIconString = ":/images/addCustomTerm.png";
extern const QString glRemoveCustomTermIconString = ":/images/removeCustomTerm.png";


extern const QString glArrayIconString = ":/images/AGreen.png";
extern const QString glSpectrumIconString = ":/images/SPurple.png";
extern const QString glCalibrationIconString = ":/images/CBlue.png";
extern const QString glWindowIconString = ":/images/WBrown.png";
extern const QString glElementIconString = ":/images/ECyan.png";

//===============================================
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/images/SDC_TA3.png");
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
                "QSplitter::handle:vertical   {height: 6px; image: url(:/images/vSplitterHandle.png);}"
                "QSplitter::handle:horizontal {width:  6px; image: url(:/images/hSplitterHandle.png);}"
                );

    splash.showMessage("Loading modules...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    a.processEvents();

    MainWindow w;
    w.show();
    splash.finish(&w);

    return a.exec();
}
//===============================================
