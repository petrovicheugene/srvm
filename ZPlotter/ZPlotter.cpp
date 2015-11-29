//====================================================
#include "ZPlotter.h"
#include "ZPlotGraphicsScene.h"
#include "ZPlotGraphicsView.h"
#include "ZRulersAndGridManager.h"
#include "ZRulerWidget.h"
#include "ZDashBoard.h"

#include <QApplication>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollBar>

#include <QDebug>
//====================================================
ZPlotter::ZPlotter(QWidget *parent) : QWidget(parent)
{

    zv_rulerWidget = 0;
    zv_plotView = 0;
    zv_verticalScrollBar = 0;
    zv_horizontalScrollBar = 0;
    zv_plotScene = 0;
    zv_rulersAndGreedManager = 0;

    zv_mouseButtonDown = false;
    zv_userResizesWidget = false;
    zh_createComponents();
    zh_createConnections();
    qApp->installEventFilter(this);

}
//====================================================
ZPlotGraphicsScene* ZPlotter::zp_plotScene()
{
    return zv_plotScene;
}
//====================================================
bool ZPlotter::zp_setMinimalVerticalScaleInterval(qreal interval)
{
    if(!zv_rulersAndGreedManager)
    {
        return false;
    }
    return zv_rulersAndGreedManager->zp_setMinimalVerticalScaleInterval(interval);
}
//====================================================
qreal ZPlotter::zp_minimalVerticalScaleInterval()
{
    if(!zv_rulersAndGreedManager)
    {
        return -1.0;
    }
    return zv_rulersAndGreedManager->zp_minimalVerticalScaleInterval();
}
//====================================================
bool ZPlotter::zp_setMinimalHorizontalScaleInterval(qreal interval)
{
    if(!zv_rulersAndGreedManager)
    {
        return false;
    }
    return zv_rulersAndGreedManager->zp_setMinimalHorizontalScaleInterval(interval);
}
//====================================================
qreal ZPlotter::zp_minimalHorizontalScaleInterval()
{
    if(!zv_rulersAndGreedManager)
    {
        return -1.0;
    }
    return zv_rulersAndGreedManager->zp_minimalHorizontalScaleInterval();
}
//====================================================
bool ZPlotter::zp_setLeftMarkFontSize(int size)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setLeftFontSize(size);
}
//====================================================
bool ZPlotter::zp_setRightMarkFontSize(int size)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setRightFontSize(size);
}
//====================================================
bool ZPlotter::zp_setTopMarkFontSize(int size)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setTopFontSize(size);
}
//====================================================
bool ZPlotter::zp_setBottomMarkFontSize(int size)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setBottomFontSize(size);
}
//====================================================
int ZPlotter::zp_leftMarkFontSize()
{
    if(!zv_rulerWidget)
    {
        return -1;
    }
    return zv_rulerWidget->zp_leftFontSize();
}
//====================================================
int ZPlotter::zp_rightMarkFontSize()
{
    if(!zv_rulerWidget)
    {
        return -1;
    }
    return zv_rulerWidget->zp_rightFontSize();
}
//====================================================
int ZPlotter::zp_topMarkFontSize()
{
    if(!zv_rulerWidget)
    {
        return -1;
    }
    return zv_rulerWidget->zp_topFontSize();
}
//====================================================
int ZPlotter::zp_bottomMarkFontSize()
{
    if(!zv_rulerWidget)
    {
        return -1;
    }
    return zv_rulerWidget->zp_bottomFontSize();
}
//====================================================
bool ZPlotter::zp_setLeftMarkFormat(char format)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setLeftMarkFormat(format);
}
//====================================================
bool ZPlotter::zp_setRightMarkFormat(char format)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setRightMarkFormat(format);
}
//====================================================
bool ZPlotter::zp_setTopMarkFormat(char format)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setTopMarkFormat(format);
}
//====================================================
bool ZPlotter::zp_setBottomMarkFormat(char format)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setBottomMarkFormat(format);
}
//====================================================
bool ZPlotter::zp_setLeftMarkPrecision(int precision)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setLeftMarkPrecision(precision);
}
//====================================================
bool ZPlotter::zp_setRightMarkPrecision(int precision)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setRightMarkPrecision(precision);
}
//====================================================
bool ZPlotter::zp_setTopMarkPrecision(int precision)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setTopMarkPrecision(precision);
}
//====================================================
bool ZPlotter::zp_setBottomMarkPrecision(int precision)
{
    if(!zv_rulerWidget)
    {
        return false;
    }
    return zv_rulerWidget->zp_setBottomMarkPrecision(precision);
}
//====================================================
void ZPlotter::zp_setLeftRuleVisible(bool visible)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setLeftRuleVisibility(visible);
}
//====================================================
void ZPlotter::zp_setRightRuleVisible(bool visible)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setRightRuleVisibility(visible);
}
//====================================================
void ZPlotter::zp_setTopRuleVisible(bool visible)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setTopRuleVisibility(visible);
}
//====================================================
void ZPlotter::zp_setBottomRuleVisible(bool visible)
{
    if(!zv_rulerWidget)
    {
        return;
    }
    zv_rulerWidget->zp_setBottomRuleVisibility(visible);
}
//====================================================
void ZPlotter::resizeEvent(QResizeEvent* event)
{
    if(zv_plotView && zv_plotScene && zv_mouseButtonDown)
    {
        zv_userResizesWidget = true;
        zv_plotView->zp_setScaleViewWhenResizeFlag(true);
    }

    QWidget::resizeEvent(event);
}
//====================================================
bool ZPlotter::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress
            || event->type() == QEvent::NonClientAreaMouseButtonPress)
    {
        zv_mouseButtonDown = true;
        return QObject::eventFilter(obj, event);
    }

    if(event->type() == QEvent::MouseButtonRelease
            || event->type() == QEvent::NonClientAreaMouseButtonRelease)
    {
        zv_mouseButtonDown = false;
        zv_userResizesWidget = false;
        zv_plotView->zp_setScaleViewWhenResizeFlag(false);

        zh_updateScrollBarsVisible();
        return QObject::eventFilter(obj, event);
    }

    return QObject::eventFilter(obj, event);
}
//====================================================
void ZPlotter::zh_createComponents()
{
    zv_verticalScrollBar = new QScrollBar(Qt::Vertical, this);
    zv_horizontalScrollBar = new QScrollBar(Qt::Horizontal, this);
    zv_plotScene = new ZPlotGraphicsScene(this);
    zv_plotView = new ZPlotGraphicsView(0);
    zv_rulerWidget = new ZRulerWidget(0);
    zv_rulersAndGreedManager = new ZRulersAndGridManager(this);
    zv_dashBoard = new ZDashBoard(this);
    zv_rulerWidget->zp_setPlotView(zv_plotView);

    // settings
    zv_plotView->setFrameStyle(QFrame::NoFrame);
    zv_plotView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    zv_plotView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // set outer scrollbars
    zv_plotView->setVerticalScrollBar(zv_verticalScrollBar);
    zv_plotView->setHorizontalScrollBar(zv_horizontalScrollBar);
    zv_plotView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    zv_dashBoard->setMaximumHeight(zv_horizontalScrollBar->height());
    zv_dashBoard->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    zv_plotScene->setSceneRect(0.0, -100.0, 2048, 100);

    // Layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    QHBoxLayout* mainScrollLayout = new QHBoxLayout(this);
    mainScrollLayout->setMargin(0);
    mainScrollLayout->setSpacing(0);
    mainLayout->addLayout(mainScrollLayout);

    //QVBoxLayout* mainViewAndRulesLayout = new QVBoxLayout(this);
    mainScrollLayout->addWidget(zv_rulerWidget);
    mainScrollLayout->addWidget(zv_verticalScrollBar);

    QHBoxLayout* bottomScrollLayout = new QHBoxLayout(this);
    bottomScrollLayout->setMargin(0);
    bottomScrollLayout->setSpacing(0);
    mainLayout->addLayout(bottomScrollLayout);
    bottomScrollLayout->addWidget(zv_horizontalScrollBar, 9999999);
    bottomScrollLayout->addSpacing(10);
    bottomScrollLayout->addStretch(0);
    bottomScrollLayout->addWidget(zv_dashBoard);
    bottomScrollLayout->addSpacing(5);

}
//====================================================
void ZPlotter::zh_createConnections()
{
    // scroll bar visible control connection
    zh_connectScrollBars();
    zv_rulersAndGreedManager->zp_setRulerWidget(zv_rulerWidget);
    zv_plotView->setScene(zv_plotScene);
    zv_plotView->zp_connectToRulersAndGridManager(zv_rulersAndGreedManager);
    zv_plotView->fitInView(zv_plotView->sceneRect());
    zv_dashBoard->zp_setPlotGraphicsView(zv_plotView);
}
//====================================================
void ZPlotter::zh_connectScrollBars()
{
    connect(zv_horizontalScrollBar, &QScrollBar::rangeChanged,
            this, &ZPlotter::zh_scrollBarVisibleControl);
    connect(zv_verticalScrollBar, &QScrollBar::rangeChanged,
            this, &ZPlotter::zh_scrollBarVisibleControl);
}
//====================================================
void ZPlotter::zh_updateScrollBarsVisible()
{
    zv_horizontalScrollBar->setHidden(zv_horizontalScrollBar->maximum() == 0
                                      && zv_horizontalScrollBar->minimum() == 0);

    zv_verticalScrollBar->setHidden(zv_verticalScrollBar->maximum() == 0
                                    && zv_verticalScrollBar->minimum() == 0);
}
//====================================================
void ZPlotter::zh_scrollBarVisibleControl(int min , int max)
{
    QScrollBar* scrollBar = qobject_cast<QScrollBar*>(sender());
    if(!scrollBar || zv_userResizesWidget)
    {
        return;
    }
    scrollBar->setHidden(min == 0 && max == 0);
}
//====================================================
