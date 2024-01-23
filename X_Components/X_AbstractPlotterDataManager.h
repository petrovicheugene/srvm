//===========================================================
#ifndef X_PLOTTERDATAMANAGER_H
#define X_PLOTTERDATAMANAGER_H
//===========================================================
#include <QObject>
#include <QGraphicsItem>
#include "X_SpectrumArrayRepository.h"
#include "X_CalibrationRepository.h"
//===========================================================
class X_Plotter;
class X_DefaultRectGraphicsItem;

typedef int EnergyLineOperationType;
//===========================================================
class X_AbstractPlotterDataManager : public QObject
{
    Q_OBJECT
public:
    explicit X_AbstractPlotterDataManager(QObject *parent = 0);

    // FUNCS
    void xp_connectToSpectraArrayRepository(X_SpectrumArrayRepository*);
    void xp_connectToCalibrationRepository(X_CalibrationRepository*);
    void xp_connectToPlotter(X_Plotter* plotter);

signals:

    void xg_requestCurrentMeasuringConditions(quint8 gainFactor, int exposition);

    void xg_requestEnergyLineEnergyValue(const QString& elementSymbol,
                                         const QString& lineName, double& energyValue);
    void xg_requestEnergyLineRelativeIntensity(const QString& elementSymbol,
                                               const QString& lineName, int& reletiveIntensity);

    void xg_requestEnergyLineVisibility(QString elementSymbol, QString lineName, bool& visibility);
    void xg_requestEnergyLineColor(QString elementSymbol, QString lineName, QColor& color);



public slots:


    void xp_onEnergyLineOperation(QString elementSymbol, QString lineName,
                                  EnergyLineOperationType operationType);
    void xp_onPlotterViewPortRectChange(QRectF rect);

private slots:

    void xh_updateRulerTool(QPointF startPoint, QPointF endPoint, bool visibility);
    void xh_onCurrentEnergyCalibrationChange(QList<double> calibrationFactors);
    void xh_updateEnergyLines();

private:

    // VARS
    QAction* xv_switchRuleMetrixAction;
    int xv_currentArrayIndex;
    qint64 xv_currentArrayId;
    qint64 xv_currentCalibrationId;
    int xv_currentCalibrationIndex;

    X_SpectrumArrayRepository* xv_spectrumArrayRepositiry;
    X_CalibrationRepository* xv_calibrationRepository;
    X_Plotter* xv_plotter;
    X_DefaultRectGraphicsItem* xv_defaultItem;

    QString xv_verticalRuleLabel;
    QString xv_horizontalRuleLabel;
    QString xv_horizontalRecalcedRuleLabel;
    bool xv_energyRuleMetrixFlag;

    qreal xv_boundingRectTopFactor = 0.0;
    QRectF xv_defaultSceneRect = QRectF(QPointF(0.0,-100.0), QPointF(2048.0, 0.0));
    //QList<QGraphicsItem*> xv_spectrumGraphicsItemList;
    QList<double> xv_calibrationFactors;

    // FUNCS
    void xh_createComponents();
    void xh_createConnections();
    void xh_setCurrentWindowId(qint64);
    void xh_setPlotterVerticalAbsMax(qreal maxIntensity);

private slots:

    void xh_onRepositoryArrayOperation(X_SpectrumArrayRepository::SpectrumOperationType type,
                                       int arrayIndex, int first, int last);
    void xh_onRepositoryCalibrationWindowOperation(X_CalibrationRepository::WindowOperationType,
                                                   int calibrationIndex, int first, int last);
    void xh_switchRuleMetrix(bool toggled);
    //void xh_updateEnergyCalibrationOnRule(qint64 arrayId);
    void xh_onArrayMaxParametersChanged(int arrayId, int maxIntensity, int channelCount);
    void xh_setMaxParametersToDefaultItem(qreal, qreal);
    void xh_definePlotScaling(bool& plotIsScaled);
    void xh_findItemInCursorAreaImage(QImage cursorAreaImage);

    void xp_currentCalibrationChanged(qint64 currentCalibrationId, int currentCalibrationIndex);
    void xp_currentCalibrationWindowChanged(qint64 currentWindowId, int currentWindowIndex,
                                            qint64 previousWindowId, int previousWindowIndex);
    void xp_currentArrayChanged(qint64 currentArrayId, int currentArrayIndex);
    void xp_currentSpectrumChanged(qint64 currentSpectrumId, int currentSpectrumIndex,
                                   qint64 previousSpectrumId, int previousSpectrumIndex);

    bool xh_convertEnergyToChannel(double energyValue, double& channel);
    void xh_updateRuleMetrix();

};
//===========================================================
#endif // X_PLOTTERDATAMANAGER_H
