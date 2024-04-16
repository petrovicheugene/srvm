//==============================================================
#include "X_CorrelationPlotterWidget.h"
#include "X_Plotter.h"
#include <QEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>

//==============================================================
X_CorrelationPlotterWidget::X_CorrelationPlotterWidget(QWidget *parent) : QWidget(parent)
{
    setObjectName("X_CorrelationPlotterWidget");
    xh_createComponents();
    xh_createConnections();
    if(xv_plotter)
    {
        QMetaObject::invokeMethod(xv_plotter, "xp_fitInBoundingRect",
                                  Qt::QueuedConnection);
    }
}
//==============================================================
X_Plotter* X_CorrelationPlotterWidget::xp_plotter()
{
    return xv_plotter;
}
//==============================================================
void X_CorrelationPlotterWidget::xp_rebuildPlotter()
{
    if(xv_plotter != nullptr)
    {
        delete xv_plotter;
    }

    xv_plotter = new X_Plotter("CorrelationPlotter", this);
    xv_plotter->xp_setInfoLabelVisible(false);
    frameLayout->addWidget(xv_plotter);
}
//==============================================================
void X_CorrelationPlotterWidget::xh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QFrame* frame = new QFrame();
    frameLayout = new QVBoxLayout;
    frame->setLayout(frameLayout);
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    frame->setLineWidth(1);

    mainLayout->addWidget(frame, INT_MAX);

    xp_rebuildPlotter();
}
//==============================================================
void X_CorrelationPlotterWidget::xh_createConnections()
{

}
//==============================================================
