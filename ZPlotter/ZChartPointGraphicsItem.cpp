//======================================================
#include "ZChartPointGraphicsItem.h"
#include "ZGraphicsItemUserTypes.h"
#include "ZChartPointOptions.h"

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QPolygonF>
//======================================================

//======================================================
ZChartPointGraphicsItem::ZChartPointGraphicsItem(const ZVisibilityPointF& visibilityPoint,
                                                   ZChartPointOptions *seriesPointOptions,
                                                   qint64 relatedObjectId,
                                                   QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    zv_relatedObjectId = relatedObjectId;
    // color of current point by default
    prepareGeometryChange();
    zv_chartPointOptions = seriesPointOptions;
    zv_chartPointOptions->zv_seriesPointItemList.append(this);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);

    zp_applyVisibilityAndPos(visibilityPoint, zv_chartPointOptions);
}
//======================================================
ZChartPointGraphicsItem::~ZChartPointGraphicsItem()
{
    int index = zv_chartPointOptions->zv_seriesPointItemList.indexOf(this);
    if(index >= 0)
    {
        zv_chartPointOptions->zv_seriesPointItemList.removeAt(index);
    }
}
//======================================================
QRectF ZChartPointGraphicsItem::boundingRect() const
{
    return zv_chartPointOptions->zv_boundingRect;
}
//======================================================
void ZChartPointGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    // window body
    painter->setBrush(QBrush(zv_chartPointOptions->zv_pointColor));
    QPen pen(QBrush(zv_chartPointOptions->zv_outLineColor), zv_chartPointOptions->zv_outLineWidth);
    //pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawPath(zv_chartPointOptions->zv_shape);
    painter->restore();
}
//======================================================
QPainterPath ZChartPointGraphicsItem::shape() const
{
    return zv_chartPointOptions->zv_shape;
}
//======================================================
int ZChartPointGraphicsItem::type() const
{
    return ChartPointItemType;
}
//======================================================
ZChartPointOptions* ZChartPointGraphicsItem::zp_seriesPointOptions() const
{
    return zv_chartPointOptions;
}
//======================================================
void ZChartPointGraphicsItem::zp_setNewBoundingRect(const QRectF& boundingRect)
{
    //    if(zv_boundingRect == boundingRect)
    //    {
    //        return;
    //    }

    prepareGeometryChange();
    // zv_boundingRect = boundingRect;
}
//======================================================
qint64 ZChartPointGraphicsItem::zp_relatedObjectId() const
{
    return zv_relatedObjectId;
}
//======================================================
void ZChartPointGraphicsItem::zp_applyVisibilityAndPos(const ZVisibilityPointF &visibilityPoint,
                                                       ZChartPointOptions* chartPointOptions)
{
    zv_chartPointOptions = chartPointOptions;
    ZVisibilityPointF vPoint = visibilityPoint;
    vPoint.setY(vPoint.y() * (-1) / zv_chartPointOptions->zp_rulerScaleValue(Qt::Vertical));
    vPoint.setX(vPoint.x() / zv_chartPointOptions->zp_rulerScaleValue(Qt::Horizontal));
    // zv_boundingRect = seriesPointOptions->zv_boundingRect;
    setPos(vPoint);
    setZValue(2);
    setVisible(vPoint.zp_isVisible());
}
//======================================================
