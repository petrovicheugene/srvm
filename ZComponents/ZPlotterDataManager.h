//===========================================================
#ifndef ZPLOTTERDATAMANAGER_H
#define ZPLOTTERDATAMANAGER_H
//===========================================================
#include <QObject>
#include <QGraphicsItem>
#include "ZSpectrumArrayRepository.h"
#include "ZCalibrationRepository.h"
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
   void zp_connectToCalibrationRepository(ZCalibrationRepository*);

   void zp_connectToPlotter(ZPlotter* plotter);

signals:


public slots:

   void zp_currentCalibrationChanged(qint64 currentCalibrationId, int currentCalibrationIndex);
   void zp_currentCalibrationWindowChanged(qint64 currentWindowId, int currentWindowIndex,
                                           qint64 previousWindowId, int previousWindowIndex);
   void zp_currentArrayChanged(qint64 currentArrayId, int currentArrayIndex);
   void zp_currentSpectrumChanged(qint64 currentSpectrumId, int currentSpectrumIndex,
                                  qint64 previousSpectrumId, int previousSpectrumIndex);

private:

   // VARS
   QAction* zv_switchRuleMetrixAction;
   int zv_currentArrayIndex;
   qint64 zv_currentArrayId;
   qint64 zv_currentCalibrationId;
   int zv_currentCalibrationIndex;

   ZSpectrumArrayRepository* zv_spectrumArrayRepositiry;
   ZCalibrationRepository* zv_calibrationRepository;
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
   void zh_setCurrentWindowId(qint64);
   void zh_setPlotterVerticalAbsMax(qreal maxIntensity);

private slots:

   void zh_onRepositoryArrayOperation(ZSpectrumArrayRepository::SpectrumOperationType type,
                                      int arrayIndex, int first, int last);
   void zh_onRepositoryCalibrationWindowOperation(ZCalibrationRepository::WindowOperationType,
                                                  int calibrationIndex, int first, int last);
   void zh_switchRuleMetrix(bool toggled);
   void zh_changeEnergyCalibrationOnRule(qint64 arrayId);
   void zh_onArrayMaxParametersChanged(int arrayId, int maxIntensity, int channelCount);
   void zh_setMaxParametersToDefaultItem(qreal, qreal);
   void zh_definePlotScaling(bool& plotIsScaled);
   void zh_findItemInCursorAreaImage(QImage cursorAreaImage);

};
//===========================================================
#endif // ZPLOTTERDATAMANAGER_H
