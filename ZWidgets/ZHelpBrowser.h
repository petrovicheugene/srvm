//==============================================
#ifndef ZHELPBROWSER_H
#define ZHELPBROWSER_H
//==============================================
#include <QDialog>
//==============================================
class QTextBrowser;
class QPushButton;
class QAction;
//==============================================
class ZHelpBrowser : public QDialog
{
    Q_OBJECT
public:

    void mf_restoreGeometry();
    static ZHelpBrowser* mf_instance(const QStringList& pathList, const QUrl& doc, QWidget *parent);
    void zp_freeInstance();

private:

    // VARS
    QAction* mv_homeAction;
    QAction* mv_backwardAction;
    QAction* mv_forwardAction;

    QTextBrowser* mv_browser;

    QByteArray mv_geometry;

    static ZHelpBrowser* mv_this;
    static int mv_refCount;

    // FUNCS
    explicit ZHelpBrowser(const QStringList& pathList, const QUrl& doc, QWidget *parent  = 0);
    virtual ~ZHelpBrowser();

    void closeEvent(QCloseEvent *e);
    void zp_createComponents(const QStringList& pathList, const QUrl& doc);
    void zp_createConnections();
    void mf_dialogControl();

signals:

public slots:

};
//==============================================
#endif // ZHELPBROWSER_H
