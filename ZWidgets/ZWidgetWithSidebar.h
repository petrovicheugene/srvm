//=======================================================
#ifndef ZSIDEAREDWIDGET_H
#define ZSIDEAREDWIDGET_H
//=======================================================
#include <QWidget>
//=======================================================
class QSplitter;
class QLabel;
class ZClickableLabel;
//=======================================================
class ZWidgetWithSidebar : public QWidget
{
    Q_OBJECT
public:
    explicit ZWidgetWithSidebar(const QString& objectName, QWidget *parent = 0);
    explicit ZWidgetWithSidebar(const QString& title, const QString& objectName, QWidget *parent = 0);

    bool zp_setMainWidget(QWidget*);
    bool zp_setSidebarWidget(QWidget*);

signals:

public slots:

    void zp_setinfoLabelText(const QString&);
    void zp_saveSettings();

private:

    // VARS
    QSplitter* zv_splitter;
    ZClickableLabel* zv_hideLabel;
    QLabel* zv_infoLabel;
    QWidget* zv_mainWidget;
    QWidget* zv_sideBarWidget;

    // FUNCS
    void zh_createComponents(QString title = QString());
    void zh_createConnections();
    void zh_restoreSettings();

    void zh_manageHideLabelPix();

private slots:

    void zh_onHideSidePanelButtonClick();

};
//=======================================================
#endif // ZSIDEAREDWIDGET_H
