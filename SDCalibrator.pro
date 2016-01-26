#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T13:50:38
#
#-------------------------------------------------
QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#PRO VARS
#Application version
RC_ICONS = "SDC1.ico"

VER_MAJ=0
VER_MIN=0
VER_PAT=2
VER_RELEASE=a

EXE_BASE_NAME=SDCalibrator
QMAKE_TARGET_PRODUCT="SD Calibrator"
QMAKE_TARGET_DESCRIPTION="Creates calibrations for the SRV stream analyser"
QMAKE_TARGET_COMPANY="TechnoAnalyt"
QMAKE_TARGET_COPYRIGHT="Copyright © $${QMAKE_TARGET_COMPANY} Ltd. 2015.  All rights reserved."
COMPANY_URL=tehnoanalit.com

CONFIG += $$VER_RELEASE
CONFIG += c++11

VERSION=$${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

#Target version
r{
TARGET=$${EXE_BASE_NAME}-$${VERSION}
}else{
TARGET=$${EXE_BASE_NAME}-$${VERSION}.$${VER_RELEASE}
}

#Define the preprocessor macro to get the application version in our application.
DEFINES += APP_PRODUCT=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\"
DEFINES += APP_EXE_BASE_NAME=\"\\\"$${EXE_BASE_NAME}\\\"\"
DEFINES += APP_VERSION=\"\\\"$${VERSION}.$${VER_RELEASE}\\\"\"
DEFINES += APP_COMPANY=\"\\\"$${QMAKE_TARGET_COMPANY}\\\"\"
DEFINES += APP_COMPANY_URL=\"\\\"$${COMPANY_URL}\\\"\"
DEFINES += APP_COPYRIGHT=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\"

TRANSLATIONS = $${EXE_BASE_NAME}_ru.ts
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

#END OF CHIEF SETTINGS

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
    ZModelViews/ZWindowTypeComboBoxDelegate

SOURCES += main.cpp \
    MainWindow.cpp \
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
    ZWidgets/ZHelpBrowser.cpp \
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
    ZComponents/ZAdvancedAction.cpp \
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
    ZComponents/ZAbstractPredictor.cpp \
    ZComponents/ZSimplePredictor.cpp \
    ZComponents/ZQuadraticPredictor.cpp \
    ZComponents/ZMixedPredictor.cpp \
    ZComponents/ZCustomPredictor.cpp \
    ZComponents/ZPredictorNormalizator.cpp


HEADERS  += \
    MainWindow.h \
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
    ZWidgets/ZHelpBrowser.h \
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
    globalVariables.h \
    ZComponents/ZPlotterDataManager.h \
    ZComponents/ZAdvancedAction.h \
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
    ZPlotter/ZPlotterDefaulVariables.h \
    ZComponents/ZAbstractPredictor.h \
    ZComponents/ZSimplePredictor.h \
    ZComponents/ZQuadraticPredictor.h \
    ZComponents/ZMixedPredictor.h \
    ZComponents/ZCustomPredictor.h \
    ZComponents/ZPredictorNormalizator.h


RESOURCES += \
    resources.qrc

DISTFILES += \
    PLAN
