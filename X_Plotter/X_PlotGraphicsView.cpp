//=============================================================
#include "X_PlotGraphicsView.h"
#include "X_RulersAndGridManager.h"
#include "X_General.h"

#include <QApplication>
#include <QDebug>
#include <QRubberBand>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QPixmap>
//=============================================================
X_PlotGraphicsView::X_PlotGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    xv_plotMode = PM_REGULAR;
    xv_sceneCenterPos = QPointF();
    xv_sceneMousePos = QPointF();
    xv_mousePressStartViewPos = QPoint();
    xv_rubberBand = nullptr;
    xv_scaleViewWhenResizeFlag = false;
    xv_drawGridFlag = true;
    xv_rulersAndGreedManager = nullptr;
    xv_minSideSizeOfVisibleScene = 15;
    xv_rubberBandSideMinSize = 5;
    xv_colorPickUpAuxCoverageSize = 1;
    xv_gridColor = viewport()->palette().color(QPalette::Mid);

    xv_XRuleList = nullptr;
    xv_YRuleList = nullptr;

    viewport()->setMinimumSize(QSize(100, 100));

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setMouseTracking(true);
    xh_createConnections();
}
//=============================================================
void X_PlotGraphicsView::xp_setScaleViewWhenResizeFlag(bool flag)
{
    xv_scaleViewWhenResizeFlag = flag;
}
//=============================================================
void X_PlotGraphicsView::xp_setViewport(QWidget * widget)
{
    QGraphicsView::setViewport(widget);
}
//=============================================================
void X_PlotGraphicsView::xp_connectToRulersAndGridManager(X_RulersAndGridManager* manager)
{
    xv_rulersAndGreedManager = manager;
    xv_rulersAndGreedManager->xp_setPlotGraphicsView(this);
}
//=============================================================
void X_PlotGraphicsView::xp_setHorizontalRulePointList(QList<RulePoint> * XRuleList)
{
    xv_XRuleList = XRuleList;
}
//=============================================================
void X_PlotGraphicsView::xp_setVerticalRulePointList(QList<RulePoint> * YRuleList)
{
    xv_YRuleList = YRuleList;
}
//=============================================================
bool X_PlotGraphicsView::xp_viewPortCoordinates(QRect& globalRect, QRectF& sceneRect, QPoint& globalX_eroPoint)
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

    // global X_ero Point
    globalX_eroPoint = viewport()->mapToGlobal(mapFromScene(0.0, 0.0));

    return true;
}
//=============================================================
bool X_PlotGraphicsView::xp_viewPortSceneRect(QRectF&sceneRect)
{
    if(!scene())
    {
        return false;
    }

    sceneRect = mapToScene(viewport()->rect().adjusted(1,1,-1,-1)).boundingRect();
    return true;
}
//=============================================================
void X_PlotGraphicsView::xp_scale(Qt::Orientation orientation, bool increase)
{
    QRectF oldDisplayedSceneRect = mapToScene(viewport()->rect()).boundingRect();
    QRectF sceneItemBoundingRect = scene()->itemsBoundingRect();

    qreal scaleRate;
    if(orientation == Qt::Vertical)
    {
        if(increase)
        {
            scaleRate = oldDisplayedSceneRect.height() / xv_minSideSizeOfVisibleScene;
            if(scaleRate > 1.1)
            {
                scaleRate = 1.1;
            }
            if(scaleRate < 1.0)
            {
                scaleRate = 1.0;
            }
        }
        else
        {
            scaleRate = oldDisplayedSceneRect.height() / sceneItemBoundingRect.height();
            if(scaleRate < 0.9)
            {
                scaleRate = 0.9;
            }
            if(scaleRate > 1.0)
            {
                scaleRate = 1.0;
            }
        }

        scale(1, scaleRate);
        // center moving
        if(scaleRate > 1)
        {
            QRectF newDisplayedSceneRect = mapToScene(viewport()->rect()).boundingRect();
            qreal centerPosShift = (oldDisplayedSceneRect.height() - newDisplayedSceneRect.height()) / 2;
            xv_sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();
            xv_sceneCenterPos.setY(xv_sceneCenterPos.y() + centerPosShift);
            centerOn(xv_sceneCenterPos);
        }
    }
    else
    {
        if(increase)
        {
            scaleRate = oldDisplayedSceneRect.width() / xv_minSideSizeOfVisibleScene;
            if(scaleRate > 1.1)
            {
                scaleRate = 1.1;
            }
            else if(scaleRate < 1.0)
            {
                scaleRate = 1.0;
            }
        }
        else
        {
            scaleRate = oldDisplayedSceneRect.width() / sceneItemBoundingRect.width();
            if(scaleRate < 0.9)
            {
                scaleRate = 0.9;
            }
            else if(scaleRate > 1.0)
            {
                scaleRate = 1.0;
            }
        }
        scale(scaleRate, 1);
    }

    //    QRectF oldDisplayedSceneRect = mapToScene(viewport()->rect().adjusted(2,-1,-2,1)).boundingRect();
    //    //QRectF oldDisplayedSceneRect = mapToScene(viewport()->rect()).boundingRect();
    //    QRectF displayedSceneRect = oldDisplayedSceneRect;
    //    QRectF sceneItemBoundingRect = scene()->itemsBoundingRect();

    //    qreal scaleRate;
    //    if(increase)
    //    {
    //        scaleRate = 0.1;
    //    }
    //    else
    //    {
    //        scaleRate = -0.1;
    //    }

    //    if(orientation == Qt::Vertical)
    //    {
    //        if(increase)
    //        {
    //            displayedSceneRect.setTop(displayedSceneRect.top() + displayedSceneRect.height() * scaleRate);
    //        }
    //        else
    //        {
    //            displayedSceneRect.setTop(displayedSceneRect.top() + displayedSceneRect.height() * scaleRate/2);
    //            // displayedSceneRect.setBottom(displayedSceneRect.bottom() - displayedSceneRect.height() * scaleRate/2);
    //        }

    ////        if(displayedSceneRect.top() < sceneItemBoundingRect.top())
    ////        {
    ////            displayedSceneRect.setTop(sceneItemBoundingRect.top());
    ////        }
    ////        if(displayedSceneRect.bottom() > sceneItemBoundingRect.bottom())
    ////        {
    ////            displayedSceneRect.setBottom(sceneItemBoundingRect.bottom());
    ////        }
    //    }
    //    else
    //    {
    //        displayedSceneRect.setRight(displayedSceneRect.right() - displayedSceneRect.width() * scaleRate);
    //        displayedSceneRect.setLeft(displayedSceneRect.left() + displayedSceneRect.width() * scaleRate);

    ////        if(displayedSceneRect.left() < sceneItemBoundingRect.left())
    ////        {
    ////            displayedSceneRect.setLeft(sceneItemBoundingRect.left());
    ////        }
    ////        if(displayedSceneRect.right() > sceneItemBoundingRect.right())
    ////        {
    ////            displayedSceneRect.setRight(sceneItemBoundingRect.right());
    ////        }
    //    }

    //    if(orientation == Qt::Horizontal && (displayedSceneRect.width() <= xv_minSideSizeOfVisibleScene
    //                                         || displayedSceneRect.width() > sceneItemBoundingRect.width()))
    //    {
    //        return;
    //    }

    //    if(orientation == Qt::Vertical && (displayedSceneRect.height() <= xv_minSideSizeOfVisibleScene
    //                                       || displayedSceneRect.height() > sceneItemBoundingRect.height()))
    //    {
    //        return;
    //    }

    //    fitInView(displayedSceneRect.normalized());
    //    //ensureVisible(displayedSceneRect.normalized(), 2, 2);
}
//=============================================================
void X_PlotGraphicsView::xp_setGridHidden(bool hideGrid)
{
    xv_drawGridFlag = !hideGrid;
    viewport()->update();
}
//=============================================================
bool X_PlotGraphicsView::xp_setColorPickUpAuxCoverageSize(int size)
{
    if(size < 0)
    {
        return false;
    }
    xv_colorPickUpAuxCoverageSize = size;
    return true;
}
//=============================================================
int X_PlotGraphicsView::xp_colorPickUpAuxCoverageSize() const
{
    return xv_colorPickUpAuxCoverageSize;
}
//=============================================================
bool X_PlotGraphicsView::xp_setRubberBandSideMinSize(int size)
{
    if(size < 0)
    {
        return false;
    }
    xv_rubberBandSideMinSize = size;
    return true;
}
//=============================================================
int X_PlotGraphicsView::xp_rubberBandSideMinSize() const
{
    return xv_rubberBandSideMinSize;
}
//=============================================================
QColor X_PlotGraphicsView::xp_gridColor() const
{
    return xv_gridColor;
}
//=============================================================
void X_PlotGraphicsView::xp_fitInView(const QRectF &rect, Qt::AspectRatioMode aspectRatioMode)
{
    QGraphicsView::fitInView(rect, aspectRatioMode);
    emit xg_viewportRectChanged(rect);

    if(xv_plotMode == PM_RULER)
    {
        emit xg_rulerToolChanged(mapToScene(xv_mousePressStartViewPos),
                                 mapToScene(xv_currentMousePos), true);
    }
}
//=============================================================
void X_PlotGraphicsView::xp_update()
{
    viewport()->update();
}
//=============================================================
QRectF X_PlotGraphicsView::xp_currentVisibleSceneRect() const
{
    // QRect rect = viewport()->rect().adjusted(1,1,-1,-1);
    QRect rect = viewport()->rect().adjusted(2,-1,-2,1);
    QPointF sceneTopLeft = mapToScene(rect.topLeft());
    QPointF scenebottomRight = mapToScene(rect.bottomRight());

    QRectF sRect(sceneTopLeft, scenebottomRight);
    return sRect;
}
//=============================================================
void X_PlotGraphicsView::wheelEvent(QWheelEvent * event)
{
    if(xv_plotMode != PM_REGULAR || !scene())
    {
        return;
    }

    xv_sceneMousePos = mapToScene(event->position().toPoint());
    QRectF oldDisplayedSceneRect = mapToScene(viewport()->rect().adjusted(2,-1,-2,1)).boundingRect();
    //QRectF oldDisplayedSceneRect = mapToScene(viewport()->rect()).boundingRect();
    QRectF displayedSceneRect = oldDisplayedSceneRect;
    qreal rightPartFactor = 0.1 *(displayedSceneRect.right() - xv_sceneMousePos.x())
            / (displayedSceneRect.right() - displayedSceneRect.left());
    qreal topPartFactor = 0.1* (xv_sceneMousePos.y() - displayedSceneRect.top())
            / (displayedSceneRect.bottom() - displayedSceneRect.top());

    qreal leftPartFactor = 0.1 - rightPartFactor;
    qreal bottomPartFactor = 0.1 - topPartFactor;

    if(event->angleDelta().y() > 0)
    {
        topPartFactor *= -1;
        leftPartFactor *= -1;
    }
    else
    {
        rightPartFactor *= -1;
        bottomPartFactor *= -1;
    }

    displayedSceneRect.setTop(displayedSceneRect.top() + topPartFactor*displayedSceneRect.height());
    displayedSceneRect.setBottom(displayedSceneRect.bottom() + bottomPartFactor*displayedSceneRect.height());
    displayedSceneRect.setLeft(displayedSceneRect.left() + leftPartFactor*displayedSceneRect.width());
    displayedSceneRect.setRight(displayedSceneRect.right() + rightPartFactor*displayedSceneRect.width());

    QRectF sceneItemBoundingRect = scene()->itemsBoundingRect();
    if(displayedSceneRect.top() < sceneItemBoundingRect.top())
    {
        displayedSceneRect.setTop(sceneItemBoundingRect.top());
    }
    if(displayedSceneRect.bottom() > sceneItemBoundingRect.bottom())
    {
        displayedSceneRect.setBottom(sceneItemBoundingRect.bottom());
    }
    if(displayedSceneRect.left() < sceneItemBoundingRect.left())
    {
        displayedSceneRect.setLeft(sceneItemBoundingRect.left());
    }
    if(displayedSceneRect.right() > sceneItemBoundingRect.right())
    {
        displayedSceneRect.setRight(sceneItemBoundingRect.right());
    }

    if(displayedSceneRect.height() < xv_minSideSizeOfVisibleScene)
    {
        displayedSceneRect.setTop(oldDisplayedSceneRect.top());
        displayedSceneRect.setBottom(oldDisplayedSceneRect.bottom());
    }
    if(displayedSceneRect.width() < xv_minSideSizeOfVisibleScene)
    {
        displayedSceneRect.setLeft(oldDisplayedSceneRect.left());
        displayedSceneRect.setRight(oldDisplayedSceneRect.right());
    }

    xp_fitInView(displayedSceneRect.normalized());
    //ensureVisible(displayedSceneRect.normalized(), 2, 2);
    xv_sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();
    xv_sceneMousePos = mapToScene(event->position().toPoint());
}
//=============================================================
void X_PlotGraphicsView::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        // pad moving
        //      QGraphicsItem* item = itemAt(event->pos());
        //      if(!item || !(item->flags() & QGraphicsItem::ItemIsMovable))
        //      if(!xh_findSpectrumInCursorArea(event->pos()))
        //      {
        //xv_plotMode = PM_PAD_DRAGGING;
        xv_sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();
        xv_sceneMousePos = mapToScene(event->pos());
        xv_mousePressStartViewPos = event->pos();
        return;
        // }
    }
    else if(event->button() == Qt::RightButton)
    {
        xv_mousePressStartViewPos = event->pos();
    }

    QGraphicsView::mousePressEvent(event);
}
//=============================================================
void X_PlotGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if(xv_plotMode == PM_PAD_DRAGGING)
    {

    }
    else if(xv_plotMode == PM_RUBBER_BAND)
    {
        xv_rubberBand->hide();
        QPoint topLeft;
        if(xv_mousePressStartViewPos.x() > event->pos().x())
        {
            topLeft.setX(event->pos().x());
        }
        else
        {
            topLeft.setX(xv_mousePressStartViewPos.x());
        }

        if(xv_mousePressStartViewPos.y() > event->pos().y())
        {
            topLeft.setY(event->pos().y());
        }
        else
        {
            topLeft.setY(xv_mousePressStartViewPos.y());
        }

        QRect rubberBandRect = xv_rubberBand->rect();
        rubberBandRect.moveTo(topLeft);

        QRectF newSceneRect = mapToScene(rubberBandRect).boundingRect().normalized();
        QPointF newSceneCenter =  newSceneRect.center();
        if(newSceneRect.width() < xv_minSideSizeOfVisibleScene)
        {
            newSceneRect.setWidth(xv_minSideSizeOfVisibleScene);
        }
        if(newSceneRect.height() < xv_minSideSizeOfVisibleScene)
        {
            newSceneRect.setHeight(xv_minSideSizeOfVisibleScene);
        }

        if(newSceneRect.center() != newSceneCenter)
        {
            newSceneRect.moveCenter(newSceneCenter);
        }
        xp_fitInView(newSceneRect);
        //ensureVisible(newSceneRect, 2, 2);
    }
    else if(xv_plotMode == PM_REGULAR && xv_mousePressStartViewPos == event->pos())
    {
        emit xg_cursorAreaImage(xh_grabCursorArea(event->pos()));
    }
    else if(xv_plotMode == PM_RULER)
    {
        emit xg_rulerToolChanged(QPointF(), QPointF(), false);
    }

    qApp->restoreOverrideCursor();
    xv_plotMode = PM_REGULAR;
    QGraphicsView::mouseReleaseEvent(event);
}
//=============================================================
void X_PlotGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    emit xg_mouseScenePositionChanged(mapToScene(event->pos()));

    xv_currentMousePos = event->pos();

    if(xv_plotMode == PM_PAD_DRAGGING)
    {
        QPointF newSceneMousePos = mapToScene(event->pos());
        qreal deltaX = xv_sceneMousePos.x() - newSceneMousePos.x();
        qreal deltaY = xv_sceneMousePos.y() - newSceneMousePos.y();
        QPointF newSceneCenter(xv_sceneCenterPos.x() + deltaX, xv_sceneCenterPos.y() + deltaY);
        centerOn(newSceneCenter);
        xv_sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();
        xv_sceneMousePos = mapToScene(event->pos());
        return;
    }
    else if(xv_plotMode == PM_RUBBER_BAND)
    {
        if(qAbs(xv_mousePressStartViewPos.x() - xv_currentMousePos.x()) < xv_rubberBandSideMinSize
                || qAbs(xv_mousePressStartViewPos.y() - xv_currentMousePos.y()) < xv_rubberBandSideMinSize)
        {
            xv_rubberBand->hide();
            xv_plotMode = PM_REGULAR;
        }
        else
        {
            xv_rubberBand->setGeometry(QRect(xv_mousePressStartViewPos, event->pos()).normalized());
            return;
        }
    }
    else if(xv_plotMode == PM_REGULAR && event->buttons() == Qt::RightButton)
    {
        if(qAbs(xv_mousePressStartViewPos.x() - xv_currentMousePos.x()) > xv_rubberBandSideMinSize
                && qAbs(xv_mousePressStartViewPos.y() - xv_currentMousePos.y()) > xv_rubberBandSideMinSize
                && !(event->modifiers() & Qt::CTRL))
        {
            QRectF sceneRect = mapToScene(viewport()->rect().adjusted(1,1,-1,-1)).boundingRect().normalized();
            if(sceneRect.width() > xv_minSideSizeOfVisibleScene + 1
                    || sceneRect.height() > xv_minSideSizeOfVisibleScene +1)
            {
                xv_plotMode = PM_RUBBER_BAND;
                if(!xv_rubberBand)
                {
                    xv_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
                }
                xv_rubberBand->setGeometry(QRect(xv_mousePressStartViewPos, QSize()));
                xv_rubberBand->show();
            }
        }
        else if(event->modifiers() & Qt::CTRL/*qAbs(xv_mousePressStartViewPos.x() - xv_currentMousePos.x()) > xv_ruleToolMinSize*/)
        {
            xv_plotMode = PM_RULER;
            emit xg_rulerToolChanged(mapToScene(xv_mousePressStartViewPos),
                                     mapToScene(xv_currentMousePos), true);
        }
        else
        {
            xv_plotMode = PM_REGULAR;
            emit xg_rulerToolChanged(QPointF(), QPointF(), false);
        }
    }
    else if(xv_plotMode == PM_REGULAR && event->buttons() == Qt::LeftButton)
    {
        if(xv_mousePressStartViewPos != event->pos())
        {
            qApp->setOverrideCursor(Qt::ClosedHandCursor);
            xv_plotMode = PM_PAD_DRAGGING;
            QPointF newSceneMousePos = mapToScene(event->pos());
            qreal deltaX = xv_sceneMousePos.x() - newSceneMousePos.x();
            qreal deltaY = xv_sceneMousePos.y() - newSceneMousePos.y();
            QPointF newSceneCenter(xv_sceneCenterPos.x() + deltaX, xv_sceneCenterPos.y() + deltaY);
            centerOn(newSceneCenter);
            xv_sceneCenterPos = mapToScene(viewport()->rect()).boundingRect().center();
            xv_sceneMousePos = mapToScene(event->pos());
            return;
        }
    }
    else if(xv_plotMode == PM_RULER)
    {
        if(qAbs(xv_mousePressStartViewPos.x() - xv_currentMousePos.x()) > xv_rubberBandSideMinSize
                && qAbs(xv_mousePressStartViewPos.y() - xv_currentMousePos.y()) > xv_rubberBandSideMinSize
                && !(event->modifiers() & Qt::CTRL))
        {
            QRectF sceneRect = mapToScene(viewport()->rect().adjusted(1,1,-1,-1)).boundingRect().normalized();
            if(sceneRect.width() > xv_minSideSizeOfVisibleScene + 1
                    || sceneRect.height() > xv_minSideSizeOfVisibleScene +1)
            {
                xv_plotMode = PM_RUBBER_BAND;
                emit xg_rulerToolChanged(QPointF(), QPointF(), false);
                if(!xv_rubberBand)
                {
                    xv_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
                }
                xv_rubberBand->setGeometry(QRect(xv_mousePressStartViewPos, QSize()));
                xv_rubberBand->show();
            }
        }
        else if(event->modifiers() & Qt::CTRL)
        {
            emit xg_rulerToolChanged(mapToScene(xv_mousePressStartViewPos),
                                     mapToScene(xv_currentMousePos), true);
        }
        else
        {
            xv_plotMode = PM_REGULAR;
            emit xg_rulerToolChanged(QPointF(), QPointF(), false);
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}
//=============================================================
/* bool X_PlotGraphicsView::viewportEvent(QEvent * event)
{
    if(event->type() == QEvent::Paint && xv_plotMode == PM_RULER)
    {
        emit xg_rulerToolChanged(mapToScene(xv_mousePressStartViewPos),
                                 mapToScene(xv_currentMousePos), true);
    }

    if(event->type() == QEvent::HoverLeave || event->type() == QEvent::Leave)
    {
        emit xg_mouseLeaved();
    }

    if(event->type() == QEvent::Resize && scene())
    {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);

        if(xv_scaleViewWhenResizeFlag
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
    else if(event->type() == QEvent::Paint && scene())
    {
        if(xv_rulersAndGreedManager)
        {
            xv_rulersAndGreedManager->xp_recalcRulesAndGrid();
        }
    }

    return QGraphicsView::viewportEvent(event);
}
*/
//=============================================================
bool X_PlotGraphicsView::viewportEvent(QEvent * event)
{
    if(event->type() == QEvent::Paint && scene())
    {
        QTransform matrix = transform();

        if(matrix.m11() < 0.0 || matrix.m22() < 0.0 || matrix.m33() < 0.0)
        {
            matrix.reset();
            setTransform(matrix);
            xp_fitInView(sceneRect());
        }

        if( xv_plotMode == PM_RULER)
        {
            emit xg_rulerToolChanged(mapToScene(xv_mousePressStartViewPos),
                                     mapToScene(xv_currentMousePos), true);
        }

        if(xv_rulersAndGreedManager )
        {
            xv_rulersAndGreedManager->xp_recalcRulesAndGrid();
        }
    }
    else if(event->type() == QEvent::HoverLeave || event->type() == QEvent::Leave)
    {
        emit xg_mouseLeaved();
    }
    else if(event->type() == QEvent::Resize && scene())
    {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);

        if(xv_scaleViewWhenResizeFlag
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

    return QGraphicsView::viewportEvent(event);
}
//=============================================================
void X_PlotGraphicsView::drawBackground(QPainter * painter, const QRectF & rect)
{
    if(xv_rulersAndGreedManager != 0 && xv_drawGridFlag)
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
        if(xv_XRuleList != 0)
        {
            for(int i = 0; i < xv_XRuleList->count(); i++)
            {
                // pen settings
                QPen pen;
                if(xv_XRuleList->value(i).markType == RulePoint::MT_SCRATCH)
                {
                    pen = QPen(QBrush(xv_gridColor), 1, Qt::DotLine);
                    pen.setCosmetic(true);
                }
                else
                {
                    pen = QPen(QBrush(xv_gridColor), 1, Qt::DashLine);
                    pen.setCosmetic(true);
                }
                painter->setPen(pen);
                painter->drawLine(xv_XRuleList->value(i).scenePos, adjustedRect.top(),
                                  xv_XRuleList->value(i).scenePos, adjustedRect.bottom());
            }
        }

        // Y
        if(xv_YRuleList != 0)
        {
            for(int i = 0; i < xv_YRuleList->count(); i++)
            {
                // pen settings
                QPen pen;
                if(xv_YRuleList->value(i).markType == RulePoint::MT_SCRATCH)
                {
                    pen = QPen(QBrush(xv_gridColor), 1, Qt::DotLine);
                    pen.setCosmetic(true);
                }
                else
                {
                    pen = QPen(QBrush(xv_gridColor), 1, Qt::DashLine);
                    pen.setCosmetic(true);
                }
                painter->setPen(pen);
                painter->drawLine(adjustedRect.left(), xv_YRuleList->value(i).scenePos,
                                  adjustedRect.right(), xv_YRuleList->value(i).scenePos);
            }
        }
        painter->restore();
        return;
    }

    QGraphicsView::drawBackground(painter, rect);
}
//=============================================================
void X_PlotGraphicsView::xh_createConnections()
{

}
//=============================================================
QImage X_PlotGraphicsView::xh_grabCursorArea(QPoint mousePos)
{
    int rectSide = xv_colorPickUpAuxCoverageSize*2 + 1; // 1 - central pixel
    QPoint topLeft;
    topLeft.setY(mousePos.y() - xv_colorPickUpAuxCoverageSize);
    topLeft.setX(mousePos.x() - xv_colorPickUpAuxCoverageSize);

    QRect rect = QRect(topLeft.x(), topLeft.y(), rectSide, rectSide);
    QImage grabbedArea = grab(rect).toImage();

    return grabbedArea;
}
//=============================================================
