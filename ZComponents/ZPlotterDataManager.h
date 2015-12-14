//===========================================================
#ifndef ZPLOTTERDATAMANAGER_H
#define ZPLOTTERDATAMANAGER_H
//===========================================================
#include <QObject>
#include <QGraphicsItem>
#include "ZSpectraArrayRepository.h"

//===========================================================
class ZPlotter;
//===========================================================
class ZPlotterDataManager : public QObject
{
    Q_OBJECT
public:
    explicit ZPlotterDataManager(QObject *parent = 0);

    // FUNCS
    void zp_connectToSpectraArrayRepository(ZSpectraArrayRepository*);
    void zp_connectToPlotter(ZPlotter* plotter);

signals:


public slots:

    void zp_currentArrayChanged(qint64 currentArrayId, int currentArrayIndex);

private:

    // VARS
    QAction* zv_switchRuleMetrixAction;
    int zv_currentArrayIndex;
    qint64 zv_currentArrayId;
    ZSpectraArrayRepository* zv_spectrumArrayRepositiry;
    ZPlotter* zv_plotter;

    QString zv_verticalRuleLabel;
    QString zv_horizontalRuleLabel;
    QString zv_horizontalRecalcedRuleLabel;

    qreal zv_boundingRectTopFactor;
    //QList<QGraphicsItem*> zv_spectrumGraphicsItemList;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

private slots:

    void zh_onRepositoryArrayOperation(ZSpectraArrayRepository::SpectrumOperationType type,
                                       int arrayIndex, int first, int last);
    void zh_switchRuleMetrix(bool toggled);
    void zh_changeEnergyCalibrationOnRule(qint64 arrayId);
    void zh_onArrayMaxParametersChanged(int arrayId, int maxIntensity, int channelCount);

};
//===========================================================
#endif // ZPLOTTERDATAMANAGER_H
