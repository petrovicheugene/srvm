//========================================================
#include "X_RulersAndGridManager.h"
#include "X_PlotGraphicsView.h"
#include "X_RulerWidget.h"
#include "X_Plotter.h"
#include "X_General.h"

#include <math.h>
#include <QPoint>
#include <QPointF>
#include <QDebug>
//========================================================
X_RulersAndGridManager::X_RulersAndGridManager(X_Plotter *parent) : QObject(parent)
{
    xv_minVpMarkInterval = 0;
    xv_markIntervalList << 1 << 2 << 5 << 10;
    xv_plotView = nullptr;
    xv_rulerWidget = nullptr;
    xv_minimalVerticalScaleInterval = 1.0;
    xv_minimalHorizontalScaleInterval = 1.0;
    xv_minimalScratchVpInterval = 10;
    xv_plotter = parent;
}
//========================================================
//QList<qreal> X_RulersAndGridManager::xp_XMarkGridList() const
//{
//    return xv_XMarkGridMap.values();
//}
////========================================================
//QList<qreal> X_RulersAndGridManager::xp_YMarkGridList() const
//{
//    return xv_YMarkGridMap.values();
//}
////========================================================
//QList<qreal> X_RulersAndGridManager::xp_XScratchGridList() const
//{
//    return xv_XScratchGridMap.values();
//}
////========================================================
//QList<qreal> X_RulersAndGridManager::xp_YScratchGridList() const
//{
//    return xv_YScratchGridMap.values();
//}
//========================================================
void X_RulersAndGridManager::xp_setPlotGraphicsView(X_PlotGraphicsView* plotView)
{
    xv_plotView = plotView;

    xv_plotView->xp_setHorizontalRulePointList(&xv_XRulePointList);
    xv_plotView->xp_setVerticalRulePointList(&xv_YRulePointList);

}
//========================================================
void X_RulersAndGridManager::xp_setRulerWidget(X_RulerWidget* rulerWidget)
{
    xv_rulerWidget = rulerWidget;
    //    xv_rulerWidget->xp_setHorizontalGridMaps(&xv_XScratchGridMap);
    //    xv_rulerWidget->xp_setHorizontalMarkMaps(&xv_XMarkGridMap);
    //    xv_rulerWidget->xp_setVerticalGridMaps(&xv_YScratchGridMap);
    //    xv_rulerWidget->xp_setVerticalMarkMaps(&xv_YMarkGridMap);

    xv_rulerWidget->xp_setHorizontalRulePointList(&xv_XRulePointList);
    xv_rulerWidget->xp_setVerticalRulePointList(&xv_YRulePointList);
}
//========================================================
bool X_RulersAndGridManager::xp_setMinimalVerticalScaleInterval(qreal interval)
{
    if(interval <= 0)
    {
        return false;
    }

    xv_minimalVerticalScaleInterval = interval;
    return true;
}
//========================================================
qreal X_RulersAndGridManager::xp_minimalVerticalScaleInterval()
{
    return xv_minimalVerticalScaleInterval;
}
//========================================================
bool X_RulersAndGridManager::xp_setMinimalHorizontalScaleInterval(qreal interval)
{
    if(interval <= 0)
    {
        return false;
    }

    xv_minimalHorizontalScaleInterval = interval;
    return true;
}
//========================================================
qreal X_RulersAndGridManager::xp_minimalHorizontalScaleInterval()
{
    return xv_minimalHorizontalScaleInterval;
}
//========================================================
void X_RulersAndGridManager::xp_recalcRulesAndGrid()
{
    if(xv_plotView == nullptr)
    {
        xv_XRulePointList.clear();
        xv_YRulePointList.clear();
        return;
    }

    QRect viewportGlobalRect;
    QRectF viewportSceneRect;
    QPoint globalX_eroPoint;
    if(!xv_plotView->xp_viewPortCoordinates(viewportGlobalRect,viewportSceneRect, globalX_eroPoint))
    {
        xv_XRulePointList.clear();
        xv_YRulePointList.clear();
        return;
    }

    qreal distortionFactor;
    qreal distortionCorrectionFactor;
    xv_plotter->xp_verticalDistortionFactors(distortionFactor, distortionCorrectionFactor);

    if(xv_viewportGlobalRect == viewportGlobalRect
            && xv_viewportSceneRect == viewportSceneRect
            && xv_globalX_eroPoint == globalX_eroPoint
            && xv_distortionFactor - distortionFactor == 0.0
            && xv_distortionCorrectionFactor - distortionCorrectionFactor == 0.0)
    {
        // coordinates are not changed
        return;
    }

    xv_distortionFactor = distortionFactor;
    xv_distortionCorrectionFactor = distortionCorrectionFactor;
    xv_viewportGlobalRect = viewportGlobalRect;
    xv_viewportSceneRect = viewportSceneRect;
    xv_globalX_eroPoint = globalX_eroPoint;
    zh_recalcRectsForPaint();

    // bottom rule haven't be distorted
    zh_recalcBottomRule();

    if(distortionFactor == 0.0 || distortionFactor == 1.0 || distortionCorrectionFactor == 0.0)
    {
        zh_recalcLeftRule();
    }
    else
    {
        zh_recalcDistortedLeftRule(distortionFactor, distortionCorrectionFactor);
    }

    xv_rulerWidget->update();
}
//========================================================
double X_RulersAndGridManager::xp_recalcSceneVerticalPos(double scenePosition) const
{
    if(xv_distortionFactor == 0.0 || xv_distortionFactor == 1.0 || xv_distortionCorrectionFactor == 0.0)
    {
        return -scenePosition;
    }
    else
    {
        double posValue = pow((qAbs(scenePosition) / xv_distortionCorrectionFactor), 1/xv_distortionFactor);
        return posValue;
    }
}
//========================================================
void X_RulersAndGridManager::zh_recalcBottomRule()
{
    xv_XRulePointList.clear();

    if(xv_viewportGlobalRect.left() != xv_viewportGlobalRect.right()
            && xv_viewportSceneRect.left() - xv_viewportSceneRect.right() != 0.0
            && xv_rulerWidget != nullptr)
    {
        int maxVpMarkInterval = xv_rulerWidget->xp_maxMarkWidth() * 2 ;
        qreal maxScMarkInterval = xv_plotView->mapToScene(QPoint(maxVpMarkInterval,0)).x()
                - xv_plotView->mapToScene(QPoint(0,0)).x();

        // new mark and scratch interval calculation
        qreal newScMarkInterval = 1;
        qreal newScScratchInterval = 1;

        if(maxScMarkInterval < xv_minimalHorizontalScaleInterval)
        {
            newScMarkInterval = xv_minimalHorizontalScaleInterval;
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

            for(int i = 0; i < xv_markIntervalList.count(); i++)
            {
                if(xv_markIntervalList.value(i) < nIntegralPart)
                {
                    continue;
                }

                integralPart = QString::number(xv_markIntervalList.value(i), 'f', 1);
                maxScMarkIntervalString = integralPart + QString("E") + intervalPartList.value(1);

                newScMarkInterval = maxScMarkIntervalString.toDouble(&ok);
                if(!ok || newScMarkInterval < xv_minimalHorizontalScaleInterval)
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
                    QPoint vp_scratchIntervalPos = xv_plotView->mapFromScene(QPointF(newScScratchInterval, xv_viewportSceneRect.top()));
                    int firstSratchXPos = xv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).x();
                    int newVpScratchInterval = qAbs(firstSratchXPos - xv_globalX_eroPoint.x());
                    if(newVpScratchInterval < xv_minimalScratchVpInterval || newScScratchInterval < xv_minimalHorizontalScaleInterval)
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
                    QPoint vp_scratchIntervalPos = xv_plotView->mapFromScene(QPointF(newScScratchInterval, xv_viewportSceneRect.top()));
                    int firstSratchXPos = xv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).x();
                    int newVpScratchInterval = qAbs(firstSratchXPos - xv_globalX_eroPoint.x());
                    if(newVpScratchInterval < xv_minimalScratchVpInterval || newScScratchInterval < xv_minimalHorizontalScaleInterval)
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
                QPoint vp_scratchIntervalPos = xv_plotView->mapFromScene(QPointF(newScScratchInterval, xv_viewportSceneRect.top()));
                int firstSratchXPos = xv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).x();
                int newVpScratchInterval = qAbs(firstSratchXPos - xv_globalX_eroPoint.x());
                if(newVpScratchInterval < xv_minimalScratchVpInterval || newScScratchInterval < xv_minimalHorizontalScaleInterval)
                {
                    newScScratchInterval = newScMarkInterval;
                }
            }
        }  // end new mark interval calculation


        // vp-coord = sc-coord*k + a
        // solution:
        // k = (vp-max - vp-min) / (sc-max - sc-min)
        // a = vp-min - sc-min*k

        xv_K = ((qreal)xv_viewportGlobalRect.right() - (qreal)xv_viewportGlobalRect.left())
                / (xv_viewportSceneRect.right() - xv_viewportSceneRect.left());
        xv_A = (qreal)xv_viewportGlobalRect.left() - xv_viewportSceneRect.left()*xv_K;


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
        for(qreal sc = newScScratchInterval; sc <= xv_sceneRectForPaint.right(); sc+= newScScratchInterval)
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

            if(sc < xv_sceneRectForPaint.left())
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

            qreal vp = sc*xv_K + xv_A;
            rulePoint = RulePoint(vp, sc, markType);
            xv_XRulePointList.append(rulePoint);
        }

        scratchInMarkIntervalCount = -1;
        scratchInEmptyMarkInterval = -1;
        for(qreal sc = 0; sc >= xv_sceneRectForPaint.left(); sc-= newScScratchInterval)
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

            if(sc > xv_sceneRectForPaint.right())
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

            qreal vp = sc*xv_K + xv_A;
            rulePoint = RulePoint(vp, sc, markType);
            xv_XRulePointList.append(rulePoint);
        }
    }
}
//========================================================
void X_RulersAndGridManager::zh_recalcLeftRule()
{
    // cleaning
    xv_YRulePointList.clear();


    if(xv_viewportGlobalRect.top() != xv_viewportGlobalRect.bottom()
            && xv_viewportSceneRect.top() != xv_viewportSceneRect.bottom()
            && xv_rulerWidget != nullptr)
    {

        xv_minVpMarkInterval = xv_rulerWidget->xp_maxMarkHeight() * 3 ;
        qreal minScMarkInterval = xv_plotView->mapToScene(QPoint(0, xv_minVpMarkInterval)).y()
                - xv_plotView->mapToScene(QPoint(0,0)).y();

        // new mark and scratch interval calculation
        qreal newScMarkInterval = xv_minimalVerticalScaleInterval;
        qreal newScScratchInterval = xv_minimalVerticalScaleInterval;

        if(minScMarkInterval < xv_minimalVerticalScaleInterval)
        {
            newScMarkInterval = xv_minimalVerticalScaleInterval;
        }
        else
        {
            QString maxScMarkIntervalString = QString::number(minScMarkInterval, 'E', 1);
            QStringList intervalPartList = maxScMarkIntervalString.split('E');
            QString integralPart = intervalPartList.value(0, QString());

            bool ok;
            double nIntegralPart = qAbs(integralPart.toDouble(&ok));
            if(!ok)
            {
                newScMarkInterval = minScMarkInterval;
            }

            for(int i = 0; i < xv_markIntervalList.count(); i++)
            {
                if(xv_markIntervalList.value(i) < nIntegralPart)
                {
                    continue;
                }

                integralPart = QString::number(xv_markIntervalList.value(i), 'f', 1);
                maxScMarkIntervalString = integralPart + QString("E") + intervalPartList.value(1);

                newScMarkInterval = maxScMarkIntervalString.toDouble(&ok);
                if(!ok || newScMarkInterval < xv_minimalHorizontalScaleInterval)
                {
                    newScMarkInterval = minScMarkInterval;
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
                    QPoint vp_scratchIntervalPos = xv_plotView->mapFromScene(QPointF(xv_viewportSceneRect.left(), newScScratchInterval));
                    int firstSratchYPos = xv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).y();
                    int newVpScratchInterval = qAbs(firstSratchYPos - xv_globalX_eroPoint.y());
                    if(newVpScratchInterval < xv_minimalScratchVpInterval || newScScratchInterval < xv_minimalVerticalScaleInterval)
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
                    QPoint vp_scratchIntervalPos = xv_plotView->mapFromScene(QPointF(xv_viewportSceneRect.left(), newScScratchInterval));
                    int firstSratchYPos = xv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).y();
                    int newVpScratchInterval = qAbs(firstSratchYPos - xv_globalX_eroPoint.y());
                    if(newVpScratchInterval < xv_minimalScratchVpInterval || newScScratchInterval < xv_minimalVerticalScaleInterval)
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
                QPoint vp_scratchIntervalPos = xv_plotView->mapFromScene(QPointF(xv_viewportSceneRect.left(), newScScratchInterval));
                int firstSratchYPos = xv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).y();
                int newVpScratchInterval = qAbs(firstSratchYPos - xv_globalX_eroPoint.y());
                if(newVpScratchInterval < xv_minimalScratchVpInterval || newScScratchInterval < xv_minimalVerticalScaleInterval)
                {
                    newScScratchInterval = newScMarkInterval;
                }
            }
        }  // end new mark interval calculation

        // vp-coord = sc-coord*k + a
        // solution:
        // k = (vp-max - vp-min) / (sc-max - sc-min)
        // a = vp-min - sc-min*k

        xv_K = ((qreal)xv_viewportGlobalRect.bottom() - (qreal)xv_viewportGlobalRect.top())
                / (xv_viewportSceneRect.bottom() - xv_viewportSceneRect.top());
        xv_A = (qreal)xv_viewportGlobalRect.top() - xv_viewportSceneRect.top()*xv_K;

        // defining empty mark interval
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
        for(qreal sc = newScScratchInterval; sc <= xv_sceneRectForPaint.bottom(); sc+= newScScratchInterval)
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

            if(sc < xv_sceneRectForPaint.top() )
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

            qreal vp = sc*xv_K + xv_A;
            rulePoint = RulePoint(vp, sc, markType);
            xv_YRulePointList.append(rulePoint);
        }

        scratchInMarkIntervalCount = -1;
        scratchInEmptyMarkInterval = -1;
        for(qreal sc = 0; sc >= xv_sceneRectForPaint.top(); sc-= newScScratchInterval)
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

            if(sc > xv_sceneRectForPaint.bottom())
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

            qreal vp = sc*xv_K + xv_A;
            rulePoint = RulePoint(vp, sc, markType);
            xv_YRulePointList.append(rulePoint);
        }
    }
}
//========================================================
void X_RulersAndGridManager::zh_recalcDistortedLeftRule(qreal distortionFactor, qreal distortionCorrectionFactor)
{
    xv_YRulePointList.clear();

    xv_distortionFactor = distortionFactor;
    xv_distortionCorrectionFactor = distortionCorrectionFactor;

    if(xv_viewportGlobalRect.top() != xv_viewportGlobalRect.bottom()
            && xv_viewportSceneRect.top() != xv_viewportSceneRect.bottom()
            && xv_rulerWidget != 0)
    {
        xv_minVpMarkInterval = xv_rulerWidget->xp_maxMarkHeight() * 3 ;

        // vp-coord = sc-coord*k + a
        // sc-coord = (vp-coord - a) / k
        // solution:
        // k = (vp-max - vp-min) / (sc-max - sc-min)
        // a = vp-min - sc-min*k

        // sc-coord = value^distortionFactor * distortionCorrectionFactor
        //

        xv_K = ((qreal)xv_viewportGlobalRect.bottom() - (qreal)xv_viewportGlobalRect.top())
                / (xv_viewportSceneRect.bottom() - xv_viewportSceneRect.top());
        xv_A = (qreal)xv_viewportGlobalRect.top() - xv_viewportSceneRect.top()*xv_K;

        RulePoint rulePoint;
        RulePoint::MarkType markType;


        qreal currentMarkValue = 0;
        qreal currentMarkScPos = 0;
        qreal currentMarkVpPos = xv_A;

        //
        xv_displayedTopYValue = pow((qAbs(xv_viewportSceneRect.top()) / xv_distortionCorrectionFactor), 1/xv_distortionFactor);
        if(xv_viewportSceneRect.top() < 0)
        {
            xv_displayedTopYValue *= -1;
        }
        xv_displayedBottomYValue = pow(qAbs((xv_viewportSceneRect.bottom()) / xv_distortionCorrectionFactor), 1/xv_distortionFactor);
        if(xv_viewportSceneRect.bottom() < 0)
        {
            xv_displayedBottomYValue *= -1;
        }

        qreal prevMarkValue;
        while(currentMarkValue >= xv_displayedTopYValue)
        {
            if(currentMarkValue <= xv_displayedBottomYValue)
            {
                markType = RulePoint::MT_MARK;
                //qreal vp = *k + a;
                rulePoint = RulePoint(currentMarkVpPos, currentMarkScPos, currentMarkValue, markType);
                xv_YRulePointList.append(rulePoint);
            }

            prevMarkValue = currentMarkValue;

            // next min passible current value
            currentMarkVpPos = currentMarkVpPos - xv_minVpMarkInterval;
            currentMarkScPos = (currentMarkVpPos - xv_A) / xv_K;
            currentMarkValue = pow((qAbs(currentMarkScPos) / xv_distortionCorrectionFactor), 1/xv_distortionFactor);
            // next to the min passible value tenfold value
            qreal p = 0.0;
            for(;;p+=1.0 )
            {
                if(currentMarkValue <= pow(10.0, p))
                {
                    currentMarkValue = pow(10.0, p);
                    break;
                }
            }

            if(currentMarkScPos < 0)
            {
                currentMarkValue *= -1;
            }

            // back recalculation
            currentMarkScPos = pow(qAbs(currentMarkValue), xv_distortionFactor) * xv_distortionCorrectionFactor;
            if(currentMarkValue < 0)
            {
                currentMarkScPos *= -1;
            }
            currentMarkVpPos = currentMarkScPos * xv_K + xv_A;

            // sratches defining (they may be marks if interval will allow)

            zh_divideDistortedVerticalInterval(p, currentMarkValue, currentMarkVpPos, prevMarkValue);
        }


        // from zero to bottom
        currentMarkValue = xv_minVpMarkInterval;
        currentMarkScPos = pow(qAbs(currentMarkValue), xv_distortionFactor) * xv_distortionCorrectionFactor;
        currentMarkVpPos = currentMarkScPos * xv_K + xv_A;


        while(currentMarkValue <= xv_displayedBottomYValue)
        {
            if(currentMarkValue >= xv_displayedTopYValue)
            {
                markType = RulePoint::MT_MARK;
                //qreal vp = *k + a;
                rulePoint = RulePoint(currentMarkVpPos, currentMarkScPos, currentMarkValue, markType);
                xv_YRulePointList.append(rulePoint);
            }

            prevMarkValue = currentMarkValue;

            // next min passible current value
            currentMarkVpPos = currentMarkVpPos + xv_minVpMarkInterval;
            currentMarkScPos = (currentMarkVpPos - xv_A) / xv_K;
            currentMarkValue = pow((qAbs(currentMarkScPos) / xv_distortionCorrectionFactor), 1/xv_distortionFactor);
            // next to the min passible value tenfold value
            qreal p = 0.0;
            for(;;p+=1.0 )
            {
                if(currentMarkValue <= pow(10.0, p))
                {
                    currentMarkValue = pow(10.0, p);
                    break;
                }
            }

//            if(currentMarkScPos < 0)
//            {
//                currentMarkValue *= -1;
//            }

            // back recalculation
            currentMarkScPos = pow(qAbs(currentMarkValue), xv_distortionFactor) * xv_distortionCorrectionFactor;
//            if(currentMarkValue < 0)
//            {
//                currentMarkScPos *= -1;
//            }
            currentMarkVpPos = currentMarkScPos * xv_K + xv_A;

            // sratches defining (they may be marks if interval will allow)

            zh_divideDistortedVerticalInterval(p, currentMarkValue, currentMarkVpPos, prevMarkValue);
        }
    }
}
//========================================================
bool X_RulersAndGridManager::zh_divideDistortedVerticalInterval(qreal p, qreal currentMarkValue,
                                                               qreal currentMarkVpPos, qreal prevMarkValue)
{
    qreal scratchValue;
    qreal scratchScPos;
    qreal scratchVpPos;
    RulePoint rulePoint;
    RulePoint::MarkType markType;

    //    if(qAbs(currentMarkValue) <= pow(10, p-1.0))
    //    {
    //        continue;
    //    }
    // top scratch value
    scratchValue = qAbs(currentMarkValue) - pow(10, p-1.0);
    if(currentMarkValue < 0)
    {
        scratchValue *= -1;
    }
    // top scratch coords
    scratchScPos = pow(qAbs(scratchValue), xv_distortionFactor) * xv_distortionCorrectionFactor;
    if(scratchValue < 0)
    {
        scratchScPos *= -1;
    }
    scratchVpPos = scratchScPos * xv_K + xv_A;

    // if vp interval is big scratches become marks
    bool scratchIsMark = false;
    bool divideCurrentInterval = false;
    qreal prevScratchValue;
    qreal prevScratchVpPos;

    if(qAbs(scratchVpPos - currentMarkVpPos) >= xv_minVpMarkInterval)
    {
        scratchIsMark = true;
    }
    else
    {
        scratchIsMark = false;
    }

    if(qAbs(scratchVpPos - currentMarkVpPos) >= xv_minimalScratchVpInterval && scratchIsMark)
    {
        divideCurrentInterval = zh_divideDistortedVerticalInterval(p - 1, currentMarkValue,
                                                                   currentMarkVpPos, scratchValue);
    }

    if(qAbs(scratchVpPos - currentMarkVpPos) >= xv_minimalScratchVpInterval
            && qAbs(scratchValue - currentMarkValue) >= xv_minimalVerticalScaleInterval)
    {
        int scratchCount = 0;
        while(qAbs(scratchValue) > qAbs(prevMarkValue))
        {
            if(scratchValue >=  xv_displayedTopYValue && scratchValue <= xv_displayedBottomYValue)
            {
                if(scratchIsMark)
                {
                    markType = RulePoint::MT_MARK;
                }
                else if(scratchCount == 4)
                {
                    markType = RulePoint::MT_MARK;
                }
                else
                {
                    markType = RulePoint::MT_SCRATCH;
                }

                //qreal vp = *k + a;
                rulePoint = RulePoint(scratchVpPos, scratchScPos, scratchValue, markType);
                xv_YRulePointList.append(rulePoint);
            }

            // next scratch to below
            prevScratchValue = scratchValue;
            prevScratchVpPos = scratchVpPos;

            scratchValue = qAbs(scratchValue) - pow(10, p-1.0);
            if(prevScratchValue < 0)
            {
                scratchValue *= -1;
            }
            //scratchValue = scratchValue + pow(10, p-1.0);
            scratchScPos = pow(qAbs(scratchValue), xv_distortionFactor) * xv_distortionCorrectionFactor;
            if(scratchValue < 0)
            {
                scratchScPos *= -1;
            }
            scratchVpPos = scratchScPos * xv_K + xv_A;

            if(divideCurrentInterval && scratchIsMark)
            {
                zh_divideDistortedVerticalInterval(p - 1, prevScratchValue,
                                                   prevScratchVpPos, scratchValue);
            }
            scratchCount++;
        }
        return true;
    }
    else
    {
        // trying to divide to half
        scratchValue = qAbs(currentMarkValue) - (pow(10, p-1.0) * 5);
        if(currentMarkValue < 0)
        {
            scratchValue *= -1;
        }
        // top scratch coords
        scratchScPos = pow(qAbs(scratchValue), xv_distortionFactor) * xv_distortionCorrectionFactor;
        if(scratchValue < 0)
        {
            scratchScPos *= -1;
        }
        scratchVpPos = scratchScPos * xv_K + xv_A;

        bool scratchIsMark;
        if(qAbs(scratchVpPos - currentMarkVpPos) >= xv_minVpMarkInterval)
        {
            scratchIsMark = true;
        }
        else
        {
            scratchIsMark = false;
        }

        if(qAbs(scratchVpPos - currentMarkVpPos) >= xv_minimalScratchVpInterval
                && qAbs(scratchValue - currentMarkValue) >= xv_minimalVerticalScaleInterval)
        {
            if(scratchValue >=  xv_displayedTopYValue && scratchValue <= xv_displayedBottomYValue)
            {
                if(scratchIsMark)
                {
                    markType = RulePoint::MT_MARK;
                }
                else
                {
                    markType = RulePoint::MT_EMPTY_MARK;
                }

                //qreal vp = *k + a;
                rulePoint = RulePoint(scratchVpPos, scratchScPos, scratchValue, markType);
                xv_YRulePointList.append(rulePoint);
            }
            return true;
        }
    }
    return false;
}
//========================================================
void X_RulersAndGridManager::zh_recalcRectsForPaint()
{
    qreal K_width = xv_viewportSceneRect.width() /
            (qreal)xv_viewportGlobalRect.width();
    qreal K_height = xv_viewportSceneRect.height() /
            (qreal)xv_viewportGlobalRect.height();

    int dPix = 1;
    xv_globalRectForPaint = xv_viewportGlobalRect.adjusted(-1*dPix, -1*dPix, dPix, dPix);
    xv_sceneRectForPaint = xv_viewportSceneRect.adjusted(-1*K_width*dPix, -1*K_height*dPix, K_width*dPix, K_height*dPix);
}
//========================================================

