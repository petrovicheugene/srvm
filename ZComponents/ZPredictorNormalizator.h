//======================================================================
#ifndef ZPREDICTORNORMALIZATOR_H
#define ZPREDICTORNORMALIZATOR_H
//======================================================================
#include <QObject>
//======================================================================
class ZPredictorNormalizer : public QObject
{
   Q_OBJECT
public:
   explicit ZPredictorNormalizer(QObject *parent = 0);

signals:

   void zg_normalizerChanged() const;

public slots:

   void zp_isValid(bool&) const;
   void zp_value(qreal&) const;
};
//======================================================================
#endif // ZPREDICTORNORMALIZATOR_H
