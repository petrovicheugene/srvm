//======================================================
#include "ZBaseMainWindow.h"
#include "ZHelpBrowser.h"

#include <QApplication>

#include <QAction>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QUrl>
//======================================================
ZBaseMainWindow::ZBaseMainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(qApp->property("glAppProduct").toString());

    zv_helpBrowser = nullptr;

    zh_createBaseActions();
    zh_createBaseConnections();
}
//======================================================
void ZBaseMainWindow::zp_handleStandardLogMessage(QtMsgType type,
                                                  const QMessageLogContext &context,
                                                  const QString &msg) const
{
    // send message to ApplicationLogger
    emit zg_standardLogMessage(type, msg);
}
//======================================================
void ZBaseMainWindow::zh_createBaseActions()
{
    zv_helpAction = new QAction(tr("Help"), this);
    zv_aboutAction = new QAction(tr("About"), this);
    zv_aboutQtAction = new QAction(tr("About Qt"), this);
}
//======================================================
void ZBaseMainWindow::zh_createBaseConnections()
{
    connect(zv_helpAction, &QAction::triggered,
            this, &ZBaseMainWindow::zh_help);
    connect(zv_aboutAction, &QAction::triggered,
            this, &ZBaseMainWindow::zh_about);
    connect(zv_aboutQtAction, &QAction::triggered,
            qApp, &QApplication::aboutQt);
}
//======================================================
bool ZBaseMainWindow::zh_openSettingsGroup(QSettings* settings)
{
    if(settings == nullptr)
    {
        return false;
    }
    // open version group
    QString verString = qApp->property("glAppVersion").toString();
    if(!verString.isEmpty())
    {
        settings->beginGroup(verString);
    }

    settings->beginGroup("AppState");

    return true;
}
//======================================================
bool ZBaseMainWindow::zh_closeSettingsGroup(QSettings* settings)
{
    if(settings == nullptr)
    {
        return false;
    }

    // close groups
    while(!settings->group().isEmpty())
    {
        settings->endGroup();
    }

    return true;
}
//======================================================
void ZBaseMainWindow::zh_help()
{
    if(zv_helpBrowser == nullptr)
    {
        QUrl source = QUrl(".htm");
        QStringList searchList;

        QDir dir = QApplication::applicationDirPath();
        dir.cd("Doc");
        searchList << dir.absolutePath(); // << ":/docs"; Streams.files
        dir.cd(QString("%1.files").arg(qApp->property("glAppExeBaseName").toString()));
        searchList << dir.absolutePath() << ":/docs";

        zv_helpBrowser = ZHelpBrowser::zp_instance(searchList, source, centralWidget());
        zv_helpBrowser->setAttribute(Qt::WA_GroupLeader);
        zv_helpBrowser->show();
    }
    else if(!zv_helpBrowser->isVisible())
    {
        zv_helpBrowser->setVisible(true);
    }
}
//======================================================
void ZBaseMainWindow::zh_about()
{
    QString title = tr("About %1").arg(qApp->property("glAppProduct").toString());
    //    QString text = tr("The application is a supplement to a SRV spectrometer software. It provides to make extra calculation of chemical concentration that cannot be directly  measured."
    //                      "");
    //    QString htmlText = QString(
    //                "<table border=0 cellspacing = 15>"
    //                "<tr>"
    //                "<td align = left><img src=:/images/ZImages/CR3_64.png></td>"
    //                "<td align = left><h2 align = center>CRecalc 2.3</h2>"
    //                "<p>Copyright &copy; TechnoAnalyt Ltd., 2014.  All rights reserved.</p>"
    //                "<p>%1</p>"
    //                "</td>"
    //                "</tr>"
    //                "</table>").arg(text);

    QString text = QString();//tr("%1 is an application that controls other console application.").arg(glAppProduct);

    QString htmlText = QString(
                "<table border=0 cellspacing = 15>"
                "<tr>"
                "<td align = left><img src=:/images/appIcon.png></td>"
                "<td align = left><h1 align = center>%1</h1>"
                "</td>"
                "</tr>"
                "</table>"
                "<p>%6 - %2</p>"
                "<p>%3</p>"
                "<p>%7<br> "
                "Company website: <a href=\"http://%8/\">%8</a><br>"
                "%5: %4.<br>"
                "Author's email: <a href=mailto:petrovich.eugene@gmail.com?Subject=My%20Subject>petrovich.eugene@gmail.com</a></p>"
                ).arg(qApp->property("glAppProduct").toString(),
                      qApp->applicationVersion(),
                      text,
                      tr("Eugene Petrovich"),
                      tr("Author"),
                      tr("Version"),
                      qApp->property("glAppCopyright").toString(),
                      qApp->property("glAppCompanyURL").toString()
                      );

    QMessageBox::about(centralWidget(), title, htmlText);

}
//======================================================
