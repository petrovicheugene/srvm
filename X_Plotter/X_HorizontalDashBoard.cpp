//===========================================================
#include "X_HorizontalDashBoard.h"
#include "X_PlotGraphicsView.h"
#include "X_PlotGraphicsScene.h"
#include "X_General.h"

#include <QToolButton>
#include <QHBoxLayout>
#include <QTimerEvent>
#include <QAction>
#include <QSlider>

//===========================================================
X_HorizontalDashBoard::X_HorizontalDashBoard(QWidget *parent) : QFrame(parent)
{
    xv_plotGraphicsView = 0;
    xv_plotGraphicsScene = 0;
    xv_timer = 0;
    zh_createComponents();
    zh_createConnections();
}
//===========================================================
void X_HorizontalDashBoard::zh_createComponents()
{
    xv_mainLayout = new QHBoxLayout;
    xv_mainLayout->setContentsMargins(0, 0, 0, 0);
    xv_mainLayout->setSpacing(3);
    setLayout(xv_mainLayout);

    xv_distortionSlider = new QSlider(this);
    xv_distortionSlider->setMinimum(0);
    xv_distortionSlider->setMaximum(15);
    xv_distortionSlider->setOrientation(Qt::Horizontal);
    xv_distortionSlider->setTickInterval(3);
    xv_distortionSlider->setTickPosition(QSlider::TicksAbove);
    xv_distortionSlider->setValue(0);
    xv_mainLayout->addWidget(xv_distortionSlider);

    xv_hideGridButton = new QToolButton(this);
    xv_hideGridButton->setIcon(QIcon(NS_Icons::glIconShowGrid));
    xv_hideGridButton->setCheckable(true);
    xv_mainLayout->addWidget(xv_hideGridButton, 0, Qt::AlignRight);

    xv_increaseVerticalButton = new QToolButton(this);
    xv_increaseVerticalButton->setIcon(QIcon(NS_Icons::glIconVerticalIncrease));
    xv_mainLayout->addWidget(xv_increaseVerticalButton, 0, Qt::AlignRight);

    xv_decreaseVerticalButton = new QToolButton(this);
    xv_decreaseVerticalButton->setIcon(QIcon(NS_Icons::glIconVerticalDecrease));
    xv_mainLayout->addWidget(xv_decreaseVerticalButton, 0, Qt::AlignRight);

    xv_increaseHorizontalButton = new QToolButton(this);
    xv_increaseHorizontalButton->setIcon(QIcon(NS_Icons::glIconHorizontalIncrease));
    xv_mainLayout->addWidget(xv_increaseHorizontalButton, 0, Qt::AlignRight);

    xv_decreaseHorizontalButton = new QToolButton(this);
    xv_decreaseHorizontalButton->setIcon(QIcon(NS_Icons::glIconHorizontalDecrease));
    xv_mainLayout->addWidget(xv_decreaseHorizontalButton, 0, Qt::AlignRight);

    xv_fitViewInSceneButton = new QToolButton(this);
    xv_fitViewInSceneButton->setIcon(QIcon(NS_Icons::glIconFitInRect));
    xv_mainLayout->addWidget(xv_fitViewInSceneButton, 0, Qt::AlignRight);

}
//===========================================================
void X_HorizontalDashBoard::xp_setPlotGraphicsView(X_PlotGraphicsView* plotGraphicsView)
{
    xv_plotGraphicsView = plotGraphicsView;
}
//===========================================================
void X_HorizontalDashBoard::xp_setPlotGraphicsScene(X_PlotGraphicsScene* plotGraphicsScene)
{
    xv_plotGraphicsScene = plotGraphicsScene;
}
//===========================================================
void X_HorizontalDashBoard::xp_appendButtonsToDashboard(QList<QAction*>& actionList,
                                                       AuxWidgetPositon position,
                                                       Qt::AlignmentFlag alignment,
                                                       int extraSeparatorSpacing)
{
    int insertIndex;
    if(position == AWP_LEFT_OF_BUTTONS)
    {
        insertIndex = 1; // 0 - log slider
    }
    else
    {
        //insertIndex = actionList.count();
        insertIndex = xv_mainLayout->count();
        xv_mainLayout->addSpacing(extraSeparatorSpacing);
    }

    QToolButton* button;
    for(int i = 0; i < actionList.count(); i++, insertIndex++)
    {
        button = new QToolButton(this);
        button->setIcon(actionList.value(i)->icon());
        button->setToolTip(actionList.value(i)->toolTip());
        button->setCheckable(actionList.value(i)->isCheckable());
        if(actionList.value(i)->isCheckable())
        {
            //            connect(button, &QToolButton::toggled ,
            //                    actionList[i], &QAction::trigger);
            connect(button, &QToolButton::toggled ,
                    actionList[i], &QAction::setChecked);
            connect(actionList[i], &QAction::toggled,
                    button, &QToolButton::setChecked);
        }
        else
        {
            connect(button, &QToolButton::clicked,
                    actionList[i], &QAction::trigger);

        }
        xv_mainLayout->insertWidget(insertIndex, button, alignment);
    }

    if(position == AWP_LEFT_OF_BUTTONS)
    {
        if(alignment == Qt::AlignLeft)
        {
            xv_mainLayout->insertStretch(actionList.count(), 999999);
        }

        if(extraSeparatorSpacing > 0)
        {
            xv_mainLayout->insertSpacing(actionList.count(), extraSeparatorSpacing);
        }
    }
}
//===========================================================
void X_HorizontalDashBoard::xp_appendWidgetToDashboard(QWidget* widget,
                                                      AuxWidgetPositon position,
                                                      Qt::AlignmentFlag alignment,
                                                      int extraSeparatorSpacing)
{
    int insertIndex;
    if(position == AWP_LEFT_OF_BUTTONS)
    {
        insertIndex = 0; // 0 - log slider
    }
    else
    {
        insertIndex = xv_mainLayout->count();
        if(extraSeparatorSpacing > 0)
        {
            xv_mainLayout->addSpacing(extraSeparatorSpacing);
        }
    }

    xv_mainLayout->insertWidget(insertIndex, widget, alignment);
    if(position == AWP_LEFT_OF_BUTTONS)
    {
        if(alignment == Qt::AlignLeft)
        {
            xv_mainLayout->insertStretch(1, 999999);
        }

        if(extraSeparatorSpacing > 0)
        {
            xv_mainLayout->insertSpacing(1, extraSeparatorSpacing);
        }
    }
}
//===========================================================
int X_HorizontalDashBoard::xp_distortionValue()
{
    if(xv_distortionSlider == 0)
    {
        return 0;
    }
    return xv_distortionSlider->value();
}
//===========================================================
void X_HorizontalDashBoard::xp_setDistortionSliderVisible(bool visible)
{
    if(xv_distortionSlider == 0)
    {
        return;
    }
    xv_distortionSlider->setVisible(visible);
}
//===========================================================
void X_HorizontalDashBoard::timerEvent(QTimerEvent* event)
{
    zh_killTimer();
    if(!xv_plotGraphicsView)
    {
        return;
    }

    xv_plotGraphicsView->xp_scale(xv_orientation, xv_increase);
    xv_timer = startTimer(100);
}
//===========================================================
void X_HorizontalDashBoard::zh_createConnections()
{
    connect(xv_increaseVerticalButton, &QToolButton::pressed,
            this, &X_HorizontalDashBoard::zh_increaseVertical);
    connect(xv_increaseHorizontalButton, &QToolButton::pressed,
            this, &X_HorizontalDashBoard::zh_increaseHorizontal);
    connect(xv_decreaseVerticalButton, &QToolButton::pressed,
            this, &X_HorizontalDashBoard::zh_decreaseVertical);
    connect(xv_decreaseHorizontalButton, &QToolButton::pressed,
            this, &X_HorizontalDashBoard::zh_decreaseHorizontal);

    connect(xv_increaseVerticalButton, &QToolButton::released,
            this, &X_HorizontalDashBoard::zh_killTimer);
    connect(xv_increaseHorizontalButton, &QToolButton::released,
            this, &X_HorizontalDashBoard::zh_killTimer);
    connect(xv_decreaseVerticalButton, &QToolButton::released,
            this, &X_HorizontalDashBoard::zh_killTimer);
    connect(xv_decreaseHorizontalButton, &QToolButton::released,
            this, &X_HorizontalDashBoard::zh_killTimer);

    connect(xv_hideGridButton, SIGNAL(toggled(bool)),
            this, SLOT(zh_hideGrid(bool)));
    connect(xv_fitViewInSceneButton, &QToolButton::clicked,
            this, &X_HorizontalDashBoard::zh_fitViewInScene);

    connect(xv_distortionSlider, &QSlider::valueChanged,
            this, &X_HorizontalDashBoard::xg_distortionFactorChanged);
}
//===========================================================
void X_HorizontalDashBoard::zh_killTimer()
{
    if(xv_timer)
    {
        killTimer(xv_timer);
        xv_timer = 0;
    }
}
//===========================================================
void X_HorizontalDashBoard::zh_increaseVertical()
{
    if(xv_plotGraphicsView == 0)
    {
        return;
    }
    xv_plotGraphicsView->xp_scale(Qt::Vertical, true);
    xv_orientation = Qt::Vertical;
    xv_increase = true;
    zh_startTimer();
}
//===========================================================
void X_HorizontalDashBoard::zh_increaseHorizontal()
{
    if(xv_plotGraphicsView == 0)
    {
        return;
    }
    xv_plotGraphicsView->xp_scale(Qt::Horizontal, true);
    xv_orientation = Qt::Horizontal;
    xv_increase = true;
    zh_startTimer();
}
//===========================================================
void X_HorizontalDashBoard::zh_decreaseVertical()
{
    if(xv_plotGraphicsView == 0)
    {
        return;
    }
    xv_plotGraphicsView->xp_scale(Qt::Vertical, false);
    xv_orientation = Qt::Vertical;
    xv_increase = false;
    zh_startTimer();
}
//===========================================================
void X_HorizontalDashBoard::zh_decreaseHorizontal()
{
    if(xv_plotGraphicsView == 0)
    {
        return;
    }
    xv_plotGraphicsView->xp_scale(Qt::Horizontal, false);
    xv_orientation = Qt::Horizontal;
    xv_increase = false;
    zh_startTimer();
}
//===========================================================
void X_HorizontalDashBoard::zh_fitViewInScene() const
{
    if(xv_plotGraphicsView == 0 || xv_plotGraphicsScene == 0)
    {
        return;
    }

    QRectF itemBoundingRect = xv_plotGraphicsScene->itemsBoundingRect();
    xv_plotGraphicsScene->setSceneRect(itemBoundingRect);
    QRectF rectToFit;
    if(itemBoundingRect.isValid())
    {
        rectToFit = itemBoundingRect;
    }
    else
    {
        rectToFit = xv_plotGraphicsScene->sceneRect();
    }
    xv_plotGraphicsView->xp_fitInView(rectToFit);
    //xv_plotGraphicsView->ensureVisible(rectToFit, 2, 2);
}
//===========================================================
void X_HorizontalDashBoard::zh_hideGrid(bool toggled) const
{
    if(xv_plotGraphicsView == 0)
    {
        return;
    }
    xv_plotGraphicsView->xp_setGridHidden(toggled);
}
//===========================================================
void X_HorizontalDashBoard::zh_startTimer()
{
    xv_timer = startTimer(300);
}
//===========================================================
