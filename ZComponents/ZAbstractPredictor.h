//============================================================
#ifndef ZABSTRACTPREDICTOR_H
#define ZABSTRACTPREDICTOR_H
//============================================================
#include <QObject>
//============================================================
class ZAbstractPredictor : public QObject
{
   Q_OBJECT
public:
   explicit ZAbstractPredictor(QObject *parent = 0);

signals:

public slots:


private:

   // VARS
   qreal zv_K;


   // FUNCS



};
//============================================================
#endif // ZABSTRACTPREDICTOR_H
