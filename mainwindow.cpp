//==========================================================
#include "MainWindow.h"
#include "ZGeneral.h"
#include <QApplication>
#include <QDir>
// components
#include "ZFileActionManager.h"
#include "ZSpectrumArrayRepository.h"
#include "ZJointSpectraDataManager.h"
#include "ZCalibrationWindowDataManager.h"
#include "ZCalibrationRepository.h"
#include "ZChemElementDataManager.h"
#include "ZPlotterDataManager.h"
#include "ZCorrelationPlotterDataManager.h"
#include "ZTermCorrelationTableManager.h"
#include "ZSpeSpectrum.h"
#include "ZEquationSettingsData.h"
#include "ZHelpBrowser.h"
#include "ZStandardMessagePictureDispatcher.h"
#include "ZTranslatorManager.h"

// views
#include "ZWidgetWithSidebar.h"
#include "ZSpectrumArrayWidget.h"
#include "ZJointSpectraTableWidget.h"
#include "ZChemElementWidget.h"
#include "ZCalibrationTableWidget.h"
#include "ZCalibrationWindowTableWidget.h"
#include "ZCorrelationPlotterWidget.h"
#include "ZTermCorrelationTableWidget.h"
#include "ZPlotter.h"
#include "ZMessagePanel.h"
#include "ZMessageWidget.h"
#include "ZEnergyLineTableWidget.h"
// models
#include "ZArrayModel.h"
#include "ZCalibrationModel.h"
#include "ZCalibrationWindowModel.h"
#include "ZChemElementModel.h"
#include "ZJointSpectraModel.h"
#include "ZTermCorrelationTableModel.h"

// qt
#include <QApplication>
#include <QAction>
#include <QByteArray>
#include <QCloseEvent>
#include <QDebug>
#include <QDockWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QMetaType>
#include <QPalette>
#include <QStatusBar>
#include <QToolBar>
#include <QSettings>
#include <QSplitter>
#include <QStatusBar>
#include <QStyle>
#include <QVBoxLayout>

//==========================================================
MainWindow::MainWindow(QWidget *parent)
    : ZBaseMainWindow(parent)
{
    qRegisterMetaType<ZEquationSettingsData>("ZEquationSettingsData");
    //    qRegisterMetaType<ZSpeSpectrum>("ZSpeSpectrum");

    setWindowTitle(qApp->applicationDisplayName());

    //    QPalette palette = QPalette(Qt::darkBlue);
    //    this->setPalette(palette);
    mv_helpBrowser = nullptr;
    zv_fileActionManager = nullptr;
    zv_spectrumArrayRepository = nullptr;
    zv_calibrationRepository = nullptr;
    zv_jointSpectraDataManager = nullptr;
    zv_chemElementDataManager = nullptr;
    zv_jointCalibrationWindowDataManager = nullptr;
    zv_plotterDataManager = nullptr;
    zv_calculationPlotterManager = nullptr;
    zv_termCorrelationTableManager = nullptr;
    zv_energyLineTableWidget = nullptr;

    zv_arrayModel = nullptr;
    zv_jointSpectraModel = nullptr;
    zv_chemElementModel = nullptr;
    zv_calibrationModel = nullptr;
    zv_jointCalibrationWindowModel = nullptr;
    zv_termCorrelationTableModel = nullptr;
    zv_plotter = nullptr;

    zh_createActions();
    zh_createComponents();
    zh_createPlotterWidget();

    zh_createMenu();
    zh_createToolbar();
    zh_createConnections();

    zh_restoreSettings();

    // plotter starting settings
    if(zv_plotter != nullptr)
    {
        QMetaObject::invokeMethod(zv_plotter, "zp_fitInBoundingRect",
                                  Qt::QueuedConnection);
    }
}
//==========================================================
MainWindow::~MainWindow()
{

}
//==========================================================
void MainWindow::closeEvent(QCloseEvent* e)
{
    //    QString questionString = tr("Quit the application?");
    //    if(QMessageBox::question(this, tr("Quit the application"), questionString,
    //                             QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
    //    {
    //        e->ignore();
    //        return;
    //    }

    // check diry of the spectrum array repo
    if(zv_spectrumArrayRepository->zp_isDirty())
    {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("The spectrum array list has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.resize(msgBox.sizeHint());
        int res = msgBox.exec();

        switch (res)
        {
        case QMessageBox::Yes:
            zv_fileActionManager->zp_triggerSaveArrayToFileAction();
            break;
        case QMessageBox::No:
            // Don't Save was clicked
            break;
        case QMessageBox::Cancel:
        default:
            e->ignore();
            return;
        }
    }

    // check dirty of the calibration repo
    QList<int> dirtyRowList;
    zv_calibrationRepository->zp_hasDirtyCalibrations(dirtyRowList);
    if(!dirtyRowList.isEmpty())
    {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("There are calibrations that have been modified."));
        msgBox.setInformativeText(tr("Do you want to save them?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int res = msgBox.exec();

        switch (res)
        {
        case QMessageBox::Yes:
            for(int i = 0; i < dirtyRowList.count(); i++ )
            {
                zv_calibrationTableWidget->zp_setCurrentCalibrationIndex(dirtyRowList.at(i));
                zv_fileActionManager->zp_triggerSaveCalibrationsToFile();
            }

            break;
        case QMessageBox::No:
            // Don't Save was clicked
            break;
        case QMessageBox::Cancel:
        default:
            e->ignore();
            return;
        }
    }

    zh_saveSettings();
}
//==========================================================
void MainWindow::zh_createActions()
{
    //    zv_languageControlAction = new QAction(this);
    //    zv_languageControlAction->setIcon(QIcon(":/images/ZImages/earthGlobe-16.png"));
    //    zv_languageControlAction->setText(tr("Language"));
    //    zv_languageControlAction->setToolTip(tr("Application language"));

    zv_exitAction = new QAction(this);
    zv_exitAction->setIcon(QIcon(NS_Icons::glIconExitApp));
    zv_exitAction->setText(tr("Exit"));
    zv_exitAction->setToolTip(tr("Exit the application"));

    //    zv_aboutAction = new QAction(this);
    zv_aboutAction->setIcon(QIcon(NS_Icons::glIconAbout));
    //    zv_aboutAction->setText(tr("About"));
    //    zv_aboutAction->setToolTip(tr("About the application"));
    zv_aboutQtAction->setIcon(QIcon(NS_Icons::glIconQt));
    //    zv_helpAction = new QAction(this);
    zv_helpAction->setIcon(QIcon(NS_Icons::glIconHelp));
    //    zv_helpAction->setText(tr("Help"));
    //    zv_helpAction->setToolTip(tr("Show user guide"));

}
//==========================================================
void MainWindow::zh_createPlotterWidget()
{
    // Plotter
    if(zv_plotter)
    {
        zv_plotter->deleteLater();
    }

    zv_plotter = new ZPlotter(this);
    QFrame* frame = zh_setWidgetToFrame(zv_plotter);
    setCentralWidget(frame);

    if(zv_plotterDataManager)
    {
        zv_plotterDataManager->zp_connectToPlotter(zv_plotter);
    }

    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_fitPlotInBoundingRect,
            zv_plotter, &ZPlotter::zp_fitInBoundingRect);
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_requestCurrentVisibleSceneRect,
            zv_plotter, &ZPlotter::zp_currentVisibleSceneRect);

}
//==========================================================
void MainWindow::zh_appLanguageControl()
{
    if(!sender())
    {
        return;
    }

    QAction* action = dynamic_cast<QAction*>(sender());
    if(!action)
    {
        return;
    }

    ZTranslatorManager translatorManager;
    QString currentLanguageName;
    bool ok = false;
    translatorManager.zp_currentLanguageName(currentLanguageName, &ok);

    qDebug() << "Current lang" << currentLanguageName << "Installied lang" << action->text();

    if(ok && action->text() == currentLanguageName)
    {
        return;
    }

    ok = false;
    translatorManager.zp_setApplicationLanguage(action->text(), &ok);
    QString msg;
    if(ok)
    {
        msg = tr("The language of application has been changed.\n"
                 "In order for the changes to take effect, please restart the application.");
    }
    else
    {
        msg = translatorManager.zp_lastError().isEmpty() ?
                    tr("Unknown language settings error.") : translatorManager.zp_lastError();

    }

    QString title = tr("Language settings");
    QMessageBox::information(this, title, msg, QMessageBox::Ok);
}
//==========================================================
void MainWindow::zh_fillLanguageMenu()
{
    zv_languageMenu->clear();
    ZTranslatorManager translatorManager;
    QStringList availableLanguageNameList;
    translatorManager.zp_availableLanguageNameList(availableLanguageNameList);

    foreach(QString languageName, availableLanguageNameList)
    {
        QAction* action = zv_languageMenu->addAction(languageName);
        connect(action, &QAction::triggered,
                this, &MainWindow::zh_appLanguageControl);
    }
}
//==========================================================
void MainWindow::zh_createComponents()
{
    // CENTRAL WIDGET
    //    // Plotter
    //    zv_plotter = new ZPlotter(this);
    //    QFrame* frame = zh_setWidgetToFrame(zv_plotter);
    //    setCentralWidget(frame);

    // DOCKS
    // Spectrum array dock
    zv_spectrumArrayDock = new QDockWidget(this);
    zv_spectrumArrayDock->setObjectName("SPECTRUM_ARRAY_DOCK");
    zv_spectrumArrayDock->setWindowTitle(tr("Spectra"));
    zv_dockList << zv_spectrumArrayDock;
    addDockWidget(Qt::TopDockWidgetArea, zv_spectrumArrayDock);

    zv_spectraSidebarWidget = new ZWidgetWithSidebar("SPECTRUM_SIDEBAR_WIDGET", true, this);
    zv_spectraSidebarWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    zv_spectraSidebarWidget->setLineWidth(1);
    zv_spectrumArrayWidget = new ZSpectrumArrayWidget(this);
    zv_chemElementWidget = new ZChemElementWidget(this);
    zv_spectrumTableWidget = new ZJointSpectrumTableWidget(this);

    QSplitter* spectrumArraySplitter = new QSplitter(Qt::Vertical, this);
    spectrumArraySplitter->addWidget(zv_spectrumArrayWidget);
    spectrumArraySplitter->addWidget(zv_chemElementWidget);
    spectrumArraySplitter->setChildrenCollapsible(false);

    zv_spectraSidebarWidget->zp_setSidebarWidget(spectrumArraySplitter);
    zv_spectraSidebarWidget->zp_setMainWidget(zv_spectrumTableWidget);
    // setting to dock

    zv_spectrumArrayDock->setWidget(zv_spectraSidebarWidget);

    // Calibration View
    zv_calibrationDock = new QDockWidget(this);
    zv_calibrationDock->setObjectName("CALIBRATION_DOCK");
    zv_calibrationDock->setWindowTitle(tr("Calibrations"));
    zv_dockList << zv_calibrationDock;
    addDockWidget(Qt::TopDockWidgetArea, zv_calibrationDock);

    zv_calibrationSidebarWidget = new ZWidgetWithSidebar("CALIBRATION_SIDEBAR_WIDGET", false, this);
    zv_calibrationSidebarWidget->zp_setMargin(0);
    // zv_calibrationSidebarWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    zv_calibrationSidebarWidget->setFrameShape(QFrame::NoFrame);
    zv_calibrationSidebarWidget->setLineWidth(0);

    zv_calibrationTableWidget = new ZCalibrationTableWidget(this);
    zv_termCorrelationTableWidget = new ZTermCorrelationTableWidget(this);
    zv_calibrationWindowTableWidget = new ZCalibrationWindowTableWidget(this);
    //zv_equationSettingsPanelWidget = new ZEquationSettingsDashBoard(this);

    // sideWidget composition
    zv_calibrationSidebarWidget->zp_setSidebarWidget(zv_calibrationWindowTableWidget);
    zv_calibrationSidebarWidget->zp_setMainWidget(zv_calibrationTableWidget);

    // left side widget
    QWidget* calibrationWidget = new QWidget();
    QVBoxLayout*leftWidgetLayout = new QVBoxLayout;
    leftWidgetLayout->setMargin(0);
    calibrationWidget->setLayout(leftWidgetLayout);

    QSplitter* calibrationSplitter = new QSplitter(Qt::Vertical, this);
    calibrationSplitter->setFrameShape(QFrame::NoFrame);
    calibrationSplitter->addWidget(zv_calibrationSidebarWidget);
    calibrationSplitter->addWidget(zv_termCorrelationTableWidget);
    calibrationSplitter->setChildrenCollapsible(false);
    leftWidgetLayout->addWidget(calibrationSplitter);

    //frame = zh_setWidgetToFrame(zv_equationSettingsPanelWidget);
    //frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //leftWidgetLayout->addWidget(frame);

    // setting to dock
    QFrame* frame = zh_setWidgetToFrame(calibrationWidget);
    zv_calibrationDock->setWidget(frame);

    // Correlation Plotter View
    zv_correlationPlotterDock= new QDockWidget(this);
    zv_correlationPlotterDock->setObjectName("PLOT_DOCK");
    zv_correlationPlotterDock->setWindowTitle(tr("Plot"));
    zv_dockList << zv_correlationPlotterDock;
    addDockWidget(Qt::TopDockWidgetArea, zv_correlationPlotterDock);

    zv_correlationPlotterWidget = new ZCorrelationPlotterWidget(this);

    // setting to dock
    zv_correlationPlotterDock->setWidget(zv_correlationPlotterWidget);

    // Message Panel
    zv_messagePanelDock = new QDockWidget(this);
    zv_messagePanelDock->setObjectName("MESSAGE_PANEL_DOCK");
    zv_messagePanelDock->setWindowTitle(tr("Message"));
    zv_dockList << zv_messagePanelDock;
    addDockWidget(Qt::LeftDockWidgetArea, zv_messagePanelDock);

    zv_messageWidget = new ZMessageWidget(this);
    frame = zh_setWidgetToFrame(zv_messageWidget);
    // setting to dock
    zv_messagePanelDock->setWidget(frame);

    // energy line table
    zv_energyLineTableDock = new QDockWidget(this);
    zv_energyLineTableDock->setObjectName("ENERGY_LINE_TABLE_DOCK");
    zv_energyLineTableDock->setWindowTitle(tr("Energy lines"));
    zv_dockList << zv_energyLineTableDock;
    addDockWidget(Qt::RightDockWidgetArea, zv_energyLineTableDock);

    // create widget
    zv_energyLineTableWidget = new ZEnergyLineTableWidget(this);
    zv_energyLineTableWidget->show();
    frame = zh_setWidgetToFrame(zv_energyLineTableWidget);

    // setting to widget dock
    zv_energyLineTableDock->setWidget(frame);


    // tabblifying docks by default
    // this->tabifyDockWidget(zv_messagePanelDock, zv_calibrationArrayDock);

    // Components
    zv_fileActionManager = new ZFileActionManager(this);
    zv_spectrumArrayRepository = new ZSpectrumArrayRepository(this);
    zv_jointSpectraDataManager = new ZJointSpectraDataManager(this);
    zv_chemElementDataManager = new ZChemElementDataManager(this);
    zv_calibrationRepository = new ZCalibrationRepository(this);
    zv_jointCalibrationWindowDataManager = new ZCalibrationWindowDataManager(this);
    zv_plotterDataManager = new ZPlotterDataManager(this);
    zv_calculationPlotterManager = new ZCorrelationPlotterDataManager(this);
    zv_termCorrelationTableManager = new ZTermCorrelationTableManager(this);

    // Models
    zv_arrayModel = new ZArrayModel(this);
    zv_jointSpectraModel = new ZJointSpectraModel(this);
    zv_chemElementModel = new ZChemElementModel(this);
    zv_calibrationModel = new ZCalibrationModel(this);
    zv_jointCalibrationWindowModel = new ZCalibrationWindowModel(this);
    zv_termCorrelationTableModel = new ZTermCorrelationTableModel(this);

    statusBar();
}
//==========================================================
QFrame* MainWindow::zh_setWidgetToFrame(QWidget* widget)
{
    QFrame* frame = new QFrame();
    QVBoxLayout* frameLayout = new QVBoxLayout;
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
    zv_spectrumArrayRepository->zp_appendActionsToMenu(menu);
    zv_calibrationRepository->zp_appendActionsToMenu(menu);
    zh_appendActionsToMenu(menu);

    // Edit
    menu = menuBar()->addMenu(tr("Edit"));
    // menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName("Edit");
    zv_spectrumArrayRepository->zp_appendActionsToMenu(menu);
    zv_calibrationRepository->zp_appendActionsToMenu(menu);
    zh_appendActionsToMenu(menu);

    // View
    menu = menuBar()->addMenu(tr("View"));
    // menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName("View");
    zv_spectrumArrayRepository->zp_appendActionsToMenu(menu);
    zv_calibrationRepository->zp_appendActionsToMenu(menu);
    zh_appendActionsToMenu(menu);

    // Actions
    menu = menuBar()->addMenu(tr("Actions"));
    // menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName("Actions");
    zv_spectrumArrayRepository->zp_appendActionsToMenu(menu);
    zv_calibrationRepository->zp_appendActionsToMenu(menu);
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

    connect(this, &MainWindow::zg_standardLogMessage,
            zv_messageWidget, &ZMessageWidget::zp_appendMessage, Qt::QueuedConnection);
    connect(this, &MainWindow::zg_plainLogMessage,
            zv_messageWidget, &ZMessageWidget::zp_appendPlainMessage, Qt::QueuedConnection);
    connect(zv_messageWidget, &ZMessageWidget::zg_requestMessageIconPixmap,
            this, &MainWindow::zh_messageIconPixmap);


    // main window actions
    connect(zv_exitAction, &QAction::triggered,
            this, &MainWindow::close);
    //    connect(zv_languageControlAction, &QAction::triggered,
    //            this, &MainWindow::zh_appLanguageControl);
    connect(zv_languageMenu, &QMenu::aboutToShow,
            this, &MainWindow::zh_fillLanguageMenu);

    //    connect(zv_aboutAction, &QAction::triggered,
    //            this, &MainWindow::zh_onAboutAction);
    //    connect(zv_helpAction, &QAction::triggered,
    //            this, &MainWindow::zh_onHelpAction);

    // main window save settings command
    connect(this, &MainWindow::zg_saveSettings,
            zv_spectraSidebarWidget, &ZWidgetWithSidebar::zp_saveSettings);
    connect(this, &MainWindow::zg_saveSettings,
            zv_calibrationSidebarWidget, &ZWidgetWithSidebar::zp_saveSettings);
    //    connect(this, &MainWindow::zg_saveSettings,
    //            zv_termCorrelationTableSidebarWidget, &ZWidgetWithSidebar::zp_saveSettings);

    connect(this, &MainWindow::zg_saveSettings,
            zv_fileActionManager, &ZFileActionManager::zp_saveSettings);

    // views set models
    zv_spectrumArrayWidget->zp_setModel(zv_arrayModel);
    zv_spectrumTableWidget->zp_setModel(zv_jointSpectraModel);
    zv_calibrationTableWidget->zp_setModel(zv_calibrationModel);
    zv_chemElementWidget->zp_setModel(zv_chemElementModel);
    zv_calibrationWindowTableWidget->zp_setModel(zv_jointCalibrationWindowModel);
    zv_termCorrelationTableWidget->zp_setModel(zv_termCorrelationTableModel);

    // File Action Manager <-> other components
    zv_calibrationRepository->zp_connectToFileManager(zv_fileActionManager);
    //zv_calibrationRepository->zp_connectToSpectrumArrayRepository(zv_spectrumArrayRepository);
    zv_spectrumArrayRepository->zp_connectToFileActionManager(zv_fileActionManager);

    // spectra repository <-> spectra model and array model and plotter model
    // spectra
    zv_arrayModel->zp_connectToSpectraArrayRepository(zv_spectrumArrayRepository);
    zv_jointSpectraDataManager->zp_connectToSpectrumArrayRepository(zv_spectrumArrayRepository);
    zv_jointSpectraDataManager->zp_connectToCalibrationRepository(zv_calibrationRepository);
    zv_jointSpectraModel->zp_connectToSpectraDataManager(zv_jointSpectraDataManager);
    // plotter
    zv_plotterDataManager->zp_connectToSpectraArrayRepository(zv_spectrumArrayRepository);
    zv_plotterDataManager->zp_connectToCalibrationRepository(zv_calibrationRepository);
    // zv_plotterDataManager->zp_connectToPlotter(zv_plotter);
    // chem element
    zv_chemElementDataManager->zp_connectToSpectraArrayRepository(zv_spectrumArrayRepository);
    zv_chemElementModel->zp_connectToChemElementDataManager(zv_chemElementDataManager);

    // spectra repository <-> array view
    zv_spectrumArrayWidget->zp_connectToSpectrumArrayRepository(zv_spectrumArrayRepository);
    zv_spectrumTableWidget->zp_connectToSpectrumArrayRepository(zv_spectrumArrayRepository);
    zv_spectrumTableWidget->zp_connectToCalibrationRepository(zv_calibrationRepository);

    zv_chemElementWidget->zp_appendButtonActions(zv_spectrumArrayRepository->zp_chemElementActions());
    zv_chemElementWidget->zp_appendContextMenuActions(zv_spectrumArrayRepository->zp_chemElementContextMenuActions());

    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_currentFile,
            zv_spectraSidebarWidget, &ZWidgetWithSidebar::zp_setInfoLabelText);
    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_setCurrentChemElementIndex,
            zv_chemElementWidget, &ZChemElementWidget::zp_setCurrentChemElementIndex);
    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_startCurrentChemElementEdition,
            zv_chemElementWidget, &ZChemElementWidget::zp_startCurrentChemElementEdition);
    //    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_fitPlotInBoundingRect,
    //            zv_plotter, &ZPlotter::zp_fitInBoundingRect);


    // spectra repository <-> joint spectrum view
    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_requestSelectedSpectrumIndexList,
            zv_spectrumTableWidget, &ZJointSpectrumTableWidget::zp_selectedSpectrumIndexList);
    //    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_requestSelectionToStringConversion,
    //            zv_spectrumTableWidget, &ZJointSpectrumTableWidget::zp_selectionToString);
    //    connect(zv_spectrumTableWidget, &ZJointSpectrumTableWidget::zg_selectionEnable,
    //            zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zp_onSelectionEnable );

    // spectra repository <-> chemical element view
    connect(zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zg_requestSelectedChemElementIndexList,
            zv_chemElementWidget, &ZChemElementWidget::zp_selectedChemElementIndexList);

    // calibration view <-> zv_spectraArrayRepository
    zv_calibrationTableWidget->zp_connectToCalibrationRepository(zv_calibrationRepository);

    connect(zv_calibrationTableWidget, &ZCalibrationTableWidget::zg_requestChemElementList,
            zv_spectrumArrayRepository, &ZSpectrumArrayRepository::zp_chemElementListForCurrentArray);

    // calibration repository <-> calibration model
    zv_calibrationModel->zp_connectToCalibrationRepository(zv_calibrationRepository);

    // calibration repository <-> plotter
    //    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_requestCurrentVisibleSceneRect,
    //            zv_plotter, &ZPlotter::zp_currentVisibleSceneRect);
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_setCurrentWindowIndex,
            zv_calibrationWindowTableWidget, &ZCalibrationWindowTableWidget::zp_setCurrentWindowIndex);
    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_startCurrentWindowEdition,
            zv_calibrationWindowTableWidget, &ZCalibrationWindowTableWidget::zp_startCurrentWindowEdition);


    // joint calibration window data manager <-> to repositories
    zv_jointCalibrationWindowDataManager->zp_connectToCalibrationRepository(zv_calibrationRepository);
    zv_jointCalibrationWindowDataManager->zp_connectToSpectraArrayRepository(zv_spectrumArrayRepository);

    // calibration window model to <-> joint calibration window data manager
    zv_jointCalibrationWindowModel->zp_connectToJointCalibrationWindowDataManager(zv_jointCalibrationWindowDataManager);

    // calibration repository <-> joint window view
    zv_calibrationWindowTableWidget->zp_connectToCalibrationRepository(zv_calibrationRepository);

    // joint calibration window data manager <->  joint window view
    connect(zv_jointCalibrationWindowDataManager, &ZCalibrationWindowDataManager::zg_setChannelMinMax,
            zv_calibrationWindowTableWidget, &ZCalibrationWindowTableWidget::zp_setChannelNumberMinMax);

    // term correlation table and calculation plotter
    zv_calculationPlotterManager->zp_connectToPlotter(zv_correlationPlotterWidget->zp_plotter());
    zv_calculationPlotterManager->zp_connectToCalibrationRepository(zv_calibrationRepository);
    zv_calculationPlotterManager->zp_connectToSpectrumArrayRepository(zv_spectrumArrayRepository);
    zv_calculationPlotterManager->zp_connectToTermCorrelationWidget(zv_termCorrelationTableWidget);

    connect(zv_calculationPlotterManager, &ZCorrelationPlotterDataManager::zg_initPlotRebuild,
            this, &MainWindow::zh_rebuildCorrelationPlot, Qt::QueuedConnection);

    zv_termCorrelationTableModel->zp_connectToTermCorrelationTableManager(zv_termCorrelationTableManager);
    zv_termCorrelationTableManager->zp_connectToCalibrationRepository(zv_calibrationRepository);
    zv_termCorrelationTableManager->zp_connectToSpectrumArrayRepository(zv_spectrumArrayRepository);

    zv_termCorrelationTableManager->zp_connectToJointSpectraDataManager(zv_jointSpectraDataManager);

    zv_termCorrelationTableWidget->zp_appendContextMenuActions(zv_calibrationRepository->zp_termContextMenuActions());
    connect(zv_jointSpectraDataManager, &ZJointSpectraDataManager::zg_calibrationQualityData,
            zv_calibrationModel, &ZCalibrationModel::zp_calibrationQualityDataChanged);
    connect(zv_jointSpectraDataManager, &ZJointSpectraDataManager::zg_calibrationQualityData,
            zv_calibrationRepository, &ZCalibrationRepository::zp_calibrationQualityDataChanged);

    // energy line widget
    connect(zv_energyLineTableWidget, &ZEnergyLineTableWidget::zg_energyLineOperation,
            zv_plotterDataManager, &ZPlotterDataManager::zp_onEnergyLineOperation);
    connect(zv_plotterDataManager, &ZPlotterDataManager::zg_requestEnergyLineEnergyValue,
            zv_energyLineTableWidget, &ZEnergyLineTableWidget::zp_energyLineEnergyValue);
    connect(zv_plotterDataManager, &ZPlotterDataManager::zg_requestEnergyLineRelativeIntensity,
            zv_energyLineTableWidget, &ZEnergyLineTableWidget::zp_energyLineRelativeIntensity);

    connect(zv_plotterDataManager, &ZPlotterDataManager::zg_requestEnergyLineVisibility,
            zv_energyLineTableWidget, &ZEnergyLineTableWidget::zp_energyLineVisibility);
    connect(zv_plotterDataManager, &ZPlotterDataManager::zg_requestEnergyLineColor,
            zv_energyLineTableWidget, &ZEnergyLineTableWidget::zp_energyLineColor);


    // equation dashboard
    //zv_equationSettingsPanelWidget->zp_connectToCalibrationRepository(zv_calibrationRepository);
}
//======================================================
void MainWindow::zh_messageIconPixmap(int type,
                                      QSize size,
                                      QStyle* style,
                                      QPixmap& pixmap,
                                      bool& ok) const
{
    if(//type == QtDebugMsg ||
            type == QtInfoMsg ||
            type == QtWarningMsg ||
            type == QtCriticalMsg ||
            type == QtFatalMsg)
    {
        ok = ZStandardMessagePictureDispatcher::zp_pictureForStandardMessage(static_cast<QtMsgType>(type),
                                                                             size,
                                                                             style,
                                                                             pixmap);
        return;
    }

    if(type == QtDebugMsg)
    {
        QPixmap debugPixmap = QPixmap(":/images/ZImages/Bug.png").scaled(size,
                                                                         Qt::KeepAspectRatio,
                                                                         Qt::FastTransformation);
        if(!debugPixmap.isNull())
        {
            pixmap = debugPixmap;
            ok = true;
            return;
        }
    }

    // No Pixmap
    ok = false;
}
//==========================================================
void MainWindow::zh_appendActionsToMenu(QMenu* menu)
{
    if(menu->objectName() == "File")
    {
        // language
        // menu->addAction(zv_languageControlAction);
        zv_languageMenu = new QMenu;
        zv_languageMenu->setIcon(QIcon(":/images/ZImages/earthGlobe-16.png"));
        ZTranslatorManager translatorManager;
        QString currentLanguage;
        bool ok = false;
        translatorManager.zp_currentLanguageName(currentLanguage, &ok);
        if(ok)
        {
            zv_languageMenu->setTitle(currentLanguage);
        }

        zv_languageMenu->setToolTip(tr("Application language"));

        menu->addMenu(zv_languageMenu);
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
        menu->addAction(zv_aboutQtAction);

        menu->addSeparator();
        return;
    }
}
//==========================================================
void MainWindow::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(qApp->applicationVersion());
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
    settings.endGroup(); // qApp->applicationVersion()

}
//==========================================================
void MainWindow::zh_saveSettings()
{
    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("Common");

    settings.setValue("appGeometry", saveGeometry());
    settings.setValue("appState", saveState());

    // central widget
    settings.endGroup(); // Common
    settings.endGroup(); // qApp->applicationVersion()

    emit zg_saveSettings();
}
//==========================================================
//void MainWindow::zh_about()
//{
//    //    QString title = tr("About %1").arg(glAppProduct);

//    //    QString text = tr("%1 is an application that displays in graphical form values of concentration of chemical elements technological streams contain. "
//    //                      "It works with a X-ray spectrometer SRV.").arg(glAppProduct);

//    //    QString htmlText = QString("<h2><font color=darkblue>%1</font></h2>"
//    //                               "<p>%6 - %2</p>"
//    //                               "<p>%3</p>"
//    //                               //"<p>Copyright &copy;  <b>TechnoAnalyt Ltd.</b> 2014, 2015. All rights reserved.<br>"
//    //                               //"<a href=\"http://tehnoanalit.com/\">tehnoanalit.com</a><br>"
//    //                               "<p>%7<br> "
//    //                               "Company website: <a href=\"http://tehnoanalit.com/\">tehnoanalit.com</a><br>"
//    //                               "%5: %4.<br>"
//    //                               "Author's email: <a href=mailto:petrovich.eugene@gmail.com?Subject=My%20Subject>petrovich.eugene@gmail.com</a></p>"
//    //                               ).arg(glAppProduct,
//    //                                     qApp->applicationVersion(),
//    //                                     //qApp->applicationVersion(),
//    //                                     text,
//    //                                     tr("Eugene Petrovich"),
//    //                                     tr("Author"),
//    //                                     tr("Version"),
//    //                                     glAppCopyright
//    //                                     );

//    //    QMessageBox::about(centralWidget(), title, htmlText);

//    QString title = tr("About %1").arg(glAppProduct);
//    //    QString text = tr("The application is a supplement to a SRV spectrometer software. It provides to make extra calculation of chemical concentration that cannot be directly  measured."
//    //                      "");
//    //    QString htmlText = QString(
//    //                "<table border=0 cellspacing = 15>"
//    //                "<tr>"
//    //                "<td align = left><img src=:/images/ZImages/CR3_64.png></td>"
//    //                "<td align = left><h2 align = center>CRecalc 2.3</h2>"
//    //                "<p>Copyright &copy; TechnoAnalyt Ltd., 2014.  All rights reserved.</p>"
//    //                "<p>%1</p>"
//    //                "</td>"
//    //                "</tr>"
//    //                "</table>").arg(text);

//    QString text = QString();//tr("%1 is an application that controls other console application.").arg(glAppProduct);

//    QString htmlText = QString(
//                "<table border=0 cellspacing = 15>"
//                "<tr>"
//                "<td align = left><img src=:/images/ZImages/SDC2.png></td>"
//                "<td align = left><h1 align = center>%1</h1>"
//                "</td>"
//                "</tr>"
//                "</table>"
//                "<p>%6 - %2</p>"
//                "<p>%3</p>"
//                "<p>%7<br> "
//                "Company website: <a href=\"http://%8/\">%8</a><br>"
//                "%5: %4.<br>"
//                "Author's email: <a href=mailto:petrovich.eugene@gmail.com?Subject=My%20Subject>petrovich.eugene@gmail.com</a></p>"
//                ).arg(glAppProduct,
//                      qApp->applicationVersion(),
//                      text,
//                      tr("Eugene Petrovich"),
//                      tr("Author"),
//                      tr("Version"),
//                      glAppCopyright,
//                      glAppCompanyURL
//                      );

//    QMessageBox::about(centralWidget(), title, htmlText);
//}
//==========================================================
//void MainWindow::zh_help()
//{
//    if(mv_helpBrowser == 0)
//    {
//        QUrl source = QUrl("StreamControl.htm");
//        QStringList searchList;

//        QDir dir = QApplication::applicationDirPath();
//        dir.cd("Doc");
//        searchList << dir.absolutePath(); // << ":/docs"; Streams.files
//        dir.cd("StreamControl.files");
//        searchList << dir.absolutePath() << ":/docs";

//        mv_helpBrowser = ZHelpBrowser::mf_instance(searchList, source, centralWidget());
//        mv_helpBrowser->setAttribute(Qt::WA_GroupLeader);
//        QSettings settings;
//        settings.beginGroup(qApp->applicationVersion());

//        settings.beginGroup("AppState");
//        QVariant vData = settings.value("help browser geometry");
//        settings.endGroup();
//        settings.endGroup();

//        if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
//        {
//            mv_helpBrowser->restoreGeometry(vData.toByteArray());
//        }
//        mv_helpBrowser->show();
//    }
//    else if(!mv_helpBrowser->isVisible())
//    {
//        mv_helpBrowser->setVisible(true);
//        mv_helpBrowser->mf_restoreGeometry();
//    }
//}
//==========================================================
void MainWindow::zh_rebuildCorrelationPlot()
{
    zv_correlationPlotterWidget->zp_rebuildPlotter();
    zv_calculationPlotterManager->zp_connectToPlotter(zv_correlationPlotterWidget->zp_plotter());
    zv_correlationPlotterWidget->zp_plotter()->zp_fitInBoundingRect();
    zv_calculationPlotterManager-> zh_rebuildChart();
}
//==========================================================
