//===================================================================
#include "X_SimpleTerm.h"
#include "X_CalibrationWindow.h"
//===================================================================
X_SimpleTerm::X_SimpleTerm(const X_CalibrationWindow* window,
                         X_Calibration *parent) : X_AbstractTerm(parent)
{
    xv_window = window;
    xv_type = TT_SIMPLE;
    xv_name = window->xp_windowName();

    zh_connectToWindow(window);
}
//===================================================================
bool X_SimpleTerm::xp_calcValue(const X_AbstractSpectrum * spectrum, qreal& value)
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

    value = value * xv_termFactor;
    return true;
}
//===================================================================
bool X_SimpleTerm::xp_calcTermVariablePart(const X_AbstractSpectrum* spectrum, qreal &value)  // w/o factor
{
    value = 0;
    bool ok;
    emit xg_requestWindowIntensity((const QObject*)spectrum, value, false, &ok);
    return ok;
}
//===================================================================
bool X_SimpleTerm::xp_termBelongsToWindow(const X_CalibrationWindow* window) const
{
    return window == xv_window;
}
//===================================================================
QList<qint64> X_SimpleTerm::xp_termWindowIdList() const
{
    QList<qint64> windowIdList;
    windowIdList << xv_window->xp_windowId();
    return windowIdList;
}
//===================================================================
const X_CalibrationWindow* X_SimpleTerm::xp_window() const
{
    return xv_window;
}
//===================================================================
void X_SimpleTerm::zh_updateTermNameForWindowName()
{
    if(!sender() || xv_window->xp_windowName() == xv_name)
    {
        return;
    }

    xv_name = xv_window->xp_windowName();
    emit xg_termNameChanged();
}
//===================================================================
void X_SimpleTerm::zh_onWindowTypeChange(X_CalibrationWindow::WindowType previousType,
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
void X_SimpleTerm::zh_connectToWindow(const X_CalibrationWindow* window)
{
    if(window)
    {
        connect(window, &X_CalibrationWindow::destroyed,
                this, &X_SimpleTerm::zh_onWindowDestroying);
        connect(window, &X_CalibrationWindow::xg_windowNameChanged,
                this, &X_SimpleTerm::zh_updateTermNameForWindowName);
        connect(this, &X_SimpleTerm::xg_requestWindowIntensity,
                window, &X_CalibrationWindow::xp_calcWindowIntensity);
        connect(window, &X_CalibrationWindow::xg_windowMarginsChanged,
                this, &X_SimpleTerm::xg_termWindowMarginChanged);
        connect(window, &X_CalibrationWindow::xg_windowTypeChanged,
                this, &X_SimpleTerm::zh_onWindowTypeChange);
    }
}
//===================================================================

