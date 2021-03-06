//==========================================================
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//==========================================================
#include "ZBaseMainWindow.h"
#include "ZTermCorrelationTableWidget.h"
//==========================================================
class ZFileActionManager;
class ZSpectrumArrayWidget;
class ZJointSpectrumTableWidget;
class ZChemElementWidget;
class ZCalibrationTableWidget;
class ZCalibrationWindowTableWidget;
class ZTermCorrelationTableWidget;
class ZEquationSettingsDashBoard;
class ZCorrelationPlotterWidget;
class ZWidgetWithSidebar;
class ZMessagePanel;
class ZMessageWidget;
class ZEnergyLineTableWidget;

class QAction;
class QFrame;
class QMenu;
class ZPlotter;
class ZHelpBrowser;

class ZArrayModel;
class ZJointSpectraModel;
class ZChemElementModel;
class ZCalibrationModel;
class ZCalibrationWindowModel;
class ZTermCorrelationTableModel;

class ZSpectrumArrayRepository;
class ZCalibrationRepository;

class ZJointSpectraDataManager;
class ZChemElementDataManager;
class ZCalibrationWindowDataManager;
class ZAbstractPlotterDataManager;
class ZCorrelationPlotterDataManager;
class ZTermCorrelationTableManager;
//==========================================================
class MainWindow : public ZBaseMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

    void zg_saveSettings();

    void zg_plainLogMessage(int msgType,
                            const QString &msg);

private slots:

    void zh_messageIconPixmap(int type,
                              QSize size,
                              QStyle *style,
                              QPixmap& pixmap,
                              bool &ok) const;



private:

    // VARS
    // actions
    QAction* zv_exitAction;
    //QAction* zv_languageControlAction;
    QMenu* zv_languageMenu;
    // QAction* zv_openCalibrationProjectAction;
    //    QAction* zv_helpAction;

    // docks
    QList<QDockWidget*> zv_dockList;
    QDockWidget* zv_spectrumArrayDock;
    QDockWidget* zv_calibrationDock;
    QDockWidget* zv_correlationPlotterDock;
    QDockWidget* zv_chemElementArrayDock;
    QDockWidget* zv_messagePanelDock;
    QDockWidget* zv_energyLineTableDock;

    // views
    ZWidgetWithSidebar* zv_spectraSidebarWidget;
    ZWidgetWithSidebar* zv_calibrationSidebarWidget;

    ZSpectrumArrayWidget* zv_spectrumArrayWidget;
    ZJointSpectrumTableWidget* zv_spectrumTableWidget;
    ZChemElementWidget* zv_chemElementWidget;
    ZCalibrationTableWidget* zv_calibrationTableWidget;
    ZCalibrationWindowTableWidget* zv_calibrationWindowTableWidget;

    ZTermCorrelationTableWidget* zv_termCorrelationTableWidget;
    ZEquationSettingsDashBoard* zv_equationSettingsPanelWidget;
    ZCorrelationPlotterWidget* zv_correlationPlotterWidget;
    ZEnergyLineTableWidget* zv_energyLineTableWidget;

    ZMessageWidget* zv_messageWidget;
    ZPlotter* zv_plotter;

    // models
    ZArrayModel* zv_arrayModel;
    ZJointSpectraModel* zv_jointSpectraModel;
    ZChemElementModel* zv_chemElementModel;
    ZCalibrationModel* zv_calibrationModel;
    ZCalibrationWindowModel* zv_jointCalibrationWindowModel;
    ZTermCorrelationTableModel* zv_termCorrelationTableModel;

    // components
    ZHelpBrowser* mv_helpBrowser;
    ZFileActionManager* zv_fileActionManager;
    ZSpectrumArrayRepository* zv_spectrumArrayRepository;
    ZJointSpectraDataManager* zv_jointSpectraDataManager;
    ZCalibrationRepository* zv_calibrationRepository;
    ZCalibrationWindowDataManager* zv_jointCalibrationWindowDataManager;
    ZChemElementDataManager* zv_chemElementDataManager;
    ZAbstractPlotterDataManager* zv_plotterDataManager;
    ZCorrelationPlotterDataManager* zv_calculationPlotterManager;
    ZTermCorrelationTableManager* zv_termCorrelationTableManager;

    // FUNCS
    void closeEvent(QCloseEvent* e);
    void zh_createActions();
    void zh_createComponents();
    QFrame* zh_setWidgetToFrame(QWidget*);
    void zh_createMenu();
    void zh_createToolbar();
    void zh_createConnections();

    void zh_appendActionsToMenu(QMenu* menu);
    void zh_restoreSettings();
    void zh_saveSettings();

private slots:


    void zh_rebuildCorrelationPlot();
    void zh_createPlotterWidget();
    void zh_appLanguageControl();
    void zh_fillLanguageMenu();

};
Q_DECLARE_METATYPE(QtMsgType)

//==========================================================
#endif // MAINWINDOW_H
