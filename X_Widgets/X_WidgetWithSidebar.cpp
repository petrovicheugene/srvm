//=======================================================
#include "X_WidgetWithSidebar.h"
#include "X_ClickableLabel.h"
#include "X_General.h"

#include <QApplication>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QSettings>
//=======================================================
X_WidgetWithSidebar::X_WidgetWithSidebar(const QString &objectName,
                                       bool sideBarOnLeft,
                                       QWidget *parent) : QFrame(parent)
{
    xv_sideBarOnLeft = sideBarOnLeft;
    xv_sideBarWidget = nullptr;
    xv_mainWidget = nullptr;
    hideToolTipString = xv_sideBarOnLeft? tr("Hide left panel") : tr("Hide right panel");
    showToolTipString = xv_sideBarOnLeft? tr("Show left panel") : tr("Show right panel");

    setObjectName(objectName);
    xh_createComponents();
    xh_createConnections();
    // xh_restoreState();

}
//=======================================================
X_WidgetWithSidebar::X_WidgetWithSidebar(const QString &title,
                                       const QString &objectName,
                                       bool sideBarIsOnLeft,
                                       QWidget *parent) : QFrame(parent)
{
    xv_sideBarWidget = nullptr;
    xv_mainWidget = nullptr;
    setObjectName(objectName);
    xh_createComponents(title);
    xh_createConnections();
    // xh_restoreState();

}
//=======================================================
bool X_WidgetWithSidebar::xp_setMainWidget(QWidget* mainWidget)
{
    if(mainWidget == nullptr || xv_mainWidget != 0 || !(xv_splitter->count() < 2))
    {
        return false;
    }
    xv_mainWidget = mainWidget;
    int widgetIndex = xv_sideBarOnLeft? xv_splitter->count() : 0;

    xv_splitter->insertWidget(widgetIndex, xv_mainWidget);
    xv_splitter->setCollapsible(widgetIndex, false);
    xh_restoreSettings();
    return true;
}
//=======================================================
bool X_WidgetWithSidebar::xp_setSidebarWidget(QWidget* sideBarWidget)
{
    if(sideBarWidget == nullptr || xv_sideBarWidget != 0 || !(xv_splitter->count() < 2))
    {
        return false;
    }

    xv_sideBarWidget = sideBarWidget;
    int widgetIndex = xv_sideBarOnLeft? 0 : xv_splitter->count();
    xv_splitter->insertWidget(widgetIndex, xv_sideBarWidget);
    xv_splitter->setCollapsible(widgetIndex, false);

    xv_hideLabel->setPixmap(xv_closePixmap); // QPixmap(":/images/X_Widgets/sidebarRightBlueClose.png"));
    xv_hideLabel->setVisible(true);
    xv_hideLabel->setToolTip(hideToolTipString);
    xh_restoreSettings();
    return true;
}
//=======================================================
void X_WidgetWithSidebar::xp_setMargin(int margin)
{
    xv_mainLayout->setContentsMargins(margin, margin, margin, margin);
}
//=======================================================
void X_WidgetWithSidebar::xp_setInfoLabelText(bool dirty, const QString& text)
{
    QString infoText = text;
    if(dirty)
    {
        infoText += "*";
        infoText = "<font color=red><b>" + infoText + "</b></font>";
    }
    xv_infoLabel->setText(infoText);
    xv_infoLabel->setToolTip(infoText);
}
//=======================================================
void X_WidgetWithSidebar::xh_createComponents(QString title)
{
    // Main Layout
    xv_mainLayout = new QVBoxLayout;
    xv_margin = xv_mainLayout->contentsMargins().right();
    // xv_mainLayout->setMargin(0);
    xv_mainLayout->setSpacing(0);
    setLayout(xv_mainLayout);

    // Title
    if(!title.isEmpty())
    {
        QLabel* titleLabel = new QLabel(this);
        titleLabel->setText(title);
        titleLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        QHBoxLayout* titleLayout = new QHBoxLayout;
        titleLayout->addSpacing(xv_margin);
        titleLayout->addWidget(titleLabel);
        titleLayout->addStretch();
        xv_mainLayout->addSpacing(xv_margin);
        xv_mainLayout->addLayout(titleLayout);
    }
    // Splitter
    xv_splitter = new QSplitter(this);
    xv_splitter->setFrameShape(QFrame::NoFrame);
    xv_mainLayout->addWidget(xv_splitter);

    // Basement Widget
    QWidget* basement = new QWidget(this);
    //basement->setFrameShape(QFrame::HLine);
    //basement->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    // basement layout
    QHBoxLayout* basementLayout = new QHBoxLayout;
    basement->setLayout(basementLayout);
    // int margin = basementLayout->margin();
    basementLayout->setContentsMargins(0, 0, 0, 0);
    basementLayout->setSpacing(xv_margin);
    // basementLayout->addSpacing(margin);

    QSizePolicy sizePolicy = basement->sizePolicy();
    sizePolicy.setVerticalPolicy(QSizePolicy::Fixed);
    basement->setSizePolicy(sizePolicy);

    xv_mainLayout->addWidget(basement);
    xv_mainLayout->addSpacing(xv_margin);

    // labels
    xv_hideLabel = new X_ClickableLabel(this);
    xv_hideLabel->setCursor(Qt::PointingHandCursor);
    xv_hideLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    xv_hideLabel->setVisible(false);
    basementLayout->addWidget(xv_hideLabel);

    xv_infoLabel = new QLabel(this);
    xv_infoLabel->setSizePolicy(QSizePolicy::Ignored , QSizePolicy::Expanding);
    basementLayout->addWidget(xv_infoLabel);
    //basementLayout->addSpacing(margin);

    if(xv_sideBarOnLeft)
    {
        xv_closePixmap = QPixmap(NS_Icons::glIconSidebarRightClose);
        xv_openPixmap = QPixmap(NS_Icons::glIconSidebarRightOpen);
        basementLayout->addWidget(xv_hideLabel, 0, Qt::AlignLeft);
        basementLayout->addSpacing(5);
        basementLayout->addWidget(xv_infoLabel, 999999, Qt::AlignLeft);
    }
    else
    {
        xv_closePixmap = QPixmap(NS_Icons::glIconSidebarLeftClose);
        xv_openPixmap = QPixmap(NS_Icons::glIconSidebarLeftOpen);
        basementLayout->addWidget(xv_infoLabel, 999999, Qt::AlignRight);
        basementLayout->addSpacing(5);
        basementLayout->addWidget(xv_hideLabel, 0, Qt::AlignRight);
    }
}
//=======================================================
void X_WidgetWithSidebar::xh_createConnections()
{
    connect(xv_hideLabel, &X_ClickableLabel::clicked,
            this, &X_WidgetWithSidebar::xh_onHideSidePanelButtonClick);
}
//=======================================================
void X_WidgetWithSidebar::xh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup(this->objectName());

    vData = settings.value("splitterGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        xv_splitter->restoreState(vData.value<QByteArray>());
    }


    settings.endGroup();
    settings.endGroup();
}
//=======================================================
void X_WidgetWithSidebar::xp_saveSettings()
{
    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup(this->objectName());

    settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(xv_splitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//=======================================================
void X_WidgetWithSidebar::xh_manageHideLabelPix()
{
    if(xv_sideBarWidget->isVisible())
    {
        xv_hideLabel->setPixmap(xv_closePixmap); //  QPixmap(":/images/X_Widgets/sidebarRightBlueClose.png"));
        xv_hideLabel->setToolTip(hideToolTipString);
    }
    else
    {
        xv_hideLabel->setPixmap(xv_openPixmap);// ":/images/X_Widgets/sidebarRightBlueOpen.png"));
        xv_hideLabel->setToolTip(showToolTipString);
    }
}
//=======================================================
void X_WidgetWithSidebar::xh_onHideSidePanelButtonClick()
{
    if(xv_sideBarWidget)
    {
        xv_sideBarWidget->setVisible(!xv_sideBarWidget->isVisible());
    }

    xh_manageHideLabelPix();
}
//=======================================================

