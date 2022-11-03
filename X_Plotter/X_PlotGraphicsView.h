//=============================================================
#ifndef X_GRAPHICSVIEWPLOT_H
#define X_GRAPHICSVIEWPLOT_H
//=============================================================
#include <QGraphicsView>
#include "X_RulersAndGridManager.h"
//=============================================================
class QWheelEvent;
class QRubberBand;
class X_HorizontalDashBoard;
//=============================================================
class X_PlotGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit X_PlotGraphicsView(QWidget *parent = nullptr);

    // FUNC
    QRectF xp_currentVisibleSceneRect() const;

    void xp_setScaleViewWhenResizeFlag(bool);
    void xp_setViewport(QWidget * widget);
    void xp_connectToRulersAndGridManager(X_RulersAndGridManager*);

    void xp_setHorizontalRulePointList(QList<RulePoint> *);
    void xp_setVerticalRulePointList(QList<RulePoint> *);
    bool xp_viewPortCoordinates(QRect&globalRect, QRectF&sceneRect, QPoint &nullGlobalPoint);
    bool xp_viewPortSceneRect(QRectF&sceneRect);

    void xp_scale(Qt::Orientation orientation, bool increase);
    void xp_setGridHidden(bool);

    bool xp_setColorPickUpAuxCoverageSize(int);
    int xp_colorPickUpAuxCoverageSize() const;

    bool xp_setRubberBandSideMinSize(int);
    int xp_rubberBandSideMinSize() const;

    QColor xp_gridColor() const;

    void xp_fitInView(const QRectF &rect, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio);

    void xp_update();

signals:

    void xg_mouseScenePositionChanged(QPointF scenePos) const;
    void xg_mouseLeaved() const;

   void xg_requestForScrollBarVisible(Qt::Orientation, bool&);
   void xg_cursorAreaImage(QImage);
   void xg_viewportRectChanged(QRectF) const;
   void xg_rulerToolChanged(QPointF startPoint, QPointF endPoint, bool visibility) const;


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

    void fitInView(const QRectF &rect, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio);
    void fitInView(qreal x, qreal y, qreal w, qreal h, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio);
    void fitInView(const QGraphicsItem *item, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio);

private:

    // VARS
    enum PlotMode{PM_REGULAR,
                  PM_PAD_DRAGGING,
                  PM_RUBBER_BAND,
                  PM_RULER};


    X_RulersAndGridManager* xv_rulersAndGreedManager;
    PlotMode xv_plotMode;
    QPointF xv_sceneCenterPos;
    QPointF xv_sceneMousePos;
    QPoint xv_mousePressStartViewPos;
    QPoint xv_currentMousePos;
    QRubberBand* xv_rubberBand;
    bool xv_scaleViewWhenResizeFlag;
    bool xv_drawGridFlag;

    qreal xv_minSideSizeOfVisibleScene;
    int xv_rubberBandSideMinSize;
    int xv_ruleToolMinSize;

    QList<RulePoint> * xv_XRuleList;
    QList<RulePoint> * xv_YRuleList;

    int xv_colorPickUpAuxCoverageSize;
    QColor xv_gridColor;
    // FUNCS
    void zh_createConnections();
    QImage zh_grabCursorArea(QPoint);
};
//=============================================================
#endif // X_GRAPHICSVIEWPLOT_H
