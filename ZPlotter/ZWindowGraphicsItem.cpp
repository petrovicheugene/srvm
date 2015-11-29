//======================================================
#include "ZWindowGraphicsItem.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
//======================================================
QColor ZWindowGraphicsItem::zv_selectionColor = QColor(Qt::red);
//======================================================
ZWindowGraphicsItem::ZWindowGraphicsItem(const QPair<double, double> &data, QColor color, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    zp_setData(data);
    zv_color = color;
    zv_topY = 1;
    zv_bottomY = -1;

    zh_recalcBoundingRect();
    zh_recalcShape();
}
//======================================================
QRectF ZWindowGraphicsItem::boundingRect() const
{
    return zv_boundingRect;
}
//======================================================
void ZWindowGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    painter->setClipRect( option->exposedRect );
    QColor lineColor = isSelected()? zv_selectionColor : zv_color;
    QPen pen(lineColor,1);
    pen.setCosmetic(true);
    painter->setPen(pen);

    QColor fillColor = lineColor;
    fillColor.setAlpha(100);
    painter->setBrush(QBrush(fillColor));

    painter->drawRect(zv_boundingRect);
//    QRectF fillRect;
//    QPointF topPoint(zv_data.first, zv_topY);
//    QPointF bottomPoint(zv_data.first, zv_bottomY);
//    painter->drawLine(topPoint, bottomPoint);

//    fillRect.setTopLeft(topPoint);
//    fillRect.setBottomLeft(bottomPoint);

//    topPoint.setX(zv_data.second);
//    bottomPoint.setX(zv_data.second);
//    painter->drawLine(topPoint, bottomPoint);

//    fillRect.setTopRight(topPoint);
//    fillRect.setBottomRight(bottomPoint);

//    painter->setPen(QPen(Qt::NoPen));
//    fillColor = lineColor;
//    fillColor.setAlpha(100);
//    painter->fillRect(fillRect, QBrush(fillColor));

    painter->restore();
}
//======================================================
QPainterPath ZWindowGraphicsItem::shape() const
{
    return zv_shape;
}
//======================================================
int	 ZWindowGraphicsItem::type() const
{
    return WindowItemType;
}
//======================================================
void ZWindowGraphicsItem::zp_setSelectionColor(QColor color)
{
    zv_selectionColor = color;
}
//======================================================
void ZWindowGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
//======================================================
void ZWindowGraphicsItem::zp_setData(const QPair<double, double> & data)
{
    zv_data = data;
    if(zv_data.first > zv_data.second)
    {
        qSwap(zv_data.first, zv_data.second);
    }
    update();
}
//======================================================
void ZWindowGraphicsItem::zp_setColor(QColor color)
{
    zv_color = color;
    update();
}
//======================================================
void ZWindowGraphicsItem::zp_fitInSceneRect(const QRectF& rect)
{
    zv_topY = rect.top() +1;
    zv_bottomY = rect.bottom() - 1;
    zh_recalcBoundingRect();
    zh_recalcShape();
    update();
}
//======================================================
void ZWindowGraphicsItem::zh_recalcShape()
{
    QPainterPath path;
//    QPointF topPoint(zv_data.first, zv_topY);
//    QPointF bottomPoint(zv_data.first, zv_bottomY);
//    path.moveTo(topPoint);
//    path.lineTo(bottomPoint);
//    topPoint.setX(zv_data.second);
//    bottomPoint.setX(zv_data.second);
//    path.moveTo(topPoint);
//    path.lineTo(bottomPoint);

    path.addRect(zv_boundingRect);
    zv_shape = path;
}
//======================================================
void ZWindowGraphicsItem::zh_recalcBoundingRect()
{
    QPointF topPoint(zv_data.first, zv_topY);
    QPointF bottomPoint(zv_data.first, zv_bottomY);
    zv_boundingRect.setTopLeft(topPoint);
    zv_boundingRect.setBottomLeft(bottomPoint);
    topPoint.setX(zv_data.second);
    bottomPoint.setX(zv_data.second);
    zv_boundingRect.setTopRight(topPoint);
    zv_boundingRect.setBottomRight(bottomPoint);
}
//======================================================
