//======================================================
#include "X_ChartPointGraphicsItem.h"
#include "X_ChartPointOptions.h"
#include "X_CorrelationPlotterDataManager.h"
#include "X_GraphicsItemUserTypes.h"
#include "X_PlotterDefaultVariables.h"

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QPolygonF>
//======================================================
qint64 X_ChartPointGraphicsItem::xv_currentSpectrumId = -1;
QColor X_ChartPointGraphicsItem::xv_currentColor = QColor(Qt::red);
//======================================================
X_ChartPointGraphicsItem::X_ChartPointGraphicsItem(const X_CorrelationPlotterDataManager* dataManager,
                                                 const X_VisibilityPointF& visibilityPoint,
                                                 X_ChartPointOptions *seriesPointOptions,
                                                 qint64 relatedObjectId,
                                                 QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    xv_dataManager = dataManager;
    xv_relatedObjectId = relatedObjectId;
    // color of current point by default
    prepareGeometryChange();
    xv_chartPointOptions = seriesPointOptions;
    xv_chartPointOptions->xv_seriesPointItemList.append(this);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setZValue(gl_defaultSpectrumZValue);

    xp_applyVisibilityAndPos(visibilityPoint, xv_chartPointOptions);
}
//======================================================
X_ChartPointGraphicsItem::~X_ChartPointGraphicsItem()
{
    int index = xv_chartPointOptions->xv_seriesPointItemList.indexOf(this);
    if(index >= 0)
    {
        xv_chartPointOptions->xv_seriesPointItemList.removeAt(index);
    }
}
//======================================================
QRectF X_ChartPointGraphicsItem::boundingRect() const
{
    return xv_chartPointOptions->xv_boundingRect;
}
//======================================================
void X_ChartPointGraphicsItem::xp_setCurrentSpectrumColor(QColor color)
{
    xv_currentColor = color;
}
//======================================================
void X_ChartPointGraphicsItem::xp_setCurrentSpectrumId(qint64 currentSpectrumId)
{
    xv_currentSpectrumId = currentSpectrumId;
}
//======================================================
void X_ChartPointGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    // window body
    painter->setBrush(QBrush(xv_paintColor));
    QPen pen(QBrush(xv_chartPointOptions->xv_outLineColor), xv_chartPointOptions->xv_outLineWidth);
    //pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawPath(xv_chartPointOptions->xv_shape);
    painter->restore();
}
//======================================================
QPainterPath X_ChartPointGraphicsItem::shape() const
{
    return xv_chartPointOptions->xv_shape;
}
//======================================================
int X_ChartPointGraphicsItem::type() const
{
    return ChartPointItemType;
}
//======================================================
X_ChartPointOptions* X_ChartPointGraphicsItem::xp_seriesPointOptions() const
{
    return xv_chartPointOptions;
}
//======================================================
void X_ChartPointGraphicsItem::xp_setNewBoundingRect(const QRectF& boundingRect)
{
    //    if(zv_boundingRect == boundingRect)
    //    {
    //        return;
    //    }

    prepareGeometryChange();
    // zv_boundingRect = boundingRect;
}
//======================================================
qint64 X_ChartPointGraphicsItem::xp_relatedObjectId() const
{
    return xv_relatedObjectId;
}
//======================================================
void X_ChartPointGraphicsItem::xp_applyVisibilityAndPos(const X_VisibilityPointF &visibilityPoint,
                                                       X_ChartPointOptions* chartPointOptions)
{
    xv_chartPointOptions = chartPointOptions;
    X_VisibilityPointF vPoint = visibilityPoint;
    vPoint.setY(vPoint.y() * (-1) / xv_chartPointOptions->xp_rulerScaleValue(Qt::Vertical));
    vPoint.setX(vPoint.x() / xv_chartPointOptions->xp_rulerScaleValue(Qt::Horizontal));
    // zv_boundingRect = seriesPointOptions->zv_boundingRect;
    setPos(vPoint);
    //    setZValue(2);
    setVisible(vPoint.xp_isVisible());
    xp_updateCurrentItem();
}

//======================================================
void X_ChartPointGraphicsItem::xp_updateCurrentItem()
{
    if(xv_currentSpectrumId == xv_relatedObjectId)
    {
        setZValue(gl_currentSpectrumZValue);
        xv_paintColor = xv_currentColor;
        setVisible(isVisible());
    }
    else
    {
        //      if(zValue() != gl_defaultSpectrumZValue)
        //      {
        setZValue(gl_defaultSpectrumZValue);
        xv_paintColor = xv_chartPointOptions->xv_pointColor;
        setVisible(isVisible());
        //      }
    }
}
//======================================================
void X_ChartPointGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    xv_dataManager->xp_setCurrentSpectrum(xv_relatedObjectId);

    //QGraphicsItem::mousePressEvent(event);
}
//======================================================
void X_ChartPointGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
#ifdef DBG
    qDebug() << "ITEM MOUSE RELEASE";
#endif

    xv_dataManager->xp_setCurrentSpectrum(xv_relatedObjectId);
}
//======================================================
