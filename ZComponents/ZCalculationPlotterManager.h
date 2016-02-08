//=====================================================================
#ifndef ZCALCULATIONPLOTTERMANAGER_H
#define ZCALCULATIONPLOTTERMANAGER_H
//=====================================================================
#include <QObject>
//=====================================================================
class ZPlotter;
//=====================================================================
class ZCalculationPlotterManager : public QObject
{
   Q_OBJECT
public:
   explicit ZCalculationPlotterManager(QObject *parent = 0);

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
#endif // ZCALCULATIONPLOTTERMANAGER_H
