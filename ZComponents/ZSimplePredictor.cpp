//===================================================================
#include "ZSimplePredictor.h"
#include "ZCalibrationWindow.h"
//===================================================================
ZSimplePredictor::ZSimplePredictor(const ZCalibrationWindow* window,
                                   ZCalibration *parent) : ZAbstractPredictor(parent)
{
   zv_type = PT_SIMPLE;
   zv_name = window->zp_windowName();

   zh_connectToWindow(window);
}
//===================================================================
qreal ZSimplePredictor::zp_calcValue()
{
   return 0;
}
//===================================================================
void ZSimplePredictor::zh_connectToWindow(const ZCalibrationWindow* window)
{
    connect(window, &ZCalibrationWindow::destroyed,
            this, &ZSimplePredictor::zh_onWindowDestroying);
}
//===================================================================

