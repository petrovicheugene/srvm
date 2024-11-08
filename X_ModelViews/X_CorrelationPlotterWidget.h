//==============================================================
#ifndef X_CORRELATIONPLOTWIDGET_H
#define X_CORRELATIONPLOTWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class X_Plotter;
class QVBoxLayout;
//==============================================================
class X_CorrelationPlotterWidget : public QWidget
{
   Q_OBJECT
public:
   explicit X_CorrelationPlotterWidget(QWidget *parent = 0);

   // FUNCS
   X_Plotter* xp_plotter();
   void xp_rebuildPlotter();

signals:

public slots:

protected:


private:


   // VARS
   QVBoxLayout* frameLayout = nullptr;
   X_Plotter* xv_plotter = nullptr;

   // FUNCS
   void xh_createComponents();
   void xh_createConnections();

};
//==============================================================
#endif // X_CORRELATIONPLOTWIDGET_H
