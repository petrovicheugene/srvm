//======================================================
#include "X_WindowGraphicsItem.h"
#include "X_CalibrationWindow.h"
#include "X_PlotterDefaultVariables.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
//======================================================
QColor X_WindowGraphicsItem::xv_currentWindowColor = QColor(Qt::red);
qreal X_WindowGraphicsItem::xv_topMargin = -100;
qreal X_WindowGraphicsItem::xv_bottomMargin = 0;
qint64 X_WindowGraphicsItem::xv_currentWindowId = -1;
quint8 X_WindowGraphicsItem::xv_alphaCannelValue = 30;

//======================================================
X_WindowGraphicsItem::X_WindowGraphicsItem(const X_CalibrationWindow* window,
                                         QColor color,
                                         QGraphicsItem * parent)
    : QGraphicsItem(parent)
{
    xv_windowId = window->xp_windowId();
    xv_color = color;
    xv_leftMargin = (qreal)window->xp_firstChannel();
    xv_rightMargin = (qreal)(window->xp_lastChannel() + 1);

    setZValue(gl_defaultWindowX_Value);
    xv_lineColor = xv_color;
    xv_brushColor = xv_color;
    xv_brushColor.setAlpha(xv_alphaCannelValue);
    zh_recalcShapeAndBoundingRect();
    xp_updateCurrentWindow(window->xp_isWindowVisible());
}
//======================================================
QRectF X_WindowGraphicsItem::boundingRect() const
{
    return xv_boundingRect;
}
//======================================================
void X_WindowGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    // window body
    painter->setBrush(QBrush(xv_brushColor));
    painter->setPen(Qt::NoPen);
    painter->drawPath(xv_paintRect);

    // margins
    painter->setBrush(Qt::NoBrush);
    QPen pen(xv_lineColor, 1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawPath(xv_marginsPainterPath);

    painter->restore();
}
//======================================================
QPainterPath X_WindowGraphicsItem::shape() const
{
    return xv_shape;
}
//======================================================
int X_WindowGraphicsItem::type() const
{
    return WindowItemType;
}
//======================================================
void X_WindowGraphicsItem::xp_setCurrentWindowColor(QColor color)
{
    xv_currentWindowColor = color;
}
//======================================================
void X_WindowGraphicsItem::xp_setCurrentWindowId(qint64 id)
{
    xv_currentWindowId = id;
}
//======================================================
bool X_WindowGraphicsItem::xp_setTopAndButtonMargins(qreal top, qreal bottom)
{
    if(top == bottom)
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
bool X_WindowGraphicsItem::xp_setAlphaChannelValue(quint8 alpha)
{
    xv_alphaCannelValue = alpha;
    return true;
}
//======================================================
qint64 X_WindowGraphicsItem::xp_windowId() const
{
    return xv_windowId;
}
//======================================================
void X_WindowGraphicsItem::xp_setLeftRightMargins(qreal first, qreal last)
{
    if(first > last)
    {
        qSwap(last, last);
    }

    xv_leftMargin = first;
    xv_rightMargin = last + 1;
    zh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
void X_WindowGraphicsItem::xp_updateCurrentWindow(bool visible)
{
    if(xv_currentWindowId == xv_windowId)
    {
        setZValue(gl_currentWindowX_Value);
        xv_lineColor = xv_color;
        xv_brushColor = xv_currentWindowColor;
        xv_brushColor.setAlpha(xv_alphaCannelValue);
        setVisible(true);
    }
    else
    {
        //      if(zValue() != gl_defaultWindowX_Value)
        //      {
        setZValue(gl_defaultWindowX_Value);
        xv_lineColor = xv_color;
        xv_brushColor = xv_color;
        xv_brushColor.setAlpha(xv_alphaCannelValue);
        setVisible(visible);
        //      }
    }
}
//======================================================
void X_WindowGraphicsItem::xp_updateItem()
{
    zh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
void X_WindowGraphicsItem::xp_setVisible(bool visible)
{
    if(xv_windowId == xv_currentWindowId && visible == false)
    {
        return;
    }
    setVisible(visible);
}
//======================================================
void X_WindowGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
//======================================================
//void X_WindowGraphicsItem::zh_recalcShape()
//{
//   QPainterPath path;
//   //    QPointF topPoint(xv_data.first, xv_topY);
//   //    QPointF bottomPoint(xv_data.first, xv_bottomY);
//   //    path.moveTo(topPoint);
//   //    path.lineTo(bottomPoint);
//   //    topPoint.setX(xv_data.second);
//   //    bottomPoint.setX(xv_data.second);
//   //    path.moveTo(topPoint);
//   //    path.lineTo(bottomPoint);

//   path.addRect(xv_boundingRect);
//   xv_shape = path;
//}
//======================================================
void X_WindowGraphicsItem::zh_recalcShapeAndBoundingRect()
{
    prepareGeometryChange();
    // shape
    QPainterPath newShape;
    QPointF topLeftPoint(xv_leftMargin, xv_topMargin);
    QPointF bottomRightPoint(xv_rightMargin, xv_bottomMargin);
    QRectF windowRect(topLeftPoint, bottomRightPoint);

    newShape.addRect(windowRect);
    xv_shape = newShape;
    xv_boundingRect = windowRect;

    // paint rect shape
    QPainterPath newRectPaintShape;
    qreal shift = (qAbs(xv_bottomMargin - xv_topMargin)) / 2;
    topLeftPoint = QPointF(xv_leftMargin, xv_topMargin - shift);
    bottomRightPoint = QPointF(xv_rightMargin, xv_bottomMargin + shift);
    QRectF paintRect(topLeftPoint, bottomRightPoint);
    newRectPaintShape.addRect(paintRect);
    xv_paintRect = newRectPaintShape;

    // line shape
    QPainterPath newLinePaintShape;
    QPointF bottomLeftPoint = QPointF(xv_leftMargin, xv_bottomMargin + shift);
    QPointF topRightPoint = QPointF(xv_rightMargin, xv_topMargin - shift);

    newLinePaintShape.moveTo(topLeftPoint);
    newLinePaintShape.lineTo(bottomLeftPoint);
    newLinePaintShape.moveTo(topRightPoint);
    newLinePaintShape.lineTo(bottomRightPoint);

    xv_marginsPainterPath = newLinePaintShape;
}
//======================================================
