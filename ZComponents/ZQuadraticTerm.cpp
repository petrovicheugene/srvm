//===================================================================
#include "ZQuadraticTerm.h"
#include "ZCalibrationWindow.h"
//===================================================================
ZQuadraticTerm::ZQuadraticTerm(const ZCalibrationWindow *window,
                                         ZCalibration *parent) :
    ZAbstractTerm(parent)
{
    zv_type = PT_QUADRATIC;
    zv_name = "(" + window->zp_windowName() + ")" + QChar(0x00B2);

    zh_connectToWindow(window);

}
//===================================================================
qreal ZQuadraticTerm::zp_calcValue()
{
    return 0;
}
//===================================================================
void ZQuadraticTerm::zh_connectToWindow(const ZCalibrationWindow* window)
{
    connect(window, &ZCalibrationWindow::destroyed,
            this, &ZQuadraticTerm::zh_onWindowDestroying);
}
//===================================================================

