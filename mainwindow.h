//==========================================================
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//==========================================================
#include "X_BaseMainWindow.h"
#include "X_TermCorrelationTableWidget.h"
//==========================================================
class X_FileActionManager;
class X_SpectrumArrayWidget;
class X_JointSpectrumTableWidget;
class X_ChemElementWidget;
class X_CalibrationTableWidget;
class X_CalibrationWindowTableWidget;
class X_TermCorrelationTableWidget;
class X_EquationSettingsDashBoard;
class X_CorrelationPlotterWidget;
class X_WidgetWithSidebar;
class X_MessagePanel;
class X_MessageWidget;
class X_EnergyLineTableWidget;

class QAction;
class QFrame;
class QMenu;
class X_Plotter;
class X_HelpBrowser;

class X_ArrayModel;
class X_JointSpectraModel;
class X_ChemElementModel;
class X_CalibrationModel;
class X_CalibrationWindowModel;
class X_TermCorrelationTableModel;

class X_SpectrumArrayRepository;
class X_CalibrationRepository;

class X_JointSpectraDataManager;
class X_ChemElementDataManager;
class X_CalibrationWindowDataManager;
class X_AbstractPlotterDataManager;
class X_CorrelationPlotterDataManager;
class X_TermCorrelationTableManager;
//==========================================================
class MainWindow : public X_BaseMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

    void xg_saveSettings();

    void xg_plainLogMessage(int msgType,
                            const QString &msg);

private slots:

    void xh_messageIconPixmap(int type,
                              QSize size,
                              QStyle *style,
                              QPixmap& pixmap,
                              bool &ok) const;



private:

    // VARS
    // actions
    QAction* xv_exitAction;
    //QAction* xv_languageControlAction;
    QMenu* xv_languageMenu;
    // QAction* xv_openCalibrationProjectAction;
    //    QAction* xv_helpAction;

    // docks
    QList<QDockWidget*> xv_dockList;
    QDockWidget* xv_spectrumArrayDock;
    QDockWidget* xv_calibrationDock;
    QDockWidget* xv_correlationPlotterDock;
    QDockWidget* xv_chemElementArrayDock;
    QDockWidget* xv_messagePanelDock;
    QDockWidget* xv_energyLineTableDock;

    // views
    X_WidgetWithSidebar* xv_spectraSidebarWidget;
    X_WidgetWithSidebar* xv_calibrationSidebarWidget;

    X_SpectrumArrayWidget* xv_spectrumArrayWidget;
    X_JointSpectrumTableWidget* xv_spectrumTableWidget;
    X_ChemElementWidget* xv_chemElementWidget;
    X_CalibrationTableWidget* xv_calibrationTableWidget;
    X_CalibrationWindowTableWidget* xv_calibrationWindowTableWidget;

    X_TermCorrelationTableWidget* xv_termCorrelationTableWidget;
    X_EquationSettingsDashBoard* xv_equationSettingsPanelWidget;
    X_CorrelationPlotterWidget* xv_correlationPlotterWidget;
    X_EnergyLineTableWidget* xv_energyLineTableWidget;

    X_MessageWidget* xv_messageWidget;
    X_Plotter* xv_plotter;

    // models
    X_ArrayModel* xv_arrayModel;
    X_JointSpectraModel* xv_jointSpectraModel;
    X_ChemElementModel* xv_chemElementModel;
    X_CalibrationModel* xv_calibrationModel;
    X_CalibrationWindowModel* xv_jointCalibrationWindowModel;
    X_TermCorrelationTableModel* xv_termCorrelationTableModel;

    // components
    X_HelpBrowser* mv_helpBrowser;
    X_FileActionManager* xv_fileActionManager;
    X_SpectrumArrayRepository* xv_spectrumArrayRepository;
    X_JointSpectraDataManager* xv_jointSpectraDataManager;
    X_CalibrationRepository* xv_calibrationRepository;
    X_CalibrationWindowDataManager* xv_jointCalibrationWindowDataManager;
    X_ChemElementDataManager* xv_chemElementDataManager;
    X_AbstractPlotterDataManager* xv_plotterDataManager;
    X_CorrelationPlotterDataManager* xv_calculationPlotterManager;
    X_TermCorrelationTableManager* xv_termCorrelationTableManager;

    // FUNCS
    void closeEvent(QCloseEvent* e);
    void xh_createActions();
    void xh_createComponents();
    QFrame* xh_setWidgetToFrame(QWidget*);
    void xh_createMenu();
    void xh_createToolbar();
    void xh_createConnections();

    void xh_appendActionsToMenu(QMenu* menu);
    void xh_restoreSettings();
    void xh_saveSettings();

private slots:


    void xh_rebuildCorrelationPlot();
    void xh_createPlotterWidget();
    void xh_appLanguageControl();
    void xh_fillLanguageMenu();

};
Q_DECLARE_METATYPE(QtMsgType)

//==========================================================
#endif // MAINWINDOW_H
