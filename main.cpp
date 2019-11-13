//===============================================
#include "mainwindow.h"
#include "ZGeneral.h"
#include "ZTranslatorManager.h"

#include <windows.h>
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QDir>
#include <QSplashScreen>
#include <QPixmap>
#include <iostream>
#include <QSet>
//===============================================
//======================================================
/*!
\brief  This module contains a function "messageHandler" for receiving standard
 messages qDebug, qInfo, qWarning, qCritical and qFatal.

 Further these messages are redirected to MainWindow that should have a receive function
 "zp_handleStandardLogMessage(QtMsgType type,
                         const QMessageLogContext &context,
                         const QString &msg)".
 At the same time the messages are outputing in standard output stream
 std and are displayed in Qt Creator.

 In class MainWindow the messages may be handled as log messages.

 In main function qApp object gets new properties that hold the defines
 created in .pro file. These defines becomes accessable from every point
 of the code.

*/
//======================================================
//extern const QString glAppExeBaseName =  APP_EXE_BASE_NAME; // w/o version
//extern const QString glAppProduct =  APP_PRODUCT;
//extern const QString qApp->applicationVersion() = APP_VERSION;
//extern const QString glAppCompany = APP_COMPANY;
//extern const QString glAppCopyright = APP_COPYRIGHT;
//extern const QString glAppCompanyURL = APP_COMPANY_URL;
//======================================================
namespace
{
// main window pointer
static MainWindow* pMainWindow = nullptr;
static QSet<QtMsgType> msgTypesToHandleInMainWindowSet;

// settings of format of debug output mssages to stderr
enum DebugOutputOption {
    DO_MsgOnly = 0x0,
    DO_ShowFile = 0x1,
    DO_ShowFunction = 0x2,
    DO_ShowLine = 0x4,
    DO_ShowVersion = 0x8};

Q_DECLARE_FLAGS(DebugOutputOptions, DebugOutputOption)
Q_DECLARE_OPERATORS_FOR_FLAGS(DebugOutputOptions)

static DebugOutputOptions debugOutputOption =
        DO_MsgOnly
        | DO_ShowFile
        | DO_ShowFunction
        | DO_ShowLine
        // | DO_ShowVersion
        ;

}
//======================================================
void initMsgTypesToHandleInMainWindow()
{
#ifndef QT_NO_DEBUG_OUTPUT
    msgTypesToHandleInMainWindowSet.insert(QtDebugMsg);
#endif
    msgTypesToHandleInMainWindowSet.insert(QtInfoMsg);
    msgTypesToHandleInMainWindowSet.insert(QtWarningMsg);
    msgTypesToHandleInMainWindowSet.insert(QtCriticalMsg);
    msgTypesToHandleInMainWindowSet.insert(QtFatalMsg);
}
//======================================================
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(pMainWindow != nullptr && (msgTypesToHandleInMainWindowSet.contains(type)))
    {
        // let the application handles message (for example for saving in log)
        pMainWindow->zp_handleStandardLogMessage(type, context, msg);
    }

    // Standard output
    QString outputMsg = msg;
    // it msg type is debug add context to message
    if(type == QtDebugMsg)
    {
        QString contextString;
        if(debugOutputOption & DO_ShowFile)
        {
            contextString += QString(context.file);
        }
        if(debugOutputOption & DO_ShowFunction)
        {
            if(!contextString.isEmpty())
            {
                contextString += "; ";
            }
            contextString += QString(context.function);
        }
        if(debugOutputOption & DO_ShowLine)
        {
            if(!contextString.isEmpty())
            {
                contextString += "; ";
            }
            contextString += "line:" + QString::number(context.line);
        }
        if(debugOutputOption & DO_ShowVersion)
        {
            if(!contextString.isEmpty())
            {
                contextString += "; ";
            }
            contextString +=  "ver:" + QString::number(context.version);
        }

        if(!contextString.isEmpty())
        {
            outputMsg += " | " + contextString;
        }
    }

    std::cerr <<  outputMsg.toStdString() <<  std::endl;

    if(type == QtFatalMsg)
    {
        abort();
    }
}
//======================================================
int main(int argc, char *argv[])
{
    //    QApplication a(argc, argv);
    //    QPixmap pixmap(":/images/ZImages/SDC_TA3.png");
    //    QSplashScreen splash(pixmap);
    //    splash.show();
    //    splash.showMessage("Loading codecs...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    //    a.processEvents();

    QTextCodec* codec = QTextCodec::codecForName("windows-1251");
    QTextCodec::setCodecForLocale(codec);

    //    QApplication::setOrganizationName(glAppCompany);
    //    QApplication::setApplicationName(glAppExeBaseName);
    //    QApplication::setApplicationVersion(qApp->applicationVersion());

    //    splash.showMessage("Loading translations...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    //    a.processEvents();

    //    QTranslator appTranslator;
    //    QDir appDir(QApplication::applicationDirPath());

    //    bool res = appTranslator.load(appDir.absoluteFilePath(QString("%1_%2").arg(glAppExeBaseName, QLocale::system().name())));
    //    if(!res)
    //    {
    //        res = appTranslator.load(QString(":/trans/%1_%2").arg(glAppExeBaseName, QLocale::system().name()));
    //    }

    //    if(res)
    //    {
    //        a.installTranslator(&appTranslator);
    //    }

    //    QTranslator qtTranslator;
    //    res = qtTranslator.load(appDir.absoluteFilePath(QString("%1_%2").arg("qtbase", QLocale::system().name())));

    //    if(!res)
    //    {
    //        res = qtTranslator.load(QString(":/trans/qtbase_%2").arg(QLocale::system().name()));
    //    }

    //    if(res)
    //    {
    //        a.installTranslator(&qtTranslator);
    //    }

    //    splash.showMessage("Loading styles...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    //    a.processEvents();

    //    splash.showMessage("Loading modules...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    //    a.processEvents();

    //    MainWindow w;
    //    w.show();
    //    splash.finish(&w);

    //    return a.exec();
    QApplication a(argc, argv);
    //
    initMsgTypesToHandleInMainWindow();
    // custom message handler for logging via qInfo qWarning qCritical
    qInstallMessageHandler(messageHandler);

    // set translators
    // translator attempts to load from expernal file first and if not from resources
    //    QDir appDir(QApplication::applicationDirPath());
    //    QTranslator appTranslator;
    //    if(appTranslator.load(appDir.absoluteFilePath(QString("%1_%2").arg(APP_EXE_BASE_NAME, QLocale::system().name())))
    //            || appTranslator.load(QString(":/translators/%1_%2").arg(APP_EXE_BASE_NAME, QLocale::system().name())))
    //    {
    //        a.installTranslator(&appTranslator);
    //    }

    //    QTranslator qtTranslator;
    //    if(qtTranslator.load(appDir.absoluteFilePath(QString("%1_%2").arg("qtbase", QLocale::system().name())))
    //            || qtTranslator.load(QString(":/translators/qtbase_%2").arg(QLocale::system().name())))
    //    {
    //        a.installTranslator(&qtTranslator);
    //    }

    //    // set application properties
    //    QApplication::setOrganizationName(APP_COMPANY);
    //    QApplication::setApplicationName(APP_EXE_BASE_NAME);
    //    QApplication::setApplicationVersion(APP_VERSION);

    // create qApp properties and set .pro defines into them
    //#ifdef APP_EXE_BASE_NAME
    //    qApp->setProperty("glAppExeBaseName", QString(APP_EXE_BASE_NAME));
    //#endif
    //#ifdef APP_PRODUCT
    //    qApp->setProperty("glAppProduct", QString(APP_PRODUCT));
    //#endif
    //#ifdef APP_VERSION
    //    qApp->setProperty("qApp->applicationVersion()", QString(APP_VERSION));
    //#endif
    //#ifdef APP_COMPANY
    //    qApp->setProperty("glAppCompany", QString(APP_COMPANY));
    //#endif
    //#ifdef APP_COPYRIGHT
    //    qApp->setProperty("glAppCopyright", QString(APP_COPYRIGHT));
    //#endif
    //#ifdef APP_COMPANY_URL
    //    qApp->setProperty("glAppCompanyURL", QString(APP_COMPANY_URL));
    //#endif
    //#ifdef APP_ICON
    //    qApp->setProperty("glAppIcon", QString(APP_ICON));
    //#endif

    // create qApp properties and set .pro defines into them
#ifdef APP_DISPLAY_NAME
    QApplication::setApplicationDisplayName(APP_DISPLAY_NAME);
#endif

#ifdef APP_PRODUCT
    QApplication::setApplicationName(APP_PRODUCT);
#endif

#ifdef APP_VERSION
    QApplication::setApplicationVersion(APP_VERSION);
#endif

#ifdef APP_COMPANY
    QApplication::setOrganizationName(APP_COMPANY);
#endif

#ifdef APP_COMPANY_URL
    QApplication::setOrganizationDomain(APP_COMPANY_URL);
#endif

#ifdef APP_COPYRIGHT
    qApp->setProperty("appCopyright", QString(APP_COPYRIGHT));
#endif

#ifdef APP_ICON
    qApp->setProperty("appIcon", QString(APP_ICON));
#endif

    ZTranslatorManager languageManager;
    languageManager.zp_installTranslatorsToApplication();


    // set dots on the splitter handle
    qApp->setStyleSheet(
                "QSplitter::handle:vertical {height: 4px; image: url(:/images/ZImages/vSplitterHandler.png);}"
                "QSplitter::handle:horizontal {width: 4px; image: url(:/images/ZImages/hSplitterHandler.png);}"
                );


    // launch app

    //  pixmap = QPixmap(":/images/ZImages/Chemistry.png");
    //  QSplashScreen dbSplash(pixmap);
    //  dbSplash.show();
    //  dbSplash.showMessage("Connecting to database...", Qt::AlignBottom | Qt::AlignRight, Qt::white );

    // main window
    MainWindow w;

    // pointer to main window for message handler
    pMainWindow = &w;
    w.show();
    return a.exec();
}
//===============================================
