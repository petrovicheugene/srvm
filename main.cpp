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

extern const QString glOpenIconString = ":/images/addGreen.png";
extern const QString glAddArrayIconString = ":/images/A-Plus1.png";
extern const QString glAddSpectrumIconString = ":/images/addSpectrum-8.png";
extern const QString glAddChemElementIconString = ":/images/addChemElement-5.png";
extern const QString glAddCalibrationIconString = ":/images/C-Plus1.png";
extern const QString glAddWindowIconString = ":/images/W-Plus1.png";
extern const QString glAddTermIconString = ":/images/T-Plus1.png";

extern const QString glRemoveIconString = ":/images/cancel-red-1s.png";
extern const QString glRemoveSpectrumIconString = ":/images/removeSpectrum-8.png";
extern const QString glRemoveChemElementIconString = ":/images/removeChemElement-5.png";
extern const QString glRemoveArrayIconString = ":/images/removeChemElement-4.png";
extern const QString glRemoveCalibrationIconString = ":/images/removeChemElement-4.png";
extern const QString glRemoveWindowIconString = ":/images/trashBasket.png";


extern const QString glClearIconString = ":/images/eraser.png";
extern const QString glCopyIconString = ":/images/addSpectrum-7.png";
extern const QString glPasteIconString = ":/images/addSpectrum-5.png";
extern const QString glCalcIconString = ":/images/calc.png";

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
    splash.showMessage("Loading codecs...");
    a.processEvents();

    QTextCodec* codec = QTextCodec::codecForName("windows-1251");
    QTextCodec::setCodecForLocale(codec);


    QApplication::setOrganizationName(glAppCompany);
    QApplication::setApplicationName(glAppExeBaseName);
    QApplication::setApplicationVersion(glAppVersion);

    splash.showMessage("Loading translations...");
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

    splash.showMessage("Loading styles...");
    a.processEvents();

    qApp->setStyleSheet(
                "QSplitter::handle:vertical   {height: 6px; image: url(:/images/vSplitterHandle.png);}"
                "QSplitter::handle:horizontal {width:  6px; image: url(:/images/hSplitterHandle.png);}"
                );

    splash.showMessage("Loading modules...");
    a.processEvents();

    MainWindow w;
    w.show();
    splash.finish(&w);

    return a.exec();
}
//===============================================
