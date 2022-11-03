//===========================================================
#include "X_ColorButton.h"

#include <QPaintEvent>
#include <QPainter>
#include <QColorDialog>

//===========================================================
X_ColorButton::X_ColorButton(QWidget *parent) : QWidget(parent)
{
    xp_setSizeHint(16);
    xv_color = QColor(Qt::transparent);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}
//===========================================================
void X_ColorButton::xp_setSizeHint(int size)
{
    if(size > 3)
    {
        xv_sizeHint = QSize(size, size);
    }
}
//===========================================================
void X_ColorButton::xp_setColor(QColor color)
{
    xv_color = color;
    update();
    emit xg_colorChangedNotification();
    emit xg_colorChanged(xv_color);
}
//===========================================================
QColor X_ColorButton::xp_color() const
{
    return xv_color;
}
//===========================================================
void X_ColorButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPen pen = QPen(QBrush(QColor(Qt::black)), 1);
    painter.setPen(pen);

    if(isEnabled())
    {
        painter.setBrush(QBrush(xv_color));
    }
    else
    {
        int gray = qGray(xv_color.rgb());
        painter.setBrush(QBrush(QColor(gray, gray, gray, 255)));
    }

    painter.drawRect(event->rect().adjusted(0,0, -1, -1));
}
//===========================================================
QSize X_ColorButton::sizeHint() const
{
    return xv_sizeHint;
}
//===========================================================
void X_ColorButton::mouseReleaseEvent(QMouseEvent *)
{
    xp_runColorSelectionDialog();
}
//===========================================================
void X_ColorButton::xp_runColorSelectionDialog()
{
    QColor color = QColorDialog::getColor(xv_color, this, tr("Select color"));
    if(!color.isValid())
    {
        return;
    }

    xp_setColor(color);
}
//===========================================================
