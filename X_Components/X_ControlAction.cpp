//======================================================
#include "X_ControlAction.h"
#include "QActionEvent"
#include <QDebug>
//======================================================
X_ControlAction::X_ControlAction(QObject *parent) :
    QAction(parent)
{
    xv_enabled = this->isEnabled();
}
//======================================================
bool X_ControlAction::event(QEvent *event)
{
    if(isEnabled() != xv_enabled)
    {
        xv_enabled = isEnabled();
        emit xg_enableChanged(xv_enabled);
    }

    return false;
}
//======================================================
