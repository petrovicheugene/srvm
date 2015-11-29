//==========================================================
#ifndef ZXMLCALIBRATIONARRAYIOHANDLER_H
#define ZXMLCALIBRATIONARRAYIOHANDLER_H
//==========================================================
#include "ZAbstractCalibrationIOHandler.h"
#include "globalVariables.h"
//==========================================================
class QXmlStreamReader;
//==========================================================
class ZXMLCalibrationIOHandler : public ZAbstractCalibrationIOHandler
{
    Q_OBJECT
public:
    explicit ZXMLCalibrationIOHandler(QObject* calibrationParent, QObject *parent = 0);
    virtual ~ZXMLCalibrationIOHandler();

    virtual  bool zp_getCalibrationFromFile(QFile&, ZAbstractCalibration*&) override;
    virtual  bool zp_getCalibrationFromFile(const QString&, ZAbstractCalibration*&) override;

public slots:


signals:



private:

    // VARS
    const QString zv_magicString = glAppCompany + " 7D385RTNOW9SH31JZQL";
    // elment names
    const QString zv_ROOT = "root";
    const QString zv_TYPE = "type";
    const QString zv_CHEMELEMENT = "chemelement";
    const QString zv_NAME = "name";
    const QString zv_CALIBRATION = "calibration";

    const QString zv_YES = "yes";
    const QString zv_NO = "no";

    // FUNCS
    bool zh_detectRoot(const QXmlStreamReader& reader, bool& magicStringDetectionFlag) const;
//    bool zh_parseXMLElement(ZAbstractCalibration*&,
//                            int currentArrayIndex,
//                            QXmlStreamReader&);
    bool zh_checkfilePath(const QString& path) const;


};
//==========================================================
#endif // ZXMLCALIBRATIONARRAYIOHANDLER_H
