//=====================================================================
#ifndef ZCORRELATIONPLOTTERMANAGER_H
#define ZCORRELATIONPLOTTERMANAGER_H
//=====================================================================
#include <QObject>
#include <QAction>
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

    // Actions
    QList<QAction*> zv_sourceChooseActionList;
    QAction* zv_showTermCorrelationAction;
    QAction* zv_showCalibrationConcentrationToChemConcentrationAction;


    QString zv_verticalRuleLabel;
    QString zv_horizontalRuleLabel;

    // FUNCS
    void zh_createActions();
    QWidget* zh_createSourceComboBoxWidget() const;

};
//=====================================================================
#endif // ZCORRELATIONPLOTTERMANAGER_H
