//=======================================================
#ifndef X_SIDEAREDWIDGET_H
#define X_SIDEAREDWIDGET_H
//=======================================================
#include <QFrame>
//=======================================================
class QSplitter;
class QLabel;
class X_ClickableLabel;
class QVBoxLayout;
//=======================================================
class X_WidgetWithSidebar : public QFrame
{
    Q_OBJECT
public:
    explicit X_WidgetWithSidebar(const QString& objectName,
                                bool sideBarOnLeft,
                                QWidget *parent);
    explicit X_WidgetWithSidebar(const QString& title,
                                const QString& objectName,
                                bool sideBarIsOnLeft,
                                QWidget *parent);

    bool xp_setMainWidget(QWidget*);
    bool xp_setSidebarWidget(QWidget*);

    void xp_setMargin(int margin);

signals:

public slots:

    void xp_setInfoLabelText(bool, const QString&);
    void xp_saveSettings();

private:

    // VARS
    QVBoxLayout* xv_mainLayout;
    int xv_margin;
    bool xv_sideBarOnLeft;

    QSplitter* xv_splitter;
    X_ClickableLabel* xv_hideLabel;
    QLabel* xv_infoLabel;
    QWidget* xv_mainWidget;
    QWidget* xv_sideBarWidget;

    QPixmap xv_closePixmap;
    QPixmap xv_openPixmap;
    QString hideToolTipString;
    QString showToolTipString;

    // FUNCS
    void xh_createComponents(QString title = QString());
    void xh_createConnections();
    void xh_restoreSettings();

    void xh_manageHideLabelPix();

private slots:

    void xh_onHideSidePanelButtonClick();

};
//=======================================================
#endif // X_SIDEAREDWIDGET_H
