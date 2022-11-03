//=================================================
#include "X_ClickableLabel.h"
#include <QMoveEvent>
//=================================================
X_ClickableLabel::X_ClickableLabel(QWidget* parent) : QLabel(parent)
{

}
//=================================================
void X_ClickableLabel::mouseReleaseEvent(QMouseEvent* e)
{
    emit clicked();
}
//=================================================
