//============================================================
#ifndef ZABSTRACTPREDICTOR_H
#define ZABSTRACTPREDICTOR_H
//============================================================
#include <QObject>
#include "ZPredictorNormalizator.h"
#include "ZCalibration.h"
//============================================================
class ZAbstractSpectrum;
class ZCalibrationWindow;
class ZPredictorNormalizer;
//============================================================
class ZAbstractPredictor : public QObject
{
   Q_OBJECT
public:
   explicit ZAbstractPredictor(ZCalibration *parent);
   // FUNCS
   virtual qreal zp_calcValue() = 0;
   virtual QString zp_predictorName() = 0;

signals:

   void zg_isNormalizerValid(bool&);
   void zg_normalizerValue(qreal&);
   void zg_predictorChanged();
   void zg_requestForDelete(ZAbstractPredictor*);

public slots:


private slots:

   void zh_normalizerChanged();

private:

   // VARS
   qreal zv_K;

   // FUNCS
   void zh_connectToNormalizer(ZPredictorNormalizer *);
   void zh_connectToCalibration(ZCalibration* calibration);
};
//============================================================
#endif // ZABSTRACTPREDICTOR_H
