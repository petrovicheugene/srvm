//===================================================================
#include "ZSimpleTerm.h"
#include "ZCalibrationWindow.h"
//===================================================================
ZSimpleTerm::ZSimpleTerm(const ZCalibrationWindow* window,
                                   ZCalibration *parent) : ZAbstractTerm(parent)
{
   zv_type = PT_SIMPLE;
   zv_name = window->zp_windowName();

   zh_connectToWindow(window);
}
//===================================================================
qreal ZSimpleTerm::zp_calcValue()
{
   return 0;
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

}
//===================================================================

