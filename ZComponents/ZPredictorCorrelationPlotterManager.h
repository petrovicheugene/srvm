//=====================================================================
#ifndef ZPREDICTORCORRELATIONPLOTTERMANAGER_H
#define ZPREDICTORCORRELATIONPLOTTERMANAGER_H
//=====================================================================
#include <QObject>
//=====================================================================
class ZPlotter;
//=====================================================================
class ZPredictorCorrelationPlotterManager : public QObject
{
   Q_OBJECT
public:
   explicit ZPredictorCorrelationPlotterManager(QObject *parent = 0);

   // FUNCS
   void zp_connectToPlotter(ZPlotter* plotter);


signals:

public slots:

private:
   // VARS
   ZPlotter* zv_plotter;

   QString zv_verticalRuleLabel;
   QString zv_horizontalRuleLabel;

};
//=====================================================================
#endif // ZPREDICTORCORRELATIONPLOTTERMANAGER_H
