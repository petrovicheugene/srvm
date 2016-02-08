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
void ZSimpleTerm::zh_connectToWindow(const ZCalibrationWindow* window)
{
    connect(window, &ZCalibrationWindow::destroyed,
            this, &ZSimpleTerm::zh_onWindowDestroying);
}
//===================================================================

