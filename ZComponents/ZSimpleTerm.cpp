//===================================================================
#include "ZSimpleTerm.h"
#include "ZCalibrationWindow.h"
//===================================================================
ZSimpleTerm::ZSimpleTerm(const ZCalibrationWindow* window,
                         ZCalibration *parent) : ZAbstractTerm(parent)
{
    zv_window = window;
    zv_type = TT_SIMPLE;
    zv_name = window->zp_windowName();

    zh_connectToWindow(window);
}
//===================================================================
bool ZSimpleTerm::zp_calcValue(const ZAbstractSpectrum * spectrum, qreal& value)
{
    value = 0;
    if(zv_termState != TS_CONST_INCLUDED && zv_termState != TS_INCLUDED)
    {
        return true;
    }

    bool ok;
    emit zg_requestWindowIntensity((const QObject*)spectrum, value, true, &ok);
    if(!ok)
    {
        return false;
    }

    value = value * zv_termFactor;
    return true;
}
//===================================================================
bool ZSimpleTerm::zp_calcTermVariablePart(const ZAbstractSpectrum* spectrum, qreal &value)  // w/o factor
{
    bool ok;
    emit zg_requestWindowIntensity((const QObject*)spectrum, value, false, &ok);
    return ok;
}
//===================================================================
bool ZSimpleTerm::zp_termBelongsToWindow(const ZCalibrationWindow* window) const
{
    return window == zv_window;
}
//===================================================================
const ZCalibrationWindow* ZSimpleTerm::zp_window() const
{
    return zv_window;
}
//===================================================================
bool ZSimpleTerm::zh_updateTermNameForWindowName(const QString& windowName)
{
    if(windowName.isEmpty() || windowName == zv_name)
    {
        return false;
    }

    zv_name = windowName;
    emit zg_termNameChanged();
    return true;
}
//===================================================================
void ZSimpleTerm::zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType,
                                        ZCalibrationWindow::WindowType currentType)
{
    if(currentType != ZCalibrationWindow::WT_PEAK)
    {
        emit zg_requestForDelete(this);
    }
}
//===================================================================
void ZSimpleTerm::zh_connectToWindow(const ZCalibrationWindow* window)
{
    if(window)
    {
        connect(window, &ZCalibrationWindow::destroyed,
                this, &ZSimpleTerm::zh_onWindowDestroying);
        connect(window, &ZCalibrationWindow::zg_windowNameChanged,
                this, &ZSimpleTerm::zh_updateTermNameForWindowName);
        connect(this, &ZSimpleTerm::zg_requestWindowIntensity,
                window, &ZCalibrationWindow::zp_calcWindowIntensity);
        connect(window, &ZCalibrationWindow::zg_windowMarginsChanged,
                this, &ZSimpleTerm::zg_termWindowMarginChanged);
        connect(window, &ZCalibrationWindow::zg_windowTypeChanged,
                this, &ZSimpleTerm::zh_onWindowTypeChange);
    }
}
//===================================================================

