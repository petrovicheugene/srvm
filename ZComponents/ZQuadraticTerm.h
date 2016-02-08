//===================================================================
#ifndef ZQUADRATICTERM_H
#define ZQUADRATICTERM_H
//===================================================================
#include "ZAbstractTerm.h"
//===================================================================
class ZCalibrationWindow;
//===================================================================
class ZQuadraticTerm : public ZAbstractTerm
{
   Q_OBJECT
public:
   explicit ZQuadraticTerm(const ZCalibrationWindow* window,
                                ZCalibration *parent);

   // FUNCS
   qreal zp_calcValue() override;

private:

   // VARS


   // FUNCS

   void zh_connectToWindow(const ZCalibrationWindow* window);

};
//===================================================================
#endif // ZQUADRATICTERM_H
