//===================================================================
#ifndef ZCUSTOMTERM_H
#define ZCUSTOMTERM_H
//===================================================================
#include "ZAbstractTerm.h"
//===================================================================
class ZCustomTerm : public ZAbstractTerm
{
   Q_OBJECT
public:
   explicit ZCustomTerm(ZCalibration* parent);

   // FUNCS
    bool zp_calcValue(const ZAbstractSpectrum*, qreal&) override;
    bool zp_calcTermVariablePart(const ZAbstractSpectrum*, qint64& value) override; // w/o factor
    bool zp_termBelongsToWindow(const ZCalibrationWindow*) const override;

   QString zp_termName() const override;

};
//===================================================================
#endif // ZCUSTOMTERM_H
