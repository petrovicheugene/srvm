//==============================================
#ifndef ZHELPBROWSER_H
#define ZHELPBROWSER_H
//==============================================
#include <QDialog>
//==============================================
class QAction;
class QPushButton;
class QSettings;
class QTextBrowser;
//==============================================
class ZHelpBrowser : public QDialog
{
    Q_OBJECT
public:

    enum ButtonIconColor {BIC_GREEN, BIC_BLUE};

    static ZHelpBrowser* zp_instance(QWidget *parent);
    static ZHelpBrowser* zp_instance(const QStringList& pathList, const QUrl& doc, QWidget *parent);
    void zp_freeInstance();

    void zp_loadContentSourceUrl( const QUrl& doc);
    void zp_loadSearchPathList(const QStringList& pathList);
    void zp_setButtonIcon(ZHelpBrowser::ButtonIconColor buttonIconColor);

private:

    // VARS
    const QString zv_geometryValueName = "geometry";

    QAction* zv_homeAction;
    QAction* zv_backwardAction;
    QAction* zv_forwardAction;

    QTextBrowser* zv_browser;

    static ZHelpBrowser* zv_this;
    static int zv_refCount;

    // FUNCS
    explicit ZHelpBrowser(QWidget *parent  = 0);
    virtual ~ZHelpBrowser();

    void zh_createComponents();
    void zh_createConnections();
    void zh_dialogControl();

    bool zh_openSettingsGroup(QSettings* settings) const;
    bool zh_closeSettingsGroup(QSettings* settings) const;
    void zh_restoreSettings();
    void zh_saveSettings() const;

};
//==============================================
#endif // ZHELPBROWSER_H
