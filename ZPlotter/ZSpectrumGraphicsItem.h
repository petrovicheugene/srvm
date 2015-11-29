//======================================================
#ifndef ZSPECTRUMGRAPHICSITEM_H
#define ZSPECTRUMGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "ZGraphicsItemUserTypes.h"
//======================================================
class ZSpectrumGraphicsItem : public QGraphicsItem
{
public:
    ZSpectrumGraphicsItem(QGraphicsItem * parent = 0);
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const;
    int	 type() const;

protected:

    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
};
//======================================================
#endif // ZSPECTRUMGRAPHICSITEM_H
