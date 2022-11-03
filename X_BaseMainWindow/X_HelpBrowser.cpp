//==============================================
#include "X_HelpBrowser.h"

#include <QAction>
#include <QApplication>
#include <QSettings>
#include <QTextBrowser>
#include <QToolBar>
#include <QUrl>
#include <QVBoxLayout>
//==============================================
X_HelpBrowser* X_HelpBrowser::xv_this = 0;
int X_HelpBrowser::xv_refCount = 0;
//==============================================
X_HelpBrowser::X_HelpBrowser(QWidget *parent ) :
    QDialog(parent)
{
    QString winTitle = QString("%1 - %2").arg(qApp->applicationDisplayName(),
                                              tr("Help"));
    setWindowTitle(winTitle);

    xh_createComponents();
    xh_createConnections();
    xp_setButtonIcon(BIC_BLUE);
    xh_dialogControl();
    xh_restoreSettings();
}
//==============================================
X_HelpBrowser::~X_HelpBrowser()
{
    xh_saveSettings();
}
//==============================================
void X_HelpBrowser::xh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    xv_homeAction = new QAction(this);
    xv_backwardAction = new QAction(this);
    xv_forwardAction = new QAction(this);

    QToolBar* toolBar = new QToolBar(this);
    toolBar->setIconSize(QSize(32, 32));

    toolBar->addAction(xv_homeAction);
    toolBar->addSeparator();
    toolBar->addAction(xv_backwardAction);
    toolBar->addAction(xv_forwardAction);
    mainLayout->addWidget(toolBar);

    xv_browser = new QTextBrowser(this);
    xv_browser->setOpenExternalLinks(true);
    xv_browser->setOpenLinks(true);

    mainLayout->addWidget(xv_browser);
}
//==============================================
void X_HelpBrowser::xp_loadContentSourceUrl( const QUrl& doc)
{
    xv_browser->setSource(doc);
}
//==============================================
void X_HelpBrowser::xp_loadSearchPathList(const QStringList& pathList)
{
    xv_browser->setSearchPaths(pathList);
}
//==============================================
void X_HelpBrowser::xp_setButtonIcon(X_HelpBrowser::ButtonIconColor buttonIconColor)
{
    QIcon homeIcon;
    QIcon backwardIcon;
    QIcon forwardIcon;

    switch(buttonIconColor)
    {
    case BIC_GREEN:
        homeIcon = QIcon(":/images/green_home.ico");
        backwardIcon = QIcon(":/images/green_left.ico");
        forwardIcon = QIcon(":/images/green_right.ico");
        break;
    case BIC_BLUE:
        homeIcon = QIcon(":/images/blue_home.ico");
        backwardIcon = QIcon(":/images/blue_left.ico");
        forwardIcon = QIcon(":/images/blue_right.ico");
        break;
    }

    xv_homeAction->setIcon(homeIcon);
    xv_backwardAction->setIcon(backwardIcon);
    xv_forwardAction->setIcon(forwardIcon);
}
//==============================================
void X_HelpBrowser::xh_createConnections()
{
    //connect(mv_browser, SIGNAL((bool)), mv_homeAction, SLOT(setEnabled(bool)));
    connect(xv_browser, SIGNAL(backwardAvailable(bool)), xv_backwardAction, SLOT(setEnabled(bool)));
    connect(xv_browser, SIGNAL(forwardAvailable(bool)), xv_forwardAction, SLOT(setEnabled(bool)));

    connect(xv_homeAction, SIGNAL(triggered()), xv_browser, SLOT(home()));
    connect(xv_backwardAction, SIGNAL(triggered()), xv_browser, SLOT(backward()));
    connect(xv_forwardAction, SIGNAL(triggered()), xv_browser, SLOT(forward()));
}
//==============================================
void X_HelpBrowser::xh_dialogControl()
{
    xv_backwardAction->setEnabled(xv_browser->isBackwardAvailable());
    xv_forwardAction->setEnabled(xv_browser->isForwardAvailable());
}
//==============================================
bool X_HelpBrowser::xh_openSettingsGroup(QSettings* settings) const
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

    // open dialog group
    settings->beginGroup("helpBrowser");
    return true;
}
//==============================================
bool X_HelpBrowser::xh_closeSettingsGroup(QSettings* settings) const
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
//==============================================
void X_HelpBrowser::xh_restoreSettings()
{
    QSettings settings;
    if(!xh_openSettingsGroup(&settings))
    {
        return;
    }

    QVariant vData = settings.value(xv_geometryValueName);
    if(vData.isValid() && vData.canConvert<QByteArray>())
    {
        restoreGeometry(vData.toByteArray());
    }

    xh_closeSettingsGroup(&settings);
}
//==============================================
void X_HelpBrowser::xh_saveSettings() const
{
    QSettings settings;
    if(!xh_openSettingsGroup(&settings))
    {
        return;
    }

    settings.setValue(xv_geometryValueName, saveGeometry());

    xh_closeSettingsGroup(&settings);
}
//==============================================
X_HelpBrowser* X_HelpBrowser::xp_instance(QWidget *parent)
{
    if(! xv_this)
    {
        xv_this = new X_HelpBrowser(parent);
    }
    xv_refCount++;
    return  xv_this;
}
//==============================================
X_HelpBrowser* X_HelpBrowser::xp_instance(const QStringList& pathList,
                                               const QUrl& doc,
                                               QWidget *parent)
{
    xp_instance(parent);
    if(xv_this != nullptr)
    {
        xv_this->xp_loadSearchPathList(pathList);
        xv_this->xp_loadContentSourceUrl(doc);
    }
    return xv_this;
}
//==============================================
void X_HelpBrowser::xp_freeInstance()
{
    xv_refCount--;
    if(xv_refCount < 1)
    {
        QSettings settings;
        settings.beginGroup(qApp->applicationVersion());
        settings.setValue("help geometry", saveGeometry());
        settings.endGroup();

        delete this;
        xv_this = 0;
    }
}
//==============================================
