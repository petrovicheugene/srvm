//==============================================================
#ifndef ZCORRELATIONPLOTWIDGET_H
#define ZCORRELATIONPLOTWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class ZPlotter;

//==============================================================
class ZCorrelationPlotterWidget : public QWidget
{
   Q_OBJECT
public:
   explicit ZCorrelationPlotterWidget(QWidget *parent = 0);

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
#endif // ZCORRELATIONPLOTWIDGET_H
