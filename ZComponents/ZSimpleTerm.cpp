//===================================================================
#include "ZSimpleTerm.h"
#include "ZCalibrationWindow.h"
//===================================================================
ZSimpleTerm::ZSimpleTerm(const ZCalibrationWindow* window,
                         ZCalibration *parent) : ZAbstractTerm(parent)
{
    zv_type = TT_SIMPLE;
    zv_name = window->zp_windowName();

    zh_connectToWindow(window);
}
//===================================================================
bool ZSimpleTerm::zp_calcValue(const ZAbstractSpectrum * spectrum, qreal& value)
{


    return 0;
}
//===================================================================
bool ZSimpleTerm::zp_calcTermVariablePart(const ZAbstractSpectrum* spectrum, qint64& value)  // w/o factor
{
    bool ok;
    emit zg_requestWindowIntensity((const QObject*)spectrum, value, &ok);
    return ok;
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
void ZSimpleTerm::zh_connectToWindow(const ZCalibrationWindow* window)
{
    connect(window, &ZCalibrationWindow::destroyed,
            this, &ZSimpleTerm::zh_onWindowDestroying);
    connect(window, &ZCalibrationWindow::zg_windowNameChanged,
            this, &ZSimpleTerm::zh_updateTermNameForWindowName);
    connect(this, &ZSimpleTerm::zg_requestWindowIntensity,
            window, &ZCalibrationWindow::zp_calcWindowIntensity);
    connect(window, &ZCalibrationWindow::zg_widowMarginsChanged,
            this, &ZSimpleTerm::zg_invokeAverageValueRecalc);
}
//===================================================================

