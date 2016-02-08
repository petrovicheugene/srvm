//===================================================================
#ifndef ZSIMPLETERM_H
#define ZSIMPLETERM_H
//===================================================================
#include "ZAbstractTerm.h"
//===================================================================
class ZCalibrationWindow;
//===================================================================
class ZSimpleTerm : public ZAbstractTerm
{
   Q_OBJECT
public:
   explicit ZSimpleTerm(const ZCalibrationWindow* window,
                             ZCalibration *parent);

   // FUNCS
   qreal zp_calcValue() override;

private:

   // VARS


   // FUNCS

   void zh_connectToWindow(const ZCalibrationWindow* window);

};
//===================================================================
#endif // ZSIMPLETERM_H
