//===================================================================
#ifndef ZSIMPLEPREDICTOR_H
#define ZSIMPLEPREDICTOR_H
//===================================================================
#include "ZAbstractPredictor.h"
//===================================================================
class ZCalibrationWindow;
//===================================================================
class ZSimplePredictor : public ZAbstractPredictor
{
   Q_OBJECT
public:
   explicit ZSimplePredictor(const ZCalibrationWindow& window,
                             ZCalibration *parent);

   // FUNCS
   qreal zp_calcValue() override;
   QString zp_predictorName() override;

private:

   // VARS

};
//===================================================================
#endif // ZSIMPLEPREDICTOR_H
