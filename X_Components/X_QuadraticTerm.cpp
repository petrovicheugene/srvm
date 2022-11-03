//===================================================================
#include "X_QuadraticTerm.h"
#include "X_CalibrationWindow.h"
#include <math.h>
//===================================================================
X_QuadraticTerm::X_QuadraticTerm(const X_CalibrationWindow *window,
                               X_Calibration *parent) :
    X_AbstractTerm(parent)
{
    xv_type = TT_QUADRATIC;
    xv_window = window;
    xh_setName();
    xh_connectToWindow();
    xh_connectToCalibration(parent);
}
//===================================================================
bool X_QuadraticTerm::xp_calcValue(const X_AbstractSpectrum * spectrum, qreal& value)
{
    value = 0.0;
    if(xv_termState != TS_CONST_INCLUDED && xv_termState != TS_INCLUDED)
    {
        return true;
    }

    bool ok;
    emit xg_requestWindowIntensity((const QObject*)spectrum, value, true, &ok);
    if(!ok)
    {
        return false;
    }

    value = pow(value, 2.0) * xv_termFactor;
    return true;
}
//===================================================================
bool X_QuadraticTerm::xp_calcTermVariablePart(const X_AbstractSpectrum* spectrum, qreal& value)  // w/o factor
{
    value = 0.0;
    bool ok;
    emit xg_requestWindowIntensity((const QObject*)spectrum, value, false, &ok);
    value = value * value;
    return ok;
}
//===================================================================
bool X_QuadraticTerm::xp_termBelongsToWindow(const X_CalibrationWindow* window) const
{
    return window == xv_window;
}
//===================================================================
QList<qint64> X_QuadraticTerm::xp_termWindowIdList() const
{
    QList<qint64> windowIdList;
    windowIdList << xv_window->xp_windowId();
    return windowIdList;
}
//===================================================================
const X_CalibrationWindow* X_QuadraticTerm::xp_window() const
{
    return xv_window;
}
//===================================================================
void X_QuadraticTerm::xh_updateTermNameForWindowName()
{
    if(!sender())
    {
        return;
    }

    xh_setName();
    emit xg_termNameChanged();
}
//===================================================================
void X_QuadraticTerm::xh_setName()
{
    // xv_name = "(" + windowName + ")" + QChar(0x00B2);
    xv_name = xv_window->xp_windowName() + QChar(0x00B2);
}
//===================================================================
void X_QuadraticTerm::xh_onWindowTypeChange(X_CalibrationWindow::WindowType previousType,
                                           X_CalibrationWindow::WindowType currentType)
{
    if(!sender() || sender() != xv_window)
    {
        return;
    }

    if(currentType != X_CalibrationWindow::WT_PEAK)
    {
        emit xg_requestForDelete(this);
    }
}
//===================================================================
void X_QuadraticTerm::xh_connectToWindow()
{
    if(xv_window)
    {
        connect(xv_window, &X_CalibrationWindow::destroyed,
                this, &X_QuadraticTerm::xh_onWindowDestroying);
        connect(xv_window, &X_CalibrationWindow::xg_windowNameChanged,
                this, &X_QuadraticTerm::xh_updateTermNameForWindowName);
        connect(this, &X_QuadraticTerm::xg_requestWindowIntensity,
                xv_window, &X_CalibrationWindow::xp_calcWindowIntensity);
        connect(xv_window, &X_CalibrationWindow::xg_windowMarginsChanged,
                this, &X_QuadraticTerm::xg_termWindowMarginChanged);
        connect(xv_window, &X_CalibrationWindow::xg_windowTypeChanged,
                this, &X_QuadraticTerm::xh_onWindowTypeChange);
    }
}
//===================================================================

