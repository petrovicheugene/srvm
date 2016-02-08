//==============================================================
#ifndef ZCALCULATIONPLOTWIDGET_H
#define ZCALCULATIONPLOTWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class ZPlotter;

//==============================================================
class ZCalculationPlotterWidget : public QWidget
{
   Q_OBJECT
public:
   explicit ZCalculationPlotterWidget(QWidget *parent = 0);

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
#endif // ZCALCULATIONPLOTWIDGET_H
