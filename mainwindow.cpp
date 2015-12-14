//==========================================================
#include "MainWindow.h"
#include "globalVariables.h"
// components
#include "ZFileActionManager.h"
#include "ZSpectraArrayRepository.h"
#include "ZJointSpectraDataManager.h"
#include "ZCalibrationRepository.h"
#include "ZChemElementDataManager.h"
#include "ZPlotterDataManager.h"

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
// qt
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
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
#include <QStatusBar>
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
    zv_chemElementDataManager = 0;

    zv_arrayModel = 0;
    zv_jointSpectraModel = 0;
    zv_chemElementModel = 0;
    zv_calibrationModel = 0;
    zv_plotterDataManager = 0;

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
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* cwLayout = new QVBoxLayout(this);
    cwLayout->setMargin(2);
    centralWidget->setLayout(cwLayout);
    zv_centralSplitter = new QSplitter(Qt::Vertical, this);
    cwLayout->addWidget(zv_centralSplitter);
    setCentralWidget(centralWidget);

    // Spectrum Array View - central widget
    zv_spectraSidebarWidget = new ZWidgetWithSidebar(tr("Spectra"), "SPECTRUM_SIDEBAR_WIDGET", this);
    zv_spectraSidebarWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    zv_spectraSidebarWidget->setLineWidth(1);
    zv_spectrumArrayWidget = new ZSpectrumArrayWidget(this);
    zv_spectrumTableWidget = new ZJointSpectrumTableWidget(this);

    zv_spectraSidebarWidget->zp_setSidebarWidget(zv_spectrumArrayWidget);
    zv_spectraSidebarWidget->zp_setMainWidget(zv_spectrumTableWidget);
    // put into central widget
    zv_centralSplitter->addWidget(zv_spectraSidebarWidget);

    // Plotter
    zv_plotter = new ZPlotter(this);
    QFrame* frameWidget = new QFrame(this);
    QVBoxLayout* plotterLayout = new QVBoxLayout(frameWidget);
    frameWidget->setLayout(plotterLayout);
    frameWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    frameWidget->setLineWidth(1);
    plotterLayout->addWidget(zv_plotter);
    zv_centralSplitter->addWidget(frameWidget);

    zv_centralSplitter->setCollapsible(0, false);
    zv_centralSplitter->setCollapsible(0, false);

    // DOCKS
    // Chem Element Array View
    zv_chemElementArrayDock = new QDockWidget(this);
    zv_chemElementArrayDock->setObjectName("CHEMELEMENT_ARRAY_DOCK");
    zv_chemElementArrayDock->setWindowTitle(tr("Chemical elements"));
    zv_dockList << zv_chemElementArrayDock;
    addDockWidget(Qt::LeftDockWidgetArea, zv_chemElementArrayDock);

    // zv_calibrationSidebarWidget = new ZWidgetWithSidebar("CALIBRATION_SIDEBAR_WIDGET", this);
    zv_chemElementWidget = new ZChemElementWidget(this);
    // setting to dock
    zv_chemElementArrayDock->setWidget(zv_chemElementWidget);

    // Calibration View
    zv_calibrationArrayDock= new QDockWidget(this);
    zv_calibrationArrayDock->setObjectName("CALIBRATION_ARRAY_DOCK");
    zv_calibrationArrayDock->setWindowTitle(tr("Calibrations"));
    zv_dockList << zv_calibrationArrayDock;
    addDockWidget(Qt::LeftDockWidgetArea, zv_calibrationArrayDock);

    zv_calibrationTableWidget = new ZCalibrationTableWidget(this);
    // setting to dock
    zv_calibrationArrayDock->setWidget(zv_calibrationTableWidget);

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
    zv_chemElementDataManager = new ZChemElementDataManager(this);
    zv_calibrationRepository = new ZCalibrationRepository(this);

    // Models
    zv_arrayModel = new ZArrayModel(this);
    zv_jointSpectraModel = new ZJointSpectraModel(this);
    zv_chemElementModel = new ZChemElementModel(this);
    zv_calibrationModel = new ZCalibrationModel(this);
    zv_plotterDataManager = new ZPlotterDataManager(this);

    statusBar();
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

    // Edit
    menu = menuBar()->addMenu(tr("Edit"));
    // menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName("Edit");
    zv_spectraArrayRepository->zp_appendActionsToMenu(menu);
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
            zv_fileActionManager, &ZFileActionManager::zp_saveSettings);


    // views set models
    zv_spectrumArrayWidget->zp_setModel(zv_arrayModel);
    zv_spectrumTableWidget->zp_setModel(zv_jointSpectraModel);
    zv_calibrationTableWidget->zp_setModel(zv_calibrationModel);
    zv_chemElementWidget->zp_setModel(zv_chemElementModel);

    // current array changing connection
    //    connect(zv_spectrumArrayWidget, &ZSpectrumArrayWidget::zg_currentArrayChanged,
    //            zv_jointSpectraDataManager, &ZJointSpectraDataManager::zp_currentArrayChanged);
    //    connect(zv_spectrumArrayWidget, &ZSpectrumArrayWidget::zg_currentArrayChanged,
    //            zv_chemElementDataManager, &ZChemElementDataManager::zp_currentArrayChanged);
    //    connect(zv_spectrumArrayWidget, &ZSpectrumArrayWidget::zg_currentArrayChanged,
    //            zv_plotterDataManager, &ZPlotterDataManager::zp_currentArrayChanged);
    connect(zv_spectraArrayRepository, &ZSpectraArrayRepository::zg_currentArrayId,
            zv_jointSpectraDataManager, &ZJointSpectraDataManager::zp_currentArrayChanged);
    connect(zv_spectraArrayRepository, &ZSpectraArrayRepository::zg_currentArrayId,
            zv_chemElementDataManager, &ZChemElementDataManager::zp_currentArrayChanged);
    connect(zv_spectraArrayRepository, &ZSpectraArrayRepository::zg_currentArrayId,
            zv_plotterDataManager, &ZPlotterDataManager::zp_currentArrayChanged);

    connect(zv_spectrumArrayWidget, &ZSpectrumArrayWidget::zg_currentArrayChanged,
            zv_spectraArrayRepository, &ZSpectraArrayRepository::zp_currentArrayChanged);


    // File Action Manager <-> other components
    zv_calibrationRepository->zp_connectToFileManager(zv_fileActionManager);
    zv_spectraArrayRepository->zp_connectToFileActionManager(zv_fileActionManager);

    // spectra repository <-> spectra model and array model and plotter model
    // spectra
    zv_arrayModel->zp_connectToSpectraArrayRepository(zv_spectraArrayRepository);
    zv_jointSpectraDataManager->zp_connectToSpectraArrayRepository(zv_spectraArrayRepository);
    zv_jointSpectraDataManager->zp_connectToCalibrationRepository(zv_calibrationRepository);
    zv_jointSpectraModel->zp_connectToSpectraDataManager(zv_jointSpectraDataManager);
    // plotter
    zv_plotterDataManager->zp_connectToSpectraArrayRepository(zv_spectraArrayRepository);
    zv_plotterDataManager->zp_connectToPlotter(zv_plotter);
    // chem element
    zv_chemElementDataManager->zp_connectToSpectraArrayRepository(zv_spectraArrayRepository);
    zv_chemElementModel->zp_connectToChemElementDataManager(zv_chemElementDataManager);

    // spectra repository <-> array view
    zv_spectrumArrayWidget->zp_appendButtonActions(zv_spectraArrayRepository->zp_arrayActions());
    zv_spectrumTableWidget->zp_appendButtonActions(zv_spectraArrayRepository->zp_spectrumActions());
    zv_chemElementWidget->zp_appendButtonActions(zv_spectraArrayRepository->zp_chemElementActions());

    connect(zv_spectraArrayRepository, &ZSpectraArrayRepository::zg_currentFile,
            zv_spectraSidebarWidget, &ZWidgetWithSidebar::zp_setInfoLabelText);
    connect(zv_spectraArrayRepository, &ZSpectraArrayRepository::zg_setCurrentArrayIndex,
            zv_spectrumArrayWidget, &ZSpectrumArrayWidget::zp_setCurrentArrayIndex);
    connect(zv_spectraArrayRepository, &ZSpectraArrayRepository::zg_startCurrentArrayEdition,
            zv_spectrumArrayWidget, &ZSpectrumArrayWidget::zp_startCurrentArrayEdition);

    connect(zv_spectraArrayRepository, &ZSpectraArrayRepository::zg_setCurrentChemElementIndex,
            zv_chemElementWidget, &ZChemElementWidget::zp_setCurrentChemElementIndex);
    connect(zv_spectraArrayRepository, &ZSpectraArrayRepository::zg_startCurrentChemElementEdition,
            zv_chemElementWidget, &ZChemElementWidget::zp_startCurrentChemElementEdition);

    connect(zv_spectraArrayRepository, &ZSpectraArrayRepository::zg_requestCurrentArrayIndex,
            zv_spectrumArrayWidget, &ZSpectrumArrayWidget::zp_currentArrayIndex);

    connect(zv_spectraArrayRepository, &ZSpectraArrayRepository::zg_arrayFileLoaded,
            zv_plotter, &ZPlotter::zp_fitInBoundingRect);


    // spectra repository <-> joint spectrum view
    connect(zv_spectraArrayRepository, &ZSpectraArrayRepository::zg_requestSelectedSpectrumIndexList,
            zv_spectrumTableWidget, &ZJointSpectrumTableWidget::zp_selectedSpectrumIndexList);

    // calibration repository <-> chemical element view
    connect(zv_spectraArrayRepository, &ZSpectraArrayRepository::zg_requestSelectedChemElementIndexList,
            zv_chemElementWidget, &ZChemElementWidget::zp_selectedChemElementIndexList);


    // calibration repository <-> calibration model
    zv_calibrationModel->zp_connectToCalibrationRepository(zv_calibrationRepository);

    // calibration repository <-> calibration view
    zv_calibrationTableWidget->zp_appendButtonActions(zv_calibrationRepository->zp_arrayActions());
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_requestSelectedCalibrationIndexList,
            zv_calibrationTableWidget, &ZCalibrationTableWidget::zp_selectedCalibrationIndexList);
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_requestCurrentCalibrationIndex,
            zv_calibrationTableWidget, &ZCalibrationTableWidget::zp_currentCalibrationIndex);
    connect(zv_calibrationTableWidget, &ZCalibrationTableWidget::zg_currentCalibrationChanged,
            zv_calibrationRepository, &ZCalibrationRepository::zp_onCurrentCalibrationChanged);

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
