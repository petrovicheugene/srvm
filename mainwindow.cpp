//==========================================================
#include "MainWindow.h"
#include "globalVariables.h"
// components
#include "ZFileActionManager.h"
#include "ZSpectraArrayRepository.h"
#include "ZJointSpectraDataManager.h"
#include "ZJointCalibrationWindowDataManager.h"
#include "ZCalibrationRepository.h"
#include "ZChemElementDataManager.h"
#include "ZPlotterDataManager.h"

// views
#include "ZWidgetWithSidebar.h"
#include "ZSpectrumArrayWidget.h"
#include "ZJointSpectraTableWidget.h"
#include "ZChemElementWidget.h"
#include "ZCalibrationTableWidget.h"
#include "ZJointCalibrationWindowTableWidget.h"
#include "ZPlotter.h"
#include "ZMessagePanel.h"
// models
#include "ZArrayModel.h"
#include "ZCalibrationModel.h"
#include "ZJointCalibrationWindowModel.h"
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
    zv_fileActionManager = 0;
    zv_spectrumArrayRepository = 0;
    zv_jointSpectraDataManager = 0;
    zv_chemElementDataManager = 0;
    zv_jointCalibrationWindowDataManager = 0;
    zv_plotterDataManager = 0;

    zv_arrayModel = 0;
    zv_jointSpectraModel = 0;
    zv_chemElementModel = 0;
    zv_calibrationModel = 0;
    zv_jointCalibrationWindowModel = 0;

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
    // Plotter
    zv_plotter = new ZPlotter(this);
    QFrame* frame = zh_setWidgetToFrame(zv_plotter);
    setCentralWidget(frame);

    // DOCKS
    // Spectrum array dock
    zv_spectrumArrayDock = new QDockWidget(this);
    zv_spectrumArrayDock->setObjectName("SPECTRUM_ARRAY_DOCK");
    zv_spectrumArrayDock->setWindowTitle(tr("Spectra"));
    zv_dockList << zv_spectrumArrayDock;
    addDockWidget(Qt::TopDockWidgetArea, zv_spectrumArrayDock);

    zv_spectraSidebarWidget = new ZWidgetWithSidebar("SPECTRUM_SIDEBAR_WIDGET", this);
    zv_spectraSidebarWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    zv_spectraSidebarWidget->setLineWidth(1);
    zv_spectrumArrayWidget = new ZSpectrumArrayWidget(this);
    zv_spectrumTableWidget = new ZJointSpectrumTableWidget(this);

    zv_spectraSidebarWidget->zp_setSidebarWidget(zv_spectrumArrayWidget);
    zv_spectraSidebarWidget->zp_setMainWidget(zv_spectrumTableWidget);
    // setting to dock
    zv_spectrumArrayDock->setWidget(zv_spectraSidebarWidget);

    // Chem Element Array View
    zv_chemElementArrayDock = new QDockWidget(this);
    zv_chemElementArrayDock->setObjectName("CHEMELEMENT_ARRAY_DOCK");
    zv_chemElementArrayDock->setWindowTitle(tr("Chemical elements"));
    zv_dockList << zv_chemElementArrayDock;
    addDockWidget(Qt::LeftDockWidgetArea, zv_chemElementArrayDock);

    zv_chemElementWidget = new ZChemElementWidget(this);
    frame = zh_setWidgetToFrame(zv_chemElementWidget);
    // setting to dock
    zv_chemElementArrayDock->setWidget(frame);

    // Calibration View
    zv_calibrationDock= new QDockWidget(this);
    zv_calibrationDock->setObjectName("CALIBRATION_DOCK");
    zv_calibrationDock->setWindowTitle(tr("Calibrations"));
    zv_dockList << zv_calibrationDock;
    addDockWidget(Qt::LeftDockWidgetArea, zv_calibrationDock);

    zv_calibrationSidebarWidget = new ZWidgetWithSidebar("CALIBRATION_SIDEBAR_WIDGET", this);
    zv_calibrationSidebarWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    zv_calibrationSidebarWidget->setLineWidth(1);

    zv_calibrationTableWidget = new ZCalibrationTableWidget(this);
    zv_jointCalibrationWindowtableWidget = new ZJointCalibrationWindowTableWidget(this);

    zv_calibrationSidebarWidget->zp_setSidebarWidget(zv_calibrationTableWidget);
    zv_calibrationSidebarWidget->zp_setMainWidget(zv_jointCalibrationWindowtableWidget);

    // setting to dock
    zv_calibrationDock->setWidget(zv_calibrationSidebarWidget);

    // Message Panel
    zv_messagePanelDock = new QDockWidget(this);
    zv_messagePanelDock->setObjectName("MESSAGE_PANEL_DOCK");
    zv_messagePanelDock->setWindowTitle(tr("Message"));
    zv_dockList << zv_messagePanelDock;
    addDockWidget(Qt::LeftDockWidgetArea, zv_messagePanelDock);

    zv_messagePanel = new ZMessagePanel(this);
    frame = zh_setWidgetToFrame(zv_messagePanel);
    // setting to dock
    zv_messagePanelDock->setWidget(frame);

    // tabblifying docks by default
    // this->tabifyDockWidget(zv_messagePanelDock, zv_calibrationArrayDock);

    // Components
    zv_fileActionManager = new ZFileActionManager(this);
    zv_spectrumArrayRepository = new ZSpectrumArrayRepository(this);
    zv_jointSpectraDataManager = new ZJointSpectraDataManager(this);
    zv_chemElementDataManager = new ZChemElementDataManager(this);
    zv_calibrationRepository = new ZCalibrationRepository(this);
    zv_jointCalibrationWindowDataManager = new ZJointCalibrationWindowDataManager(this);

    // Models
    zv_arrayModel = new ZArrayModel(this);
    zv_jointSpectraModel = new ZJointSpectraModel(this);
    zv_chemElementModel = new ZChemElementModel(this);
    zv_calibrationModel = new ZCalibrationModel(this);
    zv_jointCalibrationWindowModel = new ZJointCalibrationWindowModel(this);
    zv_plotterDataManager = new ZPlotterDataManager(this);

    statusBar();
}
//==========================================================
QFrame* MainWindow::zh_setWidgetToFrame(QWidget* widget)
{
    QFrame* frame = new QFrame();
    QVBoxLayout* frameLayout = new QVBoxLayout(frame);
    frame->setLayout(frameLayout);
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    frame->setLineWidth(1);

    frameLayout->addWidget(widget);
    return frame;
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
    zv_spectrumArrayRepository->zp_appendActionsToMenu(menu);
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


    // views set models
    zv_spectrumArrayWidget->zp_setModel(zv_arrayModel);
    zv_spectrumTableWidget->zp_setModel(zv_jointSpectraModel);
    zv_calibrationTableWidget->zp_setModel(zv_calibrationModel);
    zv_chemElementWidget->zp_setModel(zv_chemElementModel);
    zv_jointCalibrationWindowtableWidget->zp_setModel(zv_jointCalibrationWindowModel);

    // current array changing connection
    //    connect(zv_spectrumArrayWidget, &ZSpectrumArrayWidget::zg_currentArrayChanged,
    //            zv_jointSpectraDataManager, &ZJointSpectraDataManager::zp_currentArrayChanged);
    //    connect(zv_spectrumArrayWidget, &ZSpectrumArrayWidget::zg_currentArrayChanged,
    //            zv_chemElementDataManager, &ZChemElementDataManager::zp_currentArrayChanged);
    //    connect(zv_spectrumArrayWidget, &ZSpectrumArrayWidget::zg_currentArrayChanged,
    //            zv_plotterDataManager, &ZPlotterDataManager::zp_currentArrayChanged);
    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_currentArrayId,
            zv_jointSpectraDataManager, &ZJointSpectraDataManager::zp_currentArrayChanged);
    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_currentArrayId,
            zv_chemElementDataManager, &ZChemElementDataManager::zp_currentArrayChanged);
    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_currentArrayId,
            zv_plotterDataManager, &ZPlotterDataManager::zp_currentArrayChanged);

    connect(zv_spectrumArrayWidget, &ZSpectrumArrayWidget::zg_currentArrayChanged,
            zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zp_currentArrayChanged);


    // File Action Manager <-> other components
    zv_calibrationRepository->zp_connectToFileManager(zv_fileActionManager);
    zv_spectrumArrayRepository->zp_connectToFileActionManager(zv_fileActionManager);

    // spectra repository <-> spectra model and array model and plotter model
    // spectra
    zv_arrayModel->zp_connectToSpectraArrayRepository(zv_spectrumArrayRepository);
    zv_jointSpectraDataManager->zp_connectToSpectraArrayRepository(zv_spectrumArrayRepository);
    zv_jointSpectraDataManager->zp_connectToCalibrationRepository(zv_calibrationRepository);
    zv_jointSpectraModel->zp_connectToSpectraDataManager(zv_jointSpectraDataManager);
    // plotter
    zv_plotterDataManager->zp_connectToSpectraArrayRepository(zv_spectrumArrayRepository);
    zv_plotterDataManager->zp_connectToPlotter(zv_plotter);
    // chem element
    zv_chemElementDataManager->zp_connectToSpectraArrayRepository(zv_spectrumArrayRepository);
    zv_chemElementModel->zp_connectToChemElementDataManager(zv_chemElementDataManager);

    // spectra repository <-> array view
    zv_spectrumArrayWidget->zp_appendButtonActions(zv_spectrumArrayRepository->zp_arrayActions());
    zv_spectrumTableWidget->zp_appendButtonActions(zv_spectrumArrayRepository->zp_spectrumActions());
    zv_chemElementWidget->zp_appendButtonActions(zv_spectrumArrayRepository->zp_chemElementActions());

    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_currentFile,
            zv_spectraSidebarWidget, &ZWidgetWithSidebar::zp_setInfoLabelText);
    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_setCurrentArrayIndex,
            zv_spectrumArrayWidget, &ZSpectrumArrayWidget::zp_setCurrentArrayIndex);
    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_startCurrentArrayEdition,
            zv_spectrumArrayWidget, &ZSpectrumArrayWidget::zp_startCurrentArrayEdition);

    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_setCurrentChemElementIndex,
            zv_chemElementWidget, &ZChemElementWidget::zp_setCurrentChemElementIndex);
    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_startCurrentChemElementEdition,
            zv_chemElementWidget, &ZChemElementWidget::zp_startCurrentChemElementEdition);

    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_requestCurrentArrayIndex,
            zv_spectrumArrayWidget, &ZSpectrumArrayWidget::zp_currentArrayIndex);

    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_fitPlotInBoundingRect,
            zv_plotter, &ZPlotter::zp_fitInBoundingRect);


    // spectra repository <-> joint spectrum view
    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_requestSelectedSpectrumIndexList,
            zv_spectrumTableWidget, &ZJointSpectrumTableWidget::zp_selectedSpectrumIndexList);

    // spectra repository <-> chemical element view
    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_requestSelectedChemElementIndexList,
            zv_chemElementWidget, &ZChemElementWidget::zp_selectedChemElementIndexList);

    // calibration view <-> zv_spectraArrayRepository
    connect(zv_calibrationTableWidget, &ZCalibrationTableWidget::zg_requestChemElementList,
            zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zp_chemElementListForCurrentArray);


    // calibration repository <-> calibration model
    zv_calibrationModel->zp_connectToCalibrationRepository(zv_calibrationRepository);

    // joint calibration window data manager <-> to repositories
    zv_jointCalibrationWindowDataManager->zp_connectToCalibrationRepository(zv_calibrationRepository);
    zv_jointCalibrationWindowDataManager->zp_connectToSpectraArrayRepository(zv_spectrumArrayRepository);

    // calibration window model to <-> joint calibration window data manager
    zv_jointCalibrationWindowModel->zp_connectToJointCalibrationWindowDataManager(zv_jointCalibrationWindowDataManager);


    // calibration repository
    zv_calibrationTableWidget->zp_appendButtonActions(zv_calibrationRepository->zp_arrayActions());
    zv_jointCalibrationWindowtableWidget->zp_appendButtonActions(zv_calibrationRepository->zp_windowActions());
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_requestSelectedCalibrationIndexList,
            zv_calibrationTableWidget, &ZCalibrationTableWidget::zp_selectedCalibrationIndexList);
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_requestCurrentCalibrationIndex,
            zv_calibrationTableWidget, &ZCalibrationTableWidget::zp_currentCalibrationIndex);
    connect(zv_calibrationTableWidget, &ZCalibrationTableWidget::zg_currentCalibrationChanged,
            zv_calibrationRepository, &ZCalibrationRepository::zp_onCurrentCalibrationChanged);
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_setCurrentCalibrationIndex,
            zv_calibrationTableWidget, &ZCalibrationTableWidget::zp_setCurrentCalibrationIndex);
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_startCurrentCalibrationEdition,
            zv_calibrationTableWidget, &ZCalibrationTableWidget::zp_startCurrentCalibrationEdition);




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
