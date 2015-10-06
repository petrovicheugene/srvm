//==========================================================
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//==========================================================
#include <QMainWindow>
//==========================================================
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    // VARS


    // FUNCS
    void closeEvent(QCloseEvent* e);
    void zh_createActions();
    void zh_createComponents();
    void zh_createMenu();
    void zh_createToolbar();
    void zh_createConnections();
    void zh_restoreSettings();
    void zh_saveSettings();

};
//==========================================================
#endif // MAINWINDOW_H
