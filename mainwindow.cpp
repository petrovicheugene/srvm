//==========================================================
#include "MainWindow.h"
#include "glVariables.h"
// components
#include "ZFileActionManager.h"
#include "ZSpectraArrayRepository.h"
#include "ZSpectraJointDataManager.h"
// views
#include "ZWidgetWithSidebar.h"
#include "ZSpectrumArrayWidget.h"
#include "ZJointSpectraTableWidget.h"
#include "ZChemElementWidget.h"
#include "ZCalibrationTableWidget.h"
#include "ZPlotter.h"
#include "ZMessagePanel.h"
// models
#include "ZArrayModel.h"
#include "ZCalibrationModel.h"
#include "ZChemElementModel.h"
#include "ZJointSpectraModel.h"



#include <QMessageBox>
#include <QCloseEvent>
#include <QAction>
#include <QSettings>
#include <QByteArray>
#include <QPalette>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QDockWidget>
#include <QSplitter>
//==========================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(glAppProduct);

    //    QPalette palette = QPalette(Qt::darkBlue);
    //    this->setPalette(palette);
    zv_centralSplitter = 0;
    zv_fileActionManager = 0;
    zv_spectraArrayRepository = 0;
    zv_jointSpectraDataManager = 0;

    zv_arrayModel = 0;
    zv_jointSpectraModel = 0;
    zv_chemElementModel = 0;
    zv_calibrationModel = 0;

    zh_createActions();
    zh_createComponents();
    zh_createMenu();
    zh_createToolbar();
    zh_createConnections();
    zh_restoreSettings();
}
//==========================================================
MainWindow::~MainWindow()
{

}
//==========================================================
void MainWindow::closeEvent(QCloseEvent* e)
{
    QString questionString = tr("Quit the application?");
    if(QMessageBox::question(this, tr("Quit the application"), questionString,
                             QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
    {
        e->ignore();
        return;
    }

    zh_saveSettings();
}
//==========================================================
void MainWindow::zh_createActions()
{
    zv_exitAction = new QAction(this);
    zv_exitAction->setText(tr("Exit"));
    zv_exitAction->setToolTip(tr("Exit the application"));

    zv_aboutAction = new QAction(this);
    zv_aboutAction->setText(tr("About"));
    zv_aboutAction->setToolTip(tr("About the application"));

    zv_helpAction = new QAction(this);
    zv_helpAction->setText(tr("Help"));
    zv_helpAction->setToolTip(tr("Show user guide"));

}
//==========================================================
void MainWindow::zh_createComponents()
{
    // CENTRAL WIDGET
    // Central widget
    zv_centralSplitter = new QSplitter(Qt::Vertical, this);
    setCentralWidget(zv_centralSplitter);

    // Spectrum Array View - central widget
    zv_spectraSidebarWidget = new ZWidgetWithSidebar(tr("Spectra"), "SPECTRUM_SIDEBAR_WIDGET", this);
    zv_spectrumArrayWidget = new ZSpectrumArrayWidget(this);
    zv_spectrumTableWidget = new ZSpectrumTableWidget(this);

    zv_spectraSidebarWidget->zp_setSidebarWidget(zv_spectrumArrayWidget);
    zv_spectraSidebarWidget->zp_setMainWidget(zv_spectrumTableWidget);
    // put into central widget
    zv_centralSplitter->addWidget(zv_spectraSidebarWidget);

    // Plotter
     zv_plotter = new ZPlotter(this);
     zv_centralSplitter->addWidget(zv_plotter);

    zv_centralSplitter->setCollapsible(0, false);
    zv_centralSplitter->setCollapsible(0, false);

    // DOCKS
    // Calibration Array View
    zv_calibrationArrayDock= new QDockWidget(this);
    zv_calibrationArrayDock->setObjectName("CALIBRATION_ARRAY_DOCK");
    zv_calibrationArrayDock->setWindowTitle(tr("Calibrations"));
    zv_dockList << zv_calibrationArrayDock;
    addDockWidget(Qt::LeftDockWidgetArea, zv_calibrationArrayDock);

    zv_calibrationSidebarWidget = new ZWidgetWithSidebar("CALIBRATION_SIDEBAR_WIDGET", this);
    zv_calibrationArrayWidget = new ZChemElementWidget(this);
    zv_calibrationTableWidget = new ZCalibrationTableWidget(this);

    zv_calibrationSidebarWidget->zp_setSidebarWidget(zv_calibrationArrayWidget);
    zv_calibrationSidebarWidget->zp_setMainWidget(zv_calibrationTableWidget);
    // setting to dock
    zv_calibrationArrayDock->setWidget(zv_calibrationSidebarWidget);

    // Message Panel
    zv_messagePanelDock = new QDockWidget(this);
    zv_messagePanelDock->setObjectName("MESSAGE_PANEL_DOCK");
    zv_messagePanelDock->setWindowTitle(tr("Message"));
    zv_dockList << zv_messagePanelDock;
    addDockWidget(Qt::LeftDockWidgetArea, zv_messagePanelDock);

    zv_messagePanel = new ZMessagePanel(this);
    // setting to dock
    zv_messagePanelDock->setWidget(zv_messagePanel);

    // tabblifying docks by default
    // this->tabifyDockWidget(zv_messagePanelDock, zv_calibrationArrayDock);

    // Components
    zv_fileActionManager = new ZFileActionManager(this);
    zv_spectraArrayRepository = new ZSpectraArrayRepository(this);
    zv_jointSpectraDataManager = new ZJointSpectraDataManager(this);

    // Models
     zv_arrayModel = new ZArrayModel(this);
     zv_jointSpectraModel = new ZJointSpectraModel(this);
     zv_chemElementModel = new ZChemElementModel(this);
     zv_calibrationModel = new ZCalibrationModel(this);

}
//==========================================================
void MainWindow::zh_createMenu()
{
    // File
    QMenu * menu = menuBar()->addMenu(tr("File"));
    // menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName("File");
    zv_fileActionManager->zp_appendActionsToMenu(menu);
    zh_appendActionsToMenu(menu);

    // View
    menu = menuBar()->addMenu(tr("View"));
    // menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName("View");
    zh_appendActionsToMenu(menu);

    // Actions
    menu = menuBar()->addMenu(tr("Actions"));
    // menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName("Actions");
    zh_appendActionsToMenu(menu);

    // Help
    menu = menuBar()->addMenu(tr("Help"));
    // menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName("Help");
    zh_appendActionsToMenu(menu);
}
//==========================================================
void MainWindow::zh_createToolbar()
{

}
//==========================================================
void MainWindow::zh_createConnections()
{
    // main window actions
    connect(zv_exitAction, &QAction::triggered,
            this, &MainWindow::close);
    connect(zv_aboutAction, &QAction::triggered,
            this, &MainWindow::zh_onAboutAction);
    connect(zv_helpAction, &QAction::triggered,
            this, &MainWindow::zh_onHelpAction);

    // main window save settings command
    connect(this, &MainWindow::zg_saveSettings,
            zv_spectraSidebarWidget, &ZWidgetWithSidebar::zp_saveSettings);
    connect(this, &MainWindow::zg_saveSettings,
            zv_calibrationSidebarWidget, &ZWidgetWithSidebar::zp_saveSettings);
    connect(this, &MainWindow::zg_saveSettings,
            zv_fileActionManager, &ZFileActionManager::zp_saveSettings);


    // views <-> model and other comps
    zv_spectrumArrayWidget->zp_setModel(zv_arrayModel);
    zv_spectrumTableWidget->zp_setModel(zv_jointSpectraModel);
    connect(zv_spectrumArrayWidget, &ZSpectrumArrayWidget::zg_currentArrayChenged,
            zv_jointSpectraDataManager, &ZJointSpectraDataManager::zp_currentArrayChanged);

    // file action manager <-> array repository
    connect(zv_fileActionManager, &ZFileActionManager::zg_arrayList,
            zv_spectraArrayRepository, &ZSpectraArrayRepository::zp_appendArrays);

    // repository <-> model
    zv_arrayModel->zp_setSpectraArrayRepository(zv_spectraArrayRepository);
    zv_jointSpectraDataManager->zp_setSpectraArrayRepository(zv_spectraArrayRepository);
    zv_jointSpectraModel->zp_setDataManager(zv_jointSpectraDataManager);


    // repository <-> array view
    connect(zv_spectraArrayRepository, &ZSpectraArrayRepository::zg_currentFile,
            zv_spectraSidebarWidget, &ZWidgetWithSidebar::zp_setInfoLabelText);

}
//==========================================================
void MainWindow::zh_appendActionsToMenu(QMenu* menu)
{
    if(menu->objectName() == "File")
    {
        menu->addAction(zv_exitAction);
        menu->addSeparator();
        return;
    }

    if(menu->objectName() == "View")
    {
        foreach(QDockWidget* dock, zv_dockList)
        {
//            QAction* viewAction = new QAction(this);
//            viewAction->setText(dock->windowTitle());
//            viewAction->setCheckable(true);
//            QAction* viewAction = new QAction(this);
//            viewAction->setText(dock->windowTitle());
//            viewAction->setCheckable(true);

//            connect(viewAction, &QAction::toggled,
//                    dock, &QDockWidget::setVisible);
//            connect(dock, &QDockWidget::visibilityChanged,
//                    viewAction, &QAction::setChecked);

            menu->addAction(dock->toggleViewAction());
        }
        menu->addSeparator();
        return;
    }

    if(menu->objectName() == "Help")
    {
        menu->addAction(zv_helpAction);
        menu->addAction(zv_aboutAction);
        menu->addSeparator();
        return;
    }
}
//==========================================================
void MainWindow::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("Common");

    // app geometry
    vData = settings.value("appGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        restoreGeometry(vData.toByteArray());
    }
    // app state
    vData = settings.value("appState");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        restoreState(vData.toByteArray());
    }

    if(zv_centralSplitter)
    {
        vData = settings.value("centralSplitterState");
        if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
        {
            zv_centralSplitter->restoreState(vData.toByteArray());
        }
    }

    settings.endGroup(); // Common
    settings.endGroup(); // glAppVersion
}
//==========================================================
void MainWindow::zh_saveSettings()
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("Common");

    settings.setValue("appGeometry", saveGeometry());
    settings.setValue("appState", saveState());

    // central widget
    if(zv_centralSplitter)
    {
        settings.setValue("centralSplitterState", zv_centralSplitter->saveState());
    }

    settings.endGroup(); // Common
    settings.endGroup(); // glAppVersion

    emit zg_saveSettings();
}
//==========================================================
void MainWindow::zh_onAboutAction()
{
#ifdef DBG
    qDebug() << "ABOUT";
#endif
}
//==========================================================
void MainWindow::zh_onHelpAction()
{
#ifdef DBG
    qDebug() << "HELP";
#endif
}
//==========================================================
