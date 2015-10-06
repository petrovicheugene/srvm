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
#RC_ICONS = "CMLogo.ico"

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

SOURCES += main.cpp \
    MainWindow.cpp

HEADERS  += \
    glVariables.h \
    MainWindow.h
