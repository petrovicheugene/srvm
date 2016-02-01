//===================================================================
#include "ZQuadraticPredictor.h"
#include "ZCalibrationWindow.h"
//===================================================================
ZQuadraticPredictor::ZQuadraticPredictor(const ZCalibrationWindow *window,
                                         ZCalibration *parent) :
    ZAbstractPredictor(parent)
{
    zv_type = PT_QUADRATIC;
    zv_name = "(" + window->zp_windowName() + ")" + QChar(0x00B2);

    zh_connectToWindow(window);

}
//===================================================================
qreal ZQuadraticPredictor::zp_calcValue()
{
    return 0;
}
//===================================================================
void ZQuadraticPredictor::zh_connectToWindow(const ZCalibrationWindow* window)
{
    connect(window, &ZCalibrationWindow::destroyed,
            this, &ZQuadraticPredictor::zh_onWindowDestroying);
}
//===================================================================

