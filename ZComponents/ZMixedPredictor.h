//===================================================================
#ifndef ZMIXEDPREDICTOR_H
#define ZMIXEDPREDICTOR_H
//===================================================================
#include "ZAbstractPredictor.h"
//===================================================================
class ZCalibrationWindow;
//===================================================================
class ZMixedPredictor : public ZAbstractPredictor
{
   Q_OBJECT
public:
   explicit ZMixedPredictor(const ZCalibrationWindow &window1,
                            const ZCalibrationWindow &window2,
                            ZCalibration *parent);

   // FUNCS
   qreal zp_calcValue() override;
   QString zp_predictorName() override;

private:

   // VARS

};
//===================================================================
#endif // ZMIXEDPREDICTOR_H
