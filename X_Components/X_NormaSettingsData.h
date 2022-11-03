//========================================================
#ifndef X_NORMASETTINGSDATA_H
#define X_NORMASETTINGSDATA_H
//========================================================
#include "X_Calibration.h"
//========================================================
class X_NormaSettingsData
{
public:
    X_NormaSettingsData();
    explicit X_NormaSettingsData(const X_Calibration* calibration);

    // VARS
    X_TermNormalizer::NormaType normaType;
    QString customNormaString;
    QList<QPair<qint64, QString> > termList;


};
//========================================================
Q_DECLARE_METATYPE(X_NormaSettingsData)
#endif // X_NORMASETTINGSDATA_H
