//========================================================
#ifndef ZRULERSANDGREEDMANAGER_H
#define ZRULERSANDGREEDMANAGER_H
//========================================================
#include <QObject>
#include <QPair>
#include <QMap>
#include <QRect>
#include <QRectF>
#include <QList>
//========================================================
class ZPlotGraphicsView;
class ZRulerWidget;
//========================================================
class RulePoint
{
public:
    enum MarkType{MT_SCRATCH, MT_MARK, MT_EMPTY_MARK};

    RulePoint()
    {
        globalPos = 0;
        scenePos = 0;
        markType = MT_SCRATCH;
    }

    RulePoint(qreal glPos, qreal scPos, RulePoint::MarkType type)
    {
        globalPos = glPos;
        scenePos = scPos;
        markType = type;
    }

    // VARS
    MarkType markType;
    qreal globalPos;
    qreal scenePos;
};

//========================================================
class ZRulersAndGridManager : public QObject
{
    Q_OBJECT
public:
    explicit ZRulersAndGridManager(QObject *parent = 0);

    void zp_setPlotGraphicsView(ZPlotGraphicsView* plotView);
    void zp_setRulerWidget(ZRulerWidget* rulerWidget);

    bool zp_setMinimalVerticalScaleInterval(qreal interval);
    qreal zp_minimalVerticalScaleInterval();
    bool zp_setMinimalHorizontalScaleInterval(qreal interval);
    qreal zp_minimalHorizontalScaleInterval();

    void zp_recalcRulesAndGrid();

signals:

public slots:


private:

    // VARS
    ZPlotGraphicsView* zv_plotView;
    ZRulerWidget* zv_rulerWidget;

    QList<RulePoint> zv_XRulePointList;
    QList<RulePoint> zv_YRulePointList;


    qreal zv_minimalVerticalScaleInterval;
    qreal zv_minimalHorizontalScaleInterval;

    QRect zv_viewportGlobalRect;
    QRectF zv_viewportSceneRect;

    QRect zv_globalRectForPaint; // +1 pix more than zv_viewportGlobalRect
    QRectF zv_sceneRectForPaint; // +1 pix more than zv_viewportSceneRect

    QPoint zv_globalZeroPoint;
    QList<qreal> zv_markIntervalList;

    // FUNCS
    void zh_recalcBottomRule();
    void zh_recalcLeftRule();


    void zh_recalcRectsForPaint();

};
//========================================================
#endif // ZRULERSANDGREEDMANAGER_H
