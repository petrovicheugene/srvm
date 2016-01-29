//==============================================================
#include "ZPredictorCorrelationPlotterWidget.h"
#include "ZPlotter.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>

//==============================================================
ZPredictorCorrelationPlotterWidget::ZPredictorCorrelationPlotterWidget(QWidget *parent) : QWidget(parent)
{
   zh_createComponents();
   zh_createConnections();
}
//==============================================================
ZPlotter* ZPredictorCorrelationPlotterWidget::zp_plotter()
{
   return zv_plotter;
}
//==============================================================
void ZPredictorCorrelationPlotterWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    zv_plotter = new ZPlotter(this);
    QFrame* frame = new QFrame();
    QVBoxLayout* frameLayout = new QVBoxLayout(frame);
    frame->setLayout(frameLayout);
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    frame->setLineWidth(1);
    frameLayout->addWidget(zv_plotter);

    mainLayout->addWidget(frame, INT_MAX);

}
//==============================================================
void ZPredictorCorrelationPlotterWidget::zh_createConnections()
{

}
//==============================================================
