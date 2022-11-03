//========================================================
#include "X_NormaSettingsData.h"
//========================================================
X_NormaSettingsData::X_NormaSettingsData()
{
    normaType = X_TermNormalizer::NT_NONE;
}
//========================================================
X_NormaSettingsData::X_NormaSettingsData(const X_Calibration *calibration)
{
    normaType = calibration->xp_normaType();
    customNormaString = calibration->xp_normaCustomString();

    QPair<qint64, QString> termData;
    for(int t = 0; t < calibration->xp_termCount(); t++)
    {
        termData.first = calibration->xp_termId(t);
        termData.second = calibration->xp_termName(t);
        termList.append(termData);
    }
}
//========================================================
