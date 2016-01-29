//==============================================================
#ifndef ZPREDICTORCORRELATIONPLOTWIDGET_H
#define ZPREDICTORCORRELATIONPLOTWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class ZPlotter;

//==============================================================
class ZPredictorCorrelationPlotterWidget : public QWidget
{
   Q_OBJECT
public:
   explicit ZPredictorCorrelationPlotterWidget(QWidget *parent = 0);

   // FUNCS
   ZPlotter* zp_plotter();

signals:

public slots:


private:


   // VARS
   ZPlotter* zv_plotter;
   // FUNCS
   void zh_createComponents();
   void zh_createConnections();

};
//==============================================================
#endif // ZPREDICTORCORRELATIONPLOTWIDGET_H
