//============================================================
#ifndef ZABSTRACTPREDICTOR_H
#define ZABSTRACTPREDICTOR_H
//============================================================
#include <QObject>
#include "ZPredictorNormalizator.h"
//============================================================
class ZAbstractSpectrum;
class ZCalibrationWindow;
class ZPredictorNormalizer;
class ZCalibration;

//============================================================
class ZAbstractPredictor : public QObject
{
   Q_OBJECT
public:
   explicit ZAbstractPredictor(ZCalibration*);

   // VARS
   enum PredictorType {PT_NOT_DEFINED,
                       PT_SIMPLE,
                       PT_QUADRATIC,
                       PT_MIXED,
                       PT_CUSTOM};

   // FUNCS
   virtual qreal zp_calcValue() = 0;
   virtual QString zp_predictorName();
   virtual ZAbstractPredictor::PredictorType zp_predictorType();

signals:

   void zg_isNormalizerValid(bool&);
   void zg_normalizerValue(qreal&);
   void zg_predictorChanged();
   void zg_requestForDelete(ZAbstractPredictor*);

public slots:


protected slots:

   void zh_normalizerChanged();
   void zh_onWindowDestroying();

protected:

   // VARS
   PredictorType zv_type;
   qreal zv_K;

   QString zv_name;
   // FUNCS
   void zh_connectToNormalizer(ZPredictorNormalizer *);
   void zh_connectToCalibration(ZCalibration* calibration);
};
//============================================================
#endif // ZABSTRACTPREDICTOR_H
