//==========================================================
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//==========================================================
#include <QMainWindow>
//==========================================================
class ZFileActionManager;
class ZSpectrumArrayWidget;
class ZSpectrumTableWidget;
class ZChemElementWidget;
class ZCalibrationTableWidget;
class ZWidgetWithSidebar;
class ZMessagePanel;

class QAction;
class QSplitter;
class ZPlotter;

class ZArrayModel;
class ZJointSpectraModel;
class ZChemElementModel;
class ZCalibrationModel;
//==========================================================
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

    void zg_saveSettings();

private:

    // VARS


    // actions
    QAction* zv_exitAction;
    QAction* zv_aboutAction;
    QAction* zv_helpAction;

    // central widget
    QSplitter* zv_centralSplitter;

    // docks
    QList<QDockWidget*> zv_dockList;
    QDockWidget* zv_calibrationArrayDock;
    QDockWidget* zv_messagePanelDock;

    // views
    ZWidgetWithSidebar* zv_spectraSidebarWidget;
    ZWidgetWithSidebar* zv_calibrationSidebarWidget;

    ZSpectrumArrayWidget* zv_spectrumArrayWidget;
    ZSpectrumTableWidget* zv_spectrumTableWidget;
    ZChemElementWidget* zv_calibrationArrayWidget;
    ZCalibrationTableWidget* zv_calibrationTableWidget;

    ZMessagePanel* zv_messagePanel;
    ZPlotter* zv_plotter;

    // models
    ZArrayModel* zv_arrayModel;
    ZJointSpectraModel* zv_jointSpectraModel;
    ZChemElementModel* zv_chemElementModel;
    ZCalibrationModel* zv_calibrationModel;

    // components
    ZFileActionManager* zv_fileActionManager;


    // FUNCS
    void closeEvent(QCloseEvent* e);
    void zh_createActions();
    void zh_createComponents();
    void zh_createMenu();
    void zh_createToolbar();
    void zh_createConnections();
    void zh_appendActionsToMenu(QMenu* menu);
    void zh_restoreSettings();
    void zh_saveSettings();

private slots:

    void zh_onAboutAction();
    void zh_onHelpAction();


};
//==========================================================
#endif // MAINWINDOW_H
