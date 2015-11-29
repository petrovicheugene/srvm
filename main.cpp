//===============================================
#include "MainWindow.h"
#include "globalVariables.h"

#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QDir>
//===============================================

extern const QString glAppExeBaseName =  APP_EXE_BASE_NAME; // w/o version
extern const QString glAppProduct =  APP_PRODUCT;
extern const QString glAppVersion = APP_VERSION;
extern const QString glAppCompany = APP_COMPANY;
extern const QString glAppCopyright = APP_COPYRIGHT;
extern const QString glAppCompanyURL = APP_COMPANY_URL;

//===============================================
int main(int argc, char *argv[])
{
    QTextCodec* codec = QTextCodec::codecForName("windows-1251");
    QTextCodec::setCodecForLocale(codec);

    QApplication::setOrganizationName(glAppCompany);
    QApplication::setApplicationName(glAppExeBaseName);
    QApplication::setApplicationVersion(glAppVersion);

    QApplication a(argc, argv);

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

    MainWindow w;
    w.show();

    return a.exec();
}
//===============================================
