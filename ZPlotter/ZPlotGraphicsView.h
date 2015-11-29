//=============================================================
#ifndef ZGRAPHICSVIEWPLOT_H
#define ZGRAPHICSVIEWPLOT_H
//=============================================================
#include <QGraphicsView>
#include "ZRulersAndGridManager.h"

//=============================================================
class QWheelEvent;
class QRubberBand;
class ZDashBoard;
//=============================================================
class ZPlotGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ZPlotGraphicsView(QWidget *parent = 0);

    // FUNC
    QRectF zp_currentVisibleSceneRect() const;

    void zp_setScaleViewWhenResizeFlag(bool);
    void zp_setViewport(QWidget * widget);
    void zp_connectToRulersAndGridManager(ZRulersAndGridManager*);

    void zp_setHorizontalRulePointList(QList<RulePoint> *);
    void zp_setVerticalRulePointList(QList<RulePoint> *);
    bool zp_viewPortCoordinates(QRect&globalRect, QRectF&sceneRect, QPoint &nullGlobalPoint);

    void zp_scale(Qt::Orientation orientation, bool increase);
    void zp_setGridHidden(bool);

signals:


protected:

    // FUNCS
    // reimplemented
    void wheelEvent(QWheelEvent * event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    bool viewportEvent(QEvent* event);
    void drawBackground(QPainter * painter, const QRectF & rect);

    // just hidden
    void	setViewport(QWidget * widget);
private:

    // VARS
    enum PlotMode{PM_REGULAR,
                  PM_PAD_DRAGGING,
                 PM_RUBBER_BAND};

    ZRulersAndGridManager* zv_rulersAndGreedManager;
    PlotMode zv_plotMode;
    QPointF zv_sceneCenterPos;
    QPointF zv_sceneMousePos;
    QPoint zv_mousePressStartViewPos;
    QRubberBand* zv_rubberBand;
    bool zv_scaleViewWhenResizeFlag;
    bool zv_drawGridFlag;

    qreal zv_minSideSizeOfVisibleScene;
    QList<RulePoint> * zv_XRuleList;
    QList<RulePoint> * zv_YRuleList;

    // FUNCS
    void zh_createConnections();

};
//=============================================================
#endif // ZGRAPHICSVIEWPLOT_H