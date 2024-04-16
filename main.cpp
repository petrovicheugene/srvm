//======================================================
#include "MainWindow.h"
#include "X_TranslatorManager.h"

// #include <windows.h>
#include <QApplication>
#include <QDir>
#include <iostream>
#include <QOperatingSystemVersion>
#include <QPixmap>
#include <QPointer>
#include <QSet>
#include <QStyleFactory>
#include <QSplashScreen>
#include <QTranslator>
#include <QTextCodec>

//#include <QException>
//======================================================
/*!
\brief  This module contains a function "messageHandler" for receiving standard
 messages qDebug, qInfo, qWarning, qCritical and qFatal.

 Further these messages are redirected to MainWindow that should have a receive function
 "xp_handleStandardLogMessage(QtMsgType type,
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
//namespace
//{

// main window pointer
static QPointer<MainWindow> pMainWindow = nullptr;
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

//}
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
    if(pMainWindow && (msgTypesToHandleInMainWindowSet.contains(type)))
    {
        // let the application handles message (for example for saving in log)
        try
        {
            pMainWindow->xp_handleStandardLogMessage(type, context, msg);
        }
        catch (...)
        {

        }
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
    //    QPixmap pixmap(":/images/X_Images/SDC_TA3.png");
    //    QSplashScreen splash(pixmap);
    //    splash.show();
    //    splash.showMessage("Loading codecs...", Qt::AlignBottom | Qt::AlignRight, Qt::white );
    //    a.processEvents();

    QTextCodec* codec = QTextCodec::codecForName("windows-1251");
    QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);
    qApp->setStyle(QStyleFactory::create("windowsvista"));
    //
    initMsgTypesToHandleInMainWindow();
    // custom message handler for logging via qInfo qWarning qCritical
    qInstallMessageHandler(messageHandler);

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

    X_TranslatorManager languageManager;
    languageManager.xp_installTranslatorsToApplication();

    // set dots on the splitter handle
    qApp->setStyleSheet(
                "QSplitter::handle:vertical {height: 4px; image: url(:/images/X_Images/vSplitterHandler.png);}"
                "QSplitter::handle:horizontal {width: 4px; image: url(:/images/X_Images/hSplitterHandler.png);}"
                );

    // horizontal lines on table header views on win10
//    if (QSysInfo::productVersion() == "10")
//    {
//        qApp->setStyleSheet("QHeaderView::section{"
//                            "border-top:0px solid #D8D8D8;"
//                            "border-left:0px solid #D8D8D8;"
//                            "border-right:1px solid #D8D8D8;"
//                            "border-bottom: 1px solid #D8D8D8;"
//                            "background-color:white;"
//                            "padding:4px;"
//                            "}"
//                            "QTableCornerButton::section{"
//                            "border-top:0px solid #D8D8D8;"
//                            "border-left:0px solid #D8D8D8;"
//                            "border-right:1px solid #D8D8D8;"
//                            "border-bottom: 1px solid #D8D8D8;"
//                            "background-color:white;"
//                            "}");
//    }

    QOperatingSystemVersion currentOS = QOperatingSystemVersion::current();
    if (currentOS >= QOperatingSystemVersion(QOperatingSystemVersion::Windows10))
    {
        qApp->setStyleSheet("QHeaderView::section {"
                            "border-top:0px solid #D8D8D8;"
                            "border-left:0px solid #D8D8D8;"
                            "border-right:1px solid #D8D8D8;"
                            "border-bottom: 1px solid #D8D8D8;"
                            "background-color:white;"
                            "padding:4px;"
                            "}"
                            "QTableCornerButton::section {"
                            "border-top:0px solid #D8D8D8;"
                            "border-left:0px solid #D8D8D8;"
                            "border-right:1px solid #D8D8D8;"
                            "border-bottom: 1px solid #D8D8D8;"
                            "background-color:white;"
                            "}"
                            "QPushButton {font : bold;}"
                            "QHeaderView {font : bold;}");
    }
    else
    {
        qApp->setStyleSheet("QPushButton {font : bold;}"
                            "QHeaderView {font : bold;}");
    }

    // set path for sqlite plugin
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("platforms");
    //paths.append("sqldrivers");
    QCoreApplication::setLibraryPaths(paths);


    // launch app

    //  pixmap = QPixmap(":/images/X_Images/Chemistry.png");
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
