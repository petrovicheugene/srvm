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
   enum PredictorType {PT_NOT_DEFINED = 0,
                       PT_SIMPLE = 1,
                       PT_QUADRATIC = 2,
                       PT_MIXED = 3,
                       PT_CUSTOM = 4};

   // FUNCS
   virtual qreal zp_calcValue() = 0;
   virtual QString zp_predictorName() = 0;
   virtual ZAbstractPredictor::PredictorType zp_predictorType();

signals:

   void zg_isNormalizerValid(bool&);
   void zg_normalizerValue(qreal&);
   void zg_predictorChanged();
   void zg_requestForDelete(ZAbstractPredictor*);

public slots:


private slots:

   void zh_normalizerChanged();


protected:

   // VARS
   PredictorType zv_type;
   qreal zv_K;

   // FUNCS
   void zh_connectToNormalizer(ZPredictorNormalizer *);
   void zh_connectToCalibration(ZCalibration* calibration);
};
//============================================================
#endif // ZABSTRACTPREDICTOR_H
