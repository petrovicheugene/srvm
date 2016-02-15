//===================================================================
#ifndef ZCROSSPRODUCTTERM_H
#define ZCROSSPRODUCTTERM_H
//===================================================================
#include "ZAbstractTerm.h"
//===================================================================
class ZCalibrationWindow;
//===================================================================
class ZCrossProductTerm : public ZAbstractTerm
{
   Q_OBJECT
public:
   explicit ZCrossProductTerm(const ZCalibrationWindow &window1,
                            const ZCalibrationWindow &window2,
                            ZCalibration *parent);

   // FUNCS
   qreal zp_calcValue() override;
   QString zp_termName() const override;

private:

   // VARS

};
//===================================================================
#endif // ZCROSSPRODUCTTERM_H
