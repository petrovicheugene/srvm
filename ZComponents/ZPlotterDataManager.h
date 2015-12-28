//===========================================================
#ifndef ZPLOTTERDATAMANAGER_H
#define ZPLOTTERDATAMANAGER_H
//===========================================================
#include <QObject>
#include <QGraphicsItem>
#include "ZSpectrumArrayRepository.h"

//===========================================================
class ZPlotter;
//===========================================================
class ZPlotterDataManager : public QObject
{
    Q_OBJECT
public:
    explicit ZPlotterDataManager(QObject *parent = 0);

    // FUNCS
    void zp_connectToSpectraArrayRepository(ZSpectrumArrayRepository*);
    void zp_connectToPlotter(ZPlotter* plotter);

signals:


public slots:

    void zp_currentArrayChanged(qint64 currentArrayId, int currentArrayIndex);

private:

    // VARS
    QAction* zv_switchRuleMetrixAction;
    int zv_currentArrayIndex;
    qint64 zv_currentArrayId;
    ZSpectrumArrayRepository* zv_spectrumArrayRepositiry;
    ZPlotter* zv_plotter;

    QString zv_verticalRuleLabel;
    QString zv_horizontalRuleLabel;
    QString zv_horizontalRecalcedRuleLabel;

    qreal zv_boundingRectTopFactor;
    QRectF zv_defaultSceneRect = QRectF(QPointF(0.0,-100.0), QPointF(2048.0, 0.0));
    //QList<QGraphicsItem*> zv_spectrumGraphicsItemList;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

private slots:

    void zh_onRepositoryArrayOperation(ZSpectrumArrayRepository::SpectrumOperationType type,
                                       int arrayIndex, int first, int last);
    void zh_switchRuleMetrix(bool toggled);
    void zh_changeEnergyCalibrationOnRule(qint64 arrayId);
    void zh_onArrayMaxParametersChanged(int arrayId, int maxIntensity, int channelCount);
    void zh_setMaxParametersToDefaultItem(qreal, qreal);
    void zh_definePlotScaling(bool& plotIsScaled);

};
//===========================================================
#endif // ZPLOTTERDATAMANAGER_H
