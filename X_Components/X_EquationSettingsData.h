//========================================================
#ifndef X_EQUATIONSETTINGSDATA_H
#define X_EQUATIONSETTINGSDATA_H
//========================================================
#include "X_Calibration.h"
#include "X_NormaSettingsData.h"
//========================================================
class X_EquationSettingsData
{
public:
    X_EquationSettingsData();
    explicit X_EquationSettingsData(const X_Calibration* calibration);

    // VARS

    X_Calibration::EquationType equationType;
    qint64 baseTermId;
    QList<QPair<qint64, QString> > termList;
    X_NormaSettingsData fractionalBaseNormaSettingsData;
};
Q_DECLARE_METATYPE(X_EquationSettingsData)
//========================================================
#endif // X_EQUATIONSETTINGSDATA_H
