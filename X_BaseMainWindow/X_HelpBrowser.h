//==============================================
#ifndef X_HELPBROWSER_H
#define X_HELPBROWSER_H
//==============================================
#include <QDialog>
//==============================================
class QAction;
class QPushButton;
class QSettings;
class QTextBrowser;
//==============================================
class X_HelpBrowser : public QDialog
{
    Q_OBJECT
public:

    enum ButtonIconColor {BIC_GREEN, BIC_BLUE};

    static X_HelpBrowser* xp_instance(QWidget *parent);
    static X_HelpBrowser* xp_instance(const QStringList& pathList, const QUrl& doc, QWidget *parent);
    void xp_freeInstance();

    void xp_loadContentSourceUrl( const QUrl& doc);
    void xp_loadSearchPathList(const QStringList& pathList);
    void xp_setButtonIcon(X_HelpBrowser::ButtonIconColor buttonIconColor);

private:

    // VARS
    const QString xv_geometryValueName = "geometry";

    QAction* xv_homeAction;
    QAction* xv_backwardAction;
    QAction* xv_forwardAction;

    QTextBrowser* xv_browser;

    static X_HelpBrowser* xv_this;
    static int xv_refCount;

    // FUNCS
    explicit X_HelpBrowser(QWidget *parent  = 0);
    virtual ~X_HelpBrowser();

    void xh_createComponents();
    void xh_createConnections();
    void xh_dialogControl();

    bool xh_openSettingsGroup(QSettings* settings) const;
    bool xh_closeSettingsGroup(QSettings* settings) const;
    void xh_restoreSettings();
    void xh_saveSettings() const;

};
//==============================================
#endif // X_HELPBROWSER_H
