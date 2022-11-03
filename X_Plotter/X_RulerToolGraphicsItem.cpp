//======================================================
#include "X_RulerToolGraphicsItem.h"
#include "X_TextGraphicsItem.h"

#include <QBrush>
#include <QColor>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QPen>

//======================================================
X_RulerToolGraphicsItem::X_RulerToolGraphicsItem(QGraphicsItem *parent)
{
    xv_textItem = new QGraphicsTextItem(this);

    xv_textItem->setVisible(true);
    setZValue(9999);
    xv_textItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    xp_updateItem();
}
//======================================================
X_RulerToolGraphicsItem::~X_RulerToolGraphicsItem()
{
    delete xv_textItem;
}
//======================================================
QRectF X_RulerToolGraphicsItem::boundingRect() const
{
    return xv_boundingRect;
}
//======================================================
void X_RulerToolGraphicsItem::paint(QPainter * painter,
                                   const QStyleOptionGraphicsItem * option,
                                   QWidget * widget)
{
    painter->save();
    QBrush brush(QColor(255, 0, 255, 20));
    QPen pen(Qt::transparent);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(xv_boundingRect);

    pen = QPen(QColor(Qt::red), 1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawPath(xv_linePainterPath);

    painter->restore();
}
//======================================================
QPainterPath X_RulerToolGraphicsItem::shape() const
{
    return xv_shape;
}
//======================================================
int X_RulerToolGraphicsItem::type() const
{
    return RulerToolItemType;
}
//======================================================
void X_RulerToolGraphicsItem::xp_setProperties(QPointF startPosition,
                                              QPointF endPosition,
                                              QRectF viewPortSceneRect,
                                              QSize viewportSize,
                                              const QString& text)
{
    xv_startPosition = startPosition;
    xv_endPosition = endPosition;
    xv_viewportSceneRect = viewPortSceneRect;
    xv_viewportSize = viewportSize;
    xv_text = text;

    xp_updateItem();
}
//======================================================
void X_RulerToolGraphicsItem::xp_updateItem()
{
    zh_recalcShapeAndBoundingRect();
    update();
}
//======================================================
void X_RulerToolGraphicsItem::zh_recalcShapeAndBoundingRect()
{
    prepareGeometryChange();

    QPainterPath linePainterPath;

    double offset = zh_convertPixelsToSceneHeight(5);
    linePainterPath.moveTo(xv_startPosition.x(), xv_viewportSceneRect.top() + offset);
    linePainterPath.lineTo(xv_startPosition.x(), 0.0);

    linePainterPath.moveTo(xv_endPosition.x(), xv_viewportSceneRect.top() + offset);
    linePainterPath.lineTo(xv_endPosition.x(), 0.0);

    xv_linePainterPath = linePainterPath;
    xv_shape = xv_linePainterPath;

    double left = qMin(xv_startPosition.x(), xv_endPosition.x());

    xv_boundingRect = QRectF(left,
                             xv_viewportSceneRect.top() + offset,
                             qAbs(xv_startPosition.x() - xv_endPosition.x()),
                             -(xv_viewportSceneRect.top() + offset));


    xv_textItem->setHtml(xv_text);
    QRectF textRect = xv_textItem->boundingRect();
    QPointF textPos = xv_endPosition;

    textPos.setY(textPos.y() - zh_convertPixelsToSceneHeight(qRound(textRect.height())));

    if(xv_endPosition.x() + zh_convertPixelsToSceneWidth(qRound(textRect.width())) >  xv_viewportSceneRect.right())
    {
        textPos.setX(xv_viewportSceneRect.right() - zh_convertPixelsToSceneWidth(qRound(textRect.width())));
    }

    xv_textItem->setPos(textPos);
}
//======================================================
double X_RulerToolGraphicsItem::zh_convertPixelsToSceneWidth(int pixelCount) const
{
    return (xv_viewportSceneRect.width() / xv_viewportSize.width()) * pixelCount;
}
//======================================================
double X_RulerToolGraphicsItem::zh_convertPixelsToSceneHeight(int pixelCount) const
{
    return (xv_viewportSceneRect.height() / xv_viewportSize.height()) * pixelCount;
}
//======================================================

