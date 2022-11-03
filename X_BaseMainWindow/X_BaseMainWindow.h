//======================================================
/*!
 * \brief This base main window provides the most common functionality
 *
 * The main window class provides the following functionality:
 * For receiving and further handling messages sent via function
 * qDebug, qWarning, qCritical and qFatal.
 * Help and about actions. Help file should be in HTML format
 * and have the name EXE_BASE_NAME
 * Opening and closing settings groups
*/
//======================================================
#ifndef X_MAINWINDOWBASE_H
#define X_MAINWINDOWBASE_H
//======================================================
#include <QMainWindow>
//======================================================
class QSettings;
class X_HelpBrowser;
//======================================================
class X_BaseMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit X_BaseMainWindow(QWidget *parent = nullptr);

    virtual void xp_handleStandardLogMessage(QtMsgType type,
                                     const QMessageLogContext &context,
                                     const QString &msg) const;

signals:

    void xg_standardLogMessage(QtMsgType type,
                               const QString& msg) const;

private slots:

    virtual void zh_help();
    virtual void zh_about();


protected:

    // VARS
    X_HelpBrowser* xv_helpBrowser;

    QAction* xv_helpAction;
    QAction* xv_aboutAction;
    QAction* xv_aboutQtAction;

    // FUNCS
    void zh_createBaseActions();
    void zh_createBaseConnections();

    virtual bool zh_openSettingsGroup(QSettings* settings);
    virtual bool zh_closeSettingsGroup(QSettings* settings);

};
//======================================================
#endif // X_MAINWINDOWBASE_H
