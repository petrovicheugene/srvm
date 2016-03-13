//=====================================================================
#include "ZCorrelationPlotterManager.h"
#include "ZPlotter.h"
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
//=====================================================================
ZCorrelationPlotterManager::ZCorrelationPlotterManager(QObject *parent) : QObject(parent)
{
    zv_plotter = 0;

    zv_verticalRuleLabel = tr("Term value");
    zv_horizontalRuleLabel = tr("Concentration");
    zh_createActions();
}
//=====================================================================
void ZCorrelationPlotterManager::zp_connectToPlotter(ZPlotter* plotter)
{
    zv_plotter = plotter;
    zv_plotter->zp_setAutoDefineVerticalAbsMax(false);

    zv_plotter->zp_appendWidgetToDashboard(zh_createSourceComboBoxWidget(), Qt::AlignLeft);

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
void ZCorrelationPlotterManager::zh_createActions()
{
     zv_showTermCorrelationAction = new QAction(this);
     zv_showTermCorrelationAction->setText(tr("Term correlation"));
     zv_sourceChooseActionList.append(zv_showTermCorrelationAction);

     zv_showCalibrationConcentrationToChemConcentrationAction = new QAction(this);
     zv_showCalibrationConcentrationToChemConcentrationAction->setText(tr("Calibration concentration"));
     zv_sourceChooseActionList.append(zv_showCalibrationConcentrationToChemConcentrationAction);

}
//=====================================================================
QWidget* ZCorrelationPlotterManager::zh_createSourceComboBoxWidget() const
{
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);
    widget->setLayout(layout);
    layout->setMargin(0);

    QLabel* label = new QLabel();
    label->setText(tr("Display:"));
    layout->addWidget(label);

    QComboBox* sourceComboBox = new QComboBox();
    sourceComboBox->addActions(zv_sourceChooseActionList);
    sourceComboBox->setCurrentIndex(0);
    layout->addWidget(sourceComboBox);

    return widget;
}
//=====================================================================
