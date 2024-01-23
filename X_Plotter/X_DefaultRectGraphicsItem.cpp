//========================================================
#include "X_DefaultRectGraphicsItem.h"
#include "X_GraphicsItemUserTypes.h"
#include <QPainter>
//========================================================
X_DefaultRectGraphicsItem::X_DefaultRectGraphicsItem(const QRectF& rect,
                                                   bool XAxesVisible,
                                                   bool YAxesVisible, bool medianaLineVisible,
                                                   QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    xv_axisColor = QColor(Qt::black);
    xv_medianaLineColor = QColor(Qt::green);

    xv_XAxesVisible = XAxesVisible;
    xv_YAxesVisible = YAxesVisible;
    xv_medianaLineVisible = medianaLineVisible;
    QRectF itemRect;
    if(!rect.normalized().isValid() || rect.normalized().isEmpty())
    {
        itemRect = QRectF(0.0, -100.0, 100.0, 100.0);
    }
    else
    {
        itemRect = rect.normalized();
    }


    xp_fitItemInRect(itemRect);
}
//========================================================
QRectF X_DefaultRectGraphicsItem::boundingRect() const
{
    return xv_boundingRect;
}
//========================================================
void X_DefaultRectGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    // painter->setClipRect(xv_boundingRect);
    QPen pen;
    pen.setCosmetic(true);

    pen.setBrush(xv_medianaLineColor);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->drawPath(xv_medianaLinePath);

    pen.setBrush(xv_axisColor);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawPath(xv_axisPath);

    painter->restore();
}
//========================================================
QPainterPath X_DefaultRectGraphicsItem::shape() const
{
    return xv_shape;
}
//========================================================
int X_DefaultRectGraphicsItem::type() const
{
    return DefaultRectItemType;
}
//========================================================
void X_DefaultRectGraphicsItem::xp_fitItemInRect(const QRectF& rect)
{
    if(!rect.normalized().isValid() || rect.normalized().isEmpty())
    {
        return;
    }

    prepareGeometryChange();
    xv_boundingRect = rect.normalized().adjusted(-1,-1,1,1);
    QPainterPath newShape;
    newShape.addRect(xv_boundingRect);
    xv_shape = newShape;

    xh_remakePainterPaths();

    //    QPainterPath newAxisPath;
    //    if(xv_XAxesVisible)
    //    {
    //        newAxisPath.moveTo(xv_boundingRect.left(), 0.0);
    //        newAxisPath.lineTo(xv_boundingRect.right(), 0.0);
    //    }
    //    if(xv_YAxesVisible)
    //    {
    //        newAxisPath.moveTo(0.0, xv_boundingRect.top());
    //        newAxisPath.lineTo(0.0, xv_boundingRect.bottom());
    //    }
    //    xv_axisPath = newAxisPath;

    //    QPainterPath newMedianaLinePath;
    //    if(xv_medianaLineVisible)
    //    {
    //        newMedianaLinePath.moveTo( -1*(xv_boundingRect.bottom() + 0.5), (xv_boundingRect.bottom() + 0.5));
    //        newMedianaLinePath.lineTo( -1*(xv_boundingRect.top() + 0.5), (xv_boundingRect.top() + 0.5));
    //    }
    //    xv_medianaLinePath = newMedianaLinePath;

    update();
}
//========================================================
void X_DefaultRectGraphicsItem::xh_remakePainterPaths()
{
    QPainterPath newAxisPath;
    if(xv_XAxesVisible)
    {
        newAxisPath.moveTo(xv_boundingRect.left(), 0.0);
        newAxisPath.lineTo(xv_boundingRect.right(), 0.0);
    }
    if(xv_YAxesVisible)
    {
        newAxisPath.moveTo(0.0, xv_boundingRect.top());
        newAxisPath.lineTo(0.0, xv_boundingRect.bottom());
    }
    xv_axisPath = newAxisPath;

    QPainterPath newMedianaLinePath;
    if(xv_medianaLineVisible)
    {
        newMedianaLinePath.moveTo( -1*(xv_boundingRect.bottom() + 0.5), (xv_boundingRect.bottom() + 0.5));
        newMedianaLinePath.lineTo( -1*(xv_boundingRect.top() + 0.5), (xv_boundingRect.top() + 0.5));
    }
    xv_medianaLinePath = newMedianaLinePath;
}
//========================================================
void X_DefaultRectGraphicsItem::xp_setAxisVisible(Qt::Orientation orientation, bool visibilityFlag )
{
    if(orientation == Qt::Vertical)
    {
        xv_YAxesVisible = visibilityFlag;
    }
    else
    {
        xv_XAxesVisible = visibilityFlag;
    }

    xh_remakePainterPaths();
    update();
}
//========================================================
void X_DefaultRectGraphicsItem::xp_setAxisColor(QColor color)
{
    if(xv_axisColor == color)
    {
        return;
    }
    xv_axisColor = color;
    update();
}
//========================================================
void X_DefaultRectGraphicsItem::xp_setMedianaLineColor(QColor color)
{
    if(xv_medianaLineColor == color)
    {
        return;
    }
    xv_medianaLineColor = color;
    update();
}
//========================================================
void X_DefaultRectGraphicsItem::xp_setMedianaLineVisible(bool visibilityFlag)
{
    if(xv_medianaLineVisible == visibilityFlag)
    {
        return;
    }
    xv_medianaLineVisible = visibilityFlag;
    xh_remakePainterPaths();
    update();
}
//========================================================
