//=============================================================
#include "ZPlotGraphicsView.h"
#include "ZRulersAndGridManager.h"

#include <QApplication>
#include <QRubberBand>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>
//=============================================================
ZPlotGraphicsView::ZPlotGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    zv_plotMode = PM_REGULAR;
    zv_sceneCenterPos = QPointF();
    zv_sceneMousePos = QPointF();
    zv_mousePressStartViewPos = QPoint();
    zv_rubberBand = 0;
    zv_scaleViewWhenResizeFlag = false;
    zv_drawGridFlag = true;
    zv_rulersAndGreedManager = 0;
    zv_minSideSizeOfVisibleScene = 15;

    zv_XRuleList = 0;
    zv_YRuleList = 0;

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setMouseTracking(true);
    zh_createConnections();
}
//=============================================================
void ZPlotGraphicsView::zp_setScaleViewWhenResizeFlag(bool flag)
{
    zv_scaleViewWhenResizeFlag = flag;
}
//=============================================================
void ZPlotGraphicsView::zp_setViewport(QWidget * widget)
{
    QGraphicsView::setViewport(widget);
}
//=============================================================
void ZPlotGraphicsView::zp_connectToRulersAndGridManager(ZRulersAndGridManager* manager)
{
    zv_rulersAndGreedManager = manager;
    zv_rulersAndGreedManager->zp_setPlotGraphicsView(this);

}
//=============================================================
void ZPlotGraphicsView::zp_setHorizontalRulePointList(QList<RulePoint> * XRuleList)
{
    zv_XRuleList = XRuleList;
}
//=============================================================
void ZPlotGraphicsView::zp_setVerticalRulePointList(QList<RulePoint> * YRuleList)
{
    zv_YRuleList = YRuleList;
}
//=============================================================
bool ZPlotGraphicsView::zp_viewPortCoordinates(QRect& globalRect, QRectF& sceneRect, QPoint& globalZeroPoint)
{
    if(!scene())
    {
        return false;
    }

    // rects
    QRect viewportRect = viewport()->rect();
    sceneRect = mapToScene(viewport()->rect().adjusted(1,1,-1,-1)).boundingRect();

    globalRect = QRect(viewport()->mapToGlobal(viewportRect.topLeft()),
                       viewport()->mapToGlobal(viewportRect.bottomRight()));

    // global Zero Point
    globalZeroPoint = viewport()->mapToGlobal(mapFromScene(0.0, 0.0));

    return true;
}
//=============================================================
void ZPlotGraphicsView::zp_scale(Qt::Orientation orientation, bool increase)
{
//    QRect viewportRect = viewport()->rect().adjusted(1,1,-1,-1);
    zv_sceneCenterPos = mapToScene(viewport()->rect().center());

//    QRect vpRect = rect();//.adjusted(1,1,-1,-1);
//    QRectF nVisibleSceneRect = mapToScene(vpRect).boundingRect();

//    fitInView(nVisibleSceneRect);
//    centerOn(zv_sceneCenterPos);

//    return;


    double scaleRate;
    if(increase)
    {
        scaleRate = 0.9;
    }
    else
    {
        scaleRate = 1.1;
    }

    // new view rect
    QRectF oldVisibleSceneRect = mapToScene(viewport()->rect().adjusted(1,1,-1,-1)).boundingRect();
    QRectF newVisibleSceneRect = oldVisibleSceneRect;

    if(orientation == Qt::Vertical )
    {
        newVisibleSceneRect.setHeight(newVisibleSceneRect.height() * scaleRate);
    }
    else
    {
        newVisibleSceneRect.setWidth(newVisibleSceneRect.width() * scaleRate);
    }

    QRectF sceneRect = scene()->sceneRect();
    // fitting visible rect size if it is larger then scene size
    if(newVisibleSceneRect.width() > sceneRect.width())
    {
        newVisibleSceneRect.setWidth(sceneRect.width());
    }

    if(newVisibleSceneRect.height() > sceneRect.height())
    {
        newVisibleSceneRect.setHeight(sceneRect.height());
    }

    // fitting rect position if it lap over scene rect margins
    if(orientation == Qt::Vertical)
    {
        if(newVisibleSceneRect.top() < sceneRect.top())
        {
            newVisibleSceneRect.moveTop(sceneRect.top());
        }

        if(newVisibleSceneRect.bottom() > sceneRect.bottom())
        {
            newVisibleSceneRect.moveBottom(sceneRect.bottom());
        }

        if(newVisibleSceneRect.height() < zv_minSideSizeOfVisibleScene)
        {
            newVisibleSceneRect.setHeight(zv_minSideSizeOfVisibleScene);
        }
    }

    if(orientation == Qt::Horizontal)
    {
        if(newVisibleSceneRect.left() < sceneRect.left())
        {
            newVisibleSceneRect.moveLeft(sceneRect.left());
        }

        if(newVisibleSceneRect.right() > sceneRect.right())
        {
            newVisibleSceneRect.moveRight(sceneRect.right());
        }

        if(newVisibleSceneRect.width() < zv_minSideSizeOfVisibleScene)
        {
            newVisibleSceneRect.setWidth(zv_minSideSizeOfVisibleScene);
        }
    }

    if(oldVisibleSceneRect.size() == newVisibleSceneRect.size())
    {
        return;
    }

    newVisibleSceneRect.moveCenter(zv_sceneCenterPos);
    newVisibleSceneRect = newVisibleSceneRect.normalized();
    fitInView(newVisibleSceneRect.normalized());
}
//=============================================================
void ZPlotGraphicsView::zp_setGridHidden(bool hideGrid)
{
    zv_drawGridFlag = !hideGrid;
    viewport()->update();
}
//=============================================================
QRectF ZPlotGraphicsView::zp_currentVisibleSceneRect() const
{
    QRect rect = viewport()->rect().adjusted(1,1,-1,-1);
    QPointF sceneTopLeft = mapToScene(rect.topLeft());
    QPointF scenebottomRight = mapToScene(rect.bottomRight());

    QRectF sRect(sceneTopLeft, scenebottomRight);
    return sRect;
}
//=============================================================
void ZPlotGraphicsView::wheelEvent(QWheelEvent * event)
{
    if(zv_plotMode != PM_REGULAR)
    {
        return;
    }

    zv_sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();
    zv_sceneMousePos = mapToScene(event->pos());

    double scaleRate;
    if(event->delta() > 0)
    {
        scaleRate = 1.1;
    }
    else
    {
        scaleRate = 0.9;
    }

    // new view rect
    QRectF oldVisibleSceneRect = mapToScene(viewport()->rect().adjusted(1,1,-1,-1)).boundingRect();

    QSize newViewportRectSize = viewport()->rect().adjusted(1,1,-1,-1).size()*scaleRate;
    QRect newViewportRect(QPoint(0,0), newViewportRectSize);

    QRectF newVisibleSceneRect = mapToScene(newViewportRect).boundingRect();
    QRectF sceneRect = scene()->sceneRect();
    // fitting visible rect size if it is larger then scene size
    if(newVisibleSceneRect.width() > sceneRect.width())
    {
        newVisibleSceneRect.setWidth(sceneRect.width());
    }

    if(newVisibleSceneRect.height() > sceneRect.height())
    {
        newVisibleSceneRect.setHeight(sceneRect.height());
    }

    // fitting rect position if it lap over scene rect margins
    if(newVisibleSceneRect.top() < sceneRect.top())
    {
        newVisibleSceneRect.moveTop(sceneRect.top());
    }

    if(newVisibleSceneRect.bottom() > sceneRect.bottom())
    {
        newVisibleSceneRect.moveBottom(sceneRect.bottom());
    }

    if(newVisibleSceneRect.left() < sceneRect.left())
    {
        newVisibleSceneRect.moveLeft(sceneRect.left());
    }

    if(newVisibleSceneRect.right() > sceneRect.right())
    {
        newVisibleSceneRect.moveRight(sceneRect.right());
    }

    if(newVisibleSceneRect.width() < zv_minSideSizeOfVisibleScene)
    {
        newVisibleSceneRect.setWidth(zv_minSideSizeOfVisibleScene);
    }
    if(newVisibleSceneRect.height() < zv_minSideSizeOfVisibleScene)
    {
        newVisibleSceneRect.setHeight(zv_minSideSizeOfVisibleScene);
    }

    if(oldVisibleSceneRect.size() == newVisibleSceneRect.size())
    {
        return;
    }

    newVisibleSceneRect.moveCenter(zv_sceneCenterPos);
    newVisibleSceneRect = newVisibleSceneRect.normalized();
    fitInView(newVisibleSceneRect.normalized());

    // moving center
    QPointF newSceneMousePos = mapToScene(event->pos());
    qreal deltaX = zv_sceneMousePos.x() - newSceneMousePos.x();
    qreal deltaY = zv_sceneMousePos.y() - newSceneMousePos.y();
    QPointF newSceneCenter(zv_sceneCenterPos.x() + deltaX, zv_sceneCenterPos.y() + deltaY);
    centerOn(newSceneCenter);
    zv_sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();
    zv_sceneMousePos = mapToScene(event->pos());
}
//=============================================================
void ZPlotGraphicsView::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        // pad moving
        QGraphicsItem* item = itemAt(event->pos());
        if(!item || !(item->flags() & QGraphicsItem::ItemIsMovable))
        {
            zv_plotMode = PM_PAD_DRAGGING;
            qApp->setOverrideCursor(Qt::ClosedHandCursor);
            zv_sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();
            zv_sceneMousePos = mapToScene(event->pos());
            return;
        }
    }
    else if(event->button() == Qt::RightButton)
    {
        zv_mousePressStartViewPos = event->pos();
    }

    QGraphicsView::mousePressEvent(event);
}
//=============================================================
void ZPlotGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if(zv_plotMode == PM_PAD_DRAGGING)
    {
        qApp->restoreOverrideCursor();
    }
    else if(zv_plotMode == PM_RUBBER_BAND)
    {
        zv_rubberBand->hide();
        QPoint topLeft;
        if(zv_mousePressStartViewPos.x() > event->pos().x())
        {
            topLeft.setX(event->pos().x());
        }
        else
        {
            topLeft.setX(zv_mousePressStartViewPos.x());
        }

        if(zv_mousePressStartViewPos.y() > event->pos().y())
        {
            topLeft.setY(event->pos().y());
        }
        else
        {
            topLeft.setY(zv_mousePressStartViewPos.y());
        }

        QRect rubberBandRect = zv_rubberBand->rect();
        rubberBandRect.moveTo(topLeft);

        QRectF newSceneRect = mapToScene(rubberBandRect).boundingRect().normalized();
        QPointF newSceneCenter =  newSceneRect.center();
        if(newSceneRect.width() < zv_minSideSizeOfVisibleScene)
        {
            newSceneRect.setWidth(zv_minSideSizeOfVisibleScene);
        }
        if(newSceneRect.height() < zv_minSideSizeOfVisibleScene)
        {
            newSceneRect.setHeight(zv_minSideSizeOfVisibleScene);
        }

        if(newSceneRect.center() != newSceneCenter)
        {
            newSceneRect.moveCenter(newSceneCenter);
        }
        fitInView(newSceneRect);
    }
    zv_plotMode = PM_REGULAR;
    QGraphicsView::mouseReleaseEvent(event);
}
//=============================================================
void ZPlotGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if(zv_plotMode == PM_PAD_DRAGGING)
    {
        QPointF newSceneMousePos = mapToScene(event->pos());
        qreal deltaX = zv_sceneMousePos.x() - newSceneMousePos.x();
        qreal deltaY = zv_sceneMousePos.y() - newSceneMousePos.y();
        QPointF newSceneCenter(zv_sceneCenterPos.x() + deltaX, zv_sceneCenterPos.y() + deltaY);
        centerOn(newSceneCenter);
        zv_sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();
        zv_sceneMousePos = mapToScene(event->pos());
        return;
    }
    else if(zv_plotMode == PM_RUBBER_BAND)
    {
        zv_rubberBand->setGeometry(QRect(zv_mousePressStartViewPos, event->pos()).normalized());
        return;
    }
    else if(zv_plotMode == PM_REGULAR && event->buttons() == Qt::RightButton)
    {
        QRectF sceneRect = mapToScene(viewport()->rect().adjusted(1,1,-1,-1)).boundingRect().normalized();
        if(sceneRect.width() > zv_minSideSizeOfVisibleScene + 1
                || sceneRect.height() > zv_minSideSizeOfVisibleScene +1)
        {
            zv_plotMode = PM_RUBBER_BAND;
            if(!zv_rubberBand)
            {
                zv_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
            }
            zv_rubberBand->setGeometry(QRect(zv_mousePressStartViewPos, QSize()));
            zv_rubberBand->show();
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}
//=============================================================
bool ZPlotGraphicsView::viewportEvent(QEvent * event)
{
    if(event->type() == QEvent::Resize && scene())
    {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);

        if(zv_scaleViewWhenResizeFlag
                &&resizeEvent->oldSize().width() != 0
                && resizeEvent->oldSize().height() != 0)
        {
            QPointF sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();

            double dx = (double)(resizeEvent->size().width())
                    / (double)(resizeEvent->oldSize().width());

            double dy = (double)(resizeEvent->size().height())
                    / (double)(resizeEvent->oldSize().height());

            scale(dx, dy);
            centerOn(sceneCenterPos);
        }
    }

    if(event->type() == QEvent::Paint && scene() && zv_rulersAndGreedManager)
    {
        zv_rulersAndGreedManager->zp_recalcRulesAndGrid();
    }

    return QGraphicsView::viewportEvent(event);
}
//=============================================================
void ZPlotGraphicsView::drawBackground(QPainter * painter, const QRectF & rect)
{
    if(zv_rulersAndGreedManager != 0 && zv_drawGridFlag)
    {
        // rect adjusting
        qreal K_width = rect.width() * 1.1/
                rect.width();
        qreal K_height = rect.height()  * 1.1/
                rect.height();

        int dPix = 1;
        const QRectF adjustedRect = rect.adjusted(-1*K_width*dPix, -1*K_height*dPix, K_width*dPix, K_height*dPix);

        // painter for MARK GRID
        painter->save();

        // X
        if(zv_XRuleList != 0)
        {
            for(int i = 0; i < zv_XRuleList->count(); i++)
            {
                // pen settings
                QPen pen;
                if(zv_XRuleList->value(i).markType == RulePoint::MT_SCRATCH)
                {
                    pen = QPen(QBrush(viewport()->palette().color(QPalette::Mid)), 1, Qt::DotLine);
                    pen.setCosmetic(true);
                }
                else
                {
                    pen = QPen(QBrush(viewport()->palette().color(QPalette::Mid)), 1, Qt::DashLine);
                    pen.setCosmetic(true);
                }
                painter->setPen(pen);

                painter->drawLine(zv_XRuleList->value(i).scenePos, adjustedRect.top(),
                                  zv_XRuleList->value(i).scenePos, adjustedRect.bottom());
            }
        }

        // Y
        if(zv_YRuleList != 0)
        {
            for(int i = 0; i < zv_YRuleList->count(); i++)
            {
                // pen settings
                QPen pen;
                if(zv_YRuleList->value(i).markType == RulePoint::MT_SCRATCH)
                {
                    pen = QPen(QBrush(viewport()->palette().color(QPalette::Mid)), 1, Qt::DotLine);
                    pen.setCosmetic(true);
                }
                else
                {
                    pen = QPen(QBrush(viewport()->palette().color(QPalette::Mid)), 1, Qt::DashLine);
                    pen.setCosmetic(true);
                }
                painter->setPen(pen);

                painter->drawLine(adjustedRect.left(), zv_YRuleList->value(i).scenePos,
                                  adjustedRect.right(), zv_YRuleList->value(i).scenePos);
            }
        }
        painter->restore();
        return;
    }

    QGraphicsView::drawBackground(painter, rect);
}
//=============================================================
void ZPlotGraphicsView::zh_createConnections()
{

}
//=============================================================
