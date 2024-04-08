//===================================================================
#include "X_MixedTerm.h"
//===================================================================
X_MixedTerm::X_MixedTerm(const X_CalibrationWindow* window1,
                       const X_CalibrationWindow* window2,
                       X_Calibration *parent) : X_AbstractTerm(parent)
{
    xv_type = TT_MIXED;
    xv_window1 = window1;
    xv_window2 = window2;

    xh_setName();
    xh_connectToWindows();
    xh_connectToCalibration(parent);
}
//===================================================================
bool X_MixedTerm::xp_calcValue(const X_AbstractSpectrum * spectrum, qreal& value)
{
    value = 0.0;
    if(xv_termState != TS_CONST_INCLUDED && xv_termState != TS_INCLUDED)
    {
        return true;
    }

    bool ok;
    qreal win1Intensity = 0.0;

    emit xg_requestWindow1Intensity((const QObject*)spectrum, win1Intensity,  &ok);
    if(!ok)
    {
        return false;
    }

    qreal win2Intensity = 0.0;
    emit xg_requestWindow2Intensity((const QObject*)spectrum, win2Intensity,  &ok);
    if(!ok)
    {
        return false;
    }

    value = win1Intensity * win2Intensity * xv_termFactor;
    return true;
}
//===================================================================
bool X_MixedTerm::xp_calcTermIntensity(const X_AbstractSpectrum* spectrum, qreal &value)  // w/o factor
{
    value= 0.0;
    bool ok;
    qreal win1Intensity = 0.0;
    emit xg_requestWindow1Intensity((const QObject*)spectrum, win1Intensity, &ok);
    if(!ok)
    {
        return false;
    }

    qreal win2Intensity = 0.0;
    emit xg_requestWindow2Intensity((const QObject*)spectrum, win2Intensity, &ok);
    if(!ok)
    {
        return false;
    }

    value = win1Intensity * win2Intensity;
    return ok;
}
//===================================================================
bool X_MixedTerm::xp_termBelongsToWindow(const X_CalibrationWindow* window) const
{
    return window == xv_window1 || window == xv_window2;
}
//===================================================================
QList<qint64> X_MixedTerm::xp_termWindowIdList() const
{
    QList<qint64> windowIdList;
    windowIdList << xv_window1->xp_windowId();
    windowIdList << xv_window2->xp_windowId();
    return windowIdList;
}
//===================================================================
void X_MixedTerm::xh_updateTermNameForWindowName()
{
    if(!sender())
    {
        return;
    }
    else if(sender() != xv_window1 && sender() != xv_window2)
    {
        return;
    }

    xh_setName();
    emit xg_termNameChanged();
}
//===================================================================
void X_MixedTerm::xh_onWindowTypeChange(X_CalibrationWindow::WindowType previousType,
                                   X_CalibrationWindow::WindowType currentType)
{
    if(!sender() || (sender() != xv_window1 && sender() != xv_window2))
    {
        return;
    }

    if(currentType != X_CalibrationWindow::WT_PEAK)
    {
        emit xg_requestForDelete(this);
    }
}
//===================================================================
void X_MixedTerm::xh_setName()
{
    xv_name = xv_window1->xp_windowName() +"*"+ xv_window2->xp_windowName();
}
//===================================================================
void X_MixedTerm::xh_connectToWindows()
{
    if(xv_window1)
    {
        connect(xv_window1, &X_CalibrationWindow::destroyed,
                this, &X_MixedTerm::xh_onWindowDestroying);
        connect(xv_window1, &X_CalibrationWindow::xg_windowNameChanged,
                this, &X_MixedTerm::xh_updateTermNameForWindowName);
        connect(this, &X_MixedTerm::xg_requestWindow1Intensity,
                xv_window1, &X_CalibrationWindow::xp_calcWindowIntensity);
        connect(xv_window1, &X_CalibrationWindow::xg_windowMarginsChanged,
                this, &X_MixedTerm::xg_termWindowMarginChanged);
        connect(xv_window1, &X_CalibrationWindow::xg_windowTypeChanged,
                this, &X_MixedTerm::xh_onWindowTypeChange);
    }

    if(xv_window2)
    {
        connect(xv_window2, &X_CalibrationWindow::destroyed,
                this, &X_MixedTerm::xh_onWindowDestroying);
        connect(xv_window2, &X_CalibrationWindow::xg_windowNameChanged,
                this, &X_MixedTerm::xh_updateTermNameForWindowName);
        connect(this, &X_MixedTerm::xg_requestWindow2Intensity,
                xv_window2, &X_CalibrationWindow::xp_calcWindowIntensity);
        connect(xv_window2, &X_CalibrationWindow::xg_windowMarginsChanged,
                this, &X_MixedTerm::xg_termWindowMarginChanged);
        connect(xv_window2, &X_CalibrationWindow::xg_windowTypeChanged,
                this, &X_MixedTerm::xh_onWindowTypeChange);
    }

}
//===================================================================
