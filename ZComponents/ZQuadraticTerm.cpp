//===================================================================
#include "ZQuadraticTerm.h"
#include "ZCalibrationWindow.h"
//===================================================================
ZQuadraticTerm::ZQuadraticTerm(const ZCalibrationWindow *window,
                               ZCalibration *parent) :
    ZAbstractTerm(parent)
{
    zv_type = TT_QUADRATIC;
    zv_window = window;
    zh_setName(window->zp_windowName());
    zh_connectToWindow(window);
    zh_connectToCalibration(parent);
}
//===================================================================
bool ZQuadraticTerm::zp_calcValue(const ZAbstractSpectrum * spectrum, qreal& value)
{


    return 0;
}
//===================================================================
bool ZQuadraticTerm::zp_calcTermVariablePart(const ZAbstractSpectrum* spectrum, qint64& value)  // w/o factor
{
    bool ok;
    emit zg_requestWindowIntensity((const QObject*)spectrum, value, true, &ok);
    value = value * value;
    return ok;
}
//===================================================================
bool ZQuadraticTerm::zp_termBelongsToWindow(const ZCalibrationWindow* window) const
{
    return window == zv_window;
}
//===================================================================
const ZCalibrationWindow* ZQuadraticTerm::zp_window() const
{
    return zv_window;
}
//===================================================================
bool ZQuadraticTerm::zh_updateTermNameForWindowName(const QString& windowName)
{
    if(windowName.isEmpty() || windowName == zv_name)
    {
        return false;
    }

    zh_setName(windowName);
    emit zg_termNameChanged();

    return true;
}
//===================================================================
void ZQuadraticTerm::zh_setName(const QString& windowName)
{
    zv_name = "(" + windowName + ")" + QChar(0x00B2);
}
//===================================================================
void ZQuadraticTerm::zh_onWindowTypeChange(ZCalibrationWindow::WindowType type)
{
    if(type != ZCalibrationWindow::WT_BASE_PEAK && type != ZCalibrationWindow::WT_PEAK)
    {
        emit zg_requestForDelete(this);
    }
}
//===================================================================
void ZQuadraticTerm::zh_connectToWindow(const ZCalibrationWindow* window)
{
    if(window)
    {
        connect(window, &ZCalibrationWindow::destroyed,
                this, &ZQuadraticTerm::zh_onWindowDestroying);
        connect(window, &ZCalibrationWindow::zg_windowNameChanged,
                this, &ZQuadraticTerm::zh_updateTermNameForWindowName);
        connect(this, &ZQuadraticTerm::zg_requestWindowIntensity,
                window, &ZCalibrationWindow::zp_calcWindowIntensity);
        connect(window, &ZCalibrationWindow::zg_widowMarginsChanged,
                this, &ZQuadraticTerm::zg_termWindowMarginChanged);
        connect(window, &ZCalibrationWindow::zg_windowTypeChanged,
                this, &ZQuadraticTerm::zh_onWindowTypeChange);
    }
}
//===================================================================

