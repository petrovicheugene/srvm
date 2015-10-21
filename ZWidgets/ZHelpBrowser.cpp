//==============================================
#include "ZHelpBrowser.h"
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QAction>
#include <QToolBar>
#include <QUrl>
#include <QSettings>
#include <glVariables.h>
//==============================================
ZHelpBrowser* ZHelpBrowser::mv_this = 0;
int ZHelpBrowser::mv_refCount = 0;

//==============================================
ZHelpBrowser::ZHelpBrowser(const QStringList& pathList, const QUrl& doc, QWidget *parent ) :
    QDialog(parent)
{
    zp_createComponents(pathList, doc);
    zp_createConnections();
    mf_dialogControl();
}
//==============================================
void ZHelpBrowser::zp_createComponents(const QStringList& pathList, const QUrl& doc)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    mv_homeAction = new QAction(this);
    mv_homeAction->setIcon(QIcon(":/images/blue_home.ico"));

    mv_backwardAction = new QAction(this);
    mv_backwardAction->setIcon(QIcon(":/images/blue_left.ico"));

    mv_forwardAction = new QAction(this);
    mv_forwardAction->setIcon(QIcon(":/images/blue_right.ico"));

    QToolBar* toolBar = new QToolBar(this);
    toolBar->setIconSize(QSize(32, 32));

    toolBar->addAction(mv_homeAction);
    toolBar->addSeparator();
    toolBar->addAction(mv_backwardAction);
    toolBar->addAction(mv_forwardAction);
    mainLayout->addWidget(toolBar);

    mv_browser = new QTextBrowser(this);
    mv_browser->setOpenExternalLinks(true);
    mv_browser->setOpenLinks(true);
    mv_browser->setSearchPaths(pathList);
    mv_browser->setSource(doc);

    mainLayout->addWidget(mv_browser);
}
//==============================================
void ZHelpBrowser::zp_createConnections()
{
    //connect(mv_browser, SIGNAL((bool)), mv_homeAction, SLOT(setEnabled(bool)));
    connect(mv_browser, SIGNAL(backwardAvailable(bool)), mv_backwardAction, SLOT(setEnabled(bool)));
    connect(mv_browser, SIGNAL(forwardAvailable(bool)), mv_forwardAction, SLOT(setEnabled(bool)));

    connect(mv_homeAction, SIGNAL(triggered()), mv_browser, SLOT(home()));
    connect(mv_backwardAction, SIGNAL(triggered()), mv_browser, SLOT(backward()));
    connect(mv_forwardAction, SIGNAL(triggered()), mv_browser, SLOT(forward()));
}
//==============================================
ZHelpBrowser::~ZHelpBrowser()
{

}
//==============================================
void ZHelpBrowser::closeEvent(QCloseEvent *e)
{
    mv_geometry = saveGeometry();
    QDialog::closeEvent(e);
}
//==============================================
void ZHelpBrowser::mf_restoreGeometry()
{
    if(!mv_geometry.isEmpty())
    {
        restoreGeometry(mv_geometry);
    }
}
//==============================================
void ZHelpBrowser::mf_dialogControl()
{
    mv_backwardAction->setEnabled(mv_browser->isBackwardAvailable());
    mv_forwardAction->setEnabled(mv_browser->isForwardAvailable());
}
//==============================================
ZHelpBrowser* ZHelpBrowser::mf_instance(const QStringList &pathList, const QUrl &doc, QWidget *parent)
{
    if(! mv_this)
    {
        mv_this = new ZHelpBrowser(pathList, doc, parent);
        QSettings settings;
        settings.beginGroup(glAppVersion);

        QVariant vData = settings.value("help geometry");

        settings.endGroup();

        if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>()  )
        {
            mv_this->restoreGeometry(vData.toByteArray());
        }
    }
    mv_refCount++;
    return  mv_this;
}
//==============================================
void ZHelpBrowser::zp_freeInstance()
{
    mv_refCount--;
    if(mv_refCount < 1)
    {
        QSettings settings;
        settings.beginGroup(glAppVersion);
        settings.setValue("help geometry", saveGeometry());
        settings.endGroup();

        delete this;
        mv_this = 0;
    }
}
//==============================================
