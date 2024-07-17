//==========================================================
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//==========================================================
#include "X_BaseMainWindow.h"
#include "X_TermCorrelationTableWidget.h"
#include <QDockWidget>
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
    QAction* xv_exitAction = nullptr;
    //QAction* xv_languageControlAction;
    QMenu* xv_languageMenu = nullptr;
    // QAction* xv_openCalibrationProjectAction;
    //    QAction* xv_helpAction;

    // docks
    QList<QDockWidget*> xv_dockList;
    QDockWidget* xv_spectrumArrayDock = nullptr;
    QDockWidget* xv_calibrationDock = nullptr;
    QDockWidget* xv_correlationPlotterDock = nullptr;
//    QDockWidget* xv_chemElementArrayDock = nullptr;
    QDockWidget* xv_messagePanelDock = nullptr;
    QDockWidget* xv_energyLineTableDock = nullptr;

    // views
    X_WidgetWithSidebar* xv_spectraSidebarWidget = nullptr;
    X_WidgetWithSidebar* xv_calibrationSidebarWidget = nullptr;

    X_SpectrumArrayWidget* xv_spectrumArrayWidget = nullptr;
    X_JointSpectrumTableWidget* xv_spectrumTableWidget = nullptr;
    X_ChemElementWidget* xv_chemElementWidget = nullptr;
    X_CalibrationTableWidget* xv_calibrationTableWidget = nullptr;
    X_CalibrationWindowTableWidget* xv_calibrationWindowTableWidget = nullptr;

    X_TermCorrelationTableWidget* xv_termCorrelationTableWidget = nullptr;
    X_EquationSettingsDashBoard* xv_equationSettingsPanelWidget = nullptr;
    X_CorrelationPlotterWidget* xv_correlationPlotterWidget = nullptr;
    X_EnergyLineTableWidget* xv_energyLineTableWidget = nullptr;

    X_MessageWidget* xv_messageWidget = nullptr;
    X_Plotter* xv_plotter = nullptr;

    // models
    X_ArrayModel* xv_arrayModel = nullptr;
    X_JointSpectraModel* xv_jointSpectraModel = nullptr;
    X_ChemElementModel* xv_chemElementModel = nullptr;
    X_CalibrationModel* xv_calibrationModel = nullptr;
    X_CalibrationWindowModel* xv_jointCalibrationWindowModel = nullptr;
    X_TermCorrelationTableModel* xv_termCorrelationTableModel = nullptr;

    // components
    X_HelpBrowser* mv_helpBrowser = nullptr;
    X_FileActionManager* xv_fileActionManager = nullptr;
    X_SpectrumArrayRepository* xv_spectrumArrayRepository = nullptr;
    X_JointSpectraDataManager* xv_jointSpectraDataManager = nullptr;
    X_CalibrationRepository* xv_calibrationRepository = nullptr;
    X_CalibrationWindowDataManager* xv_jointCalibrationWindowDataManager = nullptr;
    X_ChemElementDataManager* xv_chemElementDataManager = nullptr;
    X_AbstractPlotterDataManager* xv_plotterDataManager = nullptr;
    X_CorrelationPlotterDataManager* xv_calculationPlotterManager = nullptr;
    X_TermCorrelationTableManager* xv_termCorrelationTableManager = nullptr;

    // FUNCS
    void closeEvent(QCloseEvent* e) override;
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
