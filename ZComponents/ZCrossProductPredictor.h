//===================================================================
#ifndef ZCrossProductPredictor_H
#define ZCrossProductPredictor_H
//===================================================================
#include "ZAbstractPredictor.h"
//===================================================================
class ZCalibrationWindow;
//===================================================================
class ZCrossProductPredictor : public ZAbstractPredictor
{
   Q_OBJECT
public:
   explicit ZCrossProductPredictor(const ZCalibrationWindow &window1,
                            const ZCalibrationWindow &window2,
                            ZCalibration *parent);

   // FUNCS
   qreal zp_calcValue() override;
   QString zp_predictorName() override;

private:

   // VARS

};
//===================================================================
#endif // ZCrossProductPredictor_H
