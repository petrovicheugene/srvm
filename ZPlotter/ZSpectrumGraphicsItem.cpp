//======================================================
#include "ZSpectrumGraphicsItem.h"
//======================================================
ZSpectrumGraphicsItem::ZSpectrumGraphicsItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{

}
//======================================================
QRectF ZSpectrumGraphicsItem::boundingRect() const
{
    return QRectF();
}
//======================================================
void ZSpectrumGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{

}
//======================================================
QPainterPath ZSpectrumGraphicsItem::shape() const
{
    return QPainterPath();
}
//======================================================
int	 ZSpectrumGraphicsItem::type() const
{
    return SpectrumItemType;
}
//======================================================
void ZSpectrumGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
//======================================================
