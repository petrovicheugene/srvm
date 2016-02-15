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
   qreal zp_calcValue() override;
   QString zp_termName() const override;

};
//===================================================================
#endif // ZCUSTOMTERM_H
