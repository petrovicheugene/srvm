//========================================================
#ifndef ZEQUATIONSETTINGSDATA_H
#define ZEQUATIONSETTINGSDATA_H
//========================================================
#include "ZCalibration.h"
//========================================================
class ZEquationSettingsData
{
public:
    ZEquationSettingsData();
    ZEquationSettingsData(const ZCalibration* calibration);

    // VARS

    ZCalibration::EquationType equationType;
    qint64 baseTermId;
    QList<QPair<qint64, QString> > termList;

};
Q_DECLARE_METATYPE(ZEquationSettingsData)
//========================================================
#endif // ZEQUATIONSETTINGSDATA_H
