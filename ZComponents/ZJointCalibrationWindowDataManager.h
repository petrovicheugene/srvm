//=============================================================================
#ifndef ZJOINTCALIBRATIONWINDOWDATAMANAGER_H
#define ZJOINTCALIBRATIONWINDOWDATAMANAGER_H
//=============================================================================
#include <QObject>
#include "ZSpectraArrayRepository.h"
#include "ZCalibrationRepository.h"
//=============================================================================

//=============================================================================
class ZJointCalibrationWindowDataManager : public QObject
{
    Q_OBJECT
public:
    explicit ZJointCalibrationWindowDataManager(QObject *parent = 0);

    // FUNCS
    void zp_connectToSpectraArrayRepository(ZSpectrumArrayRepository*);
    void zp_connectToCalibrationRepository(ZCalibrationRepository*);

    int zp_rowCount() const;
    int zp_columnCount() const;
    QString zp_columnName(int) const;


signals:

public slots:
};
//=============================================================================
#endif // ZJOINTCALIBRATIONWINDOWDATAMANAGER_H
