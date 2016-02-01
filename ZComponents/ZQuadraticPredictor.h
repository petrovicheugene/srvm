//===================================================================
#ifndef ZQUADRATICPREDICTOR_H
#define ZQUADRATICPREDICTOR_H
//===================================================================
#include "ZAbstractPredictor.h"
//===================================================================
class ZCalibrationWindow;
//===================================================================
class ZQuadraticPredictor : public ZAbstractPredictor
{
   Q_OBJECT
public:
   explicit ZQuadraticPredictor(const ZCalibrationWindow* window,
                                ZCalibration *parent);

   // FUNCS
   qreal zp_calcValue() override;

private:

   // VARS


   // FUNCS

   void zh_connectToWindow(const ZCalibrationWindow* window);

};
//===================================================================
#endif // ZQUADRATICPREDICTOR_H
