//======================================================
#include "X_EnergyLineGraphicsItem.h"
#include "X_EnergyCalibrationLine.h"
#include <QDebug>
#include "X_TextGraphicsItem.h"
#include <QPainter>
#include <QFontMetrics>
//======================================================
double X_EnergyLineGraphicsItem::xv_topMargin = -100.0;
double X_EnergyLineGraphicsItem::xv_bottomMargin = 0.0;
int X_EnergyLineGraphicsItem::xv_fontPixelSize = 10;
//======================================================
X_EnergyLineGraphicsItem::X_EnergyLineGraphicsItem(const QString &chemicalElementSymbol,
                                                 const QString& name,
                                                 int heightPercent,
                                                 double energyValue,
                                                 QGraphicsItem *parent)
{
    xv_chemicalElementSymbol = chemicalElementSymbol;
    xv_lineName = name;
    xv_heightPercent = heightPercent;
    xv_energyValue = energyValue;
    xv_textItem = new X_TextGraphicsItem(xv_chemicalElementSymbol, this);

    //xv_textItem->setVisible(true);
    setZValue(999);
    xv_textItem->setToolTip(QString("%1 %2").arg(xv_chemicalElementSymbol, xv_lineName));
    xv_textItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setToolTip(xv_textItem->toolTip());
    xv_xPosition = 0.0;
    xp_updateItem();
}
//======================================================
X_EnergyLineGraphicsItem::~X_EnergyLineGraphicsItem()
{
    delete xv_textItem;
}
//======================================================
QRectF X_EnergyLineGraphicsItem::boundingRect() const
{
    return xv_boundingRect;
}
//======================================================
void X_EnergyLineGraphicsItem::paint(QPainter * painter,
                                    const QStyleOptionGraphicsItem * option,
                                    QWidget * widget)
{
    if(xv_xPosition <= 0.0)
    {
        xv_textItem->setVisible(false);
        return;
    }

    painter->save();

    // line
    QPen pen(xv_color, 1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(xv_linePainterPath);

    painter->restore();

    xv_textItem->setVisible(true);
}
//======================================================
QPainterPath X_EnergyLineGraphicsItem::shape() const
{
    return xv_shape;
}
//======================================================
int X_EnergyLineGraphicsItem::type() const
{
    return EnergyLineItemType;
}
//======================================================
void X_EnergyLineGraphicsItem::xp_updateItem()
{
    xh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
void X_EnergyLineGraphicsItem::xh_recalcShapeAndBoundingRect()
{
    prepareGeometryChange();

    // line
    double shift = (qAbs(xv_bottomMargin - xv_topMargin)) * (1 - ((0.8/100) * xv_heightPercent));
    QPainterPath newLinePainterPath;
    QPointF bottomPoint = QPointF(xv_xPosition, 0.0);
    QPointF topPoint = QPointF(xv_xPosition, xv_topMargin + shift);
    newLinePainterPath.moveTo(topPoint);
    newLinePainterPath.lineTo(bottomPoint);

    xv_linePainterPath = newLinePainterPath;

    //xv_shape = xv_linePainterPath;
    QRectF shapeRect = QRectF(xv_xPosition - 1, topPoint.y() - 1,
                              3, bottomPoint.y() - topPoint.y());
    QPainterPath newShape;
    newShape.addRect(shapeRect);
    xv_shape = newShape;

    xv_boundingRect = xv_shape.boundingRect();

    // text
    QPointF textPos(topPoint);
    xv_textItem->setPos(textPos);
}
//======================================================
bool X_EnergyLineGraphicsItem::xp_setTopAndButtonMargins(double top, double bottom)
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
void X_EnergyLineGraphicsItem::xp_setFontPixelSize(int pixelSize)
{
    xv_fontPixelSize = pixelSize;
}
//======================================================
void X_EnergyLineGraphicsItem::xp_setHeightPercent(int percent)
{
    xv_heightPercent = percent;
    xh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
void X_EnergyLineGraphicsItem::xp_setXPosition(double xPos)
{
    xv_xPosition = xPos;
    xh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
void X_EnergyLineGraphicsItem::xp_setColor(QColor color)
{
    xv_color = color;
    xh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
double X_EnergyLineGraphicsItem::xp_energyValue() const
{
    return xv_energyValue;
}
//======================================================
QString X_EnergyLineGraphicsItem::xp_chemicalElementSymbol() const
{
    return xv_chemicalElementSymbol;
}
//======================================================
QString X_EnergyLineGraphicsItem::xp_lineName() const
{
    return xv_lineName;
}
//======================================================
