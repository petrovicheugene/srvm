#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T13:50:38
#
#-------------------------------------------------
QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#PRO VARS
RC_ICONS = "ZImages/SRVM-8.ico"

#Application version
VER_MAJ=1
VER_MIN=0
VER_PAT=8

PRODUCT_SHORT_NAME="SRVM"
PRODUCT_FILE_BASE_NAME="SRVM"
QMAKE_TARGET_PRODUCT="SRV M"
QMAKE_TARGET_DESCRIPTION="Creation of spectrometer calibration"
QMAKE_TARGET_COMPANY="TechnoAnalyt"
QMAKE_TARGET_COPYRIGHT="Copyright © $${QMAKE_TARGET_COMPANY} Ltd. 2015 - 2019. All rights reserved."
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
}

#END

#PRO VARS
#Application version
#RC_ICONS = "ZImages/SRVM-8.ico"

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

INCLUDEPATH += ZPlotter \
    ZModels \
    ZModelViews \
    ZWidgets \
    ZComponents \
    ZDialogs \
    ZModelViews/ZNumericDelegate \
    ZModelViews/ZStringDelegate \
    ZModelViews/ZSpectrumTableDelegate \
    ZModelViews/ZVisibilityStringDelegate \
    ZModelViews/ZChemElementComboBoxDelegate \
    ZModelViews/ZIntDelegate \
    ZModelViews/ZWindowTypeComboBoxDelegate \
    ZModelViews/ZCustomCheckableVerticalHeaderView \
    ZModelViews/ZEquationDelegate \
    ZModelViews/ZNormaDelegate \
    ZPeriodicTableDialog \
    ZPeriodicTable \
    ZEnergyLineManager \
    ZEnergyCalibrator \
    ZBaseMainWindow

SOURCES += main.cpp \
    MainWindow.cpp \
    ZComponents/ZMathExpressionHandler.cpp \
    ZComponents/ZMathExpressionVariableListMaker.cpp \
    ZComponents/ZXMLCalibrationProjectIOHandler.cpp \
    ZDialogs/ZCustomTermDialog.cpp \
    ZGeneral.cpp \
    ZComponents/ZSpectrumArray.cpp \
    ZComponents/ZFileActionManager.cpp \
    ZComponents/ZAbstractSpectrumIOHandler.cpp \
    ZComponents/ZSpeIOHandler.cpp \
    ZComponents/ZSpeSpectrum.cpp \
    ZComponents/ZAbstractSpectrumAuxData.cpp \
    ZComponents/ZAbstractSpectrum.cpp \
    ZComponents/ZSpeAuxData.cpp \
    ZModels/ZArrayModel.cpp \
    ZModels/ZChemElementModel.cpp \
    ZModels/ZCalibrationModel.cpp \
    ZModels/ZJointSpectraModel.cpp \
    ZModelViews/ZSpectrumArrayWidget.cpp \
    ZModelViews/ZCalibrationTableWidget.cpp \
    ZModelViews/ZChemElementWidget.cpp \
    ZModelViews/ZJointSpectraTableWidget.cpp \
    ZWidgets/ZClickableLabel.cpp \
    ZWidgets/ZMessagePanel.cpp \
    #ZWidgets/ZHelpBrowser.cpp \
    ZWidgets/ZWidgetWithSidebar.cpp \
    ZComponents/ZXMLSpectrumArrayIOHandler.cpp \
    ZComponents/ZXMLCalibrationIOHandler.cpp \
    ZComponents/ZCalibrationRepository.cpp \
    ZComponents/ZJointSpectraDataManager.cpp \
    ZComponents/ZChemElementList.cpp \
    ZComponents/ZChemElementDataManager.cpp \
    ZModelViews/ZNumericDelegate/ZNumericDelegate.cpp \
    ZModelViews/ZNumericDelegate/ZNumericEditor.cpp \
    ZModelViews/ZStringDelegate/ZStringDelegate.cpp \
    ZModelViews/ZSpectrumTableDelegate/ZSpectrumTableDelegate.cpp \
    ZModelViews/ZSpectrumTableDelegate/ZPrimitivePlot.cpp \
    ZModelViews/ZVisibilityStringDelegate/ZVisibilityStringDelegate.cpp \
    ZModelViews/ZChemElementComboBoxDelegate/ZChemElementComboBoxDelegate.cpp \
    ZModelViews/ZWindowTypeComboBoxDelegate/ZWindowTypeComboBoxDelegate.cpp \
    ZComponents/ZPlotterDataManager.cpp \
    ZPlotter/ZPlotGraphicsScene.cpp \
    ZPlotter/ZPlotGraphicsView.cpp \
    ZPlotter/ZPlotter.cpp \
    ZPlotter/ZRulersAndGridManager.cpp \
    ZPlotter/ZRulerWidget.cpp \
    ZPlotter/ZSpectrumGraphicsItem.cpp \
    ZPlotter/ZWindowGraphicsItem.cpp \
    ZPlotter/ZHorizontalDashBoard.cpp \
    ZComponents/ZCalibration.cpp \
    ZComponents/ZSpectrumArrayRepository.cpp \
    ZModels/ZCalibrationWindowModel.cpp \
    ZComponents/ZCalibrationWindowDataManager.cpp \
    ZComponents/ZCalibrationWindow.cpp \
    ZModelViews/ZCalibrationWindowTableWidget.cpp \
    ZPlotter/ZDefaultRectGraphicsItem.cpp \
    ZModelViews/ZIntDelegate/ZChannelNumberDelegate.cpp \
    ZModelViews/ZTermCorrelationTableWidget.cpp \
    ZModels/ZTermCorrelationTableModel.cpp \
    ZComponents/ZAbstractTerm.cpp \
    ZComponents/ZTermCorrelationTableManager.cpp \
    ZComponents/ZSimpleTerm.cpp \
    ZComponents/ZQuadraticTerm.cpp \
    ZComponents/ZCustomTerm.cpp \
    ZModelViews/ZCustomCheckableVerticalHeaderView/ZCustomCheckableVerticalHeaderView.cpp \
    ZComponents/ZTermNormalizer.cpp \
    ZModelViews/ZCorrelationPlotterWidget.cpp \
    ZModelViews/ZEquationDelegate/ZEquationDelegate.cpp \
    ZDialogs/ZEquationChooseDialog.cpp \
    ZComponents/ZEquationSettingsData.cpp \
    ZDialogs/ZNormaSettingsDialog.cpp \
    ZModelViews/ZNormaDelegate/ZNormaDelegate.cpp \
    ZComponents/ZNormaSettingsData.cpp \
    ZComponents/ZCorrelationPlotterDataManager.cpp \
    ZComponents/ZVisibilityPointF.cpp \
    ZPlotter/ZChartPointGraphicsItem.cpp \
    ZPlotter/ZChartPointOptions.cpp \
    ZComponents/ZQrealToStringConverter.cpp \
    ZComponents/ZCalibrationQualityData.cpp \
    ZComponents/ZPasteData.cpp \
    ZComponents/ZMixedTerm.cpp \
    ZModelViews/ZActiveStyledItemDelegate.cpp \
    ZComponents/ZRawTerm.cpp \
    ZComponents/ZRawWindow.cpp \
    ZDialogs/ZSetGainFactorToCalibrationDialog.cpp \
    ZComponents/ZLESGaussSolver.cpp \
    ZPlotter/ZEnergyLineGraphicsItem.cpp \
    ZComponents/ZEnergyCalibrationLine.cpp \
    ZPlotter/ZTextGraphicsItem.cpp \
    ZPlotter/ZRulerToolGraphicsItem.cpp \
    ZEnergyCalibrator/ZEnergyCalibrationDialog.cpp \
    ZEnergyCalibrator/ZEnergyCalibrationDialogV2.cpp \
    ZEnergyCalibrator/ZEnergyCalibrationSpectrumTableModel.cpp \
    ZEnergyCalibrator/ZEnergyCalibrator.cpp \
    ZEnergyCalibrator/ZPeakWindow.cpp \
    ZEnergyCalibrator/ZPeakWindowTableModel.cpp \
    ZEnergyLineManager/ZEnergyLineDelegate.cpp \
    ZEnergyLineManager/ZEnergyLineManager.cpp \
    ZEnergyLineManager/ZEnergyLineSetItem.cpp \
    ZEnergyLineManager/ZEnergyLineTableWidget.cpp \
    ZEnergyLineManager/ZSelectedEnergyLineTableModel.cpp \
    ZPeriodicTable/ZAbstractChemicalElementPropertyRepository.cpp \
    ZPeriodicTable/ZBaseDialog.cpp \
    ZPeriodicTable/ZCaptionFormatter.cpp \
    ZPeriodicTable/ZChemicalElementButton.cpp \
    ZPeriodicTable/ZChemicalElementPropertyIOManager.cpp \
    ZPeriodicTable/ZChemicalElementPropertyItem.cpp \
    ZPeriodicTable/ZChemicalElementPropertyTreeModel.cpp \
    ZPeriodicTable/ZChemicalElementProprtyEditor.cpp \
    ZPeriodicTable/ZChemicalPropertyProxyTableModel.cpp \
    ZPeriodicTable/ZPeriodicTableWidget.cpp \
    ZPeriodicTable/ZPropertyEditDialog.cpp \
    ZPeriodicTable/ZPropertySectionEditDialog.cpp \
    ZPeriodicTableDialog/ZEnergyLineSelectionDialog.cpp \
    ZWidgets/ZColorButton.cpp \
    ZPlotter/ZVerticalLineGraphicsItem.cpp \
    ZModelViews/ZBaseTableWidget.cpp \
    ZComponents/ZControlAction.cpp \
    ZBaseMainWindow/ZBaseMainWindow.cpp \
    ZBaseMainWindow/ZHelpBrowser.cpp \
    ZWidgets/ZMessageWidget.cpp \
    ZComponents/ZStandardMessagePictureDispatcher.cpp \
    ZComponents/ZTranslatorManager.cpp

HEADERS  += \
    MainWindow.h \
    ZComponents/ZMathExpressionHandler.h \
    ZComponents/ZMathExpressionVariableListMaker.h \
    ZComponents/ZXMLCalibrationProjectIOHandler.h \
    ZDialogs/ZCustomTermDialog.h \
    ZGeneral.h \
    ZComponents/ZSpectrumArray.h \
    ZComponents/ZFileActionManager.h \
    ZComponents/ZAbstractSpectrumIOHandler.h \
    ZComponents/ZSpeIOHandler.h \
    ZComponents/ZSpeSpectrum.h \
    ZComponents/ZAbstractSpectrumAuxData.h \
    ZComponents/ZAbstractSpectrum.h \
    ZComponents/ZSpeAuxData.h \
    ZModels/ZArrayModel.h \
    ZModels/ZChemElementModel.h \
    ZModels/ZCalibrationModel.h \
    ZModels/ZJointSpectraModel.h \
    ZModelViews/ZSpectrumArrayWidget.h \
    ZModelViews/ZCalibrationTableWidget.h \
    ZModelViews/ZChemElementWidget.h \
    ZModelViews/ZJointSpectraTableWidget.h \
    ZWidgets/ZClickableLabel.h \
    ZWidgets/ZMessagePanel.h \
    #ZWidgets/ZHelpBrowser.h \
    ZWidgets/ZWidgetWithSidebar.h \
    ZComponents/ZRawSpectrumArray.h \
    ZComponents/ZXMLSpectrumArrayIOHandler.h \
    ZComponents/ZXMLCalibrationIOHandler.h \
    ZComponents/ZCalibrationRepository.h \
    ZComponents/ZJointSpectraDataManager.h \
    ZComponents/ZChemElementList.h \
    ZComponents/ZChemElementDataManager.h \
    ZModelViews/ZNumericDelegate/ZNumericDelegate.h \
    ZModelViews/ZNumericDelegate/ZNumericEditor.h \
    ZModelViews/ZStringDelegate/ZStringDelegate.h \
    ZModelViews/ZSpectrumTableDelegate/ZSpectrumTableDelegate.h \
    ZModelViews/ZSpectrumTableDelegate/ZPrimitivePlot.h \
    ZModelViews/ZVisibilityStringDelegate/ZVisibilityStringDelegate.h \
    ZModelViews/ZChemElementComboBoxDelegate/ZChemElementComboBoxDelegate.h \
    ZModelViews/ZWindowTypeComboBoxDelegate/ZWindowTypeComboBoxDelegate.h \
    ZComponents/ZSpectrumPaintData.h \
    ZComponents/ZPlotterDataManager.h \
    ZPlotter/ZGraphicsItemUserTypes.h \
    ZPlotter/ZPlotGraphicsScene.h \
    ZPlotter/ZPlotGraphicsView.h \
    ZPlotter/ZPlotter.h \
    ZPlotter/ZRulersAndGridManager.h \
    ZPlotter/ZRulerWidget.h \
    ZPlotter/ZSpectrumGraphicsItem.h \
    ZPlotter/ZWindowGraphicsItem.h \
    ZPlotter/ZHorizontalDashBoard.h \
    ZComponents/ZCalibration.h \
    ZComponents/ZSpectrumArrayRepository.h \
    ZModels/ZCalibrationWindowModel.h \
    ZComponents/ZCalibrationWindowDataManager.h \
    ZComponents/ZCalibrationWindow.h \
    ZModelViews/ZCalibrationWindowTableWidget.h \
    ZPlotter/ZDefaultRectGraphicsItem.h \
    ZModelViews/ZIntDelegate/ZChannelNumberDelegate.h \
    ZPlotter/ZPlotterDefaultVariables.h \
    ZModelViews/ZTermCorrelationTableWidget.h \
    ZModels/ZTermCorrelationTableModel.h \
    ZComponents/ZTermCorrelationTableManager.h \
    ZComponents/ZAbstractTerm.h \
    ZComponents/ZTermNormalizer.h \
    ZComponents/ZCustomTerm.h \
    ZComponents/ZQuadraticTerm.h \
    ZComponents/ZSimpleTerm.h \
    ZModelViews/ZCustomCheckableVerticalHeaderView/ZCustomCheckableVerticalHeaderView.h \
    ZModelViews/ZCorrelationPlotterWidget.h \
    ZModelViews/ZEquationDelegate/ZEquationDelegate.h \
    ZDialogs/ZEquationChooseDialog.h \
    ZComponents/ZEquationSettingsData.h \
    ZDialogs/ZNormaSettingsDialog.h \
    ZModelViews/ZNormaDelegate/ZNormaDelegate.h \
    ZComponents/ZNormaSettingsData.h \
    ZComponents/ZCorrelationPlotterDataManager.h \
    ZComponents/ZVisibilityPointF.h \
    ZPlotter/ZChartPointGraphicsItem.h \
    ZPlotter/ZChartPointOptions.h \
    ZComponents/ZQrealToStringConverter.h \
    ZComponents/ZCalibrationQualityData.h \
    ZComponents/ZPasteData.h \
    ZComponents/ZMixedTerm.h \
    ZModelViews/ZActiveStyledItemDelegate.h \
    ZComponents/ZRawTerm.h \
    ZComponents/ZRawWindow.h \
    ZDialogs/ZSetGainFactorToCalibrationDialog.h \
    ZComponents/ZLESGaussSolver.h \
    ZPlotter/ZEnergyLineGraphicsItem.h \
    ZComponents/ZEnergyCalibrationLine.h \
    ZPlotter/ZTextGraphicsItem.h \
    ZPlotter/ZRulerToolGraphicsItem.h \
    ZEnergyCalibrator/ZEnergyCalibrationDialog.h \
    ZEnergyCalibrator/ZEnergyCalibrationDialogV2.h \
    ZEnergyCalibrator/ZEnergyCalibrationSpectrumTableModel.h \
    ZEnergyCalibrator/ZEnergyCalibrator.h \
    ZEnergyCalibrator/ZPeakWindow.h \
    ZEnergyCalibrator/ZPeakWindowTableModel.h \
    ZEnergyLineManager/ZEnergyLineDelegate.h \
    ZEnergyLineManager/ZEnergyLineManager.h \
    ZEnergyLineManager/ZEnergyLineSetItem.h \
    ZEnergyLineManager/ZEnergyLineTableWidget.h \
    ZEnergyLineManager/ZSelectedEnergyLineTableModel.h \
    ZPeriodicTable/ZAbstractChemicalElementPropertyRepository.h \
    ZPeriodicTable/ZBaseDialog.h \
    ZPeriodicTable/ZCaptionFormatter.h \
    ZPeriodicTable/ZChemicalElementButton.h \
    ZPeriodicTable/ZChemicalElementPropertyIOManager.h \
    ZPeriodicTable/ZChemicalElementPropertyItem.h \
    ZPeriodicTable/ZChemicalElementPropertyTreeModel.h \
    ZPeriodicTable/ZChemicalElementProprtyEditor.h \
    ZPeriodicTable/ZChemicalPropertyProxyTableModel.h \
    ZPeriodicTable/ZPeriodicTableWidget.h \
    ZPeriodicTable/ZPropertyEditDialog.h \
    ZPeriodicTable/ZPropertySectionEditDialog.h \
    ZPeriodicTableDialog/ZEnergyLineSelectionDialog.h \
    ZWidgets/ZColorButton.h \
    ZPlotter/ZVerticalLineGraphicsItem.h \
    ZModelViews/ZBaseTableWidget.h \
    ZComponents/ZControlAction.h \
    ZBaseMainWindow/ZBaseMainWindow.h \
    ZBaseMainWindow/ZHelpBrowser.h \
    ZWidgets/ZMessageWidget.h \
    ZComponents/ZStandardMessagePictureDispatcher.h \
    ZComponents/ZTranslatorManager.h


RESOURCES += \
    resources.qrc \
    ZPeriodicTable/ChemicalElementModelResources.qrc \
    ZBaseMainWindow/HelpBrowserResources.qrc

DISTFILES += \
    myBacklog.txt \
    ZPeriodicTable/chemicalElementBasicProperties.xml \
    ZPeriodicTable/chemicalElementBasicProperties.csv \
    ZBaseMainWindow/blue_home.png \
    ZBaseMainWindow/blue_left.png \
    ZBaseMainWindow/blue_right.png \
    ZBaseMainWindow/editclear.png \
    ZBaseMainWindow/green_home.png \
    ZBaseMainWindow/green_left.png \
    ZBaseMainWindow/green_right.png \
    ZBaseMainWindow/blue_home.ico \
    ZBaseMainWindow/blue_left.ico \
    ZBaseMainWindow/blue_right.ico \
    ZBaseMainWindow/green_home.ico \
    ZBaseMainWindow/green_left.ico \
    ZBaseMainWindow/green_right.ico
