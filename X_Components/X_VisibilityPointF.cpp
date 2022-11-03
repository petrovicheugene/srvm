//===============================================================
#include "X_VisibilityPointF.h"
//===============================================================
X_VisibilityPointF::X_VisibilityPointF() : QPointF()
{
    xv_visibility = true;
}
//===============================================================
X_VisibilityPointF::X_VisibilityPointF(const QPoint &point, bool visible) : QPointF(point)
{
    xv_visibility = visible;
}
//===============================================================
X_VisibilityPointF::X_VisibilityPointF(qreal xpos, qreal ypos, bool visible) : QPointF(xpos, ypos)
{
    xv_visibility = visible;
}
//===============================================================
bool X_VisibilityPointF::xp_isVisible() const
{
    return xv_visibility;
}
//===============================================================
void X_VisibilityPointF::xp_setVisible(bool visible)
{
    xv_visibility = visible;
}
//===============================================================
