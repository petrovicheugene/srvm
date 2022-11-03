//==================================================
#include "X_PeakWindow.h"
//==================================================
X_PeakWindow::X_PeakWindow(QObject *parent) : QObject(parent)
{
    xv_peakName = "Unknown";
}
//==================================================
bool X_PeakWindow::xp_setPeakName(const QString& peakName)
{
    if(peakName.isEmpty())
    {
        return false;
    }

    xv_peakName = peakName;
    return true;
}
//==================================================
QString X_PeakWindow::xp_peakName() const
{
    return xv_peakName;
}
//==================================================
