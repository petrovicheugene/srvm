//========================================================
#ifndef ZDEFAULTRECTGRAPHICSITEM_H
#define ZDEFAULTRECTGRAPHICSITEM_H
//========================================================
#include <QGraphicsItem>
//========================================================
class ZDefaultRectGraphicsItem : public QGraphicsItem
{
public:
    ZDefaultRectGraphicsItem(QRectF, QGraphicsItem* parent=0);
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    void zp_setSceneRect(QRectF rect);

private:

    // VARS
    QRectF zv_boundingRect;
    QPainterPath zv_rectPainterPath;


};
//========================================================
#endif // ZDEFAULTRECTGRAPHICSITEM_H
