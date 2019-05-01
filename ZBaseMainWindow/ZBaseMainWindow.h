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
#ifndef ZMAINWINDOWBASE_H
#define ZMAINWINDOWBASE_H
//======================================================
#include <QMainWindow>
//======================================================
class QSettings;
class ZHelpBrowser;
//======================================================
class ZBaseMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ZBaseMainWindow(QWidget *parent = nullptr);

    virtual void zp_handleStandardLogMessage(QtMsgType type,
                                     const QMessageLogContext &context,
                                     const QString &msg) const;

signals:

    void zg_standardLogMessage(QtMsgType type,
                               const QString& msg) const;

private slots:

    virtual void zh_help();
    virtual void zh_about();


protected:

    // VARS
    ZHelpBrowser* zv_helpBrowser;

    QAction* zv_helpAction;
    QAction* zv_aboutAction;
    QAction* zv_aboutQtAction;

    // FUNCS
    void zh_createBaseActions();
    void zh_createBaseConnections();

    virtual bool zh_openSettingsGroup(QSettings* settings);
    virtual bool zh_closeSettingsGroup(QSettings* settings);

};
//======================================================
#endif // ZMAINWINDOWBASE_H
