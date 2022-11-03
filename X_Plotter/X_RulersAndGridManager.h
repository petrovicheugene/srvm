//========================================================
#ifndef X_RULERSANDGREEDMANAGER_H
#define X_RULERSANDGREEDMANAGER_H
//========================================================
#include <QObject>
#include <QPair>
#include <QMap>
#include <QRect>
#include <QRectF>
#include <QList>
//========================================================
class X_PlotGraphicsView;
class X_RulerWidget;
class X_Plotter;
//========================================================
class RulePoint
{
public:
    enum MarkType{MT_SCRATCH, MT_MARK, MT_EMPTY_MARK};

    RulePoint()
    {
        globalPos = 0.0;
        scenePos = 0.0;
        markValue = 0.0;
        markType = MT_SCRATCH;
    }

    RulePoint(qreal glPos, qreal scPos, RulePoint::MarkType type)
    {
        globalPos = glPos;
        scenePos = scPos;
        markValue = scPos;
        markType = type;
    }


    RulePoint(qreal glPos, qreal scPos,  qreal markVal, RulePoint::MarkType type)
    {
        globalPos = glPos;
        scenePos = scPos;
        markValue = markVal;
        markType = type;
    }

    // VARS
    MarkType markType;
    qreal globalPos;
    qreal scenePos;
    qreal markValue;
};

//========================================================
class X_RulersAndGridManager : public QObject
{
    Q_OBJECT
public:
    explicit X_RulersAndGridManager(X_Plotter *parent);

    void xp_setPlotGraphicsView(X_PlotGraphicsView* plotView);
    void xp_setRulerWidget(X_RulerWidget* rulerWidget);

    bool xp_setMinimalVerticalScaleInterval(qreal interval);
    qreal xp_minimalVerticalScaleInterval();
    bool xp_setMinimalHorizontalScaleInterval(qreal interval);
    qreal xp_minimalHorizontalScaleInterval();

    void xp_recalcRulesAndGrid();

    double xp_recalcSceneVerticalPos(double scenePosition) const;

signals:

public slots:


private:

    // VARS
    X_PlotGraphicsView* xv_plotView;
    X_RulerWidget* xv_rulerWidget;
    X_Plotter* xv_plotter;

    QList<RulePoint> xv_XRulePointList;
    QList<RulePoint> xv_YRulePointList;


    qreal xv_minimalVerticalScaleInterval;
    qreal xv_minimalHorizontalScaleInterval;
    int xv_minimalScratchVpInterval;


    // for recalc only
    int xv_minVpMarkInterval;
    qreal xv_K;
    qreal xv_A;
    qreal xv_displayedTopYValue;
    qreal xv_displayedBottomYValue;
    qreal xv_distortionFactor;
    qreal xv_distortionCorrectionFactor;


    QRect xv_viewportGlobalRect;
    QRectF xv_viewportSceneRect;

    QRect xv_globalRectForPaint; // +1 pix more than xv_viewportGlobalRect
    QRectF xv_sceneRectForPaint; // +1 pix more than xv_viewportSceneRect

    QPoint xv_globalX_eroPoint;
    QList<qreal> xv_markIntervalList;

    // FUNCS
    void zh_recalcBottomRule();
    void zh_recalcLeftRule();
    void zh_recalcDistortedLeftRule(qreal xv_distortionFactor,
                                    qreal xv_distortionCorrectionFactor);

    void zh_recalcRectsForPaint();
    bool zh_divideDistortedVerticalInterval(qreal p, qreal currentMarkValue,
                           qreal currentMarkVpPos, qreal prevMarkValue);
};
//========================================================
#endif // X_RULERSANDGREEDMANAGER_H
