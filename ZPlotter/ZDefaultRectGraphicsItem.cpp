//========================================================
#include "ZDefaultRectGraphicsItem.h"
#include "ZGraphicsItemUserTypes.h"
//========================================================
ZDefaultRectGraphicsItem::ZDefaultRectGraphicsItem(QRectF rect, QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    zp_setSceneRect(rect);
}
//========================================================
QRectF ZDefaultRectGraphicsItem::boundingRect() const
{
    return zv_boundingRect;
}
//========================================================
void ZDefaultRectGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{

}
//========================================================
QPainterPath ZDefaultRectGraphicsItem::shape() const
{
    return zv_rectPainterPath;
}
//========================================================
int ZDefaultRectGraphicsItem::type() const
{
    return DefaultRectItemType;
}
//========================================================
void ZDefaultRectGraphicsItem::zp_setSceneRect(QRectF rect)
{
    if(!rect.isValid() || rect.isEmpty())
    {
        rect = QRectF(QPointF(0.0, -100.0), QPointF(2048, 0.0));
    }

    QPainterPath newPainterPath;
    newPainterPath.addRect(rect.normalized());
    zv_rectPainterPath = newPainterPath;
    zv_boundingRect = zv_rectPainterPath.boundingRect();
    update();
}
//========================================================
