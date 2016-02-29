//=====================================================================
#ifndef ZCORRELATIONPLOTTERMANAGER_H
#define ZCORRELATIONPLOTTERMANAGER_H
//=====================================================================
#include <QObject>
//=====================================================================
class ZPlotter;
//=====================================================================
class ZCorrelationPlotterManager : public QObject
{
   Q_OBJECT
public:
   explicit ZCorrelationPlotterManager(QObject *parent = 0);

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
#endif // ZCORRELATIONPLOTTERMANAGER_H
