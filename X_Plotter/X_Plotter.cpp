//====================================================
#include "X_Plotter.h"
#include "X_PlotGraphicsScene.h"
#include "X_PlotGraphicsView.h"
#include "X_RulersAndGridManager.h"
#include "X_RulerWidget.h"
#include "X_EnergyLineGraphicsItem.h"
#include "X_SpectrumGraphicsItem.h"

#include <QApplication>
#include <QLabel>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollBar>
#include <QDebug>
#include <math.h>
//====================================================
extern const double gl_defaultSpectrumZValue = 2;
extern const double gl_currentSpectrumZValue = 3;
extern const double gl_defaultWindowZValue = 1;
extern const double gl_currentWindowZValue = 4;

//====================================================
X_Plotter::X_Plotter(QWidget *parent) : QWidget(parent)
{
    xv_rulerWidget = nullptr;
    xv_plotView = nullptr;
    xv_verticalScrollBar = nullptr;
    xv_horizontalScrollBar = nullptr;
    xv_plotScene = nullptr;
    xv_rulersAndGreedManager = nullptr;
    xv_dashBoard = nullptr;

    xv_verticalAbsMax = 0;
    xv_autoDefineVerticalAbsMax = true;
    xv_verticalDistortionFactor = 0;
    xv_verticalDistortionCorrectionFactor = 0;

    xv_mouseButtonDown = false;
    xv_userResizesWidget = false;
    xh_createComponents();
    xh_createConnections();
    qApp->installEventFilter(this);
}
//====================================================
//X_PlotGraphicsScene* X_Plotter::xp_plotScene()
//{
//    return xv_plotScene;
//}
//====================================================
X_Plotter::~X_Plotter()
{
    delete xv_plotScene;
}
//====================================================
bool X_Plotter::xp_setMinimalVerticalScaleInterval(double interval)
{
    if(!xv_rulersAndGreedManager)
    {
        return false;
    }
    return xv_rulersAndGreedManager->xp_setMinimalVerticalScaleInterval(interval);
}
//====================================================
double X_Plotter::xp_minimalVerticalScaleInterval()
{
    if(!xv_rulersAndGreedManager)
    {
        return -1.0;
    }
    return xv_rulersAndGreedManager->xp_minimalVerticalScaleInterval();
}
//====================================================
bool X_Plotter::xp_setMinimalHorizontalScaleInterval(double interval)
{
    if(!xv_rulersAndGreedManager)
    {
        return false;
    }
    return xv_rulersAndGreedManager->xp_setMinimalHorizontalScaleInterval(interval);
}
//====================================================
double X_Plotter::xp_minimalHorizontalScaleInterval()
{
    if(!xv_rulersAndGreedManager)
    {
        return -1.0;
    }
    return xv_rulersAndGreedManager->xp_minimalHorizontalScaleInterval();
}
//====================================================
bool X_Plotter::xp_setLeftMarkFontSize(int size)
{
    if(!xv_rulerWidget)
    {
        return false;
    }
    return xv_rulerWidget->xp_setLeftFontSize(size);
}
//====================================================
bool X_Plotter::xp_setRightMarkFontSize(int size)
{
    if(!xv_rulerWidget)
    {
        return false;
    }
    return xv_rulerWidget->xp_setRightFontSize(size);
}
//====================================================
bool X_Plotter::xp_setTopMarkFontSize(int size)
{
    if(!xv_rulerWidget)
    {
        return false;
    }
    return xv_rulerWidget->xp_setTopFontSize(size);
}
//====================================================
bool X_Plotter::xp_setBottomMarkFontSize(int size)
{
    if(!xv_rulerWidget)
    {
        return false;
    }
    return xv_rulerWidget->xp_setBottomFontSize(size);
}
//====================================================
int X_Plotter::xp_leftMarkFontSize()
{
    if(!xv_rulerWidget)
    {
        return -1;
    }
    return xv_rulerWidget->xp_leftFontSize();
}
//====================================================
int X_Plotter::xp_rightMarkFontSize()
{
    if(!xv_rulerWidget)
    {
        return -1;
    }
    return xv_rulerWidget->xp_rightFontSize();
}
//====================================================
int X_Plotter::xp_topMarkFontSize()
{
    if(!xv_rulerWidget)
    {
        return -1;
    }
    return xv_rulerWidget->xp_topFontSize();
}
//====================================================
int X_Plotter::xp_bottomMarkFontSize()
{
    if(!xv_rulerWidget)
    {
        return -1;
    }
    return xv_rulerWidget->xp_bottomFontSize();
}
//====================================================
bool X_Plotter::xp_setLeftMarkFormat(char format)
{
    if(!xv_rulerWidget)
    {
        return false;
    }
    return xv_rulerWidget->xp_setLeftMarkFormat(format);
}
//====================================================
bool X_Plotter::xp_setRightMarkFormat(char format)
{
    if(!xv_rulerWidget)
    {
        return false;
    }
    return xv_rulerWidget->xp_setRightMarkFormat(format);
}
//====================================================
bool X_Plotter::xp_setTopMarkFormat(char format)
{
    if(!xv_rulerWidget)
    {
        return false;
    }
    return xv_rulerWidget->xp_setTopMarkFormat(format);
}
//====================================================
bool X_Plotter::xp_setBottomMarkFormat(char format)
{
    if(!xv_rulerWidget)
    {
        return false;
    }
    return xv_rulerWidget->xp_setBottomMarkFormat(format);
}
//====================================================
bool X_Plotter::xp_setLeftMarkPrecision(int precision)
{
    if(!xv_rulerWidget)
    {
        return false;
    }
    return xv_rulerWidget->xp_setLeftMarkPrecision(precision);
}
//====================================================
bool X_Plotter::xp_setRightMarkPrecision(int precision)
{
    if(!xv_rulerWidget)
    {
        return false;
    }
    return xv_rulerWidget->xp_setRightMarkPrecision(precision);
}
//====================================================
bool X_Plotter::xp_setTopMarkPrecision(int precision)
{
    if(!xv_rulerWidget)
    {
        return false;
    }
    return xv_rulerWidget->xp_setTopMarkPrecision(precision);
}
//====================================================
bool X_Plotter::xp_setBottomMarkPrecision(int precision)
{
    if(!xv_rulerWidget)
    {
        return false;
    }
    return xv_rulerWidget->xp_setBottomMarkPrecision(precision);
}
//====================================================
void X_Plotter::xp_setHorizontalMarkRecalcFactors(const QString &label, double K0, double K1, double K2)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setHorizontalMarkRecalcFactors(label, K0, K1, K2);
}
//====================================================
void X_Plotter::xp_setVerticalMarkRecalcFactors(const QString &label, double K0, double K1, double K2)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setVerticalMarkRecalcFactors(label, K0, K1, K2);
}
//====================================================
void X_Plotter::xp_setLeftMarkRecalcMetrix(const QString& label, bool labelVisibility,double K0, double K1, double K2)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setLeftMarkRecalcMetrix(label, labelVisibility, K0, K1, K2);
}
//====================================================
void X_Plotter::xp_setRightMarkRecalcMetrix(const QString& label, bool labelVisibility,double K0, double K1, double K2)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setRightMarkRecalcMetrix(label, labelVisibility, K0, K1, K2);
}
//====================================================
void X_Plotter::xp_setTopMarkRecalcMetrix(const QString& label, bool labelVisibility,double K0, double K1, double K2)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setTopMarkRecalcMetrix(label, labelVisibility, K0, K1, K2);
}
//====================================================
void X_Plotter::xp_setBottomMarkRecalcMetrix(const QString& label, bool labelVisibility,double K0, double K1, double K2)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setBottomMarkRecalcMetrix(label, labelVisibility, K0, K1, K2);
}
//====================================================
void X_Plotter::xp_setLeftMarkRecalcFlag(bool flag)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setLeftMarkRecalcFlag(flag);
}
//====================================================
void X_Plotter::xp_setRightMarkRecalcFlag(bool flag)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setRightMarkRecalcFlag(flag);
}
//====================================================
void X_Plotter::xp_setTopMarkRecalcFlag(bool flag)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setTopMarkRecalcFlag(flag);
}
//====================================================
void X_Plotter::xp_setBottomMarkRecalcFlag(bool flag)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setBottomMarkRecalcFlag(flag);
}
//====================================================
void X_Plotter::xp_setTopRuleLabel(const QString &label)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setTopRuleLabel(label);
}
//====================================================
void X_Plotter::xp_setBottomRuleLabel(const QString &label)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setBottomRuleLabel(label);
}
//====================================================
void X_Plotter::xp_setLeftRuleLabel(const QString &label)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setLeftRuleLabel(label);
}
//====================================================
void X_Plotter::xp_setRightRuleLabel(const QString &label)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setRightRuleLabel(label);
}
//====================================================
void X_Plotter::xp_clearItemsForType(int type)
{
    QList<QGraphicsItem*> itemList = xv_plotScene->items();
    for(int i = 0; i < itemList.count(); i++)
    {
        if(itemList.value(i)->type() == type)
        {
            //itemList.value(i)->setVisible(false);

            xv_plotScene->xp_removeItem(itemList.value(i));
        }
    }

    //xv_plotView->update();
}
//====================================================
void X_Plotter::xp_clearItemsExeptType(int type)
{
    QList<QGraphicsItem*> itemList = xv_plotScene->items();
    for(int i = 0; i < itemList.count(); i++)
    {
        if(itemList.value(i)->type() == type)
        {
            continue;
        }
        xv_plotScene->xp_removeItem(itemList.value(i));
    }

    //xv_plotView->update();
}
//====================================================
void X_Plotter::xp_addItem(QGraphicsItem * item)
{
    if(item == nullptr)
    {
        return;
    }

    xv_plotScene->xp_addItem(item);
    QRectF itemBoundingSceneRect = xv_plotScene->itemsBoundingRect();

    if(xv_autoDefineVerticalAbsMax)
    {
        xp_setVerticalAbsMax(itemBoundingSceneRect.top());
    }
}
//====================================================
void X_Plotter::xp_removeItem(QGraphicsItem * item)
{
//    if(!item)
//    {
//        return;
//    }

    //item->setVisible(false);
    xv_plotScene->xp_removeItem(item);
    xv_plotView->update();
}
//====================================================
void X_Plotter::xp_removeItemsForType(int type)
{
    QList<QGraphicsItem*> itemList = xp_itemListForType(type);
    for(int i = itemList.count() - 1; i >= 0; i--)
    {
        xp_removeItem(itemList.at(i));
    }
}
//====================================================
QList<QGraphicsItem*> X_Plotter::xp_itemListForType(int type) const
{
    QList<QGraphicsItem*> itemList = xv_plotScene->items();
    for(int i = itemList.count()-1; i >= 0; i--)
    {
        if(itemList.value(i)->type() != type)
        {
            itemList.removeAt(i);
        }
    }

    return itemList;
}
//====================================================
int X_Plotter::xp_itemCount() const
{
    if(xv_plotScene == nullptr)
    {
        return 0;
    }

    return xv_plotScene->items().count();
}
//====================================================
void X_Plotter::xp_appendButtonsToDashboard(QList<QAction*>& actionList,
                                           X_HorizontalDashBoard::AuxWidgetPositon position,
                                           Qt::AlignmentFlag alignment,
                                           int extraSeparatorSpacing)
{
    if(!xv_dashBoard)
    {
        return;
    }
    xv_dashBoard->xp_appendButtonsToDashboard(actionList,
                                              position,
                                              alignment,
                                              extraSeparatorSpacing);
}
//====================================================
void X_Plotter::xp_appendWidgetToDashboard(QWidget* widget,
                                          X_HorizontalDashBoard::AuxWidgetPositon position,
                                          Qt::AlignmentFlag alignment,
                                          int extraSeparatorSpacing)
{
    if(!xv_dashBoard)
    {
        return;
    }
    xv_dashBoard->xp_appendWidgetToDashboard(widget,
                                             position,
                                             alignment,
                                             extraSeparatorSpacing);
}
//====================================================
void X_Plotter::xp_setVerticalAbsMax(double max)
{
    if(xv_verticalAbsMax - qAbs(max) == 0.0)
    {
        return;
    }
    xv_verticalAbsMax = qAbs(max);

    if(xh_recalcVerticalDistortionFactors(xv_dashBoard->xp_distortionValue()))
    {
        xh_recalcRulesAndItemCoordinates();
    }
}
//====================================================
double X_Plotter::xp_verticalAbsMax() const
{
    return xv_verticalAbsMax;
}
//====================================================
void X_Plotter::xp_setAutoDefineVerticalAbsMax(bool flag)
{
    xv_autoDefineVerticalAbsMax = flag;
}
//====================================================
void X_Plotter::xp_verticalDistortionFactors(double& distortionFactor, double& distortionCorrectionFactor)
{
    distortionFactor = xv_verticalDistortionFactor;
    distortionCorrectionFactor = xv_verticalDistortionCorrectionFactor;
}
//====================================================
bool X_Plotter::xp_isPlotScaled()
{
    return xv_horizontalScrollBar->isVisible()
            || xv_verticalScrollBar->isVisible();
}
//====================================================
void X_Plotter::xp_updatePlot()
{
    xv_plotView->update();
}
//====================================================
QBrush X_Plotter::xp_backgroundBrush()
{
    return xv_plotView->backgroundBrush();
}
//====================================================
void X_Plotter::xp_setContextMenu(QList<QAction*>& actionList)
{
    xv_plotView->addActions(actionList);
    xv_plotView->setContextMenuPolicy(Qt::DefaultContextMenu);
}
//====================================================
void X_Plotter::xp_setEnergyCalibration(QList<double> energyCalibrationFactorList)
{
    xv_energyCalibrationFactorList = energyCalibrationFactorList;
}
//====================================================
QRectF X_Plotter::xp_viewportSceneRect() const
{
    QRectF rect;
    if(!xv_plotView->xp_viewPortSceneRect(rect))
    {
        return QRectF();
    }

    return rect;
}
//====================================================
QSize X_Plotter::xp_viewportPixelSize() const
{
    return xv_plotView->viewport()->size();
}
//====================================================
void X_Plotter::xp_setInfoLabelVisible(bool visible)
{
    xv_rulerWidget->xp_setInfoLabelVisible(visible);
}
//====================================================
void X_Plotter::xp_setLeftRuleVisible(bool visible)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setLeftRuleVisibility(visible);
}
//====================================================
void X_Plotter::xp_setRightRuleVisible(bool visible)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setRightRuleVisibility(visible);
}
//====================================================
void X_Plotter::xp_setTopRuleVisible(bool visible)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setTopRuleVisibility(visible);
}
//====================================================
void X_Plotter::xp_setBottomRuleVisible(bool visible)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setBottomRuleVisibility(visible);
}
//====================================================
void X_Plotter::xp_setTopRuleLabelVisible(bool visible)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setTopRuleLabelVisible(visible);
}
//====================================================
void X_Plotter::xp_setBottomRuleLabelVisible(bool visible)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setBottomRuleLabelVisible(visible);
}
//====================================================
void X_Plotter::xp_setLeftRuleLabelVisible(bool visible)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setLeftRuleLabelVisible(visible);
}
//====================================================
void X_Plotter::xp_setRightRuleLabelVisible(bool visible)
{
    if(!xv_rulerWidget)
    {
        return;
    }
    xv_rulerWidget->xp_setRightRuleLabelVisible(visible);
}
//====================================================
void X_Plotter::xp_setDistortionSliderVisible(bool visible)
{
    if(!xv_dashBoard)
    {
        return;
    }
    xv_dashBoard->xp_setDistortionSliderVisible(visible);
}
//====================================================
void X_Plotter::xp_currentVisibleSceneRect(QRectF& sceneRect)
{
    xv_plotView->xp_viewPortSceneRect(sceneRect);
}
//====================================================
void X_Plotter::xp_fitInBoundingRect()
{
    if(!xv_plotView || !xv_plotScene)
    {
        return;
    }

    // block signals for scroll bar flicker prevention
    xv_verticalScrollBar->blockSignals(true);
    xv_horizontalScrollBar->blockSignals(true);

    QRectF itemBoundingRect = xv_plotScene->itemsBoundingRect();
    QRectF rectToFit;

    if(itemBoundingRect.isValid())
    {
        rectToFit = itemBoundingRect;
        xv_plotScene->setSceneRect(itemBoundingRect);
    }
    else
    {
        rectToFit = xv_plotScene->sceneRect();
    }
    xv_plotView->xp_fitInView(rectToFit);

    // deblock signals
    xv_verticalScrollBar->blockSignals(false);
    xv_horizontalScrollBar->blockSignals(false);

    xh_updateScrollBarsVisible();
    //xv_plotView->ensureVisible(rectToFit);
}
//====================================================
QRectF X_Plotter::xp_boundingRect() const
{
    return xv_plotScene->itemsBoundingRect();
}
//====================================================
void X_Plotter::resizeEvent(QResizeEvent* event)
{
    if(xv_plotView && xv_plotScene && xv_mouseButtonDown)
    {
        xv_userResizesWidget = true;
        xv_plotView->xp_setScaleViewWhenResizeFlag(true);
    }

    QWidget::resizeEvent(event);
}
//====================================================
bool X_Plotter::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress
            || event->type() == QEvent::NonClientAreaMouseButtonPress)
    {
        xv_mouseButtonDown = true;
        bool res = QObject::eventFilter(obj, event);
        return res;
    }
    else if(event->type() == QEvent::MouseButtonRelease
            || event->type() == QEvent::NonClientAreaMouseButtonRelease)
    {
        xv_mouseButtonDown = false;
        xv_userResizesWidget = false;
        xv_plotView->xp_setScaleViewWhenResizeFlag(false);
        xh_updateScrollBarsVisible();
        bool res = QObject::eventFilter(obj, event);
        return res;
    }

    bool res = QObject::eventFilter(obj, event);
    return res;
}
//====================================================
void X_Plotter::xh_verticalDistortionChanged(int distortionValue)
{
    if(xh_recalcVerticalDistortionFactors(static_cast<double>(distortionValue)))
    {
        xh_recalcRulesAndItemCoordinates();
        xv_plotView->xp_update();
    }
}
//====================================================
void X_Plotter::xh_scrollBarVisible(Qt::Orientation orientation, bool& visible)
{
    if(orientation == Qt::Vertical)
    {
        visible = xv_verticalScrollBar->isVisible();
    }
    else
    {
        visible = xv_horizontalScrollBar->isVisible();
    }
}
//====================================================
void X_Plotter::xh_onMousePress(QPointF mouseScenePos)
{

}
//====================================================
void X_Plotter::xh_mouseScenePositionChanged(QPointF scenePos) const
{
    if(!xv_rulerWidget)
    {
        return;
    }


    QString text = tr("Channel: %1").arg(QString::number(scenePos.x(), 'f', 0));

    if(!(xv_energyCalibrationFactorList.value(2, 0.0) == 0.0 &&
         (xv_energyCalibrationFactorList.value(1, 0.0) == 1.0 ||
          xv_energyCalibrationFactorList.value(1, 0.0) == 0.0) &&
         xv_energyCalibrationFactorList.value(0, 0.0) == 0.0))
    {
        double XValue = (pow(scenePos.x(), 2.0) *xv_energyCalibrationFactorList.value(2)) +
                (scenePos.x() *xv_energyCalibrationFactorList.value(1)) +
                xv_energyCalibrationFactorList.value(0);
        text += tr(" Energy: %1").arg(QString::number(XValue, 'f', 2));
    }

    double YValue = xv_rulersAndGreedManager->xp_recalcSceneVerticalPos(scenePos.y());
    text += tr(" Intensity: %1").arg(QString::number(YValue, 'f', 0));
    xv_rulerWidget->xp_setInfoLabelText(text);

}
//====================================================
void X_Plotter::xh_mouseLeaved() const
{
    xv_rulerWidget->xp_setInfoLabelText(QString());
}
//====================================================
void X_Plotter::xh_createComponents()
{
    xv_verticalScrollBar = new QScrollBar(Qt::Vertical, this);
    xv_horizontalScrollBar = new QScrollBar(Qt::Horizontal, this);
    xv_plotScene = new X_PlotGraphicsScene(this);
    xv_plotView = new X_PlotGraphicsView(nullptr);
    xv_rulerWidget = new X_RulerWidget(nullptr);
    xv_rulersAndGreedManager = new X_RulersAndGridManager(this);
    xv_dashBoard = new X_HorizontalDashBoard(this);
    xv_rulerWidget->xp_setPlotView(xv_plotView);

    // settings
    xv_plotView->setFrameStyle(QFrame::NoFrame);
    xv_plotView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    xv_plotView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // set outer scrollbars
    xv_plotView->setVerticalScrollBar(xv_verticalScrollBar);
    xv_plotView->setHorizontalScrollBar(xv_horizontalScrollBar);
    xv_plotView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    xv_verticalScrollBar->setVisible(false);
    xv_horizontalScrollBar->setVisible(false);

    xv_dashBoard->setMaximumHeight(xv_horizontalScrollBar->height());
    xv_dashBoard->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    // Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    QHBoxLayout* mainScrollLayout = new QHBoxLayout;
    mainScrollLayout->setContentsMargins(0, 0, 0, 0);
    mainScrollLayout->setSpacing(0);
    mainLayout->addLayout(mainScrollLayout);

    //QVBoxLayout* mainViewAndRulesLayout = new QVBoxLayout(this);
    mainScrollLayout->addWidget(xv_rulerWidget);
    mainScrollLayout->addWidget(xv_verticalScrollBar);

    QHBoxLayout* bottomScrollLayout = new QHBoxLayout;
    bottomScrollLayout->setContentsMargins(0, 0, 0, 0);
    bottomScrollLayout->setSpacing(0);
    mainLayout->addLayout(bottomScrollLayout);
    bottomScrollLayout->addWidget(xv_horizontalScrollBar, 9999999);
    bottomScrollLayout->addSpacing(10);
    bottomScrollLayout->addStretch(0);
    bottomScrollLayout->addWidget(xv_dashBoard);
    bottomScrollLayout->addSpacing(5);

}
//====================================================
void X_Plotter::xh_createConnections()
{
    // scroll bar visible control connection
    xh_connectScrollBars();
    xv_rulersAndGreedManager->xp_setRulerWidget(xv_rulerWidget);
    xv_plotView->setScene(xv_plotScene);
    xv_plotView->xp_connectToRulersAndGridManager(xv_rulersAndGreedManager);
    //xv_plotView->fitInView(QRectF(QPointF(0.0, -100.0), QPointF(2048, -100)));
    xv_dashBoard->xp_setPlotGraphicsView(xv_plotView);
    xv_dashBoard->xp_setPlotGraphicsScene(xv_plotScene);
    connect(xv_dashBoard, &X_HorizontalDashBoard::xg_distortionFactorChanged,
            this, &X_Plotter::xh_verticalDistortionChanged);
    connect(xv_plotView, &X_PlotGraphicsView::xg_inquiryForScrollBarVisible,
            this, &X_Plotter::xh_scrollBarVisible);
    connect(xv_plotView, &X_PlotGraphicsView::xg_cursorAreaImage,
            this, &X_Plotter::xg_cursorAreaImage);
    connect(xv_plotView, &X_PlotGraphicsView::xg_mousePressedAt,
            this, &X_Plotter::xg_mousePressedAt);
    connect(xv_plotView, &X_PlotGraphicsView::xg_viewportRectChanged,
            this, &X_Plotter::xg_viewportRectChanged);

    connect(xv_plotView, &X_PlotGraphicsView::xg_mouseScenePositionChanged,
            this, &X_Plotter::xh_mouseScenePositionChanged);
    connect(xv_plotView, &X_PlotGraphicsView::xg_mouseLeaved,
            this, &X_Plotter::xh_mouseLeaved);
    connect(xv_plotView, &X_PlotGraphicsView::xg_rulerToolChanged,
            this, &X_Plotter::xg_rulerToolChanged);


}
//====================================================
void X_Plotter::xh_connectScrollBars()
{
    connect(xv_horizontalScrollBar, &QScrollBar::rangeChanged,
            this, &X_Plotter::xh_scrollBarVisibleControl);
    connect(xv_verticalScrollBar, &QScrollBar::rangeChanged,
            this, &X_Plotter::xh_scrollBarVisibleControl);

    connect(xv_horizontalScrollBar, &QScrollBar::valueChanged,
            this, &X_Plotter::xh_notifySceneRect);
    connect(xv_verticalScrollBar, &QScrollBar::valueChanged,
            this, &X_Plotter::xh_notifySceneRect);

}
//====================================================
void X_Plotter::xh_updateScrollBarsVisible()
{
    xv_horizontalScrollBar->setHidden(xv_horizontalScrollBar->maximum() == 0
                                      && xv_horizontalScrollBar->minimum() == 0);

    xv_verticalScrollBar->setHidden(xv_verticalScrollBar->maximum() == 0
                                    && xv_verticalScrollBar->minimum() == 0);
}
//====================================================
void X_Plotter::xh_scrollBarVisibleControl(int min , int max)
{
    QScrollBar* scrollBar = qobject_cast<QScrollBar*>(sender());
    if(!scrollBar || xv_userResizesWidget)
    {
        return;
    }
    scrollBar->setHidden(min == 0 && max == 0);
}
//====================================================
void X_Plotter::xh_notifySceneRect(int value)
{
    QRectF viewportRect;
    xv_plotView->xp_viewPortSceneRect(viewportRect);
    if(!viewportRect.isNull())
    {
        emit xg_viewportRectChanged(viewportRect);
    }
}
//====================================================
bool X_Plotter::xh_recalcVerticalDistortionFactors(double distortionValue)
{
    if(xv_verticalAbsMax == 0.0)
    {
        return false;
    }

    if(distortionValue < 0)
    {
        distortionValue = 0;
    }
    else if(distortionValue > 15)
    {
        distortionValue = 15;
    }
    xv_verticalDistortionFactor = 1.0 - static_cast<double>(distortionValue) / 20.0;
    xv_verticalDistortionCorrectionFactor = xv_verticalAbsMax / pow(xv_verticalAbsMax, xv_verticalDistortionFactor);

    return true;
}
//====================================================
void X_Plotter::xh_recalcRulesAndItemCoordinates()
{
    if(xv_rulersAndGreedManager != nullptr)
    {
        xv_rulersAndGreedManager->xp_recalcRulesAndGrid();
        xv_rulerWidget->update();
    }

    QList<QGraphicsItem*> itemList = xv_plotScene->items();
    for(int i = 0; i < itemList.count(); i++)
    {
        if(itemList.value(i)->type() == SpectrumItemType)
        {
            X_SpectrumGraphicsItem* spectrumItem = qgraphicsitem_cast<X_SpectrumGraphicsItem*>(itemList.value(i));
            if(!spectrumItem)
            {
                continue;
            }

            spectrumItem->xp_setDistortion(xv_verticalDistortionFactor, xv_verticalDistortionCorrectionFactor);
        }
        else if(itemList.value(i)->type() == EnergyLineItemType)
        {
            X_EnergyLineGraphicsItem* energyLineItem = qgraphicsitem_cast<X_EnergyLineGraphicsItem*>(itemList.value(i));
            if(!energyLineItem)
            {
                continue;
            }

            energyLineItem->xp_updateItem();
        }
    }
}
//====================================================
