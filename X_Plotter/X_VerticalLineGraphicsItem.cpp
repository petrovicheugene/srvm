//======================================================
#include "X_VerticalLineGraphicsItem.h"
#include "X_EnergyCalibrationLine.h"

#include <QPainter>
//======================================================
double X_VerticalLineGraphicsItem::xv_topMargin = -100.0;
double X_VerticalLineGraphicsItem::xv_bottomMargin = 0.0;
//======================================================
X_VerticalLineGraphicsItem::X_VerticalLineGraphicsItem(X_EnergyCalibrationLine* energyCalibrationLine,
                                                     QGraphicsItem *parent)
 : QGraphicsItem(parent)
{
    xv_energyCalibrationLine = energyCalibrationLine;
    xp_updateItem();
}
//======================================================
QRectF X_VerticalLineGraphicsItem::boundingRect() const
{
    return xv_boundingRect;
}
//======================================================
void X_VerticalLineGraphicsItem::paint(QPainter * painter,
           const QStyleOptionGraphicsItem * option,
           QWidget * widget)
{
    painter->save();
    painter->setBrush(Qt::NoBrush);
    QPen pen(xv_energyCalibrationLine->xp_lineColor(), 1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawPath(xv_linePainterPath);
    painter->restore();

}
//======================================================
QPainterPath X_VerticalLineGraphicsItem::shape() const
{
    return xv_shape;
}
//======================================================
int X_VerticalLineGraphicsItem::type() const
{
    return VerticalLineItemType;
}
//======================================================
void X_VerticalLineGraphicsItem::xp_updateItem()
{
    setVisible(xv_energyCalibrationLine->xp_isEnabled());
    zh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
bool X_VerticalLineGraphicsItem::xp_setTopAndButtonMargins(qreal top, qreal bottom)
{
    if(top - bottom == 0.0)
    {
        return false;
    }

    if(top > bottom)
    {
        qSwap(top, bottom);
    }

    xv_bottomMargin = bottom;
    xv_topMargin = top;
    return true;
}
//======================================================
void X_VerticalLineGraphicsItem::zh_recalcShapeAndBoundingRect()
{
    prepareGeometryChange();
    double xPosition = static_cast<double>(xv_energyCalibrationLine->xp_channelNumber()) + 0.5;
    // shape
    QPainterPath newShape;
    QPointF topLeftPoint(xPosition - 1, xv_topMargin);
    QPointF bottomRightPoint(xPosition + 1, xv_bottomMargin);
    QRectF windowRect(topLeftPoint, bottomRightPoint);

    newShape.addRect(windowRect);
    xv_shape = newShape;
    xv_boundingRect = windowRect;

    // line
    qreal shift = (qAbs(xv_bottomMargin - xv_topMargin)) / 2;

    QPainterPath newLinePaintShape;
    QPointF bottomPoint = QPointF(xPosition, xv_bottomMargin + shift);
    QPointF topPoint = QPointF(xPosition, xv_topMargin - shift);

    newLinePaintShape.moveTo(topPoint);
    newLinePaintShape.lineTo(bottomPoint);

    xv_linePainterPath = newLinePaintShape;
}
//======================================================

