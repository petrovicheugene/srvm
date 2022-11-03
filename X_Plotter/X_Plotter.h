//====================================================
#ifndef X_PLOTTER_H
#define X_PLOTTER_H
//====================================================
#include <QWidget>
#include <QGraphicsItem>
#include "X_HorizontalDashBoard.h"
//====================================================
class X_MarkLine;
class X_VerticalMarkLine;
class X_ScratchLine;
class X_VerticalScratchLine;
class X_PlotGraphicsView;
class X_PlotGraphicsScene;
class QScrollBar;
class X_RulersAndGridManager;
class X_RulerWidget;
//====================================================
class X_Plotter : public QWidget
{
   Q_OBJECT
public:
   explicit X_Plotter(QWidget *parent = nullptr);

   // X_PlotGraphicsScene* xp_plotScene();

   bool xp_setMinimalVerticalScaleInterval(qreal);
   qreal xp_minimalVerticalScaleInterval();
   bool xp_setMinimalHorizontalScaleInterval(qreal);
   qreal xp_minimalHorizontalScaleInterval();

   // rule control
   bool xp_setLeftMarkFontSize(int size) ;
   bool xp_setRightMarkFontSize(int size);
   bool xp_setTopMarkFontSize(int size);
   bool xp_setBottomMarkFontSize(int size);

   int xp_leftMarkFontSize();
   int xp_rightMarkFontSize();
   int xp_topMarkFontSize();
   int xp_bottomMarkFontSize();

   bool xp_setLeftMarkFormat(char);
   bool xp_setRightMarkFormat(char);
   bool xp_setTopMarkFormat(char);
   bool xp_setBottomMarkFormat(char);

   bool xp_setLeftMarkPrecision(int);
   bool xp_setRightMarkPrecision(int);
   bool xp_setTopMarkPrecision(int);
   bool xp_setBottomMarkPrecision(int);

   // rule control
   void xp_setHorizontalMarkRecalcFactors(const QString &label, qreal K0, qreal K1, qreal K2);
   void xp_setVerticalMarkRecalcFactors(const QString &label, qreal K0, qreal K1, qreal K2);

   void xp_setLeftMarkRecalcMetrix(const QString&label, bool labelVisibility, qreal K0, qreal K1, qreal K2);
   void xp_setRightMarkRecalcMetrix(const QString&label, bool labelVisibility, qreal K0, qreal K1, qreal K2);
   void xp_setTopMarkRecalcMetrix(const QString&label, bool labelVisibility, qreal K0, qreal K1, qreal K2);
   void xp_setBottomMarkRecalcMetrix(const QString& label, bool labelVisibility,qreal K0, qreal K1, qreal K2);

   void xp_setLeftMarkRecalcFlag(bool flag);
   void xp_setRightMarkRecalcFlag(bool flag);
   void xp_setTopMarkRecalcFlag(bool flag);
   void xp_setBottomMarkRecalcFlag(bool flag);

   void xp_setTopRuleLabel(const QString &label);
   void xp_setBottomRuleLabel(const QString &label);
   void xp_setLeftRuleLabel(const QString &label);
   void xp_setRightRuleLabel(const QString &label);

   // ***
   void xp_clearItemsForType(int type);
   void xp_clearItemsExeptType(int type);
   void xp_addItem(QGraphicsItem * item);
   void xp_removeItem(QGraphicsItem * item);
   void xp_removeItemsForType(int type);
   QList<QGraphicsItem*> xp_itemListForType(int type) const;
   int xp_itemCount() const;

   void xp_appendButtonsToDashboard(QList<QAction*>&,
                                    X_HorizontalDashBoard::AuxWidgetPositon position,
                                    Qt::AlignmentFlag align,
                                    int extraSperatorSpacing = 0);
   void xp_appendWidgetToDashboard(QWidget* widget,
                                   X_HorizontalDashBoard::AuxWidgetPositon position,
                                   Qt::AlignmentFlag alignment,
                                   int extraSeparatorSpacing = 0);

   void xp_setVerticalAbsMax(qreal);
   qreal xp_verticalAbsMax() const;
   void xp_setAutoDefineVerticalAbsMax(bool);
   void xp_verticalDistortionFactors(qreal& distortionFactor, qreal& distortionCorrectionFactor);

   bool xp_isPlotScaled();
   void xp_updatePlot();

   QBrush xp_backgroundBrush();

   void xp_setContextMenu(QList<QAction *> &actionList);

   void xp_setEnergyCalibration(QList<double> energyCalibrationFactorList);

   QRectF xp_viewportSceneRect() const;
   QSize xp_viewportPixelSize() const;

   void xp_setInfoLabelVisible(bool visible);

signals:

   void xg_cursorAreaImage(QImage);
   void xg_mousePressedAt(QPointF mousePos) const;
   void xg_viewportRectChanged(QRectF) const;
   void xg_rulerToolChanged(QPointF startPoint, QPointF endPoint, bool visibility) const;

public slots:

   void xp_setLeftRuleVisible(bool visible);
   void xp_setRightRuleVisible(bool visible);
   void xp_setTopRuleVisible(bool visible);
   void xp_setBottomRuleVisible(bool visible);

   void xp_setTopRuleLabelVisible(bool visible);
   void xp_setBottomRuleLabelVisible(bool visible);
   void xp_setLeftRuleLabelVisible(bool visible);
   void xp_setRightRuleLabelVisible(bool visible);

   void xp_setDistortionSliderVisible(bool visible);

   void xp_currentVisibleSceneRect(QRectF& sceneRect);
   void xp_fitInBoundingRect();
   QRectF xp_boundingRect() const;

protected:

   // FUNCS
   void resizeEvent(QResizeEvent* event);
   bool eventFilter(QObject *obj, QEvent *event);

private slots:

   void xh_verticalDistortionChanged(int);
   void xh_scrollBarVisible(Qt::Orientation, bool&);

   void xh_mouseScenePositionChanged(QPointF scenePos) const;
   void xh_mouseLeaved() const;

private:

   // VARS
   X_RulersAndGridManager* xv_rulersAndGreedManager;

   X_RulerWidget* xv_rulerWidget;
   X_PlotGraphicsView* xv_plotView;
   X_PlotGraphicsScene* xv_plotScene;
   X_HorizontalDashBoard* xv_dashBoard;

   qreal xv_verticalAbsMax;
   bool xv_autoDefineVerticalAbsMax;
   qreal xv_verticalDistortionFactor;
   qreal xv_verticalDistortionCorrectionFactor;
   QList<double> xv_energyCalibrationFactorList;

   // recalc when max or log base are changed
   //
   //
   // xv_verticalDistortionCorrectionFactor =  max / max^xv_verticalDistortionFactor
   //
   //

   //  Y_log = Y^xv_verticalDistortionFactor * xv_verticalDistortionCorrectionFactor


   // scrollbar
   QScrollBar* xv_verticalScrollBar;
   QScrollBar* xv_horizontalScrollBar;
   bool xv_mouseButtonDown;
   bool xv_userResizesWidget;

   // FUNCS
   void xh_createComponents();
   void xh_createConnections();
   void xh_connectScrollBars();
   void xh_updateScrollBarsVisible();

   void xh_scrollBarVisibleControl(int min , int max);
   void xh_notifySceneRect(int value);

   bool xh_recalcVerticalDistortionFactors(qreal distortionValue);
   void xh_recalcRulesAndItemCoordinates();

};
//====================================================
#endif // X_PLOTTER_H
