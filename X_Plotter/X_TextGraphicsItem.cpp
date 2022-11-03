//======================================================
#include "X_TextGraphicsItem.h"
#include "X_GraphicsItemUserTypes.h"

#include <QApplication>
#include <QDebug>
#include <QFontMetrics>
#include <QPainter>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QTextItem>
//======================================================
X_TextGraphicsItem::X_TextGraphicsItem(const QString& text, QGraphicsItem * parent)
    : QGraphicsItem(parent)
{
    xv_text = text;
    xv_font = qApp->font();
    xv_textPixelSize = 6;
    zh_recalcShapeAndBoundingRect();
}
//======================================================
X_TextGraphicsItem::X_TextGraphicsItem(QGraphicsItem * parent)
{
    xv_font = qApp->font();
    xv_textPixelSize = 6;
    zh_recalcShapeAndBoundingRect();
}
//======================================================
QRectF X_TextGraphicsItem::boundingRect() const
{
    return xv_boundingRect;
}
//======================================================
void X_TextGraphicsItem::paint(QPainter * painter,
                              const QStyleOptionGraphicsItem * option,
                              QWidget * widget)
{
    painter->save();

    QRect bkgRect = option->rect;
    bkgRect.adjust(-1, +2, 0, 0);

    QColor bkgColor(255, 255, 255, 255);
    painter->setPen(QPen(bkgColor, 1));
    painter->setBrush(QBrush(bkgColor));
    painter->drawRect(bkgRect);

    painter->setPen(QPen(Qt::black, 1));
    painter->setFont(xv_font);
    painter->drawText(option->rect, Qt::AlignCenter, xv_text);

    painter->restore();
}
//======================================================
QPainterPath X_TextGraphicsItem::shape() const
{
    return xv_shape;
}
//======================================================
int X_TextGraphicsItem::type() const
{
    return TextItemType;
}
//======================================================
void X_TextGraphicsItem::zh_recalcShapeAndBoundingRect()
{
    QFontMetrics fm(xv_font);
    //xv_boundingRect = fm.boundingRect(xv_text);
    xv_boundingRect = QRect(0, 0, fm.horizontalAdvance(xv_text), fm.height());
    xv_boundingRect.moveCenter(xv_boundingRect.topLeft());

    QPainterPath newShapePainterPath;
    newShapePainterPath.addRect(xv_boundingRect);
    xv_shape = newShapePainterPath;

//    QPainterPath newTextPainterPath;
//    newTextPainterPath.addText(xv_boundingRect.topLeft(), xv_font, xv_text);
//    xv_textPainterPath = newTextPainterPath;

}
//======================================================
void X_TextGraphicsItem::xp_updateItem()
{
    zh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
void X_TextGraphicsItem::xp_setText(const QString& text)
{
    xv_text = text;
    xp_updateItem();
}
//======================================================
void X_TextGraphicsItem::xp_setFont(QFont font)
{
    xv_font = font;
    xp_updateItem();
}
//======================================================
void X_TextGraphicsItem::xp_setTextPixelSize(int pixelSize)
{
    xv_textPixelSize = pixelSize;
    xp_updateItem();
}
//======================================================
void X_TextGraphicsItem::xp_setColor(QColor color)
{
    xv_color = color;
    xp_updateItem();
}
//======================================================
