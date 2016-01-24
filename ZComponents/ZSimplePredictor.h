//===================================================================
#ifndef ZSIMPLEPREDICTOR_H
#define ZSIMPLEPREDICTOR_H
//===================================================================
#include "ZAbstractPredictor.h"
//===================================================================
class ZCalibrationWindow;
//===================================================================
class ZSimplePredictor : public ZAbstractPredictor
{
   Q_OBJECT
public:
   explicit ZSimplePredictor(QObject *parent = 0);


};
//===================================================================
#endif // ZSIMPLEPREDICTOR_H
