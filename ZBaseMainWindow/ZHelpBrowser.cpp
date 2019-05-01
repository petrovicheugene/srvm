//==============================================
#include "ZHelpBrowser.h"

#include <QAction>
#include <QApplication>
#include <QSettings>
#include <QTextBrowser>
#include <QToolBar>
#include <QUrl>
#include <QVBoxLayout>
//==============================================
ZHelpBrowser* ZHelpBrowser::zv_this = 0;
int ZHelpBrowser::zv_refCount = 0;
//==============================================
ZHelpBrowser::ZHelpBrowser(QWidget *parent ) :
    QDialog(parent)
{
    QString winTitle = QString("%1 - %2").arg(qApp->applicationDisplayName(),
                                              tr("Help"));
    setWindowTitle(winTitle);

    zh_createComponents();
    zh_createConnections();
    zp_setButtonIcon(BIC_BLUE);
    zh_dialogControl();
    zh_restoreSettings();
}
//==============================================
ZHelpBrowser::~ZHelpBrowser()
{
    zh_saveSettings();
}
//==============================================
void ZHelpBrowser::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    zv_homeAction = new QAction(this);
    zv_backwardAction = new QAction(this);
    zv_forwardAction = new QAction(this);

    QToolBar* toolBar = new QToolBar(this);
    toolBar->setIconSize(QSize(32, 32));

    toolBar->addAction(zv_homeAction);
    toolBar->addSeparator();
    toolBar->addAction(zv_backwardAction);
    toolBar->addAction(zv_forwardAction);
    mainLayout->addWidget(toolBar);

    zv_browser = new QTextBrowser(this);
    zv_browser->setOpenExternalLinks(true);
    zv_browser->setOpenLinks(true);

    mainLayout->addWidget(zv_browser);
}
//==============================================
void ZHelpBrowser::zp_loadContentSourceUrl( const QUrl& doc)
{
    zv_browser->setSource(doc);
}
//==============================================
void ZHelpBrowser::zp_loadSearchPathList(const QStringList& pathList)
{
    zv_browser->setSearchPaths(pathList);
}
//==============================================
void ZHelpBrowser::zp_setButtonIcon(ZHelpBrowser::ButtonIconColor buttonIconColor)
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

    zv_homeAction->setIcon(homeIcon);
    zv_backwardAction->setIcon(backwardIcon);
    zv_forwardAction->setIcon(forwardIcon);
}
//==============================================
void ZHelpBrowser::zh_createConnections()
{
    //connect(mv_browser, SIGNAL((bool)), mv_homeAction, SLOT(setEnabled(bool)));
    connect(zv_browser, SIGNAL(backwardAvailable(bool)), zv_backwardAction, SLOT(setEnabled(bool)));
    connect(zv_browser, SIGNAL(forwardAvailable(bool)), zv_forwardAction, SLOT(setEnabled(bool)));

    connect(zv_homeAction, SIGNAL(triggered()), zv_browser, SLOT(home()));
    connect(zv_backwardAction, SIGNAL(triggered()), zv_browser, SLOT(backward()));
    connect(zv_forwardAction, SIGNAL(triggered()), zv_browser, SLOT(forward()));
}
//==============================================
void ZHelpBrowser::zh_dialogControl()
{
    zv_backwardAction->setEnabled(zv_browser->isBackwardAvailable());
    zv_forwardAction->setEnabled(zv_browser->isForwardAvailable());
}
//==============================================
bool ZHelpBrowser::zh_openSettingsGroup(QSettings* settings) const
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
bool ZHelpBrowser::zh_closeSettingsGroup(QSettings* settings) const
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
void ZHelpBrowser::zh_restoreSettings()
{
    QSettings settings;
    if(!zh_openSettingsGroup(&settings))
    {
        return;
    }

    QVariant vData = settings.value(zv_geometryValueName);
    if(vData.isValid() && vData.canConvert<QByteArray>())
    {
        restoreGeometry(vData.toByteArray());
    }

    zh_closeSettingsGroup(&settings);
}
//==============================================
void ZHelpBrowser::zh_saveSettings() const
{
    QSettings settings;
    if(!zh_openSettingsGroup(&settings))
    {
        return;
    }

    settings.setValue(zv_geometryValueName, saveGeometry());

    zh_closeSettingsGroup(&settings);
}
//==============================================
ZHelpBrowser* ZHelpBrowser::zp_instance(QWidget *parent)
{
    if(! zv_this)
    {
        zv_this = new ZHelpBrowser(parent);
    }
    zv_refCount++;
    return  zv_this;
}
//==============================================
ZHelpBrowser* ZHelpBrowser::zp_instance(const QStringList& pathList,
                                               const QUrl& doc,
                                               QWidget *parent)
{
    zp_instance(parent);
    if(zv_this != nullptr)
    {
        zv_this->zp_loadSearchPathList(pathList);
        zv_this->zp_loadContentSourceUrl(doc);
    }
    return zv_this;
}
//==============================================
void ZHelpBrowser::zp_freeInstance()
{
    zv_refCount--;
    if(zv_refCount < 1)
    {
        QSettings settings;
        settings.beginGroup(qApp->applicationVersion());
        settings.setValue("help geometry", saveGeometry());
        settings.endGroup();

        delete this;
        zv_this = 0;
    }
}
//==============================================
