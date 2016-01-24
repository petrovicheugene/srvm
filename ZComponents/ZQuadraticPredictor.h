//===================================================================
#ifndef ZQUADRATICPREDICTOR_H
#define ZQUADRATICPREDICTOR_H
//===================================================================
#include "ZAbstractPredictor.h"
//===================================================================
class ZQuadraticPredictor : public ZAbstractPredictor
{
   Q_OBJECT
public:
   explicit ZQuadraticPredictor(QObject* parent = 0);
};
//===================================================================
#endif // ZQUADRATICPREDICTOR_H
