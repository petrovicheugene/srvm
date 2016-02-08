//======================================================================
#ifndef ZTERMNORMALIZATOR_H
#define ZTERMNORMALIZATOR_H
//======================================================================
#include <QObject>
//======================================================================
class ZTermNormalizer : public QObject
{
   Q_OBJECT
public:
   explicit ZTermNormalizer(QObject *parent = 0);

signals:

   void zg_normalizerChanged() const;

public slots:

   void zp_isValid(bool&) const;
   void zp_value(qreal&) const;
};
//======================================================================
#endif // ZTERMNORMALIZATOR_H
