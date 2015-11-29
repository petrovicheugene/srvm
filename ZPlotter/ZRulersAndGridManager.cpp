//========================================================
#include "ZRulersAndGridManager.h"
#include "ZPlotGraphicsView.h"
#include "ZRulerWidget.h"

#include <QPoint>
#include <QPointF>
#include <QDebug>

//========================================================
ZRulersAndGridManager::ZRulersAndGridManager(QObject *parent) : QObject(parent)
{
    zv_markIntervalList << 1 << 2 << 5 << 10;
    zv_plotView = 0;
    zv_rulerWidget = 0;
    zv_minimalVerticalScaleInterval = 1.0;
    zv_minimalHorizontalScaleInterval = 1.0;
}
//========================================================
//QList<qreal> ZRulersAndGridManager::zp_XMarkGridList() const
//{
//    return zv_XMarkGridMap.values();
//}
////========================================================
//QList<qreal> ZRulersAndGridManager::zp_YMarkGridList() const
//{
//    return zv_YMarkGridMap.values();
//}
////========================================================
//QList<qreal> ZRulersAndGridManager::zp_XScratchGridList() const
//{
//    return zv_XScratchGridMap.values();
//}
////========================================================
//QList<qreal> ZRulersAndGridManager::zp_YScratchGridList() const
//{
//    return zv_YScratchGridMap.values();
//}
//========================================================
void ZRulersAndGridManager::zp_setPlotGraphicsView(ZPlotGraphicsView* plotView)
{
    zv_plotView = plotView;

    zv_plotView->zp_setHorizontalRulePointList(&zv_XRulePointList);
    zv_plotView->zp_setVerticalRulePointList(&zv_YRulePointList);

}
//========================================================
void ZRulersAndGridManager::zp_setRulerWidget(ZRulerWidget* rulerWidget)
{
    zv_rulerWidget = rulerWidget;
//    zv_rulerWidget->zp_setHorizontalGridMaps(&zv_XScratchGridMap);
//    zv_rulerWidget->zp_setHorizontalMarkMaps(&zv_XMarkGridMap);
//    zv_rulerWidget->zp_setVerticalGridMaps(&zv_YScratchGridMap);
//    zv_rulerWidget->zp_setVerticalMarkMaps(&zv_YMarkGridMap);

    zv_rulerWidget->zp_setHorizontalRulePointList(&zv_XRulePointList);
    zv_rulerWidget->zp_setVerticalRulePointList(&zv_YRulePointList);
}
//========================================================
bool ZRulersAndGridManager::zp_setMinimalVerticalScaleInterval(qreal interval)
{
    if(interval <= 0)
    {
        return false;
    }

    zv_minimalVerticalScaleInterval = interval;
    return true;
}
//========================================================
qreal ZRulersAndGridManager::zp_minimalVerticalScaleInterval()
{
    return zv_minimalVerticalScaleInterval;
}
//========================================================
bool ZRulersAndGridManager::zp_setMinimalHorizontalScaleInterval(qreal interval)
{
    if(interval <= 0)
    {
        return false;
    }

    zv_minimalHorizontalScaleInterval = interval;
    return true;
}
//========================================================
qreal ZRulersAndGridManager::zp_minimalHorizontalScaleInterval()
{
    return zv_minimalHorizontalScaleInterval;
}
//========================================================
void ZRulersAndGridManager::zp_recalcRulesAndGrid()
{
    if(zv_plotView == 0)
    {
        zv_XRulePointList.clear();
        zv_YRulePointList.clear();
        return;
    }

    QRect viewportGlobalRect;
    QRectF viewportSceneRect;
    QPoint globalZeroPoint;
    if(!zv_plotView->zp_viewPortCoordinates(viewportGlobalRect,viewportSceneRect, globalZeroPoint))
    {
        zv_XRulePointList.clear();
        zv_YRulePointList.clear();
        return;
    }

    if(zv_viewportGlobalRect == viewportGlobalRect
            && zv_viewportSceneRect == viewportSceneRect
            && zv_globalZeroPoint == globalZeroPoint)
    {
        // coordinates are not changed
        return;
    }

    zv_viewportGlobalRect = viewportGlobalRect;
    zv_viewportSceneRect = viewportSceneRect;
    zv_globalZeroPoint = globalZeroPoint;
    zh_recalcRectsForPaint();

    zh_recalcBottomRule();
    zh_recalcLeftRule();

    zv_rulerWidget->update();
}
//========================================================
void ZRulersAndGridManager::zh_recalcBottomRule()
{
    zv_XRulePointList.clear();

    if(zv_viewportGlobalRect.left() != zv_viewportGlobalRect.right()
            && zv_viewportSceneRect.left() != zv_viewportSceneRect.right()
            && zv_rulerWidget != 0)
    {
        int maxVpMarkInterval = zv_rulerWidget->zp_maxMarkWidth() * 2 ;
        qreal maxScMarkInterval = zv_plotView->mapToScene(QPoint(maxVpMarkInterval,0)).x()
                - zv_plotView->mapToScene(QPoint(0,0)).x();

        // new mark and scratch interval calculation
        qreal newScMarkInterval;
        qreal newScScratchInterval;

        if(maxScMarkInterval < zv_minimalHorizontalScaleInterval)
        {
            newScMarkInterval = zv_minimalHorizontalScaleInterval;
        }
        else
        {
            QString maxScMarkIntervalString = QString::number(maxScMarkInterval, 'E', 1);
            QStringList intervalPartList = maxScMarkIntervalString.split('E');
            QString integralPart = intervalPartList.value(0, QString());

            bool ok;
            double nIntegralPart = qAbs(integralPart.toDouble(&ok));
            if(!ok)
            {
                newScMarkInterval = maxScMarkInterval;
            }

            for(int i = 0; i < zv_markIntervalList.count(); i++)
            {
                if(zv_markIntervalList.value(i) < nIntegralPart)
                {
                    continue;
                }

                integralPart = QString::number(zv_markIntervalList.value(i), 'f', 1);
                maxScMarkIntervalString = integralPart + QString("E") + intervalPartList.value(1);

                newScMarkInterval = maxScMarkIntervalString.toDouble(&ok);
                if(!ok || newScMarkInterval < zv_minimalHorizontalScaleInterval)
                {
                    newScMarkInterval = maxScMarkInterval;
                }

                break;
            }

            // scratch interval calculation
            if(integralPart.at(0) == '1')
            {
                QList<qreal> devisionList;
                devisionList << 10.0 << 5.0 << 2.0;
                for(int i = 0; i < devisionList.count(); i++)
                {
                    newScScratchInterval = newScMarkInterval / devisionList.value(i, 1.0);
                    QPoint vp_scratchIntervalPos = zv_plotView->mapFromScene(QPointF(newScScratchInterval, zv_viewportSceneRect.top()));
                    int firstSratchXPos = zv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).x();
                    int newVpScratchInterval = qAbs(firstSratchXPos - zv_globalZeroPoint.x());
                    if(newVpScratchInterval < 10 || newScScratchInterval < zv_minimalHorizontalScaleInterval)
                    {
                        newScScratchInterval = newScMarkInterval;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else if(integralPart.at(0) == '2')
            {
                QList<qreal> devisionList;
                devisionList << 20.0 << 4.0 << 2.0;
                for(int i = 0; i < devisionList.count(); i++)
                {
                    newScScratchInterval = newScMarkInterval / devisionList.value(i, 1.0);
                    QPoint vp_scratchIntervalPos = zv_plotView->mapFromScene(QPointF(newScScratchInterval, zv_viewportSceneRect.top()));
                    int firstSratchXPos = zv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).x();
                    int newVpScratchInterval = qAbs(firstSratchXPos - zv_globalZeroPoint.x());
                    if(newVpScratchInterval < 10 || newScScratchInterval < zv_minimalHorizontalScaleInterval)
                    {
                        newScScratchInterval = newScMarkInterval;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else if(integralPart.at(0) == '5')
            {
                newScScratchInterval = newScMarkInterval / 5.0;
                QPoint vp_scratchIntervalPos = zv_plotView->mapFromScene(QPointF(newScScratchInterval, zv_viewportSceneRect.top()));
                int firstSratchXPos = zv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).x();
                int newVpScratchInterval = qAbs(firstSratchXPos - zv_globalZeroPoint.x());
                if(newVpScratchInterval < 10 || newScScratchInterval < zv_minimalHorizontalScaleInterval)
                {
                    newScScratchInterval = newScMarkInterval;
                }
            }
        }  // end new mark interval calculation


        // vp-coord = sc-coord*k + a
        // solution:
        // k = (vp-max - vp-min) / (sc-max - sc-min)
        // a = vp-min - sc-min*k

        qreal k;
        qreal a;

        k = ((qreal)zv_viewportGlobalRect.right() - (qreal)zv_viewportGlobalRect.left())
                / (zv_viewportSceneRect.right() - zv_viewportSceneRect.left());
        a = (qreal)zv_viewportGlobalRect.left() - zv_viewportSceneRect.left()*k;


        int markScratchesInterval;
        if(newScScratchInterval > 0)
        {
            markScratchesInterval = qRound(newScMarkInterval / newScScratchInterval);
        }
        else
        {
            markScratchesInterval = 1;
        }

        int emptyMarkScratchesInterval;
        switch(markScratchesInterval)
        {
        case 1:
        case 2:
            emptyMarkScratchesInterval = 1;
            break;
        case 4:
            emptyMarkScratchesInterval = 2;
            break;
        case 5:
            emptyMarkScratchesInterval = markScratchesInterval;
            break;
        case 10:
            emptyMarkScratchesInterval = 5;
            break;
        case 20:
            emptyMarkScratchesInterval = 5;
            break;
        default:
            emptyMarkScratchesInterval = markScratchesInterval;
        }

        int scratchInMarkIntervalCount = 0;
        int scratchInEmptyMarkInterval = 0;
        RulePoint rulePoint;
        RulePoint::MarkType markType;
        // Marks
        for(qreal sc = newScScratchInterval; sc <= zv_sceneRectForPaint.right(); sc+= newScScratchInterval)
        {
            // count in mark interval
            if(++scratchInMarkIntervalCount >= markScratchesInterval)
            {
                scratchInMarkIntervalCount = 0;
            }

            if(++scratchInEmptyMarkInterval >= emptyMarkScratchesInterval)
            {
                scratchInEmptyMarkInterval = 0;
            }

            if(sc < zv_sceneRectForPaint.left())
            {
                continue;
            }

            if(scratchInMarkIntervalCount == 0)
            {
                markType = RulePoint::MT_MARK;
            }
            else if(scratchInEmptyMarkInterval == 0)
            {
                markType = RulePoint::MT_EMPTY_MARK;
            }
            else
            {
                markType = RulePoint::MT_SCRATCH;
            }

            qreal vp = sc*k + a;
            rulePoint = RulePoint(vp, sc, markType);
            zv_XRulePointList.append(rulePoint);
        }

        scratchInMarkIntervalCount = -1;
        scratchInEmptyMarkInterval = -1;
        for(qreal sc = 0; sc >= zv_sceneRectForPaint.left(); sc-= newScScratchInterval)
        {
            // count in mark interval
            if(++scratchInMarkIntervalCount >= markScratchesInterval)
            {
                scratchInMarkIntervalCount = 0;
            }

            if(++scratchInEmptyMarkInterval >= emptyMarkScratchesInterval)
            {
                scratchInEmptyMarkInterval = 0;
            }

            if(sc > zv_sceneRectForPaint.right())
            {
                continue;
            }

            if(scratchInMarkIntervalCount == 0)
            {
                markType = RulePoint::MT_MARK;
            }
            else if(scratchInEmptyMarkInterval == 0)
            {
                markType = RulePoint::MT_EMPTY_MARK;
            }
            else
            {
                markType = RulePoint::MT_SCRATCH;
            }

            qreal vp = sc*k + a;
            rulePoint = RulePoint(vp, sc, markType);
            zv_XRulePointList.append(rulePoint);
        }
    }
}
//========================================================
void ZRulersAndGridManager::zh_recalcLeftRule()
{
    zv_YRulePointList.clear();

    if(zv_viewportGlobalRect.top() != zv_viewportGlobalRect.bottom()
            && zv_viewportSceneRect.top() != zv_viewportSceneRect.bottom()
            && zv_rulerWidget != 0)
    {
        int maxVpMarkInterval = zv_rulerWidget->zp_maxMarkHeight() * 3 ;
        qreal maxScMarkInterval = zv_plotView->mapToScene(QPoint(0, maxVpMarkInterval)).y()
                - zv_plotView->mapToScene(QPoint(0,0)).y();

        // new mark and scratch interval calculation
        qreal newScMarkInterval;
        qreal newScScratchInterval;

        if(maxScMarkInterval < zv_minimalVerticalScaleInterval)
        {
            newScMarkInterval = zv_minimalVerticalScaleInterval;
        }
        else
        {
            QString maxScMarkIntervalString = QString::number(maxScMarkInterval, 'E', 1);
            QStringList intervalPartList = maxScMarkIntervalString.split('E');
            QString integralPart = intervalPartList.value(0, QString());

            bool ok;
            double nIntegralPart = qAbs(integralPart.toDouble(&ok));
            if(!ok)
            {
                newScMarkInterval = maxScMarkInterval;
            }

            for(int i = 0; i < zv_markIntervalList.count(); i++)
            {
                if(zv_markIntervalList.value(i) < nIntegralPart)
                {
                    continue;
                }

                integralPart = QString::number(zv_markIntervalList.value(i), 'f', 1);
                maxScMarkIntervalString = integralPart + QString("E") + intervalPartList.value(1);

                newScMarkInterval = maxScMarkIntervalString.toDouble(&ok);
                if(!ok || newScMarkInterval < zv_minimalHorizontalScaleInterval)
                {
                    newScMarkInterval = maxScMarkInterval;
                }

                break;
            }

            // scratch interval calculation

            if(integralPart.at(0) == '1')
            {
                QList<qreal> devisionList;
                devisionList << 10.0 << 5.0 << 2.0;
                for(int i = 0; i < devisionList.count(); i++)
                {
                    newScScratchInterval = newScMarkInterval / devisionList.value(i, 1.0);
                    QPoint vp_scratchIntervalPos = zv_plotView->mapFromScene(QPointF(zv_viewportSceneRect.left(), newScScratchInterval));
                    int firstSratchYPos = zv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).y();
                    int newVpScratchInterval = qAbs(firstSratchYPos - zv_globalZeroPoint.y());
                    if(newVpScratchInterval < 10 || newScScratchInterval < zv_minimalVerticalScaleInterval)
                    {
                        newScScratchInterval = newScMarkInterval;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else if(integralPart.at(0) == '2')
            {
                QList<qreal> devisionList;
                devisionList << 20.0 << 4.0 << 2.0;
                for(int i = 0; i < devisionList.count(); i++)
                {
                    newScScratchInterval = newScMarkInterval / devisionList.value(i, 1.0);
                    QPoint vp_scratchIntervalPos = zv_plotView->mapFromScene(QPointF(zv_viewportSceneRect.left(), newScScratchInterval));
                    int firstSratchYPos = zv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).y();
                    int newVpScratchInterval = qAbs(firstSratchYPos - zv_globalZeroPoint.y());
                    if(newVpScratchInterval < 10 || newScScratchInterval < zv_minimalVerticalScaleInterval)
                    {
                        newScScratchInterval = newScMarkInterval;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else if(integralPart.at(0) == '5')
            {
                newScScratchInterval = newScMarkInterval / 5.0;
                QPoint vp_scratchIntervalPos = zv_plotView->mapFromScene(QPointF(zv_viewportSceneRect.left(), newScScratchInterval));
                int firstSratchYPos = zv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).y();
                int newVpScratchInterval = qAbs(firstSratchYPos - zv_globalZeroPoint.y());
                if(newVpScratchInterval < 10 || newScScratchInterval < zv_minimalVerticalScaleInterval)
                {
                    newScScratchInterval = newScMarkInterval;
                }
            }
        }  // end new mark interval calculation

        // vp-coord = sc-coord*k + a
        // solution:
        // k = (vp-max - vp-min) / (sc-max - sc-min)
        // a = vp-min - sc-min*k

        qreal k;
        qreal a;

        k = ((qreal)zv_viewportGlobalRect.bottom() - (qreal)zv_viewportGlobalRect.top())
                / (zv_viewportSceneRect.bottom() - zv_viewportSceneRect.top());
        a = (qreal)zv_viewportGlobalRect.top() - zv_viewportSceneRect.top()*k;

        int markScratchesInterval;
        if(newScScratchInterval > 0)
        {
            markScratchesInterval = qRound(newScMarkInterval / newScScratchInterval);
        }
        else
        {
            markScratchesInterval = 1;
        }

        int emptyMarkScratchesInterval;
        switch(markScratchesInterval)
        {
        case 1:
        case 2:
            emptyMarkScratchesInterval = 1;
            break;
        case 4:
            emptyMarkScratchesInterval = 2;
            break;
        case 5:
            emptyMarkScratchesInterval = markScratchesInterval;
            break;
        case 10:
            emptyMarkScratchesInterval = 5;
            break;
        case 20:
            emptyMarkScratchesInterval = 5;
            break;
        default:
            emptyMarkScratchesInterval = markScratchesInterval;
        }


        int scratchInMarkIntervalCount = 0;
        int scratchInEmptyMarkInterval = 0;
        RulePoint rulePoint;
        RulePoint::MarkType markType;
        // Marks
        for(qreal sc = newScScratchInterval; sc <= zv_sceneRectForPaint.bottom(); sc+= newScScratchInterval)
        {
            // count in mark interval
            if(++scratchInMarkIntervalCount >= markScratchesInterval)
            {
                scratchInMarkIntervalCount = 0;
            }

            if(++scratchInEmptyMarkInterval >= emptyMarkScratchesInterval)
            {
                scratchInEmptyMarkInterval = 0;
            }

            if(sc < zv_sceneRectForPaint.top() )
            {
                continue;
            }

            if(scratchInMarkIntervalCount == 0)
            {
                markType = RulePoint::MT_MARK;
            }
            else if(scratchInEmptyMarkInterval == 0)
            {
                markType = RulePoint::MT_EMPTY_MARK;
            }
            else
            {
                markType = RulePoint::MT_SCRATCH;
            }

            qreal vp = sc*k + a;
            rulePoint = RulePoint(vp, sc, markType);
            zv_YRulePointList.append(rulePoint);
        }

        scratchInMarkIntervalCount = -1;
        scratchInEmptyMarkInterval = -1;
        for(qreal sc = 0; sc >= zv_sceneRectForPaint.top(); sc-= newScScratchInterval)
        {
            // count in mark interval
            if(++scratchInMarkIntervalCount >= markScratchesInterval)
            {
                scratchInMarkIntervalCount = 0;
            }

            if(++scratchInEmptyMarkInterval >= emptyMarkScratchesInterval)
            {
                scratchInEmptyMarkInterval = 0;
            }

            if(sc > zv_sceneRectForPaint.bottom())
            {
                continue;
            }

            if(scratchInMarkIntervalCount == 0)
            {
                markType = RulePoint::MT_MARK;
            }
            else if(scratchInEmptyMarkInterval == 0)
            {
                markType = RulePoint::MT_EMPTY_MARK;
            }
            else
            {
                markType = RulePoint::MT_SCRATCH;
            }

            qreal vp = sc*k + a;
            rulePoint = RulePoint(vp, sc, markType);
            zv_YRulePointList.append(rulePoint);
        }
    }
}
//========================================================
void ZRulersAndGridManager::zh_recalcRectsForPaint()
{
    qreal K_width = zv_viewportSceneRect.width() /
            (qreal)zv_viewportGlobalRect.width();
    qreal K_height = zv_viewportSceneRect.height() /
            (qreal)zv_viewportGlobalRect.height();

    int dPix = 1;
    zv_globalRectForPaint = zv_viewportGlobalRect.adjusted(-1*dPix, -1*dPix, dPix, dPix);
    zv_sceneRectForPaint = zv_viewportSceneRect.adjusted(-1*K_width*dPix, -1*K_height*dPix, K_width*dPix, K_height*dPix);
}
//========================================================

