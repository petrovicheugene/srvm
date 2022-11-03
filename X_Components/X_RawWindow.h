//===============================================
#ifndef X_RAWWINDOW_H
#define X_RAWWINDOW_H
//===============================================
#include <QString>
#include <X_CalibrationWindow.h>
//===============================================
class X_RawWindow
{
public:
    X_RawWindow();

    QString name;
    X_CalibrationWindow::WindowType windowType;
    QString firstChannel;
    QString lastChannel;

};
//===============================================
#endif // X_RAWWINDOW_H
