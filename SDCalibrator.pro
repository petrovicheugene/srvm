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
VER_PAT=0
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
    ZComponents

SOURCES += main.cpp \
    MainWindow.cpp \
    ZComponents/ZSpectrumArray.cpp \
    ZComponents/ZAbstractArrayIOHandler.cpp \
    ZComponents/ZXMLArrayIOHandler.cpp \
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
    ZPlotter/ZPlotter.cpp \
    ZComponents/ZSpectraArrayRepository.cpp \
    ZComponents/ZSpectraJointDataManager.cpp


HEADERS  += glVariables.h \
    MainWindow.h \
    ZComponents/ZSpectrumArray.h \
    ZComponents/ZAbstractArrayIOHandler.h \
    ZComponents/ZXMLArrayIOHandler.h \
    ZComponents/ZFileActionManager.h \
    ZComponents/ZAbstractSpectrumIOHandler.h \
    ZComponents/ZSpeIOHandler.h \
    ZComponents/ZSpeSpectrum.h \
    ZComponents/ZAbstractSpectrumAuxData.h \
    ZComponents/ZAbstractSpectrum.h \
    ZComponents/ZSpeAuxData.h \
    ZComponents/ZRawArray.h \
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
    ZPlotter/ZPlotter.h \
    ZComponents/ZSpectraArrayRepository.h \
    ZComponents/ZSpectraJointDataManager.h


RESOURCES += \
    resources.qrc
