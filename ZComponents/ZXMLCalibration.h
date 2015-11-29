//==========================================================
#ifndef ZXMLCALIBRATION_H
#define ZXMLCALIBRATION_H
//==========================================================
#include "ZAbstractCalibration.h"

//==========================================================

//==========================================================
class ZXMLCalibration : public ZAbstractCalibration
{
    Q_OBJECT
public:

    explicit ZXMLCalibration(const QString& path, QObject* parent = 0);
    virtual ~ZXMLCalibration();

    double zp_calcConcentration(ZAbstractSpectrum* const spectrum, bool *ok = 0) override;

};
//==========================================================
#endif // ZXMLCALIBRATION_H
