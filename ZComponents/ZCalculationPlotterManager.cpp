//=====================================================================
#include "ZCalculationPlotterManager.h"
#include "ZPlotter.h"

//=====================================================================
ZCalculationPlotterManager::ZCalculationPlotterManager(QObject *parent) : QObject(parent)
{
   zv_plotter = 0;

   zv_verticalRuleLabel = tr("Term value");
   zv_horizontalRuleLabel = tr("Concentration");
}
//=====================================================================
void ZCalculationPlotterManager::zp_connectToPlotter(ZPlotter* plotter)
{
   zv_plotter = plotter;
   zv_plotter->zp_setAutoDefineVerticalAbsMax(false);
//   QList<QAction*> actionList;
//   actionList << zv_switchRuleMetrixAction;
//   zv_plotter->zp_appendButtonsToDashboard(actionList, Qt::AlignLeft, 0);

//   connect(zv_plotter, &ZPlotter::zg_cursorAreaImage,
//           this, &ZPlotterDataManager::zh_findItemInCursorAreaImage);

   // rule settings // No label on right and top
   zv_plotter->zp_setLeftMarkRecalcFlag(false);
   zv_plotter->zp_setLeftRuleLabel(zv_verticalRuleLabel);
   zv_plotter->zp_setLeftRuleLabelVisible(true);

   zv_plotter->zp_setRightMarkRecalcFlag(false);
   zv_plotter->zp_setRightRuleLabelVisible(false);
   zv_plotter->zp_setRightRuleVisible(false);

   zv_plotter->zp_setTopMarkRecalcFlag(false);
   zv_plotter->zp_setTopRuleLabelVisible(false);
   zv_plotter->zp_setTopRuleVisible(false);

   zv_plotter->zp_setBottomMarkRecalcFlag(false);
   zv_plotter->zp_setBottomRuleLabel(zv_horizontalRuleLabel);
   zv_plotter->zp_setBottomRuleLabelVisible(true);

   zv_plotter->zp_setDistortionSliderVisible(false);

//   if(zv_plotter->zp_itemCount() <= 0)
//   {
//      ZDefaultRectGraphicsItem* defaultItem = new ZDefaultRectGraphicsItem(zv_defaultSceneRect);
//      zv_plotter->zp_addItem(defaultItem);
//   }
}
//=====================================================================