//==========================================================
#ifndef X_XMLCALIBRATIONARRAYIOHANDLER_H
#define X_XMLCALIBRATIONARRAYIOHANDLER_H
//==========================================================
#include <QObject>
#include <QApplication>
#include <QFile>
#include <QStack>
#include "X_Calibration.h"

//==========================================================
class QXmlStreamReader;
class QFileDialog;
//==========================================================
class X_XMLCalibrationIOHandler : public QObject
{
    Q_OBJECT
public:
    explicit X_XMLCalibrationIOHandler(/*QObject* calibrationParent,*/ QObject *parent = 0);
    virtual ~X_XMLCalibrationIOHandler();

    static QString xp_getCalibrationOpenFile(const QString& calibrationFolderPath);
    static QStringList xp_getCalibrationOpenFiles(const QString& calibrationFolderPath);
    static QString xp_getCalibrationSaveFile(const QString& calibrationFolderPath);
    static QString xp_checkDirPath(const QString& calibrationFolderPath);

    bool xp_getCalibrationFromFile(QFile&, X_Calibration*);
    bool xp_writeCalibrationToFile(QFile&file, const X_Calibration*calibration);

    QString xp_message() const;

public slots:


signals:

    void xg_message(const QString&);

private:

    // VARS
    // QObject* xv_calibrationParent;
    // QString parentTagName;
    QStack<QString> parentTagStack;
    X_RawTerm xv_rawTerm;
    X_RawWindow xv_rawWindow;
    QString xv_message;

    const QString xv_magicString = qApp->organizationName()  + " 7D385RTNOW9SH31JZQL";
    // elment names
    const QString xv_ROOT = "root";
    const QString xv_TYPE = "type";
    const QString xv_STATE = "state";

    const QString xv_DATE_TIME = "date-time";
//    const QString xv_ENERGY_K0 = "energy_K0";S
//    const QString xv_ENERGY_K1 = "energy_K1";
//    const QString xv_ENERGY_K2 = "energy_K2";
//    const QString xv_ENERGY_UNIT = "energy_unit";
    const QString xv_EXPOSITION = "exposition";
    const QString xv_GAIN_FACTOR = "gainFactor";

    const QString xv_CHEMELEMENT = "chem_element";
    const QString xv_DETERMINATION_R2 = "R2";
    const QString xv_ADJUSTED_DETERMINATION_R2ADJ = "R2adj";
    const QString xv_STANDARD_DEVIATION = "s";

    const QString xv_WINDOW_LIST = "window_list";
    const QString xv_WINDOW = "window";
    const QString xv_NAME = "name";
    const QString xv_FIRST_CHANNEL = "first_channel";
    const QString xv_LAST_CHANNEL = "last_channel";

    const QString xv_TERM_LIST = "term_list";
    const QString xv_TERM = "term";
    const QString xv_TERM_WINDOW = "term_window";
    const QString xv_FACTOR = "factor";

    const QString xv_EQUATION = "equation";
    const QString xv_POLYNOMIAL = "polynomial";
    const QString xv_FRACTIONAL = "fractional";
    const QString xv_BASE_TERM = "base_term";
    const QString xv_NORMALIX_ER = "normalizer";
    const QString xv_FRACTIONAL_BASE_NORMALIX_ER = "fractional_base_normalizer";

    const QString xv_CUSTOM_STRING = "custom_string";
    const QString xv_DESCRIPTION = "description";
    const QString xv_INTERCEPT = "intercept";

    const QString xv_YES = "yes";
    const QString xv_NO = "no";

    const QString xv_dateTimeFormat = "dd.MM.yyyy hh:mm:ss";

    // FUNCS
    bool xh_detectRoot(const QXmlStreamReader& reader, bool& magicStringDetectionFlag) const;
    void xh_parseXMLElement(X_Calibration*,
                            QXmlStreamReader&);
    bool xh_checkfilePath(const QString& path) const;


};
//==========================================================
#endif // X_XMLCALIBRATIONARRAYIOHANDLER_H
