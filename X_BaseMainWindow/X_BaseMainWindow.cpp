//======================================================
#include "X_BaseMainWindow.h"
#include "X_HelpBrowser.h"

#include <QApplication>

#include <QAction>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QUrl>
//======================================================
X_BaseMainWindow::X_BaseMainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(qApp->applicationDisplayName());

    xv_helpBrowser = nullptr;

    xh_createBaseActions();
    xh_createBaseConnections();
}
//======================================================
void X_BaseMainWindow::xp_handleStandardLogMessage(QtMsgType type,
                                                  const QMessageLogContext &context,
                                                  const QString &msg) const
{
    // send message to ApplicationLogger
    emit xg_standardLogMessage(type, msg);
}
//======================================================
void X_BaseMainWindow::xh_createBaseActions()
{
    xv_helpAction = new QAction(tr("Help"), this);
    xv_aboutAction = new QAction(tr("About"), this);
    xv_aboutQtAction = new QAction(tr("About Qt"), this);
}
//======================================================
void X_BaseMainWindow::xh_createBaseConnections()
{
    connect(xv_helpAction, &QAction::triggered,
            this, &X_BaseMainWindow::xh_help);
    connect(xv_aboutAction, &QAction::triggered,
            this, &X_BaseMainWindow::xh_about);
    connect(xv_aboutQtAction, &QAction::triggered,
            qApp, &QApplication::aboutQt);
}
//======================================================
bool X_BaseMainWindow::xh_openSettingsGroup(QSettings* settings)
{
    if(settings == nullptr)
    {
        return false;
    }
    // open version group
    if(!qApp->applicationVersion().isEmpty())
    {
        settings->beginGroup(qApp->applicationVersion());
    }

    settings->beginGroup("AppState");

    return true;
}
//======================================================
bool X_BaseMainWindow::xh_closeSettingsGroup(QSettings* settings)
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
void X_BaseMainWindow::xh_help()
{
    if(xv_helpBrowser == nullptr)
    {
        QUrl source = QUrl(".htm");
        QStringList searchList;

        QDir dir = QApplication::applicationDirPath();
        dir.cd("Doc");
        searchList << dir.absolutePath(); // << ":/docs"; Streams.files
        dir.cd(QString("%1.files").arg(qApp->property("glAppExeBaseName").toString()));
        searchList << dir.absolutePath() << ":/docs";

        xv_helpBrowser = X_HelpBrowser::xp_instance(searchList, source, centralWidget());
        // xv_helpBrowser->setAttribute(Qt::WA_GroupLeader);
        xv_helpBrowser->show();
    }
    else if(!xv_helpBrowser->isVisible())
    {
        xv_helpBrowser->setVisible(true);
    }
}
//======================================================
void X_BaseMainWindow::xh_about()
{
    QString title = tr("About %1").arg(qApp->property("glAppProduct").toString());
    //    QString text = tr("The application is a supplement to a SRV spectrometer software. It provides to make extra calculation of chemical concentration that cannot be directly  measured."
    //                      "");
    //    QString htmlText = QString(
    //                "<table border=0 cellspacing = 15>"
    //                "<tr>"
    //                "<td align = left><img src=:/images/X_Images/CR3_64.png></td>"
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
                "%5: %4<br>"
                "Author's email: <a href=mailto:petrovich.eugene@gmail.com?Subject=My%20Subject>petrovich.eugene@gmail.com</a></p>"
                ).arg(qApp->property("glAppProduct").toString(),
                      qApp->applicationVersion(),
                      text,
                      tr("Eugene Petrovich"),
                      tr("Author"),
                      tr("Version"),
                      qApp->property("appCopyright").toString(),
                      qApp->organizationDomain()
                      );

    QMessageBox::about(centralWidget(), title, htmlText);

}
//======================================================
