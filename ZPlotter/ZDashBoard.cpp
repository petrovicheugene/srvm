//===========================================================
#include "ZDashBoard.h"
#include "ZPlotGraphicsView.h"
#include <QToolButton>
#include <QHBoxLayout>
#include <QTimerEvent>
//===========================================================
ZDashBoard::ZDashBoard(QWidget *parent) : QFrame(parent)
{
    zv_plotGraphicsView = 0;
    zv_timer = 0;
    zh_createComponents();
    zh_createConnections();
}
//===========================================================
void ZDashBoard::zh_createComponents()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(3);
    setLayout(mainLayout);

    zv_hideGridButton = new QToolButton(this);
    zv_hideGridButton->setCheckable(true);
    mainLayout->addWidget(zv_hideGridButton, 0, Qt::AlignRight);

    zv_increaseVerticalButton = new QToolButton(this);
    mainLayout->addWidget(zv_increaseVerticalButton, 0, Qt::AlignRight);

    zv_decreaseVerticalButton = new QToolButton(this);
    mainLayout->addWidget(zv_decreaseVerticalButton, 0, Qt::AlignRight);

    zv_increaseHorizontalButton = new QToolButton(this);
    mainLayout->addWidget(zv_increaseHorizontalButton, 0, Qt::AlignRight);

    zv_decreaseHorizontalButton = new QToolButton(this);
    mainLayout->addWidget(zv_decreaseHorizontalButton, 0, Qt::AlignRight);

    zv_fitViewInSceneButton = new QToolButton(this);
    mainLayout->addWidget(zv_fitViewInSceneButton, 0, Qt::AlignRight);

}
//===========================================================
void ZDashBoard::zp_setPlotGraphicsView(ZPlotGraphicsView* plotGraphicsView)
{
    zv_plotGraphicsView = plotGraphicsView;
}
//===========================================================
void ZDashBoard::timerEvent(QTimerEvent* event)
{
    zh_killTimer();
    if(!zv_plotGraphicsView)
    {
        return;
    }

    zv_plotGraphicsView->zp_scale(zv_orientation, zv_increase);
    zv_timer = startTimer(100);
}
//===========================================================
void ZDashBoard::zh_createConnections()
{
    connect(zv_increaseVerticalButton, &QToolButton::pressed,
            this, &ZDashBoard::zh_increaseVertical);
    connect(zv_increaseHorizontalButton, &QToolButton::pressed,
            this, &ZDashBoard::zh_increaseHorizontal);
    connect(zv_decreaseVerticalButton, &QToolButton::pressed,
            this, &ZDashBoard::zh_decreaseVertical);
    connect(zv_decreaseHorizontalButton, &QToolButton::pressed,
            this, &ZDashBoard::zh_decreaseHorizontal);

    connect(zv_increaseVerticalButton, &QToolButton::released,
            this, &ZDashBoard::zh_killTimer);
    connect(zv_increaseHorizontalButton, &QToolButton::released,
            this, &ZDashBoard::zh_killTimer);
    connect(zv_decreaseVerticalButton, &QToolButton::released,
            this, &ZDashBoard::zh_killTimer);
    connect(zv_decreaseHorizontalButton, &QToolButton::released,
            this, &ZDashBoard::zh_killTimer);

    connect(zv_hideGridButton, SIGNAL(toggled(bool)),
            this, SLOT(zh_hideGrid(bool)));
    connect(zv_fitViewInSceneButton, &QToolButton::clicked,
            this, &ZDashBoard::zh_fitViewinScene);

}
//===========================================================
void ZDashBoard::zh_killTimer()
{
    if(zv_timer)
    {
        killTimer(zv_timer);
        zv_timer = 0;
    }
}
//===========================================================
void ZDashBoard::zh_increaseVertical()
{
    if(zv_plotGraphicsView == 0)
    {
        return;
    }
    zv_plotGraphicsView->zp_scale(Qt::Vertical, true);
    zv_orientation = Qt::Vertical;
    zv_increase = true;
    zh_startTimer();
}
//===========================================================
void ZDashBoard::zh_increaseHorizontal()
{
    if(zv_plotGraphicsView == 0)
    {
        return;
    }
    zv_plotGraphicsView->zp_scale(Qt::Horizontal, true);
    zv_orientation = Qt::Horizontal;
    zv_increase = true;
    zh_startTimer();
}
//===========================================================
void ZDashBoard::zh_decreaseVertical()
{
    if(zv_plotGraphicsView == 0)
    {
        return;
    }
    zv_plotGraphicsView->zp_scale(Qt::Vertical, false);
    zv_orientation = Qt::Vertical;
    zv_increase = false;
    zh_startTimer();
}
//===========================================================
void ZDashBoard::zh_decreaseHorizontal()
{
    if(zv_plotGraphicsView == 0)
    {
        return;
    }
    zv_plotGraphicsView->zp_scale(Qt::Horizontal, false);
    zv_orientation = Qt::Horizontal;
    zv_increase = false;
    zh_startTimer();
}
//===========================================================
void ZDashBoard::zh_fitViewinScene()
{
    zv_plotGraphicsView->fitInView(zv_plotGraphicsView->sceneRect());
}
//===========================================================
void ZDashBoard::zh_hideGrid(bool toggled)
{
    if(zv_plotGraphicsView == 0)
    {
        return;
    }
    zv_plotGraphicsView->zp_setGridHidden(toggled);
}
//===========================================================
void ZDashBoard::zh_startTimer()
{
    zv_timer = startTimer(300);
}
//===========================================================
