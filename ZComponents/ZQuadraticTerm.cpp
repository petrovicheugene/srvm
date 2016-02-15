//===================================================================
#include "ZQuadraticTerm.h"
#include "ZCalibrationWindow.h"
//===================================================================
ZQuadraticTerm::ZQuadraticTerm(const ZCalibrationWindow *window,
                               ZCalibration *parent) :
    ZAbstractTerm(parent)
{
    zv_type = TT_QUADRATIC;
    zh_setName(window->zp_windowName());
    zh_connectToWindow(window);
    zh_connectToCalibration(parent);
}
//===================================================================
qreal ZQuadraticTerm::zp_calcValue()
{
    return 0;
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
void ZQuadraticTerm::zh_connectToWindow(const ZCalibrationWindow* window)
{
    connect(window, &ZCalibrationWindow::destroyed,
            this, &ZQuadraticTerm::zh_onWindowDestroying);
    connect(window, &ZCalibrationWindow::zg_windowNameChanged,
            this, &ZQuadraticTerm::zh_updateTermNameForWindowName);

}
//===================================================================

