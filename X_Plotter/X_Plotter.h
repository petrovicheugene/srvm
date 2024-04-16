//====================================================
#ifndef X_PLOTTER_H
#define X_PLOTTER_H
//====================================================
#include <QWidget>
#include <QGraphicsItem>
#include "X_HorizontalDashBoard.h"
//====================================================
class QLabel;
class QScrollBar;

class X_MarkLine;
class X_VerticalMarkLine;
class X_ScratchLine;
class X_VerticalScratchLine;
class X_PlotGraphicsView;
class X_PlotGraphicsScene;
class X_RulersAndGridManager;
class X_RulerWidget;
//====================================================
class X_Plotter : public QWidget
{
    Q_OBJECT
public:
    explicit X_Plotter(const QString &objectName, QWidget *parent = nullptr);
    ~X_Plotter();

    bool xp_setMinimalVerticalScaleInterval(double);
    double xp_minimalVerticalScaleInterval();
    bool xp_setMinimalHorizontalScaleInterval(double);
    double xp_minimalHorizontalScaleInterval();

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
    void xp_setHorizontalMarkRecalcFactors(const QString &label, double K0, double K1, double K2);
    void xp_setVerticalMarkRecalcFactors(const QString &label, double K0, double K1, double K2);

    void xp_setLeftMarkRecalcMetrix(const QString&label, bool labelVisibility, double K0, double K1, double K2);
    void xp_setRightMarkRecalcMetrix(const QString&label, bool labelVisibility, double K0, double K1, double K2);
    void xp_setTopMarkRecalcMetrix(const QString&label, bool labelVisibility, double K0, double K1, double K2);
    void xp_setBottomMarkRecalcMetrix(const QString& label, bool labelVisibility,double K0, double K1, double K2);

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

    void xp_setVerticalAbsMax(double);
    double xp_verticalAbsMax() const;
    void xp_setAutoDefineVerticalAbsMax(bool);
    void xp_verticalDistortionFactors(double& distortionFactor, double& distortionCorrectionFactor);

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
    void xs_chartPointItemSizeChanged(double relativeSizingFactor);

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
    void xh_onMousePress(QPointF mouseScenePos);

    void xh_mouseScenePositionChanged(QPointF scenePos) const;
    void xh_mouseLeaved() const;
    void xh_updateGraphicsItemsSizes(QPoint wheelPoint);

    void xh_notifySizingFactor();

private:

    // VARS
    X_RulersAndGridManager* xv_rulersAndGreedManager;

    X_RulerWidget* xv_rulerWidget;
    X_PlotGraphicsView* xv_plotView;
    X_PlotGraphicsScene* xv_plotScene;
    X_HorizontalDashBoard* xv_dashBoard;

    double xv_verticalAbsMax;
    bool xv_autoDefineVerticalAbsMax;
    double xv_verticalDistortionFactor;
    double xv_verticalDistortionCorrectionFactor;

    QList<double> xv_energyCalibrationFactorList;

    // scrollbar
    QScrollBar* xv_verticalScrollBar;
    QScrollBar* xv_horizontalScrollBar;
    bool xv_mouseButtonDown;
    bool xv_userResizesWidget;
    double xv_relativeSizingFactor = 1.0;

    // FUNCS
    void xh_createComponents();
    void xh_createConnections();
    void xh_restoreSettings(const QString& parentName);
    void xh_saveSettings(const QString & parentName);

    void xh_connectScrollBars();
    void xh_updateScrollBarsVisible();

    void xh_scrollBarVisibleControl(int min , int max);
    void xh_notifySceneRect(int value);

    bool xh_recalcVerticalDistortionFactors(double distortionValue);
    void xh_recalcRulesAndItemCoordinates();
    void xh_updateItemSize(QGraphicsItem * item);


};
//====================================================
#endif // X_PLOTTER_H
