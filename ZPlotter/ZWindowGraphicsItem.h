//======================================================
#ifndef ZWINDOWGRAPHICSITEM_H
#define ZWINDOWGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "ZGraphicsItemUserTypes.h"
#include <QColor>
#include <QPair>
//======================================================
class ZWindowGraphicsItem : public QGraphicsItem
{
public:
    ZWindowGraphicsItem(const QPair<double, double> & data, QColor color, QGraphicsItem * parent = 0);
    // overrided
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const;
    int	 type() const;

    // FUNCS
    static void zp_setSelectionColor(QColor);

    void zp_setData(const QPair<double, double> &);
    void zp_setColor(QColor color);
    void zp_fitInSceneRect(const QRectF& rect);

protected:

    // VARS
    QColor zv_color;
    static QColor zv_selectionColor;
    double zv_topY;
    double zv_bottomY;
    QPainterPath zv_shape;
    QRectF zv_boundingRect;

    QPair<double, double> zv_data;

    // FUNCS
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    void zh_recalcShape();
    void zh_recalcBoundingRect();


};
//======================================================
#endif // ZWINDOWGRAPHICSITEM_H
