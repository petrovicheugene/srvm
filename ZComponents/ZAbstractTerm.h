//============================================================
#ifndef ZABSTRACTTERM_H
#define ZABSTRACTTERM_H
//============================================================
#include <QObject>
#include "ZTermNormalizer.h"
//============================================================
class ZAbstractSpectrum;
class ZCalibrationWindow;
class ZTermNormalizer;
class ZCalibration;

//============================================================
class ZAbstractTerm : public QObject
{
   Q_OBJECT
public:
   explicit ZAbstractTerm(ZCalibration*);

   // VARS
   enum TermType {PT_NOT_DEFINED,
                       PT_SIMPLE,
                       PT_QUADRATIC,
                       PT_MIXED,
                       PT_CUSTOM};

   // FUNCS
   virtual qreal zp_calcValue() = 0;
   virtual QString zp_termName();
   virtual ZAbstractTerm::TermType zp_termType();

signals:

   void zg_isNormalizerValid(bool&);
   void zg_normalizerValue(qreal&);
   void zg_termChanged();
   void zg_requestForDelete(ZAbstractTerm*);

public slots:


protected slots:

   void zh_normalizerChanged();
   void zh_onWindowDestroying();

protected:

   // VARS
   TermType zv_type;
   qreal zv_K;

   QString zv_name;
   // FUNCS
   void zh_connectToNormalizer(ZTermNormalizer *);
   void zh_connectToCalibration(ZCalibration* calibration);
};
//============================================================
#endif // ZABSTRACTTERM_H
