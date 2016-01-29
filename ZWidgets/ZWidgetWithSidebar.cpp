//=======================================================
#include "ZWidgetWithSidebar.h"
#include "ZClickableLabel.h"
#include "globalVariables.h"

#include <QCloseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QSettings>
//=======================================================
ZWidgetWithSidebar::ZWidgetWithSidebar(const QString &objectName,
                                       bool sideBarIsOnLeft,
                                       QWidget *parent) : QFrame(parent)
{
    zv_sideBarWidget = 0;
    zv_mainWidget = 0;
    setObjectName(objectName);
    zh_createComponents();
    zh_createConnections();
    // zh_restoreState();

}
//=======================================================
ZWidgetWithSidebar::ZWidgetWithSidebar(const QString &title,
                                       const QString &objectName,
                                       bool sideBarIsOnLeft,
                                       QWidget *parent) : QFrame(parent)
{
    zv_sideBarWidget = 0;
    zv_mainWidget = 0;
    setObjectName(objectName);
    zh_createComponents(title);
    zh_createConnections();
    // zh_restoreState();

}
//=======================================================
bool ZWidgetWithSidebar::zp_setMainWidget(QWidget* mainWidget)
{
    if(mainWidget == 0 || zv_mainWidget != 0 || !(zv_splitter->count() < 2))
    {
        return false;
    }
    zv_mainWidget = mainWidget;
    zv_splitter->insertWidget(zv_splitter->count(), zv_mainWidget);
    zv_splitter->setCollapsible(zv_splitter->count() - 1, false);
    zh_restoreSettings();
    return true;
}
//=======================================================
bool ZWidgetWithSidebar::zp_setSidebarWidget(QWidget* sideBarWidget)
{
    if(sideBarWidget == 0 || zv_sideBarWidget != 0 || !(zv_splitter->count() < 2))
    {
        return false;
    }
    zv_sideBarWidget = sideBarWidget;
    zv_splitter->insertWidget(0, zv_sideBarWidget);
    zv_splitter->setCollapsible(0, false);

    zv_hideLabel->setPixmap(QPixmap(":/images/ZWidgets/sidebarBlueClose.png"));
    zv_hideLabel->setVisible(true);
    zv_hideLabel->setToolTip(tr("Hide left panel"));
    zh_restoreSettings();
    return true;
}
//=======================================================
void ZWidgetWithSidebar::zp_setInfoLabelText(bool dirty, const QString& text)
{
    QString infoText = text;
    if(dirty)
    {
        infoText += "*";
        infoText = "<font color=red><b>" + infoText + "</b></font>";
    }
    zv_infoLabel->setText(infoText);
}
//=======================================================
void ZWidgetWithSidebar::zh_createComponents(QString title)
{
    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    int margin = mainLayout->margin();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    // Title
    if(!title.isEmpty())
    {
        QLabel* titleLabel = new QLabel(this);
        titleLabel->setText(title);
        titleLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        QHBoxLayout* titleLayout = new QHBoxLayout(this);
        titleLayout->addSpacing(margin);
        titleLayout->addWidget(titleLabel);
        titleLayout->addStretch();
        mainLayout->addSpacing(margin);
        mainLayout->addLayout(titleLayout);
    }
    // Splitter
    zv_splitter = new QSplitter(this);
    zv_splitter->setFrameShape(QFrame::NoFrame);
    mainLayout->addWidget(zv_splitter);

    // Basement Widget
    QFrame* basement = new QFrame(this);
    //basement->setFrameShape(QFrame::HLine);
    //basement->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    // basement layout
    QHBoxLayout* basementLayout = new QHBoxLayout(this);
    basement->setLayout(basementLayout);
    // int margin = basementLayout->margin();
    basementLayout->setMargin(0);
    basementLayout->setSpacing(margin);
    basementLayout->addSpacing(margin);

    QSizePolicy sizePolicy = basement->sizePolicy();
    sizePolicy.setVerticalPolicy(QSizePolicy::Fixed);
    basement->setSizePolicy(sizePolicy);

    mainLayout->addWidget(basement);
    mainLayout->addSpacing(margin);

    // labels
    zv_hideLabel = new ZClickableLabel(this);
    zv_hideLabel->setCursor(Qt::PointingHandCursor);
    zv_hideLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    zv_hideLabel->setVisible(false);
    basementLayout->addWidget(zv_hideLabel);

    zv_infoLabel = new QLabel(this);
    basementLayout->addWidget(zv_infoLabel);
    basementLayout->addSpacing(margin);

}
//=======================================================
void ZWidgetWithSidebar::zh_createConnections()
{
    connect(zv_hideLabel, &ZClickableLabel::clicked,
            this, zh_onHideSidePanelButtonClick);
}
//=======================================================
void ZWidgetWithSidebar::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup(this->objectName());

    vData = settings.value("splitterGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        zv_splitter->restoreState(vData.value<QByteArray>());
    }


    settings.endGroup();
    settings.endGroup();
}
//=======================================================
void ZWidgetWithSidebar::zp_saveSettings()
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup(this->objectName());

    settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(zv_splitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//=======================================================
void ZWidgetWithSidebar::zh_manageHideLabelPix()
{
    if(zv_sideBarWidget->isVisible())
    {
        zv_hideLabel->setPixmap(QPixmap(":/images/ZWidgets/sidebarBlueClose.png"));
        zv_hideLabel->setToolTip(tr("Hide left panel"));
    }
    else
    {
        zv_hideLabel->setPixmap(QPixmap(":/images/ZWidgets/sidebarBlueOpen.png"));
        zv_hideLabel->setToolTip(tr("Show left panel"));
    }
}
//=======================================================
void ZWidgetWithSidebar::zh_onHideSidePanelButtonClick()
{
    if(zv_sideBarWidget)
    {
        zv_sideBarWidget->setVisible(!zv_sideBarWidget->isVisible());
    }

    zh_manageHideLabelPix();
}
//=======================================================

