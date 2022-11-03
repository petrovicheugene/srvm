//============================================
#ifndef X_CALIBRATIONQUALITYDATA_H
#define X_CALIBRATIONQUALITYDATA_H
//============================================
#include <QString>
//============================================
class X_CalibrationQualityData
{
public:
    X_CalibrationQualityData();

    QString determination;
    QString adj_determination;
    QString squareSigma;
    QString standardDeviation;

};
//============================================
#endif // X_CALIBRATIONQUALITYDATA_H
