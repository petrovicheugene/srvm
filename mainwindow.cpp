//==========================================================
#include "MainWindow.h"
#include "X_General.h"
#include <QApplication>
#include <QDir>
// components
#include "X_FileActionManager.h"
#include "X_SpectrumArrayRepository.h"
#include "X_JointSpectraDataManager.h"
#include "X_CalibrationWindowDataManager.h"
#include "X_CalibrationRepository.h"
#include "X_ChemElementDataManager.h"
#include "X_AbstractPlotterDataManager.h"
#include "X_CorrelationPlotterDataManager.h"
#include "X_TermCorrelationTableManager.h"
#include "X_SpeSpectrum.h"
#include "X_EquationSettingsData.h"
#include "X_HelpBrowser.h"
#include "X_StandardMessagePictureDispatcher.h"
#include "X_TranslatorManager.h"

// views
#include "X_WidgetWithSidebar.h"
#include "X_SpectrumArrayWidget.h"
#include "X_JointSpectraTableWidget.h"
#include "X_ChemElementWidget.h"
#include "X_CalibrationTableWidget.h"
#include "X_CalibrationWindowTableWidget.h"
#include "X_CorrelationPlotterWidget.h"
#include "X_TermCorrelationTableWidget.h"
#include "X_Plotter.h"
#include "X_MessagePanel.h"
#include "X_MessageWidget.h"
#include "X_EnergyLineTableWidget.h"
// models
#include "X_ArrayModel.h"
#include "X_CalibrationModel.h"
#include "X_CalibrationWindowModel.h"
#include "X_ChemElementModel.h"
#include "X_JointSpectraModel.h"
#include "X_TermCorrelationTableModel.h"

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
    : X_BaseMainWindow(parent)
{
    qRegisterMetaType<X_EquationSettingsData>("X_EquationSettingsData");
    qRegisterMetaType<X_SpeSpectrum>("X_SpeSpectrum");

    setWindowTitle(qApp->applicationDisplayName());

    //    QPalette palette = QPalette(Qt::darkBlue);
    //    this->setPalette(palette);
    mv_helpBrowser = nullptr;
    xv_fileActionManager = nullptr;
    xv_spectrumArrayRepository = nullptr;
    xv_calibrationRepository = nullptr;
    xv_jointSpectraDataManager = nullptr;
    xv_chemElementDataManager = nullptr;
    xv_jointCalibrationWindowDataManager = nullptr;
    xv_plotterDataManager = nullptr;
    xv_calculationPlotterManager = nullptr;
    xv_termCorrelationTableManager = nullptr;
    xv_energyLineTableWidget = nullptr;

    xv_arrayModel = nullptr;
    xv_jointSpectraModel = nullptr;
    xv_chemElementModel = nullptr;
    xv_calibrationModel = nullptr;
    xv_jointCalibrationWindowModel = nullptr;
    xv_termCorrelationTableModel = nullptr;
    xv_plotter = nullptr;

    zh_createActions();
    zh_createComponents();
    zh_createPlotterWidget();

    zh_createMenu();
    zh_createToolbar();
    zh_createConnections();

    zh_restoreSettings();

    // plotter starting settings
    if(xv_plotter != nullptr)
    {
        QMetaObject::invokeMethod(xv_plotter, "xp_fitInBoundingRect",
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
    if(xv_spectrumArrayRepository->xp_isDirty())
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
            xv_fileActionManager->xp_triggerSaveArrayToFileAction();
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
    xv_calibrationRepository->xp_hasDirtyCalibrations(dirtyRowList);
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
                xv_calibrationTableWidget->xp_setCurrentCalibrationIndex(dirtyRowList.at(i));
                xv_fileActionManager->xp_triggerSaveCalibrationsToFile();
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
    //    xv_languageControlAction = new QAction(this);
    //    xv_languageControlAction->setIcon(QIcon(":/images/X_Images/earthGlobe-16.png"));
    //    xv_languageControlAction->setText(tr("Language"));
    //    xv_languageControlAction->setToolTip(tr("Application language"));

    xv_exitAction = new QAction(this);
    xv_exitAction->setIcon(QIcon(NS_Icons::glIconExitApp));
    xv_exitAction->setText(tr("Exit"));
    xv_exitAction->setToolTip(tr("Exit the application"));

    //    xv_aboutAction = new QAction(this);
    xv_aboutAction->setIcon(QIcon(NS_Icons::glIconAbout));
    //    xv_aboutAction->setText(tr("About"));
    //    xv_aboutAction->setToolTip(tr("About the application"));
    xv_aboutQtAction->setIcon(QIcon(NS_Icons::glIconQt));
    //    xv_helpAction = new QAction(this);
    xv_helpAction->setIcon(QIcon(NS_Icons::glIconHelp));
    //    xv_helpAction->setText(tr("Help"));
    //    xv_helpAction->setToolTip(tr("Show user guide"));

}
//==========================================================
void MainWindow::zh_createPlotterWidget()
{
    // Plotter
    if(xv_plotter)
    {
        xv_plotter->deleteLater();
    }

    xv_plotter = new X_Plotter(this);
    QFrame* frame = zh_setWidgetToFrame(xv_plotter);
    setCentralWidget(frame);

    if(xv_plotterDataManager)
    {
        xv_plotterDataManager->xp_connectToPlotter(xv_plotter);
    }

    connect(xv_spectrumArrayRepository, &X_SpectrumArrayRepository::xg_fitPlotInBoundingRect,
            xv_plotter, &X_Plotter::xp_fitInBoundingRect);
    connect(xv_calibrationRepository, &X_CalibrationRepository::xg_requestCurrentVisibleSceneRect,
            xv_plotter, &X_Plotter::xp_currentVisibleSceneRect);

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

    X_TranslatorManager translatorManager;
    QString currentLanguageName;
    bool ok = false;
    translatorManager.xp_currentLanguageName(currentLanguageName, &ok);

    qDebug() << "Current lang" << currentLanguageName << "Installied lang" << action->text();

    if(ok && action->text() == currentLanguageName)
    {
        return;
    }

    ok = false;
    translatorManager.xp_setApplicationLanguage(action->text(), &ok);
    QString msg;
    if(ok)
    {
        msg = tr("The language of application has been changed.\n"
                 "In order for the changes to take effect, please restart the application.");
    }
    else
    {
        msg = translatorManager.xp_lastError().isEmpty() ?
                    tr("Unknown language settings error.") : translatorManager.xp_lastError();

    }

    QString title = tr("Language settings");
    QMessageBox::information(this, title, msg, QMessageBox::Ok);
}
//==========================================================
void MainWindow::zh_fillLanguageMenu()
{
    xv_languageMenu->clear();
    X_TranslatorManager translatorManager;
    QStringList availableLanguageNameList;
    translatorManager.xp_availableLanguageNameList(availableLanguageNameList);

    foreach(QString languageName, availableLanguageNameList)
    {
        QAction* action = xv_languageMenu->addAction(languageName);
        connect(action, &QAction::triggered,
                this, &MainWindow::zh_appLanguageControl);
    }
}
//==========================================================
void MainWindow::zh_createComponents()
{
    // CENTRAL WIDGET
    //    // Plotter
    //    xv_plotter = new X_Plotter(this);
    //    QFrame* frame = zh_setWidgetToFrame(xv_plotter);
    //    setCentralWidget(frame);

    // DOCKS
    // Spectrum array dock
    xv_spectrumArrayDock = new QDockWidget(this);
    xv_spectrumArrayDock->setObjectName("SPECTRUM_ARRAY_DOCK");
    xv_spectrumArrayDock->setWindowTitle(tr("Spectra"));
    xv_dockList << xv_spectrumArrayDock;
    addDockWidget(Qt::TopDockWidgetArea, xv_spectrumArrayDock);

    xv_spectraSidebarWidget = new X_WidgetWithSidebar("SPECTRUM_SIDEBAR_WIDGET", true, this);
    xv_spectraSidebarWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    xv_spectraSidebarWidget->setLineWidth(1);
    xv_spectrumArrayWidget = new X_SpectrumArrayWidget(this);
    xv_chemElementWidget = new X_ChemElementWidget(this);
    xv_spectrumTableWidget = new X_JointSpectrumTableWidget(this);

    QSplitter* spectrumArraySplitter = new QSplitter(Qt::Vertical, this);
    spectrumArraySplitter->addWidget(xv_spectrumArrayWidget);
    spectrumArraySplitter->addWidget(xv_chemElementWidget);
    spectrumArraySplitter->setChildrenCollapsible(false);

    xv_spectraSidebarWidget->xp_setSidebarWidget(spectrumArraySplitter);
    xv_spectraSidebarWidget->xp_setMainWidget(xv_spectrumTableWidget);
    // setting to dock

    xv_spectrumArrayDock->setWidget(xv_spectraSidebarWidget);

    // Calibration View
    xv_calibrationDock = new QDockWidget(this);
    xv_calibrationDock->setObjectName("CALIBRATION_DOCK");
    xv_calibrationDock->setWindowTitle(tr("Calibrations"));
    xv_dockList << xv_calibrationDock;
    addDockWidget(Qt::TopDockWidgetArea, xv_calibrationDock);

    xv_calibrationSidebarWidget = new X_WidgetWithSidebar("CALIBRATION_SIDEBAR_WIDGET", false, this);
    xv_calibrationSidebarWidget->xp_setMargin(0);
    // xv_calibrationSidebarWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    xv_calibrationSidebarWidget->setFrameShape(QFrame::NoFrame);
    xv_calibrationSidebarWidget->setLineWidth(0);

    xv_calibrationTableWidget = new X_CalibrationTableWidget(this);
    xv_termCorrelationTableWidget = new X_TermCorrelationTableWidget(this);
    xv_calibrationWindowTableWidget = new X_CalibrationWindowTableWidget(this);
    //xv_equationSettingsPanelWidget = new X_EquationSettingsDashBoard(this);

    // sideWidget composition
    xv_calibrationSidebarWidget->xp_setSidebarWidget(xv_calibrationWindowTableWidget);
    xv_calibrationSidebarWidget->xp_setMainWidget(xv_calibrationTableWidget);

    // left side widget
    QWidget* calibrationWidget = new QWidget();
    QVBoxLayout*leftWidgetLayout = new QVBoxLayout;
    leftWidgetLayout->setContentsMargins(0, 0, 0, 0);
    calibrationWidget->setLayout(leftWidgetLayout);

    QSplitter* calibrationSplitter = new QSplitter(Qt::Vertical, this);
    calibrationSplitter->setFrameShape(QFrame::NoFrame);
    calibrationSplitter->addWidget(xv_calibrationSidebarWidget);
    calibrationSplitter->addWidget(xv_termCorrelationTableWidget);
    calibrationSplitter->setChildrenCollapsible(false);
    leftWidgetLayout->addWidget(calibrationSplitter);

    //frame = zh_setWidgetToFrame(xv_equationSettingsPanelWidget);
    //frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //leftWidgetLayout->addWidget(frame);

    // setting to dock
    QFrame* frame = zh_setWidgetToFrame(calibrationWidget);
    xv_calibrationDock->setWidget(frame);

    // Correlation Plotter View
    xv_correlationPlotterDock= new QDockWidget(this);
    xv_correlationPlotterDock->setObjectName("PLOT_DOCK");
    xv_correlationPlotterDock->setWindowTitle(tr("Plot"));
    xv_dockList << xv_correlationPlotterDock;
    addDockWidget(Qt::TopDockWidgetArea, xv_correlationPlotterDock);

    xv_correlationPlotterWidget = new X_CorrelationPlotterWidget(this);

    // setting to dock
    xv_correlationPlotterDock->setWidget(xv_correlationPlotterWidget);

    // Message Panel
    xv_messagePanelDock = new QDockWidget(this);
    xv_messagePanelDock->setObjectName("MESSAGE_PANEL_DOCK");
    xv_messagePanelDock->setWindowTitle(tr("Message"));
    xv_dockList << xv_messagePanelDock;
    addDockWidget(Qt::LeftDockWidgetArea, xv_messagePanelDock);

    xv_messageWidget = new X_MessageWidget(this);
    frame = zh_setWidgetToFrame(xv_messageWidget);
    // setting to dock
    xv_messagePanelDock->setWidget(frame);

    // energy line table
    xv_energyLineTableDock = new QDockWidget(this);
    xv_energyLineTableDock->setObjectName("ENERGY_LINE_TABLE_DOCK");
    xv_energyLineTableDock->setWindowTitle(tr("Energy lines"));
    xv_dockList << xv_energyLineTableDock;
    addDockWidget(Qt::RightDockWidgetArea, xv_energyLineTableDock);

    // create widget
    xv_energyLineTableWidget = new X_EnergyLineTableWidget(this);
    xv_energyLineTableWidget->show();
    frame = zh_setWidgetToFrame(xv_energyLineTableWidget);

    // setting to widget dock
    xv_energyLineTableDock->setWidget(frame);


    // tabblifying docks by default
    // this->tabifyDockWidget(xv_messagePanelDock, xv_calibrationArrayDock);

    // Components
    xv_fileActionManager = new X_FileActionManager(this);
    xv_spectrumArrayRepository = new X_SpectrumArrayRepository(this);
    xv_jointSpectraDataManager = new X_JointSpectraDataManager(this);
    xv_chemElementDataManager = new X_ChemElementDataManager(this);
    xv_calibrationRepository = new X_CalibrationRepository(this);
    xv_jointCalibrationWindowDataManager = new X_CalibrationWindowDataManager(this);
    xv_plotterDataManager = new X_AbstractPlotterDataManager(this);
    xv_calculationPlotterManager = new X_CorrelationPlotterDataManager(this);
    xv_termCorrelationTableManager = new X_TermCorrelationTableManager(this);

    // Models
    xv_arrayModel = new X_ArrayModel(this);
    xv_jointSpectraModel = new X_JointSpectraModel(this);
    xv_chemElementModel = new X_ChemElementModel(this);
    xv_calibrationModel = new X_CalibrationModel(this);
    xv_jointCalibrationWindowModel = new X_CalibrationWindowModel(this);
    xv_termCorrelationTableModel = new X_TermCorrelationTableModel(this);

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
    xv_fileActionManager->xp_appendActionsToMenu(menu);
    xv_spectrumArrayRepository->xp_appendActionsToMenu(menu);
    xv_calibrationRepository->xp_appendActionsToMenu(menu);
    zh_appendActionsToMenu(menu);

    // Edit
    menu = menuBar()->addMenu(tr("Edit"));
    // menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName("Edit");
    xv_spectrumArrayRepository->xp_appendActionsToMenu(menu);
    xv_calibrationRepository->xp_appendActionsToMenu(menu);
    zh_appendActionsToMenu(menu);

    // View
    menu = menuBar()->addMenu(tr("View"));
    // menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName("View");
    xv_spectrumArrayRepository->xp_appendActionsToMenu(menu);
    xv_calibrationRepository->xp_appendActionsToMenu(menu);
    zh_appendActionsToMenu(menu);

    // Actions
    menu = menuBar()->addMenu(tr("Actions"));
    // menu->setCursor(Qt::PointingHandCursor);
    menu->setObjectName("Actions");
    xv_spectrumArrayRepository->xp_appendActionsToMenu(menu);
    xv_calibrationRepository->xp_appendActionsToMenu(menu);
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

    connect(this, &MainWindow::xg_standardLogMessage,
            xv_messageWidget, &X_MessageWidget::xp_appendMessage, Qt::QueuedConnection);
    connect(this, &MainWindow::xg_plainLogMessage,
            xv_messageWidget, &X_MessageWidget::xp_appendPlainMessage, Qt::QueuedConnection);
    connect(xv_messageWidget, &X_MessageWidget::xg_requestMessageIconPixmap,
            this, &MainWindow::zh_messageIconPixmap);


    // main window actions
    connect(xv_exitAction, &QAction::triggered,
            this, &MainWindow::close);
    //    connect(xv_languageControlAction, &QAction::triggered,
    //            this, &MainWindow::zh_appLanguageControl);
    connect(xv_languageMenu, &QMenu::aboutToShow,
            this, &MainWindow::zh_fillLanguageMenu);

    //    connect(xv_aboutAction, &QAction::triggered,
    //            this, &MainWindow::zh_onAboutAction);
    //    connect(xv_helpAction, &QAction::triggered,
    //            this, &MainWindow::zh_onHelpAction);

    // main window save settings command
    connect(this, &MainWindow::xg_saveSettings,
            xv_spectraSidebarWidget, &X_WidgetWithSidebar::xp_saveSettings);
    connect(this, &MainWindow::xg_saveSettings,
            xv_calibrationSidebarWidget, &X_WidgetWithSidebar::xp_saveSettings);
    //    connect(this, &MainWindow::xg_saveSettings,
    //            xv_termCorrelationTableSidebarWidget, &X_WidgetWithSidebar::xp_saveSettings);

    connect(this, &MainWindow::xg_saveSettings,
            xv_fileActionManager, &X_FileActionManager::xp_saveSettings);

    // views set models
    xv_spectrumArrayWidget->xp_setModel(xv_arrayModel);
    xv_spectrumTableWidget->xp_setModel(xv_jointSpectraModel);
    xv_calibrationTableWidget->xp_setModel(xv_calibrationModel);
    xv_chemElementWidget->xp_setModel(xv_chemElementModel);
    xv_calibrationWindowTableWidget->xp_setModel(xv_jointCalibrationWindowModel);
    xv_termCorrelationTableWidget->xp_setModel(xv_termCorrelationTableModel);

    // File Action Manager <-> other components
    xv_calibrationRepository->xp_connectToFileActionManager(xv_fileActionManager);
    //xv_calibrationRepository->xp_connectToSpectrumArrayRepository(xv_spectrumArrayRepository);
    xv_spectrumArrayRepository->xp_connectToFileActionManager(xv_fileActionManager);

    // spectra repository <-> spectra model and array model and plotter model
    // spectra
    xv_arrayModel->xp_connectToSpectraArrayRepository(xv_spectrumArrayRepository);
    xv_jointSpectraDataManager->xp_connectToSpectrumArrayRepository(xv_spectrumArrayRepository);
    xv_jointSpectraDataManager->xp_connectToCalibrationRepository(xv_calibrationRepository);
    xv_jointSpectraModel->xp_connectToSpectraDataManager(xv_jointSpectraDataManager);
    // plotter
    xv_plotterDataManager->xp_connectToSpectraArrayRepository(xv_spectrumArrayRepository);
    xv_plotterDataManager->xp_connectToCalibrationRepository(xv_calibrationRepository);
    // xv_plotterDataManager->xp_connectToPlotter(xv_plotter);
    // chem element
    xv_chemElementDataManager->xp_connectToSpectraArrayRepository(xv_spectrumArrayRepository);
    xv_chemElementModel->xp_connectToChemElementDataManager(xv_chemElementDataManager);

    // spectra repository <-> array view
    xv_spectrumArrayWidget->xp_connectToSpectrumArrayRepository(xv_spectrumArrayRepository);
    xv_spectrumTableWidget->xp_connectToSpectrumArrayRepository(xv_spectrumArrayRepository);
    xv_spectrumTableWidget->xp_connectToCalibrationRepository(xv_calibrationRepository);

    xv_chemElementWidget->xp_appendButtonActions(xv_spectrumArrayRepository->xp_chemElementActions());
    xv_chemElementWidget->xp_appendContextMenuActions(xv_spectrumArrayRepository->xp_chemElementContextMenuActions());

    connect(xv_spectrumArrayRepository, &X_SpectrumArrayRepository::xg_currentFile,
            xv_spectraSidebarWidget, &X_WidgetWithSidebar::xp_setInfoLabelText);
    connect(xv_spectrumArrayRepository, &X_SpectrumArrayRepository::xg_setCurrentChemElementIndex,
            xv_chemElementWidget, &X_ChemElementWidget::xp_setCurrentChemElementIndex);
    connect(xv_spectrumArrayRepository, &X_SpectrumArrayRepository::xg_startCurrentChemElementEdition,
            xv_chemElementWidget, &X_ChemElementWidget::xp_startCurrentChemElementEdition);
    //    connect(xv_spectrumArrayRepository, &X_SpectrumArrayRepository::xg_fitPlotInBoundingRect,
    //            xv_plotter, &X_Plotter::xp_fitInBoundingRect);


    // spectra repository <-> joint spectrum view
    connect(xv_spectrumArrayRepository, &X_SpectrumArrayRepository::xg_requestSelectedSpectrumIndexList,
            xv_spectrumTableWidget, &X_JointSpectrumTableWidget::xp_selectedSpectrumIndexList);
    //    connect(xv_spectrumArrayRepository, &X_SpectrumArrayRepository::xg_requestSelectionToStringConversion,
    //            xv_spectrumTableWidget, &X_JointSpectrumTableWidget::xp_selectionToString);
    //    connect(xv_spectrumTableWidget, &X_JointSpectrumTableWidget::xg_selectionEnable,
    //            xv_spectrumArrayRepository, &X_SpectrumArrayRepository::xp_onSelectionEnable );

    // spectra repository <-> chemical element view
    connect(xv_spectrumArrayRepository, &X_SpectrumArrayRepository::xg_requestSelectedChemElementIndexList,
            xv_chemElementWidget, &X_ChemElementWidget::xp_selectedChemElementIndexList);

    // calibration view <-> xv_spectraArrayRepository
    xv_calibrationTableWidget->xp_connectToCalibrationRepository(xv_calibrationRepository);

    connect(xv_calibrationTableWidget, &X_CalibrationTableWidget::xg_requestChemElementList,
            xv_spectrumArrayRepository, &X_SpectrumArrayRepository::xp_chemElementListForCurrentArray);

    // calibration repository <-> calibration model
    xv_calibrationModel->xp_connectToCalibrationRepository(xv_calibrationRepository);

    // calibration repository <-> plotter
    //    connect(xv_calibrationRepository, &X_CalibrationRepository::xg_requestCurrentVisibleSceneRect,
    //            xv_plotter, &X_Plotter::xp_currentVisibleSceneRect);
    connect(xv_calibrationRepository, &X_CalibrationRepository::xg_setCurrentWindowIndex,
            xv_calibrationWindowTableWidget, &X_CalibrationWindowTableWidget::xp_setCurrentWindowIndex);
    connect(xv_calibrationRepository, &X_CalibrationRepository::xg_startCurrentWindowEdition,
            xv_calibrationWindowTableWidget, &X_CalibrationWindowTableWidget::xp_startCurrentWindowEdition);


    // joint calibration window data manager <-> to repositories
    xv_jointCalibrationWindowDataManager->xp_connectToCalibrationRepository(xv_calibrationRepository);
    xv_jointCalibrationWindowDataManager->xp_connectToSpectraArrayRepository(xv_spectrumArrayRepository);

    // calibration window model to <-> joint calibration window data manager
    xv_jointCalibrationWindowModel->xp_connectToJointCalibrationWindowDataManager(xv_jointCalibrationWindowDataManager);

    // calibration repository <-> joint window view
    xv_calibrationWindowTableWidget->xp_connectToCalibrationRepository(xv_calibrationRepository);

    // joint calibration window data manager <->  joint window view
    connect(xv_jointCalibrationWindowDataManager, &X_CalibrationWindowDataManager::xg_setChannelMinMax,
            xv_calibrationWindowTableWidget, &X_CalibrationWindowTableWidget::xp_setChannelNumberMinMax);

    // term correlation table and calculation plotter
    xv_calculationPlotterManager->xp_connectToPlotter(xv_correlationPlotterWidget->xp_plotter());
    xv_calculationPlotterManager->xp_connectToCalibrationRepository(xv_calibrationRepository);
    xv_calculationPlotterManager->xp_connectToSpectrumArrayRepository(xv_spectrumArrayRepository);
    xv_calculationPlotterManager->xp_connectToTermCorrelationWidget(xv_termCorrelationTableWidget);

    connect(xv_calculationPlotterManager, &X_CorrelationPlotterDataManager::xg_initPlotRebuild,
            this, &MainWindow::zh_rebuildCorrelationPlot, Qt::QueuedConnection);

    xv_termCorrelationTableModel->xp_connectToTermCorrelationTableManager(xv_termCorrelationTableManager);
    xv_termCorrelationTableManager->xp_connectToCalibrationRepository(xv_calibrationRepository);
    xv_termCorrelationTableManager->xp_connectToSpectrumArrayRepository(xv_spectrumArrayRepository);

    xv_termCorrelationTableManager->xp_connectToJointSpectraDataManager(xv_jointSpectraDataManager);

    xv_termCorrelationTableWidget->xp_appendContextMenuActions(xv_calibrationRepository->xp_termContextMenuActions());
    connect(xv_jointSpectraDataManager, &X_JointSpectraDataManager::xg_calibrationQualityData,
            xv_calibrationModel, &X_CalibrationModel::xp_calibrationQualityDataChanged);
    connect(xv_jointSpectraDataManager, &X_JointSpectraDataManager::xg_calibrationQualityData,
            xv_calibrationRepository, &X_CalibrationRepository::xp_calibrationQualityDataChanged);

    // energy line widget
    connect(xv_energyLineTableWidget, &X_EnergyLineTableWidget::xg_energyLineOperation,
            xv_plotterDataManager, &X_AbstractPlotterDataManager::xp_onEnergyLineOperation);
    connect(xv_plotterDataManager, &X_AbstractPlotterDataManager::xg_requestEnergyLineEnergyValue,
            xv_energyLineTableWidget, &X_EnergyLineTableWidget::xp_energyLineEnergyValue);
    connect(xv_plotterDataManager, &X_AbstractPlotterDataManager::xg_requestEnergyLineRelativeIntensity,
            xv_energyLineTableWidget, &X_EnergyLineTableWidget::xp_energyLineRelativeIntensity);

    connect(xv_plotterDataManager, &X_AbstractPlotterDataManager::xg_requestEnergyLineVisibility,
            xv_energyLineTableWidget, &X_EnergyLineTableWidget::xp_energyLineVisibility);
    connect(xv_plotterDataManager, &X_AbstractPlotterDataManager::xg_requestEnergyLineColor,
            xv_energyLineTableWidget, &X_EnergyLineTableWidget::xp_energyLineColor);


    // equation dashboard
    //xv_equationSettingsPanelWidget->xp_connectToCalibrationRepository(xv_calibrationRepository);
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
        ok = X_StandardMessagePictureDispatcher::xp_pictureForStandardMessage(static_cast<QtMsgType>(type),
                                                                             size,
                                                                             style,
                                                                             pixmap);
        return;
    }

    if(type == QtDebugMsg)
    {
        QPixmap debugPixmap = QPixmap(":/images/X_Images/Bug.png").scaled(size,
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
        // menu->addAction(xv_languageControlAction);
        xv_languageMenu = new QMenu;
        xv_languageMenu->setIcon(QIcon(":/images/X_Images/earthGlobe-16.png"));
        X_TranslatorManager translatorManager;
        QString currentLanguage;
        bool ok = false;
        translatorManager.xp_currentLanguageName(currentLanguage, &ok);
        if(ok)
        {
            xv_languageMenu->setTitle(currentLanguage);
        }

        xv_languageMenu->setToolTip(tr("Application language"));

        menu->addMenu(xv_languageMenu);
        menu->addAction(xv_exitAction);
        menu->addSeparator();
        return;
    }

    if(menu->objectName() == "View")
    {
        foreach(QDockWidget* dock, xv_dockList)
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
        menu->addAction(xv_helpAction);
        menu->addAction(xv_aboutAction);
        menu->addAction(xv_aboutQtAction);

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

    emit xg_saveSettings();
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
//    //                "<td align = left><img src=:/images/X_Images/CR3_64.png></td>"
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
//                "<td align = left><img src=:/images/X_Images/SDC2.png></td>"
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

//        mv_helpBrowser = X_HelpBrowser::mf_instance(searchList, source, centralWidget());
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
    xv_correlationPlotterWidget->xp_rebuildPlotter();
    xv_calculationPlotterManager->xp_connectToPlotter(xv_correlationPlotterWidget->xp_plotter());
    xv_correlationPlotterWidget->xp_plotter()->xp_fitInBoundingRect();
    xv_calculationPlotterManager-> zh_rebuildChart();
}
//==========================================================
