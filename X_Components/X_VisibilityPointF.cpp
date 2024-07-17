//===============================================================
#include "X_VisibilityPointF.h"
//===============================================================
X_VisibilityPointF::X_VisibilityPointF() : QPointF()
{

}
//===============================================================
X_VisibilityPointF::X_VisibilityPointF(const QPoint &point, bool visible, Qt::CheckState checkState ) : QPointF(point)
{
    xv_visibility = visible;
    xv_checkState = checkState;
}
//===============================================================
X_VisibilityPointF::X_VisibilityPointF(qreal xpos, qreal ypos, bool visible, Qt::CheckState checkState) : QPointF(xpos, ypos)
{
    xv_visibility = visible;
    xv_checkState = checkState;
}
//===============================================================
X_VisibilityPointF::X_VisibilityPointF(const QPoint &point, bool visible, bool checkState ) : QPointF(point)
{
    xv_visibility = visible;
    xv_checkState = checkState? Qt::Checked : Qt::Unchecked;
}
//===============================================================
X_VisibilityPointF::X_VisibilityPointF(qreal xpos, qreal ypos, bool visible, bool checkState ) : QPointF(xpos, ypos)
{
    xv_visibility = visible;
    xv_checkState = checkState? Qt::Checked : Qt::Unchecked;
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
};
//===============================================================
Qt::CheckState X_VisibilityPointF::xp_isChecked() const
{
    return xv_checkState;
}
//===============================================================
void X_VisibilityPointF::xp_setChecked(Qt::CheckState checkState)
{
    xv_checkState = checkState;
}
//===============================================================
