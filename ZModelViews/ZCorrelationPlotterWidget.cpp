//==============================================================
#include "ZCorrelationPlotterWidget.h"
#include "ZPlotter.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>

//==============================================================
ZCorrelationPlotterWidget::ZCorrelationPlotterWidget(QWidget *parent) : QWidget(parent)
{
    zv_plotter = 0;

    zh_createComponents();
    zh_createConnections();
    if(zv_plotter != 0)
    {
        QMetaObject::invokeMethod(zv_plotter, "zp_fitInBoundingRect",
                                  Qt::QueuedConnection);
    }
}
//==============================================================
ZPlotter* ZCorrelationPlotterWidget::zp_plotter()
{
    return zv_plotter;
}
//==============================================================
void ZCorrelationPlotterWidget::zp_rebuildPlotter()
{
    if(zv_plotter != 0)
    {
        delete zv_plotter;
    }

    zv_plotter = new ZPlotter(this);
    frameLayout->addWidget(zv_plotter);
}
//==============================================================
void ZCorrelationPlotterWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);
    mainLayout->setMargin(0);

    //zv_plotter = new ZPlotter(this);
    QFrame* frame = new QFrame();
    frameLayout = new QVBoxLayout(frame);
    frame->setLayout(frameLayout);
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    frame->setLineWidth(1);
    frameLayout->addWidget(zv_plotter);

    mainLayout->addWidget(frame, INT_MAX);

    zp_rebuildPlotter();
}
//==============================================================
void ZCorrelationPlotterWidget::zh_createConnections()
{

}
//==============================================================
