//==========================================================
#include "MainWindow.h"
#include "glVariables.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QAction>
//==========================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
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
    QString questionString = tr("Application will be closed. Are you sure?");
    if(QMessageBox::question(this, tr("Application exit"), questionString,
                             QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
    {
        e->ignore();
        return;
    }
}
//==========================================================
void MainWindow::zh_createActions()
{

}
//==========================================================
void MainWindow::zh_createComponents()
{

}
//==========================================================
void MainWindow::zh_createMenu()
{

}
//==========================================================
void MainWindow::zh_createToolbar()
{

}
//==========================================================
void MainWindow::zh_createConnections()
{

}
//==========================================================
void MainWindow::zh_restoreSettings()
{

}
//==========================================================
void MainWindow::zh_saveSettings()
{

}
//==========================================================
