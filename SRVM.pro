#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T13:50:38
#
#-------------------------------------------------
QT       += core gui
QT       += xml
QT       +=  core5compat

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#PRO VARS
RC_ICONS = "X_Images/SRVM-8.ico"

#Application version
VER_MAJ=22
VER_MIN=09
VER_PAT=01

PRODUCT_SHORT_NAME="SRVM"
PRODUCT_FILE_BASE_NAME="SRVM"
QMAKE_TARGET_PRODUCT="SRV M"
QMAKE_TARGET_DESCRIPTION="The application for the creation of the spectrometer calibration"
QMAKE_TARGET_COMPANY="TechnoAnalyt"
QMAKE_TARGET_COPYRIGHT="Copyright © $${QMAKE_TARGET_COMPANY} Ltd. 2015 - 2022. All rights reserved."
COMPANY_URL=www.tehnoanalit.com

#-------------------------------------------------
# in common.pri will be defined VERSION, TARGET, DEBUG SETTINGS
#  global APP DEFINES
#-------------------------------------------------
#include(./common.pri )
#include(./app.pri )

# common.pri
#-------------------------------------------------
PROJECT_ROOT_PATH = $${PWD}/

win32: OS_SUFFIX = win32
linux-g++: OS_SUFFIX = linux

CONFIG += c++11
CONFIG += c++14
#CONFIG += c++17
#preserve src directory structure in build directory
#CONFIG += object_parallel_to_source

#CONFIG -= debug_and_release
#CONFIG -= debug_and_release_target

VERSION=$${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

CONFIG(debug, debug|release) {
    BUILD_FLAG = debug
    LIB_SUFFIX = d

} else {
    BUILD_FLAG = release
}

#Define the preprocessor macro to get the application version in the application.
DEFINES += APP_DISPLAY_NAME=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\"
DEFINES += APP_PRODUCT=\"\\\"$${PRODUCT_SHORT_NAME}\\\"\"
DEFINES += APP_VERSION=\"\\\"$${VERSION}.$${VER_RELEASE}\\\"\"
DEFINES += APP_COMPANY=\"\\\"$${QMAKE_TARGET_COMPANY}\\\"\"
DEFINES += APP_COMPANY_URL=\"\\\"$${COMPANY_URL}\\\"\"
DEFINES += APP_COPYRIGHT=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\"
DEFINES += APP_ICON=\"\\\"$${RC_ICONS}\\\"\"

#DEBUG SETTINGS
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
#by default defined: in Debug mode QT_DEBUG, in Release mode QT_NO_DEBUG

QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

#LIBS_PATH = $${PROJECT_ROOT_PATH}/lib.$${OS_SUFFIX}/
#EXPORT_PATH = $${PROJECT_ROOT_PATH}/export/
#IMPORT_PATH = $${PROJECT_ROOT_PATH}/import/
#COMMON_PATH = $${PROJECT_ROOT_PATH}/src/common/
#BIN_PATH = $${PROJECT_ROOT_PATH}/bin/$${QMAKE_TARGET_PRODUCT}/$${BUILD_FLAG}/

#BUILD_PATH = $${PROJECT_ROOT_PATH}/build/$${QMAKE_TARGET_PRODUCT}/$${BUILD_FLAG}/
#RCC_DIR = $${BUILD_PATH}/rcc/
#UI_DIR = $${BUILD_PATH}/ui/
#MOC_DIR = $${BUILD_PATH}/moc/
#OBJECTS_DIR = $${BUILD_PATH}/obj/
#MAKEFILE = $${BUILD_PATH}/Makefile

#LIBS += -L$${LIBS_PATH}/
#INCLUDEPATH += $${EXPORT_PATH}/
#INCLUDEPATH += $${IMPORT_PATH}/
#INCLUDEPATH += $${COMMON_PATH}/
linux-g++: QMAKE_CXXFLAGS += -std=c++11

# app.pri
#-------------------------------------------------
#DESTDIR = $${BIN_PATH}/
linux-g++: QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../../lib.$${OS_SUFFIX}/

CONFIG(debug, debug|release) {
    TARGET=$${PRODUCT_FILE_BASE_NAME}-$${VERSION}.$${BUILD_FLAG}
} else {
    TARGET=$${PRODUCT_FILE_BASE_NAME}
    DESTDIR=$${PWD}\..\bin\
}


#END

#PRO VARS
#Application version
#RC_ICONS = "X_Images/SRVM-8.ico"

#VER_MAJ=1
#VER_MIN=0
#VER_PAT=0

#VER_RELEASE=b

#DEBUG SETTINGS
#CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
#by default defined: in Debug mode QT_DEBUG, in Release mode QT_NO_DEBUG

#EXE_BASE_NAME=SRVM
#QMAKE_TARGET_PRODUCT="SRV M"
#QMAKE_TARGET_DESCRIPTION="Creation of spectrometer calibration"
#QMAKE_TARGET_COMPANY="TechnoAnalyt"
#QMAKE_TARGET_COPYRIGHT="Copyright © $${QMAKE_TARGET_COMPANY} Ltd. 2015 - 2017.  All rights reserved."
#COMPANY_URL=tehnoanalit.com

#CONFIG += $$VER_RELEASE
#CONFIG += c++11

#VERSION=$${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

#Target version
#CONFIG(debug, debug|release):{
#TARGET=$${EXE_BASE_NAME}-$${VERSION}
#}else{
#TARGET=$${EXE_BASE_NAME}-$${VERSION}.$${VER_RELEASE}
#}

#Define the preprocessor macro to get the application version in the application.
#DEFINES += APP_PRODUCT=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\"
#DEFINES += APP_EXE_BASE_NAME=\"\\\"$${EXE_BASE_NAME}\\\"\"
#DEFINES += APP_VERSION=\"\\\"$${VERSION}.$${VER_RELEASE}\\\"\"
#DEFINES += APP_COMPANY=\"\\\"$${QMAKE_TARGET_COMPANY}\\\"\"
#DEFINES += APP_COMPANY_URL=\"\\\"$${COMPANY_URL}\\\"\"
#DEFINES += APP_COPYRIGHT=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\"

#TRANSLATIONS = $${EXE_BASE_NAME}_ru.ts
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

#END OF CHIEF SETTINGS
TRANSLATIONS = $${PRODUCT_FILE_BASE_NAME}_ru.ts \
$${PRODUCT_FILE_BASE_NAME}_en.ts \
$${PRODUCT_FILE_BASE_NAME}_kk.ts

TEMPLATE = app

INCLUDEPATH += X_Plotter \
    X_Models \
    X_ModelViews \
    X_Widgets \
    X_Components \
    X_Dialogs \
    X_ModelViews/X_NumericDelegate \
    X_ModelViews/X_StringDelegate \
    X_ModelViews/X_SpectrumTableDelegate \
    X_ModelViews/X_VisibilityStringDelegate \
    X_ModelViews/X_ChemElementComboBoxDelegate \
    X_ModelViews/X_IntDelegate \
    X_ModelViews/X_WindowTypeComboBoxDelegate \
    X_ModelViews/X_CustomCheckableVerticalHeaderView \
    X_ModelViews/X_EquationDelegate \
    X_ModelViews/X_NormaDelegate \
    X_PeriodicTableDialog \
    X_PeriodicTable \
    X_EnergyLineManager \
    X_EnergyCalibrator \
    X_BaseMainWindow

SOURCES += main.cpp \
    MainWindow.cpp \
    X_Components/X_AbstractPlotterDataManager.cpp \
    X_Components/X_MathExpressionHandler.cpp \
    X_Components/X_MathExpressionVariableListMaker.cpp \
    X_Components/X_XMLCalibrationProjectIOHandler.cpp \
    X_Dialogs/X_CustomTermDialog.cpp \
    X_General.cpp \
    X_Components/X_SpectrumArray.cpp \
    X_Components/X_FileActionManager.cpp \
    X_Components/X_AbstractSpectrumIOHandler.cpp \
    X_Components/X_SpeIOHandler.cpp \
    X_Components/X_SpeSpectrum.cpp \
    X_Components/X_AbstractSpectrumAuxData.cpp \
    X_Components/X_AbstractSpectrum.cpp \
    X_Components/X_SpeAuxData.cpp \
    X_Models/X_ArrayModel.cpp \
    X_Models/X_ChemElementModel.cpp \
    X_Models/X_CalibrationModel.cpp \
    X_Models/X_JointSpectraModel.cpp \
    X_ModelViews/X_SpectrumArrayWidget.cpp \
    X_ModelViews/X_CalibrationTableWidget.cpp \
    X_ModelViews/X_ChemElementWidget.cpp \
    X_ModelViews/X_JointSpectraTableWidget.cpp \
    X_Widgets/X_ClickableLabel.cpp \
    X_Widgets/X_MessagePanel.cpp \
    #X_Widgets/X_HelpBrowser.cpp \
    X_Widgets/X_WidgetWithSidebar.cpp \
    X_Components/X_XMLSpectrumArrayIOHandler.cpp \
    X_Components/X_XMLCalibrationIOHandler.cpp \
    X_Components/X_CalibrationRepository.cpp \
    X_Components/X_JointSpectraDataManager.cpp \
    X_Components/X_ChemElementList.cpp \
    X_Components/X_ChemElementDataManager.cpp \
    X_ModelViews/X_NumericDelegate/X_NumericDelegate.cpp \
    X_ModelViews/X_NumericDelegate/X_NumericEditor.cpp \
    X_ModelViews/X_StringDelegate/X_StringDelegate.cpp \
    X_ModelViews/X_SpectrumTableDelegate/X_SpectrumTableDelegate.cpp \
    X_ModelViews/X_SpectrumTableDelegate/X_PrimitivePlot.cpp \
    X_ModelViews/X_VisibilityStringDelegate/X_VisibilityStringDelegate.cpp \
    X_ModelViews/X_ChemElementComboBoxDelegate/X_ChemElementComboBoxDelegate.cpp \
    X_ModelViews/X_WindowTypeComboBoxDelegate/X_WindowTypeComboBoxDelegate.cpp \
    X_Plotter/X_PlotGraphicsScene.cpp \
    X_Plotter/X_PlotGraphicsView.cpp \
    X_Plotter/X_Plotter.cpp \
    X_Plotter/X_RulersAndGridManager.cpp \
    X_Plotter/X_RulerWidget.cpp \
    X_Plotter/X_SpectrumGraphicsItem.cpp \
    X_Plotter/X_WindowGraphicsItem.cpp \
    X_Plotter/X_HorizontalDashBoard.cpp \
    X_Components/X_Calibration.cpp \
    X_Components/X_SpectrumArrayRepository.cpp \
    X_Models/X_CalibrationWindowModel.cpp \
    X_Components/X_CalibrationWindowDataManager.cpp \
    X_Components/X_CalibrationWindow.cpp \
    X_ModelViews/X_CalibrationWindowTableWidget.cpp \
    X_Plotter/X_DefaultRectGraphicsItem.cpp \
    X_ModelViews/X_IntDelegate/X_ChannelNumberDelegate.cpp \
    X_ModelViews/X_TermCorrelationTableWidget.cpp \
    X_Models/X_TermCorrelationTableModel.cpp \
    X_Components/X_AbstractTerm.cpp \
    X_Components/X_TermCorrelationTableManager.cpp \
    X_Components/X_SimpleTerm.cpp \
    X_Components/X_QuadraticTerm.cpp \
    X_Components/X_CustomTerm.cpp \
    X_ModelViews/X_CustomCheckableVerticalHeaderView/X_CustomCheckableVerticalHeaderView.cpp \
    X_Components/X_TermNormalizer.cpp \
    X_ModelViews/X_CorrelationPlotterWidget.cpp \
    X_ModelViews/X_EquationDelegate/X_EquationDelegate.cpp \
    X_Dialogs/X_EquationChooseDialog.cpp \
    X_Components/X_EquationSettingsData.cpp \
    X_Dialogs/X_NormaSettingsDialog.cpp \
    X_ModelViews/X_NormaDelegate/X_NormaDelegate.cpp \
    X_Components/X_NormaSettingsData.cpp \
    X_Components/X_CorrelationPlotterDataManager.cpp \
    X_Components/X_VisibilityPointF.cpp \
    X_Plotter/X_ChartPointGraphicsItem.cpp \
    X_Plotter/X_ChartPointOptions.cpp \
    X_Components/X_QrealToStringConverter.cpp \
    X_Components/X_CalibrationQualityData.cpp \
    X_Components/X_PasteData.cpp \
    X_Components/X_MixedTerm.cpp \
    X_ModelViews/X_ActiveStyledItemDelegate.cpp \
    X_Components/X_RawTerm.cpp \
    X_Components/X_RawWindow.cpp \
    X_Dialogs/X_SetGainFactorToCalibrationDialog.cpp \
    X_Components/X_LESGaussSolver.cpp \
    X_Plotter/X_EnergyLineGraphicsItem.cpp \
    X_Components/X_EnergyCalibrationLine.cpp \
    X_Plotter/X_TextGraphicsItem.cpp \
    X_Plotter/X_RulerToolGraphicsItem.cpp \
    X_EnergyCalibrator/X_EnergyCalibrationDialog.cpp \
    X_EnergyCalibrator/X_EnergyCalibrationDialogV2.cpp \
    X_EnergyCalibrator/X_EnergyCalibrationSpectrumTableModel.cpp \
    X_EnergyCalibrator/X_PeakWindow.cpp \
    X_EnergyCalibrator/X_PeakWindowTableModel.cpp \
    X_EnergyLineManager/X_EnergyLineDelegate.cpp \
    X_EnergyLineManager/X_EnergyLineManager.cpp \
    X_EnergyLineManager/X_EnergyLineSetItem.cpp \
    X_EnergyLineManager/X_EnergyLineTableWidget.cpp \
    X_EnergyLineManager/X_SelectedEnergyLineTableModel.cpp \
    X_PeriodicTable/X_AbstractChemicalElementPropertyRepository.cpp \
    X_PeriodicTable/X_BaseDialog.cpp \
    X_PeriodicTable/X_CaptionFormatter.cpp \
    X_PeriodicTable/X_ChemicalElementButton.cpp \
    X_PeriodicTable/X_ChemicalElementPropertyIOManager.cpp \
    X_PeriodicTable/X_ChemicalElementPropertyItem.cpp \
    X_PeriodicTable/X_ChemicalElementPropertyTreeModel.cpp \
    X_PeriodicTable/X_ChemicalElementProprtyEditor.cpp \
    X_PeriodicTable/X_ChemicalPropertyProxyTableModel.cpp \
    X_PeriodicTable/X_PeriodicTableWidget.cpp \
    X_PeriodicTable/X_PropertyEditDialog.cpp \
    X_PeriodicTable/X_PropertySectionEditDialog.cpp \
    X_PeriodicTableDialog/X_EnergyLineSelectionDialog.cpp \
    X_Widgets/X_ColorButton.cpp \
    X_Plotter/X_VerticalLineGraphicsItem.cpp \
    X_ModelViews/X_BaseTableWidget.cpp \
    X_Components/X_ControlAction.cpp \
    X_BaseMainWindow/X_BaseMainWindow.cpp \
    X_BaseMainWindow/X_HelpBrowser.cpp \
    X_Widgets/X_MessageWidget.cpp \
    X_Components/X_StandardMessagePictureDispatcher.cpp \
    X_Components/X_TranslatorManager.cpp

HEADERS  += \
    MainWindow.h \
    X_Components/X_AbstractPlotterDataManager.h \
    X_Components/X_MathExpressionHandler.h \
    X_Components/X_MathExpressionVariableListMaker.h \
    X_Components/X_XMLCalibrationProjectIOHandler.h \
    X_Dialogs/X_CustomTermDialog.h \
    X_General.h \
    X_Components/X_SpectrumArray.h \
    X_Components/X_FileActionManager.h \
    X_Components/X_AbstractSpectrumIOHandler.h \
    X_Components/X_SpeIOHandler.h \
    X_Components/X_SpeSpectrum.h \
    X_Components/X_AbstractSpectrumAuxData.h \
    X_Components/X_AbstractSpectrum.h \
    X_Components/X_SpeAuxData.h \
    X_Models/X_ArrayModel.h \
    X_Models/X_ChemElementModel.h \
    X_Models/X_CalibrationModel.h \
    X_Models/X_JointSpectraModel.h \
    X_ModelViews/X_SpectrumArrayWidget.h \
    X_ModelViews/X_CalibrationTableWidget.h \
    X_ModelViews/X_ChemElementWidget.h \
    X_ModelViews/X_JointSpectraTableWidget.h \
    X_Widgets/X_ClickableLabel.h \
    X_Widgets/X_MessagePanel.h \
    #X_Widgets/X_HelpBrowser.h \
    X_Widgets/X_WidgetWithSidebar.h \
    X_Components/X_RawSpectrumArray.h \
    X_Components/X_XMLSpectrumArrayIOHandler.h \
    X_Components/X_XMLCalibrationIOHandler.h \
    X_Components/X_CalibrationRepository.h \
    X_Components/X_JointSpectraDataManager.h \
    X_Components/X_ChemElementList.h \
    X_Components/X_ChemElementDataManager.h \
    X_ModelViews/X_NumericDelegate/X_NumericDelegate.h \
    X_ModelViews/X_NumericDelegate/X_NumericEditor.h \
    X_ModelViews/X_StringDelegate/X_StringDelegate.h \
    X_ModelViews/X_SpectrumTableDelegate/X_SpectrumTableDelegate.h \
    X_ModelViews/X_SpectrumTableDelegate/X_PrimitivePlot.h \
    X_ModelViews/X_VisibilityStringDelegate/X_VisibilityStringDelegate.h \
    X_ModelViews/X_ChemElementComboBoxDelegate/X_ChemElementComboBoxDelegate.h \
    X_ModelViews/X_WindowTypeComboBoxDelegate/X_WindowTypeComboBoxDelegate.h \
    X_Components/X_SpectrumPaintData.h \
    X_Plotter/X_GraphicsItemUserTypes.h \
    X_Plotter/X_PlotGraphicsScene.h \
    X_Plotter/X_PlotGraphicsView.h \
    X_Plotter/X_Plotter.h \
    X_Plotter/X_RulersAndGridManager.h \
    X_Plotter/X_RulerWidget.h \
    X_Plotter/X_SpectrumGraphicsItem.h \
    X_Plotter/X_WindowGraphicsItem.h \
    X_Plotter/X_HorizontalDashBoard.h \
    X_Components/X_Calibration.h \
    X_Components/X_SpectrumArrayRepository.h \
    X_Models/X_CalibrationWindowModel.h \
    X_Components/X_CalibrationWindowDataManager.h \
    X_Components/X_CalibrationWindow.h \
    X_ModelViews/X_CalibrationWindowTableWidget.h \
    X_Plotter/X_DefaultRectGraphicsItem.h \
    X_ModelViews/X_IntDelegate/X_ChannelNumberDelegate.h \
    X_Plotter/X_PlotterDefaultVariables.h \
    X_ModelViews/X_TermCorrelationTableWidget.h \
    X_Models/X_TermCorrelationTableModel.h \
    X_Components/X_TermCorrelationTableManager.h \
    X_Components/X_AbstractTerm.h \
    X_Components/X_TermNormalizer.h \
    X_Components/X_CustomTerm.h \
    X_Components/X_QuadraticTerm.h \
    X_Components/X_SimpleTerm.h \
    X_ModelViews/X_CustomCheckableVerticalHeaderView/X_CustomCheckableVerticalHeaderView.h \
    X_ModelViews/X_CorrelationPlotterWidget.h \
    X_ModelViews/X_EquationDelegate/X_EquationDelegate.h \
    X_Dialogs/X_EquationChooseDialog.h \
    X_Components/X_EquationSettingsData.h \
    X_Dialogs/X_NormaSettingsDialog.h \
    X_ModelViews/X_NormaDelegate/X_NormaDelegate.h \
    X_Components/X_NormaSettingsData.h \
    X_Components/X_CorrelationPlotterDataManager.h \
    X_Components/X_VisibilityPointF.h \
    X_Plotter/X_ChartPointGraphicsItem.h \
    X_Plotter/X_ChartPointOptions.h \
    X_Components/X_QrealToStringConverter.h \
    X_Components/X_CalibrationQualityData.h \
    X_Components/X_PasteData.h \
    X_Components/X_MixedTerm.h \
    X_ModelViews/X_ActiveStyledItemDelegate.h \
    X_Components/X_RawTerm.h \
    X_Components/X_RawWindow.h \
    X_Dialogs/X_SetGainFactorToCalibrationDialog.h \
    X_Components/X_LESGaussSolver.h \
    X_Plotter/X_EnergyLineGraphicsItem.h \
    X_Components/X_EnergyCalibrationLine.h \
    X_Plotter/X_TextGraphicsItem.h \
    X_Plotter/X_RulerToolGraphicsItem.h \
    X_EnergyCalibrator/X_EnergyCalibrationDialog.h \
    X_EnergyCalibrator/X_EnergyCalibrationDialogV2.h \
    X_EnergyCalibrator/X_EnergyCalibrationSpectrumTableModel.h \
    X_EnergyCalibrator/X_PeakWindow.h \
    X_EnergyCalibrator/X_PeakWindowTableModel.h \
    X_EnergyLineManager/X_EnergyLineDelegate.h \
    X_EnergyLineManager/X_EnergyLineManager.h \
    X_EnergyLineManager/X_EnergyLineSetItem.h \
    X_EnergyLineManager/X_EnergyLineTableWidget.h \
    X_EnergyLineManager/X_SelectedEnergyLineTableModel.h \
    X_PeriodicTable/X_AbstractChemicalElementPropertyRepository.h \
    X_PeriodicTable/X_BaseDialog.h \
    X_PeriodicTable/X_CaptionFormatter.h \
    X_PeriodicTable/X_ChemicalElementButton.h \
    X_PeriodicTable/X_ChemicalElementPropertyIOManager.h \
    X_PeriodicTable/X_ChemicalElementPropertyItem.h \
    X_PeriodicTable/X_ChemicalElementPropertyTreeModel.h \
    X_PeriodicTable/X_ChemicalElementProprtyEditor.h \
    X_PeriodicTable/X_ChemicalPropertyProxyTableModel.h \
    X_PeriodicTable/X_PeriodicTableWidget.h \
    X_PeriodicTable/X_PropertyEditDialog.h \
    X_PeriodicTable/X_PropertySectionEditDialog.h \
    X_PeriodicTableDialog/X_EnergyLineSelectionDialog.h \
    X_Widgets/X_ColorButton.h \
    X_Plotter/X_VerticalLineGraphicsItem.h \
    X_ModelViews/X_BaseTableWidget.h \
    X_Components/X_ControlAction.h \
    X_BaseMainWindow/X_BaseMainWindow.h \
    X_BaseMainWindow/X_HelpBrowser.h \
    X_Widgets/X_MessageWidget.h \
    X_Components/X_StandardMessagePictureDispatcher.h \
    X_Components/X_TranslatorManager.h


RESOURCES += \
    resources.qrc \
    X_PeriodicTable/ChemicalElementModelResources.qrc \
    X_BaseMainWindow/HelpBrowserResources.qrc

DISTFILES += \
    myBacklog.txt \
    X_PeriodicTable/chemicalElementBasicProperties.xml \
    X_PeriodicTable/chemicalElementBasicProperties.csv \
    X_BaseMainWindow/blue_home.png \
    X_BaseMainWindow/blue_left.png \
    X_BaseMainWindow/blue_right.png \
    X_BaseMainWindow/editclear.png \
    X_BaseMainWindow/green_home.png \
    X_BaseMainWindow/green_left.png \
    X_BaseMainWindow/green_right.png \
    X_BaseMainWindow/blue_home.ico \
    X_BaseMainWindow/blue_left.ico \
    X_BaseMainWindow/blue_right.ico \
    X_BaseMainWindow/green_home.ico \
    X_BaseMainWindow/green_left.ico \
    X_BaseMainWindow/green_right.ico

CONFIG(release, debug|release) {
    QMAKE_POST_LINK="C:\Qt\6.4.0\mingw_64\bin\windeployqt.exe "$${DESTDIR} $${DESTDIR}
}
