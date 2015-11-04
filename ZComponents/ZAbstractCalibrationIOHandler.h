//================================================================
#ifndef ZABSTRACTCALIBRATIONIOHANDLER_H
#define ZABSTRACTCALIBRATIONIOHANDLER_H
//================================================================
#include <QObject>
#include <QFile>
#include "ZAbstractCalibration.h"
//================================================================

//================================================================
class ZAbstractCalibrationIOHandler : public QObject
{
    Q_OBJECT
public:
    explicit ZAbstractCalibrationIOHandler(QObject* calibrationParent, QObject *parent = 0);
    virtual ~ZAbstractCalibrationIOHandler();

    virtual  bool zp_getCalibrationFromFile(QFile &, ZAbstractCalibration*&) = 0;
    virtual  bool zp_getCalibrationFromFile(const QString& path, ZAbstractCalibration*&) = 0;

signals:

    void zg_message(QString) const;

public slots:

protected:

    //VARS
    QObject* zv_calibrationParent;


};
//================================================================
#endif // ZABSTRACTCALIBRATIONIOHANDLER_H
