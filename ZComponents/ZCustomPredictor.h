//===================================================================
#ifndef ZCUSTOMPREDICTOR_H
#define ZCUSTOMPREDICTOR_H
//===================================================================
#include "ZAbstractPredictor.h"
//===================================================================
class ZCustomPredictor : public ZAbstractPredictor
{
   Q_OBJECT
public:
   explicit ZCustomPredictor(ZCalibration* parent);

   // FUNCS
   qreal zp_calcValue() override;
   QString zp_predictorName() override;

};
//===================================================================
#endif // ZCUSTOMPREDICTOR_H
