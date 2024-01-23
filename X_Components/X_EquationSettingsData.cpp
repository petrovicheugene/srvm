//========================================================
#include "X_EquationSettingsData.h"
//========================================================
X_EquationSettingsData::X_EquationSettingsData()
{
    equationType = X_Calibration::ET_NOT_DEFINED;
    baseTermId = -1;
}
//========================================================
X_EquationSettingsData::X_EquationSettingsData(const X_Calibration *calibration)
{
    equationType = calibration->xp_equationType();
    baseTermId = calibration->xp_baseTermId();
    fractionalBaseNormaSettingsData.normaType = calibration->xp_baseTermNormaType();
    fractionalBaseNormaSettingsData.customNormaString = calibration->xp_baseTermNormaCustomString();

    QPair<qint64, QString> termData;
    for(int t = 0; t < calibration->xp_termCount(); t++)
    {
        termData.first = calibration->xp_termId(t);
        termData.second = calibration->xp_termName(t);
        termList.append(termData);
    }
}
//========================================================
