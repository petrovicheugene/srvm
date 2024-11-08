//======================================================
#include "X_ChartPointGraphicsItem.h"
#include "X_ChartPointOptions.h"
#include "X_ChartPointInfoLabel.h"

#include "X_CorrelationPlotterDataManager.h"
#include "X_GraphicsItemUserTypes.h"
#include "X_PlotterDefaultVariables.h"

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
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

    xp_applyPointProperties(visibilityPoint, xv_chartPointOptions);
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
    return xv_checkState == Qt::Checked? xv_chartPointOptions->xv_checkedBoundingRect :
               xv_chartPointOptions->xv_uncheckedBoundingRect;
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
    painter->setBrush(xv_checkState == Qt::Checked? QBrush(xv_paintColor) : QBrush(Qt::transparent));

    QPen pen(QBrush(xv_checkState == Qt::Unchecked && xv_currentSpectrumId == xv_relatedObjectId ?
                        xv_chartPointOptions->xv_currentPointColor : xv_chartPointOptions->xv_outLineColor),
             xv_chartPointOptions->xv_outLineWidth);
    // pen.setCosmetic(true);
    painter->setPen(pen);
    // painter->drawPath(xv_checkState == Qt::Checked?
    //                       xv_chartPointOptions->xv_checkedShape : xv_chartPointOptions->xv_uncheckedShape);
    painter->drawPath(xv_chartPointOptions->xv_checkedShape);

    painter->restore();
}
//======================================================
QPainterPath X_ChartPointGraphicsItem::shape() const
{
    return xv_checkState == Qt::Checked? xv_chartPointOptions->xv_checkedShape : xv_chartPointOptions->xv_uncheckedShape;
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
    prepareGeometryChange();
}
//======================================================
qint64 X_ChartPointGraphicsItem::xp_relatedObjectId() const
{
    return xv_relatedObjectId;
}
//======================================================
void X_ChartPointGraphicsItem::xp_applyPointProperties(const X_VisibilityPointF &visibilityPoint,
                                                       X_ChartPointOptions* chartPointOptions)
{
    xv_chartPointOptions = chartPointOptions;
    X_VisibilityPointF vPoint = visibilityPoint;
    vPoint.setY(vPoint.y() * (-1) / xv_chartPointOptions->xp_rulerScaleValue(Qt::Vertical));
    vPoint.setX(vPoint.x() / xv_chartPointOptions->xp_rulerScaleValue(Qt::Horizontal));
    setPos(vPoint);
    setVisible(vPoint.xp_isVisible());
    xv_checkState = vPoint.xp_isChecked();
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
        setZValue(gl_defaultSpectrumZValue);
        xv_paintColor = xv_chartPointOptions->xv_pointColor;
        setVisible(isVisible());
    }

    update();
}
//======================================================
void X_ChartPointGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    xv_dataManager->xp_setCurrentSpectrum(xv_relatedObjectId);
}
//======================================================
#include <QGraphicsView>
#include  <QLabel>
void X_ChartPointGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if(event->button() == Qt::RightButton)
    {
        if(scene()->views().isEmpty())
        {
            return;
        }

        QGraphicsView* view = scene()->views().at(0);
        QPoint globalPos = view->viewport()->mapToGlobal(view->mapFromScene(scenePos()));
        X_ChartPointInfoLabel* label = new X_ChartPointInfoLabel;

        QString infoString = xv_dataManager->xp_infoForSpectrumId(xv_relatedObjectId);
        label->setText(infoString);
        label->move(globalPos);

        label->xp_correctBottomRightShift();
        label->show();

    }

    QGraphicsItem::mouseReleaseEvent(event);
}
//======================================================
void X_ChartPointGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    xv_dataManager->xp_toogleSpectrum(xv_relatedObjectId);
}
//======================================================
