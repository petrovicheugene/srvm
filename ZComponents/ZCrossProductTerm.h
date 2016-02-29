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
    bool zp_calcValue(const ZAbstractSpectrum*, qreal&) override;
    bool zp_calcTermVariablePart(const ZAbstractSpectrum*, qint64 &value) override; // w/o factor
    bool zp_termBelongsToWindow(const ZCalibrationWindow*) const override;

   QString zp_termName() const override;

private:

   // VARS

};
//===================================================================
#endif // ZCROSSPRODUCTTERM_H
